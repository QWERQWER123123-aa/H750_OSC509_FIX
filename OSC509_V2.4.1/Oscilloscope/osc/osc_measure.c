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
#include "hal_tim.h"
#include "osc_ui.h"
#include "gui.h"
#include "string.h"
#include "math.h"
#include "osc_measure.h"
#include "stdio.h"
#include "osc_api.h"
#include "osc.h"
#include "osc_cfg.h"
#include "osc_menu.h"
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("osc_measure",osc_measure_thread_init,0,0,14);
FOS_TSK_REGISTER(osc_measure_thread,PRIORITY_0,1000);/* run as 1s */
/* extern */
extern unsigned char osc_run_mode(void);
extern widget_def measure_group[10];
extern unsigned char osc_X10X1_get(unsigned char chn);
static unsigned short max,min;
/* static freq mearsure */
static int freq_measure_ch(unsigned char chn , char * buf)
{
	/* set data */
	static unsigned int last_sys_time_us[2];
	static unsigned int last_tim2_cnt_pulse[2];	
	/* get time */
	unsigned int now_time = hal_sys_time_us();
	/* calbrate freq */
	unsigned int now_cnt = ( chn == 0 ) ? hal_tim2_cnt() : hal_tim4_cnt();
	/* cnt tes */
	float freq = (float)( now_cnt - last_tim2_cnt_pulse[chn] ) / ((float)( now_time - last_sys_time_us[chn] ) / 1000000) ;
	/* reset */
	last_tim2_cnt_pulse[chn] = now_cnt;
	last_sys_time_us[chn] = now_time;
	/* check */
	if( freq > 1000000 )
	{
		/* gewei */
		if( freq < 10000000 )
		{
		  sprintf(buf,"%1.3fMHz",freq / 1000000.0f);
		}
		else if( freq >= 10000000 && freq < 100000000 )
		{
			sprintf(buf,"%2.2fMHz",freq / 1000000.0f);
		}
		else
		{
			sprintf(buf,"%3.1fMHz",freq / 1000000.0f);
		}
	}
	else if( freq >= 1000 )
	{
		/* gewei */
		if( freq < 10000 )
		{
			sprintf(buf,"%1.3fKHz",freq / 1000.0f);
		}
		else if( freq >= 10000 && freq < 100000 )
		{
			sprintf(buf,"%2.2fKHz",freq / 1000.0f);
		}
		else
		{
			sprintf(buf,"%3.1fKHz",freq / 1000.0f);
		}	
	}
	else
	{
		/* gewei */
		if( freq < 10 )
		{
			sprintf(buf,"%1.3fHz ",freq );
		}
		else if( freq >= 10 && freq < 100 )
		{
			sprintf(buf,"%2.2fHz ",freq );
		}
		else
		{
			sprintf(buf,"%3.1fHz ",freq );
		}		
	}
	/* get len */
	osc_set_str(buf,8);	
	/* return OK */
	return FS_OK;
}
/* period copy */
static void osc_measure_peroid(unsigned char chn,char * bufd)
{
	/* set data */
	static unsigned int last_sys_time_us1[2];
	static unsigned int last_tim2_cnt_pulse1[2];	
	/* get time */
	unsigned int now_time = hal_sys_time_us();
	/* calbrate freq */
	unsigned int now_cnt = ( chn == 0 ) ? hal_tim2_cnt() : hal_tim4_cnt();
	/* cnt tes */
	float freq = (float)( now_cnt - last_tim2_cnt_pulse1[chn] ) / ((float)( now_time - last_sys_time_us1[chn] ) / 1000000) ;
	/* reset */
	last_tim2_cnt_pulse1[chn] = now_cnt;
	last_sys_time_us1[chn] = now_time;
	/* fds */
	float period = 1.0f / freq ;
	char * un;
	/* check */
	if( (int)(period * 1000.0f))
	{
		period *= 1000.0f;
		un = "ms";
	}
	else if( (int)(period * 1000000.0f))
	{
		period *= 1000000.0f;
		un = "us";
	}
	else if( (int)(period * 1000000000.0f))
	{
		period *= 1000000000.0f;
		un = "ns";
	}
	else
	{
		period *= 1.0f;
		un = "s";
	}
	/* sheck */
	sprintf(bufd,"%3.1f%s",period,un);
	/* get len */
	osc_set_str(bufd,7);
}
/* p-p measure */
static int peek_measure_ch(unsigned char chn , char * buf)
{
	/* osc vol scale */
	const osc_vol_scale_def * ovs;
	/* get */
	int ret = osc_api_peek(chn,&max,&min);
	/* check */
	if( ret == FS_OK )
	{
		/* get vol scale */
		ovs = osc_get_vol_scale(chn);
		/* calbrate ppk */
		float peek_mv = ((float)max - (float)min) / 50.0f * ovs->mv_int;
		/* check */
		if( osc_X10X1_get(chn) == 0 )
		{
			/* X 10 */
			peek_mv *= 10;
		}
		/* show */
		if( peek_mv > 1000 )
		{
			/* change */
			sprintf(buf,"%2.1fV", peek_mv / 1000.0f);				
		}
		else
		{
			sprintf(buf,"%3.1fmV", peek_mv );
		}
		/* get len */
		osc_set_str(buf,7);
		/* return OK */
		return FS_OK;
	}
	else
	{
		sprintf(buf,"0.0fmV");
	}
	/* bad data */
	return FS_ERR;
}
/* p-p measure */
static int max_measure_ch(unsigned char chn , char * buf)
{
	/*dd *//* dfe */
	osc_run_msg_def * runmsg8 = get_run_msg();
	/* osc vol scale */
	const osc_vol_scale_def * ovs;
	/* get */
	int ret = osc_api_peek(chn,&min,&max);
	/* check */
	if( ret == FS_OK )
	{
		/* get vol scale */
		ovs = osc_get_vol_scale(chn);
		/* calbrate ppk */
		float max_mv = (runmsg8->vol_offset_scale[chn] - (float)max) / 50.0f * ovs->mv_int;
		/* check */
		if( osc_X10X1_get(chn) == 0 )
		{
			/* X 10 */
			max_mv *= 10;
		}		
		/* show */
		if( max_mv >= 1000 || max_mv <= -1000 )
		{
			/* change */
			sprintf(buf,"%2.1fV", max_mv / 1000.0f);				
		}
		else
		{
			sprintf(buf,"%3.1fmV", max_mv );
		}
		/* get len */
		osc_set_str(buf,7);
		/* return OK */
		return FS_OK;
	}
	
	else
	{
		sprintf(buf,"0.0fmV");
	}
	/* bad data */
	return FS_ERR;
}
/* p-p measure */
static int min_measure_ch(unsigned char chn , char * buf)
{
	/*dd *//* dfe */
	osc_run_msg_def * runmsg8 = get_run_msg();
	/* osc vol scale */
	const osc_vol_scale_def * ovs;
	/* get */
	int ret = osc_api_peek(chn,&min,&max);
	/* check */
	if( ret == FS_OK )
	{
		/* get vol scale */
		ovs = osc_get_vol_scale(chn);
		/* calbrate ppk */
		float min_mv = (runmsg8->vol_offset_scale[chn] - (float)min) / 50.0f * ovs->mv_int;
		/* check */
		if( osc_X10X1_get(chn) == 0 )
		{
			/* X 10 */
			min_mv *= 10;
		}		
		/* show */
		if( min_mv >= 1000 || min_mv <= -1000 )
		{
			/* change */
			sprintf(buf,"%2.1fV", min_mv / 1000.0f);				
		}
		else
		{
			sprintf(buf,"%3.1fmV", min_mv );
		}
		/* get len */
		osc_set_str(buf,7);
		/* return OK */
		return FS_OK;
	}
	else
	{
		sprintf(buf,"0.0fmV");
	}
	/* bad data */
	return FS_ERR;
}





