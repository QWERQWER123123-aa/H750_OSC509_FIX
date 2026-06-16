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
#include "fatfs.h"
#include "usb_device.h"
#include "osc_win.h"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "stdio.h"
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */
/* USER CODE BEGIN PTD */
/* B 1.0.12 */
#define BLVH  'V'
#define BLV0  (2)
#define BLV1  (4)
#define BLV2  (0)
/* transfer */
#define SW_VERSION_ALONE ((BLVH<<24)|(BLV0<<16)|(BLV1<<8)|(BLV2))
/* USER CODE END PTD */
extern void osc_feed_iwdg(void);
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const unsigned int __FS_version_export[3] = {0xfabc2747,SW_VERSION_ALONE,0xb546ee89};
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
FATFS fs;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "fatfs.h"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "stdio.h"
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern void MX_SDMMC2_SD_Init(void);
extern uint8_t BSP_SD_Init(void);
extern void MX_FATFS_Init(void);
//extern int boot_update_logic(void);
extern void MX_IWDG1_Init(void);
extern void MX_USB_DEVICE_Init(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static char bufferv[16];//buffer 
int sd_card_sta = 1;
/* USER CODE END PFP */
char * app_version(void)
{
	/* create version */
	sprintf(bufferv,"APP:%c%d.%d.%d",(char)(__FS_version_export[1]>>24),(unsigned char)(__FS_version_export[1]>>16),
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
	osc_feed_iwdg();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	//boot_update_logic();
  /* USER CODE END SysInit */
  osc_feed_iwdg();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	/* init SD */
	MX_SDMMC2_SD_Init();
	/* init FATFS */
	MX_FATFS_Init();
	/* mount SD */
	sd_card_sta = f_mount(&fs,"0:",1);
	/* system init (loads params from SD or Flash) */
	fs_system_initialization();
  /* USER CODE END 2 */	
	/* USB Device MSC init */
	if( osc_usb_device_enable() == 1 )
	{
		MX_USB_DEVICE_Init();
	}
	else
	{
		MX_USB_DEVICE_DeInit();
	}
  /* USER CODE BEGIN WHILE */
//	/* init */
	//MX_IWDG1_Init();
//	/* feed */
	osc_feed_iwdg();
	/* loop */
  while (1)
  {
    /* USER CODE END WHILE */
		run_thead_priority_idle();
    /* USER CODE BEGIN 3 */
		osc_feed_iwdg();
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if OSC_STM32H750
		RCC_OscInitStruct.PLL.PLLM = 5;
		RCC_OscInitStruct.PLL.PLLN = 160;
		RCC_OscInitStruct.PLL.PLLP = 2;
		RCC_OscInitStruct.PLL.PLLQ = 16;
		RCC_OscInitStruct.PLL.PLLR = 2;
#else
		RCC_OscInitStruct.PLL.PLLM = 5;
		RCC_OscInitStruct.PLL.PLLN = 80;
		RCC_OscInitStruct.PLL.PLLP = 2;
		RCC_OscInitStruct.PLL.PLLQ = 8;
		RCC_OscInitStruct.PLL.PLLR = 2;
#endif
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
#if OSC_STM32H750
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
#else
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
#endif	
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

#if OSC_STM32H750
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
#else
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
#endif
  {
    Error_Handler();
  }
	
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC|RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
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