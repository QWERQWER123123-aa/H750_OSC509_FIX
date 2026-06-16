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

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#ifndef __SDRAM_H__
#define __SDRAM_H__

/* Define SDRAM start Addr */
#define Bank5_SDRAM_ADDR    ((unsigned int)(0xC0000000))

/* SDRAM param */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((unsigned short)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((unsigned short)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((unsigned short)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((unsigned short)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((unsigned short)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((unsigned short)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((unsigned short)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((unsigned short)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((unsigned short)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((unsigned short)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((unsigned short)0x0200)

/* declares */

void sdram_init(void);

/* end */
#endif