extern float_t rms_ch[2];
/* rms measure */
static int rms_measure_ch(unsigned char chn , char * buf)
{
	/*dd *//* dfe */
	osc_run_msg_def * runmsg8 = get_run_msg();
	/* osc vol scale */
	const osc_vol_scale_def * ovs;
	/* get vol scale */
	ovs = osc_get_vol_scale(chn);
	/* calbrate ppk if( osc_X10X1_get(chn) == 0 ) */
	float rms_mv;
	/* get */
	if( chn == 0 )
	{
		rms_mv = (rms_ch[0] * 1.953125f) / 50.0f * ovs->mv_int;	
	}
	else
	{
		rms_mv = (rms_ch[1] * 1.953125f) / 50.0f * ovs->mv_int;	
	}
	/* change */
	if( osc_X10X1_get(chn) == 0 )
	{
		rms_mv *= 10.0f;
	}		
	/* set */
	sprintf(buf,"%3.2fV", rms_mv / 1000.0f);				
	/* get len */
	osc_set_str(buf,7);
	/* return OK */
	return FS_OK;
}

/* p-p measure */
static int pcnt_measure_ch(unsigned char chn , char * buf)
{
	unsigned int pcnt = 0;
	if(chn==0)
	{
		pcnt = hal_tim2_cnt();
	}
	else
	{
		pcnt = hal_tim4_cnt();
	}
	sprintf(buf,"%d",pcnt);
	osc_set_str(buf,7);
	
	return FS_OK;
}

