#ifndef _WUHUAN_H
#define _WUHUAN_H

// const unsigned char manufacturer_id;
// const unsigned char device_type;
// const unsigned char unique_device_id[3];

#define MANUFACTURER_ID  0x3A
#define DEVICE_TYPE      0x15
#define UNIQUE_DEVICE_ID0 0x17
#define UNIQUE_DEVICE_ID1 0x3E
#define UNIQUE_DEVICE_ID2 0xE1

enum {
	PERCENT_UNIT = 57, // %
	CURRENT_UNIT = 39, // mA
	PV_CLASS,  //table 21
	SV_CLASS,
	TV_CLASS,
	QV_CLASS,
	/* DV code */
	BATTERY_LIFE = 243,  //table 34
	PERCENT_RANGE = 244,
	LOOP_CURRENT,
	PV_CODE,
	SV_CODE,
	TV_CODE,
	QV_CODE,
};

enum pv_unit{
	G_S = 22,
	G_M = 16,
	G_H = 136,
	M3_S = 28,
	M3_M = 131,
	M3_H = 19,
	L_S = 24,
	L_M = 17,
	L_H = 138,
};

enum alarm{
	HIGH_ALARM = 0,
	LOW_ALARM = 1,
	NONE_ALARM = 251,
};

enum transfer_func{
	LINEAR = 0,
	SQUARE_ROOT,
	NONE_FUNC = 251,
};

enum protect{
	YES_PROTECT,
	NO_PROTECT,
};

enum burst_mode{
	BURST_OFF,
	BURST_ON,
};

enum analog_channel{
	CHANNEL_FLAG = 0x01,
};

typedef void (*PerformSelfTest)(void);
typedef void (*PerformDeviceReset)(void);
typedef void (*TrimLoopCurrent)(void *data);

/* DV */
extern void  set_pv(float pv);
extern float get_pv(void);
extern void  set_sv(float sv); 
extern float get_sv(void);  
extern void  set_tv(float tv);
extern float get_tv(void);    
extern void  set_qv(float qv); 
extern float get_qv(void);   
/* loop current and percent of range */
extern void  set_loop_current(float current);
extern float get_loop_current(void);
extern void  set_percent_of_range(float percent_of_range);
extern float get_percent_of_range(void);
/* fixed current mode */
extern void set_fixed_current(float fixed_current);
extern float get_fixed_current(void);
/* upper/lower range unit */
extern void set_ul_range_unit(unsigned char ul_range_unit);
extern unsigned char get_ul_range_unit(void);
/* polling address and loop current mode */
extern void set_polling_addr(unsigned char polling_addr);
extern unsigned char get_polling_addr(void); 
extern void set_loop_current_mode(unsigned char current_mode); 
extern unsigned char get_loop_current_mode(void); 
/* message */
extern void set_message(unsigned char *msg);
extern unsigned char *get_message(void);
/* tag and descriptor and date */
extern void set_tag(unsigned char *tag);
extern unsigned char *get_tag(void);
extern void set_descriptor(unsigned char *dscp);
extern unsigned char *get_descriptor(void);
extern void set_date(unsigned char *date);
extern unsigned char *get_date(void);
/* PV transducer information */
extern void set_transducer_serial_num(unsigned char *tsn);
extern unsigned char *get_transducer_serial_num(void);
extern void set_transducer_upper(float tsd_upper); 
extern float get_transducer_upper(void);
extern void set_transducer_lower(float tsd_lower);
extern float get_transducer_lower(void);
extern void set_pv_min_span(float min_span);
extern float get_pv_min_span(void);
/* PV information or device information */
extern void set_pv_upper_range(float upper_range);
extern float get_pv_upper_range(void);
extern void set_pv_lower_range(float lower_range);
extern float get_pv_lower_range(void);
extern void set_pv_damping_time(float damping_time);
extern float get_pv_damping_time(void);
extern void set_alarm_sw(enum alarm alarm);
extern enum alarm get_alarm_sw(void);
extern void set_transfer_func(enum transfer_func tsf_func);
extern enum transfer_func get_transfer_func(void);
extern void set_protect(enum protect protect_status); 
extern enum protect get_protect(void);
extern void set_analog_channel(enum analog_channel analog_chl);
extern enum analog_channel get_analog_channel(void);
/* final assembly number */
extern void set_final_assembly_num(unsigned char *fan);
extern unsigned char *get_final_assembly_num(void);
/* perform self test */
extern void set_perform_self_test_ptr(void *func);
extern void *get_perform_self_test_ptr(void);
/* perform device reset */
extern void set_perform_device_reset_ptr(void *func);
extern void *get_perform_device_reset_ptr(void);
/* PV zero */
extern void set_pv_zero(float pv_zero);
extern float get_pv_zero(void);
/* DV unit */
void set_pv_unit(unsigned char pv_unit);
unsigned char get_pv_unit(void);
void set_sv_unit(unsigned char sv_unit);
unsigned char get_sv_unit(void);
void set_tv_unit(unsigned char tv_unit);
unsigned char get_tv_unit(void);
void set_qv_unit(unsigned char qv_unit);
unsigned char get_qv_unit(void);
/* DV code */
void set_pv_code(unsigned char pv_code);
unsigned char get_pv_code(void);
void set_sv_code(unsigned char sv_code);
unsigned char get_sv_code(void);
void set_tv_code(unsigned char tv_code);
unsigned char get_tv_code(void);
void set_qv_code(unsigned char qv_code);
unsigned char get_qv_code(void);
// /* externally trim current zero */
// extern void set_ext_zero_current(float ext_zero_curr);
// extern float get_ext_zero_current(void);
/* actual trim current zero */
extern void set_act_zero_current(float act_zero_curr);
extern float get_act_zero_current(void);
// /* externally trim current gain */
// extern void set_ext_gain_current(float ext_gain_current);
// extern float get_ext_gain_current(void);
/* actual trim current gain */
extern void set_act_gain_current(float act_gain_current);
extern float get_act_gain_current(void);
/* loop current trim function pointer */
extern void set_zero_trim_ptr(void *func);
extern void *get_zero_trim_ptr(void);
extern void set_gain_trim_ptr(void *func);
extern void *get_gain_trim_ptr(void);
/* numblers of response preambles */
extern void set_response_preamble_num(unsigned char rsp_preamble_num);
extern unsigned char get_response_preamble_num(void);
/* burst mode command number */
extern void set_burst_mode_cmd_num(unsigned char bt_cmd);
extern unsigned char get_burst_mode_cmd_num(void);
/* burst mode control code */
extern void set_burst_mode_code(unsigned char bt_code);
extern unsigned char get_burst_mode_code(void);

extern unsigned char packed_ascii(unsigned char* Src,unsigned int SrcLen,unsigned char* Dst,unsigned int DstLen);
extern unsigned char unpacked_ascii(unsigned char* Src,unsigned int SrcLen,unsigned char *Dst,unsigned int DstLen);

#endif
