/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__	

#define GPIO_DEFAULT_HIGH   (1)
#define GPIO_DEFAULT_LOW    (0)

/* hal afio map */
#define DIO_HC_A0           (6)
#define DIO_HC_B0           (7)
#define DIO_HC_C0           (0)
#define DIO_HC_A1           (15)
#define DIO_HC_B1           (8)
#define DIO_HC_C1           (1)
#define DIO_HC_A2           (10)
#define DIO_HC_B2           (13)
#define DIO_HC_C2           (14)
/* RLY */
#define DIO_RLY0            (5)
#define DIO_RLY1            (2)
/* S1 S2 */
#define DIO_S1              (11)
#define DIO_S2              (12)
/*CUR */
#define DIO_CUR             (9)
/* typedef gpio config table */

typedef struct{
	char * capital;
	void * GPIO_GROUP;
	unsigned int GPIO_PIN;
	unsigned int GPIO_MODE;
	unsigned int GPIO_PULL;
	unsigned int GPIO_DEFAULT;
}GPIO_CONFIG_DEF;

/* function declears */
static int hal_gpio_init(void);
void hal_write_gpio(unsigned short index,unsigned short sta);
unsigned short hal_read_gpio(unsigned short index);
void osc_hc595_serial(unsigned short Q0_Q15);
void osc_gain_ctrl(unsigned char chn,unsigned short index);
void osc_dcac_gpio_set(unsigned char chn,unsigned char dcac);
/* end of files */
#endif


