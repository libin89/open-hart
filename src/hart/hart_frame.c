#include "hart_frame.h"
#include "hart_driver.h"
#include "soft_timer.h"
#include "wuhuan.h"
#include "hart_signal.h"

typedef struct 
{
	unsigned char preamble_num;
	unsigned char delimiter;
	unsigned char address_size;
	unsigned char cmd;
	unsigned char byte_count;
	unsigned char data_buf[50];
	unsigned char check_byte;
}frame_type;

typedef struct 
{
	unsigned char manufacturer_id;
	unsigned char device_type;
	unsigned char unique_device_id[3];
}long_addr_type;

//unsigned char polling_addr = 0; //short address
long_addr_type long_addr = {
	MANUFACTURER_ID,DEVICE_TYPE,UNIQUE_DEVICE_ID0,UNIQUE_DEVICE_ID1,UNIQUE_DEVICE_ID2,
};
frame_type g_Rx, g_Tx;
//unsigned char hrt_respose_code;

#if 0
/* address */
unsigned char PollingAddr = 0;
unsigned char ManufacturerId = 0x1F;
unsigned char DeviceType = 0x15;
unsigned char UniqueDeviceId[3] = {0x17,0x3E,0xE1};
#endif

#if 0
unsigned char g_RxDataBuf[50];
unsigned char g_TxDataBuf[50];
unsigned char g_FrameSize;
unsigned char g_RxCmdPos;
unsigned int g_TxDataCount;
unsigned char g_PreambleNum = PREAMBLE_DEFAULT_NUM;
unsigned char g_TxFrameType = LONG_ADDR_SIZE;
#endif

// unsigned char BurstMode = FALSE;
unsigned char g_Burst = FALSE;
unsigned char g_Bt = 0;
unsigned char g_Host = PRIMARY_MASTER;
rcv_msg_type g_RcvMsgType;
xmt_msg_type g_XmtMsgType;

//unsigned char g_PreambleNum = 0;
// unsigned char g_AppliCompletedNotify = 0;

hrt_state g_HartState = HRT_WAIT;
rsm_state g_RcvState = RCV_WAIT_IDLE;
tsm_state g_XmtState = XMT_INIT;

static volatile unsigned char *pXmtBufferCur;
static volatile unsigned int s_XmtBufferCnt;
static volatile unsigned char s_XmtPreambleNum;
static volatile unsigned int s_RcvBufferPos;
// static unsigned char s_XmtBufferPos;




static unsigned char longitudinal_parity(unsigned char *data, unsigned int cnt);
static void hart_wait(void);
static unsigned char is_addr_match(void);
static void hart_process(void);


static unsigned char longitudinal_parity(unsigned char *data, unsigned int cnt)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
{
	unsigned char i;
	unsigned char check_byte = 0x00;
	
	for(i = 0; i < cnt; i++)
	{
		check_byte ^= *(data+i);
	}
	return check_byte;
}

extern void hart_poll(void)
{
	if(RcvFrameCnt)
	{
		get_rcv_frame_count();
		if(g_HartState == HRT_WAIT)
		{
			hart_wait( );
		}
		if(g_HartState == HRT_PROCESS)
		{
			hart_process( );
			s_XmtPreambleNum = get_response_preamble_num();
			s_XmtBufferCnt = g_Tx.address_size + 3 + g_Tx.byte_count + 1;
			pXmtBufferCur = g_Tx.data_buf;
			serical_enable(FALSE,TRUE);
		}
	}
}

/* func : implementation on the application layer */
// extern void set_burst_mode( unsigned char burst_mode )
// {
// 	if(burst_mode)
// 	{
// 		BurstMode = TRUE;
// 	}
// 	else
// 	{
// 		BurstMode = FALSE;
// 	}
// }

/* get a pointer to rx_data_buf */
extern unsigned char *get_rx_data_pointer(void)
{
	unsigned char *data;
	
	if(g_Rx.address_size == LONG_ADDR_SIZE)
	{
		data = &g_Rx.data_buf[HRT_LONGF_REQDATA_OFF];
	}
	else
	{
		data = &g_Rx.data_buf[HRT_SHORTF_REQDATA_OFF];
	}
	return data;
}

