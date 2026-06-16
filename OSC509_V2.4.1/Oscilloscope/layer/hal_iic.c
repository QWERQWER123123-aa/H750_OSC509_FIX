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
#include "hal_gpio.h"
#include "fos.h"
#include "hal_iic.h"
/* register a init inode */
FOS_INODE_REGISTER("hal_iic",hal_iic_init,0,0,3);
/* iic busy flag */
static unsigned int iic_busy = 0;
/* static iic sem */
static unsigned char iic_sem = 0;
/* hal iic init */
static int hal_iic_init(void)
{
	/* init iic hardware */
	iic_init();
	/* return OK */
	return FS_OK;
}
/* set sem */
void hal_iic_sem_set(unsigned char dem)
{
	iic_sem = dem;
}
/* get sem */
unsigned char hal_iic_sem_get(void)
{
	return iic_sem;
}
/* set sta */
void hal_iic_sta(void)
{
	iic_busy ++;
}
/* get sta */
unsigned int hal_iic_busy(void)
{
	return iic_busy;
}
/* delay for iic */
static void delay_us_ic(unsigned int t)
{
#if OSC_STM32H750		
	/* set a simple delay for iic bus */
	t *= 200;
	/* wait */
	while(t--);
#else
	/* set a simple delay for iic bus */
	while(t--);	
#endif	
}
/* iic init */
static void iic_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
  /* clock */
	__HAL_RCC_GPIOB_CLK_ENABLE();
  /* pin init */
	GPIO_Initure.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  /* set up */
	IIC_SDA(1);
	IIC_SCL(1);  
}
/* iic start */
void IIC_Start(void)
{
	SDA_OUT(); 
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us_ic(4);
 	IIC_SDA(0);
	delay_us_ic(4);
	IIC_SCL(0);
}	  
/* iic stop */
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
 	delay_us_ic(4);
	IIC_SCL(1); 
	delay_us_ic(4);			
	IIC_SDA(1);		   	
}
/* iic wait ack */
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	/* set gpio in */
	SDA_IN();
	IIC_SDA(1);
	delay_us_ic(1);	   
	IIC_SCL(1);
	delay_us_ic(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);
  /* return OK */	
	return 0;  
} 
/* iic ack */
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us_ic(2);
	IIC_SCL(1);
	delay_us_ic(2);
	IIC_SCL(0);
}
/* NAVCK */		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us_ic(2);
	IIC_SCL(1);
	delay_us_ic(2);
	IIC_SCL(0);
}					 				     
/* send data */	  
void IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;   
	SDA_OUT(); 	    
	IIC_SCL(0);
	for(t = 0;t<8;t++)
	{              
		IIC_SDA((txd&0x80)>>7);
		txd<<=1; 	  
		delay_us_ic(2);
		IIC_SCL(1);
		delay_us_ic(2); 
		IIC_SCL(0);	
		delay_us_ic(2);
	}	 
} 	    
/* read data */
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive = 0;
	SDA_IN();
  for( i = 0;i<8;i++ )
	{
		IIC_SCL(0); 
		delay_us_ic(2);
		IIC_SCL(1);
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us_ic(1); 
  }					 
	if (!ack)
			IIC_NAck();
	else
			IIC_Ack();
	
    return receive;
}
/* dac create th */
void dac_update(unsigned short volA,unsigned short B,unsigned short C,unsigned short D)
{

}























