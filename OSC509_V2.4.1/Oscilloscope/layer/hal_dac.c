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
#include "fos.h"
#include "hal_dac.h"
/* register a init inode */
FOS_INODE_REGISTER("hal_dac",hal_dac_init,0,0,9);
/* Handle */
DAC_HandleTypeDef DAC1_Handler;
/* hal init */
static int hal_dac_init(void)
{
	/* enable clock */
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_DAC12_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Initure.Pin=GPIO_PIN_5;
	GPIO_Initure.Mode=GPIO_MODE_ANALOG; 
	GPIO_Initure.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	/* init dac */
	DAC_ChannelConfTypeDef DACCH1_Config;
	/* struct */
	DAC1_Handler.Instance = DAC1;
	HAL_DAC_Init(&DAC1_Handler);
	/* init */
	DACCH1_Config.DAC_Trigger = DAC_TRIGGER_NONE;
	DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_2);
  /* start DAC */
	HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);
#if 0 // test	
	osc_set_dac(0);
#endif
	/* return */
	return FS_OK;
}
/* set value */
void osc_set_dac(signed short mv)
{
	/* ef */
	unsigned short ret;
	float k,b;
	/* dc */
	const unsigned short cal_buffer[7] = {2450,2300,2137,1950,1800,1630,1450};/* 375 250 125 0 -125 -250 -375 */
	/* last */
	static unsigned short out_last;
	/* check */
	if( mv >= 375 )
	{
		ret = (unsigned short)(1.2f * ( mv - 375.0f ) + cal_buffer[0]); 
	}
	else if( mv >= 250 )
	{
		k = ((float)cal_buffer[0] - (float)cal_buffer[1]) / 125.0f;
		b = (float)cal_buffer[1];
		/* ret */
		ret = (unsigned short)(k * ( mv - 250.0f ) + b); 
	}
	else if( mv >= 125 )
	{
		k = ((float)cal_buffer[1] - (float)cal_buffer[2]) / 125.0f;
		b = (float)cal_buffer[2];
		/* ret */
		ret = (unsigned short)(k * ( mv - 125.0f ) + b); 		
	}
	else if( mv >= 0 )
	{
		k = ((float)cal_buffer[2] - (float)cal_buffer[3]) / 125.0f;
		b = (float)cal_buffer[3];
		/* ret */
		ret = (unsigned short)(k * ( mv - 0.0f ) + b); 		
	}
	else if( mv >= -125 )
	{
		k = ((float)cal_buffer[3] - (float)cal_buffer[4]) / 125.0f;
		b = (float)cal_buffer[4];
		/* ret */
		ret = (unsigned short)(k * ( mv + 125.0f ) + b); 		
	}
	else if( mv >= -250 )
	{
		k = ((float)cal_buffer[4] - (float)cal_buffer[5]) / 125.0f;
		b = (float)cal_buffer[5];
		/* ret */
		ret = (unsigned short)(k * ( mv + 250.0f ) + b);		
	}
	else if( mv >= -375 )
	{
		k = ((float)cal_buffer[5] - (float)cal_buffer[6]) / 125.0f;
		b = (float)cal_buffer[6];
		/* ret */
		ret = (unsigned short)(k * ( mv + 375.0f ) + b);		
	}
	else
	{
		ret = (unsigned short)((float)cal_buffer[6] + 1.2f * ( mv + 375.0f )); 
	}
	/* transfer data */
	unsigned short out = ret;
	/* out data */
	if( out_last != out )
	{
 		HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,out);
	}
	/* update */
	out_last = out;
}


















