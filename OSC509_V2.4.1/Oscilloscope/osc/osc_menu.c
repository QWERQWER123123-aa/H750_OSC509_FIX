/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "fos.h"
#include "osc.h"
#include "hal_tim.h"
#include "osc_ui.h"
#include "osc_menu.h"
#include "osc_api.h"
#include "string.h"
#include "hal_exit.h"
#include "osc_cfg.h"
#include "hal_usart.h"
#include "osc_calibrate.h"
#include "osc_param.h"
#include "usbh_app.h"
#include "osc_win.h"
#include "math.h"

/* Private extern */
extern const osc_time_def osc_tim[];
extern int osc_msg_box(unsigned char index);
extern void osc_msg_close(void);
extern void osc_refluse_dcac_chn(void);
extern void osc_exit_edge_reg(unsigned char mo);
extern unsigned char osc_trig_source(void);
extern draw_area_def * get_draw_area_msg(void);
/* Private helper functions for auto-set */
static float osc_get_timebase_ns(unsigned int time_idx)
{
	const osc_time_def *t = &osc_tim[time_idx];
	float ns;
	if(t->osc_unit == OSC_UINT_NS)
		ns = t->osc_time;
	else if(t->osc_unit == OSC_UNIT_US)
		ns = t->osc_time * 1000.0f;
	else if(t->osc_unit == OSC_UNIT_MS)
		ns = t->osc_time * 1000000.0f;
	else /* OSC_UNIT_S */
		ns = t->osc_time * 1000000000.0f;
	return ns;
}
/* Private includes ----------------------------------------------------------*/
//FOS_TSK_REGISTER(osc_menu_thread,PRIORITY_2,5);/* run as 10ms */
/* create cfg task gui detecter task run as 100 ms */
//FOS_TSK_REGISTER(osc_menu_hide_thread,PRIORITY_4,1000);
/* static link */
static osc_run_msg_def osc_run_msg;
/* LONG press cnt */
#define LONG_PRESS_LIMIT (12)
/* function */
static unsigned char ctrl_menu_sta = 1;
/* single thread */
void osc_single_thread(void)
{
}
/* */
/* check */
void osc_ctrl_change(void)
{
	/* */
	if( ctrl_menu_sta )
	{
		osc_ui_menu_show(0);
		osc_ui_menu1_show(1);
		/* show up */
		osc_ui_vol_scale(2,0);
		osc_ui_vol_scale(3,0);		
	}
	else
	{
		osc_ui_menu1_show(0);	
		osc_ui_menu_show(1);
	}
	/* change */
	ctrl_menu_sta ^= 1;	
}
/* return sta */
unsigned char osc_ctrl_menu_sta(void)
{
	return ctrl_menu_sta;
}
/* return ksc msg */
osc_run_msg_def * get_run_msg(void)
{
  return &osc_run_msg;
}
/* auto callback - auto adjust timebase, trigger, voltage (smart measurement) */
void key_auto_callback(void)
{
	osc_run_msg_def * runmsg = get_run_msg();
	draw_area_def * area = get_draw_area_msg();
	int ts_len = osc_time_scan_leng();
	int vs_len = osc_vol_scale_leng();

	/* determine current active channel: 0=CH1, 1=CH2 */
	unsigned char chn = osc_trig_source();

	/* show "Auto..." tips */
	osc_ui_tips_show(1);
	osc_ui_tips_str("Auto...");

	/* 1. set trigger to AUTO mode */
	osc_param_sys_set(&runmsg->sys_menu_set, RUN_TRIG_AUTO, 15);
	osc_ui_set_trig_type(RUN_TRIG_AUTO);

	/* 2. measure current channel frequency and auto-select timebase */
	{
		unsigned int t_strt = hal_sys_time_us();
		unsigned int c_strt = (chn == 0) ? hal_tim2_cnt() : hal_tim4_cnt();
		while(hal_sys_time_us() - t_strt < 10000);
		unsigned int t_end = hal_sys_time_us();
		unsigned int c_end = (chn == 0) ? hal_tim2_cnt() : hal_tim4_cnt();

		int time_idx = 12;
		if(c_end > c_strt && t_end > t_strt)
		{
			float freq = (float)(c_end - c_strt) / ((float)(t_end - t_strt) / 1000000.0f);
			float period_ns = 1000000000.0f / freq;
			float target_ns = period_ns * 0.05f;        //显示的周期数0.25f
			int i;
			for(i = 0; i < ts_len; i++)
			{
				if(osc_get_timebase_ns(i) >= target_ns)
				{
					time_idx = i;
					break;
				}
			}
			if(i == ts_len) time_idx = ts_len - 1;
		}
		runmsg->time_scale = time_idx;
		osc_scan_time(time_idx);
		osc_ui_time_str(osc_tim[time_idx].str);
	}

	/* 3. measure current channel peak-to-peak and auto-select voltage scale */
	{
		unsigned short max, min;
		int vol_idx = 5;
		if(osc_api_peek(chn, &max, &min) == FS_OK && max > min)
		{
			float vpp_mv = ((float)(max - min) / 50.0f) * osc_vol_scale_s(runmsg->vol_scale_ch[chn])->mv_int;
		float target_mv = vpp_mv / 6.0f;
		int i;
		for(i = 0; i < vs_len; i++)
		{
			if(osc_vol_scale_s(i)->mv_int >= (unsigned short)target_mv)
				{
					vol_idx = i;
					break;
				}
			}
			if(i == vs_len) vol_idx = vs_len - 1;
		}
		runmsg->vol_scale_ch[0] = vol_idx;
		runmsg->vol_scale_ch[1] = vol_idx;
		osc_vol_scale_set_dir(0, vol_idx);
		osc_vol_scale_set_dir(1, vol_idx);
		if(!osc_ctrl_menu_sta())
		{
			osc_ui_vol_scale(2,0);
			osc_ui_vol_scale(3,0);
		}
	}

	/* 4. center the vertical offset */
	{
		unsigned short center_offset = area->total_pixel_v / 2;
		runmsg->vol_offset_scale[0] = center_offset;
		runmsg->vol_offset_scale[1] = center_offset;
		osc_offset_scale_thread(0);
		osc_offset_scale_thread(1);
	}

	/* 5. center the trigger level */
	{
		unsigned short center_offset = area->total_pixel_v / 2;
		runmsg->trig_vol_level_ch[0] = center_offset;
		runmsg->trig_vol_level_ch[1] = center_offset;
		osc_trig_scale_thread(0);
		osc_trig_scale_thread(1);
	}

	/* 6. set trigger source to current channel, rising edge */
	osc_param_sys_set(&runmsg->sys_menu_set, chn, 13);
	osc_ui_set_trig_source(osc_trig_source());
	osc_ui_set_trig_edge(0);
	osc_exit_edge_reg(0);

	/* 7. set AC coupling */
	osc_param_sys_set(&runmsg->sys_menu_set, 1, 8);
	osc_param_sys_set(&runmsg->sys_menu_set, 1, 11);
	osc_refluse_dcac_chn();
}