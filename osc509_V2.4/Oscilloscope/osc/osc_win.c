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
#include "osc_win.h"
#include "string.h"
#include "middle.h"
#include "gui.h"
#include "osc_win_bmp.h"
#include "osc_menu.h"
#include "osc.h"
#include "osc_ui.h"
#include "hal_tim.h"
#include "osc_api.h"
#include "osc_cfg.h"
#include "stdio.h"
/* init node */
FOS_INODE_REGISTER("osc_win_init",osc_win_init,0,0,14);
/* create cfg task gui detecter task run as 100 ms */
FOS_TSK_REGISTER(osc_l2_task,PRIORITY_4,10);
/* create */
static display_info_def * dev;
/* create the but */
const char * persist_table[6] = {"Persist","OFF ","1sec","2sec","5sec","inf "};//3
const char * fAI[3] = {"SmartAI","ON","OFF"};//1
const char * format[3] = {"Format","Y-T","X-Y"};//1
const char * language[4] = {"language","English","简体中文","繁体中文"};//2
const char * usb_tab[3] = {"USB","OFF","ON"};//1
const char * test_mode[3] = {"save","Bmp","Bin"};//1
/* reboot */
const char * btn_tab1[1] = {"恢复出厂"};
const char * btn_tab2[1] = {"储存设备"};
const char * btn_tab3[1] = {"基线校准"};
/* ch1 */
const char * ch12_tab_OF[2] = {"ON","OFF"};
const char * ch12_tab_XX[2] = {"X10","X1"};//2
const char * ch12_tab_DC[2] = {"DC","AC"};
/* trig */
const char * trig_tab_sur[2] = {"CH1","CH2"};
const char * trig_tab_edge[2] = {"Rising","Falling"};
const char * trig_tab_type[3] = {"Auto ","Normal","Single"};
/* cd */
const char * measure_item_chinese[19] = {"频率","周期","峰峰值","最大值","最小值","有效值","占空比","脉宽","脉冲数","相位","垂直光标","水平光标","细调","A + B","A - B","A * B","A / B","CH1","CH2"};
const char * measure_item_english[19] = {"Freq-\nuency","Period"," Peak \n Peak ","Maximum","Minimum","RMS"," Pos- \n Duty "," Pos- \n Width","Pulse\n Cnt  ","Parse"," Amp \nCursor","Time \nCursor","Precise\nAdjust","A + B","A - B","A * B","A / B","CH1","CH2"};//"Rise Time","Fall Time","Pos Duty","Neg Width"
const char * measure_item_english_simple[13] = {"FreQ","PerD","Pk-Pk","Max","Min","RMS","P-Duty","PWidth","P-Cnt","Phase","dVy","dTx","Adj"};
//const char * cursor_chinese[3] = {"垂直光标","水平光标","细调"};
//const char * cursor_english[3] = {" Amp \nCursor","Time \nCursor","Precise\nAdjust"};
/* chand */
widget_def group[30];
window_def win_l2_main;
/* static */
static unsigned int touch_in_cnt[31];
static unsigned short TOUCH_ID = 0xffff;
/* static slider posx */
static unsigned short touch_slider_px = 0;
/* task flag */
static unsigned short osc_l2_task_flag = 0;
/* extern */
extern widget_def btn_ch[4];
/* run msg */
static osc_run_msg_def * runmsg;
/* pwm */
extern void user_back_light(unsigned short pwm);
extern char * app_version(void);
extern void osc_exit_edge_reg(unsigned char mo);
extern int osc_msg_box(unsigned char index);
extern char * boot_version(void);
extern void osc_gui_gb2312_string_l2(unsigned short pw, const char * hzd, unsigned short x, unsigned short y, unsigned int color);
extern unsigned char osc_trig_source(void);
extern void osc_dev_l2_disable(void);
extern void osc_dev_l2_enable(unsigned short x, unsigned short y, unsigned short x_size, unsigned short y_size, unsigned char alpha);
extern unsigned int osc_dev_resize(void);
extern void osc_offset_scale_thread(unsigned char chn);
unsigned char osc_fm = 0xff;
/* fine tune panel */
widget_def fine_tune_btn[8];
window_def win_fine_tune;
static unsigned int fine_tune_touch_cnt[9];
static unsigned short FINE_TUNE_TOUCH_ID = 0xffff;
static unsigned short fine_tune_trigger_btn = 0xffff;
/* init */
static int osc_win_init(void)
{
	dev = get_display_dev_info();
	/* get */
	runmsg = get_run_msg();
	/* return ok */
	return FS_OK;
}
/* show task */
static void osc_l2_task(void)
{
	/* check */
	if( osc_l2_task_flag == 0 )
	{
		return;
	}
}
/* create win */
void osc_create_ch12t_menu(unsigned char index)
{
	/* check */
	if( index >= 3 )
	{
		return;
	}
	/* get size */
	unsigned int resize = osc_dev_resize();
	/* create size */
	unsigned short x_size;
	unsigned short y_size;
	/* pos */
	unsigned short posx ;
	unsigned short posy ;
	/* check */
	if( index == 2 )
	{
		x_size = btn_ch[index].msg.x_size * 2 ;	
		y_size = 160;		
		posx = btn_ch[index].msg.x - btn_ch[index].msg.x_size / 2;
		posy = btn_ch[index].msg.y + btn_ch[index].msg.y_size + 2;		
	}
	else
	{
		x_size = btn_ch[index].msg.x_size;	
		y_size = 160;		
		posx = btn_ch[index].msg.x;
		posy = btn_ch[index].msg.y + btn_ch[index].msg.y_size + 2;
	}
	/* check size */
	if( x_size * y_size > resize )
	{
		return;//over size
	}
	/* memset */
	memset(&win_l2_main,0,sizeof(win_l2_main));
	memset(&group,0,sizeof(group));
	/* create wind */
	win_l2_main.msg.x = 0;
	win_l2_main.msg.y = 0;
	win_l2_main.msg.x_size = x_size;
	win_l2_main.msg.y_size = y_size;
	/* draw */
	win_l2_main.draw = osc_draw_l2_win;
	/* upd */
	win_l2_main.msg.upd = 1;
	win_l2_main.msg.mark_flag = index+2;
	/* abx */
	win_l2_main.msg.abx = posx;
	win_l2_main.msg.aby = posy;	
	/* create widget */
	osc_create_ch12_menu(&win_l2_main,group);
	/* create */
	gui_show_win_noload(&win_l2_main);
	/* set default */
		/* index */
	if( index == 0 )
	{
		osc_select(&group[0],osc_param_sys_get(runmsg->sys_menu_set,6));
		osc_select(&group[1],osc_param_sys_get(runmsg->sys_menu_set,7));
		osc_select(&group[2],osc_param_sys_get(runmsg->sys_menu_set,8));
	}
	else if( index == 1 )
	{
		osc_select(&group[0],osc_param_sys_get(runmsg->sys_menu_set,9));
		osc_select(&group[1],osc_param_sys_get(runmsg->sys_menu_set,10));
		osc_select(&group[2],osc_param_sys_get(runmsg->sys_menu_set,11));
	}
	else
	{
		osc_select(&group[0],osc_param_sys_get(runmsg->sys_menu_set,13));
		osc_select(&group[1],osc_param_sys_get(runmsg->sys_menu_set,14));
		osc_select(&group[2],osc_param_sys_get(runmsg->sys_menu_set,15));	
	}
	/* set */
	osc_dev_l2_enable(posx,posy,x_size,y_size,255);
	/* end */
}
/* create ch1 */
void osc_create_system_menu(void)
{
	/* get size */
	unsigned int resize = osc_dev_resize();
	/* create size */
	unsigned short x_size = 380;
	unsigned short y_size = 330;
	/* pos */
	unsigned short posx = ( dev->display_dev->pwidth - x_size ) / 2;
	unsigned short posy = ( dev->display_dev->pheight - y_size ) / 2;
	/* check size */
	if( x_size * y_size > resize )
	{
		return;//over size
	}
	/* memset */
	memset(&win_l2_main,0,sizeof(win_l2_main));
	memset(&group,0,sizeof(group));
	/* create wind */
	win_l2_main.msg.x = 0;
	win_l2_main.msg.y = 0;
	win_l2_main.msg.x_size = x_size;
	win_l2_main.msg.y_size = y_size;
	/* draw */
	win_l2_main.draw = osc_draw_l2_win;
	/* upd */
	win_l2_main.msg.upd = 0;	
	/* set window */
	win_l2_main.msg.mark_flag = 1;
	/* abx */
	win_l2_main.msg.abx = posx;
	win_l2_main.msg.aby = posy;
	/* create widget */
	osc_create_sys_menu(&win_l2_main,group);
	/* create */
	gui_show_win_noload(&win_l2_main);
#if 0	
	osc_draw_time_btn_bmp(x_size,0,0,152,52,gImage_time);
#endif
	/* show default param */
	osc_select(&group[0],osc_param_sys_get(runmsg->sys_menu_set,0));
	osc_select(&group[1],osc_param_sys_get(runmsg->sys_menu_set,1));
	osc_select(&group[2],osc_param_sys_get(runmsg->sys_menu_set,2));
	osc_select(&group[3],osc_param_sys_get(runmsg->sys_menu_set,3));
	osc_select(&group[8],osc_param_sys_get(runmsg->sys_menu_set,4));
	osc_select(&group[9],osc_param_sys_get(runmsg->sys_menu_set,5));	
	//osc_btn_l2_color(&group[5],1);
	osc_slider_pos(&group[4],runmsg->back_light_per);
	/* set */
	osc_dev_l2_enable(posx,posy,x_size,y_size,255);
	/* set flag */
//	osc_l2_task_flag = 1;
//	osc_sys_menu_alph = 50;
	/* end */
}
/* create the group */
static void osc_create_ch12_menu(window_def * parent_win,widget_def * grp)
{
	/* char */
	const char ** t_char_tab[3] = {ch12_tab_OF,ch12_tab_XX,ch12_tab_DC};
	const unsigned char t_item[3] = {2,2,2};
	/* trig */
	const char ** t_char_trig[3] = {trig_tab_sur,trig_tab_edge,trig_tab_type};
	const unsigned char t_item_trig[3] = {2,2,3};
	/* set tpp */
	unsigned short tpp = (parent_win->msg.y_size - 8*4) / 3;	
	/* check */
  for( int i = 0 ; i < 3 ; i ++ )
	{
		/* set pos */
		grp[i].msg.x = 5 ;
		grp[i].msg.y = 8 + (tpp + 8 )* i;
		grp[i].msg.y_size = tpp;
		/* x size */
		
		/* title */
		if( parent_win->msg.mark_flag == 4 )
		{
			grp[i].msg.pri_data = t_char_trig[i];
			grp[i].msg.mx = t_item_trig[i];			
			grp[i].msg.mxstop = i + 40;
			/* check i */
			if( i == 0 || i == 1 )
			{
				grp[i].msg.x_size = parent_win->msg.x_size / 2 + 20;
			}
			else
			{
				grp[i].msg.x_size = parent_win->msg.x_size - 10;
			}
		}
		else
		{
			grp[i].msg.pri_data = t_char_tab[i];
			grp[i].msg.mx = t_item[i];
			grp[i].msg.mxstop = i + 30;
			grp[i].msg.x_size = parent_win->msg.x_size - 10;
		}
		/* pri */
		grp[i].msg.mystop = parent_win->msg.x_size;
		/* draw */
		grp[i].draw = draw_group_wi2;
		/* parent */
		grp[i].parent = parent_win;
		/* set upd */
		grp[i].msg.upd = 0xff;
		/* create */
		gui_widget_creater(&grp[i]);
	}
}
/* create the math and measure menu */
void osc_create_measure_math_menu(void)
{
	/* get size */
	unsigned int resize = osc_dev_resize();
	/* create size */
	unsigned short x_size = 672;
	unsigned short y_size = 203;
	/* pos */
	unsigned short posx = ( dev->display_dev->pwidth - x_size ) / 2 - 30;
	unsigned short posy = ( dev->display_dev->pheight - y_size ) / 2;
	/* check size */
	if( x_size * y_size > resize )
	{
		return;//over size
	}
	/* memset */
	memset(&win_l2_main,0,sizeof(win_l2_main));
	memset(&group,0,sizeof(group));
	/* create wind */
	win_l2_main.msg.x = 0;
	win_l2_main.msg.y = 0;
	win_l2_main.msg.x_size = x_size;
	win_l2_main.msg.y_size = y_size;
	/* draw */
	win_l2_main.draw = osc_draw_l2_win;
	/* upd */
	win_l2_main.msg.upd = 1;	
	/* set window */
	win_l2_main.msg.mark_flag = 6;
	/* abx */
	win_l2_main.msg.abx = posx;
	win_l2_main.msg.aby = posy;
	/* create widget */
	osc_create_measure_menu(&win_l2_main,group);
	/* create */
	gui_show_win_noload(&win_l2_main);
	/* cet */
	osc_measure_math_reflush();	
	/* */
	osc_dev_l2_enable(posx,posy,x_size,y_size,255);
	/* end */
}
/* create the group */
static void osc_create_sys_menu(window_def * parent_win,widget_def * grp)
{
	/* char */
	const char ** t_char_tab[10] = {persist_table,language,fAI,format,0,btn_tab1,btn_tab2,btn_tab3,usb_tab,test_mode};
	const unsigned char t_item[10] = {5,2,2,2,0,0,0,0,2,2};
	/* check */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* set */
		if( i == 4 )
		{
			grp[i].msg.x = 10;
			grp[i].msg.y_size = 8;
			grp[i].msg.y = 25 + 50*i + 15;
			/* slider */
			grp[i].msg.my = 2;
			/* show */
		}
		else if( i >= 5 && i <= 7)
		{
			unsigned short tpp = (parent_win->msg.x_size - 40) / 3;
			grp[i].msg.x = 10 + (tpp + 10) * (i - 5);
			grp[i].msg.y = 25 + 50*5 - 5;
			grp[i].msg.y_size = 46;
		}
		else if( i >= 8 )
		{
			grp[i].msg.x = parent_win->msg.x_size / 2 + 58;
			grp[i].msg.y_size = 36;
			grp[i].msg.y = 25 + 50 * ( i - 6 );			
		}
		else
		{
			grp[i].msg.x = 80;
			grp[i].msg.y_size = 36;
			grp[i].msg.y = 25 + 50*i;
		}
		
		/* x size */
		if( i == 0 )
		{
			grp[i].msg.x_size = 8*8*3 + 3*30;
		}
		else if( i == 1 )
		{
			grp[i].msg.x_size = 8*8*2 + 2*30;
		}
		else if ( i == 2 || i == 3 )
		{
			grp[i].msg.x_size = (grp[0].msg.x_size - 4*8*5) / 5 * 2 + 4*8*2;
		}
		else if( i == 3 )
		{
			grp[i].msg.x_size = (grp[0].msg.x_size - 4*8*5) / 5 * 2 + 4*8*2;
		}
		else if( i >= 5 )
		{
			grp[i].msg.x_size = (parent_win->msg.x_size - 40) / 3;
		}
		else
		{
			grp[i].msg.x_size = parent_win->msg.x_size - 20;
		}
		/* title */
		grp[i].msg.pri_data = t_char_tab[i];
		/* pri */
		grp[i].msg.mxstop = i;
		grp[i].msg.mystop = parent_win->msg.x_size;
		/* draw */
		grp[i].draw = draw_group_wi2;
		/* parent */
		grp[i].parent = parent_win;
		/* set osc fin */
		grp[i].msg.mx = t_item[i];
		/* set upd */
		grp[i].msg.upd = 0xff;
		/* create */
		gui_widget_creater(&grp[i]);
	}
}
/* item measure */
static void osc_create_measure_menu(window_def * parent_win,widget_def * grp)
{
//	/* char */
//	const char ** t_char_tab[10] = {persist_table,language,fAI,format,0,btn_tab1,btn_tab2,btn_tab3,find_tab,test_mode};
//	const unsigned char t_item[10] = {5,3,2,2,0,0,0,0,2,2};
	unsigned short wid = ( parent_win->msg.x_size - 2 * 11 ) / 10;;
	/* check */
	for( int i = 0 ; i < 30 ; i ++ )
	{
    /* set */
		grp[i].msg.x = 2 + (wid+2)*(i%10);
		grp[i].msg.y = 2 + ( 65 + 2 ) * ( i / 10 );
		grp[i].msg.x_size = wid;
		grp[i].msg.y_size = 65;
		/* x size */
		/* title */
		//grp[i].msg.pri_data = t_char_tab[i];
		/* pri */
		grp[i].msg.mxstop = 60 + i;
		grp[i].msg.mystop = parent_win->msg.x_size;
		/* draw */
		grp[i].draw = draw_group_wi2;
		/* parent */
		grp[i].parent = parent_win;
		/* set osc fin */
		grp[i].msg.mx = 1;
		/* set upd */
		grp[i].msg.upd = 0xff;
		/* create */
		gui_widget_creater(&grp[i]);
	}
}
/* draw_window frame */
static void osc_draw_l2_win(window_def * win)
{
	/* check */
	if( win->msg.upd == 0 )
	{
		/* clear */
		memset(dev->gram_l2,COLOR_L2_GRAD,win->msg.x_size * win->msg.y_size);
		/* title */
#if 0
		memset(dev->gram_l2,COLOR_L2_WBLUE,win->msg.x_size * 20 );
#endif
		/* title */
		char * ts = app_version();
		char * tb = boot_version();
		/* rd */
		char tbs[32];
		/* check */
		sprintf(tbs,"%s %s",ts,tb);
		/* pos */
		unsigned short pos_t = (win->msg.x_size - strlen(tbs) * 8 ) / 2;
		/* show */
		gui_l2_string(win->msg.x_size,pos_t,2,tbs,1);
		/* set point */
#if 0
		for( int i = 0 ; i < win->msg.y_size ; i ++ )
		{
			/* set */
			set_l2_point_noload(win->msg.x_size,0,i,COLOR_L2_WBLUE);
			set_l2_point_noload(win->msg.x_size,1,i,COLOR_L2_WBLUE);
			set_l2_point_noload(win->msg.x_size,2,i,COLOR_L2_WBLUE);
			set_l2_point_noload(win->msg.x_size,win->msg.x_size - 1,i,COLOR_L2_WBLUE);
			set_l2_point_noload(win->msg.x_size,win->msg.x_size - 2,i,COLOR_L2_WBLUE);
			set_l2_point_noload(win->msg.x_size,win->msg.x_size - 3,i,COLOR_L2_WBLUE);
		}
		/* set log */
		for( int i = 0 ; i < win->msg.x_size ; i ++ )
		{
			/* set */
			set_l2_point_noload(win->msg.x_size,i,win->msg.y_size - 1,COLOR_L2_WBLUE);		
			set_l2_point_noload(win->msg.x_size,i,win->msg.y_size - 2,COLOR_L2_WBLUE);	
			set_l2_point_noload(win->msg.x_size,i,win->msg.y_size - 3,COLOR_L2_WBLUE);	
		}		
#endif
	}
	else
	{
		/* clear */
		memset(dev->gram_l2,COLOR_L2_GRAD,win->msg.x_size * win->msg.y_size);		
	}
	/* end */
}
/* draw group win */
static void draw_group_wi2(struct widget * wid)
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
	/* pwitdh */
	unsigned short pwidth = wid->msg.mystop;
	/* set */
	unsigned short xsize = wid->msg.x_size;
	unsigned short ysize = wid->msg.y_size;
	/* tit */
	const char ** char_table = wid->msg.pri_data;
	/* check */
	unsigned char color_btn,color_line;
	/* check */
	if( wid->msg.mxstop >= 60 && wid->msg.mxstop < 70 )
	{
		color_btn = COLOR_L2_GROUP_BG;
		color_line = COLOR_L2_CH1;		
	}
	else if( wid->msg.mxstop >= 70 && wid->msg.mxstop < 80 )
	{
		color_btn = COLOR_L2_GROUP_BG;
		color_line = COLOR_L2_CH2;			
	}
	else if( wid->msg.mxstop >= 80 && wid->msg.mxstop < 83 )
	{
		color_btn = COLOR_L2_GROUP_BG;
		color_line = COLOR_L2_MATH;			
	}
	else if( wid->msg.mxstop >= 83 && wid->msg.mxstop < 87 )
	{
		color_btn = COLOR_L2_GROUP_BG;
		color_line = COLOR_L2_CURC;		
	}
	else if( wid->msg.mxstop >= 87 && wid->msg.mxstop < 90 )
	{
		color_btn = COLOR_L2_GROUP_BG;
		color_line = COLOR_L2_FFTT;		
	}	
	else
	{
		color_btn = COLOR_L2_GROUP_BG;
		color_line = COLOR_L2_GROUP_BG;			
	}
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
				set_l2_point_noload(pwidth ,j + pos_x + 1 ,pos_y + i + 1,color_btn);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			set_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y,color_line);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			set_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,color_line);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			set_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,color_line);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			set_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,color_line);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		set_l2_point_noload(pwidth,pos_x + 4 + i , pos_y , color_line);
		set_l2_point_noload(pwidth,pos_x + 4 + i , pos_y + pos_end_y - 1, color_line);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		set_l2_point_noload(pwidth,pos_x , pos_y + 4 + i , color_line);
		set_l2_point_noload(pwidth,pos_x + pos_x_end - 1, pos_y + 4 + i , color_line);
	}
	/* check */
	if( wid->msg.mxstop == 0 )
	{
		/* show char */
		unsigned short grid = ( xsize - 5*4*8 ) / 5;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		/* shoow */
		gui_l2_string(pwidth,10, pos_y + ( ysize - 16 ) / 2 , (char *)char_table[0],1);
		//osc_draw_l2_bmp(pwidth,5,pos_y + ( ysize - 16 ) / 2,39,17,gImage_fftm,1);
		/* show */
		for( int i = 0 ; i < 5 ; i ++ )
		{
			/* show */
			gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i+1],0);
			/* ins */
			stx += grid + 4*8;
		}
	}
	else if( wid->msg.mxstop == 2 || wid->msg.mxstop == 3 )
	{
		/* show char */
		unsigned short grid = ( xsize - 2*3*8 ) / 2;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		gui_l2_string(pwidth,10, pos_y + ( ysize - 16 ) / 2 , (char *)char_table[0],1);
		//osc_draw_l2_bmp(pwidth,5,pos_y + ( ysize - 16 ) / 2,39,17,gImage_fftm,1);
		/* show */
		for( int i = 0 ; i < 2 ; i ++ )
		{
			/* show */
			gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i+1],0);
			/* ins */
			stx += grid + 3*8;
		}		
	}
	else if( wid->msg.mxstop == 8 || wid->msg.mxstop == 9 )
	{
		/* show char */
		unsigned short grid = ( xsize - 2*3*8 ) / 2;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		gui_l2_string(pwidth,pos_x - 40, pos_y + ( ysize - 16 ) / 2 , (char *)char_table[0],1);
		/* show */
		for( int i = 0 ; i < 2 ; i ++ )
		{
			/* show */
			gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i+1],0);
			/* ins */
			stx += grid + 3*8;
		}		
	}
	else if( wid->msg.mxstop == 1 )
	{
		/* show char */
		unsigned short grid = ( xsize - 8*2*8 ) / 2;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		gui_l2_string(pwidth,10, pos_y + ( ysize - 16 ) / 2 , (char *)char_table[0],1);
		//osc_draw_l2_bmp(pwidth,5,pos_y + ( ysize - 16 ) / 2,39,17,gImage_fftm,1);
		/* show */
		for( int i = 0 ; i < 2 ; i ++ )
		{
			/* if */
			if( i == 0 )
			{
				/* english */
				gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i+1],0);
				/* endif */
			}
			else
			{
				/* show */
				const char * dst = (const char *)char_table[i+1];
				/* show */
				draw_hz_l2(pwidth,&dst[0],stx + 16*0, pos_y + ( ysize - 18 ) / 2,0);
				draw_hz_l2(pwidth,&dst[2],stx + 16*1, pos_y + ( ysize - 18 ) / 2,0);
				draw_hz_l2(pwidth,&dst[4],stx + 16*2, pos_y + ( ysize - 18 ) / 2,0);
				draw_hz_l2(pwidth,&dst[6],stx + 16*3, pos_y + ( ysize - 18 ) / 2,0);
			}
			/* ins */
			stx += grid + 8*8;
		}		
	}
	else if( wid->msg.mxstop == 4 )
	{
		//osc_draw_l2_bmp(pwidth,pos_x + 30,pos_y - 9,26,26,gImage_s,COLOR_L2_SLIDER);
		//osc_slider_pos(wid,0);
	}
	else if( wid->msg.mxstop >= 30 && wid->msg.mxstop <= 40 )
	{
		/* show char */
		unsigned short grid = ( xsize - 2*3*8 ) / 2;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		/* show */
		for( int i = 0 ; i < 2 ; i ++ )
		{
			/* show */
			gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i],0);
			/* ins */
			stx += grid + 3*8;
		}			
	}
	else if( wid->msg.mxstop == 41 )
	{
		/* show char */
		unsigned short grid = ( xsize - 2*7*7 ) / 2;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		/* show */
		for( int i = 0 ; i < 2 ; i ++ )
		{
			/* show */
			gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i],0);
			/* ins */
			stx += grid + 7*7;
		}		
	}	
	else if( wid->msg.mxstop == 42 )
	{
		/* show char */
		unsigned short grid = ( xsize - 3*6*6 ) / 3;
		unsigned short stx = pos_x + grid / 2;		
		/* show title */
		/* show */
		for( int i = 0 ; i < 3 ; i ++ )
		{
			/* show */
			gui_l2_string(pwidth,stx, pos_y + ( ysize - 18 ) / 2 , (char *)char_table[i],0);
			/* ins */
			stx += grid + 6*6;
		}		
	}
	else if( wid->msg.mxstop >= 5 && wid->msg.mxstop <= 7 )
	{
		/* btn */
		/* show char */
		unsigned short grid = ( xsize - 4*16 ) / 2;
		unsigned short stx = pos_x + grid;		
		/* show title */
		const char * tib = char_table[0];
		/* show */
		for( int i = 0 ; i < 4 ; i ++ )
		{
			/* show */
			draw_hz_l2(pwidth,&tib[i*2],stx, pos_y + ( ysize - 16 ) / 2 ,0 );
			/* ins */
			stx += 16;
		}				
	}
	else if(wid->msg.mxstop >= 60 && wid->msg.mxstop < 89 )
	{
		/* set */
		const char * tmd;
		unsigned short srtx;
		/* ch */
		if( wid->msg.mxstop % 2 )
		{
			//osc_draw_right_icon(wid);
		}
		/* check */
		if( wid->msg.mxstop >= 60 && wid->msg.mxstop < 70 )
		{
			gui_l2_string(pwidth,wid->msg.x + 20,wid->msg.y,"CH1",COLOR_L2_CH1);
			/* get */
			tmd = osc_language_get() == 0 ? measure_item_english[wid->msg.mxstop - 60] : measure_item_chinese[wid->msg.mxstop - 60];
		}
		else if( wid->msg.mxstop >= 70 && wid->msg.mxstop < 80 )
		{
			gui_l2_string(pwidth,wid->msg.x + 20,wid->msg.y,"CH2",COLOR_L2_CH2);
			/* get */
			tmd =  osc_language_get() == 0 ? measure_item_english[wid->msg.mxstop - 70] : measure_item_chinese[wid->msg.mxstop - 70];
		}
		else if( wid->msg.mxstop >= 83 && wid->msg.mxstop < 87 )
		{
			gui_l2_string(pwidth,wid->msg.x + 15,wid->msg.y + 2,"Math",0);
			/* get */
			tmd =  osc_language_get() == 0 ? measure_item_english[wid->msg.mxstop - 70] : measure_item_chinese[wid->msg.mxstop - 70];			
			/* cali start pos */
			srtx = ( wid->msg.x_size - 5 * 8 ) / 2;
			/* show */
			gui_l2_string(pwidth,wid->msg.x + srtx,wid->msg.y + 30,( char *)tmd,0);
			/* set */			
		}
		else if( wid->msg.mxstop >= 87 && wid->msg.mxstop < 89 )
		{
			/* set FFT */
			gui_l2_string(pwidth,wid->msg.x + 20,wid->msg.y + 2,"FFT",0);
			/* get */
			tmd =  osc_language_get() == 0 ? measure_item_english[wid->msg.mxstop - 70] : measure_item_chinese[wid->msg.mxstop - 70];			
			/* cali start pos */
			srtx = ( wid->msg.x_size - 3 * 8 ) / 2;
			/* show */
			gui_l2_string(pwidth,wid->msg.x + srtx,wid->msg.y + 30,( char *)tmd,0);
			/* set */						
		}
		else
		{
		  /* get */
			tmd =  osc_language_get() == 0 ? measure_item_english[wid->msg.mxstop - 70] : measure_item_chinese[wid->msg.mxstop - 70];			
		}
		/* GET LENG */
	  unsigned int slen = strlen(tmd);
		/* check */
		if( osc_language_get() == 0 )
		{
			/* check */
			if( slen < 8 )
			{
				/* cali start pos */
				srtx = ( wid->msg.x_size - slen * 8 ) / 2;
				/* show */
				gui_l2_string(pwidth,wid->msg.x + srtx,wid->msg.y + 30,( char *)tmd,0);
				/* set */
			}
			else
			{
				/* cali start pos */
				srtx = ( wid->msg.x_size - 8 * 6 ) / 2;
				/* show */
				gui_l2_string(pwidth,wid->msg.x + srtx,wid->msg.y + 18,( char *)tmd,0);
				/* set */				
			}
		}
		else
		{
			/* show */
			if( slen == 4 )
			{
				draw_hz_l2(pwidth,tmd,wid->msg.x + 15,wid->msg.y + 30,0);
				draw_hz_l2(pwidth,&tmd[2],wid->msg.x + 35,wid->msg.y + 30,0);
			}
			else if( slen == 6 )
			{
				draw_hz_l2(pwidth,tmd,wid->msg.x + 10,wid->msg.y + 30,0);
				draw_hz_l2(pwidth,&tmd[2],wid->msg.x + 26,wid->msg.y + 30,0);		
				draw_hz_l2(pwidth,&tmd[4],wid->msg.x + 42,wid->msg.y + 30,0);		
			}
			else if( slen == 8 )
			{
				draw_hz_l2(pwidth,tmd,wid->msg.x + 13,wid->msg.y + 15,0);
				draw_hz_l2(pwidth,&tmd[2],wid->msg.x + 35,wid->msg.y + 15,0);		
				draw_hz_l2(pwidth,&tmd[4],wid->msg.x + 13,wid->msg.y + 38,0);		
				draw_hz_l2(pwidth,&tmd[6],wid->msg.x + 35,wid->msg.y + 38,0);				
			}
			else
			{
				/* noting */
			}
		}
	}
}
/* draw bmp */
void osc_draw_l2_bmp(unsigned short pwidth , unsigned short x,unsigned short y,unsigned short x_size,unsigned short y_size,const unsigned char * tab,unsigned char color)
{
	/* unsigned short */
	unsigned short xs = x_size;
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < y_size ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* show */
			if( ( tab[ i*yow + j / 8 ] << (j % 8)) & 0x80 )
			{
				set_l2_point_noload(pwidth,x + j , y + i ,color);
			}
			else
			{
				set_l2_point_noload(pwidth,x + j, y + i ,COLOR_L2_GRAD);
			}
		}
	}		
}
#if 0
/* draw bmp */
void osc_draw_time_btn_bmp(unsigned short pwidth , unsigned short x,unsigned short y,unsigned short x_size,unsigned short y_size,const unsigned char * tab)
{
	/* unsigned short */
	unsigned short xs = x_size;
	/* yow */
	unsigned short yow = ( xs % 4 ) ? ( xs / 4 + 1 ) : ( xs / 4 );
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < y_size ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* check */
			unsigned char color_index = ( tab[ i*yow + j / 4 ] << ( (j % 4) * 2 )) & 0xC0;
			/* show */
			if( color_index == 0x00  )
			{
				set_l2_point_noload(pwidth,x + j , y + i ,0);
			}
			else if( color_index == 0x40  )
			{
				set_l2_point_noload(pwidth,x + j , y + i ,1);
			}
			else if( color_index == 0x80  )
			{
				set_l2_point_noload(pwidth,x + j , y + i ,2);
			}
			else
			{
				set_l2_point_noload(pwidth,x + j , y + i ,3);
			}
		}
	}		
}
#endif
/* draw group win */
static void osc_select(struct widget * wid,unsigned char index)
{
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
	/* set */
	unsigned char flag_clear;
	unsigned short pos_x;
	unsigned short pos_y;
	unsigned short pos_x_end;
	unsigned short pos_end_y;
	unsigned short pwidth;
	unsigned char rd_color;
	/* tmp */
	unsigned char ind_d = index;
	/* check return */
	if( wid->msg.upd == index && wid->msg.upd != 0xff )
	{
		return;
	}
	/* limit */
	if( index >= wid->msg.mx )
	{
		return;
	}
	/* set */
	RE_OSC_WIN:
	/* check */
	if( wid->msg.upd == 0xff )
	{
		flag_clear = 0;
	}
	else
	{
		/* check */
		if( wid->msg.upd != ind_d )
		{
			flag_clear = 1;
			index = wid->msg.upd;		
		}
		else
		{
			flag_clear = 0;
			index = wid->msg.upd;				
		}
	}	
  /* calutie and ending pos */
	pos_x = wid->msg.x + wid->msg.x_size / wid->msg.mx * index;
	pos_y = wid->msg.y + 2;
  /* calutie and ending pos */
	pos_x_end = wid->msg.x_size / wid->msg.mx;
	pos_end_y = wid->msg.y_size - 4;
	/* pwitdh */
	pwidth = wid->msg.mystop;
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
				/* rd_color */
				rd_color =  read_l2_point_noload(pwidth ,j + pos_x + 1 ,pos_y + i + 1);
				/* check */
				if( rd_color == COLOR_L2_GROUP_BG )
				{
					set_l2_point_noload(pwidth ,j + pos_x + 1 ,pos_y + i + 1,COLOR_L2_GROUP_SEL);
				}
				else if( rd_color == COLOR_L2_GROUP_SEL )
				{
					set_l2_point_noload(pwidth ,j + pos_x + 1 ,pos_y + i + 1,COLOR_L2_GROUP_BG);
				}
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			/* read */
			rd_color = read_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y) ;
			/* check */
			if( rd_color == COLOR_L2_GROUP_BG)
			{
				/* check */
				if( !flag_clear )
				{ 
					set_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y,COLOR_L2_GROUP_SEL);
				}
			}
			else if( rd_color == COLOR_L2_GROUP_SEL)
			{
				/* check */
				if( flag_clear )
				{
					set_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y,COLOR_L2_GROUP_BG);
				}
			}
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			/* read */
			rd_color = read_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 );
			/* set */
			if( rd_color == COLOR_L2_GROUP_BG )
			{
				/* check */
				if( !flag_clear )
				{
					set_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_L2_GROUP_SEL);
				}
			}
			else if( rd_color == COLOR_L2_GROUP_SEL )
			{
				/* check */
				if( flag_clear )
				{
					set_l2_point_noload(pwidth, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_L2_GROUP_BG);
				}
			}
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			/* read */
			rd_color = read_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y);
			/* set */
			if( rd_color == COLOR_L2_GROUP_BG)
			{
				/* check */
				if( !flag_clear )
				{					
					set_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_L2_GROUP_SEL);
				}
			}
			else if( rd_color == COLOR_L2_GROUP_SEL)
			{
				/* check */
				if( flag_clear )
				{
					set_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_L2_GROUP_BG);
				}
			}
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			/* read */
			rd_color = read_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4);
			/* set */
			if( rd_color == COLOR_L2_GROUP_BG )
			{
				/* check */
				if( !flag_clear )
				{
					set_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_L2_GROUP_SEL);
				}
			}
			else if( rd_color == COLOR_L2_GROUP_SEL )
			{
				/* check */
				if( flag_clear )
				{
					set_l2_point_noload(pwidth, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_L2_GROUP_BG);
				}
			}
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		/* read */
		rd_color = read_l2_point_noload(pwidth,pos_x + 4 + i , pos_y);
		/* set */
		if( rd_color == COLOR_L2_GROUP_BG )
		{
			set_l2_point_noload(pwidth,pos_x + 4 + i , pos_y , COLOR_L2_GROUP_SEL);
		}
		else if( rd_color == COLOR_L2_GROUP_SEL )
		{
			set_l2_point_noload(pwidth,pos_x + 4 + i , pos_y , COLOR_L2_GROUP_BG);
		}
		/* read */
		rd_color = read_l2_point_noload(pwidth,pos_x + 4 + i , pos_y + pos_end_y - 1);
		/* set */
		if( rd_color == COLOR_L2_GROUP_BG)
		{
			set_l2_point_noload(pwidth,pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_L2_GROUP_SEL);
		}
		else if( rd_color == COLOR_L2_GROUP_SEL)
		{
			set_l2_point_noload(pwidth,pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_L2_GROUP_BG);
		}
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		/* read */
		rd_color = read_l2_point_noload(pwidth,pos_x , pos_y + 4 + i);
		/* set */
		if( rd_color == COLOR_L2_GROUP_BG)
		{
			set_l2_point_noload(pwidth,pos_x , pos_y + 4 + i , COLOR_L2_GROUP_SEL);
		}
		else if( rd_color == COLOR_L2_GROUP_SEL)
		{
			set_l2_point_noload(pwidth,pos_x , pos_y + 4 + i , COLOR_L2_GROUP_BG);
		}
		/* set */
		rd_color = read_l2_point_noload(pwidth,pos_x + pos_x_end - 1, pos_y + 4 + i );
		/* check */
		if( rd_color == COLOR_L2_GROUP_BG )
		{
			set_l2_point_noload(pwidth,pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_L2_GROUP_SEL);
		}
		else if( rd_color == COLOR_L2_GROUP_SEL )
		{
			set_l2_point_noload(pwidth,pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_L2_GROUP_BG);
		}
	}
	/* set */
	wid->msg.upd = ind_d;
	/* if */
	if( flag_clear == 0 )
	{
		return;//break
	}
	/* goto */
	goto RE_OSC_WIN;
}
/* silder mode */
void osc_slider_pos(struct widget * wid,unsigned short pos2)
{
	/* check */
	unsigned short pos = (unsigned short)((float)(wid->msg.x_size - 26 - 5 - 5) / 100.0f * (float)pos2 + 5);
	/* set */
	if( pos == wid->msg.upd )
	{
		return;
	}
	/* set pos */
	if( pos < 5 )
	{
		pos = 5;
	}
	else if( pos >= wid->msg.x_size - 26 - 5 )
	{
		pos = wid->msg.x_size - 26 - 5;
	}
	/* t */
	unsigned short pwidth = wid->msg.mystop;
	unsigned short pos_x = wid->msg.x;
	unsigned short pos_y = wid->msg.y;
	/* get y pos  */
	unsigned short y_pos = wid->msg.y - 9;
	/* if */
	if( wid->msg.upd != 0xff )
	{
		/* clear */
		osc_draw_l2_bmp(pwidth,pos_x + wid->msg.upd,y_pos,26,26,gImage_s,COLOR_L2_SLIDER_BG);
	}
	/* show new pos */
	osc_draw_l2_bmp(pwidth,pos_x + pos,y_pos,26,26,gImage_s,COLOR_L2_SLIDER);
	/* set */
	wid->msg.upd = pos;
		/* end */			
	/* set */
	for( int i = 0 ; i < wid->msg.x_size ; i ++ )
	{
		/* yoa */
		for( int j = 0 ; j < wid->msg.y_size ; j ++ )
		{
			/* read */
			unsigned char r_color = read_l2_point_noload(pwidth,pos_x + i,pos_y + j);
			/* check */
			if( i < pos )
			{
				/* check */
				if( r_color == COLOR_L2_SLIDER_BG || r_color == COLOR_L2_GROUP_BG )
				{
					set_l2_point_noload(pwidth,pos_x + i,pos_y + j,COLOR_L2_SLIDER_N);
				}
			}
			else
			{
				/* check */
				if( r_color == COLOR_L2_SLIDER_BG || r_color == COLOR_L2_SLIDER_N )
				{
					set_l2_point_noload(pwidth,pos_x + i,pos_y + j,COLOR_L2_GROUP_BG);
				}				
			}
		}
	}
}
/* change button color */
void osc_btn_l2_color(widget_def *wd,unsigned char mode)
{
	/* transfer to abs postion */
	unsigned short abpx = wd->msg.x;
	unsigned short abpy = wd->msg.y;
	/* pwitdh */
	unsigned short pwidth = wd->parent->msg.x_size;
	/* check */
	if( mode )
	{
		if( wd->msg.upd == COLOR_L2_BTN_FOCUS )
		{
			return;
		}
	}
	else
	{
		if( wd->msg.upd == COLOR_L2_GROUP_BG )
		{
			return;
		}		
	}
	/* change color */
	for( int i = abpx ; i < abpx + wd->msg.x_size ; i ++ )
	{
		/* next */
		for( int j = abpy ; j < abpy + wd->msg.y_size ; j ++ )
		{
			/* color */
			unsigned char tcoler = read_l2_point_noload(pwidth,i,j);
			/* tes */
			if( mode == 1 )
			{
				if( tcoler == COLOR_L2_GROUP_BG )
				{
					set_l2_point_noload(pwidth,i,j,COLOR_L2_BTN_FOCUS);
				}
				/* set backgroud color */
				wd->msg.upd = COLOR_L2_BTN_FOCUS;
				/*---------------------*/
		  }
			else
			{
				if( tcoler == COLOR_L2_BTN_FOCUS )
				{
					set_l2_point_noload(pwidth,i,j,COLOR_L2_GROUP_BG);
				}	
				/* set backgroud color */
				wd->msg.upd = COLOR_L2_GROUP_BG;
				/*---------------------*/				
			}
		}
	}
}
/* clear the data */
void osc_l2_win_clear_mem(unsigned char mod)
{
	if( mod == 0 )
	{
		/* hide */
		osc_dev_l2_disable();
	}
	/* hide */
	memset(&win_l2_main,0,sizeof(win_l2_main));
	memset(&group,0,sizeof(group));	
	/* clear */
	TOUCH_ID = 0xffff;
}
/* get touch id */
int osc_l2_window_sta(void)
{
	return win_l2_main.msg.mark_flag;
}
/* release touch */
void osc_l2_touch_release(void)
{
	/* check fine tune panel */
	if( win_fine_tune.msg.upd != 0 )
	{
		osc_fine_tune_touch_release();
		return;
	}
	/* clear */
	memset(touch_in_cnt,0,sizeof(touch_in_cnt));
	/* switch command*/
	if( TOUCH_ID != 0xffff )
	{		
		osc_l2_ui_event(TOUCH_ID);
	}
}
/* void osc event */
void osc_l2_ui_event(unsigned short ID)
{
	/* unsigned char */
	unsigned char common = 0;
	unsigned char idt;
	unsigned char param_id;
	/* case */
	switch(ID)
	{
		/* get command */
		case 21:
		case 22:
		case 23:
		case 24:
		case 29:
		case 30:
		case 41:
		case 42:
		case 43:
		case 61:
		case 62:
		case 63:
		case 81:
		case 82:
		case 83:			
			/* if */
      if( ID >= 21 && ID <= 30 )
			{
				idt = ID - 21 ;
				param_id = ID - 21 ;
				/* check */
				if( param_id == 8 )
				{
					param_id = 4;
				}
				else if( param_id == 9 )
				{
					param_id = 5;
				}
				else
				{
					/* nothing to do */
				}
			}
			else if( ID >= 41 && ID <= 43 )
			{
				idt = ID - 41 ;
				param_id = ID - 35 ;				
			}
			else if( ID >= 61 && ID <= 63 )
			{
				idt = ID - 61 ;
				param_id = ID - 52 ;					
			}
			else if( ID >= 81 && ID <= 83 )
			{
				idt = ID - 81 ;
				param_id = ID - 68 ;					
			}			
			else
			{
				/* nothing to do */
			}
			/* set data */
		  common = group[idt].msg.upd;
		  /* set next */
		  common ++;
		  /* check */
		  if( common >= group[idt].msg.mx )
			{
				common = 0;
			}
			/* set pow */
			osc_param_sys_set(&runmsg->sys_menu_set,common,param_id);
			/* set */
			osc_select(&group[idt],common);
		  /* do commander */
			osc_l2_touch_event(&group[idt],ID,common);
			/* set */
			break;	
      /* ch2 */			
		case 26:
			/* release the key and */
			osc_btn_l2_color(&group[ID-21],0);	
			/* get data */
			osc_msg_box(2);		
			break;
		case 27:
			/* release the key and */
			osc_btn_l2_color(&group[ID-21],0);
			/* get data */
			osc_msg_box(1);		
			break;
		case 28:
			/* release the key and */
			osc_btn_l2_color(&group[ID-21],0);			
			/* get data */
			osc_msg_box(0);
			/* end */
			break;	
		case 25:
			/* slider */
		  /* transfer */
//		  touch_slider_px = ( touch_slider_px - group[4].msg.x - group[4].parent->msg.abx ) * 100 / group[4].msg.x_size;
//			/* show */
//			osc_slider_pos(&group[4],touch_slider_px);
//			/* px */
//			runmsg->back_light_per = touch_slider_px;
		  /* endif */
			break;
		case 0xfffe:
			/* clear */
			osc_l2_win_clear_mem(0);
		  /* endif */
			break;
		default :
			break;
	}
	/* clear */
	TOUCH_ID = 0xffff;
}
/* data */
extern unsigned char trt;
/* exe */
void osc_l2_touch_event(widget_def * wid , unsigned char ID , unsigned char dat)
{
	/* chekc */
	const osc_vol_scale_def * fes;
	/* set trig */
	switch(ID)
	{
		/* get command */
		/* change language */
		case 22:
			/* check */
		  if( osc_language_get() < 2)
			{
				osc_language_update_measure();
				osc_side_btn_update_languase();
				osc_run_stop(osc_run_mode());
			}
			break;
		case 23:
			/* AI mode */
			if( dat == 0 )
			{
				osc_show_format(1,9);
			}
			else
			{
				osc_show_format(1,0xff);
			}		  
			break;
		case 24:
			/* XY or YT */
			if( dat == 0 )
			{
				osc_show_format(0,3);
			}
			else
			{
				osc_show_format(0,2);
			}
			/* clear all lines */
			osc_clear_all_lines();
			/* set flag */
			osc_fm = dat;
			/* break */
			break;
		case 83:
			/* clear all lines */
			osc_clear_all_lines();
			/* set trig source */
			osc_ui_set_trig_type(dat);
			/* cet */
			trt = dat;
		  /*-----------------*/
		break;
		case 81:
			/* set trig source */
			osc_ui_set_trig_source(dat);
			/* show trig lines */
		  if( dat == 0 )
			{
		  	osc_ui_trig_arrow_show(0,1);
				osc_ui_trig_arrow_show(1,0);
				osc_trig_scale_thread(0);
			}
			else
			{
		  	osc_ui_trig_arrow_show(0,0);
				osc_ui_trig_arrow_show(1,1);			
				osc_trig_scale_thread(1);				
			}		
		break;		
		case 82:
			/* set trig source */
			osc_ui_set_trig_edge(dat);
			/* set EXIT */
		  osc_exit_edge_reg(dat);
			/*-----------------*/
		break;		
		case 41:
			/* ch1 close */
			if( dat )
			{
				osc_clear_all_lines();
			}
			break;
		/* ch1 */
		case 42:
			/* set ch1 X10 X1 */
			osc_ui_set_x10_x1(0,dat);
			/* get data */
			fes = osc_get_vol_scale(0);
		  /* show oc*/
			if( dat == 0 )
			{
				/* set */
				osc_ui_vol_scale(0,fes->strX10);			 
			}
			else
			{
				/* set */
				osc_ui_vol_scale(0,fes->str);			
			}
			/* check */
			if( !osc_ctrl_menu_sta() )
			{
				osc_ui_vol_scale(2,0);
			}
		  /* --- */
			break;
		case 43:
			/* set dcac */
			osc_coupling_setting(0,dat);
			osc_ui_set_dcac(0,dat);
			break;
		case 61:
			/* ch1 close */
			if( dat )
			{
				osc_clear_all_lines();
			}
			break;
		/* ch2 */
		case 62:
			/* set ch1 X10 X1 */
			osc_ui_set_x10_x1(1,dat);
			/* get data */
			fes = osc_get_vol_scale(1);
		  /* show oc*/
			if( dat == 0 )
			{
				/* set */
				osc_ui_vol_scale(1,fes->strX10);			 
			}
			else
			{
				/* set */
				osc_ui_vol_scale(1,fes->str);			
			}
			/* check */
			if( !osc_ctrl_menu_sta() )
			{
				osc_ui_vol_scale(3,0);
			}			
		break;		
		case 63:
			/* set dcac */
			osc_coupling_setting(1,dat);
			osc_ui_set_dcac(1,dat);
			break;
		case 21:
			/* persist changed - clear waveform area */
			osc_clear_all_lines();
			break;
		default:
			break;
	}
}
/* get wid */
void osc_l2_touch(unsigned short px,unsigned short py)
{
	/* check fine tune panel first */
	if( win_fine_tune.msg.upd != 0 )
	{
		osc_fine_tune_touch(px, py);
		return;
	}
	/* check id */
	if( TOUCH_ID != 0xffff )
	{
		/* if slider */
		if( TOUCH_ID == 25 )
		{
		  /* transfer */
			unsigned short abx_x = group[4].msg.x + group[4].parent->msg.abx;
			/* check */
			if( px >= group[4].msg.x + group[4].parent->msg.abx )
			{
				touch_slider_px = ( px - abx_x ) * 100 / group[4].msg.x_size;
				/* check */
				if( touch_slider_px > 100 )
				{
				 touch_slider_px = 100;
				}
			}
			else
			{
				touch_slider_px = 0;
			}
			/* show */
			osc_slider_pos(&group[4],touch_slider_px);
			/* px */
			runmsg->back_light_per = touch_slider_px;	
			/* set backlight */
			user_back_light(100 + touch_slider_px * 9 - 1 );
			/*---------------*/
		}
		/* return */
		return;
	}
	/* --------- */
	unsigned short abx = win_l2_main.msg.x + win_l2_main.msg.abx;
	unsigned short aby = win_l2_main.msg.y + win_l2_main.msg.aby;
	/* size */
	unsigned short abx_size = abx + win_l2_main.msg.x_size;
	unsigned short aby_size = aby + win_l2_main.msg.y_size;
	/* check area */
	if( px < abx || px > abx_size || 
			py < aby || py > aby_size )
	{
		touch_in_cnt[0] ++;
		/* disable */
		if( touch_in_cnt[0] >= 5 )
		{
			TOUCH_ID = 0xfffe;
		}
		/* return */
		return;
	}
	/* in */
	touch_in_cnt[0] = 0;
	/* from zero to end creating the pic */
	window_def * base = &win_l2_main ;	
	/* index id */
	int i = 1 ;
	/* check other */
	for( widget_def * wbase = base->wchild ; wbase != 0 ; wbase = wbase->peer_linker , i ++)
	{	
		/* transfer to abs pos */
		abx = wbase->msg.x + win_l2_main.msg.abx;
		aby = wbase->msg.y + win_l2_main.msg.aby;
		/* size */
		abx_size = abx + wbase->msg.x_size;
		aby_size = aby + wbase->msg.y_size;
		/* check slider */
		if( wbase->msg.my != 2 )
		{
			/* check */
			if( px >= abx && px <= abx_size &&
					py >= aby && py <= aby_size )
			{
				touch_in_cnt[i] ++;
			}
		}
		else
		{
			/* check */
			if( px >= abx && px <= abx_size &&
					py >= (aby - 9) && py <= (aby_size + 18) )
			{
				touch_in_cnt[i] ++;
			}			
		}
	}	
	/* check */
	for( int t = 1 ; t < i ; t ++)
	{
		/* check */
		if( touch_in_cnt[t] >= 5 )
		{
			/* get ID */
			TOUCH_ID = win_l2_main.msg.mark_flag * 20 + t;
			/* check id for btn */
			if( TOUCH_ID >= 26 && TOUCH_ID <= 28 )
			{
				/* focus key */
				osc_btn_l2_color(&group[TOUCH_ID - 21],1);
				/* tine */
			}
			else if( TOUCH_ID == 25 )
			{
				/* slider */
				touch_slider_px = px;
			}
			else if( TOUCH_ID >= 121 )
			{
				/* focus key */
				if( group[TOUCH_ID - 121].msg.upd == COLOR_L2_GROUP_BG || group[TOUCH_ID - 121].msg.upd == 0xff )
				{
					/* check num */
					unsigned char sum = 0;
					/* check */
					for( int i = 0 ; i < 10 ; i ++ )
					{
						/* check ch1 */
						if( runmsg->measure_item[0] & ( 1 << i ) )
						{
							sum ++;
						}
						/* check ch2 */
						if( runmsg->measure_item[1] & ( 1 << i ) )
						{
							sum ++;
						}						
					}
					/*-----------*/
					if( sum < 10 )
					{
						osc_btn_l2_color(&group[TOUCH_ID - 121],1);			
						//osc_draw_right_icon(&group[TOUCH_ID - 121]);
						/* check */
						unsigned short t_sho = 0;
						/* get */
						if( TOUCH_ID - 121 < 10 )
						{
							/* ch1 */
							runmsg->measure_item[0] |= 1 << ( TOUCH_ID - 121);
							/* get pos */
							t_sho = osc_ui_measure_item(0,TOUCH_ID - 121);
							/* get pos id */
							group[TOUCH_ID - 121].msg.wflags = t_sho;							
						}
						else if( TOUCH_ID - 121 < 20 )
						{
							/* ch1 */
							runmsg->measure_item[1] |= 1 << ( TOUCH_ID - 131);
							/* get pos */
							t_sho = osc_ui_measure_item(1,TOUCH_ID - 131);					
							/* get pos id */
							group[TOUCH_ID - 121].msg.wflags = t_sho;							
						}
						else
						{
							
						}
					}
					/* set */
					if( TOUCH_ID - 121 >= 20 )
					{
						/* tips */
						unsigned short id_t = TOUCH_ID - 121;
						/* set flag */
						osc_btn_l2_color(&group[TOUCH_ID - 121],1);
						/* set */
						if( id_t >= 20 && id_t <= 22 )
				{
					unsigned char ind = id_t - 20 + 16;
					osc_param_sys_set(&runmsg->sys_menu_set,1,ind);
					if( id_t == 20 )
					{
						osc_cursor_init_default();
						osc_show_format(1, 10);
						unsigned short slot = osc_ui_measure_item(0, 10);
						group[TOUCH_ID - 121].msg.wflags = slot;
					}
					else if( id_t == 21 )
					{
						osc_cursor_init_default();
						osc_show_format(1, 11);
						unsigned short slot = osc_ui_measure_item(0, 11);
						group[TOUCH_ID - 121].msg.wflags = slot;
					}
					else if( id_t == 22 )
					{
						osc_show_format(1, 12);
						fine_tune_trigger_btn = (TOUCH_ID - 121);
						osc_create_fine_tune_menu();
					}
				}
						else if( id_t >= 23 && id_t <= 26 )
						{
							/* check enable */
							if( osc_param_sys_get(runmsg->sys_menu_set,19) == 0 )
							{
								/* set save param */
								osc_param_sys_set(&runmsg->sys_menu_set,1,19);//set enable 													
							}
              else
							{
								/* set item */
								unsigned char irt = osc_param_sys_get(runmsg->sys_menu_set,22);
								/*----- reset -----*/							
								osc_btn_l2_color(&group[irt + 23],0);
							}
							/* set */
							osc_param_sys_set(&runmsg->sys_menu_set,id_t - 23,22);						
							/* SET AVTIVC */
							osc_show_format(2,id_t - 23 + 5);
							// DO SOMETHING 
              /* ENABLE */							
						}
						else if( id_t >= 27 && id_t <= 28 )
						{
							/* FFT */
							unsigned char fft_index = osc_param_sys_get(runmsg->sys_menu_set,23);
							/* check */
							if( fft_index != 0 )
							{
								/* clear */
								osc_btn_l2_color(&group[fft_index + 27 - 1],0);
							}
							/* set */
							osc_param_sys_set(&runmsg->sys_menu_set,id_t - 27 + 1,23);//set enable
							/* set soutch */
							osc_show_format(3,id_t - 27);
							/* show dst */
							/*------enable------*/
						}
					}
					else
					{
						/* error */
					}
				}					
				else
				{
					/* unsigned short tdd */
					unsigned short tdd;
					/* CLOER */
					osc_btn_l2_color(&group[TOUCH_ID - 121],0);
					//osc_draw_right_icon(&group[TOUCH_ID - 121]);
					/* check */
					if( TOUCH_ID - 121 < 10 )
					{
						/* ch1 */
						runmsg->measure_item[0] &=~ (1 << ( TOUCH_ID - 121));
						/* get pos id */
						tdd = group[TOUCH_ID - 121].msg.wflags;
						/* release */
						osc_ui_release_area(tdd);							
					}
					else if( TOUCH_ID - 121 < 20 )
					{
						/* ch1 */
						runmsg->measure_item[1] &=~ (1 << ( TOUCH_ID - 131));				
						/* get pos id */
						tdd = group[TOUCH_ID - 121].msg.wflags;
						/* release */
						osc_ui_release_area(tdd);							
					}
					else if( TOUCH_ID - 121 >= 20 && TOUCH_ID - 121 < 23 )		
			{
				unsigned short id_t = TOUCH_ID - 121 - 20 + 16;				
				osc_param_sys_set(&runmsg->sys_menu_set,0,id_t);
				osc_show_format(1, 0xff);
				if( id_t == 18 )
				{
					osc_fine_tune_hide();
				}
				unsigned short tdd = group[TOUCH_ID - 121].msg.wflags;
				if( tdd < 10 ) { osc_ui_release_area(tdd); }
				if( id_t == 16 )
				{
					osc_clear_cursor_line_h(runmsg->amp_cursor_a);
					osc_clear_cursor_line_h(runmsg->amp_cursor_b);
				}
				else if( id_t == 17 )
				{
					osc_clear_cursor_line_v(runmsg->time_cursor_a);
					osc_clear_cursor_line_v(runmsg->time_cursor_b);
				}
			}						
					else if( TOUCH_ID - 121 >= 23 && TOUCH_ID - 121 <= 26 )	
					{
						/* ERROR */
						osc_param_sys_set(&runmsg->sys_menu_set,0,19);//set disable 
						/* disable the math */
						osc_show_format(2,0xff);
					}		
          else if( TOUCH_ID - 121 >= 27 && TOUCH_ID - 121 <= 28 )	
					{
						osc_param_sys_set(&runmsg->sys_menu_set,0,23);//set disable
						/* check */
						osc_show_format(3,0xff);
						/* clear screen */
						osc_clear_all_lines();
						/* end */
					}						
					else
					{
						/* ERROR */						
					}
				}
			}
			else
			{
				
			}
			/* break */
			break;
		}
	}
	/* show */
}
/* fine tune touch event */
void osc_fine_tune_touch_event(unsigned short ID)
{
	osc_run_msg_def * runmsg = get_run_msg();
	signed short offset;
	signed short trig_val;
	unsigned char chn;
	switch(ID)
	{
		case 0: /* CH1 up */
			chn = 0;
			offset = runmsg->vol_offset_scale[chn];
			offset -= 2;
			if( offset < 0 ) offset = 0;
			runmsg->vol_offset_scale[chn] = offset;
			osc_offset_scale_thread(chn);
			break;
		case 1: /* CH1 down */
			chn = 0;
			offset = runmsg->vol_offset_scale[chn];
			offset += 2;
			if( offset > 399 ) offset = 399;
			runmsg->vol_offset_scale[chn] = offset;
			osc_offset_scale_thread(chn);
			break;
		case 2: /* CH2 up */
			chn = 1;
			offset = runmsg->vol_offset_scale[chn];
			offset -= 2;
			if( offset < 0 ) offset = 0;
			runmsg->vol_offset_scale[chn] = offset;
			osc_offset_scale_thread(chn);
			break;
		case 3: /* CH2 down */
			chn = 1;
			offset = runmsg->vol_offset_scale[chn];
			offset += 2;
			if( offset > 399 ) offset = 399;
			runmsg->vol_offset_scale[chn] = offset;
			osc_offset_scale_thread(chn);
			break;
		case 4: /* TRIG up */
			chn = osc_trig_source();
			trig_val = runmsg->trig_vol_level_ch[chn];
			trig_val -= 2;
			if( trig_val < 1 ) trig_val = 1;
			runmsg->trig_vol_level_ch[chn] = trig_val;
			osc_trig_scale_thread(chn);
			break;
		case 5: /* TRIG down */
			chn = osc_trig_source();
			trig_val = runmsg->trig_vol_level_ch[chn];
			trig_val += 2;
			if( trig_val > 399 ) trig_val = 399;
			runmsg->trig_vol_level_ch[chn] = trig_val;
			osc_trig_scale_thread(chn);
			break;
		case 7: /* X close */
			osc_param_sys_set(&runmsg->sys_menu_set, 0, 18);
			osc_show_format(1, 0xff);
			osc_fine_tune_hide();
			break;
		default:
			break;
	}
}
/* fine tune panel touch */
void osc_fine_tune_touch(unsigned short px, unsigned short py)
{
	if( FINE_TUNE_TOUCH_ID != 0xffff )
	{
		return;
	}
	unsigned short abx = win_fine_tune.msg.x + win_fine_tune.msg.abx;
	unsigned short aby = win_fine_tune.msg.y + win_fine_tune.msg.aby;
	unsigned short abx_size = abx + win_fine_tune.msg.x_size;
	unsigned short aby_size = aby + win_fine_tune.msg.y_size;
	if( px < abx || px > abx_size || py < aby || py > aby_size )
	{
		fine_tune_touch_cnt[0] ++;
		if( fine_tune_touch_cnt[0] >= 5 )
		{
			FINE_TUNE_TOUCH_ID = 0xfffe;
		}
		return;
	}
	fine_tune_touch_cnt[0] = 0;
	for( int i = 0 ; i < 8 ; i ++ )
	{
		unsigned short bx = fine_tune_btn[i].msg.x + win_fine_tune.msg.abx;
		unsigned short by = fine_tune_btn[i].msg.y + win_fine_tune.msg.aby;
		unsigned short bx_size = bx + fine_tune_btn[i].msg.x_size;
		unsigned short by_size = by + fine_tune_btn[i].msg.y_size;
		if( px >= bx && px <= bx_size && py >= by && py <= by_size )
		{
			fine_tune_touch_cnt[i+1] ++;
			if( fine_tune_touch_cnt[i+1] >= 3 )
			{
				osc_btn_l2_color(&fine_tune_btn[i], 1);
				FINE_TUNE_TOUCH_ID = i;
			}
		}
		else
		{
			fine_tune_touch_cnt[i+1] = 0;
		}
	}
}
/* fine tune touch release */
void osc_fine_tune_touch_release(void)
{
	memset(fine_tune_touch_cnt, 0, sizeof(fine_tune_touch_cnt));
	if( FINE_TUNE_TOUCH_ID != 0xffff && FINE_TUNE_TOUCH_ID != 0xfffe )
	{
		osc_btn_l2_color(&fine_tune_btn[FINE_TUNE_TOUCH_ID], 0); // 释放按钮颜色
		osc_fine_tune_touch_event(FINE_TUNE_TOUCH_ID);
	}
	FINE_TUNE_TOUCH_ID = 0xffff;
}
/* fine tune panel status */
int osc_fine_tune_sta(void)
{
	return ( win_fine_tune.msg.upd == 0 ) ? 0 : 1;
}
/* hide fine tune panel */
void osc_fine_tune_hide(void)
{
	if( fine_tune_trigger_btn != 0xffff )
	{
		osc_btn_l2_color(&group[fine_tune_trigger_btn], 0);	// 释放按钮颜色
		fine_tune_trigger_btn = 0xffff;
	}
	memset(&win_fine_tune, 0, sizeof(win_fine_tune));
	memset(fine_tune_btn, 0, sizeof(fine_tune_btn));
	osc_dev_l2_disable();
}
/* create fine tune panel */
void osc_create_fine_tune_menu(void)
{
	unsigned int resize = osc_dev_resize();
	unsigned short x_size = 200;
	unsigned short y_size = 140;
	unsigned short posx = ( dev->display_dev->pwidth - x_size ) / 2;
	unsigned short posy = ( dev->display_dev->pheight - y_size ) / 2;
	if( x_size * y_size > resize ) return;
	memset(&win_fine_tune, 0, sizeof(win_fine_tune));
	memset(fine_tune_btn, 0, sizeof(fine_tune_btn));
	osc_l2_win_clear_mem(1);
	osc_dev_l2_enable(posx, posy, x_size, y_size, 255);
	memset(dev->gram_l2, COLOR_L2_GRAD, x_size * y_size);
	win_fine_tune.msg.x = 0;
	win_fine_tune.msg.y = 0;
	win_fine_tune.msg.x_size = x_size;
	win_fine_tune.msg.y_size = y_size;
	win_fine_tune.msg.upd = 1;
	win_fine_tune.msg.mark_flag = 0;
	win_fine_tune.msg.abx = posx;
	win_fine_tune.msg.aby = posy;
	unsigned short btn_w = 48;
	unsigned short btn_h = 30;
	unsigned short gap = 6;
	unsigned short start_x = (x_size - (btn_w * 2 + gap)) / 2;
	unsigned short start_y = 30;
	for( int i = 0 ; i < 6 ; i ++ )
	{
		unsigned short col = i % 2;
		unsigned short row = i / 2;
		fine_tune_btn[i].msg.x = start_x + col * (btn_w + gap);
		fine_tune_btn[i].msg.y = start_y + row * (btn_h + gap);
		fine_tune_btn[i].msg.x_size = btn_w;
		fine_tune_btn[i].msg.y_size = btn_h;
		fine_tune_btn[i].msg.upd = 0xff;
		fine_tune_btn[i].msg.abx = posx + fine_tune_btn[i].msg.x;
		fine_tune_btn[i].msg.aby = posy + fine_tune_btn[i].msg.y;
		fine_tune_btn[i].parent = &win_fine_tune;
		fine_tune_btn[i].msg.wflags = i;
		osc_gui_fine_tune_btn(&fine_tune_btn[i]);
	}
	
	fine_tune_btn[7].msg.x = x_size - 24;
	fine_tune_btn[7].msg.y = 2;
	fine_tune_btn[7].msg.x_size = 20;
	fine_tune_btn[7].msg.y_size = 20;
	fine_tune_btn[7].msg.upd = 0xff;
	fine_tune_btn[7].msg.abx = posx + fine_tune_btn[7].msg.x;
	fine_tune_btn[7].msg.aby = posy + fine_tune_btn[7].msg.y;
	fine_tune_btn[7].parent = &win_fine_tune;
	fine_tune_btn[7].msg.wflags = 7;
	osc_gui_fine_tune_btn(&fine_tune_btn[7]);
	osc_gui_gb2312_string_l2(x_size, "Precise Adjust", 5, 5, 0);           // 绘制细调按钮
}
/* draw fine tune button */
void osc_gui_fine_tune_btn(widget_def * wid)
{
	unsigned short pw = wid->parent->msg.x_size;
	unsigned short px = wid->msg.x;
	unsigned short py = wid->msg.y;
	unsigned short xs = wid->msg.x_size;
	unsigned short ys = wid->msg.y_size;
	unsigned char id = wid->msg.wflags;
	for( int i = 0 ; i < ys ; i ++ )
	{
		for( int j = 0 ; j < xs ; j ++ )
		{
			if( i == 0 || i == ys - 1 || j == 0 || j == xs - 1 )
			{
				set_l2_point_noload(pw, px + j, py + i, COLOR_L2_BUTTON_BG);
			}
			else
			{
				set_l2_point_noload(pw, px + j, py + i, COLOR_L2_GROUP_BG);
			}
		}
	}
	if( id == 0 ) osc_gui_gb2312_string_l2(pw, "CH1+", px + 8, py + 7, 0);
	else if( id == 1 ) osc_gui_gb2312_string_l2(pw, "CH1-", px + 8, py + 7, 0);
	else if( id == 2 ) osc_gui_gb2312_string_l2(pw, "CH2+", px + 8, py + 7, 0);
	else if( id == 3 ) osc_gui_gb2312_string_l2(pw, "CH2-", px + 8, py + 7, 0);
	else if( id == 4 ) osc_gui_gb2312_string_l2(pw, "TRI+", px + 8, py + 7, 0);
	else if( id == 5 ) osc_gui_gb2312_string_l2(pw, "TRI-", px + 8, py + 7, 0);
	else if( id == 7 ) osc_gui_gb2312_string_l2(pw, "X", px + 6, py + 2, 0);
	/*
	{
		for( int k = 0 ; k < xs - 4 ; k ++ )
		{
			set_l2_point_noload(pw, px + 3 + k, py + 3 + k, 0);
			set_l2_point_noload(pw, px + 3 + k, py + ys - 4 - k, 0);
		}
	}
	*/
}