/* setup para */
// extern unsigned char get_preamble_num(void)
// {
// 	return g_Tx.preamble_num;
// }
// extern void set_preamble_num(unsigned char preamble_num)
// {
// 	g_Tx.preamble_num = preamble_num;
// }
// extern void set_tx_addr_size(unsigned char addr_size)
// {
// 	g_Tx.address_size = addr_size;
// }
extern unsigned char get_xmt_msg_type(void)
{
	return g_XmtMsgType;
}
// extern void set_tx_byte_count(unsigned int byte_count)
// {
// 	g_Tx.byte_count = byte_count;
// }

extern unsigned char get_hart_state(void)
{
	return g_HartState;
}
// extern unsigned char get_error_code(void)
// {
// 	return hrt_respose_code;
// }

extern void set_data_link(void)
{
	unsigned char polling_addr;
	
	polling_addr = get_polling_addr();
	if(g_Rx.address_size == LONG_ADDR_SIZE)
	{
		g_Tx.address_size = LONG_ADDR_SIZE;
	}
	else
	{
		g_Tx.address_size = SHORT_ADDR_SIZE;
	}
	
	if(g_Burst)               //burst mode command-byte is ?
	{
		g_Tx.delimiter = 0x01;
	}
	else
	{
		g_Tx.delimiter = 0x06;
	}
	if(g_Tx.address_size == LONG_ADDR_SIZE)
	{
		g_Tx.data_buf[0] = g_Tx.delimiter | LONG_FRAME;
		g_Tx.data_buf[1] = (long_addr.manufacturer_id & 0x3F) | (g_Host<<7);
		g_Tx.data_buf[2] = long_addr.device_type;
		g_Tx.data_buf[3] = long_addr.unique_device_id[0];
		g_Tx.data_buf[4] = long_addr.unique_device_id[1];
		g_Tx.data_buf[5] = long_addr.unique_device_id[2];
	}
	else
	{
		g_Tx.data_buf[0] = g_Tx.delimiter | SHORT_FRAME;
		g_Tx.data_buf[1] = polling_addr | (g_Host<<7);
	}
	
}

/* func : implementation on the application layer 
	 
*/
extern void frame_cmd_data(void)
{
	unsigned char cmd;
	unsigned char *data;
	
	if(g_Rx.address_size == LONG_ADDR_SIZE)
	{
		cmd = g_Rx.data_buf[HRT_LONGF_CMD_OFF];
		data = &g_Tx.data_buf[HRT_LONGF_RSPCODE1_OFF];
		
		g_Tx.data_buf[HRT_LONGF_CMD_OFF] = cmd;
		g_Tx.byte_count = cmd_function(cmd,data);
		g_Tx.data_buf[HRT_LONGF_LEN_OFF] = g_Tx.byte_count;
	}
	else
	{
		cmd = g_Rx.data_buf[HRT_SHORTF_CMD_OFF];
		data = &g_Tx.data_buf[HRT_SHORTF_RSPCODE1_OFF];
		
		g_Tx.data_buf[HRT_SHORTF_CMD_OFF] = cmd;
		g_Tx.byte_count = cmd_function(cmd,data);
		g_Tx.data_buf[HRT_SHORTF_LEN_OFF] = g_Tx.byte_count;
	}
}

