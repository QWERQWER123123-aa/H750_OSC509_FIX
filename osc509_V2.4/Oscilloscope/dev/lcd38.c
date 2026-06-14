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
/* cs */
#define LCD38_CS(n)   n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
#define LCD38_SCL(n)  n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
#define LCD38_SDA(n)  n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
/* */
void LCD_SetWindows(unsigned short xStar, unsigned short yStar,unsigned short xEnd,unsigned short yEnd);
/* init */
void osc_lcd38_gpio_init(void)
{
	/* default dev */
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* USER CODE BEGIN LTDC_MspInit 0 */

	/* USER CODE END LTDC_MspInit 0 */

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	/* SET back light aways on */
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4|GPIO_PIN_8,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);	
	/* init */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8;
	/* reset */										
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  /* init */
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);	
	/* GPIOA 15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	/* init */
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
}
/* delay */
void lcd38_delay(unsigned int t)
{
	t *= 200;
	/* wait */
	while(t--);
}
/* tese */
volatile unsigned char bit_t = 0;
/* lcd spi write cmd */
void LCD_WR_REG(unsigned char cmd)
{
	/* set */
	unsigned short cmd_b = cmd;
	/* set */
	LCD38_CS(0);
	lcd38_delay(10);
	LCD38_SCL(0);
	/* set RS */
	LCD38_SDA( 0 );
	/* write rx */
	lcd38_delay(10);
	/* checkl */
	for( int i = 0 ; i < 9 ; i ++ )
	{
		/* check set */
		LCD38_SCL(1);
		/* delay */
		lcd38_delay(10);
		/* check set */
		LCD38_SCL(0);		
		/* check */
		bit_t = ( cmd_b << i ) & 0x80 ;
		/* set */
    LCD38_SDA( bit_t );
		/* delay */
		lcd38_delay(10);		
	}
	/* set */
	LCD38_CS(1);
	lcd38_delay(100);	
	LCD38_SCL(1);	
	/* delay */
	lcd38_delay(2000);
}
/* lcd spi write data */
void LCD_WR_DATA_INIT(unsigned char dat)
{
	/* set */
	unsigned short cmd_b = dat;
	/* set */
	LCD38_CS(0);
	LCD38_SCL(0);
	/* set RS */
	LCD38_SDA( 1 );
	/* write rx */
	lcd38_delay(10);
	/* checkl */
	for( int i = 0 ; i < 9 ; i ++ )
	{
		/* check set */
		LCD38_SCL(1);
		/* delay */
		lcd38_delay(10);
		/* check set */
		LCD38_SCL(0);		
		/* check */
		bit_t = ( cmd_b << i ) & 0x80 ;
		/* set */
    LCD38_SDA( bit_t );
		/* delay */
		lcd38_delay(10);		
	}
	/* set */
	LCD38_CS(1);
	lcd38_delay(100);	
	LCD38_SCL(1);		
	lcd38_delay(2000);
}
/* LCD init */
void LCD_Init(void)
{  
  /* set */					 
	osc_lcd38_gpio_init();
//************* Start Initial Sequence **********//		
	LCD_WR_REG(0XF9);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x08);
	
	LCD_WR_REG(0xC0);
	LCD_WR_DATA_INIT(0x19);//VREG1OUT POSITIVE
	LCD_WR_DATA_INIT(0x1a);//VREG2OUT NEGATIVE
	
	LCD_WR_REG(0xC1);
	LCD_WR_DATA_INIT(0x45);//VGH,VGL    VGH>=14V.
	LCD_WR_DATA_INIT(0x00);
	
	LCD_WR_REG(0xC2);
	LCD_WR_DATA_INIT(0x33);
	
	LCD_WR_REG(0XC5);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x28);//VCM_REG[7:0]. <=0X80.
	
	LCD_WR_REG(0xB1);//OSC Freq set.
	LCD_WR_DATA_INIT(0xA0);//0xA0=62HZ,0XB0 =70HZ, <=0XB0.
	LCD_WR_DATA_INIT(0x11);
	
	LCD_WR_REG(0xB4);
	LCD_WR_DATA_INIT(0x02); //2 DOT FRAME MODE,F<=70HZ.
	
	LCD_WR_REG(0xB6);
	LCD_WR_DATA_INIT(0xB0);
	LCD_WR_DATA_INIT(0x42);//0 GS SS SM ISC[3:0];
	LCD_WR_DATA_INIT(0x3B);
	
	
	LCD_WR_REG(0xB7);
	LCD_WR_DATA_INIT(0x07);
	
	LCD_WR_REG(0xE0);
	LCD_WR_DATA_INIT(0x1F);
	LCD_WR_DATA_INIT(0x25);
	LCD_WR_DATA_INIT(0x22);
	LCD_WR_DATA_INIT(0x0B);
	LCD_WR_DATA_INIT(0x06);
	LCD_WR_DATA_INIT(0x0A);
	LCD_WR_DATA_INIT(0x4E);
	LCD_WR_DATA_INIT(0xC6);
	LCD_WR_DATA_INIT(0x39);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x00);
	
	LCD_WR_REG(0XE1);
	LCD_WR_DATA_INIT(0x1F);
	LCD_WR_DATA_INIT(0x3F);
	LCD_WR_DATA_INIT(0x3F);
	LCD_WR_DATA_INIT(0x0F);
	LCD_WR_DATA_INIT(0x1F);
	LCD_WR_DATA_INIT(0x0F);
	LCD_WR_DATA_INIT(0x46);
	LCD_WR_DATA_INIT(0x49);
	LCD_WR_DATA_INIT(0x31);
	LCD_WR_DATA_INIT(0x05);
	LCD_WR_DATA_INIT(0x09);
	LCD_WR_DATA_INIT(0x03);
	LCD_WR_DATA_INIT(0x1C);
	LCD_WR_DATA_INIT(0x1A);
	LCD_WR_DATA_INIT(0x00);
	
	LCD_WR_REG(0XF1);
	LCD_WR_DATA_INIT(0x36);
	LCD_WR_DATA_INIT(0x04);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x3C);
	LCD_WR_DATA_INIT(0x0F);
	LCD_WR_DATA_INIT(0x0F);
	LCD_WR_DATA_INIT(0xA4);
	LCD_WR_DATA_INIT(0x02);
	
	LCD_WR_REG(0XF2);
	LCD_WR_DATA_INIT(0x18);
	LCD_WR_DATA_INIT(0xA3);
	LCD_WR_DATA_INIT(0x12);
	LCD_WR_DATA_INIT(0x02);
	LCD_WR_DATA_INIT(0x32);
	LCD_WR_DATA_INIT(0x12);
	LCD_WR_DATA_INIT(0xFF);
	LCD_WR_DATA_INIT(0x32);
	LCD_WR_DATA_INIT(0x00);
	
	LCD_WR_REG(0XF4);
	LCD_WR_DATA_INIT(0x40);
	LCD_WR_DATA_INIT(0x00);
	LCD_WR_DATA_INIT(0x08);
	LCD_WR_DATA_INIT(0x91);
	LCD_WR_DATA_INIT(0x04);
	
	LCD_WR_REG(0XF8);
	LCD_WR_DATA_INIT(0x21);
	LCD_WR_DATA_INIT(0x04);
	
	LCD_WR_REG(0x36);
	LCD_WR_DATA_INIT(0x18 | (5 << 5));
	
	LCD_WR_REG(0x3A);
	LCD_WR_DATA_INIT(0x55);
	
	LCD_WR_REG(0x11);
	
	lcd38_delay(12000);
	
	LCD_WR_REG(0x29);
	/* set */
	LCD_SetWindows(0,0,500-1,480-1);
}

//	0x2A=0x2A;
//	0x2B=0x2B;
//	lcddev.wramcmd=0x2C;
//#if USE_HORIZONTAL==1	//Ê¹ÓÃºáÆÁ	  
//	lcddev.dir=1;//ºáÆÁ
//	lcddev.width=480;
//	lcddev.height=320;	

void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2C);
}	 

void LCD_SetWindows(unsigned short xStar, unsigned short yStar,unsigned short xEnd,unsigned short yEnd)
{	
	LCD_WR_REG(0x2A);	
	LCD_WR_DATA_INIT(xStar>>8);
	LCD_WR_DATA_INIT(0xFF&xStar);		
	LCD_WR_DATA_INIT(xEnd>>8);
	LCD_WR_DATA_INIT(0xFF&xEnd);

	LCD_WR_REG(0x2B);	
	LCD_WR_DATA_INIT(yStar>>8);
	LCD_WR_DATA_INIT(0xFF&yStar);		
	LCD_WR_DATA_INIT(yEnd>>8);
	LCD_WR_DATA_INIT(0xFF&yEnd); 
	  
	LCD_WriteRAM_Prepare();	
}   





































