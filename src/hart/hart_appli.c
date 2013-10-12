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

static float data_to_float(unsigned char *tmp)
{
	union {
		float tmp_f;
		unsigned char buf[4];
	}U;
	unsigned char i;
	
	for(i = 0;i < 4;i++)
	{
		U.buf[i] = *(tmp+i);
	}
	return U.tmp_f;
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
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_pv();
		data[HrtByteCnt++] = PV_UNIT;
		float_to_data(data,&tmp);
	}
}

void C2_RdLoopCurrPerOfRange(unsigned char *data)
{
	float tmp1,tmp2;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp1 = get_loop_current();
		tmp2 = get_percent_of_range();
		float_to_data(data,&tmp1);
		float_to_data(data,&tmp2);
	}
}

void C3_RdDVLoopCurr(unsigned char *data)
{
	float tmp1,tmp2,tmp3,tmp4,tmp5;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp1 = get_loop_current();
		tmp2 = get_pv();
		tmp3 = get_sv();
		tmp4 = get_tv();
		tmp5 = get_qv();
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
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		data[HrtByteCnt++] = *dat;
		data[HrtByteCnt++] = *(dat+1);
		set_polling_addr(*dat);
		set_loop_current_mode(*(dat+1));
	}
	
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
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_message();
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
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat1 = get_tag();
		dat2 = get_descriptor();
		dat3 = get_date();
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
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_transducer_serial_num();
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
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp1 = get_pv_upper_range();
		tmp2 = get_pv_lower_range();
		tmp3 = get_pv_damping_time();
		data[HrtByteCnt++] = get_alarm_sw();
		data[HrtByteCnt++] = get_transfer_func();
		data[HrtByteCnt++] = get_ul_range_unit();
		float_to_data(data,&tmp1);
		float_to_data(data,&tmp2);
		float_to_data(data,&tmp3);
		data[HrtByteCnt++] = get_protect();
		data[HrtByteCnt++] = 250;
		data[HrtByteCnt++] = get_analog_channel();
	}
}

void C16_RdFinalAssemblyNum(unsigned char *data)
{
	unsigned char *dat;
	unsigned char i;	
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_final_assembly_num();
		for(i = 0;i < 3;i++)
		{
			data[HrtByteCnt++] = *(dat+i);
		}
	}
}

void C17_WrMessage(unsigned char *data)
{
	unsigned char *dat;
	unsigned char i;	
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		for(i = 0;i < 24;i++)
		{
			data[HrtByteCnt++] = *(dat+i);
		}
		set_message(dat);
	}
	
}

void C18_WrTagDescriptorDate(unsigned char *data)
{
	unsigned char *dat;
	unsigned char i;	
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		for(i = 0;i < 21;i++)
		{
			data[HrtByteCnt++] = *(dat+i);
		}
		set_tag(dat);
		set_descriptor(dat+6);
		set_date(dat+18);
	}
	
}

void C19_WrFinalAssemblyNum(unsigned char *data)
{
	unsigned char *dat;
	unsigned char i;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		for(i = 0;i < 3;i++)
		{
			data[HrtByteCnt++] = *(dat+i);
		}
		set_final_assembly_num(dat);
	}
	
}

void C33_RdDeviceVariable(unsigned char *data)
{
	unsigned char *dat;
	unsigned char i;
	float tmp;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		for(i = 0;i < 4;i++)
		{
			switch(*(dat+i))
			{
				case PERCENT_RANGE:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = PERCENT_UNIT;
					tmp = get_percent_of_range();
					float_to_data(data,&tmp);
					break;
				case LOOP_CURRENT:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = CURRENT_UNIT;
					tmp = get_loop_current();
					float_to_data(data,&tmp);
					break;
				case PV_CODE:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = PV_UNIT;
					tmp = get_pv();
					float_to_data(data,&tmp);
					break;
				case SV_CODE:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = SV_UNIT;
					tmp = get_sv();
					float_to_data(data,&tmp);
					break;
				case TV_CODE:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = TV_UNIT;
					tmp = get_tv();
					float_to_data(data,&tmp);
					break;
				case QV_CODE:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = QV_UNIT;
					tmp = get_qv();
					float_to_data(data,&tmp);
					break;
				default:
					data[HrtByteCnt++] = *(dat+i);
					data[HrtByteCnt++] = 250;   //not used
					data[HrtByteCnt++] = 0x7F;
					data[HrtByteCnt++] = 0xA0;
					data[HrtByteCnt++] = 0x00;
					data[HrtByteCnt++] = 0x00;
					break;
			}
		}
	}
}

