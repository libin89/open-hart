#ifndef _HART_FRAME_H
#define _HART_FRAME_H


// #define TRUE  1
// #define FALSE 0


#define GAP_TIMER   0    
#define SLAVE_TIMER 1
#define BT_TIMER    2

#define HRT_STO       257 /* unit:ms, sto = 256.7ms */
#define HRT_PRI_RT1   303 /* unit:ms,  = 302.5ms */
#define HRT_SEC_RT1   376 /* unit:ms,  = 375.8ms */
#define HRT_RT2       74  /* unit:ms,  = 73.3ms */
#define HRT_GAPT      9  /* unit:ms,  = 9.2ms */

#define PREAMBLE             0xFF
#define PREAMBLE_DEFAULT_NUM 5
#define PREAMBLE_MAX_NUM     20

/* short frame information offset address */
#define HRT_SHORTF_DELIMITER_OFF  0
#define HRT_SHORTF_ADDR_OFF       1
#define HRT_SHORTF_CMD_OFF        2
#define HRT_SHORTF_LEN_OFF        3
#define HRT_SHORTF_REQDATA_OFF    4
#define HRT_SHORTF_RSPCODE1_OFF   4
#define HRT_SHORTF_RSPCODE2_OFF   5
#define HRT_SHORTF_RSPDATA_OFF    6

/* long frame information offset address */
#define HRT_LONGF_DELIMITER_OFF   0
#define HRT_LONGF_ADDR_OFF        1
#define HRT_LONGF_CMD_OFF         6
#define HRT_LONGF_LEN_OFF         7
#define HRT_LONGF_REQDATA_OFF     8
#define HRT_LONGF_RSPCODE1_OFF    8
#define HRT_LONGF_RSPCODE2_OFF    9
#define HRT_LONGF_RSPDATA_OFF     10

/* host address */
typedef enum host
{
	SECONDARY_MASTER,
	PRIMARY_MASETER,
}host_addr;

/* slave/burst-mode  master */
#define PRIMARY_MASTER    1
#define SECONDARY_MASTER  0
#define BURST_MODE        1
#define SALVE_MODE        0

/* polling address(0-15): 16-63 should not assigned when muti-droping(i.e...HART versions 3-5) , 
	broadcast address(long frame):a 5 byte address whith 38 bits of 
	zeros in the place of Unique Identifier */
#define HRT_BROADCAST_ADDR 0x00

/* the most significant bit of respose code is set to one when communication error, 
	the second byte is meaningless! */
/* communication error flag */
#define HRT_VERTICAL_PARITY_ERROR     0xC0
#define HRT_OVERRUN_ERROR             0xA0
#define HRT_FRAMING_ERROR             0x90
#define HRT_LONGITUDINAL_PARITY_ERROR 0x88
#define HRT_BUFFER_OVERFLOW           0x82

/* the most significant bit of respose code is set to zero when no communication error */
/* respose code */
#define HRT_RSPCODE_ERROR        0x40 /* the second byte indicates the reason */
#define HRT_RSPCODE_WARNING      0x20 /* return the respose data bytes */
#define HRT_RSPCODE_NOTIFICATION 0x00 /* return the respose data bytes */

/* field device status in the second byte of respose code */
#define HRT_DEVICE_MALFUNCTION     0x80
#define HRT_CONFIGURETION_CHANGED  0x40
#define HRT_COLD_START             0x20
#define HRT_MORE_STATUS_AVAILABLE  0x10
#define HRT_LOOP_CURRENT_FIXED     0x08
#define HRT_LOOP_CURRENT_SATURATED 0x04
#define HRT_NON_PV_OUT_OF_LIMITS   0x02
#define HRT_PV_OUT_OF_LIMITS       0x01


#define LONG_ADDR_SIZE  5
#define SHORT_ADDR_SIZE 1

#define LONG_FRAME     0x80
#define SHORT_FRAME    0x00


/* STX : a master to slave message */
/* ACK : a slave to master message */
/* BACK : a slave message transmited to a master without an STX*/
/* the type of message received */
typedef enum
{
	RCV_ERR  = 0xff,
	RCV_COMM_ERR = 0xee,
	RCV_STX  = 0x02,
	RCV_ACK  = 0x06,
	RCV_BACK = 0x01,
}rcv_msg_type;

/* the type of message sended */
typedef enum
{
	XMT_ERR  = 0xff,
	XMT_COMM_ERR = 0xee,
	XMT_STX  = 0x02,                       
	XMT_ACK  = 0x06,                       
	XMT_BACK = 0x01,                      
}xmt_msg_type;

#if 0
/* the status of message received */
typedef enum
{
	RCV_ERR,
	RCV_VALID,
}rcv_msg_status;

/* the status of message sended */
typedef enum
{
	XMT_ERR,
	XMT_OK,
}xmt_msg_status;
#endif

/* receive state machine */
typedef enum
{
	RCV_WAIT_IDLE,
	RCV_WAIT_START,
	RCV_READ,
	RCV_DONE,
}rsm_state;

/* transmit state machine */
typedef enum
{
	XMT_INIT,
	XMT_WRITE,
	XMT_DONE,
}tsm_state;

/* slave/burst-mode device state machine */
typedef enum
{
	HRT_WAIT,
	HRT_RCV,
	HRT_XMT,
	HRT_PROCESS,
}hrt_state;

extern void hart_rcv_msg(void);
extern void hart_poll(void);
extern void set_burst_mode(unsigned char burst_mode);
extern void set_tx_addr_size(unsigned char addr_size);
extern unsigned char get_xmt_msg_type(void);
// extern unsigned char get_error_code(void);


extern void set_data_link(void);
extern void frame_cmd_data(unsigned char cmd,unsigned char *data, unsigned int cnt, \
										void (*func)(unsigned char cmd,unsigned char *data));



#endif
