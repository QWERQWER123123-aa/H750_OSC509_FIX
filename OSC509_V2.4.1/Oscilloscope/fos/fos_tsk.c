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
/* declare */
static void mx_time7_init(void);
static void mx_time13_init(void);
static void mx_time14_init(void);
static void mx_time16_init(void);
static void mx_time17_init(void);
static int thread_timer_init(void);
static int thread_start_it(void);
/* init */
FOS_INODE_REGISTER("thread",thread_timer_init,thread_start_it,0,15);
/* Private variables ---------------------------------------------------------*/
static TIM_HandleTypeDef htim7; 
static TIM_HandleTypeDef htim13;
static TIM_HandleTypeDef htim14;
static TIM_HandleTypeDef htim16;
static TIM_HandleTypeDef htim17;
/* thread timer init */
static fos_tsk_def * fos_tsk_linker[6];
static fos_tsk_def * fos_tsk_header[6];
/* thread time time_base_cnt */
static unsigned int fos_tsk_tick[6];
/* task linker init */
static void fos_task_init(void)
{
	/* get base linker */
	/* get linker */
	for( fos_tsk_def * tsk_base = FOS_TSK_BASE ; 
	     tsk_base < FOS_TSK_LIMIT ; tsk_base ++ )
	{
		/* allocate time */
		if( fos_tsk_linker[tsk_base->priority] != 0 )
		{
			/* link the task handler */
			fos_tsk_linker[tsk_base->priority]->linker = tsk_base;
			/* end if */
		}
		else
		{
			fos_tsk_header[tsk_base->priority] = tsk_base;
		}
		/* set the linker to current task handler*/
		fos_tsk_linker[tsk_base->priority] = tsk_base;
		/* end of linkder */
	}
}
/* testk */
static int thread_timer_init(void)
{
	/* init time 7 to time 14 */
	mx_time7_init();
	mx_time13_init();
	mx_time14_init();
	mx_time16_init();
	mx_time17_init();
	/* task init */
	fos_task_init();
	/* return OK */
	return FS_OK;
}
/* enable all thread */
static int thread_start_it(void)
{
	/* enable all it */
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);  
	HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn); 
	HAL_NVIC_EnableIRQ(TIM16_IRQn);
	HAL_NVIC_EnableIRQ(TIM17_IRQn);	
	/* return OK */
	return FS_OK;
}
/* run task 0 */
static void run_thead_priority_0(void)
{
	/* time tick ++ as every 1ms */
	fos_tsk_tick[0] ++;
	/* seek all task s */
	for( fos_tsk_def * task = fos_tsk_header[0] ; 
       task != 0 ; task = task->linker )
	{
		/* get enter equel */
		if( task->enter != 0 && ((fos_tsk_tick[0] % task->period) == 0 ))
		{
			task->enter();
		}
	} 
}
/* run task 1 */
static void run_thead_priority_1(void)
{
	/* time tick ++ as every 1ms */
	fos_tsk_tick[1] ++;
	/* seek all task s */
	for( fos_tsk_def * task = fos_tsk_header[1] ; 
       task != 0 ; task = task->linker )
	{
		/* get enter equel */
		if( task->enter != 0 && ((fos_tsk_tick[1] % task->period) == 0 ))
		{
			task->enter();
		}
	} 
}
/* run task 1 */
static void run_thead_priority_2(void)
{
	/* time tick ++ as every 1ms */
	fos_tsk_tick[2] ++;
	/* seek all task s */
	for( fos_tsk_def * task = fos_tsk_header[2] ; 
       task != 0 ; task = task->linker )
	{
		/* get enter equel */
		if( task->enter != 0 && ((fos_tsk_tick[2] % task->period) == 0 ))
		{
			task->enter();
		}
	} 
}
/* run task 3 */
static void run_thead_priority_3(void)
{
	/* time tick ++ as every 1ms */
	fos_tsk_tick[3] ++;
	/* seek all task s */
	for( fos_tsk_def * task = fos_tsk_header[3] ; 
       task != 0 ; task = task->linker )
	{
		/* get enter equel */
		if( task->enter != 0 && ((fos_tsk_tick[3] % task->period) == 0 ))
		{
			task->enter();
		}
	} 
}
/* run task 4 */
static void run_thead_priority_4(void)
{
	/* time tick ++ as every 1ms */
	fos_tsk_tick[4] ++;
	/* seek all task s */
	for( fos_tsk_def * task = fos_tsk_header[4] ; 
       task != 0 ; task = task->linker )
	{
		/* get enter equel */
		if( task->enter != 0 && ((fos_tsk_tick[4] % task->period) == 0 ))
		{
			task->enter();
		}
	} 
}
/* run task 1 */
void run_thead_priority_idle(void)
{
	/* time tick ++ as every 1ms */
	fos_tsk_tick[5] ++;
	/* seek all task s */
	for( fos_tsk_def * task = fos_tsk_header[5] ; 
       task != 0 ; task = task->linker )
	{
		/* get enter equel */
		if( task->enter != 0 && ((fos_tsk_tick[5] % task->period) == 0 ))
		{
			task->enter();
		}
	} 
}
/* task task */
static void mx_time7_init(void)
{
  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 200 - 1 ;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END TIM7_Init 2 */
}
/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void mx_time13_init(void)
{
  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 200 - 1;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 999;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */
  HAL_TIM_Base_Start_IT(&htim13);
  /* USER CODE END TIM10_Init 2 */
}
/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void mx_time14_init(void)
{
  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */
 
  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 200 - 1;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */
	HAL_TIM_Base_Start_IT(&htim14);
  /* USER CODE END TIM11_Init 2 */

}
/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void mx_time16_init(void)
{
  /* USER CODE BEGIN TIM13_Init 0 */
  /* USER CODE END TIM13_Init 0 */

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 200 - 1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 999;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */
  HAL_TIM_Base_Start_IT(&htim16);
  /* USER CODE END TIM13_Init 2 */

}
/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void mx_time17_init(void)
{
  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 200 - 1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 999;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */
  HAL_TIM_Base_Start_IT(&htim17);
  /* USER CODE END TIM14_Init 2 */
}
/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	/* MSP */
	if(htim_base->Instance==TIM7)
	{
		/* USER CODE BEGIN TIM7_MspInit 0 */

		/* USER CODE END TIM7_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM7_CLK_ENABLE();
		/* TIM7 interrupt Init */
		HAL_NVIC_SetPriority(TIM7_IRQn, 1, 1);
		/* USER CODE BEGIN TIM7_MspInit 1 */

		/* USER CODE END TIM7_MspInit 1 */
	}
	else if(htim_base->Instance==TIM13)
	{
		/* USER CODE BEGIN TIM13_MspInit 0 */

		/* USER CODE END TIM13_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM13_CLK_ENABLE();
		/* TIM13 interrupt Init */
		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 2, 1);
		/* USER CODE BEGIN TIM13_MspInit 1 */

		/* USER CODE END TIM13_MspInit 1 */
	}
	else if(htim_base->Instance==TIM14)
	{
		/* USER CODE BEGIN TIM14_MspInit 0 */

		/* USER CODE END TIM14_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM14_CLK_ENABLE();
		/* TIM14 interrupt Init */
		HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 3, 1);
		/* USER CODE BEGIN TIM14_MspInit 1 */

		/* USER CODE END TIM14_MspInit 1 */
	}
	else if(htim_base->Instance==TIM16)
	{
		/* USER CODE BEGIN TIM16_MspInit 0 */

		/* USER CODE END TIM16_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM16_CLK_ENABLE();
		/* TIM16 interrupt Init */
		HAL_NVIC_SetPriority(TIM16_IRQn, 4, 1);
		/* USER CODE BEGIN TIM16_MspInit 1 */

		/* USER CODE END TIM16_MspInit 1 */
	}
	else if(htim_base->Instance==TIM17)
	{
		/* USER CODE BEGIN TIM17_MspInit 0 */

		/* USER CODE END TIM17_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM17_CLK_ENABLE();
		/* TIM17 interrupt Init */
		HAL_NVIC_SetPriority(TIM17_IRQn, 5, 1);
		/* USER CODE BEGIN TIM17_MspInit 1 */

		/* USER CODE END TIM17_MspInit 1 */
	}
	else if(htim_base->Instance==TIM4)
	{
		/* USER CODE BEGIN TIM17_MspInit 0 */

		/* USER CODE END TIM17_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();
		/* TIM17 interrupt Init */
		HAL_NVIC_SetPriority(TIM4_IRQn, 6, 1);
		/* USER CODE BEGIN TIM17_MspInit 1 */

		/* USER CODE END TIM17_MspInit 1 */
	}	
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/
extern void osc_feed_iwdg(void);
/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */
  run_thead_priority_0();
  /* USER CODE END TIM7_IRQn 1 */	
	osc_feed_iwdg();
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim13);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */
	run_thead_priority_1();
  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
	osc_feed_iwdg();
}

/**
  * @brief This function handles TIM1 trigger and commutation interrupts and TIM11 global interrupt.
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 0 */

  /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 0 */
  HAL_TIM_IRQHandler(&htim14);
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 1 */
  run_thead_priority_2();
  /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 1 */
	osc_feed_iwdg();
}

/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */
	run_thead_priority_3();
  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
	osc_feed_iwdg();
}

/**
  * @brief This function handles TIM8 trigger and commutation interrupts and TIM14 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 0 */

  /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 0 */
  HAL_TIM_IRQHandler(&htim17);
  /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 1 */
  run_thead_priority_4();
  /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 1 */
	osc_feed_iwdg();
}






















































