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

#ifndef __OSC_H__
#define __OSC_H__
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

#include "gui_dev.h"
#include "gui.h"
#include "display_dev.h"
/* Includes ------------------------------------------------------------------*/
#define VERTICAL_GRID_NUM      (8)
#define HORIZONTAL_GRID_NUM    (14)
#define LITTLE_GRIG_NUM        (5)
#define VERTICAL_GRID_TOTAL    (VERTICAL_GRID_NUM * LITTLE_GRIG_NUM)
#define HORIZONTAL_GRID_TOTAL  (HORIZONTAL_GRID_NUM * LITTLE_GRIG_NUM)
/* left remind and up */
#define TOP_REMAIN_PIXEL       (46)
#define LEFT_REMAIN_PIXEL      (20)
#define BOTTOM_REMAIN_PIXEL    (25)
#define RIGHT_REMAIN_PIXEL     (60)
/* L8 mode color table */
#if LCD_MODE_L8
/* RGB color to L8 color */
#define RGTB(r,g,b)  ((r << 16) | (g << 8) | (b))
/* */
#define COLOR_GRID_POINT       (0)//(RGB(199, 195, 199))
#define COLOR_GRID_AREA_BG     (1)//(RGB(7, 3, 7))
#define COLOR_BACK_GROUND      (2)//(RGB(63, 75, 151))
/* menu win color table */
#define COLOR_MENU_ONE         (3)//(RGB(183, 83, 7))	/* color table */
/* button color table */
#define COLOR_BUTTON           (4)//(RGB(87,131,231))
/* char and hz table */
#define COLOR_CHAR             (5)//(RGB(255,255,255))
/* lines color */
#define COLOR_CH1              (6)//(RGB(255,255,7))
#define COLOR_CH2              (7)//(RGB(7,227,231))
/* TIPS_COLOR */
#define COLOR_TIPS_ERROR       (8)//(RGB(255,0,0))
/* CHN lines color */
#define COLOR_CH1_BG_0         (9)//(RGB(255,255,7))
#define COLOR_CH1_BG_1         (10)//(RGB(255,255,7))
#define COLOR_CH1_BG_F         (11)//(RGB(255,255,7))
#define COLOR_CH1_GR_0         (12)//(RGB(255,255,7))
#define COLOR_CH1_GR_1         (13)//(RGB(255,255,7))
#define COLOR_CH1_GR_F         (14)//(RGB(255,255,7))
/* CHN lines color */
#define COLOR_CH2_BG_0         (15)//(RGB(7,227,231))
#define COLOR_CH2_BG_1         (16)//(RGB(7,227,231))
#define COLOR_CH2_BG_F         (17)//(RGB(7,227,231))
#define COLOR_CH2_GR_0         (18)//(RGB(7,227,231))
#define COLOR_CH2_GR_1         (19)//(RGB(7,227,231))
#define COLOR_CH2_GR_F         (20)//(RGB(7,227,231))
/* group color */
#define COLOR_GROUP_LINE       (21)//(RGB(199, 195, 199))
#define COLOR_GROUP_BG         (22)//(RGB(7, 3, 7))
/* define point color */
#define COLOR_POINT0           (23)//(RGB(255,1,1))
#define COLOR_POINT1           (24)//(RGB(1,255,1))
/* focus color */
#define COLOR_BTN_FOCUS        (25)//(RGB(128,128,128))
/* right icon color */
#define COLOR_RIGHT_ICON       (26)//(RGB(128,128,128))
/* stop icon */
#define COLOR_STOP_ICON        (27)//(RGB(240,11,205))
/* math0 lines color */
#define COLOR_MATH0_BG_0       (28)//(RGB(255,0,0))
#define COLOR_MATH0_BG_1       (29)//(RGB(255,0,0))
#define COLOR_MATH0_BG_F       (30)//(RGB(255,0,0))
#define COLOR_MATH0_GR_0       (31)//(RGB(255,0,0))
#define COLOR_MATH0_GR_1       (32)//(RGB(255,0,0))
#define COLOR_MATH0_GR_F       (33)//(RGB(255,0,0))
/* math1 lines color */
#define COLOR_MATH1_BG_0       (34)//(RGB(0,255,0))
#define COLOR_MATH1_BG_1       (35)//(RGB(0,255,0))
#define COLOR_MATH1_BG_F       (36)//(RGB(0,255,0))
#define COLOR_MATH1_GR_0       (37)//(RGB(0,255,0))
#define COLOR_MATH1_GR_1       (38)//(RGB(0,255,0))
#define COLOR_MATH1_GR_F       (39)//(RGB(0,255,0))
/* math2 lines color */
#define COLOR_MATH2_BG_0       (40)//(RGB(0,0,255))
#define COLOR_MATH2_BG_1       (41)//(RGB(0,0,255))
#define COLOR_MATH2_BG_F       (42)//(RGB(0,0,255))
#define COLOR_MATH2_GR_0       (43)//(RGB(0,0,255))
#define COLOR_MATH2_GR_1       (44)//(RGB(0,0,255))
#define COLOR_MATH2_GR_F       (45)//(RGB(0,0,255))
/* trig lines */
#define COLOR_CH1_GR_TRIG      (46)//(RGB(7,227,231))
#define COLOR_CH1_BG_TRIG      (47)//(RGB(7,227,231))
#define COLOR_CH2_GR_TRIG      (48)//(RGB(255,255,7))
#define COLOR_CH2_BG_TRIG      (49)//(RGB(255,255,7))
/* diff pridata color */
#define COLOR_TEXT_ARROW_CH1   (50)//(RGB(7, 3, 7))
#define COLOR_TEXT_ARROW_CH2   (51)//(RGB(7, 3, 7))
/* #define */
#define COLOR_MEASURE_LINE_H0_0 (52)
#define COLOR_MEASURE_LINE_H0_1 (53)
#define COLOR_MEASURE_LINE_H1_0 (54)
#define COLOR_MEASURE_LINE_H1_1 (55)
#define COLOR_MEASURE_LINE_V0_0 (56)
#define COLOR_MEASURE_LINE_V0_1 (57)
#define COLOR_MEASURE_LINE_V1_0 (58)
#define COLOR_MEASURE_LINE_V1_1 (59)
/* botton */
#define COLOR_BTN_LINE_NEW      (60)
/* title */
#define COLOR_TITLE_BG          (61)
#define COLOR_TITLE_BTN         (62)
#define COLOR_TITLE_FOCUS0      (63)
#define COLOR_TITLE_FOCUS1      (64)
/* group */
#define COLOR_CH1_G             (65)//(RGB(255,255,7))
#define COLOR_CH2_G             (66)//(RGB(7,227,231))
#define COLOR_TRIG_G            (67)//(RGB(7,227,231))
#define COLOR_SYS               (68)
#define COLOR_TRIG_TD           (69)
#define COLOR_SYS_BG            (70)
#define COLOR_BTN_CTRL          (71)
#define COLOR_BTN_CTRL_L        (72)
#define COLOR_BAT_CAP           (73)
#define COLOR_BAT_LOW           (74)
#define COLOR_BAT_HALF          (75)
#define COLOR_CTRL_CH1          (76)
#define COLOR_CTRL_CH2          (77)
#define COLOR_BG_BTN            (78)
#define COLOR_BG_FUCUS_BTN      (79)
#define COLOR_BG_FUCUS_CTRL     (80)
#define COLOR_MEASURE_CHAR      (81)
/* typdef chn line manage */
typedef struct
{
	unsigned char BG_0;
	unsigned char BG_1;
	unsigned char BG_F;
	unsigned char GR_0;
	unsigned char GR_1;
	unsigned char GR_F;
}chn_manage_def;
/* color table */
#define COLOR_TABLE_L8         \
{                              \
RGTB(80, 80, 80),      /* 0  */\
RGTB(16, 16, 32),      /* 1  */\
RGTB(16, 16, 32),      /* 2  */\
RGTB(183, 83, 7),      /* 3  */\
RGTB(40,40,40),        /* 4  */\
RGTB(255,255,255),     /* 5  */\
RGTB(255,255,7),       /* 6  */\
RGTB(3,227,231),       /* 7  */\
RGTB(255,0,0),         /* 8  */\
RGTB(255,255,7),       /* 9  */\
RGTB(255,255,7),       /* 10 */\
RGTB(255,255,7),       /* 11 */\
RGTB(255,255,7),       /* 12 */\
RGTB(255,255,7),       /* 13 */\
RGTB(255,255,7),       /* 14 */\
RGTB(3,227,231),       /* 15 */\
RGTB(3,227,231),       /* 16 */\
RGTB(3,227,231),       /* 17 */\
RGTB(3,227,231),       /* 18 */\
RGTB(3,227,231),       /* 19 */\
RGTB(3,227,231),       /* 20 */\
RGTB(195, 195, 199),   /* 21 */\
RGTB(3, 3, 7),         /* 22 */\
RGTB(255,1,1),         /* 23 */\
RGTB(0,255,0),         /* 24 */\
RGTB(160,160,160),     /* 25 */\
RGTB(0,255,0),         /* 26 */\
RGTB(240,11,205),      /* 27 */\
RGTB(255,0,0),         /* 28 */\
RGTB(255,0,0),         /* 29 */\
RGTB(255,0,0),         /* 30 */\
RGTB(255,0,0),         /* 31 */\
RGTB(255,0,0),         /* 32 */\
RGTB(255,0,0),         /* 33 */\
RGTB(0,0,255),         /* 34 */\
RGTB(0,0,255),         /* 35 */\
RGTB(0,0,255),         /* 36 */\
RGTB(0,0,255),         /* 37 */\
RGTB(0,0,255),         /* 38 */\
RGTB(0,0,255),         /* 39 */\
RGTB(0,255,0),         /* 40 */\
RGTB(0,255,0),         /* 41 */\
RGTB(0,255,0),         /* 42 */\
RGTB(0,255,0),         /* 43 */\
RGTB(0,255,0),         /* 44 */\
RGTB(0,255,0),         /* 45 */\
RGTB(255,255,0),       /* 46 */\
RGTB(255,255,0),       /* 47 */\
RGTB(3,227,231),       /* 48 */\
RGTB(3,227,231),       /* 49 */\
RGTB(7, 3, 7),         /* 50 */\
RGTB(7, 3, 7),         /* 51 */\
RGTB(25, 206, 12),     /* 52 */\
RGTB(25, 206, 12),     /* 53 */\
RGTB(25, 206, 12),     /* 54 */\
RGTB(25, 206, 12),     /* 55 */\
RGTB(251, 3, 9),       /* 56 */\
RGTB(251, 3, 9),       /* 57 */\
RGTB(251, 3, 9),       /* 58 */\
RGTB(251, 3, 9),       /* 59 */\
RGTB(80, 80, 80),      /* 60 */\
RGTB(10, 0, 50),       /* 61 */\
RGTB(90, 90, 90),      /* 62 */\
RGTB(128, 128, 128),   /* 63 */\
RGTB(0, 128, 0),       /* 64 */\
RGTB(190,190,7),       /* 65 */\
RGTB(3,190,190),       /* 66 */\
RGTB(0,130,0),         /* 67 */\
RGTB(188,80,86),       /* 68 */\
RGTB(200,60,66),       /* 69 */\
RGTB(150,171,176),     /* 70 */\
RGTB(65,65,65),        /* 71 */\
RGTB(90,90,90),        /* 72 */\
RGTB(0,186,0),         /* 73 */\
RGTB(230,0,0),         /* 74 */\
RGTB(255,128,0),       /* 75 */\
RGTB(128,128,0),       /* 76 */\
RGTB(0,128,128),       /* 77 */\
RGTB(0, 0, 0),         /* 78 */\
RGTB(120, 120, 120),   /* 79 */\
RGTB(120, 120, 120),   /* 80 */\
RGTB(150, 150, 150),   /* 81 */\
}
#else
/* THREE color */
#define COLOR_GRID_POINT       (RGB(199, 195, 199))
#define COLOR_GRID_AREA_BG     (RGB(7, 3, 7))
#define COLOR_BACK_GROUND      (RGB(63, 75, 151))
/* menu win color table */
#define COLOR_MENU_ONE         (RGB(183, 83, 7))	/* color table */
/* button color table */
#define COLOR_BUTTON           (RGB(87,131,231))
/* char and hz table */
#define COLOR_CHAR             (RGB(255,255,255))
/* lines color */
#define COLOR_CH1              (RGB(255,255,7))
#define COLOR_CH2              (RGB(7,227,231))
/* TIPS_COLOR */
#define COLOR_TIPS_ERROR       (RGB(255,0,0))
#endif
#define COLOR_TIPS_WARNING     (COLOR_CH1)
#define COLOR_TIPS_NORMAL      (COLOR_CHAR)
/* #define tips level */
#define TIPS_ERROR             (0x6000)
#define TIPS_WARNING           (0x8000)
#define TIPS_NORMAL            (0xA000)
/* trig mode */
#define TRIG_MODE_RISING       (0x0000)
#define TRIG_MODE_FALLING      (0x0001)
#define TRIG_MODE_PULSE        (0x0002)
/* trig source */
#define TRIG_SOURCE_CH1        (0x0000)
#define TRIG_SOURCE_CH2        (0x0001)
#define TRIG_SOURCE_BOTH       (0x0002)
/* Define the area information occupied by the drawing area of the current screen */
typedef struct
{
	unsigned short start_pos_x;
	unsigned short start_pos_y;
	unsigned short stop_pos_x;
	unsigned short stop_pos_y;  
	unsigned short num_vertical;
	unsigned short num_horizontal;
	unsigned short pixel_vertiacl;
	unsigned short pixel_horizontal;
	unsigned short little_grid;
	unsigned short total_pixel_h;
	unsigned short total_pixel_v;
	unsigned short rev0;
	unsigned short rev1;
}draw_area_def;

