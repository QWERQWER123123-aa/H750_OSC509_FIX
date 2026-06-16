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
#include "middle.h"
#include "string.h"
#include "osc.h"
#include "osc_api.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PTD */
extern void osc_set_trig_exit_sta(unsigned char mo);
static int osc_analog_sample_init(void);
void osc_curve_test(void);
static int OSC_analog_tim1_init(unsigned char md);
static int OSC_analog_TIM3_init(unsigned char md);
static void MX_OSC_FIFO_DMA_CONFIG(unsigned char freq_mode , unsigned char trig_mode ,unsigned int PDA_DPH , unsigned int NORC_Mode);
/* register node */
FOS_INODE_REGISTER("analog_sample",osc_analog_sample_init,0,0,15);
/* Private typedef  for test version , if unuse it can be optimized */
static TIM_HandleTypeDef TIM_Handle; 
#define FIFO_DBS_1   ( 50 * 1024 ) // 50K * 8 = 400KBpts 
/* SRAM1 as the base */
#define FIFO_BBASE   (0x30000000)
#define FIFO_BBASE1  (0x24000000 + 800 * 480 )
#define FIFO_BBASE2  (0x38000000)
/* tpye define */
#define U16 unsigned short
/* Oscilloscope sampling buffer at block 0 */
U16 * src_fifo0_0 = (U16 *)( FIFO_BBASE );
static U16 * src_fifo0_1 = (U16 *)( FIFO_BBASE + FIFO_DBS_0 );
static U16 * src_fifo0_2 = (U16 *)( FIFO_BBASE + FIFO_DBS_0 * 2 );
static U16 * src_fifo0_3 = (U16 *)( FIFO_BBASE + FIFO_DBS_0 * 3 );
/* Oscilloscope sampling buffer at block 1 */
#if 0
static U16 * src_fifo1_0 = (U16 *)(FIFO_BBASE );
static U16 * src_fifo1_1 = (U16 *)(FIFO_BBASE + FIFO_DBS_1 );
static U16 * src_fifo1_2 = (U16 *)(FIFO_BBASE + FIFO_DBS_1 * 2);
static U16 * src_fifo1_3 = (U16 *)(FIFO_BBASE + FIFO_DBS_1 * 3);
static U16 * src_fifo1_4 = (U16 *)(FIFO_BBASE + FIFO_DBS_1 * 4);
static U16 * src_fifo1_5 = (U16 *)(FIFO_BBASE1);
static U16 * src_fifo1_6 = (U16 *)(FIFO_BBASE1 + FIFO_DBS_1 );
static U16 * src_fifo1_7 = (U16 *)(FIFO_BBASE2 );
#endif
/* Key variable definition */
static unsigned char osc_sample_mode = 1;
static unsigned int osc_flag_dma_tc = 0;
/* trig source start */
unsigned int osc_trig_start_flag = 0;
/**
  * @brief OSC_analog_tim1_init NVIC INIT
	* @param md 0 <= 40mhz , 1 > 40mhz
  * @retval None
  */
static int osc_analog_sample_init(void)
{
	/* analog */
	if( osc_sample_mode == 0 )
	{
		OSC_analog_tim1_init(0);
		MX_OSC_FIFO_DMA_CONFIG(0,1,DMA_PDATAALIGN_HALFWORD,DMA_NORMAL);
	}
	else
	{
		OSC_analog_TIM3_init(1);
	}
	/* retgurn */
	return FS_OK;
}
/**
  * @brief OSC_analog_tim1_init NVIC INIT
	* @param md 0 <= 40mhz , 1 > 40mhz
  * @retval None
  */
