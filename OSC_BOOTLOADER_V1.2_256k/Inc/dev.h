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

/* Private includes ----------------------------------------------------------*/

#ifndef __DISPLAY_DEV_H__
#define __DISPLAY_DEV_H__

/* define L8 mode or RGB mode */
#define LCD_MODE_L8    (1)
/* lcd and vga dev struct */
typedef struct
{
	/* Characters to display when setting */
	char * dev_capital;	
	/* 
		The default parameters may need to be set according to different screens. 
		Please refer to the data manual for details 
	*/
	unsigned short pwidth;
	unsigned short pheight;
	unsigned short hsw;
	unsigned short vsw;
	unsigned short hbp;
	unsigned short vbp;
	unsigned short hfp;
	unsigned short vfp;
	/* rcc clock settings */
	unsigned short PLLSAIN;
	unsigned short PLLSAIR;
	unsigned int PLLSAIDIVR;
	/* pixel clock */
	unsigned short pixel_clk;
	/* end */
}display_dev_def;
/* display info */
typedef struct 
{
	/* display dev */
	display_dev_def * display_dev;
	/* display gram addr */
	unsigned int gram_addr;
}display_info_def;
/* RCC_PLLSAIDIVR */
#define DIVR_2                0x00000000U
#define DIVR_4                0x00010000U
#define DIVR_8                0x00020000U
#define DIVR_16               0x00030000U
/* L8 mode color table */
#if LCD_MODE_L8
/* RGB color to L8 color */
#define RGTB(r,g,b)  ((r << 16) | (g << 8) | (b))
/* */
#define COLOR_GRID_POINT       (0)//(RGB(199, 195, 199))
#define COLOR_GRID_AREA_BG     (1)//(RGB(7, 3, 7))
#define COLOR_BACK_GROUND      (2)//(RGB(63, 75, 151))
/* menu win color table */
#define COLOR_MENU_ONE         (3)//(RGB(183, 83, 7))	/* color table */
/* button color table */
#define COLOR_BUTTON           (4)//(RGB(87,131,231))
/* char and hz table */
#define COLOR_CHAR             (5)//(RGB(255,255,255))
/* lines color */
#define COLOR_CH1              (6)//(RGB(255,255,7))
#define COLOR_CH2              (7)//(RGB(7,227,231))
/* TIPS_COLOR */
#define COLOR_TIPS_ERROR       (8)//(RGB(255,0,0))
/* CHN lines color */
#define COLOR_CH1_BG_0         (9)//(RGB(255,255,7))
#define COLOR_CH1_BG_1         (10)//(RGB(255,255,7))
#define COLOR_CH1_BG_F         (11)//(RGB(255,255,7))
#define COLOR_CH1_GR_0         (12)//(RGB(255,255,7))
#define COLOR_CH1_GR_1         (13)//(RGB(255,255,7))
#define COLOR_CH1_GR_F         (14)//(RGB(255,255,7))
/* CHN lines color */
#define COLOR_CH2_BG_0         (15)//(RGB(7,227,231))
#define COLOR_CH2_BG_1         (16)//(RGB(7,227,231))
#define COLOR_CH2_BG_F         (17)//(RGB(7,227,231))
#define COLOR_CH2_GR_0         (18)//(RGB(7,227,231))
#define COLOR_CH2_GR_1         (19)//(RGB(7,227,231))
#define COLOR_CH2_GR_F         (20)//(RGB(7,227,231))
/* math0 lines color */
#define COLOR_MATH0_BG_0       (21)//(RGB(255,0,0))
#define COLOR_MATH0_BG_1       (22)//(RGB(255,0,0))
#define COLOR_MATH0_BG_F       (23)//(RGB(255,0,0))
#define COLOR_MATH0_GR_0       (24)//(RGB(255,0,0))
#define COLOR_MATH0_GR_1       (25)//(RGB(255,0,0))
#define COLOR_MATH0_GR_F       (26)//(RGB(255,0,0))
/* math1 lines color */
#define COLOR_MATH1_BG_0       (27)//(RGB(0,255,0))
#define COLOR_MATH1_BG_1       (28)//(RGB(0,255,0))
#define COLOR_MATH1_BG_F       (29)//(RGB(0,255,0))
#define COLOR_MATH1_GR_0       (30)//(RGB(0,255,0))
#define COLOR_MATH1_GR_1       (31)//(RGB(0,255,0))
#define COLOR_MATH1_GR_F       (32)//(RGB(0,255,0))
/* math2 lines color */
#define COLOR_MATH2_BG_0       (33)//(RGB(0,0,255))
#define COLOR_MATH2_BG_1       (34)//(RGB(0,0,255))
#define COLOR_MATH2_BG_F       (35)//(RGB(0,0,255))
#define COLOR_MATH2_GR_0       (36)//(RGB(0,0,255))
#define COLOR_MATH2_GR_1       (37)//(RGB(0,0,255))
#define COLOR_MATH2_GR_F       (38)//(RGB(0,0,255))
/* trig lines */
#define COLOR_CH1_GR_TRIG      (39)//(RGB(7,227,231))
#define COLOR_CH1_BG_TRIG      (40)//(RGB(7,227,231))
#define COLOR_CH2_GR_TRIG      (41)//(RGB(255,255,7))
#define COLOR_CH2_BG_TRIG      (42)//(RGB(255,255,7))
/* diff pridata color */
#define COLOR_TEXT_ARROW_CH1   (43)//(RGB(7, 3, 7))
#define COLOR_TEXT_ARROW_CH2   (44)//(RGB(7, 3, 7))
/* typdef chn line manage */
typedef struct
{
	unsigned char BG_0;
	unsigned char BG_1;
	unsigned char BG_F;
	unsigned char GR_0;
	unsigned char GR_1;
	unsigned char GR_F;
}chn_manage_def;
/* color table */
#define COLOR_TABLE_L8         \
{                              \
RGTB(255, 255, 255),   /* 0  */\
RGTB(7, 3, 7),         /* 1  */\
RGTB(11, 112, 229),    /* 2  */\
RGTB(0, 0, 0),         /* 3  */\
RGTB(1,2,15),          /* 4  */\
RGTB(0,150,255),       /* 5  */\
RGTB(255,255,7),       /* 6  */\
RGTB(3,227,231),       /* 7  */\
RGTB(212, 212, 212),   /* 8  */\
RGTB(255,255,7),       /* 9  */\
RGTB(255,255,7),       /* 10 */\
RGTB(255,255,7),       /* 11 */\
RGTB(255,255,7),       /* 12 */\
RGTB(255,255,7),       /* 13 */\
RGTB(255,255,7),       /* 14 */\
RGTB(3,227,231),       /* 15 */\
RGTB(3,227,231),       /* 16 */\
RGTB(3,227,231),       /* 17 */\
RGTB(3,227,231),       /* 18 */\
RGTB(3,227,231),       /* 19 */\
RGTB(3,227,231),       /* 20 */\
RGTB(255,0,0),         /* 21 */\
RGTB(255,0,0),         /* 22 */\
RGTB(255,0,0),         /* 23 */\
RGTB(255,0,0),         /* 24 */\
RGTB(255,0,0),         /* 25 */\
RGTB(255,0,0),         /* 26 */\
RGTB(0,255,0),         /* 27 */\
RGTB(0,255,0),         /* 28 */\
RGTB(0,255,0),         /* 29 */\
RGTB(0,255,0),         /* 30 */\
RGTB(0,255,0),         /* 31 */\
RGTB(0,255,0),         /* 32 */\
RGTB(0,0,255),         /* 33 */\
RGTB(0,0,255),         /* 34 */\
RGTB(0,0,255),         /* 35 */\
RGTB(0,0,255),         /* 36 */\
RGTB(0,0,255),         /* 37 */\
RGTB(0,0,255),         /* 38 */\
RGTB(255,255,0),       /* 39 */\
RGTB(255,255,0),       /* 40 */\
RGTB(3,227,231),       /* 41 */\
RGTB(3,227,231),       /* 42 */\
RGTB(7, 3, 7),         /* 43 */\
RGTB(7, 3, 7),         /* 44 */\
}
#else 
/* THREE color */
#define COLOR_GRID_POINT       (RGB(199, 195, 199))
#define COLOR_GRID_AREA_BG     (RGB(7, 3, 7))
#define COLOR_BACK_GROUND      (RGB(63, 75, 151))
/* menu win color table */
#define COLOR_MENU_ONE         (RGB(183, 83, 7))	/* color table */
/* button color table */
#define COLOR_BUTTON           (RGB(87,131,231))
/* char and hz table */
#define COLOR_CHAR             (RGB(255,255,255))
/* lines color */
#define COLOR_CH1              (RGB(255,255,7))
#define COLOR_CH2              (RGB(7,227,231))
/* TIPS_COLOR */
#define COLOR_TIPS_ERROR       (RGB(255,0,0))
#endif
#define COLOR_TIPS_WARNING     (COLOR_CH1)
#define COLOR_TIPS_NORMAL      (COLOR_CHAR)
/* function delares */
display_info_def * get_display_dev_info(void);
static void LTDC_Init(display_dev_def * info);
int dev_init(void);
void clear_screen(unsigned int index);
unsigned short get_point( unsigned short x , unsigned short y );
void set_noload_point( unsigned short x , unsigned short y , unsigned int color );
void set_point( unsigned short * mark , unsigned short x , unsigned short y , unsigned int color );
/* end */
#endif
/* end of files */





















