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
#include "osc_config.h"
/* register a init inode */
FOS_INODE_REGISTER("hal_gpio",hal_gpio_init,0,0,14);
/* define */
#define HC595_DIN_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET)
#define HC595_DIN_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET)
#define HC595_RCK_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET)
#define HC595_RCK_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)
#define HC595_SCK_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)
#define HC595_SCK_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)
/* static */
static unsigned short Q0_Q15_S = 0;
/* gpio_config_table */
const GPIO_CONFIG_DEF gpio_config_table[] = 
{
	/* 1 */
	{
		.capital = "HC595_SCLR",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_4,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 2 */
	{
		.capital = "RA0",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_12,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 3 */
	{
		.capital = "RA1",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 4 */
	{
		.capital = "HC595 DATA LINE",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 5 */
	{
		.capital = "MXT2088",
		.GPIO_GROUP = GPIOD,
		.GPIO_PIN = GPIO_PIN_All,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 6 */
	{
		.capital = "TRIGA",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_15,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 7 */
	{
		.capital = "TRIGB",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_0,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 8 */
	{
		.capital = "PC14 SD INSTER ",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_14,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
#if OSC_CONFIG_PWM_OUT
	/* 9 */
	{
		.capital = "LED_LIGHT",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_1,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
#endif		
	/* 10 */
	{
		.capital = "LED_LIGHT",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_12,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 11 */
	{
		.capital = "LED_LIGHT",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_13,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
};
/* v */
static int hal_gpio_init(void)
{
	/* gpio struction */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/* init gpio one by one */
	for( int i = 0 ; i < sizeof(gpio_config_table) / sizeof(gpio_config_table[0]) ; i ++ )
	{
		/* Build initialization structure */
		GPIO_InitStruct.Pin = gpio_config_table[i].GPIO_PIN;
		GPIO_InitStruct.Mode = gpio_config_table[i].GPIO_MODE;
		GPIO_InitStruct.Pull = gpio_config_table[i].GPIO_PULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		/* init gpio */
		HAL_GPIO_Init((GPIO_TypeDef *)gpio_config_table[i].GPIO_GROUP , &GPIO_InitStruct);
		/* AF ? */
		if( gpio_config_table[i].GPIO_MODE != GPIO_MODE_AF_PP )//&& gpio_config_table[i].GPIO_MODE != GPIO_MODE_INPUT )
		{
			/* default status */
			HAL_GPIO_WritePin((GPIO_TypeDef *)gpio_config_table[i].GPIO_GROUP, gpio_config_table[i].GPIO_PIN, (GPIO_PinState)gpio_config_table[i].GPIO_DEFAULT);
		}
		/* OK */
	}
	/* output  */
	osc_hc595_serial(Q0_Q15_S);
	/* set */
	hal_write_gpio(DIO_S1,1);
	hal_write_gpio(DIO_S2,0);
	/* set */
	hal_write_gpio(DIO_HC_B2,1);
	hal_write_gpio(DIO_HC_C2,0);	
  /* exit the it */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,0);  
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); 	
  /* exit the it */
	HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);  
	HAL_NVIC_DisableIRQ(EXTI0_IRQn); 		
	/* return OK */
	return FS_OK;
}
/* w_gpio_sem */
static unsigned char w_gpio_sem = 0;
/* write gpio */
void hal_write_gpio(unsigned short index,unsigned short sta)
{
	/* check */
	if( w_gpio_sem )
	{
		return;
	}
	/* lock */
	w_gpio_sem = 1;
	/* check index */
	if( index > 16 )
	{
		return;
	}
	/* set */
	if( sta )
	{
		Q0_Q15_S |= ( 1 << index );
	}
	else
	{
		Q0_Q15_S &=~ ( 1 << index );
	}
	/* updata */
	osc_hc595_serial(Q0_Q15_S);
	/* uplock */
	w_gpio_sem = 0;	
}
/* set CH1 and CH2 gain */
void osc_gain_ctrl(unsigned char chn,unsigned short index)
{
	/* check */
	if( chn == 0 )
	{
		/* check inde */
		if( index & 0x1 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_A0 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_A0 );
		}
		/* check inde */
		if( index & 0x2 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_B0 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_B0 );
		}
		/* check inde */
		if( index & 0x4 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_C0 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_C0 );
		}		
	}
	else if( chn == 1 )
	{
		/* check inde */
		if( index & 0x1 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_A1 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_A1 );
		}
		/* check inde */
		if( index & 0x2 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_B1 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_B1 );
		}
		/* check inde */
		if( index & 0x4 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_C1 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_C1 );
		}		
	}
	else if( chn == 2 )
	{
		/* check inde */
		if( index & 0x1 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_A2 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_A2 );
		}
		/* check inde */
		if( index & 0x2 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_B2 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_B2 );
		}
		/* check inde */
		if( index & 0x4 )
		{
			Q0_Q15_S |= ( 1 << DIO_HC_C2 );
		}
		else
		{
			Q0_Q15_S &=~ ( 1 << DIO_HC_C2 );
		}		
	}	
	else
	{
		/*------------------------*/
	}
	/* updata */
	osc_hc595_serial(Q0_Q15_S);	
}
/* read gpio */
unsigned short hal_read_gpio(unsigned short index)
{
	/* over the gate */
	if( index > sizeof(gpio_config_table) / sizeof(gpio_config_table[0]) )
	{
		return 0xffff;//can not write
	}
  /* read */
	GPIO_TypeDef * gpio_g = (GPIO_TypeDef *)gpio_config_table[index].GPIO_GROUP;
	/* reutrn */
  return ( gpio_g->IDR & gpio_config_table[index].GPIO_PIN );
}
/* delay for yuhui */
static void delay_595(unsigned int t)
{
	while(t--);
}
/* hc595 set io */
void osc_hc595_serial(unsigned short Q15_Q0)
{
	/* set */
	HC595_DIN_L;
	HC595_RCK_L;
	HC595_SCK_L;
	/* delay */
	delay_595(10);		
	/* set */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* sck set high */
		HC595_SCK_L;		
		/* check */
		if( Q15_Q0 & 0x8000 )
		{
			HC595_DIN_H;
		}
		else
		{
			HC595_DIN_L;
		}
		/* delay */
		delay_595(1);		
		/* sck set high */
		HC595_SCK_H;
		/* delay */
		delay_595(1);
		/*---------*/
		Q15_Q0 <<= 1;
	}
	/* RCK */
	HC595_RCK_H;
	/* delay */
	delay_595(1);
	/* set low */
	HC595_RCK_L;
	HC595_SCK_L;
}
/* set DIO RA0 DA1 */
void osc_dcac_gpio_set(unsigned char chn,unsigned char dcac)
{
	/* chn */
	if( chn == 0 )
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,dcac?GPIO_PIN_RESET:GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,dcac?GPIO_PIN_RESET:GPIO_PIN_SET);
	}	
}














































