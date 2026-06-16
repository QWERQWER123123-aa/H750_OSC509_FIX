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
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "hal_gpio.h"
#include "hal_tim.h"
#include "osc_api.h"
#include "osc_cfg.h"
#include "math.h"
#include "string.h"
#include "hal_exit.h"
#include "osc_line.h"
#include "osc_menu.h"
#include "osc_ui.h"
#include "osc_win.h"
/* functions declare */
#include "arm_math.h"
/* tes */
static void osc_thread(void);
static int osc_thead_init(void);
extern void user_back_light(unsigned short pwm);
extern void osc_exit_edge_reg(unsigned char mo);
extern unsigned char osc_trig_edge(void);
extern unsigned char osc_trig_type(void);
/* Includes ------------------------------------------------------------------*/
FOS_TSK_REGISTER(osc_thread,PRIORITY_IDLE,100); /* gui detecter task run as idle */
FOS_INODE_REGISTER("osc_thread",0,osc_thead_init,0,9);
/* buffer */
static unsigned char clock_sta = 0;
/* gui dev */
static gui_dev_def * dev;
/* cnt p */
static unsigned int cnt_p = 0;
/* line_buffer */
static unsigned short line_buffer_ch1[2][800];
static unsigned short line_buffer_ch2[2][800];
static unsigned short line_buffer_fft[2][800];
static unsigned short line_buffer_math[2][800];
static unsigned short line_buffer_cht1[800];
static unsigned short line_buffer_cht2[800];
/* zoom */
unsigned short line_zoom[2];
/* ins */
unsigned int osc_inter_s = 0;
/* time sw */
const osc_time_def * osc_time_sw;
/* area msg */
const draw_area_def * area;
/* run msg */
osc_run_msg_def * runmsg;
/* run mode not save */
static unsigned char run_mode = 0 ; // 0 is run , 1 is stop , 2 is ready
//static unsigned int osc_tflag = 0;
static unsigned int deep_u = 0;
/* exter */
extern char OSC_DMA_CHECK_TC(void);
extern void OSC_DMA_CLEAR_TC(void);
extern void osc_dma_restart(unsigned int len);
extern unsigned char cal_step_start;
extern void osc_calibrate_mean(unsigned short * df0 ,unsigned short * df1);
extern unsigned char osc_fm;
/* FFT */
#define NPT  2048
float32_t  fftInput_f32[NPT];
float32_t  fftOutput_f32[NPT];
float32_t  fftOutput[NPT];
/* USER CODE END 2 */
arm_rfft_fast_instance_f32  S;
uint8_t ifftFlag = 0;  
/* fft run sta */
static unsigned char fft_sta = 0;
static unsigned char format_xy;
/* persistence counter for decay */
static unsigned int persist_frame_cnt = 0;
/* persistence frame count per decay */
static unsigned int persist_decay_frames = 0;
/* get persist frame count based on setting */
static unsigned int get_persist_decay_frames(unsigned char persist)
{
	switch(persist)
	{
		case 0: /* OFF */
			return 1;
		case 1: /* 1sec */
			return 6;  /* clear every ~6 frames */
		case 2: /* 2sec */
			return 14; /* clear every ~14 frames */
		case 3: /* 5sec */
			return 30; /* clear every ~30 frames */
		case 4: /* infinite */
		default:
			return 0; /* never decay */
	}
}
/* return run sta */
unsigned char osc_run_mode(void)
{
	return run_mode & 0x3;
}
/* set run node */
void osc_set_run_mode(unsigned char m)
{
	run_mode = m & 0x3;
}

float max_freq;
unsigned int fifo_deep = 62*1024;
float scan_gird = 10;//us
float zoom;

void tese(void)
{
	max_freq = (float)fifo_deep / ( scan_gird * 14.0f );
}