static unsigned char is_addr_match(void)
{
	unsigned char polling_addr;
	
	polling_addr = get_polling_addr();
	if(g_Rx.address_size == SHORT_ADDR_SIZE)
	{
		if( polling_addr == (g_Rx.data_buf[HRT_SHORTF_ADDR_OFF]&0x3F) )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if((!(g_Rx.data_buf[HRT_LONGF_ADDR_OFF]&0x7f)) && (!g_Rx.data_buf[HRT_LONGF_ADDR_OFF+1]) && \
				(!g_Rx.data_buf[HRT_LONGF_ADDR_OFF+2]) && (!g_Rx.data_buf[HRT_LONGF_ADDR_OFF+2]) && \
				(!g_Rx.data_buf[HRT_LONGF_ADDR_OFF+2]))
		{
			return TRUE;
		}
		else
		{
			if( (long_addr.manufacturer_id == (g_Rx.data_buf[HRT_LONGF_ADDR_OFF]&0x3F)) && \
				(long_addr.device_type == g_Rx.data_buf[HRT_LONGF_ADDR_OFF+1]) && \
				(long_addr.unique_device_id[0] == g_Rx.data_buf[HRT_LONGF_ADDR_OFF+2]) && \
				(long_addr.unique_device_id[1] == g_Rx.data_buf[HRT_LONGF_ADDR_OFF+3]) && \
				(long_addr.unique_device_id[2] == g_Rx.data_buf[HRT_LONGF_ADDR_OFF+4]) )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
}

static void hart_wait(void)
{
	rcv_msg_type rcv_msg_t;
	unsigned char BurstMode;
	
	rcv_msg_t = g_RcvMsgType;
	BurstMode = get_burst_mode_code();
	if(BurstMode)
	{
		g_Burst = TRUE;
		set_delay_time(BT_TIMER,0); //bt = 0
		g_Host = PRIMARY_MASTER;
	}
	else
	{
		g_Burst = FALSE;
	}
	
	if( g_Burst && (is_timeout_id(BT_TIMER)) )
	{
		g_XmtMsgType = XMT_BACK;
		g_Host = (~g_Host)&0x01;
		set_delay_time(BT_TIMER,HRT_RT2);
	}
	else
	{
		switch(rcv_msg_t)
		{
			case RCV_ERR:
				break;
			case RCV_ACK:
				set_delay_time(BT_TIMER,0);
				set_delay_time(SLAVE_TIMER,HRT_STO);
				break;
			case RCV_STX:
				if(is_addr_match())
				{
					set_delay_time(BT_TIMER,0);
					set_delay_time(SLAVE_TIMER,HRT_STO);
					g_HartState = HRT_PROCESS;              //
				}
				else
				{
					set_delay_time(BT_TIMER,HRT_PRI_RT1);
				}
				break;
				default:
					break;
		}
	}
}

void hart_rcv_msg(void)
{
	static unsigned char PreambleNum = 0;
	static unsigned int ByteCount;
	unsigned char Byte;
		
	serical_get_byte(&Byte);
	switch(g_RcvState)
	{
		case RCV_WAIT_IDLE:
			if(Byte == PREAMBLE)
			{
				PreambleNum = 1;
				g_RcvState = RCV_WAIT_START;
				set_delay_time(GAP_TIMER,HRT_GAPT);
			}
			break;
		case RCV_WAIT_START:
			if(is_timeout_id(GAP_TIMER))
			{
				g_RcvMsgType = RCV_ERR;
			}
			else
			{
				if(Byte == PREAMBLE)
				{
					PreambleNum++;
				}
				else
				{
					switch(Byte)
					{
						case (SHORT_FRAME|RCV_STX):
							if(PreambleNum > 1)
							{
								g_Rx.address_size = SHORT_ADDR_SIZE;
								g_RcvMsgType = RCV_STX;
								s_RcvBufferPos = 0;
								g_Rx.data_buf[s_RcvBufferPos++] = Byte;
								g_RcvState = RCV_READ;
							}		
							else
							{
								g_RcvMsgType = RCV_ERR;
							}
							break;
						case (SHORT_FRAME|RCV_ACK):
							if(PreambleNum > 1)
							{
								g_Rx.address_size = SHORT_ADDR_SIZE;
								g_RcvMsgType = RCV_ACK;
								s_RcvBufferPos = 0;
								g_Rx.data_buf[s_RcvBufferPos++] = Byte;
								g_RcvState = RCV_READ;
							}
							else
							{
								g_RcvMsgType = RCV_ERR;
							}
							break;
						case (SHORT_FRAME|RCV_BACK):
							if(PreambleNum > 1)
							{
								g_Rx.address_size = SHORT_ADDR_SIZE;
								g_RcvMsgType = RCV_BACK;
								s_RcvBufferPos = 0;
								g_Rx.data_buf[s_RcvBufferPos++] = Byte;
								g_RcvState = RCV_READ;
							}
							else
							{
								g_RcvMsgType = RCV_ERR;
							}
							break;
						case (LONG_FRAME|RCV_STX):
							if(PreambleNum > 1)
							{
								g_Rx.address_size = LONG_ADDR_SIZE;
								g_RcvMsgType = RCV_STX;
								s_RcvBufferPos = 0;
								g_Rx.data_buf[s_RcvBufferPos++] = Byte;
								g_RcvState = RCV_READ;
							}
							else
							{
								g_RcvMsgType = RCV_ERR;
							}
							break;
						case (LONG_FRAME|RCV_ACK):
							if(PreambleNum > 1)
							{
								g_Rx.address_size = LONG_ADDR_SIZE;
								g_RcvMsgType = RCV_ACK;
								s_RcvBufferPos = 0;
								g_Rx.data_buf[s_RcvBufferPos++] = Byte;
								g_RcvState = RCV_READ;
							}
							else
							{
								g_RcvMsgType = RCV_ERR;
							}
							break;
						case (LONG_FRAME|RCV_BACK):
							if(PreambleNum > 1)
							{
								g_Rx.address_size = LONG_ADDR_SIZE;
								g_RcvMsgType = RCV_BACK;
								s_RcvBufferPos = 0;
								g_Rx.data_buf[s_RcvBufferPos++] = Byte;
								g_RcvState = RCV_READ;
							}
							else
							{
								g_RcvMsgType = RCV_ERR;
							}
							break;
						default:
							g_RcvMsgType = RCV_ERR;
							g_RcvState = RCV_DONE;
							break;
					}
				}
			}	
			set_delay_time(GAP_TIMER,HRT_GAPT);
			break;
		case RCV_READ:
			if(is_timeout_id(GAP_TIMER))
			{
				g_RcvMsgType = RCV_ERR;
			}
			else
			{
				g_Rx.data_buf[s_RcvBufferPos++] = Byte;
				if(g_Rx.address_size == SHORT_ADDR_SIZE)
				{	
					if(s_RcvBufferPos == HRT_SHORTF_LEN_OFF+1)
					{
						ByteCount = Byte;
					}
					if(s_RcvBufferPos > HRT_SHORTF_LEN_OFF+ByteCount+1)
					{
						g_RcvState = RCV_DONE;
					}
				}
				else
				{
					if(s_RcvBufferPos == HRT_LONGF_LEN_OFF+1)
					{
						ByteCount = Byte;
					}
					if(s_RcvBufferPos > HRT_LONGF_LEN_OFF+ByteCount+1)
					{
						g_RcvState = RCV_DONE;
					}
				}	
			}
			set_delay_time(GAP_TIMER,HRT_GAPT);
			if(g_RcvState == RCV_DONE)
			{
				is_timeout_id(GAP_TIMER);
				g_HartState = HRT_WAIT;
				g_RcvState = RCV_WAIT_IDLE;
				PreambleNum = 0;
				ByteCount = 0;
				set_rcv_frame_count();
				serical_enable(FALSE,FALSE);
			}
			break;
		default:
			break;
	}
}

void hart_xmt_msg(void)
{
	//tsm_state XmtState = XMT_INIT;
// 	unsigned char i = 0;
// 	unsigned int cnt;
	
	if(g_Tx.byte_count >= 2)
	{
		switch(g_XmtState)
		{
			case XMT_INIT:
				g_XmtState = XMT_WRITE;
			case XMT_WRITE:
				if(s_XmtPreambleNum != 0)
				{
					serical_put_byte(PREAMBLE);
					s_XmtPreambleNum--;
				}
				else
				{
					if(s_XmtBufferCnt != 0)
					{
						serical_put_byte(*pXmtBufferCur);
						pXmtBufferCur++;
						s_XmtBufferCnt--;
					}
					else
					{
						g_XmtState = XMT_DONE;
					}
				}
				if(g_XmtState == XMT_DONE)
				{
					serical_enable(TRUE,FALSE);
					g_XmtState = XMT_INIT;
				}
				break;
			default :
				break;
		}
	}	
}

// extern void hart_appli_completed_notify(unsigned char flg)
// {
// 	g_AppliCompletedNotify = flg;
// }

static void hart_process(void)
{
	unsigned int cnt;
	unsigned char is_burst_mode;
	
	is_burst_mode = get_burst_mode_code();
	cnt = g_Rx.address_size + 3 + g_Rx.data_buf[g_Rx.address_size+2];
	if(is_timeout_id(SLAVE_TIMER))      //slave time out 
	{
		g_HartState = HRT_WAIT;
	}
	else
	{
		if(longitudinal_parity(g_Rx.data_buf,cnt) == g_Rx.data_buf[cnt]) //no comm error
		{
			g_XmtMsgType = XMT_ACK;
		}
		else   //comm error
		{
			g_XmtMsgType = XMT_COMM_ERR;
			//hrt_respose_code = HRT_LONGITUDINAL_PARITY_ERROR;
		}
	}
	frame_cmd_data();
	//hart_appli_completed_notify(TRUE);
	cnt = g_Tx.address_size + 3 + g_Tx.byte_count;
	g_Tx.data_buf[cnt] = longitudinal_parity(g_Tx.data_buf,cnt);
	if(is_burst_mode)
	{
		g_HartState = HRT_WAIT;
		//serical_enable(FALSE,TRUE);
	}
	else
	{
		g_HartState = HRT_WAIT;
		//serical_enable(FALSE,TRUE);
	}		
}


