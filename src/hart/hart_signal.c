#include "hart_signal.h"

volatile unsigned int RcvFrameCnt = 0; // receive frame count
volatile unsigned char FramePrcCompFlg = 0; // frame process completed flag
volatile unsigned char TsmFrameCompFlg = 0; // transmit frame completed flag

/* receive frame count */
void set_rcv_frame_count(void)
{
	enter_critical_section();
	RcvFrameCnt++;
	exit_critical_section();
}
void get_rcv_frame_count(void)
{
	enter_critical_section();
	RcvFrameCnt--;
	exit_critical_section();
}

/* frame process completed flag */
void set_frame_process_completed_flag(void)
{
	enter_critical_section();
	FramePrcCompFlg++;
	exit_critical_section();
}
void get_frame_process_completed_flag(void)
{
	enter_critical_section();
	FramePrcCompFlg--;
	exit_critical_section();
}

/* transmit frame completed flag */
void set_tsm_completed_flag(void)
{
	enter_critical_section();
	TsmFrameCompFlg++;
	exit_critical_section();
}
void get_tsm_completed_flag(void)
{
	enter_critical_section();
	TsmFrameCompFlg++;
	exit_critical_section();
}

