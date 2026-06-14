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

#ifndef __OSC_LINE_H__
#define __OSC_LINE_H__
/* Includes ------------------------------------------------------------------*/
#include "gui.h"
/* function declare */
void osc_create_lines(gui_dev_def * dev,unsigned short * line_d,unsigned char mode,unsigned short index,unsigned char chn ,unsigned short zm);
void osc_draw_lines(gui_dev_def * dev,unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned short mode,unsigned short chn,unsigned short index);
void osc_clear_area(gui_dev_def * dev,unsigned short start_x,unsigned short start_y);
void osc_clear_screen(gui_dev_def * dev);
void osc_lows_reset(void);
void osc_create_points_xy(gui_dev_def * dev,unsigned short * x_p,unsigned short *y_p , unsigned char mode,unsigned char index,unsigned char chn);
void osc_draw_cursor_lines(gui_dev_def * dev);
void osc_cursor_init_default(void);
#endif