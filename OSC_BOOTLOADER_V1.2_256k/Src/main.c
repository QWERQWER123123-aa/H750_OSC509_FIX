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
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dev.h"
#include "gui.h"
/* USER CODE END Includes */
#include "boot.h"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* B 1.0.12 */
#define BLVH  'V'
#define BLV0  (1)
#define BLV1  (0)
#define BLV2  (0) 
/* transfer */
#define SW_VERSION_ALONE ((BLVH<<24)|(BLV0<<16)|(BLV1<<8)|(BLV2))
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const unsigned int __FS_version_export[3] = {0xeabc2547,SW_VERSION_ALONE,0x3526ec88};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__attribute__((at(0x24000000 + 800*600 ))) unsigned char usart_rx_buffer[2048*10];
__attribute__((at(0x24000000 + 800*600 + 2048*10))) unsigned int boot_holder;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_DMA_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */
static unsigned int boot_flag = 0;
/* Private user code ---------------------------------------------------------*/
//void boot_mode(unsigned int fe)
//{
//	boot_flag = fe;
//}
/* USER CODE BEGIN 0 */
static void delay_key(unsigned int t)
{
	while(t--);
}
/* USER CODE END 0 */

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_DMA_Init();
//  MX_USART1_UART_Init();
	/* delay */	
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
	/* dev init */
	dev_init();
	/* create */
	create_boot_ui();
  /* USER CODE END 2 */
	console_out("开机自检中...");
	/* bootloader mode */
  console_out("BootLoder模式:");
	/* show bootloader version */
	char buffer[32];//buffer 
	/* create buffer */
  
	sprintf(buffer,"版本: %c%d.%d.%d",(char)(__FS_version_export[1]>>24),(unsigned char)(__FS_version_export[1]>>16),
		                                          (unsigned char)(__FS_version_export[1]>>8),(unsigned char)(__FS_version_export[1]));
	/* show */
	console_out(buffer);
	/* init usb dnd */
	usb_dnd_flash_init();
	/* check APP code */
	console_out("校验应用程序...");
	/* check */
	unsigned int appversion = app_code_check();
	/* check */
	if( appversion != 0 )
	{
		/* create buffer */
		sprintf(buffer,"校验成功!应用程序版本: %c%d.%d.%d",(char)(appversion>>24),(unsigned char)(appversion>>16),
																							(unsigned char)(appversion>>8),(unsigned char)(appversion));
		/* show */
		console_out(buffer); 	 
	}
	else
	{
		console_out("校验失败");
	}
	/* check ok */
	show_process_des();
	/* hide */
	/* check KEY */
	/* check PE8 */
	if( !(GPIOE->IDR & ( 1 << 8)) )
	{
		/* delay */
		delay_key(100000);
		/* read again */
		if( !(GPIOE->IDR & ( 1 << 8)) )
		{
			boot_flag = 1;
		}
	}	
	hide_process();
	/* clear */
	clear_screen(1);
  /* Infinite loop */
	if( appversion != 0 && boot_flag == 0 && boot_holder != 0xaef672ba )
	{
		JumpToapp();
	}
	else
	{
		/* shwo */
		console_out("等待USB连接中......");
	}
	/* release */
	//while((GPIOC->IDR & ( 1 << 1)))
	//{
		/* nothing to do */
	//}
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		//boot_loader_thread();
		usb_dnd_flash_thread();
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 144;
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
  * Enable DMA controller clock
  */
//static void MX_DMA_Init(void) 
//{

//  /* DMA controller clock enable */
//  __HAL_RCC_DMA1_CLK_ENABLE();

////  /* DMA interrupt init */
////  /* DMA1_Stream0_IRQn interrupt configuration */
////  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
////  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
////  /* DMA1_Stream1_IRQn interrupt configuration */
////  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
////  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

//}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

 	/* PE8 KEY IN */
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*SDIO*/
  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*Configure GPIO pin : PB3,PB4,PB14,PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*Configure GPIO pin : PC1 ,PC14*/
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	/* KEY IN */
//	
//	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_3;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	/*  KEY IN */
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_1;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//	GPIO_InitStruct.Pin = GPIO_PIN_6;
//	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* ADDR */
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);//A
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);//b
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);//c
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
