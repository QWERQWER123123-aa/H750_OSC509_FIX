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
static int calibrete_trig_heap_init(void);
static int calibrate_trig_config_init(void);
/* cal start */
unsigned char cal_trig_step_start = 0;
static osc_run_msg_def * runmsg_trig;
extern const osc_time_def osc_tim[];
extern void osc_refluse_dcac_chn(void);
extern void osc_param_sys_set(unsigned int * src , unsigned char b_data,unsigned char index);
extern unsigned char osc_trig_source(void);
extern void osc_exit_edge_reg(unsigned char mo);
/* register node */
FOS_INODE_REGISTER("calibrate",calibrete_trig_heap_init,calibrate_trig_config_init,0,8);
/* static */
static int calibrete_trig_heap_init(void)
{
	return FS_OK;
}
/* init param */
static int calibrate_trig_config_init(void)
{
	/* get */
	runmsg_trig = get_run_msg();
	/* return OK */
	return FS_OK;
}
/* trig start */
void osc_calibrate_trig_start(void)
{
	/* set PCS */
	/* uns */
	unsigned short vol_set0;
	unsigned short time_set;
	unsigned short base_offset;
	/* step 0 */
	vol_set0 = 4;
	/* get time */
	time_set = 5;	
	/* tim */
	base_offset = 200;
	/* step 0 */
	osc_vol_scale_set_dir(0,vol_set0);
	osc_vol_scale_set_dir(1,vol_set0);
	/* get time */
	osc_scan_time(time_set);	
	/* set str */
	osc_ui_time_str(osc_tim[time_set].str);
	/* tim */
	runmsg_trig->vol_offset_scale[0] = base_offset;
	runmsg_trig->vol_offset_scale[1] = base_offset;
	/* update */
	osc_offset_scale_thread(0);
	osc_offset_scale_thread(1);	
	/* trig */
	runmsg_trig->trig_vol_level_ch[0] = 50;//--------------------------------------------------
	runmsg_trig->trig_vol_level_ch[1] = 50;
	osc_trig_scale_thread(0);
	osc_trig_scale_thread(1);		
	/* set AC */
	osc_param_sys_set(&runmsg_trig->sys_menu_set,1,8);
	osc_param_sys_set(&runmsg_trig->sys_menu_set,1,11);
	osc_refluse_dcac_chn();
	/* set trig source ch1*/
	osc_param_sys_set(&runmsg_trig->sys_menu_set,0,13);//--------------------------------------
	osc_ui_set_trig_source(osc_trig_source());
	/* AUTO */
	osc_param_sys_set(&runmsg_trig->sys_menu_set,0,15);
	osc_ui_set_trig_type(0);
	/* set AUTO */
	/**********************************************************************************/
	/* set DAC */
	/**********************************************************************************/
	/* edge */
	osc_ui_set_trig_edge(0);
	/* set EXIT */
	osc_exit_edge_reg(0);
	/* set */
	cal_trig_step_start = 1;
}












