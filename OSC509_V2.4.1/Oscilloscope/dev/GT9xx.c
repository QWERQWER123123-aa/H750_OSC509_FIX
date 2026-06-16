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
#include	"GT9xx.h"
#include "hal_iic.h"
#include "string.h"
/* config table */
unsigned char GT9xx_ConfigMSGTBL[186] = 
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,0xFF,0XFF
};
/* ACK */
volatile unsigned char RevAckF0 = 0;
/* read data */
TouchPointRefTypeDef TPR_Structure = {0,{0},{0}};	
/* GT9XX write byte */
void GT9xx_WrNByte(unsigned char Daddr,uint16_t Waddr,unsigned char a[],unsigned char n)
{
	unsigned int try_time = 20;
	unsigned char k;
	do
	{
		IIC_Start();
		IIC_Send_Byte(Daddr & 0xfe);
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0) 
		{
			try_time--;	
			continue;
		}
		IIC_Send_Byte((unsigned char)(Waddr >> 8));
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0) 
		{
			try_time--;	
			continue;
		}
		IIC_Send_Byte((unsigned char)(Waddr));
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0) 
		{
			try_time--;	
			continue;
		}	
		for (k=0; k<n; k++)					
		{
			IIC_Send_Byte(a[k]);
			RevAckF0 = IIC_Wait_Ack();	
			if(RevAckF0) break;
		}
		try_time--;		
	} while(RevAckF0&&try_time);
	IIC_Stop();
}
/* GT9XX read byte */
void GT9xx_RdNByte(unsigned char Daddr,uint16_t Waddr,unsigned char a[],unsigned char n)
{
	unsigned char k;
	unsigned int try_time = 20;
	do
	{
		IIC_Start();
		IIC_Send_Byte(Daddr & 0xfe);
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0) 
		{
			try_time--;	
			continue;
		}
		IIC_Send_Byte((unsigned char)(Waddr >> 8));
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0)
		{
			try_time--;
			continue;
		}
		IIC_Send_Byte((unsigned char)(Waddr));
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0) 
		{
			try_time--;
			continue;
		}
		IIC_Start();
		IIC_Send_Byte(Daddr | 0x01);
		RevAckF0 = IIC_Wait_Ack();
		if(RevAckF0)
		{
			try_time--;
			continue;
		}	
	} while(RevAckF0&&try_time);      
	for(k = 0; k < n; k++)					
	{
		a[k] = IIC_Read_Byte(1);
	}
	IIC_Stop();
}
/* send msg */
void GT9xx_SendConfigMSG(unsigned char cmd)
{
	unsigned char i;
	GT9xx_ConfigMSGTBL[184] = 0;
	for(i = 0;i < 184;i++)
	{
		GT9xx_ConfigMSGTBL[184] += GT9xx_ConfigMSGTBL[i];		
	}
	GT9xx_ConfigMSGTBL[184] = ~GT9xx_ConfigMSGTBL[184] + 1;	
	GT9xx_ConfigMSGTBL[185] = cmd;
	GT9xx_WrNByte(GT9xx_DevAdr0,GT9xx_ConfigMsgReg,GT9xx_ConfigMSGTBL,186);	
}
/* delay for gtxxxx */
static void delay_us_gt(unsigned int t)
{
#if OSC_STM32H750		
	/* set a simple delay for iic bus */
	t *= 200;
	/* wait */
	while(t--);
#else
	/* set a simple delay for iic bus */
	t *= 50;
	/* wait */
	while(t--);	
#endif
}
/* INIT */
int GT9xx_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOC_CLK_ENABLE();	
	unsigned char temp = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	delay_us_gt(5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	
	unsigned char tdwe[11];
	volatile static int ret;

	GT9xx_RdNByte(GT9xx_DevAdr0,GT9xx_ProductIDReg,tdwe,11);

	GT9xx_RdNByte(GT9xx_DevAdr0,GT9xx_ConfigMsgReg,&temp,1);

	if(temp < 0x60)
	{
		GT9xx_SendConfigMSG(0x01);
	}

	/*Configure GPIO pin : PB6 */
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* check */
	if( tdwe[0] == 0x39 )
	{
		/* GT9xx serial */
		return 1;
	}
	else
	{
		return 0;
	}
}

//const uint16_t GT9xx_TPR_TBL[5] = {GT9xx_TouchPoint1Reg,GT9xx_TouchPoint2Reg,GT9xx_TouchPoint3Reg,GT9xx_TouchPoint4Reg,GT9xx_TouchPoint5Reg};
//extern void set_noload_point( unsigned short x , unsigned short y , unsigned int color );

//unsigned char codef[5] = {9,15,28,34,40};

//void GT9xx_Scan(void)
//{
//	unsigned char i,sta = 0,dat[4] = {0};
//	
//	static unsigned int dctf = 0;
//	
//	dctf ++;
//	
//	if( dctf % 20 )
//	{
//		return;
//	}
//	
//	GT9xx_RdNByte(GT9xx_DevAdr0,GT9xx_TouchStateReg,&sta,1);	//读取当前触摸情况
//	if(sta & 0x80)		//坐标数据有效，0x814E寄存器的最高位为1表示坐标已准备好
//	{					//此位使用完后应清零，若未清零，下次将不能读取坐标数据，亦即0x814E寄存器其余7位均将输出0
//		if(sta & 0x0F)	//判断几个触摸点按下，0x814E寄存器的低4位表示有效触点个数
//		{
//			TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))将点的个数转换为触摸点按下有效标志
//			for(i = 0;i < 5;i++)
//			{
//				if (TPR_Structure.TouchSta & (1<<i))			//分别判断触摸点1-5是否被按下
//				{												//被按下则读取对应触摸点坐标数据
//					GT9xx_RdNByte(GT9xx_DevAdr0,GT9xx_TPR_TBL[i],dat,4);
////					TPR_Structure.x[i] = ((uint16_t)(dat[1]) << 8) + dat[0];		//竖屏
////					TPR_Structure.y[i] = ((uint16_t)(dat[3]) << 8) + dat[2];					
//					TPR_Structure.x[i] = (((uint16_t)(dat[1]) << 8) + dat[0]);//横屏
//					TPR_Structure.y[i] = ((uint16_t)(dat[3]) << 8) + dat[2];

//					if( TPR_Structure.x[i] < 1023 && TPR_Structure.y[i] < 599 )
//					{
//						set_noload_point(		TPR_Structure.x[i],TPR_Structure.y[i]	, codef[i]);
//						set_noload_point(		TPR_Structure.x[i],TPR_Structure.y[i]	+1, codef[i]);
//						set_noload_point(		TPR_Structure.x[i]+1,TPR_Structure.y[i]	, codef[i]);
//						set_noload_point(		TPR_Structure.x[i]+1,TPR_Structure.y[i]	+1, codef[i]);
//					}
//				}
//			}
//		}
//		sta = 0;		//使用完后清空0x814E寄存器所有标志
//		GT9xx_WrNByte(GT9xx_DevAdr0,GT9xx_TouchStateReg,&sta,1);
//	}
//	else				//清除所有触摸点按下有效标志
//	{
//		TPR_Structure.TouchSta &= 0xE0;	//低5位置0
//	}
//}

/*********************End of File********************/
