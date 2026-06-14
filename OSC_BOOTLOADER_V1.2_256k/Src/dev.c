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
#include "dev.h"
#include "main.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
#if LCD_MODE_L8
unsigned char gram[800*600] __attribute__((at(0x24000000)));
#else
unsigned short gram[800*480*3] __attribute__((at(Bank5_SDRAM_ADDR)));
#endif
/* Define all supported display panel information */
const display_dev_def display_dev[7] = 
{
	{
		.dev_capital = "LCD\n480*272",
		.pwidth = 480 ,
		.pheight = 272,
		.hsw = 1,
		.vsw = 1,
		.hbp = 40,
		.vbp = 8,
		.hfp = 5,
		.vfp = 8,
		/* rcc clock settings */
		.PLLSAIN = 288,
		.PLLSAIR = 4 , 
		.PLLSAIDIVR = DIVR_8,
		.pixel_clk = 9,
	},
	{
		.dev_capital = "LCD\n800*480",
		.pwidth = 800 ,
		.pheight = 480 ,
		.hsw = 1,
		.vsw = 1,
		.hbp = 46,
		.vbp = 23,
		.hfp = 210,
		.vfp = 22,
		/* rcc clock settings */
		.PLLSAIN = 396,
		.PLLSAIR = 3 , 
		.PLLSAIDIVR = DIVR_4,
		/* pixel clock */
		.pixel_clk = 33,
	},
	{
		.dev_capital = "LCD\n1024*600",
		.pwidth = 1024 ,
		.pheight = 600,
		.hsw = 20,
		.vsw = 3,
		.hbp = 140,
		.vbp = 20,
		.hfp = 160,
		.vfp = 12,
		/* rcc clock settings */
		.PLLSAIN = 50 * 4,
		.PLLSAIR = 2 , 
		.PLLSAIDIVR = DIVR_2,
		/* pixel clock */
		.pixel_clk = 50,		
	},
	{
		.dev_capital = "VGA\n1280*800",
    .pwidth = 1280,
		.pheight = 800,
		.hsw = 136,
		.hbp = 200,
		.hfp = 64,
		.vsw = 3,
		.vbp = 24,
		.vfp = 1,
		/* rcc clock settings */
		.PLLSAIN = 83 * 4,
		.PLLSAIR = 2 , 
		.PLLSAIDIVR = DIVR_2,
		/* pixel clock */
		.pixel_clk = 83,		
	},
	{
		.dev_capital = "VGA\n1024*768",
		.pwidth = 1024,
		.pheight = 768,
		.hsw = 136,
		.hbp = 160,
		.hfp = 24,
		.vsw = 6,
		.vbp = 29,
		.vfp = 3,
		/* rcc clock settings */
		.PLLSAIN = 65 * 4,
		.PLLSAIR = 2 , 
		.PLLSAIDIVR = DIVR_2, /* 65MHZ @ 60HZ*/
		/* pixel clock */
		.pixel_clk = 65,		
	},	
	{
		.dev_capital = "VGA\n800*600",
		.pwidth = 800,
		.pheight = 600,
		.hsw = 128,
		.hbp = 88,
		.hfp = 40,
		.vsw = 4,
		.vbp = 23,
		.vfp = 1,
		/* rcc clock settings */
		.PLLSAIN = 40 * 4,
		.PLLSAIR = 2 , 
		.PLLSAIDIVR = DIVR_2,/* 40MHZ @ 60HZ */
		/* pixel clock */
		.pixel_clk = 40,		
	},	
	{
		.dev_capital = "VGA\n640*480",
    .pwidth = 640,
		.pheight = 480,
		.hsw = 96,
		.hbp = 48,		
		.hfp = 16,
		.vsw = 2,
		.vbp = 33,
		.vfp = 10,
		/* rcc clock settings */
		.PLLSAIN = 25 * 4,
		.PLLSAIR = 2 , 
		.PLLSAIDIVR = DIVR_2,/* ~25M @ 60HZ */
		/* pixel clock */
		.pixel_clk = 25,		
	},	
#if 0
	{
		.dev_capital = "VGA\n848*480",
		.pwidth = 848,
		.pheight = 480,
		.hsw = 112,
		.hbp = 112,
		.hfp = 16,
		.vsw = 8,
		.vbp = 23,
		.vfp = 6,
		/* rcc clock settings */
		.PLLSAIN = 0,
		.PLLSAIR = 0 , 
		.PLLSAIDIVR = DIVR_4,
		/* pixel clock */
		.pixel_clk = 33,	
	},
#endif	
#if 0
	{
		.dev_capital = "LCD\n848*480",
		.pwidth = 848 ,
		.pheight = 480 ,
		.hsw = 1,
		.vsw = 1,
		.hbp = 46,
		.vbp = 23,
		.hfp = 210,
		.vfp = 22,
		/* rcc clock settings */
		.PLLSAIN = 396,
		.PLLSAIR = 3 , 
		.PLLSAIDIVR = DIVR_4,
		/* pixel clock */
		.pixel_clk = 33,		
	},
#endif		
#if 0	
	{
		.dev_capital = "VGA\n1366*768",
		.pwidth = 1366,
		.pheight = 768,
		.hsw = 143,
		.hfp = 70,
		.hbp = 213,
		.vsw = 48,
		.vbp = 24,
		.vfp = 3,
		/* rcc clock settings */
		.PLLSAIN  =  0,
		.PLLSAIR = 0 , 
		.PLLSAIDIVR = DIVR_4,
		/* pixel clock */
		.pixel_clk = 33,		
	},
#endif		
#if 0
	{
		.dev_capital = "VGA\n1280*768",
		.pwidth = 1280,
		.pheight = 768,
		.hsw = 32,
		.hbp = 80,
		.hfp = 48,
		.vsw = 7,
		.vbp = 12,
		.vfp = 3,
		/* rcc clock settings */
		.PLLSAIN = 0,
		.PLLSAIR = 0 , 
		.PLLSAIDIVR = DIVR_4,
		/* pixel clock */
		.pixel_clk = 33,		
	},
#endif	
};
/* information of display dev */
static display_dev_def * display_info_s = (display_dev_def *)&display_dev[1]; /* default setting is 800*480 for test */ 
/* information of current display dev */
static display_info_def display_info;
/* init flags */
static unsigned char init_dev_flags = 0;
/* init dev */
int dev_init(void)
{
	/* read data from eeprom and config */
	
	/* pointer to curren dev */
	display_info.display_dev = display_info_s;
	/* gram */
	display_info.gram_addr = (unsigned int)gram;
	/* has inited or not */
	if( init_dev_flags == 0 )
	{
		/* set up flag */
		init_dev_flags = 1;
		/* init ltdc init or other dev */
		LTDC_Init(display_info.display_dev);
		/* init middle */ 
		clear_screen(1);
		/* end of if */
	}
	/* return INIT OK */
	return 0;
}
/* get information for the display */
display_info_def * get_display_dev_info(void)
{
	/* has inited or not */
	if( init_dev_flags == 0 )
	{
		/* set up flag */
		init_dev_flags = 1;
		/* init ltdc init or other dev */
		LTDC_Init(display_info.display_dev);
		/* init middle */
		clear_screen(1);
		/* end of if */
	}
	/* return */
	return &display_info;
}
/* set up information for the display at circle mode */
char * set_display_dev(unsigned short index)
{
	/* get num */
	unsigned int num = sizeof(display_dev_def) / sizeof(display_dev[0]) ;
	/* set ltdc */
	display_info_s = (display_dev_def *)&display_dev[ index % num ];
	/* reset LTDC */
	return display_info_s->dev_capital;
	/* end of function */
}
/**
* @brief LTDC MSP Initialization
* This function configures the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
static void LTDC_MspInit(void)
{
	/* default dev */
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* USER CODE BEGIN LTDC_MspInit 0 */

	/* USER CODE END LTDC_MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_LTDC_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/**LTDC GPIO Configuration    
	PI9     ------> LTDC_VSYNC
	PI10     ------> LTDC_HSYNC
	PF10     ------> LTDC_DE
	PH9     ------> LTDC_R3
	PH10     ------> LTDC_R4
	PH11     ------> LTDC_R5
	PH12     ------> LTDC_R6
	PG6     ------> LTDC_R7
	PG7     ------> LTDC_CLK
	PH13     ------> LTDC_G2
	PH14     ------> LTDC_G3
	PH15     ------> LTDC_G4
	PI0     ------> LTDC_G5
	PI1     ------> LTDC_G6
	PI2     ------> LTDC_G7
	PG11     ------> LTDC_B3
	PI4     ------> LTDC_B4
	PI5     ------> LTDC_B5
	PI6     ------> LTDC_B6
	PI7     ------> LTDC_B7 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6;
											
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_7; 
	
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* USER CODE BEGIN LTDC_MspInit 1 */
	/* USER SET LCD DISPLAY AND BL */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* Display on */
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET);
	
	/*set up BL pin */
	GPIO_InitStruct.Pin = GPIO_PIN_6 ;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/* SET back light aways on */
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);	
	/* USER CODE END LTDC_MspInit 1 */
}
/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void LTDC_Init(display_dev_def * info)
{
  /* USER CODE BEGIN LTDC_Init 0 */
	
  LTDC_HandleTypeDef hltdc;
	
  /* USER CODE END LTDC_Init 0 */
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = info->pixel_clk * 2;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
	/* init */
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }		
	/* gpio */
  LTDC_MspInit();
  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc.Init.HorizontalSync = info->hsw - 1;          
	hltdc.Init.VerticalSync = info->vsw - 1;            
	hltdc.Init.AccumulatedHBP = info->hsw+info->hbp - 1; 
	hltdc.Init.AccumulatedVBP = info->vsw+info->vbp - 1; 
	hltdc.Init.AccumulatedActiveW = info->hsw+info->hbp+info->pwidth - 1;
	hltdc.Init.AccumulatedActiveH = info->vsw+info->vbp+info->pheight - 1;
	hltdc.Init.TotalWidth = info->hsw+info->hbp+info->pwidth+info->hfp - 1;  
	hltdc.Init.TotalHeigh = info->vsw+info->vbp+info->pheight+info->vfp - 1;  
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
#if LCD_MODE_L8
	const unsigned int osc_color_table[256] = COLOR_TABLE_L8;
	/* config the CLUT and ENABLE */
	HAL_LTDC_ConfigCLUT(&hltdc,(unsigned int * )osc_color_table,256,LTDC_LAYER_1);
	/* enable */
	HAL_LTDC_EnableCLUT(&hltdc,LTDC_LAYER_1);	
	/* end */