/* create grid data */
static void osc_create_chn_icon(window_def * parent_win,unsigned short x,unsigned short y,unsigned short chn);
void osc_calculate_main_size(gui_dev_def * dev,window_def * win,unsigned short wf);
void osc_calculate_sg_size(gui_dev_def * dev,window_def * win0,unsigned int num);
void osc_calculate_menu_size(gui_dev_def * dev,window_def * win,unsigned short wf);
void osc_calculate_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,char ** title,char ** eng_title,unsigned short btn_num);
void osc_calculate_volage_string(window_def * pwin,widget_def *wd,int num,char * ch1,char * ch2);
void osc_calculate_time_string(window_def * pwin,widget_def *wd,int num,char * M,char * time);
void osc_calculate_measure_ch(window_def * pwin,widget_def *wd,int num,char ** item,unsigned char ch);
void osc_calculate_menu(window_def * pwin,widget_def *wd,int num,char ** item);
void osc_calculate_tips(window_def * pwin,widget_def *wd,unsigned short level,char * tip);
void osc_calculate_base_arrow(window_def * pwin,widget_def *wd,int chn);
void osc_calculate_title_string(window_def * pwin,widget_def *wd,int chn,char ** fast_title);
static void osc_create_TITLE(window_def * win);
draw_area_def * get_draw_area_msg(void);
static void osc_draw_trig_arrow(widget_def * wd);
void osc_calculate_trig_line(window_def * pwin,widget_def *wd,int chn);
void osc_btn_color(window_def * win,widget_def *wd,unsigned char mode);
void osc_draw_right_icon(widget_def * btn_wd );
void osc_calculate_right_icon(gui_dev_def * dev,window_def * win,widget_def *btn_wd,widget_def * icon_wd,unsigned short btn_num);
void osc_create_stop_icon(gui_dev_def * dev,window_def * win,widget_def *stop_wd,unsigned short px,unsigned short py);
void osc_calculate_chn_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,unsigned short btn_num);
void osc_calculate_bat_icon_size(gui_dev_def * dev,window_def * win,widget_def *wd);
void osc_ui_draw_bat(struct widget * win,unsigned char level);
void osc_calculate_ctrl_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,char ** title,char ** title_eng,unsigned short btn_num);
void osc_clear_win(window_def * win);
void draw_measure_item(struct widget * win,unsigned char chn,unsigned char item_id);
void osc_calculate_measure_item(gui_dev_def * dev,window_def * win,widget_def *wd);
static void osc_draw_offset_arrow(gui_dev_def * dev,unsigned short pos_x,unsigned short pos_y,unsigned short chn);
void osc_top_color(widget_def *wd,unsigned char mode);
void osc_ui_release_top_btn(void);
void draw_trig_type(struct widget * win , unsigned char trig_type);
void draw_trig_source(struct widget * win,unsigned char chn);
void draw_trig_icon(struct widget * win , unsigned char trig_type);
void draw_x1_x10(struct widget * win ,unsigned char x1_x10);
void draw_dcac(struct widget * win,unsigned char daca);
void draw_vol_scale(struct widget * win,char * scale);
void osc_release_area(widget_def * wid,unsigned char ddt);
void osc_redraw_grid(window_def * win);
void osc_redraw_languse(widget_def * win);
void osc_draw_rsr(widget_def * wid,unsigned char index);
unsigned char osc_run_mode(void);
void osc_set_run_mode(unsigned char m);
void draw_sys_settings(struct widget * win , unsigned char pos_id,unsigned char show_id);
void osc_draw_arrow_noload(widget_def * wid,unsigned short pos_y,unsigned short chn);
void osc_draw_trig_lines(widget_def * wd,unsigned char chn,unsigned short new_pos);
void osc_draw_cursor_lines(gui_dev_def * dev);
void osc_cursor_init_default(void);
#endif