/* upload default setting */
static int osc_thead_init(void)
{
	tese();
	/* gui dev get */
  dev = get_gui_dev();
	/* get run msg for osc */
	runmsg = get_run_msg();
	/* get area */
	area = get_draw_area_msg();
	/* update ch1 */
  osc_offset_scale_thread(0);
	/* update ch2 */
	osc_offset_scale_thread(1);
	/* default time ---------------------------------------------------------------------------------------------------------------------------------------------------------*/
	osc_ui_time_str(osc_time_get()->str);
	/* get time */
	osc_time_sw = osc_scan_time(runmsg->time_scale);
	/* set default X10 X1 */
	osc_ui_set_x10_x1(0,osc_X10X1_get(0));
	osc_ui_set_x10_x1(1,osc_X10X1_get(1));
	/* default fes */
	unsigned char tmd = runmsg->vol_scale_ch[0];
	/* set */
	if( osc_X10X1_get(0) == 0 )
	{
		osc_ui_vol_scale(0,osc_vol_scale_s(tmd)->strX10);
	}
	else
	{
		osc_ui_vol_scale(0,osc_vol_scale_s(tmd)->str);
	}
	/* check */
	osc_vol_scale_set_dir(0,tmd);
	/* set ch2 */
	tmd = runmsg->vol_scale_ch[1];
	/* set */
	if( osc_X10X1_get(0) == 0 )
	{		
		osc_ui_vol_scale(1,osc_vol_scale_s(tmd)->strX10);
	}
	else
	{
		osc_ui_vol_scale(1,osc_vol_scale_s(tmd)->str);
	}
	/* check */
	osc_vol_scale_set_dir(1,tmd);
	/* set default dcac */
	osc_refluse_dcac_chn();
	/* set trig souc */
	osc_ui_set_trig_source(osc_trig_source());
	/* set trig edge ui */
	osc_default_edge_ui();
	/* set default item */
	osc_ui_default_measure_item();
	osc_cursor_init_default();
	/* set default run mode */
	osc_run_stop(osc_run_mode());
	/* check trig source */
	osc_trig_scale_thread(0);
	osc_trig_scale_thread(1);
	/* check */
	if( osc_trig_source() == 0 )
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
	/* set default backlight pwm */
	/* set backlight */
	user_back_light(100 + runmsg->back_light_per * 9 - 1 );
	/* set */
	osc_exit_edge_reg(osc_trig_edge());
	/* trig type */
	extern unsigned char trt;
	/* get */
	trt = osc_trig_type();
	/* set */
	osc_ui_set_trig_type(trt);
	/* FFT init */
	arm_rfft_fast_init_f32(&S, 2048); 
	/* fft sta */
	fft_sta = osc_fft_sta(); 
	/* get format */
	format_xy = osc_format();
	/* return as usual */
	return FS_OK;
}
/* delay for yuhui */
static void delay_us_yo(unsigned int t)
{
	t *=100;
	
	while(t--);
}