void C34_WrPVDamping(unsigned char *data)
{
	unsigned char *dat;
	float tmp;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		tmp = data_to_float(dat);
		float_to_data(data,&tmp);
		set_pv_damping_time(tmp);
	}
	
}

void C35_WrPVRange(unsigned char *data)
{
	unsigned char *dat;
	float tmp1,tmp2;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		tmp1 = data_to_float(dat+1);
		tmp2 = data_to_float(dat+5);
		data[HrtByteCnt++] = *dat;		
		float_to_data(data,&tmp1);	
		float_to_data(data,&tmp2);	
		set_ul_range_unit(*dat);
		set_pv_upper_range(tmp1);
		set_pv_lower_range(tmp2);
	}
	
}

void C36_SetPVUpperRange(unsigned char *data)
{
	float tmp;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_pv();
		set_pv_upper_range(tmp);
	}	
}

void C37_SetPVLowerRange(unsigned char *data)
{
	float tmp;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_pv();
		set_pv_lower_range(tmp);
	}
}

void C40_EnterOrExitFixedCurrent(unsigned char *data)
{
	float tmp;
	unsigned char *dat;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_loop_current();
		float_to_data(data,&tmp);
		dat = get_rx_data_pointer();
		tmp = data_to_float(dat);
		set_fixed_current(tmp);
	}
}

void C41_PerformSelfTest(unsigned char *data)
{
	PerformSelfTest func;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		func = (PerformSelfTest)get_perform_self_test_ptr();
		func();
	}
}

void C42_PerformDeviceReset(unsigned char *data)
{
	PerformDeviceReset func;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		func = (PerformDeviceReset)get_perform_device_reset_ptr();
		func();
	}
}

void C43_PVZero(unsigned char *data)
{
	float tmp;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_pv();
		set_pv_zero(tmp);
	}
}

void C44_WrPVUnit(unsigned char *data)
{
	unsigned char *dat;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		dat = get_rx_data_pointer();
		data[HrtByteCnt++] = *dat;
		set_pv_unit(*dat);
	}
}

void C45_TrimLoopCurrentZero(unsigned char *data)
{
	unsigned char *dat;
	float tmp;
	TrimLoopCurrent func;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_fixed_current();
		if((tmp>=3.0) && (tmp<=5.0))           //enter fixed current mode
		{
			dat = get_rx_data_pointer();
			tmp = data_to_float(dat);
			func = (TrimLoopCurrent)get_zero_trim_ptr();
			func(&tmp);
			tmp = get_act_zero_current();
			float_to_data(data,&tmp);	
		}
		else
		{
			tmp = 9.0f;
			float_to_data(data,&tmp);
		}
	}
}

void C46_TrimLoopCurrentGain(unsigned char *data)
{
	unsigned char *dat;
	float tmp;
	TrimLoopCurrent func;
	
	set_respose_code(data);
	if(!HrtResposeCode)
	{
		tmp = get_fixed_current();
		if((tmp>=19.0) && (tmp<=21.0))           //enter fixed current mode
		{
			dat = get_rx_data_pointer();
			tmp = data_to_float(dat);
			func = (TrimLoopCurrent)get_gain_trim_ptr();
			func(&tmp);
			tmp = get_act_gain_current();
			float_to_data(data,&tmp);	
		}
		else
		{
			tmp = 9.0f;
			float_to_data(data,&tmp);
		}
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







