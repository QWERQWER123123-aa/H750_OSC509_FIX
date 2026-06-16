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
#include "display_dev.h"
#include "gui_dev.h"
#include "middle.h"
#include "gui_cfg.h"
#include "gui.h"
#ifdef _VC_SIMULATOR_
/* */
void set_vc_point(unsigned short x,unsigned short y,unsigned int color);
#endif
/* Private includes ----------------------------------------------------------*/
/* structe the display msg */
static gui_dev_def gui_dev_s;
/* test data that will delete*/
static unsigned char inited_flag = 0;
/* gui_dev_init */
int gui_dev_init(void)
{
#ifndef _VC_SIMULATOR_
	/* get dev info */
	display_info_def * dev_info = get_display_dev_info();
	/* get width and height */
	gui_dev_s.width = dev_info->display_dev->pwidth;
	gui_dev_s.height = dev_info->display_dev->pheight;
	/* configrate the other funcs */
	gui_dev_s.set_point = set_point;
	gui_dev_s.read_point = get_point;	
	gui_dev_s.set_noload_point = set_noload_point;
#else
	/* get width and height */
	gui_dev_s.width = 800;//dev_info->display_dev->pwidth;
	gui_dev_s.height = 480;//dev_info->display_dev->pheight;
		/* configrate the other funcs */
	gui_dev_s.set_point = set_vc_point;
	gui_dev_s.read_point = 0;
#endif
#if HARDWARE_ACCEL_SUPPLY		
	gui_dev_s.fill_rect = fill_rect;
	gui_dev_s.fill_color = fill_color;
	gui_dev_s.clear_display_dev = clear_display_dev;
#endif	
	/* set up flag */
	inited_flag = 1;
	/* reutrn OK */
	return 0;
}
/* get gui_dev */
gui_dev_def * get_gui_dev(void)
{
	/* init or not */
	if( inited_flag == 0 )
	{
		gui_dev_init();
	}
	/* return handle */
	return &gui_dev_s;
}





















































