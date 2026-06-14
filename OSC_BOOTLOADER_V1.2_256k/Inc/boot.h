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

#ifndef __BOOT_H__
#define __BOOT_H__


#define APP_FLASH_ADDR    0x08020000
#define APP_FLASH_END     0x08200000
#define APP_FLASH_SIZE    (APP_FLASH_END - APP_FLASH_ADDR)
#define FLASH_SECTOR_SIZE 0x20000
#define APP_START_SECTOR  1
#define APP_END_SECTOR    15



unsigned int app_code_check(void);
void JumpToapp(void);
void usb_dnd_flash_init(void);
void usb_dnd_flash_thread(void);

#endif