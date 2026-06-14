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
#include "main.h"
#include "osc_api.h"
#include "fos.h"
#include "hal_tim.h"
#include "osc_calibrate.h"
#include "osc_cfg.h"
#include "osc_menu.h"
/* exit init */
static int osc_exit_rot_init(void);
extern unsigned int osc_trig_start_flag;
extern unsigned char osc_trig_source(void);
/* ndtr */
unsigned short dma_trig_ndtr = 0;
/* exit init  */
FOS_INODE_REGISTER("exit_trig",osc_exit_rot_init,0,0,12);
/*----------------------------------------------------------------------------*/
/* exit init */
static int osc_exit_rot_init(void)
{
	/* init return OK */
	return FS_OK;
}
/* set exit enable */
void osc_set_trig_exit_sta(unsigned char mo)
{
	/* disable */
	if( mo )
	{
		/*-----------------------------------*/
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
		/*-----------------------------------*/
		if( osc_trig_source() )
		{
			HAL_NVIC_EnableIRQ(EXTI0_IRQn); 
		}
		else
		{
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 
		}
	}
	else
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); 
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	}
}
/* test t */
volatile unsigned int ts0,ts1,ts2;
/* set exit */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* get IO */
	switch(GPIO_Pin)
	{
		case GPIO_PIN_15:
		case GPIO_PIN_0:
			/* rot0 */
		  ts2 = hal_sys_time_us();
			ts1 = ts2 - ts0;
			ts0 = ts2;
			/* check NDTR POS */
		  if( osc_trig_start_flag == 1 )
			{
				/* clear */
				osc_trig_start_flag = 3;
				osc_set_trig_exit_sta(0);
				/* get NDTR */
				dma_trig_ndtr = FIFO_DBS_0 / 2 - DMA2_Stream0->NDTR;
			}
			else if( osc_trig_start_flag == 2 )
			{
				osc_trig_start_flag = 1;
			}
			/* cnt */
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_12|GPIO_PIN_13);
			/*----------------*/
			break;		
		default :
			break;
	}
	/* end of data */
}
/* trig edge */
void osc_exit_edge_reg(unsigned char mo)
{
	/* mo = 0 , rising */
	if( mo == 0 )
	{
		EXTI->RTSR1 |= (( 1 << 0 ) | ( 1 << 15 ));
		EXTI->FTSR1 &=~ ( ( 1 << 0 ) | ( 1 << 15 ));
	}
	else
	{
		EXTI->FTSR1 |= (( 1 << 0 ) | ( 1 << 15 ));
		EXTI->RTSR1 &=~ ( ( 1 << 0 ) | ( 1 << 15 ));		
	}
}
/* exit isr */
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
/* exit isr */
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}