static int OSC_analog_tim1_init(unsigned char md)
{
	/* enable the clk */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();
  /* define a struction */
	GPIO_InitTypeDef GPIO_Handle;
	TIM_OC_InitTypeDef TIM_OC_Handle;
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	/* clear data */
	memset(&TIM_OC_Handle,0,sizeof(TIM_OC_Handle));
	memset(&sMasterConfig,0,sizeof(sMasterConfig));
	/* define a struction */
	GPIO_Handle.Pin = GPIO_PIN_9;
	GPIO_Handle.Mode = GPIO_MODE_AF_PP;
	GPIO_Handle.Pull = GPIO_NOPULL;
	GPIO_Handle.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Handle.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOE, &GPIO_Handle);
	HAL_GPIO_Init(GPIOA, &GPIO_Handle);
  /* time INIT */
	TIM_Handle.Instance = TIM1; 
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM_Handle.Init.Period = 2 - 1; //125khz
	TIM_Handle.Init.Prescaler = 200 - 1;
	HAL_TIM_PWM_Init(&TIM_Handle);
	/* msacter config */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_ENABLE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* PWM init */
	TIM_OC_Handle.OCMode = TIM_OCMODE_PWM1; 
	TIM_OC_Handle.OCPolarity = TIM_OCPOLARITY_LOW; 
	TIM_OC_Handle.Pulse = 1;
	/* CHANNEL init */
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_1); 
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_2);
	/* check */
	if( md == 0 )
	{
		__HAL_TIM_ENABLE_DMA(&TIM_Handle, TIM_DMA_CC1);
	}
  /* start */
	HAL_TIM_PWM_Start(&TIM_Handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM_Handle, TIM_CHANNEL_2);
	/* return */
  return FS_OK;
}
/**
  * @brief hal_analog_TIM3_init NVIC INIT
	* @param md 0 <= 40mhz , 1 > 40mhz
  * @retval None
  */
