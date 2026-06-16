/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fos.h"
#include "string.h"
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static int osc_dac_init(void);
static void MX_DAC1_Init(void);
static void osc_dac_gpio_init(void);
static void osc_dac_dma_start(void);
static void osc_dac_TROG_tim_init(void);
/* register node */
FOS_INODE_REGISTER("osc_dac",osc_dac_init,0,0,15);
/* defines */
DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac1_ch1;
/* lcd inch */
extern unsigned char LCD_INCH;
/* init */
static int osc_dac_init(void)
{
	/* check VGA */
	if( LCD_INCH == 6 )
	{
		return FS_OK;
	}
	/* gpio */
	osc_dac_gpio_init();
	/* DAC and DMA */
	MX_DAC1_Init();
	/* TRGO INIT at tim6 */
	osc_dac_TROG_tim_init();
	/* set a defualt tim dac sinx */
	osc_dac_dma_start();
	/* return */
	return FS_OK;
}
/* static gpio_init */
static void osc_dac_gpio_init(void)
{
	/* GPIO def */
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/*
		DAC1 GPIO Configuration    
		PA4     ------> DAC1_OUT1 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	/* GPIO INIT */
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	/* enable */
}
/* DAC1 init function */
static void MX_DAC1_Init(void)
{
	/* INIT */
  DAC_ChannelConfTypeDef sConfig = {0};
	/* DAC1 clock enable */
	__HAL_RCC_DAC12_CLK_ENABLE();	
	/* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
	/* DAC Initialization */
  hdac1.Instance = DAC1;
	/* init */
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }
  /*
		DAC channel OUT1 config 
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	/* init */
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
	/* DAC1 DMA Init */
	/* DAC1_CH1 Init */
	hdma_dac1_ch1.Instance = DMA2_Stream2;
	hdma_dac1_ch1.Init.Request = DMA_REQUEST_DAC1;
	hdma_dac1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_dac1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_dac1_ch1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_dac1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_dac1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_dac1_ch1.Init.Mode = DMA_CIRCULAR;
	hdma_dac1_ch1.Init.Priority = DMA_PRIORITY_MEDIUM;
	hdma_dac1_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_dac1_ch1) != HAL_OK)
	{
		Error_Handler();
	}
	/* CDS */
	__HAL_LINKDMA(&hdac1,DMA_Handle1,hdma_dac1_ch1);	
}
/* TIM6 init */
static void osc_dac_TROG_tim_init(void)
{
	/* TIM6 b */
	TIM_HandleTypeDef htim6;
	/* TIM6 b */
  TIM_MasterConfigTypeDef sMasterConfig = {0};
	/* TIM6 clock enable */
	__HAL_RCC_TIM6_CLK_ENABLE();
	/* TIM6 CONFIGRATION */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 19;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	/* SET CONFIGRATION */
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
	/* UPDATA EVENT */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
/* static */
static void osc_dac_dma_start(void)
{
	/* set */
	unsigned short * g_usWaveBuff = (unsigned short *)0x38000000;
	/* create sinc buffer data */
	double sc = 0;
	/* create */
	for( int i = 0 ; i < 1000 ; i ++ )
	{
		/* sinc */
		g_usWaveBuff[i] = (unsigned short)(2000 * sin( sc ) + 2000);
//		g_usWaveBuff[i] += (unsigned short)(1000 * sin( sc * 3.0 ) + 1000);
		/* 2pi / 1000 */
		sc += 0.00628;
	}	
	/* start DMA */
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)g_usWaveBuff, 1000, DAC_ALIGN_12B_R);
	/* start tim6 */
	TIM6->CR1 |= 0x1;
}





