/* pulse-width measure */
static int width_measure_ch(unsigned char chn , char * bufd)
{
	extern float rms_buffer_ori0[800];
	extern float rms_buffer_ori1[800];
	/* total samples */
	unsigned int total = 800;
	/* calc mean */
	float mean = 0.0f;
	float * pbuf = ( chn == 0 ) ? rms_buffer_ori0 : rms_buffer_ori1;
	for( unsigned int i = 0 ; i < total ; i ++ )
	{
		mean += pbuf[i];
	}
	mean /= (float)total;
	/* count high samples */
	unsigned int high_cnt = 0;
	for( unsigned int i = 0 ; i < total ; i ++ )
	{
		if( pbuf[i] > mean )
		{
			high_cnt ++;
		}
	}
	/* set data */
	static unsigned int last_sys_time_us_w[2];
	static unsigned int last_tim2_cnt_pulse_w[2];
	/* get time */
	unsigned int now_time = hal_sys_time_us();
	/* calbrate freq */
	unsigned int now_cnt = ( chn == 0 ) ? hal_tim2_cnt() : hal_tim4_cnt();
	/* cnt tes */
	float freq = (float)( now_cnt - last_tim2_cnt_pulse_w[chn] ) / ((float)( now_time - last_sys_time_us_w[chn] ) / 1000000) ;
	/* reset */
	last_tim2_cnt_pulse_w[chn] = now_cnt;
	last_sys_time_us_w[chn] = now_time;
	/* period */
	float period = 1.0f / freq ;
	/* duty */
	float duty = (float)high_cnt / (float)total;
	/* pulse width = duty * period */
	float pw = duty * period ;
	char * un;
	/* check scale */
	if( (int)(pw * 1000.0f) )
	{
		pw *= 1000.0f;
		un = "ms";
	}
	else if( (int)(pw * 1000000.0f) )
	{
		pw *= 1000000.0f;
		un = "us";
	}
	else if( (int)(pw * 1000000000.0f) )
	{
		pw *= 1000000000.0f;
		un = "ns";
	}
	else
	{
		pw *= 1.0f;
		un = "s";
	}
	/* show */
	sprintf(bufd,"%3.1f%s",pw,un);
	/* get len */
	osc_set_str(bufd,7);
	return FS_OK;
}
/* duty measure */
#define DUTY_MAX_EDGES  80
static int duty_measure_ch(unsigned char chn , char * buf)
{
	extern float rms_buffer_ori0[800];
	extern float rms_buffer_ori1[800];
	/* total samples */
	unsigned int total = 800;
	float * pbuf = ( chn == 0 ) ? rms_buffer_ori0 : rms_buffer_ori1;
	/* find max/min for robust midpoint threshold */
	float vmax = pbuf[0];
	float vmin = pbuf[0];
	for( unsigned int i = 1 ; i < total ; i ++ )
	{
		if( pbuf[i] > vmax ) vmax = pbuf[i];
		if( pbuf[i] < vmin ) vmin = pbuf[i];
	}
	/* midpoint as threshold, with hysteresis band */
	float midpoint = (vmax + vmin) * 0.5f;
	float amp = (vmax - vmin) * 0.1f;
	float thr_hi = midpoint + amp;
	float thr_lo = midpoint - amp;
	/* static edge arrays (avoid huge stack allocation) */
	static unsigned int rising_edges[DUTY_MAX_EDGES];
	static unsigned int falling_edges[DUTY_MAX_EDGES];
	unsigned int edge_count = 0;
	unsigned int falling_count = 0;
	unsigned char state = (pbuf[0] > midpoint) ? 1 : 0;
	/* single pass edge detection with hysteresis */
	for( unsigned int i = 1 ; i < total && edge_count < DUTY_MAX_EDGES && falling_count < DUTY_MAX_EDGES ; i ++ )
	{
		if( state == 0 && pbuf[i] > thr_hi )
		{
			state = 1;
			float delta_y = pbuf[i] - pbuf[i-1];
			if( delta_y > 0.1f )
			{
				float delta_x = (midpoint - pbuf[i-1]) / delta_y;
				rising_edges[edge_count] = (unsigned int)((i - 1) + delta_x * 256);
			}
			else
			{
				rising_edges[edge_count] = i * 256;
			}
			edge_count++;
		}
		else if( state == 1 && pbuf[i] < thr_lo )
		{
			state = 0;
			float delta_y = pbuf[i-1] - pbuf[i];
			if( delta_y > 0.1f )
			{
				float delta_x = (pbuf[i-1] - midpoint) / delta_y;
				falling_edges[falling_count] = (unsigned int)((i - 1) + delta_x * 256);
			}
			else
			{
				falling_edges[falling_count] = i * 256;
			}
			falling_count++;
		}
	}
	/* calc raw duty over complete periods */
	float duty_raw = 0.0f;
	if( edge_count > 1 && falling_count > 0 )
	{
		unsigned int total_period_samples = 0;
		unsigned int total_high_samples = 0;
		for( unsigned int p = 0 ; p < edge_count - 1 ; p++ )
		{
			unsigned int start = rising_edges[p];
			unsigned int next_rising = rising_edges[p+1];
			for( unsigned int f = 0 ; f < falling_count ; f++ )
			{
				if( falling_edges[f] > start && falling_edges[f] < next_rising )
				{
					total_period_samples += next_rising - start;
					total_high_samples += falling_edges[f] - start;
					break;
				}
			}
		}
		if( total_period_samples > 0 )
		{
			duty_raw = (float)total_high_samples / (float)total_period_samples * 100.0f;
		}
	}
	else
	{
		/* fallback: midpoint threshold point-count */
		unsigned int high_cnt = 0;
		for( unsigned int i = 0 ; i < total ; i ++ )
		{
			if( pbuf[i] > midpoint ) high_cnt++;
		}
		duty_raw = (float)high_cnt / (float)total * 100.0f;
	}
	/* EMA smoothing filter (alpha=0.3) to stabilize reading */
	static float duty_filtered[2] = { -1.0f, -1.0f };
	if( duty_filtered[chn] < 0.0f )
	{
		duty_filtered[chn] = duty_raw;
	}
	else
	{
		duty_filtered[chn] = duty_filtered[chn] * 0.7f + duty_raw * 0.3f;
	}
	float duty = duty_filtered[chn];
	/* format */
	if( duty < 10.0f )
	{
		sprintf(buf,"%1.2f%%", duty);
	}
	else
	{
		sprintf(buf,"%2.1f%%", duty);
	}
	osc_set_str(buf,7);
	return FS_OK;
}