#endif
	/* init ok or not */
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
	/* config layer */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = info->pwidth;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = info->pheight;
#if LCD_MODE_L8
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
#else	
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
#endif
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = (unsigned int)gram;
  pLayerCfg.ImageWidth = info->pwidth;;
  pLayerCfg.ImageHeight = info->pheight;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
	/* init ok or not */
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END LTDC_Init 2 */
}
/* ------------------------------ end of file ------------------------------ */
/* middle functions set point */
void set_point( unsigned short * mark , unsigned short x , unsigned short y , unsigned int color )
{
	/* get GRAM */
	if( mark[0] == 0 )
	{
		gram[ y * display_info_s->pwidth + x ] = color;
	}
	else
	{
		/* check data */
		if( ( x >= mark[1] && x <= mark[3] ) && 
			  ( y >= mark[2] && y <= mark[4] ))
		{
			gram[ y * display_info_s->pwidth + x ] = color;
		}
	}
}
/* set noload point */
void set_noload_point( unsigned short x , unsigned short y , unsigned int color )
{
	/* get GRAM */
	gram[ y * display_info_s->pwidth + x ] = color;
}
/* middle get point */
unsigned short get_point( unsigned short x , unsigned short y )
{
	/* get GRAM */
	return gram[ y * display_info_s->pwidth + x ]; 
}
/* clear */
void clear_screen(unsigned int index)
{
	memset(gram,index,display_info_s->pheight * display_info_s->pwidth);
}
































