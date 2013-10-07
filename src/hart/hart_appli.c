#include "hart_appli.h"
#include "hart_frame.h"
#include "wuhuan.h"

unsigned int HrtByteCnt = 0;
unsigned char HrtResposeCode = 0;
unsigned char HrtDeviceStatus = 0;

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
	//?
}