/* phase measure */
static int phase_measure_ch(unsigned char chn , char * buf)
{
	extern float rms_buffer_ori0[800];
	extern float rms_buffer_ori1[800];
	/* total samples */
	unsigned int total = 800;
	/* calc mean ch1 */
	float mean1 = 0.0f;
	float mean2 = 0.0f;
	for( unsigned int i = 0 ; i < total ; i ++ )
	{
		mean1 += rms_buffer_ori0[i];
		mean2 += rms_buffer_ori1[i];
	}
	mean1 /= (float)total;
	mean2 /= (float)total;
	/* find first positive zero-crossing ch1 */
	int zc1 = -1;
	for( unsigned int i = 1 ; i < total ; i ++ )
	{
		if( rms_buffer_ori0[i-1] <= mean1 && rms_buffer_ori0[i] > mean1 )
		{
			zc1 = i;
			break;
		}
	}
	/* find second positive zero-crossing ch1 */
	int zc1_2 = -1;
	if( zc1 >= 0 )
	{
		for( unsigned int i = zc1 + 1 ; i < total ; i ++ )
		{
			if( rms_buffer_ori0[i-1] <= mean1 && rms_buffer_ori0[i] > mean1 )
			{
				zc1_2 = i;
				break;
			}
		}
	}
	/* find first positive zero-crossing ch2 */
	int zc2 = -1;
	for( unsigned int i = 1 ; i < total ; i ++ )
	{
		if( rms_buffer_ori1[i-1] <= mean2 && rms_buffer_ori1[i] > mean2 )
		{
			zc2 = i;
			break;
		}
	}
	/* find second positive zero-crossing ch2 */
	int zc2_2 = -1;
	if( zc2 >= 0 )
	{
		for( unsigned int i = zc2 + 1 ; i < total ; i ++ )
		{
			if( rms_buffer_ori1[i-1] <= mean2 && rms_buffer_ori1[i] > mean2 )
			{
				zc2_2 = i;
				break;
			}
		}
	}
	/* calc phase */
	if( zc1 >= 0 && zc2 >= 0 )
	{
		if( chn == 0 )
		{
			/* chn0: ch2 relative to ch1 */
			if( zc1_2 > zc1 )
			{
				int period_samps = zc1_2 - zc1;
				int lag = zc2 - zc1;
				while( lag < 0 ) lag += period_samps;
				while( lag >= period_samps ) lag -= period_samps;
				float phase = (float)lag / (float)period_samps * 360.0f;
				sprintf(buf,"%3.2f" , phase);
			}
			else
			{
				sprintf(buf,"0.0" );
			}
		}
		else
		{
			/* chn1: ch1 relative to ch2 */
			if( zc2_2 > zc2 )
			{
				int period_samps = zc2_2 - zc2;
				int lag = zc1 - zc2;
				while( lag < 0 ) lag += period_samps;
				while( lag >= period_samps ) lag -= period_samps;
				float phase = (float)lag / (float)period_samps * 360.0f;
				sprintf(buf,"%3.2f" , phase);
			}
			else
			{
				sprintf(buf,"0.0" );
			}
		}
	}
	else
	{
		sprintf(buf,"0.0" );
	}
	/* get len */
	osc_set_str(buf,7);
	return FS_OK;
}