extern unsigned char dms;
extern unsigned char chn_stop_move;
extern signed short move_chn_dp[2];
unsigned char mcd = 0;
extern float rms_buffer_ori0[800];
extern float rms_buffer_ori1[800];
float_t rms_ch[2];
/* gui task */
static void osc_thread(void)
{
	/* check run mode */
	if( osc_run_mode() == RUN_STOP_MODE && chn_stop_move == 0 )	
	{
		return;
	}
	/* nothing to do */
	if( ! OSC_DMA_CHECK_TC() && chn_stop_move == 0 )
	{
		return; // not use to deal
	}
	/*--------------*/
	OSC_DMA_CLEAR_TC();
	/* check */
	if( chn_stop_move )
	{
		if( mcd == 0 )
		{
			mcd = 1;
			memcpy(line_buffer_cht1,line_buffer_ch1,sizeof(line_buffer_cht1));
			memcpy(line_buffer_cht2,line_buffer_ch2,sizeof(line_buffer_cht2));
		}
	}
	else
	{

	}
	/* ok we've some data , stop the clock first */
	unsigned char trigd = osc_trig_source();
	/* get trig soucre */
	unsigned char trig_edge = osc_trig_edge();
	int ret = FS_ERR;
	/* transfor data */
	if( chn_stop_move == 0 )
	{
		ret = osc_trig_read(line_buffer_ch1[cnt_p%2],line_buffer_ch2[cnt_p%2],trig_edge,trigd,clock_sta,osc_inter_s,deep_u);
		/* rms */
		arm_rms_f32(rms_buffer_ori0,400,&rms_ch[0]);
		arm_rms_f32(rms_buffer_ori1,400,&rms_ch[1]);
		/* math */
		if( osc_math_enable() )
		{
			unsigned char math_op = osc_math_item() & 0x3;
			osc_create_math_data(line_buffer_math[cnt_p%2], math_op);
		}
	}
	else
	{
		if( chn_stop_move == 1 )
		{
			for( int i = 0 ; i < 800 ; i ++ )
			{
				line_buffer_ch1[cnt_p%2][i] = line_buffer_cht1[i] + move_chn_dp[0];
				/* check */
				if( (signed short)line_buffer_ch1[cnt_p%2][i] >= 399 )
				{
					line_buffer_ch1[cnt_p%2][i] = 399;
				}
				/*-------------*/
				if( (signed short)line_buffer_ch1[cnt_p%2][i] < 0 )
				{
					line_buffer_ch1[cnt_p%2][i] = 0;
				}				
			}
		}
		else if( chn_stop_move == 2 )
		{
			for( int i = 0 ; i < 800 ; i ++ )
			{
				line_buffer_ch2[cnt_p%2][i] = line_buffer_cht2[i] + move_chn_dp[1];
				/* check */
				if( (signed short)line_buffer_ch2[cnt_p%2][i] >= 399 )
				{
					line_buffer_ch2[cnt_p%2][i] = 399;
				}
				/*-------------*/
				if( (signed short)line_buffer_ch2[cnt_p%2][i] < 0 )
				{
					line_buffer_ch2[cnt_p%2][i] = 0;
				}					
			}
		}
	}
#if 0
	/* single mode */
	if( (runmsg->trig_mode == RUN_TRIG_SINGLE && ret == FS_OK) || 
		   runmsg->trig_mode == RUN_TRIG_AUTO || runmsg->trig_mode == RUN_TRIG_SMART || 
	    (runmsg->trig_mode == RUN_TRIG_NORMAL && ret == FS_OK )
	  )
#endif
	if( cal_step_start )
	{
		/* cds */
		osc_calibrate_mean(line_buffer_ch1[cnt_p%2],line_buffer_ch2[cnt_p%2]);
	}
#if 1 // FFT
	/* fft sta */
	fft_sta = osc_fft_sta(); 
	/* get */
	osc_time_sw = osc_time_get();	
	/* check */
  if( fft_sta != 0 )
	{
		/* check */
		unsigned char chn = fft_sta == 1 ? 0 : 1;
		/* get data */
		osc_fft_input(fftInput_f32,chn,2048);
		/* 1024 point */
		arm_rfft_fast_f32(&S, fftInput_f32, fftOutput_f32, ifftFlag);	
		/* transfer */
		arm_cmplx_mag_f32(fftOutput_f32, fftOutput, 2048); 
		/* float */
		unsigned int max_index;
		float maxValue;
		/* Calculates maxValue and returns corresponding BIN value */
		arm_max_f32(&fftOutput[1], 2048 / 2 - 1, &maxValue, &max_index);		
		/* create */
		osc_fft_to_lcd(fftOutput,line_buffer_fft[cnt_p%2],maxValue,max_index);
	}
	/* check XY or YT mode */
	if( osc_fm != 0xff )
	{
		format_xy = osc_fm;
		osc_fm = 0xff;
	}
#endif
	{
		unsigned char persist = osc_persist_get();
		/* get zoom buffer */
		line_zoom[cnt_p%2] = osc_time_sw->osc_zoom_factor;
		/* check Y-T mode */
		if( format_xy == 0 )
		{
			/* ch1 enable */
			if( osc_enable_chn(0) == 0 ) // enable
			{
				/* show line ch1 */
				osc_create_lines(dev,line_buffer_ch1[cnt_p%2],0,cnt_p%2,0,line_zoom[cnt_p%2]);
			}
			/* ch2 enable */
			if( osc_enable_chn(1) == 0 ) // enable
			{		
				/* show line ch2 */
				osc_create_lines(dev,line_buffer_ch2[cnt_p%2],0,cnt_p%2,1,line_zoom[cnt_p%2]);
			}
			/* fft enable */
			if( fft_sta ) // fft
			{		
				/* show line fft */
				osc_create_lines(dev,line_buffer_fft[cnt_p%2],0,cnt_p%2,2,line_zoom[cnt_p%2]);
			}
			/* math enable */
			if( osc_math_enable() ) // math
			{
				/* show math line */
				osc_create_lines(dev,line_buffer_math[cnt_p%2],0,cnt_p%2,3,line_zoom[cnt_p%2]);
			}
		}
		else
		{
			/* XY mode */
			osc_create_points_xy(dev,line_buffer_ch1[cnt_p%2],line_buffer_ch2[cnt_p%2],0,cnt_p%2,4);
		}
		/* persistence-aware clear logic */
		if( cnt_p >= 1 )
		{
			if( persist > 0 )
			{
				if( persist_decay_frames != get_persist_decay_frames(persist) )
				{
					persist_decay_frames = get_persist_decay_frames(persist);
					persist_frame_cnt = 0;
					osc_clear_all_lines();
				}
				persist_frame_cnt++;
				if( persist_decay_frames > 0 && persist_frame_cnt >= persist_decay_frames )
				{
					persist_frame_cnt = 0;
					osc_clear_all_lines();
				}
			}
			else
			{
				delay_us_yo(20);
				if( cnt_p % 2 )
				{
					if( format_xy == 0 )
					{
						if( osc_enable_chn(0) == 0 )
						osc_create_lines(dev,line_buffer_ch1[0],1,0,0,line_zoom[0]);
					if( osc_enable_chn(1) == 0 )
						osc_create_lines(dev,line_buffer_ch2[0],1,0,1,line_zoom[0]);
					if( fft_sta )
					osc_create_lines(dev,line_buffer_fft[0],1,0,2,line_zoom[0]);
				osc_create_lines(dev,line_buffer_math[0],1,0,3,line_zoom[0]);
			}
			else
				osc_create_points_xy(dev,line_buffer_ch1[0],line_buffer_ch2[0],1,0,4);
		}
		else
		{
			if( format_xy == 0 )
			{
				if( osc_enable_chn(0) == 0 )
					osc_create_lines(dev,line_buffer_ch1[1],1,1,0,line_zoom[1]);
				if( osc_enable_chn(1) == 0 )
					osc_create_lines(dev,line_buffer_ch2[1],1,1,1,line_zoom[1]);
				if( fft_sta )
					osc_create_lines(dev,line_buffer_fft[1],1,1,2,line_zoom[1]);
				osc_create_lines(dev,line_buffer_math[1],1,1,3,line_zoom[1]);
					}
					else
						osc_create_points_xy(dev,line_buffer_ch1[1],line_buffer_ch2[1],1,1,4);
				}
			}
		}
		__set_PRIMASK(1);
		osc_draw_cursor_lines(dev);
		__set_PRIMASK(0);
		/* single trigger mode: switch to STOP when trigger detected */
		if( ret == FS_OK && osc_trig_type() == RUN_TRIG_SINGLE )
		{
			osc_set_run_mode(RUN_STOP_MODE);
			osc_run_stop(RUN_STOP_MODE);
			osc_param_sys_set(&runmsg->sys_menu_set, RUN_TRIG_AUTO, 15);
			osc_ui_set_trig_type(RUN_TRIG_AUTO);
			extern unsigned char trt;
			trt = RUN_TRIG_AUTO;
		}
		cnt_p++;
	}
	/*--------------*/
	if( chn_stop_move == 0 )
	{	
		/* reser */
		osc_dma_restart(0);
	}
}
/* void line clear */
void osc_clear_all_lines(void)
{
	osc_clear_screen(dev);
	/* reset lows */
	//osc_lows_reset();
}
/* clear only cursor line - horizontal (amp cursor) */
void osc_clear_cursor_line_h(unsigned short old_cursor_pos)
{
	draw_area_def * area = get_draw_area_msg();
	if( dev == 0 || area == 0 ) return;
	unsigned short sx = area->start_pos_x;
	unsigned short ex = area->stop_pos_x;
	unsigned short y = area->start_pos_y + old_cursor_pos;
	unsigned short ey = area->stop_pos_y;
	if( y >= ey ) return;
	for( unsigned short x = sx; x < ex; x++ )
	{
		unsigned char pix = dev->read_point(x, y);
		if( pix == COLOR_MEASURE_LINE_H1_0 || pix == COLOR_MEASURE_LINE_H1_1 )
			dev->set_noload_point(x, y, COLOR_GRID_POINT);
		else if( pix == COLOR_MEASURE_LINE_H0_0 || pix == COLOR_MEASURE_LINE_H0_1 )
			dev->set_noload_point(x, y, COLOR_GRID_AREA_BG);
	}
}
/* clear only cursor line - vertical (time cursor) */
void osc_clear_cursor_line_v(unsigned short old_cursor_pos)
{
	draw_area_def * area = get_draw_area_msg();
	if( dev == 0 || area == 0 ) return;
	unsigned short sy = area->start_pos_y;
	unsigned short ey = area->stop_pos_y;
	unsigned short x = area->start_pos_x + old_cursor_pos;
	unsigned short ex = area->stop_pos_x;
	if( x >= ex ) return;
	for( unsigned short y = sy; y < ey; y++ )
	{
		unsigned char pix = dev->read_point(x, y);
		if( pix == COLOR_MEASURE_LINE_V1_0 || pix == COLOR_MEASURE_LINE_V1_1 )
			dev->set_noload_point(x, y, COLOR_GRID_POINT);
		else if( pix == COLOR_MEASURE_LINE_V0_0 || pix == COLOR_MEASURE_LINE_V0_1 )
			dev->set_noload_point(x, y, COLOR_GRID_AREA_BG);
	}
}
void osc_move_cursor_h_redraw(unsigned short old_cursor_pos)
{
	osc_clear_cursor_line_h(old_cursor_pos);
	osc_draw_cursor_lines(dev);
}
void osc_move_cursor_v_redraw(unsigned short old_cursor_pos)
{
	osc_clear_cursor_line_v(old_cursor_pos);
	osc_draw_cursor_lines(dev);
}