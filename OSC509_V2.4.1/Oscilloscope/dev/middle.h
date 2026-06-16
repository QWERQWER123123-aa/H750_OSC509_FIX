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
#ifndef __MIDDLE_H__
#define __MIDDLE_H__
/* Includes ------------------------------------------------------------------*/
#include "display_dev.h"
/* Private includes ----------------------------------------------------------*/

/* function declares */
int middle_layer_init(display_info_def * dev_info);
void set_noload_point( unsigned short x , unsigned short y , unsigned int color );
void set_point( unsigned short * mark , unsigned short x , unsigned short y , unsigned int color );
unsigned short get_point( unsigned short x , unsigned short y );
void fill_rect(unsigned short psx,unsigned short psy,unsigned short pex,unsigned short pey,unsigned int color);
void fill_color(unsigned short psx,unsigned short psy,unsigned short pex,unsigned short pey,unsigned char * color);
void clear_display_dev(unsigned short color);
void set_l2_point_noload(unsigned short pwidth_d,unsigned short x,unsigned short y,unsigned char color);
unsigned char read_l2_point_noload(unsigned short pwidth_d,unsigned short x,unsigned short y);
void osc_ltdc_dma_fill(unsigned short x,unsigned short y,unsigned short x_size,unsigned short y_size,unsigned char color);
#endif


















