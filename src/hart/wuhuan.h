#ifndef _WUHUAN_H
#define _WUHUAN_H

// const unsigned char manufacturer_id;
// const unsigned char device_type;
// const unsigned char unique_device_id[3];

#define MANUFACTURER_ID  0xFA
#define DEVICE_TYPE      0x15
#define UNIQUE_DEVICE_ID0 0x17
#define UNIQUE_DEVICE_ID1 0x3E
#define UNIQUE_DEVICE_ID2 0xE1

enum {
	PV_UNIT,
	SV_UNIT,
	TV_UNIT,
	QV_UNIT,
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

enum analog_channel{
	CHANNEL_FLAG = 0x01,
};

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

#endif
