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
#include "fos.h"
/* Private includes ----------------------------------------------------------*/
#define FOS_EXAPP_SUPPLY  (0)
/* init all nodes */
int fs_system_initialization(void)
{
	/* ret */
	int ret = FS_ERR;
	/* init seq */
	for( int i = 0 ; i < FOS_PRIORITY_MAX ; i ++ )
	{
		/* init all need init node */
		for( const inode_vmn * base = FOS_VMN_BASE ; 
				base < FOS_VMN_LIMIT ; base ++ )
		{
			/* if need init */
			if( base->head_init != 0 && ((base->seq_type & ~0xf) == 0 ) && 
  			 (base->seq_type & 0xf) == i )
			{
				/* init the base dev */
				ret = base->head_init();
				/* end of func */
			}
		}
	}
	/* config */
	for( int i = 0 ; i < FOS_PRIORITY_MAX ; i ++ )
	{
		/* init all need init node */
		for( const inode_vmn * base = FOS_VMN_BASE ; 
				base < FOS_VMN_LIMIT ; base ++ )
		{
			/* if need init */
			if( ( base->config != 0 ) && ((base->seq_type & ~0xf) == 0 ) && 
  			  (base->seq_type & 0xf) == i )
			{
				/* init the base dev */
				ret = base->config();
				/* end of func */
			}
		}
	}	
	/* return OK */
	return ret;
}














