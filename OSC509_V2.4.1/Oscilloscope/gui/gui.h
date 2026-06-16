/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
#ifndef __GUI_H__
#define __GUI_H__	

#include "gui_dev.h"

/* setting */
#define CREATE_GUI_EVENT(a)  (a++)
/* settings */
#define GUI_HIDE         (0x0001)
#define GUI_DRAWED       (0x0002)
#define GUI_REHIDE       (0x0004)
#define GUI_OVERM        (0x0008)
/* check maros */
#define CHECK_HIDE(a)    ((a&GUI_HIDE)?1:0)
#define SET_HIDE(a)      ((a |= GUI_HIDE))
#define CLEAR_HIDE(a)    ((a &=~ (GUI_HIDE|GUI_DRAWED)))
/* set drawed */
#define CHECH_DRAWED(a)  ((a&GUI_DRAWED)?1:0)
#define SET_DRAWED(a)    ((a |= GUI_DRAWED))
#define CLEAR_DRAWED(a)  ((a &=~ GUI_DRAWED))
/* set REHIDE */
#define CHECK_REHIDE(a)  ((a&GUI_REHIDE)?1:0)
#define SET_REHIDE(a)    ((a |= GUI_REHIDE))
#define CLEAR_REHIDE(a)  ((a &=~ GUI_REHIDE))
/* set OVER */
#define CHECK_OVERM(a)   ((a&GUI_OVERM)?1:0)
#define SET_OVERM(a)     ((a |= GUI_OVERM))
#define CLEAR_OVERM(a)   ((a &=~ GUI_OVERM))
/* Includes ------------------------------------------------------------------*/
#define ORIGINAL_PARENT  (0)
/* gui level */
#define GUI_SAME_LEVEL   (0)
#define GUI_CHILD_LEVEL  (1)

#ifndef _VC_SIMULATOR_
/* RGB */
#define RGB(r,g,b) (( ( r & 0xF8 ) << 8 ) | ( ( g & 0xFC ) << 3 ) | ( ( b& 0xF8 ) >> 3 ))
#endif
/* decleare */
typedef struct win window_def;
/* Message type */
typedef enum
{
	/* Message */
	CLICK = 0,
	DOUCLICK = 1,
	REDRAW = 2,
	TEXTCHANGE = 3,
	POSCHANGE  = 4,
	SIZECHANGE = 5,
	/* end of */
}gui_msg_type;
/* gui msg */
typedef struct gui_info
{
	/* pri_data */
	void * pri_data;
	/* size and pos */
	unsigned short x;
	unsigned short y;
	unsigned short x_size;
	unsigned short y_size;
	/* widget flags */
	unsigned short wflags;
	/* memds */
	unsigned short upd;
	/* mark k */
	unsigned short mark_flag;
	unsigned short mx;
	unsigned short my;
	unsigned short mxstop;
	unsigned short mystop;
	/* abs pox */
	unsigned short abx;
	unsigned short aby;
	/* end */
}gui_info_def;
/* widget strucr def */
typedef struct widget
{
	/* linker */
	struct widget * peer_linker;
	/* common define */
	gui_info_def msg;
	/* draw methed */
	window_def * parent;
	/* dev */
	gui_dev_def * dev;
	/* draw methed */
	void (*draw)(struct widget *);
	/*  Message response */
	void *msg_response;
	/* end of func */
}widget_def;
/* win test */
typedef struct win 
{
	/* same win window */
	struct win * win_child;
	/* the linker for the hight def */
	widget_def * wchild;
	/* msg */
	gui_info_def msg;
	/* dev */
	gui_dev_def * dev;	
	/* draw methed */
	void (*draw)(window_def *);
	/*  Message response */
	void *msg_response;
	/* window off */
}window_def;
/* end if file */

/* Function declaration */
static int gui_static_creater(void);
int gui_win_creater(window_def * win);
int gui_widget_creater( widget_def * widget );
int gui_find_connect(window_def * ori,unsigned short x,unsigned short y);
void gui_dynamic_string(struct widget * wid);
int gui_hide_win(window_def * win);
int gui_show_win(window_def * win);
int gui_hide_widget(widget_def * wid);
int gui_show_widget(widget_def * wid);
static void gui_thread(void);
int gui_set_wid_text(widget_def * wid,char * data);
unsigned short gui_color(unsigned short colo);
int gui_move_wid(widget_def * wid,unsigned short px,unsigned short py);
int gui_wid_inline( widget_def * wid , unsigned short * upd);
void draw_hz24(gui_dev_def * dev , unsigned char * hzd,unsigned short x,unsigned short y,unsigned int color,unsigned int backcolor,unsigned char show_m);
void draw_hz(gui_dev_def * dev , unsigned char * hzd,unsigned short x,unsigned short y,unsigned int color,unsigned int backcolor,unsigned char show_m);
void gui_char(gui_dev_def * dev,unsigned short x,unsigned short y,char num,unsigned char size,unsigned int color,unsigned int backcolor,unsigned char show_m);
int gui_show_win_noload(window_def * ori);
void gui_l2_string(unsigned short pwidth , unsigned short x,unsigned short y,char * str,unsigned int color) ;
void gui_l2_char(unsigned short pwidth , unsigned short x,unsigned short y,char num,unsigned int color);
void draw_hz_l2(unsigned short pwidth ,const char * hzd,unsigned short x,unsigned short y,unsigned int color);
void osc_l1_string(gui_dev_def * dev,unsigned short x,unsigned short y,const char * str,unsigned int color,unsigned int backcolor,unsigned char show_m);
void osc_l24_string(gui_dev_def * dev,unsigned short x,unsigned short y,const char * str,unsigned int color,unsigned int backcolor,unsigned char show_m);
/* end of files */
#endif













