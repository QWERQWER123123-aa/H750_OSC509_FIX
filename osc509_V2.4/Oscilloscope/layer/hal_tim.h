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

#ifndef __HAL_TIM_H__
#define __HAL_TIM_H__
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

static int hal_tim_init(void);
static void hal_test_init(void);
void hal_pwm_stop(void);
void hal_pwm_start(void);
void hal_tim_psc(unsigned int psc);
unsigned int hal_sys_time_us(void);
unsigned int hal_tim2_cnt(void);
unsigned int hal_tim4_cnt(void);
//static void hal_tim3_cap_init(void);
static void hal_tim2_cap_init(void);
static void mx_tim4_init(unsigned short psc,unsigned short arr);
int hal_lows_flag(void);
void osc_clear_all_lines(void);
void osc_sys_delay(unsigned int tus);
void osc_set_offset_dac(unsigned char chn,signed short pwm_dac_mv);
void MX_TIM8_Init(void);

#endif


