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

#ifndef __OSC_MEASURE_H__
#define __OSC_MEASURE_H__
/* Includes ------------------------------------------------------------------*/
/* function */
static int osc_measure_thread_init(void);
static void osc_measure_thread(void);
static int freq_measure_ch(unsigned char chn , char * buf);
static int osc_heep(void);
static int peek_measure_ch(unsigned char chn , char * buf);
static void osc_set_str(char * bdf,unsigned int limit);

/* deinen */
typedef struct
{
	const char * capital;
	int (*method)(unsigned char chn , char *);
}osc_measure_def;

void osc_measure_item_update(unsigned char chn,unsigned char item,unsigned char measure_item,unsigned char enable);
unsigned char * osc_measure_item_get(void);

#endif
























