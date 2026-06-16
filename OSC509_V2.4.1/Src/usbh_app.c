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
#include "fatfs.h"
#include "usb_host.h"
#include "fos.h"
#include "osc_ui.h"
#include "osc.h"
#include "display_dev.h"
#include "usbh_app.h"
#include "hal_tim.h"
/* declease */
static void osc_usbh_thread(void);
static void osc_usbh_save_thread(void);
/* USER CODE BEGIN 0 */
#define ENABLE_INT()   __set_PRIMASK(0)
#define DISABLE_INT()  __set_PRIMASK(1)
/* reg a idle task */
FOS_TSK_REGISTER(osc_usbh_thread,PRIORITY_IDLE,100);
/* FATFS */
FIL file;
UINT bw;
/* bmp buffer */
static unsigned char bmp_color_buffer[8*1024];
/* ssata */
static unsigned int start_save_flag = 0;
/* bmp header */
const unsigned char bmp_header[] = {
  0x42, 0x4D, 0x36, 0x94, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x20, 0x03, 0x00, 0x00, 0x20, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x94, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* */
extern char range_file_name[64];
extern void osc_msg_close(void);
extern unsigned char GLOBAL_irq;
extern void osc_set_process(unsigned int pos);
extern int osc_msg_box(unsigned char index);
extern void osc_run_and_stop_trs(void);
/* usb thread */
static void osc_usbh_thread(void)
{
	osc_usbh_save_thread();
}
/* start */
void osc_usbh_save_start(void)
{
	/* if */
	if( start_save_flag == 0 )
	{
		start_save_flag = 1;//start
		/* shoer */
	}
}
/* restart the osc */
static void osc_usbh_reset(void)
{
	/* close file */
	f_close(&file);
	/* restart */
	osc_run_and_stop_trs();
	/* enable */
	ENABLE_INT();	
}
/* start save */
static void osc_usbh_save_thread(void)
{
	/* if */
	if( start_save_flag == 0 )
	{
		return;
	}
	/* disable it */
	while( GLOBAL_irq ==0 );	
	/* clear */
	GLOBAL_irq = 0;
	/* start_save_flag */
	start_save_flag = 0;
	/* ok open file */
	FRESULT res = f_open(&file,range_file_name,FA_OPEN_ALWAYS|FA_WRITE);
	/* check */
	if( res != FR_OK )
	{
		osc_msg_box(20);
		/* clear */
		start_save_flag = 0;	
		/* reset */
		osc_usbh_reset();		
		/* break */
		return;
	}
	/* check write */
	res = f_write(&file,bmp_header,sizeof(bmp_header),&bw);
	/* check */
	if( res != FR_OK )
	{
		osc_msg_box(20);
		/* clear */
		start_save_flag = 0;
		/* reset */
		osc_usbh_reset();				
		/* break */	
		return;
	}
	/* sync */
	res = f_sync(&file);
	/* sync ok */
	if( res != FR_OK )
	{
		osc_msg_box(20);
		/* clear */
		start_save_flag = 0;
		/* reset */
		osc_usbh_reset();				
		/* break */	
		return;
	}
	/* write bmp color data */
	display_info_def * did = get_display_dev_info();
	/* gram */
	unsigned char * gram_p = (unsigned char *)did->gram_addr;
	/* color table */
	const unsigned int * color_table = get_color_table();
	/* next point */
	unsigned int next_point = 0;
	unsigned int real_write = 0;
	/* data */
	while(1)
	{
		/* clear */
		real_write = 0;
		/* create the color buffer */
	  for( int i = 0 ; i < sizeof(bmp_color_buffer) / 3 ; i ++ )
		{
			bmp_color_buffer[i*3] = color_table[gram_p[next_point]] & 0xff;
			bmp_color_buffer[i*3 + 1] =  (color_table[gram_p[next_point]] >> 8 )& 0xff;
			bmp_color_buffer[i*3 + 2] =  (color_table[gram_p[next_point]] >> 16 )& 0xff;
			/* next point */
			next_point ++;
			real_write ++;
			/* check */
			if( next_point >= 800 * 480 )
			{
				break;
			}
		}
		/* save data */
		osc_set_process(next_point);
		/* check write */
		res = f_write(&file,bmp_color_buffer,real_write * 3 ,&bw);
		/* check */
		if( res != FR_OK )
		{
			osc_msg_box(20);
			/* clear */
			start_save_flag = 0;		
			/* reset */
			osc_usbh_reset();					
			/* break */			
			return;
		}
		/* sync */
		res = f_sync(&file);
		/* sync ok */
		if( res != FR_OK )
		{
			osc_msg_box(20);
			/* clear */
			start_save_flag = 0;	
			/* reset */
			osc_usbh_reset();					
			/* break */			
			return;
	  }
		/* ok check */
		if( next_point >= 800 * 480 )
		{
			osc_msg_box(21);
			/* clear */
			start_save_flag = 0;		
			/* reset */
			osc_usbh_reset();		
			/* break */			
			return;
		}
	}
}

