static int amp_cursor_measure_ch(unsigned char chn, char * buf)
{
	osc_run_msg_def * runmsg = get_run_msg();
	const osc_vol_scale_def * ovs = osc_get_vol_scale(chn);
	draw_area_def * area = get_draw_area_msg();
	float pixel_diff = (float)((signed short)runmsg->amp_cursor_a - (signed short)runmsg->amp_cursor_b);
	if( pixel_diff < 0 ) pixel_diff = -pixel_diff;
	float mv = pixel_diff / (float)area->total_pixel_v * (float)VERTICAL_GRID_NUM * ovs->mv_int;
	if( osc_X10X1_get(chn) == 0 ) mv *= 10.0f;
	if( mv >= 1000.0f )
		sprintf(buf, "%2.1f%s",(mv / 1000.0f), "V");
	else
		sprintf(buf, "%3.0f%s",mv,"mV");
	osc_set_str(buf, 8);
	return FS_OK;
}

static int time_cursor_measure_ch(unsigned char chn, char * buf)
{
	osc_run_msg_def * runmsg = get_run_msg();
	const osc_time_def * tm = osc_time_get();
	draw_area_def * area = get_draw_area_msg();
	float pixel_diff = (float)((signed short)runmsg->time_cursor_a - (signed short)runmsg->time_cursor_b);
	if( pixel_diff < 0 ) pixel_diff = -pixel_diff;
	float time_val = pixel_diff / (float)area->total_pixel_h * tm->osc_time * tm->zoom;
	float time_us;
	switch(tm->osc_unit)
	{
		case OSC_UINT_NS:
			time_us = time_val / 1000.0f;
			break;
		case OSC_UNIT_MS:
			time_us = time_val * 1000.0f;
			break;
		case OSC_UNIT_S:
			time_us = time_val * 1000000.0f;
			break;
		case OSC_UNIT_US:
		default:
			time_us = time_val;
			break;
	}
	if( time_us >= 1000000.0f )
		sprintf(buf, "%2.1f%s",(time_us / 1000000.0f), "S");
	else if( time_us >= 1000.0f )
		sprintf(buf, "%2.1f%s", (time_us / 1000.0f), "ms");
	else if( time_us >= 1.0f )
		sprintf(buf, "%3.0f%s", (time_us), "us");
	else
		sprintf(buf, "%3.0f%s", (time_us * 1000.0f), "ns");
	osc_set_str(buf, 8);
	return FS_OK;
}




