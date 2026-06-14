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
#include "osc_ui.h"
#include "osc_calibrate.h"
#include "osc_cfg.h"
#include "osc_menu.h"
#include "osc_api.h"
#include "stdio.h"
#include "k24c02.h"
#include "string.h"
#include "hal_exit.h"
#include "stdlib.h"
#include "math.h"
/* Private includes ----------------------------------------------------------*/
extern const osc_time_def osc_tim[];
extern void osc_pwm_dir(unsigned char chn,unsigned short pwm);
extern int osc_msg_box(unsigned char index);
static osc_run_msg_def * runmsg_cl;
extern void osc_param_save_noload(void);
static void osc_cal_time_out_thread(void);
/* cal start */
unsigned char cal_step_start = 0;
/* arve */
volatile float avg0 = 0 , avg1 = 0; 
double sqrt_0,sqrt_1;
static unsigned short target_v = 0;
static float pwm_set[2];
static unsigned char msg_f = 0;
static unsigned char cal_ok_cnt[2];
unsigned short cal_rel_ch1[14];
unsigned short cal_rel_ch2[14];
unsigned int cal_time_out = 0;
/* register node */
FOS_INODE_REGISTER("calibrate",calibrete_heap_init,calibrate_config_init,0,8);
FOS_TSK_REGISTER(osc_cal_time_out_thread,PRIORITY_4,1000);
/* static */
static int calibrete_heap_init(void)
{
	return FS_OK;
}
/* init param */
static int calibrate_config_init(void)
{
	/* get */
	runmsg_cl = get_run_msg();
	/* return OK */
	return FS_OK;
}
/* tim */
static void osc_cal_time_out_thread(void)
{
	/* check time */
	if( cal_time_out > 0 )
	{
		/* -------- */
		cal_time_out --;
		/* if( */
		if( cal_time_out == 0 )
		{
			/* time out */
			osc_msg_box(18);
			cal_step_start = 0;			
		}
	}
}
/* void calibrate step init */
void osc_calibrate_step_init(unsigned char step)
{
	/* uns */
	unsigned short vol_set0;
	unsigned short time_set;
	unsigned short base_offset;
	unsigned short target_0;
	unsigned short pwm_set_0;
	/* one */
	switch(step)
	{
		case 0:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 50;
			/* target */
			target_0 = 50;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 1;		
			/* break */
			break;
		case 1:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 100;
			/* target */
			target_0 = 100;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 2;				
			/* break */			
			break;
		case 2:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 150;
			/* target */
			target_0 = 150;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 3;				
			/* break */		
			break;
		case 3:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 200;
			/* target */
			target_0 = 200;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 4;				
			/* break */				
			break;
		case 4:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 250;
			/* target */
			target_0 = 250;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 5;				
			/* break */				
			break;
		case 5:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 300;
			/* target */
			target_0 = 300;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 6;				
			/* break */				
			break;	
		case 6:
			/* step 0 */
			vol_set0 = 6;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 350;
			/* target */
			target_0 = 350;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 7;				
			/* break */				
			break;
		case 7:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 50;
			/* target */
			target_0 = 50;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 8;		
			/* break */
			break;
		case 8:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 100;
			/* target */
			target_0 = 100;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 9;				
			/* break */			
			break;
		case 9:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 150;
			/* target */
			target_0 = 150;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 10;				
			/* break */		
			break;
		case 10:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 200;
			/* target */
			target_0 = 200;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 11;				
			/* break */				
			break;
		case 11:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 250;
			/* target */
			target_0 = 250;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 12;				
			/* break */				
			break;
		case 12:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 300;
			/* target */
			target_0 = 300;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 13;				
			/* break */				
			break;	
		case 13:
			/* step 0 */
			vol_set0 = 4;
			/* get time */
			time_set = 9;	
			/* tim */
			base_offset = 350;
			/* target */
			target_0 = 350;
			pwm_set_0 = 5000;
			/* step == 0 */	
			cal_step_start = 14;				
			/* break */				
			break;			
		default:
			break;
	}
	/* step 0 */
	osc_vol_scale_set_dir(0,vol_set0);
	osc_vol_scale_set_dir(1,vol_set0);
	/* get time */
	osc_scan_time(time_set);	
	/* set str */
	osc_ui_time_str(osc_tim[time_set].str);
	/* tim */
	runmsg_cl->vol_offset_scale[0] = base_offset;
	runmsg_cl->vol_offset_scale[1] = base_offset;
	/* update */
	osc_offset_scale_thread(0);
	osc_offset_scale_thread(1);
	/* target */
	target_v = target_0;
	pwm_set[0] = pwm_set_0;
	pwm_set[1] = pwm_set_0;
	cal_ok_cnt[0] = 0;
	cal_ok_cnt[1] = 0;
	msg_f = 0;	
	cal_time_out = 10;
}
/* cal mean and avger */
void osc_calibrate_mean(unsigned short * df0 ,unsigned short * df1)
{
	/* clear */
	avg0 = 0;
	avg1 = 0;
	double mean0 = 0;
	double mean1 = 0;
	/* check */
	for( int i = 0 ; i < 700 ; i ++ )
	{
		avg0 += (float)df0[i] / 700.0f;
		avg1 += (float)df1[i] / 700.0f;
	}
	/* mean */
	for( int i = 0 ; i < 700 ; i ++ )
	{
		mean0 += ((float)df0[i] - avg0)*((float)df0[i] - avg0);
		mean1 += ((float)df1[i] - avg1)*((float)df1[i] - avg1);
	}
	/* d */
	sqrt_0 = sqrt((double)mean0);
	sqrt_1 = sqrt((double)mean1);
	/* check pass ch1 */
	if( (abs((unsigned short)avg0 - target_v) < 3 ) && ( sqrt_0 < 100 ))
	{
		cal_ok_cnt[0]++;
	}
	else
	{
		cal_ok_cnt[0] = 0;
	}
	/* check pass */
	if( (abs((unsigned short)avg1 - target_v) < 3 ) && ( sqrt_1 < 100))
	{
		cal_ok_cnt[1]++;
	}
	else
	{
		cal_ok_cnt[1] = 0;
	}	
	/* check ok */
	if( cal_ok_cnt[0] >= 10 && cal_ok_cnt[1] >= 10 )
	{
		/* return next */
		cal_rel_ch1[cal_step_start-1] = pwm_set[0];
		cal_rel_ch2[cal_step_start-1] = pwm_set[1];
		/*-------*/
		if( cal_step_start == 14 )
		{
			/* ok */
			osc_msg_box(17);
			cal_step_start = 0;
			cal_time_out = 0;
			/* copy data */
			memcpy(runmsg_cl->pos_zero_pwm_ch1,cal_rel_ch1,sizeof(cal_rel_ch1));
			memcpy(runmsg_cl->pos_zero_pwm_ch2,cal_rel_ch2,sizeof(cal_rel_ch2));
			/* save data and reboot */
			/* ret */
			return;
		}		
		/* next */
		osc_calibrate_step_init(cal_step_start);
		/* once */
		if( msg_f == 0 )
		{
			msg_f = 1;
			osc_msg_box(cal_step_start + 2);
		}
		/*-------*/
		return;
	}
	/* PID */
	float PID_P = ( avg0 - (float)target_v ) * 2;
	/* set */
	pwm_set[0] += PID_P;
	/* set */
	osc_pwm_dir(0,(unsigned short)pwm_set[0]);	
	/* ch2 */
	PID_P = ( avg1 - (float)target_v ) * 2;
	/* set */
	pwm_set[1] += PID_P;	
	/* set */
	osc_pwm_dir(1,(unsigned short)pwm_set[1]);
}


































