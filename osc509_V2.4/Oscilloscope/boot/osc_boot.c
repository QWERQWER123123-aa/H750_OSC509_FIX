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
/* Includes ------------------------------------------------------------------*/
#include "fos.h"
#include "main.h"
#ifdef OSC_STM32H750
#else
#endif
#include "string.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
#define OSC_BOOT_ADDR  ( 0x08000000)

#define BL_H  'V'
#define BL_0  (1)
#define BL_1  (0)
#define BL_2  (0)
/* transfer */
#define BOOT_VERSION_ALONE ((BL_H<<24)|(BL_0<<16)|(BL_1<<8)|(BL_2))
/* USER CODE BEGIN PD */
const unsigned int __FS_version_export_BL[3] = {0x87bc2547,0,0xFD26ec88};
/* Includes ------------------------------------------------------------------*/
static unsigned int __FS_BOOT_V;
static char boot_version_str[16];
/* local */
#if 0
static int osc_boot_remote_version_check(void)
{
	/* addr */
	unsigned int * adr = ( unsigned int * )OSC_BOOT_ADDR;
	/* source */
	for( int i = 0 ; i < 0x20000 / 4 - 20; i ++ )
	{
		/* check */
		if( adr[0] == __FS_version_export_BL[0] && adr[2] == __FS_version_export_BL[2] )
		{
			/* ok ! we got the bootloader version */
			__FS_BOOT_V = adr[1];
			/* return */
			return adr[1];
		}
		/* inc */
		adr ++;
	}
	/* else */
	return FS_ERR;
}
#ifdef OSC_STM32H750
/* check */
#if 0
/* local */
static int osc_boot_local_version_check(void)
{
	/* addr */
	unsigned int * adr = ( unsigned int * )BOOT_ROM;
	/* source */
	for( int i = 0 ; i < sizeof(BOOT_ROM) / 4 ; i ++ )
	{
		/* check */
		if( adr[0] == __FS_version_export_BL[0] && adr[2] == __FS_version_export_BL[2] )
		{
			/* ok ! we got the bootloader version */
			return adr[1];
		}
		/* inc */
		adr ++;		
	}
	/* else */
	return FS_ERR;
}
#endif
#else
#endif
/* FOS task */
int boot_update_logic(void)
{ 
	/* check */
	osc_boot_remote_version_check();
#if 0	
	int version_l = osc_boot_local_version_check();
	/* check verson */
	if( version_l == version_r )
	{
		/* check ok */
		return FS_OK;
	}
	/* need erase flash */
	HAL_FLASH_Unlock();		
	/* wait erase */
	FLASH_Erase_Sector(0,FLASH_BANK_1,FLASH_VOLTAGE_RANGE_3);
	/* process */
	unsigned int bw0 = sizeof(BOOT_ROM);
	/* real */
	int bwt = ( bw0 % 32 ) ? ( ( bw0 / 32 + 1 ) * 32 ) : bw0;
	/* for */
	for( int i = 0 ; i < bwt / 32 ; i ++ )
	{
		/* promger */
		HAL_FLASH_Program(32,0x08000000 + i * 32 ,(unsigned int)&BOOT_ROM[i*32]);
		/* progmer */
	}
	/* check again */
	version_r = osc_boot_remote_version_check();
	/* check ok */
	if( memcmp((void*)0x08000000,BOOT_ROM,sizeof(BOOT_ROM)) )
	{
		/* error */
		return FS_ERR;
	}
	/* OK ,reboot */
	NVIC_SystemReset();	
	#else
	#endif
	/* never arrive here */
	return FS_OK;
}
#endif
/* USER CODE END PFP */
char * boot_version(void)
{
	__FS_BOOT_V = BOOT_VERSION_ALONE;
	/* create version */
	sprintf(boot_version_str,"BOOT:%c%d.%d.%d",(char)(__FS_BOOT_V>>24),(unsigned char)(__FS_BOOT_V>>16),
		                           (unsigned char)(__FS_BOOT_V>>8),(unsigned char)(__FS_BOOT_V));	
	/* return */
	return boot_version_str;
}










