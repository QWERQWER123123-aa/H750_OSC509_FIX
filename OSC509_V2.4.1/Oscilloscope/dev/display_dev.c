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
#include "display_dev.h"
#include "main.h"
#include "middle.h"
#include "fos.h"
#include "osc.h"
#include "string.h"
#include "osc_win.h"
/* Private includes ----------------------------------------------------------*/
#if LCD_MODE_L8
unsigned char *gram  = ( unsigned char * )(0x24000000);
#else
unsigned short gram[800*480*3] __attribute__((at(Bank5_SDRAM_ADDR)));
#endif
/* color table */
unsigned int osc_color_table[256] = COLOR_TABLE_L8;
/* sonset color table l2 */
unsigned int osc_color_table_l2[256] = COLOR_TABLE_L2L8;
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("dev_init",dev_init,0,0,5);
/* set */
static LTDC_HandleTypeDef hltdc;
extern int osc_msg_box(unsigned char index);
/* Define all supported display panel information */
const display_dev_def display_dev[7] = 
{
	{
		.dev_capital = "LCD\n480*272",
		.pwidth = 480 ,
		.pheight = 272,
		.win_width = 480,
		.win_pheight = 272,		
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
		.win_width = 800,
		.win_pheight = 480,		
		.hsw = 1,
		.vsw = 1,
		.hbp = 46,
		.vbp = 250,
		.hfp = 46,
		.vfp = 30,
		/* rcc clock settings */
		.PLLSAIN = 396,
		.PLLSAIR = 3 , 
		.PLLSAIDIVR = DIVR_4,
		/* pixel clock */
		.pixel_clk = 30,
	},
	{
		.dev_capital = "LCD\n800*480",
		.pwidth = 800 ,
		.pheight = 480 ,
		.win_width = 800,
		.win_pheight = 480,		
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
		.pixel_clk = 18,
	},
	{
		.dev_capital = "VGA\n800*600",
		.pwidth = 800,
		.pheight = 480,
		.win_width = 800,
		.win_pheight = 600,
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
		.win_width = 640,
		.win_pheight = 480,		
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
const display_dev_def * display_info_s;
/* information of current display dev */
static display_info_def display_info;
/* init flags */
static unsigned char init_dev_flags = 0;
extern unsigned char LCD_INCH;
/* init dev */
static int dev_init(void)
{
	/* read data from eeprom and config */
	if( LCD_INCH == 7 )
	{
		/* inch 7 lcd */
		display_info_s = &display_dev[1];
	}
	else if( LCD_INCH == 5 )
	{
		/* VGA or inch 5 */
		display_info_s = &display_dev[1];		
	}
	else if( LCD_INCH == 6 )
	{
		/* VGA 800 * 600 */
		display_info_s = &display_dev[3];				
	}
	else
	{
		/* default as */
		osc_msg_box(24);
		/* VGA or inch 5 */
		display_info_s = &display_dev[1];			
	}
	/* pointer to curren dev */
	display_info.display_dev = display_info_s;
	/* gram */
	display_info.gram_addr = (unsigned int)gram;
	display_info.gram_l2 = &gram[ display_info_s->pwidth * display_info_s->pheight ];
	/* has inited or not */
	if( init_dev_flags == 0 )
	{
		/* set up flag */
		init_dev_flags = 1;
		/* init ltdc init or other dev */
		LTDC_Init(display_info.display_dev);
		/* init middle */
		middle_layer_init(&display_info);
		/* end of if */
	}
	/* return INIT OK */
	return FS_OK;
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
		middle_layer_init(&display_info);
		/* end of if */
	}
	/* return */
	return &display_info;
}
/* get color table */
const unsigned int * get_color_table(void)
{
	return osc_color_table;
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
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6;
	/* VGA */
	if( LCD_INCH == 6 )
	{
		GPIO_InitStruct.Pin |= GPIO_PIN_4;
	}
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Alternate = GPIO_AF13_LTDC;
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

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10; 
	/* VGA */
	if( LCD_INCH == 6 )
	{
		GPIO_InitStruct.Pin |= GPIO_PIN_6;
	}	
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* USER CODE BEGIN LTDC_MspInit 1 */
	/* USER CODE END LTDC_MspInit 1 */
}
/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void LTDC_Init(const display_dev_def * info)
{
  /* USER CODE BEGIN LTDC_Init 0 */
	
  /* USER CODE END LTDC_Init 0 */
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  LTDC_LayerCfgTypeDef pLayerCfg = {0};
	LTDC_LayerCfgTypeDef pLayerCfg2 = {0};
  /* USER CODE BEGIN LTDC_Init 1 */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
#if OSC_STM32H750
  PeriphClkInitStruct.PLL3.PLL3M = 25;
	/* check LCD */
	if( LCD_INCH == 7 )
	{
		PeriphClkInitStruct.PLL3.PLL3N = info->pixel_clk * 2;
		PeriphClkInitStruct.PLL3.PLL3P = 2;
		PeriphClkInitStruct.PLL3.PLL3Q = 2;
		PeriphClkInitStruct.PLL3.PLL3R = 2;	
	}
	else
	{
		PeriphClkInitStruct.PLL3.PLL3N = info->pixel_clk * 2;
		PeriphClkInitStruct.PLL3.PLL3P = 2;
		PeriphClkInitStruct.PLL3.PLL3Q = 2;
		PeriphClkInitStruct.PLL3.PLL3R = 2;		
	}
#else
  PeriphClkInitStruct.PLL3.PLL3M = 5;
	/* check LCD */
	if( LCD_INCH == 7 )
	{
		PeriphClkInitStruct.PLL3.PLL3N = info->pixel_clk;
		PeriphClkInitStruct.PLL3.PLL3P = 2;
		PeriphClkInitStruct.PLL3.PLL3Q = 2;
		PeriphClkInitStruct.PLL3.PLL3R = 5;	
	}
	else
	{
		PeriphClkInitStruct.PLL3.PLL3N = info->pixel_clk;
		PeriphClkInitStruct.PLL3.PLL3P = 2;
		PeriphClkInitStruct.PLL3.PLL3Q = 2;
		PeriphClkInitStruct.PLL3.PLL3R = 5;		
	}	
#endif
	/* init */
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
	hltdc.Init.AccumulatedActiveH = info->vsw+info->vbp+info->win_pheight - 1;
	hltdc.Init.TotalWidth = info->hsw+info->hbp+info->pwidth+info->hfp - 1;  
	hltdc.Init.TotalHeigh = info->vsw+info->vbp+info->win_pheight+info->vfp - 1;  
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
#if LCD_MODE_L8
	/* transfer */
	unsigned char r,g,b,rt,gt,bt;
	/* transfer */
	for(int i = 0 ; i < 256 ; i ++ )
	{
		/* layer 0 */
		r = osc_color_table[i] >> 16;
		g = osc_color_table[i] >> 8;
		b = osc_color_table[i];
		/* check */
		rt = r & 0xFE;
		gt = g & 0xFE;
		bt = b & 0xFE;
		/* transfer R4 to R1 , G7 to G0 */
		gt |= ( gt & ( 1 << 7 ) ) ? ( 1 << 0 ) : 0;
		rt |= ( rt & ( 1 << 4 ) ) ? ( 1 << 1 ) : 0;			
    /* set */
		osc_color_table[i] = ( rt << 16 ) | ( gt << 8 ) | bt;
		/* layer 1 */
		r = osc_color_table_l2[i] >> 16;
		g = osc_color_table_l2[i] >> 8;
		b = osc_color_table_l2[i];
		/* check */
		rt = r & 0xFE;
		gt = g & 0xFE;
		bt = b & 0xFE;
		/* transfer R4 to R1 , G7 to G0 */
		gt |= ( gt & ( 1 << 7 ) ) ? ( 1 << 0 ) : 0;
		rt |= ( rt & ( 1 << 4 ) ) ? ( 1 << 1 ) : 0;			
    /* set */
		osc_color_table_l2[i] = ( rt << 16 ) | ( gt << 8 ) | bt;		
	}	
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
	/* config layer2 */
  pLayerCfg2.WindowX0 = 0;
  pLayerCfg2.WindowX1 = 100;
  pLayerCfg2.WindowY0 = 0;
  pLayerCfg2.WindowY1 = 100;
#if LCD_MODE_L8
	pLayerCfg2.PixelFormat = LTDC_PIXEL_FORMAT_L8;//250
#else	
  pLayerCfg2.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
#endif
  pLayerCfg2.Alpha = 0;
  pLayerCfg2.Alpha0 = 0;
  pLayerCfg2.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg2.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	pLayerCfg2.FBStartAdress = (unsigned int)&gram[info->pwidth*info->pheight];
  pLayerCfg2.ImageWidth = 100;
  pLayerCfg2.ImageHeight = 100;
  pLayerCfg2.Backcolor.Blue = 0;
  pLayerCfg2.Backcolor.Green = 0;
  pLayerCfg2.Backcolor.Red = 0;
	/* init ok or not */
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg2, 1) != HAL_OK)
  {
    Error_Handler();
  }	
	/* config the CLUT and ENABLE */
	HAL_LTDC_ConfigCLUT(&hltdc,(unsigned int * )osc_color_table_l2,256,LTDC_LAYER_2);
	/* enable */
	HAL_LTDC_EnableCLUT(&hltdc,LTDC_LAYER_2);		
	/* enable it */
	HAL_LTDC_ProgramLineEvent(&hltdc,info->pheight + info->vfp);
	/* enable it */
	HAL_NVIC_SetPriority(LTDC_IRQn, 4, 1);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);	
  /* USER CODE END LTDC_Init 2 */
}
/* set layer2 */
void osc_dev_l2_enable(unsigned short x0,unsigned short y0,unsigned short x0_size,unsigned short y0_size,unsigned char alpha)
{
	/* set position */
	HAL_LTDC_SetWindowPosition(&hltdc,x0,y0,LTDC_LAYER_2);
	/* set size */
	HAL_LTDC_SetWindowSize(&hltdc,x0_size,y0_size,LTDC_LAYER_2);
	/* set alpha */
	/* should create a thread to dons */
	HAL_LTDC_SetAlpha(&hltdc,alpha,LTDC_LAYER_2);
	/* able */
}
/* pus */
void osc_dev_l2_alph(unsigned char alph)
{
	/* set alpha */
	HAL_LTDC_SetAlpha(&hltdc,alph,LTDC_LAYER_2);
	/* should be create a thread */	
}
/* set layer2 disable */
void osc_dev_l2_disable(void)
{
	/* set alpha */
	HAL_LTDC_SetWindowSize(&hltdc,1,1,LTDC_LAYER_2);
	/* disable */	
	HAL_LTDC_SetAlpha(&hltdc,0,LTDC_LAYER_2);
	/* should be create a thread */
}
/* get buffer size */
unsigned int osc_dev_resize(void)
{
  /* get reload size */
	return 512*1024 - display_info_s->pheight * display_info_s->pwidth;
}
/* da */
extern int osc_msg_ltdc_isr(void);
/* IRQ */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */
	__HAL_LTDC_CLEAR_FLAG(&hltdc, LTDC_FLAG_LI);
  /* USER CODE END LTDC_IRQn 0 */
  /* USER CODE BEGIN LTDC_IRQn 1 */
	osc_msg_ltdc_isr();
  /* USER CODE END LTDC_IRQn 1 */
}
/* ------------------------------ end of file ------------------------------ */


































