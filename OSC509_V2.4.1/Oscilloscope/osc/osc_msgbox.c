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
#include "main.h"
#include "display_dev.h"
#include "osc_win.h"
#include "string.h"
#include "middle.h"
#include "osc.h"
/* sec */
#define MAGIC_DATA0 (0x12345678)
#define MAGIC_DATA1 (0xF1D4E6A8)
/* keep in bootloader */
#define MAGIC_DATA2 (0x64864464)
#define MAGIC_DATA3 (0xFED90BAC)
/* set magic data */
static unsigned int * magic = (unsigned int *)(0x20020000 - 6*8);
/* UP */
static int osc_win_msg_init(void);
extern void osc_gui_gb2312_string_l2(unsigned short pw,const char * hzd,unsigned short x,unsigned short y,unsigned int color);
void osc_msg_close(void);
extern void osc_vol_scale_set_dir(unsigned char chn,unsigned char ind);
extern void osc_calibrate_step_init(unsigned char step);
extern void osc_param_save_noload(void);
extern unsigned char cal_step_start;
extern void osc_reset_param(void);
extern char msg_range_file_name[64];
extern void osc_usbh_save_start(void);
extern void osc_run_and_stop_trs(void);
/* init node */
FOS_INODE_REGISTER("osc_win_msg",osc_win_msg_init,0,0,14);
/* create cfg task gui detecter task run as 100 ms */
/* create */
static display_info_def * dev;
/* dfe */
widget_def group_msg[3];
window_def win_l2_main_msg;
/* static */
static unsigned short msg_touch_cnt[3];
/* int */
unsigned char message_box_index_g = 0xff;
unsigned char msg_flag = 0;
/* init */
static int osc_win_msg_init(void)
{
	/* get dev */
	dev = get_display_dev_info();
	/* return ok */
	return FS_OK;
}
/* msg */
unsigned short process_msg[6];
/* process init */
static void osc_process_init(unsigned short pw,unsigned short x,unsigned short y,unsigned short xxs,unsigned short yys)
{
	/* show */
	for( int i = 0 ; i < xxs ; i ++ )
	{
		for( int j = 0 ; j < yys ; j ++ )
		{
			set_l2_point_noload(pw,x+i,y+j,COLOR_L2_BUTTON_BG);
		}
	}
	/* set msg */
	process_msg[0] = pw;
	process_msg[1] = x;
	process_msg[2] = y;
	process_msg[3] = xxs;
	process_msg[4] = yys;
	process_msg[5] = 0xffff;
}
/* set process */
void osc_set_process(unsigned int pos)
{
	/* check and set */
	unsigned int step = pos / ( 800*480 / process_msg[3] );
	/* set dfe */
	if( step != process_msg[5] )
	{
		/* set */
		process_msg[5] = step;
		/* set procss */
		for( int j = 0 ; j < process_msg[4] ; j ++ )
		{
			set_l2_point_noload(process_msg[0],process_msg[1]+step,process_msg[2]+j,COLOR_L2_GROUP_SEL);
		}		
	}
}
/* funtion */
static void osc_gui_msg_button(widget_def * wid);
/* Includes ------------------------------------------------------------------*/
int osc_message_box(const char * msg0,unsigned int mode,unsigned char mhg)
{
	/* cal msgbox size */
	unsigned short msg_win_xsize,msg_win_ysize;
	unsigned short pox,poy;
	/* get len */
	int len_msg = strlen(msg0);
	/* check */
	if( len_msg == 0 )
	{
		msg_win_xsize = 300;
		msg_win_ysize = 120;
	}
	else
	{
		msg_win_xsize = len_msg * 8 + 10;
		/* shef */
		if( mode == 0 )
		{
			msg_win_ysize = 16 + 15 + 5;	
		}
		else if( mode == 4 )
		{
			msg_win_ysize = 16 + 10 +20 + 5 + 10;	
		}
		else
		{
			msg_win_ysize = 60 + 16 + 5 + 5;	
		}		
		/* limit */
		if( msg_win_xsize < 130 )
		{
			msg_win_xsize = 130;
		}
	}
	/* memset */
	memset(group_msg,0,sizeof(group_msg));
	memset(&win_l2_main_msg,0,sizeof(win_l2_main_msg));
	/* check */
	pox = ( 800 - msg_win_xsize )	/ 2;
	poy = ( 480 - msg_win_ysize )	/ 2;
	/* close win */
	osc_l2_win_clear_mem(1);
	/* set */
	osc_dev_l2_enable(pox,poy,msg_win_xsize,msg_win_ysize,255);		
	/* memset */
	memset(dev->gram_l2,COLOR_L2_GRAD,msg_win_xsize * msg_win_ysize);
	/* get len */
	if( len_msg )
	{
		/* show */
		osc_gui_gb2312_string_l2(msg_win_xsize,msg0,5,10,0);		
	}
	/* create wind */
	win_l2_main_msg.msg.x = 0;
	win_l2_main_msg.msg.y = 0;
	win_l2_main_msg.msg.x_size = msg_win_xsize;
	win_l2_main_msg.msg.y_size = msg_win_ysize;
	/* upd */
	win_l2_main_msg.msg.upd = 1;
	win_l2_main_msg.msg.mark_flag = 0;
	/* abx */
	win_l2_main_msg.msg.abx = pox;
	win_l2_main_msg.msg.aby = poy;	
	/* button */
	if( mode == 1 )
	{
		unsigned short xs = (msg_win_xsize - 3*5) / 2;
		/* set msg */
		group_msg[0].msg.x = 5;
		group_msg[0].msg.y = msg_win_ysize - 46 - 5;
		group_msg[0].msg.x_size = xs;
		group_msg[0].msg.y_size = 46;
		group_msg[0].msg.upd = 0xff;
		group_msg[0].msg.pri_data = "取消";
		group_msg[0].msg.abx = pox + group_msg[0].msg.x ;
		group_msg[0].msg.aby = poy + group_msg[0].msg.y ;
		group_msg[0].parent = &win_l2_main_msg;
		group_msg[0].msg.wflags = mhg;
		/* i */
		group_msg[1].msg.x = group_msg[0].msg.x + xs + 5;
		group_msg[1].msg.y = group_msg[0].msg.y;
		group_msg[1].msg.x_size = xs;
		group_msg[1].msg.y_size = 46;
		group_msg[1].msg.upd = 0xff;
		group_msg[1].msg.pri_data = "确定";
		group_msg[1].msg.abx = pox + group_msg[1].msg.x ;
		group_msg[1].msg.aby = poy + group_msg[1].msg.y ;		
		group_msg[1].parent = &win_l2_main_msg;
		group_msg[1].msg.wflags = mhg;
		/* i */
		osc_gui_msg_button(&group_msg[0]);
		osc_gui_msg_button(&group_msg[1]);
	}
	else if( mode == 2 )
	{
		unsigned short xs = msg_win_xsize - 5*2;
		/* set msg */
		group_msg[0].msg.x = 5;
		group_msg[0].msg.y = msg_win_ysize - 46 - 5;
		group_msg[0].msg.x_size = xs;
		group_msg[0].msg.y_size = 46;
		group_msg[0].msg.upd = 0xff;
		group_msg[0].msg.pri_data = "取消";
		group_msg[0].msg.abx = pox + group_msg[0].msg.x ;
		group_msg[0].msg.aby = poy + group_msg[0].msg.y ;
		group_msg[0].parent = &win_l2_main_msg;
		group_msg[0].msg.wflags = mhg;	
		/* i */
		osc_gui_msg_button(&group_msg[0]);		
	}
	else if( mode == 3 )
	{
		unsigned short xs = msg_win_xsize - 5*2;
		/* set msg */
		group_msg[0].msg.x = 5;
		group_msg[0].msg.y = msg_win_ysize - 46 - 5;
		group_msg[0].msg.x_size = xs;
		group_msg[0].msg.y_size = 46;
		group_msg[0].msg.upd = 0xff;
		group_msg[0].msg.pri_data = "确定";
		group_msg[0].msg.abx = pox + group_msg[0].msg.x ;
		group_msg[0].msg.aby = poy + group_msg[0].msg.y ;
		group_msg[0].parent = &win_l2_main_msg;
		group_msg[0].msg.wflags = mhg;	
		/* i */
		osc_gui_msg_button(&group_msg[0]);		
	}
	else if( mode == 4 )
	{
		osc_process_init(msg_win_xsize,5,msg_win_ysize - 20 - 5 , msg_win_xsize - 10,20);
	}
	/* r */
	return FS_OK;
}
/* message box index */
int osc_msg_box(unsigned char index)
{
	message_box_index_g = index;
	/* r */
	return FS_OK;
}
/* che */
int osc_msg_box_sta(void)
{
	return ( message_box_index_g == 0xfe ) ? 1 : 0;
}
/* irq */
unsigned char GLOBAL_irq = 0;
/* void mess ltdc int */
int osc_msg_ltdc_isr(void)
{
	/* get */
	switch(message_box_index_g)
	{
		case 0:
			/* set */
			osc_message_box("校准时需要拔出全部探头",1,1);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/
			break;
		case 1:
			/* set */
			osc_message_box("打开大容量存储设备",1,5);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/
			break;
		case 2:
			/* set */
			osc_message_box("恢复出厂默认参数",1,10);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/
			break;		
		case 3:
			/* set */
			osc_message_box("正在执行校准请等待(1/14)",2,20);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 4:
			/* set */
			osc_message_box("正在执行校准请等待(2/14)",2,22);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;			
		case 5:
			/* set */
			osc_message_box("正在执行校准请等待(3/14)",2,24);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 6:
			/* set */
			osc_message_box("正在执行校准请等待(4/14)",2,26);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 7:
			/* set */
			osc_message_box("正在执行校准请等待(5/14)",2,28);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 8:
			/* set */
			osc_message_box("正在执行校准请等待(6/14)",2,30);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 9:
			/* set */
			osc_message_box("正在执行校准请等待(7/14)",2,32);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;		
		case 10:
			/* set */
			osc_message_box("正在执行校准请等待(8/14)",2,34);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 11:
			/* set */
			osc_message_box("正在执行校准请等待(9/14)",2,36);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 12:
			/* set */
			osc_message_box("正在执行校准请等待(10/14)",2,38);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 13:
			/* set */
			osc_message_box("正在执行校准请等待(11/14)",2,40);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 14:
			/* set */
			osc_message_box("正在执行校准请等待(12/14)",2,42);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 15:
			/* set */
			osc_message_box("正在执行校准请等待(13/14)",2,44);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 16:
			/* set */
			osc_message_box("正在执行校准请等待(14/14)",2,46);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;	
		case 17:
			/* set */
			osc_message_box("校准成功",3,52);
		  /* clear */
			message_box_index_g = 0xfe;
			/* save */
			osc_param_save_noload();
			/*-----------*/			
			break;
		case 18:
			/* set */
			osc_message_box("校准失败",3,54);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;		
		case 19:
			/* set */
			osc_message_box(msg_range_file_name,4,56);
		  /* clear */
			message_box_index_g = 0xfe;
			/*---------*/
		  __set_PRIMASK(1);
		  /* set */
			GLOBAL_irq = 1;
			/*------------*/
			break;
		case 20:
			/* set */
			osc_message_box("保存失败,重试?",1,58);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;	
		case 21:
			/* set */
			osc_message_box("保存成功,是否打开U盘?",1,60);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/			
			break;
		case 22:
			/* set */
			osc_message_box("SD卡不存在或文件系统错误",3,62);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/				
			break;
		case 23:
			/* set */
			osc_message_box("当前暂未支持敬请期待",3,62);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/	
			break;
		case 24:
			/* set */
			osc_message_box("未识别到触摸屏",3,62);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/	
			break;
		case 25:
			/* set - auto set dialog */
			osc_message_box("Auto...",0,0);
		  /* clear */
			message_box_index_g = 0xfe;
			/*-----------*/
			break;
		default:
			break;
	}
	/* return */
	return FS_OK;
}
/* common dat */
static void osc_gui_msg_button(widget_def * wid)
{
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
  /* calutie and ending pos */
	unsigned short  pos_x = wid->msg.x , pos_y = wid->msg.y;
  /* calutie and ending pos */
	unsigned short pos_x_end = wid->msg.x_size , pos_end_y = wid->msg.y_size;
	/* pw */
	unsigned short pw = wid->parent->msg.x_size;
  /* at circle mode create gui */
	for( int i = 0 ; i < pos_end_y - 2 ; i ++ )
	{
		for( int j = 0 ; j < pos_x_end - 2 ; j ++ )
		{
			if( i == 0 && j == 0 )
			{
			}
			else if( i == 0 && (j == pos_x_end - 2 - 1) )
			{

			}
			else if( (i == pos_end_y - 2 - 1) && j == 0 )
			{

			}
			else if( (i == pos_end_y - 2 - 1) && (j == pos_x_end - 2 - 1) )
			{
			}
			else
			{
				set_l2_point_noload(pw, j + pos_x + 1 ,pos_y + i + 1,COLOR_L2_GROUP_BG);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			set_l2_point_noload(pw, i % 4 + pos_x ,i / 4 + pos_y,COLOR_L2_BUTTON_BG);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			set_l2_point_noload(pw, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_L2_BUTTON_BG);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			set_l2_point_noload(pw, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_L2_BUTTON_BG);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			set_l2_point_noload(pw, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_L2_BUTTON_BG);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		set_l2_point_noload(pw,pos_x + 4 + i , pos_y , COLOR_L2_BUTTON_BG);
		set_l2_point_noload(pw,pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_L2_BUTTON_BG);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		set_l2_point_noload(pw,pos_x , pos_y + 4 + i , COLOR_L2_BUTTON_BG);
		set_l2_point_noload(pw,pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_L2_BUTTON_BG);
	}
  /* show */
	if( wid->msg.pri_data != 0 )
	{
		/* show HZ */
		int len = strlen(wid->msg.pri_data);
		/* def 00 */
		unsigned short xt = ( wid->msg.x_size - len * 8 ) / 2 ;
		unsigned short yt = ( wid->msg.y_size - 16 ) / 2 ;
		/* ads */
		xt += wid->msg.x;
		yt += wid->msg.y;
		/* set */
		const char * dst = (const char *)wid->msg.pri_data;
		/* show */
		osc_gui_gb2312_string_l2(pw,dst,xt,yt,0);	
	}
}
/* msg touch */
void osc_msgbox_touch(unsigned short xr,unsigned short yr)
{
	/* --------- */
	unsigned short abx;
	unsigned short aby;
	/* size */
	unsigned short abx_size;
	unsigned short aby_size;
	/* chec */
	if( msg_flag )
	{
		return;
	}
	/* for */
	for( int i = 0 ; i < 3 ; i ++ )
	{
		/* check upd */
		if( group_msg[i].msg.upd == 0 )
		{
			continue;
		}
		/* skip */
		abx = group_msg[i].msg.abx;
		aby = group_msg[i].msg.aby;
		abx_size = abx + group_msg[i].msg.x_size;
		aby_size = aby + group_msg[i].msg.y_size;
		/* check area */
		if( (xr > abx) && (xr < abx_size) && 
			(yr > aby )&&( yr < aby_size ))
		{
			msg_touch_cnt[i] ++;
			/* if */
			if( msg_touch_cnt[i] >= 3 )
			{
				/* focus key */
				osc_btn_l2_color(&group_msg[i],1);
				/* set 0 */
				msg_flag = 1 ;
			}
		}
		else
		{
			msg_touch_cnt[i] = 0;
		}	
	}
}
/* open dev */
void osc_open_usb(void)
{
	/* set magic data */
	magic[0] = MAGIC_DATA0;
	magic[1] = MAGIC_DATA1;
	/* reset */
	NVIC_SystemReset();	
}
/* msg event */
void osc_msg_event(unsigned short ID)
{
	/* case */
	switch(ID)
	{
		case 1:
		case 5:
		case 10:
			/* close */
			osc_msg_close();
			/* break */
			break;
		case 2:
			/* step 1 */
			osc_msg_box(3);
			/* set vol scale */
      osc_calibrate_step_init(0);
			/*---------------*/
			break;
		case 6:
			/* open usb device */
			osc_open_usb();
			/*-----------------*/
			break;
		case 11:
			/* erase flash and reset */
			osc_reset_param();
			break;		
		case 20:
		case 22:
		case 24:
		case 26:
		case 28:
		case 30:
		case 32:	
		case 34:
		case 36:
		case 38:
		case 40:
		case 42:
		case 44:
		case 46:
			/* cancel */
			osc_msg_close();
			/* flag */
			cal_step_start = 0;
			/*--------*/
			break;	
		case 52:
			/* close ok */
			osc_msg_close();		
			break;	
		case 54:
			/* close error */
			osc_msg_close();
			break;	
		case 58:
			/* close ok */
			osc_msg_close();		
			/* endif */
			__set_PRIMASK(0);
			/* restart */
			osc_run_and_stop_trs();
			/* ---------*/
			break;		
		case 59:
			/* retry */
			osc_usbh_save_start();
			/* end */
			break;
		case 60:
			/* close ok */
			osc_msg_close();				
			break;
		case 61:
			/* open usb device */
			osc_open_usb();
			/*-----------------*/			
			break;			
		case 62:
			/* close ok */
			osc_msg_close();				
			break;
		default:
			break;
	}
}
/* release touch */
void osc_msg_touch_release(void)
{
	/* clear */
	memset(msg_touch_cnt,0,sizeof(msg_touch_cnt));
	/* id */
	int i = 0;
	/* switch command*/	
	for( i = 0 ; i < 3 ; i ++ )
	{
		/* check */
		if( group_msg[i].msg.upd == 0x0B  )
		{
			osc_btn_l2_color(&group_msg[i],0);
			/* release */
			msg_flag = 0;
			/* break */
			break;
		}
	}
	/* set */	
	if( i < 3 )
	{
		osc_msg_event(i + group_msg[i].msg.wflags);
	}
}
/* close msg */
void osc_msg_close(void)
{
	message_box_index_g = 0xff;
	/* l2 disable */
	osc_dev_l2_disable();	
}