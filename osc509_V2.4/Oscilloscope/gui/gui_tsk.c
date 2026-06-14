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
#include "fos.h"
#include "display_dev.h"
#include "gui_dev.h"
/*----------------------------------------------------------------------------*/
static void gui_tsk(void);
/*----------------------------------------------------------------------------*/
FOS_TSK_REGISTER(gui_tsk,PRIORITY_0,10); /* gui detecter task run as 10 ms*/
/* gui task */
static void gui_tsk(void)
{
	/* nothing to do */
}



























