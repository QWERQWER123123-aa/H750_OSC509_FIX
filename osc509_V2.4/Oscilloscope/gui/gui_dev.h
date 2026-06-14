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

#ifndef __GUI_DEV_H__
#define __GUI_DEV_H__
/* Includes ------------------------------------------------------------------*/
#include "gui_cfg.h"
/* Private includes ----------------------------------------------------------*/

/* set up gui dev */
typedef struct
{
	/* clear and set read point */
	void (*set_point)( unsigned short *,unsigned short , unsigned short , unsigned int );
	void (*set_noload_point)( unsigned short x , unsigned short y , unsigned int color ); 
	unsigned short (*read_point)( unsigned short , unsigned short );
#if HARDWARE_ACCEL_SUPPLY	
	/* for hardware */
	void (*fill_rect)(unsigned short,unsigned short ,unsigned short ,unsigned short ,unsigned int);
	void (*fill_color)(unsigned short ,unsigned short,unsigned short,unsigned short,unsigned char * );
	void (*clear_display_dev)(unsigned short );
#endif	
	/* screen  size information */
	unsigned int width;
	unsigned int height;
	unsigned int display_type; /* 0 is LCD , 1 is VGA */
	/* end of file */
}gui_dev_def;

/* function declares */

gui_dev_def * get_gui_dev(void);
int gui_dev_init(void);
int gui_creater(void);
/* end of file */






#endif

