static int OSC_analog_TIM3_init(unsigned char md)
{
	/* enable the clk */
	__HAL_RCC_TIM3_CLK_ENABLE();
  /* define a struction */
	TIM_OC_InitTypeDef TIM_OC_Handle;
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};	
	/* clear data */
	memset(&TIM_OC_Handle,0,sizeof(TIM_OC_Handle));
	memset(&sSlaveConfig,0,sizeof(sSlaveConfig));
	memset(&sMasterConfig,0,sizeof(sMasterConfig));
  /* time INIT */
	TIM_Handle.Instance = TIM3; 
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM_Handle.Init.Period = 8 - 1; //125khz
	TIM_Handle.Init.Prescaler = 200 - 1;
	HAL_TIM_PWM_Init(&TIM_Handle);
  /* PWM init */
	TIM_OC_Handle.OCMode = TIM_OCMODE_PWM1; 
	TIM_OC_Handle.OCPolarity = TIM_OCPOLARITY_LOW; 
	TIM_OC_Handle.Pulse = 1;
	/* CHANNEL init */
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_1); 
	TIM_OC_Handle.Pulse = 3;
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_2); 
	TIM_OC_Handle.Pulse = 5;
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_3); 
	TIM_OC_Handle.Pulse = 7;
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_4); 
	/* slaver mode */
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	/* disable master mode */
  if (HAL_TIM_SlaveConfigSynchro(&TIM_Handle, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
	/* disable master mode */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	/* disable master mode */
  if (HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
	/* LINK DMA with TIM CHNN */
	__HAL_TIM_ENABLE_DMA(&TIM_Handle, TIM_DMA_CC1);
	__HAL_TIM_ENABLE_DMA(&TIM_Handle, TIM_DMA_CC2);
	__HAL_TIM_ENABLE_DMA(&TIM_Handle, TIM_DMA_CC3);
	__HAL_TIM_ENABLE_DMA(&TIM_Handle, TIM_DMA_CC4);
	/* 32bit tim init */
	MX_OSC_FIFO_DMA_CONFIG(md,1,DMA_PDATAALIGN_HALFWORD,DMA_NORMAL);	
	/* TIM 1 */
	OSC_analog_tim1_init(md);	
	/* return */
  return FS_OK;
}
/**
  * @brief MX_DMA_Init NVIC INIT
	* @param mode 0 disable , 1 enable
  * @retval None
  */
static void MX_DMA_NVIC_Init(unsigned char mode) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
	/* check */
	if( mode == 0 )
	{
		HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
	}
	else
	{
		HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	}
}
/**
  * @brief MX_OSC_FIFO_DMA_CONFIG
	* @param freq_mode 0 less than 40mhz and enable dac , 1 more than 40mhz disable dac
	* @param trig_mode 0 single or normal , 1 auto
	* @param NOR_mode  0 disable , 1 enable
  * @retval None
  */
static void MX_OSC_FIFO_DMA_CONFIG(unsigned char freq_mode , unsigned char trig_mode ,unsigned int PDA_DPH , unsigned int NORC_Mode)
{
	/* Define initialization structure */
	DMA_HandleTypeDef hdma_cho;
	/* Set sampling frequency */
	MX_DMA_NVIC_Init(trig_mode);
	/* fill with blank */
	memset(&hdma_cho,0,sizeof(hdma_cho));
	/* check mode */
	hdma_cho.Instance = DMA2_Stream0;
	hdma_cho.Init.Request = DMA_REQUEST_TIM3_CH4;
	hdma_cho.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_cho.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_cho.Init.MemInc = DMA_MINC_ENABLE;
	hdma_cho.Init.PeriphDataAlignment = PDA_DPH;
	hdma_cho.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_cho.Init.Mode = NORC_Mode;//DMA_NORMAL;//DMA_CIRCULAR;
	hdma_cho.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_cho.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_cho.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_cho.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_cho.Init.PeriphBurst = DMA_PBURST_SINGLE;
	/* enable */
	if( freq_mode == 0 )
	{
		hdma_cho.Init.Request = DMA_REQUEST_TIM1_CH1;
	}
	/* INIT */
	HAL_DMA_Init(&hdma_cho);	
	/* start DMA with buffer */
	unsigned int len_NDTR = FIFO_DBS_0 / 2 ; // test mode ignore the 8 byte mode 
	/* start */
	if( freq_mode == 0 )
	{
		HAL_DMA_Start_IT(&hdma_cho, (uint32_t)&GPIOD->IDR,(uint32_t)src_fifo0_0,FIFO_DBS_0);// test , mode 
		/* return right now */
		DMA2_Stream0->CR |= ( 1 << 3 );		
		/* return right now */
		return;
	}	
	else
	{
		/* return right now */		
		HAL_DMA_Start_IT(&hdma_cho, (uint32_t)&GPIOD->IDR,(uint32_t)src_fifo0_3,len_NDTR);
		/* return right now */
		DMA2_Stream0->CR |= ( 1 << 3 );			
	}
	/* INIT TIM3 CH2 */
	hdma_cho.Instance = DMA2_Stream1;
	hdma_cho.Init.Request = DMA_REQUEST_TIM3_CH2;	
	/* enable */
	HAL_DMA_Init(&hdma_cho);		
	/* start */
	HAL_DMA_Start_IT(&hdma_cho, (uint32_t)&GPIOD->IDR,(uint32_t)src_fifo0_1,len_NDTR);
	/* INIT TIM3 CH3 */
	hdma_cho.Instance = DMA1_Stream0;
	hdma_cho.Init.Request = DMA_REQUEST_TIM3_CH3;
	/* enable */
	HAL_DMA_Init(&hdma_cho);		
	/* start */
	HAL_DMA_Start_IT(&hdma_cho, (uint32_t)&GPIOD->IDR,(uint32_t)src_fifo0_2,len_NDTR);
	/* INIT TIM3 CH4 */
	hdma_cho.Instance = DMA1_Stream1;
	hdma_cho.Init.Request = DMA_REQUEST_TIM3_CH1;	
	/* enable */
	HAL_DMA_Init(&hdma_cho);		
	/* start */
	HAL_DMA_Start_IT(&hdma_cho, (uint32_t)&GPIOD->IDR,(uint32_t)src_fifo0_0,len_NDTR);
}
/* test all above */

/**
  * @brief osc_dma_restart
	* @param len is DMA tranfer len
  * @retval None
  */
void osc_dma_restart(unsigned int len)
{
	/* clear */
	DMA1->LIFCR = DMA_FLAG_TCIF0_4;
	DMA1->LIFCR = DMA_FLAG_TCIF1_5;
	DMA2->LIFCR = DMA_FLAG_TCIF0_4;
	DMA2->LIFCR = DMA_FLAG_TCIF1_5;
  /* check mode */
	if( osc_sample_mode == 0 )
	{
		/* clear */
		DMA2->LIFCR = DMA_FLAG_HTIF0_4;		
		/* 40mhz mode for test */
		DMA2_Stream0->NDTR = FIFO_DBS_0;
		/* restart */
		DMA2_Stream0->CR |= 0x1;
	}
	else
	{
		/* clear */
		DMA2->LIFCR = DMA_FLAG_HTIF0_4;			
		/* more than 40mhz freq for test */
		DMA1_Stream0->NDTR = FIFO_DBS_0 / 2;
		DMA1_Stream1->NDTR = FIFO_DBS_0 / 2;
		DMA2_Stream0->NDTR = FIFO_DBS_0 / 2;
		DMA2_Stream1->NDTR = FIFO_DBS_0 / 2;
		/* enable */
		DMA1_Stream0->CR |= 0x1;
		DMA1_Stream1->CR |= 0x1;
		DMA2_Stream0->CR |= 0x1;
		DMA2_Stream1->CR |= 0x1;		
	}
	/* restart DMA */
	TIM1->CR1 |= 0x1;
	/* end if */
}
/**
  * @brief OSC_DMA_SET_TC
	* @param none
  * @retval None
  */
void OSC_DMA_SET_TC(void)
{
	osc_flag_dma_tc = 1;
}
/**
  * @brief OSC_DMA_CLEAR_TC
	* @param none
  * @retval None
  */
void OSC_DMA_CLEAR_TC(void)
{
	osc_flag_dma_tc = 0;
}
/**
  * @brief OSC_DMA_CHECK_TC
	* @param none
  * @retval None
  */
char OSC_DMA_CHECK_TC(void)
{
	return osc_flag_dma_tc;
}
/**
  * @brief OSC_DMA_STOP
	* @param none
  * @retval None
  */
void OSC_DMA_STOP(void)
{
	/* Disable the TIM1 and TIM3 */
	TIM3->CR1 &=~ 0x1;
	TIM1->CR1 &=~ 0x1;
	/* Synchronize counters for all timers */
	TIM1->CNT = 0;
	TIM3->CNT = 0;	
}
/**
  * @brief DMA2_Stream0_IRQHandler
	* @param NONE
  * @retval None
  */
extern unsigned char osc_trig_type(void);
unsigned char trt;
/* set */
void DMA2_Stream0_IRQHandler(void)
{
  /* transfer OK */
	if((DMA2->LISR & DMA_FLAG_TCIF0_4) != RESET)
	{
		/* clear */
		DMA2->LIFCR = DMA_FLAG_TCIF0_4;
    /* reset */
		if((DMA2_Stream0->CR & DMA_SxCR_CT) == RESET)
		{
			/* DMA pause */
			OSC_DMA_STOP();
			/* set TC flag */
			OSC_DMA_SET_TC();
			/* end if data */
			/* check */
			if( trt == 0 )
			{
				osc_set_trig_exit_sta(0); 
			}
			/* clear */
			osc_trig_start_flag = 0;
		}
		else
		{
			/* Double buffer settings */
		}
	}
  /* transfer OK */
	if((DMA2->LISR & DMA_FLAG_HTIF0_4) != RESET)
	{
		/* clear */
		DMA2->LIFCR = DMA_FLAG_HTIF0_4;
    /* reset */
		if((DMA2_Stream0->CR & DMA_SxCR_CT) == RESET)
		{
			/* check */
			if( trt == 0 )
			{
				/* check */
				osc_set_trig_exit_sta(1); 
				/* trig start flag */
				osc_trig_start_flag = 2;//start trig
			}
			else if( trt == 1 )
			{
				/* check data */
				if( osc_trig_start_flag != 3 )
				{
					/* read */
					OSC_DMA_STOP();
					/* enable */
					DMA1_Stream0->CR &=~ 0x1;
					DMA1_Stream1->CR &=~ 0x1;
					DMA2_Stream0->CR &=~ 0x1;
					DMA2_Stream1->CR &=~ 0x1;					
					/* restart */
					osc_dma_restart(0);
					/* check */
					osc_set_trig_exit_sta(1); 
					/* trig start flag */
					osc_trig_start_flag = 1;//start trig					
				}
				else
				{
					osc_trig_start_flag = 2;
				}
			}
			/* end if data */
		}
		else
		{
			/* Double buffer settings */
		}
	}	
}
/* start */
static unsigned int strsd;
/* set read start addr */
void osc_set_copy_start(unsigned int strs)
{
	strsd = strs;
}
/* 
  * @brief copy data from fifo one by one 
	* @param data
	* @param chn : 0 is ch1,1 is ch2
	* @param start_pos
	* @param len
  * @retval None
*/
unsigned char osc_copy_from_fifo_onebyone(unsigned char chn,unsigned int stcnt)
{
	/* double and fifth buffer mode */
	unsigned int ind = 0 , idf = 0;
	/* reda */
	unsigned char data;
	/* copy data */
	unsigned int i = strsd + stcnt;
	/* Get row and column information */
	ind = i % 4 ;
	idf = i / 4;
	/* Get row and column information */
	if( chn == 0 )
	{
		switch(ind)
		{
			case 0:					
				data= (unsigned char)(src_fifo0_0[idf] >> 8);
			break;
			case 1:	
				data = (unsigned char)(src_fifo0_1[idf] >> 8);				
			break;
			case 2:
				data = (unsigned char)(src_fifo0_2[idf] >> 8);
			break;
			case 3:
				data = (unsigned char)(src_fifo0_3[idf] >> 8);
			break;
				default:
			break;
		}
	}
	else
	{
		switch(ind)
		{
			case 0:					
				data= (unsigned char)(src_fifo0_0[idf]);
			break;
			case 1:	
				data = (unsigned char)(src_fifo0_1[idf]);				
			break;
			case 2:
				data = (unsigned char)(src_fifo0_2[idf]);
			break;
			case 3:
				data = (unsigned char)(src_fifo0_3[idf]);
			break;
				default:
			break;
		}
	}
	/* retur */
	return data;
}
/**
  * @brief osc_copy_from_fifo
	* @param data
	* @param chn : 0 is ch1,1 is ch2
	* @param start_pos
	* @param len
  * @retval None
  */
void osc_copy_from_fifo(unsigned char * data , unsigned char chn,unsigned int start_pos,unsigned int len)
{
	/* Judge the current sampling mode*/
	if( osc_sample_mode == 0 )
	{
		/* base src */
		unsigned int base_addr = (unsigned int)&src_fifo0_0[start_pos];
		/* chn */
		unsigned char * chn_base = ( chn == 0 ) ? (unsigned char *)(base_addr + 1) : (unsigned char *)base_addr;
		/* low freq mode and data is fifo_0 */
		for(int i = 0 ; i < len ; i ++ )
		{
			/* copy data at stupy */
			data[i] = *chn_base ;
			/* increamer */
			chn_base += 2;
			/* enf of that */
		}
	}
	else
	{
		/* double and fifth buffer mode */
		unsigned int ind = 0 , idf = 0 , inc = 0;
		/* copy data */
		for(int i = start_pos ; i < start_pos + len ; i ++ )
		{
			/* Get row and column information */
			ind = i % 4 ;
			idf = i / 4;
			/* Get row and column information */
			switch(ind)
			{
				case 0:					
					data[inc] = (unsigned char)(src_fifo0_0[idf] >> 8);
					break;
				case 1:	
					data[inc] = (unsigned char)(src_fifo0_1[idf] >> 8);				
					break;
				case 2:
					data[inc] = (unsigned char)(src_fifo0_2[idf] >> 8);
					break;
				case 3:
					data[inc] = (unsigned char)(src_fifo0_3[idf] >> 8);
					break;
				default:
					break;
			}
			/* increate */
			inc ++;
		}
	}
}
/**
  * @brief HAL_LTDC_LineEventCallback
	* @param NONE
  * @retval None
  */
//void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hlt4dc)
//{
//	/* set a tese */
//	/* add the lines */
////	if( OSC_DMA_CHECK_TC() )
////	{
////		OSC_DMA_CLEAR_TC();
////		osc_curve_test();
////		osc_dma_restart(0);
////	}
//  /* thread */
//}
/* Test specific array */
unsigned char dstg[700];
/**
  * @brief osc_curve_test
	* @param NONE
  * @retval None
  */
void osc_curve_test(void)
{	
//	/* FILE */
//	osc_ltdc_dma_fill(14,200,700,200,1);
//	/* tesp */
//	for( int j = 0 ; j < 700 ; j ++ )
//	{
//		osc_copy_from_fifo(dstg,0,j*100,100);
//		/* set point */
//		for( int i = 0 ; i < 100 ; i ++ )
//		{
//			set_noload_point( j + 14 , dstg[i] + 220 , COLOR_CH1);		
//		}
//	}
}
/**
  * @brief osc_sys_delay
	* @param t
  * @retval None
  */
void osc_sys_delay(unsigned int t)
{
	
}
/**
  * @brief hal_pwm_start
	* @param NONE
  * @retval None
  */
void hal_pwm_start(void)
{
	
}
/**
  * @brief hal_pwm_stop
	* @param NONE
  * @retval None
  */
void hal_pwm_stop(void)
{
	
}
/**
  * @brief hal_tim_psc
	* @param NONE
  * @retval None
  */
void hal_tim_psc(unsigned int psc)
{
	/* check */
	if( osc_sample_mode == 0 )
	{
		TIM1->PSC = psc - 1;
	}
	else
	{
		TIM1->PSC = psc - 1;
		TIM3->PSC = psc - 1;
	}
}











