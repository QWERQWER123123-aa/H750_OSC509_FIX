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
#include "osc_menu.h"
#include "osc_cfg.h"
#include "osc_config.h"
/* USER CODE END Includes */
#define VBASE_CH1 (5100)
#define VBASE_CH2 (5250)
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static int osc_timer_init(void);
static void MX_TIM15_Init(void);
/* register node */
FOS_INODE_REGISTER("timer",osc_timer_init,0,0,15);
/* USER CODE BEGIN 0 */
extern void Error_Handler(void);
static void mx_time5_init(void);
static void MX_TIM8_Init(void);
static void hal_tim2_cap_init(void);
static void hal_tim4_cap_init(void);
/* USER CODE END TIM4_MspInit 0 */
TIM_HandleTypeDef htim4;
/* TIM4 CNt */
static unsigned int tim4_cnt_gt;
/* time init */
static int osc_timer_init(void)
{
	/* tim5 time base */
	mx_time5_init();
	/* tim8 init */
	MX_TIM8_Init();
	/* tim15 */
	MX_TIM15_Init();
	/* cap TIm2 */
	hal_tim2_cap_init();
	/* cap TIm4 */
	hal_tim4_cap_init();	
	/* return */
	return FS_OK;
}
/* tim5 del */
static void mx_time5_init(void)
{
  /* USER CODE BEGIN TIM7_Init 0 */
  __HAL_RCC_TIM5_CLK_ENABLE();
  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */
  TIM_HandleTypeDef htim5;
	/* memset */
	memset(&htim5 , 0 , sizeof(htim5));
  /* USER CODE END TIM7_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 200 - 1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xFFFFFFFF;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */
  HAL_TIM_Base_Start(&htim5);
  /* USER CODE END TIM7_Init 2 */
}
/* GPIO INIT */
static void HAL_TIM15_MspPostInit(TIM_HandleTypeDef* timHandle)
{
  /* GPIO INIT */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* TIM15 */
  if(timHandle->Instance==TIM15)
  {
  /* USER CODE BEGIN TIM15_MspPostInit 0 */

  /* USER CODE END TIM15_MspPostInit 0 */
#if OSC_CONFIG_PWM_OUT
		/* PE4 to PWM */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**TIM15 GPIO Configuration    
    PE4     ------> TIM15_CH1N 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);		
#else
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM15 GPIO Configuration    
    PA1     ------> TIM15_CH1N 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM15;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
  /* USER CODE BEGIN TIM15_MspPostInit 1 */

  /* USER CODE END TIM15_MspPostInit 1 */
  }
}
/* TIM15 init function */
static void MX_TIM15_Init(void)
{
	/* tim */
	TIM_HandleTypeDef htim15;
	/* set */
	memset(&htim15,0,sizeof(htim15));
	/* TIM15 clock enable */
	__HAL_RCC_TIM15_CLK_ENABLE();	
	/* ---------------------- */
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	/* OC CONFIG */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 10 - 1;//1mhz
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 1000 - 1;//1Khz
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
	/* breakdeadtime */
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
	/* init */
  HAL_TIM15_MspPostInit(&htim15);
	/* start PWM */
  HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
	/* end if */	
}
/* TIM8 init function */
static void MX_TIM8_Init(void)
{
	/* htim 8 */
  TIM_HandleTypeDef htim8;	
	/* clear */
	memset(&htim8,0,sizeof(htim8));
	/* init */
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	/* ENABLE */
	__HAL_RCC_TIM8_CLK_ENABLE();
	/* ends */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 9999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = VBASE_CH1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_MspPostInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE END TIM8_MspPostInit 0 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	/**TIM8 GPIO Configuration    
	PA7     ------> TIM8_CH1N
	PC8     ------> TIM8_CH3 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM8_MspPostInit 1 */
	/* start PWM */
  HAL_TIMEx_PWMN_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
  /* USER CODE END TIM8_MspPostInit 1 */
}
/* TIM2 CAP */
static void hal_tim2_cap_init(void)
{
	/* USER CODE BEGIN TIM2_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE END TIM2_MspInit 0 */
	TIM_HandleTypeDef htim2;
	/* Peripheral clock enable */
	__HAL_RCC_TIM2_CLK_ENABLE();
  /* Peripheral clock enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* set */
	memset(&htim2,0,sizeof(htim2));
	/**TIM2 GPIO Configuration    
	PA0     ------> TIM2_ETR 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_TIM_Base_Start(&htim2);
  /* USER CODE END TIM2_Init 2 */	
}
/* TIM2 CAP */
static void hal_tim4_cap_init(void)
{
	/* USER CODE BEGIN TIM4_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* Peripheral clock enable */
	__HAL_RCC_TIM4_CLK_ENABLE();
  /* Peripheral clock enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/* set */
	memset(&htim4,0,sizeof(htim4));
	/**TIM4 GPIO Configuration    
	PA0     ------> TIM4_ETR 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xFFFF;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
  HAL_TIM_Base_Start_IT(&htim4);
	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM4_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);	
  /* USER CODE END TIM4_Init 2 */	
}
/**
  * @brief hal_sys_time_us
	* @param none
  * @retval None
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	static unsigned char tim4_flag = 0;
  if(tim4_flag==0)
	{ tim4_flag = 1; }
	else
	{ tim4_cnt_gt++; }
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}
/**
  * @brief hal_sys_time_us
	* @param none
  * @retval None
  */