/* get restord data */
unsigned char osc_param_sys_get(unsigned int src , unsigned char index)
{
	/* data */
	unsigned char re_date = 0;
	/* check */
	switch(index)
	{
		case 0:
			/* return */
		  re_date = src & 0x7;
			break;
		case 1:
			/* return */
		  re_date = ( src & 0x18 ) >> 3;			
			break;
		case 2:
			/* return */
			re_date = ( src & 0x20 ) ? 1 : 0;		
			/* break */		
			break;
		case 3:
			/* return */
			re_date = ( src & 0x40 ) ? 1 : 0;		
			/* break */		
			break;		
		case 4:
			/* return */
			re_date = ( src & 0x80 ) ? 1 : 0;		
			/* break */		
			break;			
		case 5:
			/* return */
			re_date = ( src & 0x100 ) ? 1 : 0;		
			/* break */		
			break;	
		case 6:
			/* return */
			re_date = ( src & 0x200 ) ? 1 : 0;		
			/* break */		
			break;		
		case 7:
			/* return */
			re_date = ( src & 0x400 ) ? 1 : 0;		
			/* break */		
			break;
		case 8:
			/* return */
			re_date = ( src & 0x800 ) ? 1 : 0;		
			/* break */		
			break;		
		case 9:
			/* return */
			re_date = ( src & 0x1000 ) ? 1 : 0;		
			/* break */		
			break;
		case 10:
			/* return */
			re_date = ( src & 0x2000 ) ? 1 : 0;		
			/* break */		
			break;
		case 11:
			/* return */
			re_date = ( src & 0x4000 ) ? 1 : 0;		
			/* break */		
			break;
		case 12:
			/* return */
			re_date = ( src & 0x8000 ) ? 1 : 0;		
			/* break */		
			break;	
		case 13:
			/* return */
			re_date = ( src & ( 0x3 << 16) ) >> 16;
			/* break */		
			break;		
		case 14:
			/* return */
			re_date = ( src & ( 0x3 << 18) ) >> 18;
			/* break */		
			break;	
		case 15:
			/* return */
			re_date = ( src & ( 0x3 << 20) ) >> 20;
			/* break */		
			break;		
		case 16://--------------------------===========================================================
			/* return */
			re_date = ( src & ( 0x1 << 22 ) ) ? 1 : 0;
			/* break */		
			break;	
		case 17:
			/* return */
			re_date = ( src & ( 0x1 << 23)) ? 1 : 0;
			/* break */		
			break;	
		case 18:
			/* return */
			re_date = ( src & ( 0x1 << 24) ) ? 1 : 0;
			/* break */		
			break;	
		case 19:
			/* return */
			re_date = ( src & ( 0x1 << 25) ) ? 1 : 0;
			/* break */		
			break;	
		case 20:
			/* return */
			re_date = ( src & ( 0x1 << 26) ) ? 1 : 0;
			/* break */		
			break;	
		case 21:
			/* return */
			re_date = ( src & ( 0x1 << 27) ) ? 1 : 0;
			/* break */		
			break;	
		case 22: // math
			/* return */
			re_date = ( src & ( 0x3 << 28) ) >> 28;
			/* break */		
			break;
		case 23: // fft
			/* return */
			re_date = ( src & ( (unsigned int)0x3 << 30) ) >> 30;
			/* break */		
			break;		
		default:
			break;
	}
	/* return */
	return re_date;
}
/* get restord data */
void osc_param_sys_set(unsigned int * src , unsigned char b_data,unsigned char index)
{
	/* td */
	unsigned int tmdd = b_data;
	/* check */
	switch(index)
	{
		case 0:
			/* return */
			*src &=~ 0x7 ;
			/* set */
			*src |= ( b_data & 0x7 );
			break;
		case 1:
			/* return */
		  *src &=~ 0x18 ;
			/* set */
			*src |= (( b_data & 0x3 ) << 3);		
			/* break */
			break;
		case 2:
			/* return */
			if( b_data )
			{
				*src |= 0x20;
			}
			else
			{
				*src &=~ 0x20;
			}
			/* break */		
			break;
		case 3:
			/* return */
			if( b_data )
			{
				*src |= 0x40;
			}
			else
			{
				*src &=~ 0x40;
			}
			/* break */	
			break;
		case 4:
			/* return */
			if( b_data )
			{
				*src |= 0x80;
			}
			else
			{
				*src &=~ 0x80;
			}
			/* break */		
			break;
		case 5:
			/* return */
			if( b_data )
			{
				*src |= 0x100;
			}
			else
			{
				*src &=~ 0x100;
			}
			/* break */
			break;
		case 6:
			/* return */
			if( b_data )
			{
				*src |= 0x200;
			}
			else
			{
				*src &=~ 0x200;
			}
			/* break */
			break;
		case 7:
			/* return */
			if( b_data )
			{
				*src |= 0x400;
			}
			else
			{
				*src &=~ 0x400;
			}
			/* break */
			break;
		case 8:
			/* return */
			if( b_data )
			{
				*src |= 0x800;
			}
			else
			{
				*src &=~ 0x800;
			}
			/* break */
			break;			
		case 9:
			/* return */
			if( b_data )
			{
				*src |= 0x1000;
			}
			else
			{
				*src &=~ 0x1000;
			}
			/* break */
			break;	
		case 10:
			/* return */
			if( b_data )
			{
				*src |= 0x2000;
			}
			else
			{
				*src &=~ 0x2000;
			}
			/* break */
			break;	
		case 11:
			/* return */
			if( b_data )
			{
				*src |= 0x4000;
			}
			else
			{
				*src &=~ 0x4000;
			}
			/* break */
			break;	
		case 12:
			/* return */
			if( b_data )
			{
				*src |= 0x8000;
			}
			else
			{
				*src &=~ 0x8000;
			}
			/* break */
			break;	
		case 13:
			/* return */
			*src &=~ ( 0x3 << 16 );
			/* set */
		  *src |= ( tmdd & 0x03 ) << 16;
			/* break */
			break;	
		case 14:
			/* return */
			*src &=~ ( 0x3 << 18 );
			/* set */
		  *src |= ( tmdd & 0x03 ) << 18;
			/* break */
			break;
		case 15:
			/* return */
			*src &=~ ( 0x3 << 20 );
			/* set */
		  *src |= ( tmdd & 0x03 ) << 20;
			/* break */
			break;	
		case 16://---------------------------------------================================================55555555555555555555555
			/* return */
			*src &=~ ( 0x1 << 22 );
			/* set */
		  *src |= ( tmdd & 0x01 ) << 22;
			/* break */
			break;	
		case 17:
			/* return */
			*src &=~ ( 0x1 << 23 );
			/* set */
		  *src |= ( tmdd & 0x01 ) << 23;
			/* break */
			break;	
		case 18:
			/* return */
			*src &=~ ( 0x1 << 24 );
			/* set */
		  *src |= ( tmdd & 0x01 ) << 24;
			/* break */
			break;	
		case 19:
			/* return */
			*src &=~ ( 0x1 << 25 );
			/* set */
		  *src |= ( tmdd & 0x01 ) << 25;
			/* break */
			break;	
		case 20:
			/* return */
			*src &=~ ( 0x1 << 26 );
			/* set */
		  *src |= ( tmdd & 0x01 ) << 26;
			/* break */
			break;	
		case 21:
			/* return */
			*src &=~ ( 0x1 << 27 );
			/* set */
		  *src |= ( tmdd & 0x01 ) << 27;
			/* break */
			break;	
		case 22:
			/* return */
			*src &=~ ( 0x3 << 28 );
			/* set */
		  *src |= ( tmdd & 0x03 ) << 28;
			/* break */
			break;
		case 23:
			/* return */
			*src &=~ ( (unsigned int)0x3 << 30 );
			/* set */
		  *src |= ( tmdd & 0x03 ) << 30;
			/* break */
			break;			
		default:
			break;
	}
	/* return */
}
/* reflush ind */
void osc_measure_math_reflush(void)
{
	/* get pos id */
	unsigned char pos_id;
	/* get */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* check ch1 */
		if( runmsg->measure_item[0] & ( 1 << i ) )
		{
			osc_btn_l2_color(&group[i],1);			
//			osc_draw_right_icon(&group[i]);	
			/* get pos id */
			pos_id = osc_search_pos_is(0,i);
			/* set */
			group[i].msg.wflags = pos_id;
		}
		/* check ch2 */
		if( runmsg->measure_item[1] & ( 1 << i ) )
		{
			osc_btn_l2_color(&group[i + 10],1);			
//			osc_draw_right_icon(&group[i + 10]);			
			/* get pos id */
			pos_id = osc_search_pos_is(1,i);
			/* set */
			group[i+10].msg.wflags = pos_id;			
		}
	}
	/* get */
	if( osc_param_sys_get(runmsg->sys_menu_set,16) )
	{
		osc_btn_l2_color(&group[20],1);
		osc_show_format(1, 10);
		pos_id = osc_search_pos_is(0, 10);
		group[20].msg.wflags = pos_id;
	}
	/* get */
	if( osc_param_sys_get(runmsg->sys_menu_set,17) )
	{
		osc_btn_l2_color(&group[21],1);
		osc_show_format(1, 11);
		pos_id = osc_search_pos_is(0, 11);
		group[21].msg.wflags = pos_id;
	}
	/* get */
	if( osc_param_sys_get(runmsg->sys_menu_set,18) )
	{
		osc_btn_l2_color(&group[22],1);
		osc_show_format(1, 12);
		pos_id = osc_search_pos_is(0, 12);
		group[22].msg.wflags = pos_id;
	}
	/* math */
	if( osc_param_sys_get(runmsg->sys_menu_set,19) == 1 )
	{
		unsigned char ind = osc_param_sys_get(runmsg->sys_menu_set,22) & 0x3;
		/* set */
		osc_btn_l2_color(&group[23 + ind],1);
	}
	/* get */
	unsigned char fft_index = osc_param_sys_get(runmsg->sys_menu_set,23) & 0x3;
	/* check */
	if( fft_index != 0 )
	{
		/* set */
		osc_btn_l2_color(&group[27 + fft_index - 1],1);		
	}
}
/* get dcac */
void osc_refluse_dcac_chn(void)
{
	/* get */
	runmsg = get_run_msg();	
	/* td */
	unsigned char gett;
	/* gset */
	gett = osc_param_sys_get(runmsg->sys_menu_set,8);
	/* update */
	osc_coupling_setting(0,gett);
	/* set dcac */
	osc_ui_set_dcac(0,gett);
	/*--------*/
	gett = osc_param_sys_get(runmsg->sys_menu_set,11);
	/* update */
	osc_coupling_setting(1,gett);
	/* set dcac */
	osc_ui_set_dcac(1,gett);		
	/*--------*/		
}
/* void osc draw rightd */
void osc_draw_right_icon(widget_def * btn_wd )
{
	/* create the base voltage icon */
	const unsigned short right_icon_table[12] = {0x0000,0x0001,0x0003,0x0007,
		0x400E,0xE01C,0x7038,0x3870,0x1CE0,0x0FC0,0x0780,0x0300};
	/* check pox */
	unsigned short start_px = btn_wd->msg.x + 2;
	unsigned short start_py = btn_wd->msg.y + 2;
	/* check */
	unsigned int color;
	/* check */
	if( btn_wd->msg.upd == COLOR_L2_BTN_FOCUS )
	{
		color = COLOR_L2_RIGHT_ICON;
	}
	else
	{
		color = COLOR_L2_GROUP_BG;
	}
	/* table */
	for( int i = 0 ; i < 12 ; i ++ )
	{
		/* lines */
		for( int j = 0 ; j < 16 ; j ++ )
		{
			/* if */
			if( ( right_icon_table[i] << j ) & 0x8000 )
			{
				set_l2_point_noload(btn_wd->parent->msg.x_size,start_px + j , start_py + i , color);
			}
		}
	}
}
/* get osc trig type */
unsigned char osc_trig_type(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,15);
}
/* get osc trig source */
unsigned char osc_trig_source(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,13);
}
/* get rising */
unsigned char osc_trig_edge(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,14);
}
/* set che default trig edge */
void osc_default_edge_ui(void)
{
	/* chec */
	unsigned char edge = osc_param_sys_get(runmsg->sys_menu_set,14);
	/* check */
	osc_ui_set_trig_edge(edge);
}
/* get enable */
unsigned char osc_enable_chn(unsigned char chn)
{
	/* check */
	if( chn == 0 )
	{
		return osc_param_sys_get(runmsg->sys_menu_set,6);
	}
	else
	{
		return osc_param_sys_get(runmsg->sys_menu_set,9);
	}
}
/* get X10 or X1 */
unsigned char osc_X10X1_get(unsigned char chn)
{
	/* check */
	if( chn == 0 )
	{
		return osc_param_sys_get(runmsg->sys_menu_set,7);
	}
	else
	{
		return osc_param_sys_get(runmsg->sys_menu_set,10);
	}
}
/* get languase */
unsigned char osc_language_get(void)//0 is english , 1 is chinese simple , 2 is chinese fanti
{
	return osc_param_sys_get(runmsg->sys_menu_set,1);
}
/* get format */
unsigned char osc_format(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,3);
}
/* get smartAI */
unsigned char osc_smartAI(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,2);
}
/* get usb device enable */
unsigned char osc_usb_device_enable(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,4);
}
/* get osc math enable */
unsigned char osc_math_enable(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,19);	
}
/* get osc math enable */
unsigned char osc_math_item(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,22);	
}
/* return fft sta */
unsigned char osc_fft_sta(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,23);	
}
/* get persist */
unsigned char osc_persist_get(void)
{
	return osc_param_sys_get(runmsg->sys_menu_set,0);
}