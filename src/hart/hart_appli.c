#include "hart_appli.h"
#include "hart_frame.h"
#include "wuhuan.h"
#include "hart_driver.h"

unsigned int HrtByteCnt = 0;
unsigned char HrtResposeCode = 0;
unsigned char HrtDeviceStatus = 0;
void (*command)(unsigned char *data);

void set_respose_code(unsigned char *data)
{
	data[HrtByteCnt++] = HrtResposeCode;
	data[HrtByteCnt++] = HrtDeviceStatus;
}

void set_ID(unsigned char *data)
{
	data[HrtByteCnt++] = 254;
	data[HrtByteCnt++] = MANUFACTURER_ID;
	data[HrtByteCnt++] = DEVICE_TYPE;
	data[HrtByteCnt++] = get_preamble_num();
	data[HrtByteCnt++] = 6; //hart revision
	data[HrtByteCnt++] = 0x00; //device revision level
	data[HrtByteCnt++] = 120;  //software revision
	data[HrtByteCnt++] = 0x02;
	data[HrtByteCnt++] = 0x01; //flag assignment
	data[HrtByteCnt++] = UNIQUE_DEVICE_ID0;
	data[HrtByteCnt++] = UNIQUE_DEVICE_ID1;
	data[HrtByteCnt++] = UNIQUE_DEVICE_ID2;
	//the 12th data byte?
}

static void float_to_data(unsigned char *data,float *tmp)
{
	data[HrtByteCnt++] = *((unsigned char *)tmp);
	data[HrtByteCnt++] = *((unsigned char *)tmp+1);
	data[HrtByteCnt++] = *((unsigned char *)tmp+2);
	data[HrtByteCnt++] = *((unsigned char *)tmp+3);
}

/* the same as C11 */
void C0_RdUniqueId(unsigned char *data) 
{
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		set_ID(data);
	}
}

void C1_RdPV(unsigned char *data)
{
	float tmp;
	
	tmp = get_pv();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		data[HrtByteCnt++] = PV_UNIT;
		float_to_data(data,&tmp);
	}
}

void C2_RdLoopCurrPerOfRange(unsigned char *data)
{
	float tmp1,tmp2;
	
	tmp1 = get_loop_current();
	tmp2 = get_percent_of_range();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		float_to_data(data,&tmp1);
		float_to_data(data,&tmp2);
	}
}

void C3_RdDVLoopCurr(unsigned char *data)
{
	float tmp1,tmp2,tmp3,tmp4,tmp5;
	
	tmp1 = get_loop_current();
	tmp2 = get_pv();
	tmp3 = get_sv();
	tmp4 = get_tv();
	tmp5 = get_qv();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		float_to_data(data,&tmp1);
		data[HrtByteCnt++] = PV_UNIT;
		float_to_data(data,&tmp2);
		data[HrtByteCnt++] = SV_UNIT;
		float_to_data(data,&tmp3);
		data[HrtByteCnt++] = TV_UNIT;
		float_to_data(data,&tmp4);
		data[HrtByteCnt++] = QV_UNIT;	
		float_to_data(data,&tmp5);
	}
}

void C6_WrPollingAddr(unsigned char *data)
{
	unsigned char *dat;
	
	dat = get_rx_data_pointer();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		data[HrtByteCnt++] = *dat;
		data[HrtByteCnt++] = *(dat+1);
	}
	set_polling_addr(*dat);
	set_loop_current_mode(*(dat+1));
}

void C7_RdLoopConfiguration(unsigned char *data)
{
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		data[HrtByteCnt++] = get_polling_addr();
		data[HrtByteCnt++] = get_loop_current_mode();
	}
}

void C8_RdDVClass(unsigned char *data)
{
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		data[HrtByteCnt++] = PV_CLASS;
		data[HrtByteCnt++] = SV_CLASS;
		data[HrtByteCnt++] = TV_CLASS;
		data[HrtByteCnt++] = QV_CLASS;
	}
}

/* message type : packed */
void C12_RdMessage(unsigned char *data) 
{
	unsigned char i = 0;
	unsigned char *dat;
	
	dat = get_message();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
			for(i = 0;i < 24;i++)
			{
				data[HrtByteCnt++] = *(dat+i);
			}
	}
}

void C13_RdTagDescriptorDate(unsigned char *data)
{
	unsigned char i = 0;
	unsigned char *dat1, *dat2, *dat3;
	
	dat1 = get_tag();
	dat2 = get_descriptor();
	dat3 = get_date();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		for(i = 0;i < 6;i++)
		{
			data[HrtByteCnt++] = *(dat1+i);
		}
		for(i = 0;i < 12;i++)
		{
			data[HrtByteCnt++] = *(dat2+i);
		}
		for(i = 0;i < 3;i++)
		{
			data[HrtByteCnt++] = *(dat3+i);
		}
	}
}

void C14_RdPVTransducerInfo(unsigned char *data)
{
	unsigned char i = 0;
	unsigned char *dat;
	
	dat = get_transducer_serial_num();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		for(i = 0;i < 3;i++)
		{
			data[HrtByteCnt++] = *(dat+i);
		}
		data[HrtByteCnt++] = PV_UNIT;
		data[HrtByteCnt++] = get_transducer_upper();
		data[HrtByteCnt++] = get_transducer_lower();
		data[HrtByteCnt++] = get_pv_min_span();
	}
}

void C15_RdDeviceInfo(unsigned char *data)
{
	float tmp1,tmp2,tmp3;
	
	tmp1 = get_pv_upper_range();
	tmp2 = get_pv_lower_range();
	tmp3 = get_pv_damping_time();
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		data[HrtByteCnt++] = get_alarm_sw();
		data[HrtByteCnt++] = get_transfer_func();
		data[HrtByteCnt++] = PV_UNIT;
		float_to_data(data,&tmp1);
		float_to_data(data,&tmp2);
		float_to_data(data,&tmp3);
		data[HrtByteCnt++] = get_protect();
		data[HrtByteCnt++] = 250;
		data[HrtByteCnt++] = get_analog_channel();
	}
}

unsigned int cmd_function(unsigned char cmd,unsigned char *data)
{
	switch(cmd)
	{
		case 0: command = C0_RdUniqueId;		break;
		case 1:	command = C1_RdPV;	  break;
		case 2:			break;
		case 3:			break;
		case 4:			break;
		case 5:			break;
		case 6:			break;
		case 7:			break;
		case 8:			break;
		case 9:			break;
		case 10:		break;
		case 11:		break;
		case 12:		break;
		case 13:		break;
		case 14:		break;
		case 15:		break;
		case 16:		break;
		case 17:		break;
		case 18:		break;
		case 19:		break;
		case 20:		break;
		case 21:		break;
		case 22:		break;
		case 23:    break;
		case 24:		break;
		case 25:		break;
		case 26:		break;
		case 27:		break;
		case 28:		break;
		case 29:    break;
		case 30:    break;
		case 31:    break;
		case 32:    break;
		case 33:    break;
		default:
			break;
	}
	command(data);
	return HrtByteCnt;
}

void hart_appli_init(void)
{
	//serical init
	serical_init(1200,8,HT_SERICAL_EVEN,1);
	//set default tx_preamble_num,tx_address_size
	set_burst_mode(TRUE);
	set_tx_addr_size(LONG_ADDR_SIZE);
	set_preamble_num(PREAMBLE_DEFAULT_NUM);
}

void hart_appli_poll(void)
{
	frame_cmd_data(cmd_function);
}







