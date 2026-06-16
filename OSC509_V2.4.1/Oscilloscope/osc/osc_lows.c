///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file           : main.c
//  * @brief          : Main program body
//  ******************************************************************************
//  * @attention
//  *
//  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
//  * All rights reserved.</center></h2>
//  *
//  * This software component is licensed by ST under Ultimate Liberty license
//  * SLA0044, the "License"; You may not use this file except in compliance with
//  * the License. You may obtain a copy of the License at:
//  *                             www.st.com/SLA0044
//  *
//  ******************************************************************************
//  */
///* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "osc.h"
//#include "gui.h"
//#include "fos.h"
//#include "hal_gpio.h"
//#include "hal_tim.h"
//#include "osc_api.h"
//#include "osc_cfg.h"
//#include "math.h"
//#include "string.h"
//#include "hal_exit.h"
//#include "osc_line.h"
//#include "osc_menu.h"
//#include "osc_ui.h"
//#include "osc_win.h"
///* declares */
//static int osc_lows_init(void);
//static int osc_lows_config(void);
///* thread */
//FOS_INODE_REGISTER("osc_lows",osc_lows_init,osc_lows_config,0,9);
///* buffer */
//static unsigned char ch1_no[2],ch2_no[2];
///* set */
//static gui_dev_def * dev;
///* run msg */
//osc_run_msg_def * low_runmsgl;
///* area */
//const draw_area_def * area_lows;
///* run msg */
////static osc_run_msg_def * runmsg;
///* static switch case data */
//static unsigned char step = 1;
//static unsigned short last_ch[2] = {0xffff,0xffff};
//static unsigned short pixel_pos = 0;
//static unsigned char win_clear_flag = 0;
///* osc set low */
//static int osc_lows_init(void)
//{
//	return FS_OK;
//}
///* staitc */
//static int osc_lows_config(void)
//{
//	/* gui dev get */
//	dev = get_gui_dev();
//	/* get run msg for osc */
//	low_runmsgl = get_run_msg();
//	/* get draw area */
//	area_lows = get_draw_area_msg();		
//	/* return OK */
//	return FS_OK;	
//}
///* clear all flags */
//void osc_lows_clear(void)
//{
//	step = 1;
//	last_ch[0] = 0xffff;
//	last_ch[1] = 0xffff;
//	pixel_pos = 0;
//	win_clear_flag = 0;
//	/* clear */
//	osc_clear_screen(dev);
//}
///* reset */
//void osc_lows_reset(void)
//{
//	/* clear */
//	pixel_pos = 0;
//	win_clear_flag = 1;
//	/* clear */
//	for( int i = 0 ; i < 75 ; i ++ )
//	{
//		osc_clear_area(dev,area_lows->start_pos_x + pixel_pos + i,0);
//	}	
//}
///* thread */
//void osc_low_speed_thread(void)
//{
//	/* check run mode */
//	if( osc_run_mode() == RUN_STOP_MODE )	
//	{
//		return;
//	}	
//	/* check */
//	if( step == 0 )
//	{
//		/* read data from fifo */
////		osc_read_fifo_data(0,1);
//		/* transfor data */
//		osc_read_fifo_noload(ch1_no,ch2_no,2);
//		/* get io sta */
//		/* set the fifo */
//		osc_fifo_reset();		
//		/* set lines */
//		signed char use_ch[2];
//		unsigned short ch_m[2];
//		/* index */
//		unsigned char index = 0;
//		/* get */
//#if 1		
//		/* set index */
//		if( 1 )//clock_sta_2 == 0 )
//		{
//			if( ch1_no[0] == 0xff && ch2_no[0] == 0xff )
//			{
//				/* get */
//				if( ch1_no[1] != 0xff || ch2_no[1] != 0xff )
//				{
//					 /* index = 1 */
//					index = 1;
//				}
//			}
//			/* get index 2*/
//			else if( ch1_no[1] == 0xff && ch2_no[1] == 0xff )
//			{
//				/* get */
//				if( ch1_no[0] != 0xff || ch2_no[0] != 0xff )
//				{
//					/* index = 0 */
//					index = 0;
//				}
//			}
//			else if( ch1_no[0] == 0xff )
//			{
//				/* get */
//				index = 1;
//			}
//			else if( ch1_no[1] == 0xff )
//			{
//				/* get */
//				index = 0;				
//			}
//			else if( ch2_no[0] == 0xff )
//			{
//				/* get */
//				index = 1;
//			}
//			else if( ch2_no[1] == 0xff )
//			{
//				/* get */
//				index = 0;				
//			}
//			else if( ((unsigned char)ch1_no[0] & 0x0f) == 0x0f )
//			{
//				/* get */
//				index = 1;
//			}
//			else if( ((unsigned char)ch1_no[0] & 0xf0) == 0xf0 )
//			{
//				/* get */
//				index = 1;				
//			}
//			else if( ((unsigned char)ch1_no[1] & 0x0f) == 0x0f )
//			{
//				/* get */
//				index = 0;				
//			}
//			else if( ((unsigned char)ch1_no[1] & 0xf0) == 0xf0 )
//			{
//				/* get */
//				index = 0;				
//			}		
//			else if( ((unsigned char)ch2_no[0] & 0x0f) == 0x0f )
//			{
//				/* get */
//				index = 1;
//			}
//			else if( ((unsigned char)ch2_no[0] & 0xf0) == 0xf0 )
//			{
//				/* get */
//				index = 1;				
//			}
//			else if( ((unsigned char)ch2_no[1] & 0x0f) == 0x0f )
//			{
//				/* get */
//				index = 0;				
//			}
//			else if( ((unsigned char)ch2_no[1] & 0xf0) == 0xf0 )
//			{
//				/* get */
//				index = 0;				
//			}			
//			else
//			{
//				/* error data */
//				index = 0;
//			}	
//	  }	
//#endif		
//		/* get data */
//		use_ch[0] = (signed char)ch1_no[index];
//		use_ch[1] = (signed char)ch2_no[index];
//	  /* chang to lcd */
//		ch_m[0] = area_lows->total_pixel_v - (float)(use_ch[0] + 128) / 255.0f * area_lows->total_pixel_v;
//		ch_m[1] = area_lows->total_pixel_v - (float)(use_ch[1] + 128) / 255.0f * area_lows->total_pixel_v;	
//		/* set lines */
//		if( last_ch[0] != 0xffff && last_ch[1] != 0xffff )
//		{
//			/* check clear */
//			if( win_clear_flag == 1 )
//			{
//				osc_clear_area(dev,area_lows->start_pos_x + pixel_pos + 75,0);
//				osc_clear_area(dev,area_lows->start_pos_x + pixel_pos + 75 + 1,0);
//			}
//			/* show lines */
//			if( osc_enable_chn(0) == 0 )
//			{
//				osc_draw_lines(dev,area_lows->start_pos_x + pixel_pos,area_lows->start_pos_y + last_ch[0],area_lows->start_pos_x + pixel_pos + 1,area_lows->start_pos_y + ch_m[0] , 0 , 0 , 0 ); 
//			}
//			/* ch2 */
//			if( osc_enable_chn(1) == 0 )
//			{
//				osc_draw_lines(dev,area_lows->start_pos_x + pixel_pos,area_lows->start_pos_y + last_ch[1],area_lows->start_pos_x + pixel_pos + 1,area_lows->start_pos_y + ch_m[1] , 0 , 1 , 0 ); 
//			}
//		}
//		/* increame */
//		pixel_pos ++ ;
//		/* check */
//		if( pixel_pos >= area_lows->total_pixel_h )
//		{
//			osc_lows_reset();
//		}
//		/* reset */
//		last_ch[0] = ch_m[0];
//		last_ch[1] = ch_m[1];
//		/* set to low */
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);//set to low
//	}
//	else
//	{
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);//set to high
//	}
//	/* step */
//	step ++ ;
//	/* */
//	step %= 2;
//}

















































