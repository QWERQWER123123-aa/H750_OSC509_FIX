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
#include "fos.h"
#include "main.h"
#include "led.h"
/* LED define */
#define LED_RED_GPIO GPIOC
#define LED_RED_GPIO_PIN GPIO_PIN_2
/* register node */
FOS_INODE_REGISTER("led",led_hardware_init,0,0,15);
///* define task run as 500ms */
FOS_TSK_REGISTER(led_thread_500ms,PRIORITY_4,1000);
/* ------------------------ */
static int led_hardware_init(void)
{
		/* default dev */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* enable clock */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	/* display */
  GPIO_InitStruct.Pin = LED_RED_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	/* init */
	HAL_GPIO_Init(LED_RED_GPIO, &GPIO_InitStruct);	
	/* led on */
	HAL_GPIO_WritePin(LED_RED_GPIO,LED_RED_GPIO_PIN,GPIO_PIN_SET);	
	
	led_thread_500ms();
	/* return */
	return FS_OK;
}
/* led thread run as 500ms */
static void led_thread_500ms(void)
{
	HAL_GPIO_TogglePin(LED_RED_GPIO,LED_RED_GPIO_PIN);
}
///* led thread run as 300ms */
//static void led_thread_300ms(void)
//{
//	/* freq ctrl */
//	static unsigned int led_freq_ctrl = 0;
//	static unsigned char flag_led = 0;
//	/* freq ctrl */
//	if( led_freq_ctrl > 48 )
//	{
//		/* test */
//		if( flag_led == 1 )
//		{
//			/* set flags */
//			flag_led = 0;
//			/* on the led */			
//			HAL_GPIO_WritePin(LED_RED_GPIO,LED_RED_GPIO_PIN,GPIO_PIN_RESET);
//		}
//	}
//	else
//	{
//		/* test */
//		if( flag_led == 0 )
//		{
//			/* set flags */
//			flag_led = 1;
//			/* off the led */
//			HAL_GPIO_WritePin(LED_RED_GPIO,LED_RED_GPIO_PIN,GPIO_PIN_SET);
//		}
//	}
//	/* clear */
//	if( led_freq_ctrl++ >= 50 )
//	{
//		led_freq_ctrl = 0;
//	}
//}





















