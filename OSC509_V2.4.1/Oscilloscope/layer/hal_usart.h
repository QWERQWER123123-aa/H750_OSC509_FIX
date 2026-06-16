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

#ifndef __HAL_USART_H__
#define __HAL_USART_H__
/* Includes ------------------------------------------------------------------*/

/* hal func init */
static int hal_usart_init(void);
static void mx_USART1_UART_Init(void);
void hal_usart_send(void * vd,unsigned short len);
void hal_usart_send_one(unsigned char c);

#endif
























