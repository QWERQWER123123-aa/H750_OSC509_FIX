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

#ifndef __OSC_K24C02_H__
#define __OSC_K24C02_H__
/* defines */

/* Includes ------------------------------------------------------------------*/

static int k24c02_heap_init(void); 
static int k24c02_config_init(void);
void osc_k24C02_read(unsigned short ,void *,unsigned short);
void osc_k24c02_write(unsigned short ,void *,unsigned short);
#endif






























