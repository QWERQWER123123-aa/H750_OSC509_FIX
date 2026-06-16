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

#ifndef __OSC_WIN_H__
#define __OSC_WIN_H__
/* Includes ------------------------------------------------------------------*/
#include "gui.h"
/* Private includes ----------------------------------------------------------*/
#define  COLOR_L2_WHITE       (0)
#define  COLOR_L2_BLACK       (1)
#define  COLOR_L2_WBLUE       (2)
#define  COLOR_L2_GRAD        (3)
#define  COLOR_L2_GROUP_BG		(4)
#define  COLOR_L2_GROUP_LINE1 (5)
#define  COLOR_L2_GROUP_SEL   (6)
#define  COLOR_L2_GROUP_OFF   (7)
#define  COLOR_L2_SLIDER      (8)
#define  COLOR_L2_SLIDER_N    (9)
#define  COLOR_L2_SLIDER_BG   (10)
#define  COLOR_L2_BTN_FOCUS   (11)
/* time ctrl bar */
#define  COLOR_L2_TIME_BAR_C  (12)
#define  COLOR_L2_BTN_T       (13)
#define  COLOR_L2_BTN_CTRL    (14)
#define  COlOR_L2_BTN_PNG     (15)
#define  COLOR_L2_BTN_TFUCOS  (16)
/* l2 right */
#define COLOR_L2_RIGHT_ICON   (17)
/* measure item */
#define COLOR_L2_MEASURE_CH1  (18)
#define COLOR_L2_MEASURE_CH2  (19)
/* cha */
#define COLOR_L2_CH1          (20)
#define COLOR_L2_CH2          (21)
/* def */
#define COLOR_L2_MATH         (22)
#define COLOR_L2_CURC         (23)
#define COLOR_L2_FFTT         (24)
/* msgbox button color */
#define COLOR_L2_BUTTON_BG    (25)
#define COLOR_L2_WIN_BG_R     (26)
/* def */
#define RGTB_L2(r,g,b)  (((r << 16)) | ((g << 8)) | ((b)))
/* color table */
#define COLOR_TABLE_L2L8       \
{                              \
RGTB_L2(252, 252, 252),   /* 0  */\
RGTB_L2(252, 252, 252),   /* 1  */\
RGTB_L2(36, 80, 182),     /* 2  */\
RGTB_L2(32, 32, 32),      /* 3  */\
RGTB_L2(40, 40, 40),      /* 4  */\
RGTB_L2(40, 40, 40),      /* 5  */\
RGTB_L2(4,200,200),       /* 6  */\
RGTB_L2(20, 140, 60),     /* 7  */\
RGTB_L2(100,100,132),     /* 8  */\
RGTB_L2(4,100,100),       /* 9  */\
RGTB_L2(32, 32, 32),      /* 10 */\
RGTB_L2(152, 152, 152),   /* 11 */\
RGTB_L2(0, 0, 0),         /* 12 */\
RGTB_L2(4, 96, 96),       /* 13 */\
RGTB_L2(152, 152, 152),   /* 14 */\
RGTB_L2(100, 100, 100),   /* 15 */\
RGTB_L2(152, 152, 152),   /* 16 */\
RGTB_L2(0,252,0),         /* 17 */\
RGTB_L2(192,192,8),       /* 18 */\
RGTB_L2(4,192,192),       /* 19 */\
RGTB_L2(200,200,0),       /* 20 */\
RGTB_L2(4,100,100),       /* 21 */\
RGTB_L2(0,200,0),         /* 22 */\
RGTB_L2(0,200,0),         /* 23 */\
RGTB_L2(0,200,0),         /* 24 */\
RGTB_L2(80,80,80),        /* 25 */\
RGTB_L2(16, 16, 32),      /* 26 */\
}
/* set */
static int osc_win_init(void);
void osc_create_system_menu(void);
static void draw_group_wi2(struct widget * wid);
static void osc_draw_l2_win(window_def * win);
void osc_draw_l2_bmp(unsigned short pwidth , unsigned short x,unsigned short y,unsigned short x_size,unsigned short y_size,const unsigned char * tab,unsigned char color);
static void osc_create_sys_menu(window_def * parent_win,widget_def * grp);
static void osc_select(struct widget * wid,unsigned char index);
static void osc_l2_task(void);
void osc_slider_pos(struct widget * wid,unsigned short pos);
void osc_create_ch12t_menu(unsigned char index);
static void osc_create_ch12_menu(window_def * parent_win,widget_def * grp);
void osc_btn_l2_color(widget_def *wd,unsigned char mode);
void osc_l2_win_clear_mem(unsigned char mod);
int osc_l2_window_sta(void);
void osc_l2_touch(unsigned short px,unsigned short py);
void osc_l2_ui_event(unsigned short ID);
void osc_l2_touch_release(void);
unsigned char osc_param_sys_get(unsigned int src , unsigned char index);
void osc_param_sys_set(unsigned int * src , unsigned char b_data,unsigned char index);
void osc_l2_touch_event(widget_def * wid , unsigned char ID , unsigned char dat);
void osc_draw_time_btn_bmp(unsigned short pwidth , unsigned short x,unsigned short y,unsigned short x_size,unsigned short y_size,const unsigned char * tab);
void osc_refluse_dcac_chn(void);
unsigned char osc_trig_source(void);
unsigned char osc_trig_edge(void);
void osc_default_edge_ui(void);
unsigned char osc_enable_chn(unsigned char chn);
unsigned char osc_X10X1_get(unsigned char chn);
void osc_create_measure_math_menu(void);
void osc_create_fine_tune_menu(void);
void osc_fine_tune_hide(void);
void osc_fine_tune_touch(unsigned short px, unsigned short py);
void osc_fine_tune_touch_release(void);
void osc_fine_tune_touch_event(unsigned short ID);
int osc_fine_tune_sta(void);
void osc_gui_fine_tune_btn(widget_def * wid);
static void osc_create_measure_menu(window_def * parent_win,widget_def * grp);
void osc_draw_right_icon(widget_def * btn_wd );
void osc_draw_chn_icon_little(widget_def * wid);
void osc_measure_math_reflush(void);
unsigned char osc_language_get(void);
unsigned char osc_format(void);
unsigned char osc_smartAI(void);
unsigned char osc_usb_device_enable(void);
unsigned char osc_math_enable(void);
unsigned char osc_math_item(void);
unsigned char osc_fft_sta(void);
unsigned char osc_persist_get(void);
/* endif */

#endif