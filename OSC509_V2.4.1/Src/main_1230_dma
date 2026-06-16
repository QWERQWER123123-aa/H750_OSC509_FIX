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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fos.h"
//#include "boot_rom.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "stdio.h"
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */
/* USER CODE BEGIN PTD */
/* B 1.0.12 */
#define BLVH  'T'
#define BLV0  (1)
#define BLV1  (0)
#define BLV2  (13)
/* transfer */
#define SW_VERSION_ALONE ((BLVH<<24)|(BLV0<<16)|(BLV1<<8)|(BLV2))
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const unsigned int __FS_version_export[3] = {0xfabc2747,SW_VERSION_ALONE,0xb546ee89};
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static char bufferv[16];//buffer 
/* USER CODE END PFP */
char * app_version(void)
{
	/* create version */
	sprintf(bufferv,"%c%d.%d.%d",(char)(__FS_version_export[1]>>24),(unsigned char)(__FS_version_export[1]>>16),
		                           (unsigned char)(__FS_version_export[1]>>8),(unsigned char)(__FS_version_export[1]));	
	return bufferv;
}
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ENABLE_INT()   __set_PRIMASK(0)
#define DISABLE_INT()  __set_PRIMASK(1)
/* JumpToBootloader */
void JumpToBootloader(void)
{
	/* define */
	unsigned int i=0;
	void (*SysMemBootJump)(void); 
	__IO unsigned int BootAddr = 0x1FF09800; /* STM32H7  BootLoader */

	/* DISABLE INT */
	DISABLE_INT();

	/* SYSTICK DISABLE */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
  /* RCC DeInit */
	HAL_RCC_DeInit();
  /* disable all it */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}
  /* enable int */
	ENABLE_INT();

	/* bootLoader MSP + 4 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));
	/* set MSP */
	__set_MSP(*(unsigned int *)BootAddr);
	__set_CONTROL(0);
  /* jump */
	SysMemBootJump();
	/* never arrival here */
	while (1)
	{
		
	}
}
/* disable all it */
void osc_disable_all_it(void)
{
	/* DISABLE INT */
	DISABLE_INT();	
	/* disable all it */
	for( int i = 0; i < 8; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	}
}

unsigned short *IO_Toggle = (unsigned short *)0x30000000;
unsigned short *IO_Toggle1 = (unsigned short *)(0x30000000+10000);
unsigned short *IO_Toggle2 = (unsigned short *)(0x30000000+20000);
unsigned short *IO_Toggle3 = (unsigned short *)(0x30000000+30000);
/* USER CODE END 0 */
void TIM12_Config(uint8_t _Mode)
{
	TIM_HandleTypeDef  htim ={0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfig = {0};
	uint32_t Period[2] = {4, 19999};
	uint32_t Pulse[2]  = {2, 9999};

	__HAL_RCC_TIM12_CLK_ENABLE();

	/*-----------------------------------------------------------------------
	TIM12CLK = 200MHz/(Period + 1) / (Prescaler + 1)
	----------------------------------------------------------------------- */  
	HAL_TIM_Base_DeInit(&htim);

	htim.Instance = TIM12;
	htim.Init.Period            = Period[_Mode];
	htim.Init.Prescaler         = 0;
	htim.Init.ClockDivision     = 0;
	htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&htim);

	sConfig.OCMode     = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_LOW;

	/* 占空比50% */
	sConfig.Pulse = Pulse[_Mode];  
	if(HAL_TIM_OC_ConfigChannel(&htim, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	{
	//Error_Handler(__FILE__, __LINE__);
	}

	/* 启动OC1 */
	if(HAL_TIM_OC_Start(&htim, TIM_CHANNEL_1) != HAL_OK)
	{
	//Error_Handler(__FILE__, __LINE__);
	}

	/* TIM12的TRGO用于触发DMAMUX的请求发生器 */
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1REF;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);
}
void bsp_InitTimDMA(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    DMA_HandleTypeDef DMA_Handle = {0};
    HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams = {0};


    __HAL_RCC_GPIOD_CLK_ENABLE();
		
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
    __HAL_RCC_DMA1_CLK_ENABLE();
		
    DMA_Handle.Instance                 = DMA1_Stream1;            
    DMA_Handle.Init.Request             = DMA_REQUEST_GENERATOR0;   
    DMA_Handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;      
    DMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;         
    DMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;           
    DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;        
    DMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;       
    DMA_Handle.Init.Mode                = DMA_CIRCULAR;            
    DMA_Handle.Init.Priority            = DMA_PRIORITY_HIGH;         
    DMA_Handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;    
    DMA_Handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL; 
    DMA_Handle.Init.MemBurst            = DMA_MBURST_SINGLE;
    DMA_Handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;
 
    /* 初始化DMA */
    if(HAL_DMA_Init(&DMA_Handle) != HAL_OK)
    {
		 //Error_Handler(__FILE__, __LINE__);     
    }

    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn); 

   
    dmamux_ReqGenParams.SignalID  = HAL_DMAMUX1_REQ_GEN_TIM12_TRGO; 
    dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;    
    dmamux_ReqGenParams.RequestNumber = 1;

    HAL_DMAEx_ConfigMuxRequestGenerator(&DMA_Handle, &dmamux_ReqGenParams); 
    HAL_DMAEx_EnableMuxRequestGenerator (&DMA_Handle);
 

    HAL_DMAEx_MultiBufferStart_IT(&DMA_Handle, (uint32_t)&GPIOD->IDR,(uint32_t)IO_Toggle,(uint32_t)IO_Toggle1, 5000);
  
    //DMA1_Stream1->CR &= ~DMA_IT_DME; 
    //DMA1_Stream1->CR &= ~DMA_IT_TE;
    //DMAMUX1_RequestGenerator0->RGCR &= ~DMAMUX_RGxCR_OIE;
    
    
}
void bsp_InitTimDMA3(void)
{
//    GPIO_InitTypeDef  GPIO_InitStruct;
    DMA_HandleTypeDef DMA_Handle = {0};
    HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams = {0};


//    __HAL_RCC_GPIOD_CLK_ENABLE();
//		
//    GPIO_InitStruct.Pin = GPIO_PIN_All;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//		
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
    __HAL_RCC_DMA1_CLK_ENABLE();
		
    DMA_Handle.Instance                 = DMA1_Stream2;            
    DMA_Handle.Init.Request             = DMA_REQUEST_GENERATOR1;   
    DMA_Handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;      
    DMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;         
    DMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;           
    DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;        
    DMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;       
    DMA_Handle.Init.Mode                = DMA_CIRCULAR;            
    DMA_Handle.Init.Priority            = DMA_PRIORITY_HIGH;         
    DMA_Handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;    
    DMA_Handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL; 
    DMA_Handle.Init.MemBurst            = DMA_MBURST_SINGLE;
    DMA_Handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;
 
    /* 初始化DMA */
    if(HAL_DMA_Init(&DMA_Handle) != HAL_OK)
    {
		 //Error_Handler(__FILE__, __LINE__);     
    }

    HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn); 

   
    dmamux_ReqGenParams.SignalID  = HAL_DMAMUX1_REQ_GEN_TIM12_TRGO; 
    dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_FALLING;    
    dmamux_ReqGenParams.RequestNumber = 1;

    HAL_DMAEx_ConfigMuxRequestGenerator(&DMA_Handle, &dmamux_ReqGenParams); 
    HAL_DMAEx_EnableMuxRequestGenerator (&DMA_Handle);
 

    HAL_DMAEx_MultiBufferStart_IT(&DMA_Handle, (uint32_t)&GPIOD->IDR,(uint32_t)IO_Toggle2,(uint32_t)IO_Toggle3, 5000);

}
volatile unsigned int t0 ,t1 , t2 ,t3,t4,t5,t6,t7;
extern unsigned int hal_sys_time_us(void);

