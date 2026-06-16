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
#include "osc.h"
#include "osc_ui.h"
#include "gui.h"
#include "string.h"
#include "math.h"
#include "display_dev.h"
#include "osc_cfg.h"
#include "osc_win.h"
#include "osc_menu.h"
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("osc_ui",osc_create_ui,0,0,6);
/* create cfg task gui detecter task run as 100 ms */
FOS_TSK_REGISTER(osc_ui_task,PRIORITY_4,1000);
/* get version */
extern char * app_version(void);
/* define win */
window_def win_main;
window_def win_group[4];
/* win menu */
window_def win_menu[2];
window_def measure_menu;
/* btn */
widget_def btn[6];
widget_def mearsure_btn[11];
widget_def ctrl_btn[7];
widget_def measure_group[10];
/* channel btn */
widget_def btn_ch[4];
/* bat */
//widget_def bat_icon;
/* text */
widget_def voltage_ch[2];
/* time */
widget_def time_ch[2];
/* ch1 measure */
widget_def measure_ch1[6];
widget_def measure_ch2[6];
/* menu text */
widget_def menu_text[6];
widget_def measure_text[12];
/* tips */
widget_def tips_text;
/* fast tips */
widget_def fast_tips[6];
/* widget arrow */
widget_def base_vol_arrow[2];
widget_def trig_vol_arrow[4];
/* trig lines */
widget_def trig_lines[6];
/* right icon */
widget_def right_icon_hd[11];
/* stop icon */
widget_def stop_icon_wd;
/* table */
char * menu_table[7] = {"控制切换","运行停止","自动调节","单次触发","测量数学","保存波形"};
char * ctrl_menu_table[7] = {"控制切换","CH1+","B","CH1-","CH2+","B","CH2-"};
/* english */
char * menu_table_english[7] = {"Ctrl\nSwitch"," RUN  \n STOP  "," Auto\n Set","Single"," Math \nMeasure"," Save \n Wave "};
char * ctrl_menu_table_english[7] = {"Ctrl/nSwitch","CH1+","B","CH1-","CH2+","B","CH2-"};
/* english end */
char * mert43[3] = {"Ready","M","500us"};
const char * mert43_eng[6] = {"RUN  ","STOP ","Ready","运行","停止","等待"};
/* gui dev */
static gui_dev_def * dev;
/* hold times */
static unsigned short tips_hold_time_s = 0;
/* osc create ui */
int osc_create_ui(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* calculate main size */
  osc_calculate_main_size(dev,&win_main,0);
	/* create the group */
	//osc_calculate_sg_size(dev,win_group,sizeof(win_group) / sizeof(win_group[0]));	
	/* create the menu win ui */
  osc_calculate_menu_size(dev,&win_menu[0],0);
	osc_calculate_menu_size(dev,&win_menu[1],GUI_HIDE);
	/* ------------------- */
	osc_calculate_menu_size(dev,&measure_menu,GUI_HIDE);
	/* btn */
  osc_calculate_btn_size(dev,&win_menu[0],btn,menu_table,menu_table_english,sizeof(btn) / sizeof(btn[0]));
	/* ctrl btn */
	osc_calculate_ctrl_btn_size(dev,&win_menu[1],ctrl_btn,ctrl_menu_table,menu_table_english,sizeof(ctrl_btn) / sizeof(ctrl_btn[0]));
	/* ccd */
	osc_calculate_chn_btn_size(dev,&win_main,btn_ch,sizeof(btn_ch) / sizeof(btn_ch[0]));
	/* bat */
//	osc_calculate_bat_icon_size(dev,&win_main,&bat_icon);
	/* measure measure_group */
	osc_calculate_measure_item(dev,&win_main,measure_group);
	//osc_calculate_btn_size(dev,&measure_menu,mearsure_btn,sizeof(mearsure_btn) / sizeof(mearsure_btn[0]));
	/* voltage */
	osc_calculate_volage_string(&win_group[1],voltage_ch,sizeof(voltage_ch) / sizeof(voltage_ch[0]) , "100mV","200mV");
	/* time */
	osc_calculate_time_string(&win_group[3],time_ch,2,"M","500ms");
	/* measure */
//	osc_calculate_measure_ch(&win_group[0],measure_ch1,6,mert,1);
//	osc_calculate_measure_ch(&win_group[0],measure_ch2,6,mert1,2);	
	/* menu */
	//osc_calculate_menu(&win_menu,menu_text,6,menu_table);
	//osc_calculate_menu(&measure_menu,measure_text,12,measure_ch1_table);
	/* tips */
	osc_calculate_tips(&win_group[2],&tips_text,TIPS_NORMAL,"  ");
	/* arrow */
	osc_calculate_base_arrow(&win_main,&base_vol_arrow[0],1);
	osc_calculate_base_arrow(&win_main,&base_vol_arrow[1],2);
	/* fast title */
	//mert43[5] = app_version();
	/* create and show */
	osc_calculate_title_string(&win_main,fast_tips,3,mert43);
	/* */
	osc_calculate_trig_arrow(&win_main,&trig_vol_arrow[0],1);
	osc_calculate_trig_arrow(&win_main,&trig_vol_arrow[1],2);
	/* test */
//	osc_calculate_trig_arrow(&win_main,&trig_vol_arrow[2],3);
//	osc_calculate_trig_arrow(&win_main,&trig_vol_arrow[3],4);
	/* lines */
	osc_calculate_trig_line(&win_main,&trig_lines[0],1);
	osc_calculate_trig_line(&win_main,&trig_lines[1],2);
//	osc_calculate_trig_line(&win_main,&trig_lines[2],3);
//	osc_calculate_trig_line(&win_main,&trig_lines[3],4);
//	osc_calculate_trig_line(&win_main,&trig_lines[4],5);
//	osc_calculate_trig_line(&win_main,&trig_lines[5],6);	
//	/* ----- */
	//osc_calculate_right_icon(dev,&measure_menu,mearsure_btn,right_icon_hd,sizeof(right_icon_hd) / sizeof(right_icon_hd[0]));
	/* create stop */
	osc_create_stop_icon(dev,&win_main,&stop_icon_wd,fast_tips[0].msg.x - 16,fast_tips[0].msg.y + 2);
	/* return */
	return FS_OK;
}
/* osc ui task */
static void osc_ui_task(void)
{
	/* tips */
	if( osc_ui_tips_sta() == 0 )
	{
		/* shown */
		if( tips_hold_time_s == 0 )
		{
			osc_ui_tips_show(0);//HIDE THE tips
		}
		else
		{
			tips_hold_time_s --;
		}
	}	
}
/* move measurelines */
void osc_ui_move_mlines(unsigned char index,unsigned short pxy)
{	
	/* check */
	if( index < 2 )
	{
		gui_move_wid(&trig_lines[index + 2],0,pxy);
	}
	else if( index < 4 )
	{
		gui_move_wid(&trig_lines[index + 2],pxy,0);
	}
	else
	{
		/* error data */
	}
}
/* find menu botton */
int osc_ui_find_win_btn(unsigned short px,unsigned short py)
{
	/* tmx */
	unsigned short tpx,tpy;
	/* check */
	for( int i = 0 ; i < sizeof(btn) / sizeof(btn[0]) ; i ++ )
	{
		/* transfer to abs posx */
		tpx = win_menu[0].msg.x + btn[i].msg.x;
		tpy = win_menu[0].msg.y + btn[i].msg.y;
		/* check */
		if(( px > tpx && px < tpx + btn[i].msg.x_size ) && 
			( py > tpy && py < tpy + btn[i].msg.y_size ))
		{
			return i;
		}
	}
	/* return error */
	return FS_ERR;
}
/* get tips sta */
int osc_ui_stop_icon_sta(void)
{
	return CHECK_HIDE(stop_icon_wd.msg.wflags) ? 1 : 0;
}
/* set stop icon show or hide */
void osc_ui_stop_icon_show(unsigned char mode)
{
	/* check */
	if( mode )
	{
		gui_show_widget(&stop_icon_wd);
	}
	else
	{
		gui_hide_widget(&stop_icon_wd);
	}
}
/* void toggle */
void osc_ui_stop_icon_toggle(void)
{
	/* check */
	if( osc_ui_stop_icon_sta() )
	{
		gui_show_widget(&stop_icon_wd);
	}
	else
	{
		gui_hide_widget(&stop_icon_wd);
	}
}
/* find menu botton */
int osc_ui_find_measure_btn(unsigned short px,unsigned short py,widget_def ** btn_wd)
{
	/* tmx */
	unsigned short tpx,tpy;
	/* check */
	for( int i = 0 ; i < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]) ; i ++ )
	{
		/* transfer to abs posx */
		tpx = win_menu[0].msg.x + mearsure_btn[i].msg.x;
		tpy = win_menu[0].msg.y + mearsure_btn[i].msg.y;
		/* check */
		if(( px > tpx && px < tpx + mearsure_btn[i].msg.x_size ) && 
			( py > tpy && py < tpy + mearsure_btn[i].msg.y_size ))
		{
			/* return */
			if( btn_wd != 0 )
			{
				*btn_wd = &mearsure_btn[i];
			}
			/* retuirn */
			return i;
		}
	}
	/* return error */
	return FS_ERR;
}
/* find menu botton by */
int osc_ui_find_measure_btn_by(unsigned short py)
{
	/* tmx */
	unsigned short tpy;
	/* check */
	for( int i = 0 ; i < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]) ; i ++ )
	{
		/* transfer to abs posx */
		tpy = win_menu[0].msg.y + mearsure_btn[i].msg.y;
		/* check */
		if( py > tpy && py < tpy + mearsure_btn[i].msg.y_size )
		{
			return i;
		}
	}
	/* return error */
	return FS_ERR;
}
/* find menu botton by */
int osc_ui_find_measure_btn_mem(void)
{
	/* check */
	for( int i = 0 ; i < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]) ; i ++ )
	{
		/* check */
		if( mearsure_btn[i].msg.upd == COLOR_BTN_FOCUS )
		{
			return i;
		}
	}
	/* return error */
	return FS_ERR;
}
/* get len */
int osc_ui_measure_len(void)
{
	return sizeof(mearsure_btn) / sizeof(mearsure_btn[0]);
}
/* find menu botton by */
void osc_ui_clear_btn_mem(void)
{
	/* check */
	for( int i = 0 ; i < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]) ; i ++ )
	{
		/* check */
		mearsure_btn[i].msg.upd = COLOR_BUTTON;
	}
}
/* set botton to focus */
void osc_ui_focus_btn(unsigned char btn_index,unsigned char mode)
{
	/* check */
	if( btn_index < sizeof(btn) / sizeof(btn[0]))
	{
		osc_btn_color(&win_menu[0],&btn[btn_index],mode);
	}
}
/* set botton to focus */
void osc_ui_focus_ctrl_btn(unsigned char btn_index,unsigned char mode)
{
	/* check */
	if( btn_index < sizeof(ctrl_btn) / sizeof(ctrl_btn[0]))
	{
		osc_btn_color(&win_menu[1],&ctrl_btn[btn_index],mode);
	}
}
/* set botton to focus */
void osc_ui_focus_measure_btn(unsigned char btn_index,unsigned char mode)
{
//	/* check */
//	if( btn_index < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]))
//	{
//		osc_btn_color(&win_menu[0],&mearsure_btn[btn_index],mode);
//	}
}
/* release */
void osc_ui_release_ctrl_btn(void)
{
	/* check */
	for( int i = 0 ; i < sizeof(ctrl_btn) / sizeof(ctrl_btn[0]) ; i ++ )
	{
		/* check */
		if( ctrl_btn[i].msg.my == COLOR_BG_FUCUS_BTN )
		{
			osc_btn_color(&win_menu[1],&ctrl_btn[i],0);
		}
	}
}
/* release the ctrl buttom */
void osc_ui_release_btn(void)
{
	/* check */
	for( int i = 0 ; i < sizeof(btn) / sizeof(btn[0]) ; i ++ )
	{
		/* check */
		if( btn[i].msg.my == COLOR_BG_FUCUS_BTN )
		{
			osc_btn_color(&win_menu[0],&btn[i],0);
		}
	}
}
/* release the btn */
void osc_ui_release_measure_btn(void)
{
	for( int i = 0 ; i < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]) ; i ++ )	
	{
		/* check */
		if( mearsure_btn[i].msg.upd == COLOR_BTN_FOCUS )
		{
			osc_ui_focus_measure_btn(i,0);
		}
	}
}
/* release the button */
void osc_ui_release_top_btn(void)
{
	for( int i = 0 ; i < sizeof(btn_ch) / sizeof(btn_ch[0]) ; i ++ )	
	{
		/* check */
		if( btn_ch[i].msg.upd == COLOR_BTN_FOCUS )
		{
			osc_top_color(&btn_ch[i],0);
		}
	}
}
/* release the btn */
int osc_ui_current_measure_btn(widget_def ** btn_wd)
{
	for( int i = 0 ; i < sizeof(mearsure_btn) / sizeof(mearsure_btn[0]) ; i ++ )	
	{
		/* check */
		if( mearsure_btn[i].msg.upd == COLOR_BTN_FOCUS )
		{
			/* return the btn */
			* btn_wd = &mearsure_btn[i];
			/* return */
			return i;
		}
	}
	/* return FS_EERR*/
	return FS_ERR;
}
/* flush right icon */
void osc_ui_flush_right_icon(unsigned int run_flag)
{
	/* check */
	for( int i = 0 ; i < sizeof(right_icon_hd) / sizeof(right_icon_hd[0]) ; i ++ )
	{
		/* check */
		if( (run_flag >> i ) & 0x01)
		{
			gui_show_widget(&right_icon_hd[i]);
		}
	}
}
/* int osc menu sta .show or hide */
int osc_uiright_sta(unsigned char index)
{
	/* sta win_menu */
	return CHECK_HIDE(right_icon_hd[index].msg.wflags) ? 1 : 0;
}
/* show or hide right */
void osc_ui_right_show(unsigned char index , unsigned char mode)
{
	/* check */
	if( index >= sizeof(right_icon_hd) / sizeof(right_icon_hd[0]) )
	{
		/* return */
		return;
	}
	/* show or hide */
	if( mode )
	{
		gui_show_widget(&right_icon_hd[index]);
	}
	else
	{
		gui_hide_widget(&right_icon_hd[index]);
	}
}
/* set time text */
void osc_ui_time_str(char * str)
{
	//gui_set_wid_text(&fast_tips[2],str);
	unsigned short px = fast_tips[2].msg.x;
	/* show */
	while(*str)
	{
		/* show*/
		gui_char(dev,px,fast_tips[2].msg.y,*str,24,COLOR_CHAR,COLOR_BACK_GROUND,1);
		/* inc */
		str ++;
		/* px */
		px += 12;
	}
}
/* void osc ui tips show hide */
void osc_ui_tips_show(unsigned char mode)
{
	/* show or hide */
	if( mode )
	{
		gui_show_widget(&tips_text);
		/* set hold time */		
		tips_hold_time_s = 5; // 5s			
	}
	else
	{
		gui_hide_widget(&tips_text);
	}
}
/* set time text */
void osc_ui_tips_str(char * str)
{
	/* check hide */
	if( CHECK_HIDE(tips_text.msg.wflags))
	{
		/* show */
		gui_show_widget(&tips_text);
		/* set text */
		gui_set_wid_text(&tips_text,str);
    /* set hold time */		
		tips_hold_time_s = 5; // 5s
	}
}
/* set tip text dir*/
void osc_ui_tips_str_dir(char * str)
{
	/* show */
	gui_show_widget(&tips_text);
	/* set text */
	gui_set_wid_text(&tips_text,str);		
	/* set hold time */		
	tips_hold_time_s = 5; // 5s	
}
/* get tips sta */
int osc_ui_tips_sta(void)
{
	return CHECK_HIDE(tips_text.msg.wflags) ? 1 : 0;
}
/* move arrow */
void osc_ui_move_offset_arrow(unsigned char chn,unsigned short pos)
{
	/* limit */
	if( chn < 2 )
	{
		osc_draw_arrow_noload(&base_vol_arrow[chn],pos,chn);
	}
}
/* show and hide arrow */
void osc_ui_show_offset_arrow(unsigned char chn , unsigned char mode)
{
	/* limit */
	if( chn < 2 )
	{
		/* show or hide */
		if( mode )
		{
			gui_show_widget(&base_vol_arrow[chn]);
		}
		else
		{
			gui_hide_widget(&base_vol_arrow[chn]);
		}		
	}		
}
/* move trig arrow */
void osc_ui_move_trig_arrow(unsigned char chn,unsigned short pos)
{
		/* limit */
	if( chn < 2 )
	{
		gui_move_wid(&trig_vol_arrow[chn] ,trig_vol_arrow[chn].msg.x , pos);
	}
}
/* move trig arrow */
void osc_ui_trig_arrow_show(unsigned char chn,unsigned char mode)
{
	/* limit */
	if( chn < 2 )
	{
		/* show or hide */
		if( mode )
		{
			/* flush */
			trig_vol_arrow[chn].msg.y = trig_vol_arrow[chn].msg.my;
			/* show */
			gui_show_widget(&trig_vol_arrow[chn]);
		}
		else
		{
			gui_hide_widget(&trig_vol_arrow[chn]);
		}
	}
}
/* check trig arrow hide */
int osc_ui_trig_arrow_sta(unsigned char chn)
{
	/* limit */
	if( chn < 2 )
	{
		return CHECK_HIDE(trig_vol_arrow[chn].msg.wflags) ? 0 : 1;
	}	
	/* return error */
	return FS_ERR;
}
/* change */
void osc_ui_vol_scale(unsigned char chn , char * str )
{
	/* limit */
	if( chn == 0 )
	{	
	  draw_vol_scale(&btn_ch[0],str);
	}
	else if( chn == 1 )
	{
		draw_vol_scale(&btn_ch[1],str);
	}
	else if( chn == 2 )
	{
		/* get data */
		const osc_vol_scale_def * fes = osc_get_vol_scale(0);
		/* set */
		if( osc_X10X1_get(0) == 0 )
		{
			draw_vol_scale(&ctrl_btn[2],fes->strX10);
		}
		else
		{
			draw_vol_scale(&ctrl_btn[2],fes->str);
		}
	}
	else
	{
		/* get data */
		const osc_vol_scale_def * fes = osc_get_vol_scale(1);
		/* set */
		/* set */
		if( osc_X10X1_get(1) == 0 )
		{		
			draw_vol_scale(&ctrl_btn[5],fes->strX10);		
		}
		else
		{
			draw_vol_scale(&ctrl_btn[5],fes->str);		
		}
	}
}
/* osc move trig lines */
void osc_ui_move_trig_lines(unsigned char chn,unsigned short posy)
{
	/* limit */
	if( chn < 2 )	
	{
		gui_move_wid(&trig_lines[chn],0,posy);
	}
}
/* void osc show hide trig lies */
void osc_ui_trig_lines_show(unsigned char chn,unsigned char mode)
{
	/* limit */
	if( chn < 2 )	
	{
		/* mode */
		if( mode == 0 )
		{
			gui_hide_widget(&trig_lines[chn]);
		}
		else
		{
			gui_show_widget(&trig_lines[chn]);
		}
	}	
}
/* void osc ui measure title */
void osc_ui_measure_capital(unsigned char chn,unsigned char item,const char * capital)
{
	/* limit */
	if( item > 2 )
	{
		/* cannot supply now */
		return;
	}		
	/* limit */
	if( chn == 0 )	
	{
		gui_set_wid_text(&measure_ch1[item*2],(char *)capital);
	}
  else
	{
		gui_set_wid_text(&measure_ch2[item*2],(char *)capital);
	}		
	/* end of func */
}
/* void osc ui measure data */
void osc_ui_measure_data(unsigned char chn,unsigned char item,char * capital)
{
	/* limit */
	if( item > 2 )
	{
		/* cannot supply now */
		return;
	}		
	/* limit */
	if( chn == 0 )	
	{
		gui_set_wid_text(&measure_ch1[item*2 + 1],capital);
	}
  else
	{
		gui_set_wid_text(&measure_ch2[item*2 + 1],capital);
	}		
	/* end of func */
}
/* int osc menu sta .show or hide */
int osc_ui_menu_sta(void)
{
	/* sta win_menu */
	return CHECK_HIDE(win_menu[0].msg.wflags) ? 1 : 0;
}
/* win menu size */
gui_info_def * osc_ui_win_meun_msg(void)
{
	return &win_menu[0].msg;
}
/* measure menu size */
gui_info_def * osc_ui_measure_meun_msg(void)
{
	return &measure_menu.msg;
}
/* int osc menu sta .show or hide */
int osc_ui_measure_sta(void)
{
	/* sta win_menu */
	return CHECK_HIDE(measure_menu.msg.wflags) ? 1 : 0;
}
/* set menu sta */
void osc_ui_menu_show(unsigned char mode)
{
	/* show or hide */
	if( mode )
	{
		gui_show_win_noload(&win_menu[0]);
	}
	else
	{
		//gui_hide_win(&win_menu[0]);
		osc_clear_win(&win_menu[0]);
	}
}
/* set menu sta */
void osc_ui_menu1_show(unsigned char mode)
{
	/* show or hide */
	if( mode )
	{
		gui_show_win_noload(&win_menu[1]);
	}
	else
	{
		//gui_hide_win(&win_menu[1]);
		osc_clear_win(&win_menu[1]);
	}
}
/* measure_menu */
void osc_ui_measure_show(unsigned char mode)
{
	/* show or hide */
	if( mode )
	{
		/* only */
		if( osc_ui_menu_sta() )
		{
			gui_show_win(&measure_menu);
		}
	}
	else
	{
		gui_hide_win(&measure_menu);
	}
}
/* void osc ui set menu text */
void osc_ui_set_menu_text_group(const char ** text,unsigned int len)
{
	/* check len */
	if( len != sizeof(menu_table) / sizeof(menu_table[0]) )
	{
		return;/* cannot supply this format */
	}
	/* set */
	for( int i = 0 ; i < len ; i ++ )
	{
		gui_set_wid_text(&menu_text[i],(char *)text[i]);
	}
}
/* void osc set one text */
void osc_ui_set_one_menu_text(unsigned char item,const char * text)
{
	/* check len */
	if( item >= sizeof(menu_table) / sizeof(menu_table[0]) )
	{
		return;/* cannot supply this format */
	}	
	/* set */
	gui_set_wid_text(&menu_text[item],(char *)text);
	/* end of func */
}
/* void osc ui get fast tips text */
void osc_ui_set_chn_text(unsigned char chn,const char * text)
{
	/* select chn */
	if( chn == 0 )
	{
		//gui_set_wid_text(&fast_tips[0],( char * )text);
	}
	else
	{
		//gui_set_wid_text(&fast_tips[1],( char * )text);
	}
}
/* set osc ui fast coupling show or hide */
void osc_ui_set_csh_show(unsigned char chn,unsigned char mode)
{
	if( chn >= 2 )
	{
		return;
	}
	/* show or hide */
	if( mode == 0 )
	{
		//gui_hide_widget(&fast_tips[chn]);
	}
	else
	{
		//gui_show_widget(&fast_tips[chn]);
	}
}
/* set trig text */
/* void osc ui get fast tips text */
void osc_ui_set_trig_text(const char * text)
{
	/* select chn */
	//gui_set_wid_text(&fast_tips[2],( char * )text);
}
/* set trig trig source text */
void osc_ui_set_trig_src(const char * text)
{
	/* select chn */
	//gui_set_wid_text(&fast_tips[3],( char * )text);
}
/* void osc point change */
void osc_ui_set_focus_arrow(unsigned char chn)
{
	/* check chn */
	if( chn >=2 )
	{
		return;
	}
	/* change */
	if( chn == 0 )
	{
		gui_show_widget(&trig_vol_arrow[2]);
		gui_hide_widget(&trig_vol_arrow[3]);
	}
	else
	{
		gui_show_widget(&trig_vol_arrow[3]);
		gui_hide_widget(&trig_vol_arrow[2]);
	}
}
/* set UI trig type */
void osc_ui_set_trig_type(unsigned char index)
{
	draw_trig_type(&btn_ch[2],index);
}
/* draw_trig_source */
void osc_ui_set_trig_source(unsigned char index)
{
	draw_trig_source(&btn_ch[2],index);
}
/* trig mode */
void osc_ui_set_trig_edge(unsigned char index)
{
	draw_trig_icon(&btn_ch[2],index);
}
/* draw_x1_x10 */
void osc_ui_set_x10_x1(unsigned char chn,unsigned char index)
{
	draw_x1_x10(&btn_ch[chn],index);
}
/* DCAC draw_dcac */
void osc_ui_set_dcac(unsigned char chn,unsigned char index)
{
	draw_dcac(&btn_ch[chn],index);
}
/* draw item */
unsigned short osc_ui_measure_item(unsigned char chn,unsigned char item)
{
	/* check availed pos */
	int i = 0;
	/* check */
	for( ; i < 10 ; i ++ )
	{
		/* check availed pos */
		if( measure_group[i].msg.upd != 0xE0 )
		{
			break;
		}
	}
	/* get */
	if( i == 10 )
	{
		/* error */
		/* tips */
		return 0xff;
	}
	/* get pos */
	draw_measure_item(&measure_group[i],chn,item);
	/* return */
	return i;
}
/* update the language */
void osc_language_update_measure(void)
{
	/* change */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* check */
		if( measure_group[i].msg.upd == 0xE0 )
		{
			/* check */
			draw_measure_item(&measure_group[i],measure_group[i].msg.mx,measure_group[i].msg.my);
		}
	}
}
/* release data */
void osc_ui_release_area(unsigned short pos_id)
{
	/* check */
	if( pos_id >= 10 )
	{
		return;
	}
	/* releae */
	if( pos_id < 5 )
	{
		osc_release_area(&measure_group[pos_id],0);
	}
	else
	{
		/* create bakc */
		osc_release_area(&measure_group[pos_id],1);
		/* redraw grid */
		osc_ui_redraw_area(&measure_group[pos_id]);
	}
	/* end */
	measure_group[pos_id].msg.upd = 0;
	/* diff */
}
/* release ares */
void osc_ui_redraw_area(widget_def * wid)
{
	/* set remark relash */
	win_main.msg.mark_flag = 1;
	/* set */
	win_main.msg.mx = wid->msg.x;
	win_main.msg.my = wid->msg.y;
	win_main.msg.mxstop = wid->msg.x_size + wid->msg.x;
	win_main.msg.mystop = wid->msg.y_size + wid->msg.y;
	/* release */
	osc_redraw_grid(&win_main);
	/* release */
	win_main.msg.mark_flag = 0;
}
/* set default measure item */
void osc_ui_default_measure_item(void)
{
	/* get run msg for osc */
	osc_run_msg_def * runmsg = get_run_msg();	
	/* set */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* check ch1 */
		if( runmsg->measure_item[0] & ( 1 << i ))
		{
			osc_ui_measure_item(0,i);
		}	
	}
	/* set */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* check ch1 */
		if( runmsg->measure_item[1] & ( 1 << i ))
		{
			osc_ui_measure_item(1,i);
		}	
	}	
}
/* search pos id */
unsigned char osc_search_pos_is(unsigned char chn,unsigned item)
{
	/* check */
	for( int  i = 0 ; i < 10 ; i ++ )
	{
		/* check */
		if( measure_group[i].msg.upd == 0xE0 )
		{
			/* check */
		  if( measure_group[i].msg.mx == chn && measure_group[i].msg.my == item )
			{
				return i;
			}
		}
	}
	/* return error */
	return 0xff;
}
/* reflush fds */
void osc_side_btn_update_languase(void)
{
	/* for */
	if( !osc_ctrl_menu_sta() )
	{
		osc_redraw_languse(&ctrl_btn[0]);
	}
	else
	{
		for( int i = 0 ; i < 6 ; i ++ )
		{
			osc_redraw_languse(&btn[i]);
		}
	}
}
/* draw ui */
void osc_run_stop(unsigned char index)
{
	osc_draw_rsr(&fast_tips[0],index);
}
/* show format */
void osc_show_format(unsigned char pos_id,unsigned char show_id)
{
	draw_sys_settings(&btn_ch[3],pos_id,show_id);
}








