/* check and set str len */
static void osc_set_str(char * bdf,unsigned int limit)
{
	/* get len */
	int len = strlen(bdf);
	/* check */
	if( len < limit )
	{
		memset( bdf + len , ' ' , limit - len );
	}
	/* set tail */
	bdf[limit] = 0;	
	/* end of func */
}
/* check */
static void osc_measure_im(widget_def* wd)
{
	/* get pos */
	unsigned short pox;
	unsigned short poy;
	/* buffer */
	char buffer_m[16];
	/* check item */
	switch(wd->msg.my)
	{
		/* freq */
		case 0:
			freq_measure_ch(wd->msg.mx,buffer_m);
			pox = wd->msg.x + 68;
			poy = wd->msg.y + 3;
			/* set str */
			osc_set_str(buffer_m,8);
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);	
			/* break */
			break;
		case 1:
			/* peroid */
			osc_measure_peroid(wd->msg.mx,buffer_m);
			pox = wd->msg.x + 68;
			poy = wd->msg.y + 3;
			/* period */
			osc_measure_peroid(wd->msg.mx,buffer_m);
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);			
			break;
		case 2:
			/* PPK */
			peek_measure_ch(wd->msg.mx,buffer_m);
			/* position */
			pox = wd->msg.x + 79;
			poy = wd->msg.y + 4;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);			
			/* show */
			break;
		case 3:
			/* PPK */
			max_measure_ch(wd->msg.mx,buffer_m);
			/* position */
			pox = wd->msg.x + 79;
			poy = wd->msg.y + 4;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);			
			/* show */			
			break;
		case 4:
			/* PPK */
			min_measure_ch(wd->msg.mx,buffer_m);
			/* position */
			pox = wd->msg.x + 79;
			poy = wd->msg.y + 4;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);			
			/* show */
			break;
		case 5:
			/* rms */
			rms_measure_ch(wd->msg.mx,buffer_m);
			/* position */
			pox = wd->msg.x + 79;
			poy = wd->msg.y + 4;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);		
			break;
		case 6:
			/*duty*/
			duty_measure_ch(wd->msg.mx,buffer_m);
			/* position */
			pox = wd->msg.x + 79;
			poy = wd->msg.y + 4;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);	
			break;
		case 7:
			/*Pulse-width*/
			width_measure_ch(wd->msg.mx,buffer_m);
			/* position */
			pox = wd->msg.x + 68;
			poy = wd->msg.y + 3;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);	
			break;
		case 8:
			/* p-cnt */
			pcnt_measure_ch(wd->msg.mx,buffer_m);
			pox = wd->msg.x + 79;
			poy = wd->msg.y + 4;
			/* show */
			osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);	
			/* break */
			break;
		case 9:
		phase_measure_ch(wd->msg.mx,buffer_m);
		pox = wd->msg.x + 68;
		poy = wd->msg.y + 3;
		osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);
		break;
	case 10:
		amp_cursor_measure_ch(wd->msg.mx,buffer_m);
		pox = wd->msg.x + 68;
		poy = wd->msg.y + 3;
		osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);
		break;
	case 11:
		time_cursor_measure_ch(wd->msg.mx,buffer_m);
		pox = wd->msg.x + 68;
		poy = wd->msg.y + 3;
		osc_l1_string(wd->dev,pox,poy,buffer_m,81,4,1);
		break;
	case 12:
		break;
	default:
			break;
	}
}
/* measure thread */
static int osc_measure_thread_init(void)
{
	osc_measure_thread();
	return FS_OK;
}
static void osc_measure_thread(void)
{
	/* check run mode */
	if( osc_run_mode() == RUN_STOP_MODE )	
	{
		return;
	}
	/* start measure thread */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* check enable */
		if( measure_group[i].msg.upd == 0xE0 )
		{
			/* measure */
			osc_measure_im(&measure_group[i]);
		}
	}
	
}