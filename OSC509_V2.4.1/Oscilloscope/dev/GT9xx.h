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

#ifndef	_GT9XX_H
#define _GT9XX_H

/*********************头文件包含*********************/
#include	"main.h"
									
/***************GT9147 Device Address****************/
#define GT9xx_DevAdr0 			0x28						
/*									
#define GT9147_DevAdr1 			0xBA								
*/
								
#define GT9xx_ConfigMsgReg 	0x8047
#define GT9xx_ProductIDReg		0x8140
#define GT9xx_TouchStateReg 	0X814E								
#define GT9xx_TouchPoint1Reg	0X8150
#define GT9xx_TouchPoint2Reg	0X8158
#define GT9xx_TouchPoint3Reg	0X8160
#define GT9xx_TouchPoint4Reg	0X8168
#define GT9xx_TouchPoint5Reg	0X8170

typedef struct			
{
	uint8_t TouchSta;	
	uint16_t x[5];
	uint16_t y[5];	
	
}TouchPointRefTypeDef;

extern TouchPointRefTypeDef TPR_Structure;

extern void GT9xx_WrNByte(uint8_t Daddr,uint16_t Waddr,uint8_t a[],uint8_t n);
extern void GT9xx_RdNByte(uint8_t Daddr,uint16_t Waddr,uint8_t a[],uint8_t n);
extern void GT9xx_SendConfigMSG(uint8_t cmd);
extern int GT9xx_Init(void);
extern void GT9147_Scan(void);
									
#endif

/*********************End of File********************/
