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
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		
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







