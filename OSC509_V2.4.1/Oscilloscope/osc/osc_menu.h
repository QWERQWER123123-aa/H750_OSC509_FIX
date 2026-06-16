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
/* USER CODE END Header */

#ifndef __OSC_MENU_H__
#define __OSC_MENU_H__
/* Includes ------------------------------------------------------------------*/
#define RUN_TRIG_AUTO    (0)
#define RUN_TRIG_NORMAL  (1)
#define RUN_TRIG_SMART   (2)
#define RUN_TRIG_SINGLE  (3)
/* run mode */
#define RUN_RUN_MODE     (0)
#define RUN_STOP_MODE    (1)
#define RUN_READY_MODE   (2)
/* algin */
#pragma pack (1) 
/* typedef global settings */
typedef struct
{
	/* trig vol ch1 ch2 */
	unsigned short trig_vol_level_ch[2];
	/* vol offset sacle , ch1 ch2*/
	unsigned short vol_offset_scale[2];
	/* vol scale ch1 and ch2 */
	unsigned char vol_scale_ch[2];
	/* user 0 */
	unsigned short measure_item[2];
	/* zero pos */
	unsigned short pos_zero_pwm_ch1[14];
	unsigned short pos_zero_pwm_ch2[14];
	/* unsigned short */
	unsigned int sys_menu_set;	
	unsigned int sys_menu_set0;	
	/* trig mode */
	unsigned char trig_mode0;// auto . normal . single
	/* time scale */
	unsigned char time_scale;
	/* backup trig mode  */
	unsigned char back_light_per;	
	/* user */
	unsigned char user0;
	unsigned char check;
	unsigned short amp_cursor_a;
	unsigned short amp_cursor_b;
	unsigned short time_cursor_a;
	unsigned short time_cursor_b;
	/*-----------*/
}osc_run_msg_def;
#pragma pack ()
/* function */
static void osc_menu_thread(void);
static int osc_menu_heep(void);
void key_runstop_callback(void);
void key_auto_callback(void);
void key_measure_callback(void);
void key_single_callback(void);
void key_menu_callback(void);
static void check_COM2_event(unsigned char * sta_buf,unsigned int len);
static void menu_update(void);
static void menu_hide_auto(void);
static void osc_menu_hide_thread(void);
osc_run_msg_def * get_run_msg(void);
void osc_clear_all_lines(void);
void osc_clear_cursor_line_h(unsigned short old_cursor_pos);
void osc_clear_cursor_line_v(unsigned short old_cursor_pos);
void osc_move_cursor_h_redraw(unsigned short old_cursor_pos);
void osc_move_cursor_v_redraw(unsigned short old_cursor_pos);
static void osc_signle_mode(void);
void osc_single_thread(void);
void key_menu_Longfress_callback(void);
static void key_trig_short_click(void);
static void key_chnn_short_click(void);
static void key_swi_short_click(void);
static void key_math_short_click(void);
static void key_onoff_short_click(void);
static void key_onoff_long_click(void);
void user_change_trig_source(unsigned char trigs);
void user_swi_shnn(unsigned char fuc);
void key_runstop_long_click(void);
unsigned char osc_ctrl_menu_sta(void);
void osc_ctrl_change(void);

#endif