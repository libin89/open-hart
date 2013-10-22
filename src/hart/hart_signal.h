#ifndef _HART_SIGNAL_H
#define _HART_SIGNAL_H

extern void enter_critical_section( void );
extern void exit_critical_section( void );
extern volatile unsigned int RcvFrameCnt; // receive frame count
extern volatile unsigned char FramePrcCompFlg; // frame process completed flag
extern volatile unsigned char TsmFrameCompFlg; // transmit frame completed flag

/* receive frame count */
extern void set_rcv_frame_count(void);
extern void get_rcv_frame_count(void);

/* frame process completed flag */
extern void set_frame_process_completed_flag(void);
extern void get_frame_process_completed_flag(void);

/* transmit frame completed flag */
extern void set_tsm_completed_flag(void);
extern void get_tsm_completed_flag(void);

#endif