void DMA1_Stream1_IRQHandler(void)
{

	if((DMA1->LISR & DMA_FLAG_TCIF1_5) != RESET)
	{
		
		DMA1->LIFCR = DMA_FLAG_TCIF1_5;

		
		if((DMA1_Stream1->CR & DMA_SxCR_CT) == RESET)
		{
			
			t0 = hal_sys_time_us() - t1;
			
			t1 = hal_sys_time_us();			
			
		}
		else
		{

		}
	
	}

	
	if((DMA1->LISR & DMA_FLAG_HTIF1_5) != RESET)
	{

		t2 = hal_sys_time_us() - t3;
		
		t3 = hal_sys_time_us();
		
		DMA1->LISR = DMA_FLAG_HTIF1_5;
	}

	
	if((DMA1->LISR & DMA_FLAG_TEIF1_5) != RESET)
	{
		
		DMA1->LISR = DMA_FLAG_TEIF1_5;
	}

	
	if((DMA1->LISR & DMA_FLAG_DMEIF1_5) != RESET)
	{
		
		DMA1->LISR = DMA_FLAG_DMEIF1_5;
	}
}

void DMA1_Stream2_IRQHandler(void)
{

	if((DMA1->LISR & DMA_FLAG_TCIF2_6) != RESET)
	{
		
		DMA1->LIFCR = DMA_FLAG_TCIF2_6;

		
		if((DMA1_Stream2->CR & DMA_SxCR_CT) == RESET)
		{
			
			t4 = hal_sys_time_us() - t5;
			
			t5 = hal_sys_time_us();			
			
		}
		else
		{

		}
	
	}

	
	if((DMA1->LISR & DMA_FLAG_HTIF2_6) != RESET)
	{

		t6 = hal_sys_time_us() - t7;
		
		t7 = hal_sys_time_us();
		
		DMA1->LISR = DMA_FLAG_HTIF2_6;
	}

	
	if((DMA1->LISR & DMA_FLAG_TEIF2_6) != RESET)
	{
		
		DMA1->LISR = DMA_FLAG_TEIF2_6;
	}

	
	if((DMA1->LISR & DMA_FLAG_DMEIF2_6) != RESET)
	{
		
		DMA1->LISR = DMA_FLAG_DMEIF2_6;
	}
}

void DMAMUX1_OVR_IRQHandler(void)
{
    if((DMAMUX1_RequestGenStatus->RGSR & DMAMUX_RGSR_OF0) != RESET)
    {
      
       DMAMUX1_RequestGenerator0->RGCR &= ~DMAMUX_RGxCR_OIE;
       
       
       DMAMUX1_RequestGenStatus->RGCFR = DMAMUX_RGSR_OF0;
    }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	/* create buffer */
	
  /* USER CODE END SysInit */
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	fs_system_initialization();
  /* USER CODE END 2 */
  bsp_InitTimDMA();
	bsp_InitTimDMA3();
	TIM12_Config(0);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		run_thead_priority_idle();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	
  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source 
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable USB Voltage detector 
  */
  HAL_PWREx_EnableUSBVoltageDetector();	
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