void user_back_light(unsigned short pwm)
{
	TIM15->CCR1 = pwm;
}
/**
  * @brief hal_sys_time_us
	* @param none
  * @retval None
  */
unsigned int hal_sys_time_us(void)
{
	return TIM5->CNT;
}
/* const +375,+250,+125,0,-125,-250,-375 */
unsigned short osc_pwm_ins(unsigned char chn,float mv)
{
	/* K */
	float k,b;
	unsigned short ret = 5000;
	unsigned short * cal_buffer;
	/* get msg */
	osc_run_msg_def * runmsg = get_run_msg();
	const osc_vol_scale_def * rtp = osc_get_vol_scale(chn);	
	/* check and set */
	if( chn == 0 )
	{
		if( rtp->gain_sel == 0 )
		{
			cal_buffer = &runmsg->pos_zero_pwm_ch1[0];
		}
		else
		{
			cal_buffer = &runmsg->pos_zero_pwm_ch1[7];
		}
	}
	else
	{
		if( rtp->gain_sel == 0 )
		{		
			cal_buffer = &runmsg->pos_zero_pwm_ch2[0];
		}
		else
		{
			cal_buffer = &runmsg->pos_zero_pwm_ch2[7];
		}
	}
	/* check */
	if( mv >= 375 )
	{
		ret = (unsigned short)(3.6f * ( mv - 375.0f ) + cal_buffer[0]); 
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
		ret = (unsigned short)((float)cal_buffer[6] + 3.6f * ( mv + 375.0f )); 
	}
	/* return */
	return ret;
}
/**
  * @brief osc_set_offset_dac
	* @param none
  * @retval None
  */
void osc_set_offset_dac(unsigned char chn,signed short pwm_dac_mv)
{
	/* get msg */
	osc_run_msg_def * runmsg = get_run_msg();
	/* set */
	unsigned short md;
	/* set pwm dac */
	float fv = (float)(-pwm_dac_mv);//* 3000.0f * 10000.0f;
	/* check chn */
	if( chn == 0 )
	{
		/* set pwm */
		md = osc_pwm_ins(0,fv);
		/* set pwm */
		TIM8->CCR1 = md;
	}
	else
	{
		/* set pwm */
		md = osc_pwm_ins(1,fv);
		/* set pwm */
		TIM8->CCR3 = md;
	}
}
/**
  * @brief hal_tim2_cnt
	* @param NONE
  * @retval None
  */
void osc_pwm_dir(unsigned char chn,unsigned short pwm)
{
	if( chn == 0 )
	{	
		/* set pwm */
		TIM8->CCR1 = (unsigned short)pwm;
	}
	else
	{
		/* set PWM */
		TIM8->CCR3 = (unsigned short)pwm;
	}	
}
/**
  * @brief hal_tim2_cnt
	* @param NONE
  * @retval None
  */
unsigned int hal_tim2_cnt(void)
{
	return TIM2->CNT;
}
/**
  * @brief hal_tim4_cnt
	* @param NONE
  * @retval None
  */
unsigned int hal_tim4_cnt(void)
{
	return (tim4_cnt_gt << 16) + TIM4->CNT;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/












