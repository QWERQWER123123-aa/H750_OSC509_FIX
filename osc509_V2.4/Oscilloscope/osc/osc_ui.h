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

#ifndef __OSC_UI_H__
#define __OSC_UI_H__

#include "gui.h"

/* Includes ------------------------------------------------------------------*/
int osc_create_ui(void);
void osc_create_button(struct widget * widget);
void osc_create_ch1_micon(struct widget * widget);
void osc_create_ch2_micon(struct widget * widget);
static void osc_main_draw(window_def * win);
static void osc_group_draw(window_def * win);
static void osc_menu_win_draw(window_def * win);
void osc_ui_time_str(char * str);
void osc_ui_vol_str(unsigned char chn,char * str);
void osc_ui_tips_str(char * str);
void osc_ui_move_offset_arrow(unsigned char chn,unsigned short pos);
void osc_calculate_trig_arrow(window_def * pwin,widget_def *wd,int chn);
void osc_ui_move_trig_arrow(unsigned char chn,unsigned short pos);
void osc_ui_vol_scale(unsigned char chn , char * str );
void osc_ui_move_trig_lines(unsigned char chn,unsigned short posy);
void osc_ui_trig_lines_show(unsigned char chn,unsigned char mode);
void osc_ui_measure_capital(unsigned char chn,unsigned char item,const char * capital);
void osc_ui_measure_data(unsigned char chn,unsigned char item,char * capital);
int osc_ui_menu_sta(void);
void osc_ui_menu_show(unsigned char mode);
void osc_ui_trig_arrow_show(unsigned char chn,unsigned char mode);
int osc_ui_trig_arrow_sta(unsigned char chn);
void osc_ui_set_menu_text_group(const char ** text,unsigned int len);
void osc_ui_set_one_menu_text(unsigned char item,const char * text);
void osc_ui_set_chn_text(unsigned char chn,const char * text);
void osc_ui_set_csh_show(unsigned char chn,unsigned char mode);
void osc_ui_set_trig_text(const char * text);
void osc_ui_set_trig_src(const char * text);
void osc_ui_show_offset_arrow(unsigned char chn , unsigned char mode);
void osc_ui_tips_show(unsigned char mode);
void osc_ui_tips_str_dir(char * str);
int osc_ui_tips_sta(void);
static void osc_ui_task(void);
void osc_ui_set_focus_arrow(unsigned char chn);
void osc_ui_measure_show(unsigned char mode);
int osc_ui_measure_sta(void);
gui_info_def * osc_ui_win_meun_msg(void);
gui_info_def * osc_ui_measure_meun_msg(void);
int osc_ui_find_win_btn(unsigned short px,unsigned short py);
void osc_ui_focus_btn(unsigned char btn_index,unsigned char mode);
void osc_ui_focus_measure_btn(unsigned char btn_index,unsigned char mode);
int osc_ui_find_measure_btn(unsigned short px,unsigned short py,widget_def ** btn_wd);
void osc_ui_release_measure_btn(void);
int osc_ui_find_measure_btn_by(unsigned short py);
int osc_ui_current_measure_btn(widget_def ** btn_wd);
void osc_ui_flush_right_icon(unsigned int run_flag);
void osc_ui_right_show(unsigned char index , unsigned char mode);
int osc_uiright_sta(unsigned char index);
void touch_measure_hide(unsigned char mode);
int osc_ui_find_measure_btn_mem(void);
void touch_measure_key_focus_mem(unsigned char index);
int osc_ui_measure_len(void);
void osc_ui_clear_btn_mem(void);
void touch_osc_right(void);
void osc_ui_stop_icon_show(unsigned char mode);
void osc_ui_stop_icon_toggle(void);
void osc_ui_move_mlines(unsigned char index,unsigned short pxy);
void osc_ui_menu1_show(unsigned char mode);
void osc_ui_release_btn(void);
void osc_ui_focus_ctrl_btn(unsigned char btn_index,unsigned char mode);
void osc_ui_release_ctrl_btn(void);
void osc_ui_set_trig_type(unsigned char index);
void osc_ui_set_trig_source(unsigned char index);
void osc_ui_set_trig_edge(unsigned char index);
void osc_ui_set_x10_x1(unsigned char chn,unsigned char index);
void osc_ui_set_dcac(unsigned char chn,unsigned char index);
unsigned short osc_ui_measure_item(unsigned char chn,unsigned char item);
void osc_ui_release_area(unsigned short pos_id);
void osc_ui_redraw_area(widget_def * wid);
void osc_ui_default_measure_item(void);
unsigned char osc_search_pos_is(unsigned char chn,unsigned item);
void osc_language_update_measure(void);
void osc_side_btn_update_languase(void);
void osc_run_stop(unsigned char index);
unsigned char osc_run_mode(void);
void osc_show_format(unsigned char pos_id,unsigned char show_id);
void osc_ui_set_trig_lines(unsigned char chn,unsigned short pos_y);
/* Private includes ----------------------------------------------------------*/

#endif

