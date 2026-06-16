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

#ifndef __OSC_PARAM_H__
#define __OSC_PARAM_H__

#include "osc_menu.h"

#define OSC_SD_PARAM_FILE     "settings.bin"
#define OSC_PARAM_MAGIC       0xA55A5AA5
#define OSC_PARAM_SIZE        sizeof(osc_run_msg_def)

static int osc_param_init(void);
static void osc_param_thread(void);
static int osc_param_heap(void);
int osc_param_save(void);
void osc_disable_all_it(void);
void osc_reset_param(void);
void osc_param_save_noload(void);

#endif