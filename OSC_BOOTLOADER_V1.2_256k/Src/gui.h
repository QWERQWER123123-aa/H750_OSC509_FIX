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
#ifndef __GUI_H__ 
#define __GUI_H__
 
void gui_char(unsigned short x,unsigned short y,char num,unsigned char size,unsigned int color,unsigned int backcolor,unsigned char show_m);
void clear_screen(unsigned int index);
unsigned short get_point( unsigned short x , unsigned short y );
void set_noload_point( unsigned short x , unsigned short y , unsigned int color );
void set_point( unsigned short * mark , unsigned short x , unsigned short y , unsigned int color );
void show_string(char * hzc);
void show_process_des(void);
void hide_process(void);
void show_string_static(char * hzc);
void gui_dynamic_string(unsigned short px,unsigned short py,char * hzc,unsigned int color);
void create_boot_ui(void);
void console_out(char * co);

#endif	

