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
#include "string.h"
#include "hal_iic.h"
#include "osc_cfg.h"
/* dac */
//static signed short osc_dac_buffer[4];
//static signed short osc_chn_offset[2];
/* temp data */
//static signed char tmp_buffer_ch1[1000];
//static signed char tmp_buffer_ch2[1000];
/* max and min */
static unsigned short max_ch[2],min_ch[2];
static unsigned char peek_flag_up[2];
/* fft trig pos */
extern void osc_copy_from_fifo(unsigned char * data , unsigned char chn,unsigned int start_pos,unsigned int len);
extern unsigned char osc_copy_from_fifo_onebyone(unsigned char chn,unsigned int stcnt);
extern void osc_set_copy_start(unsigned int strs);
/* Private includes ----------------------------------------------------------*/
void osc_stop_adc_clock(void)
{
	/* select internal clock and disable the clock */
	hal_pwm_stop();
	/* end */
}
/* start clock */
void osc_start_adc_clock(unsigned char internal)
{
	/* internal clock or not */
	if( internal == 0 )
	{
		/* restart pwm */
		hal_pwm_start();
		/* end */
	}
	else
	{
		/* restart pwm */
		hal_pwm_stop();		
	}
}
/* fifo reset */
void osc_fifo_reset(void)
{
	/* set w to high */
	osc_fifo_clock(0);
	/* rs to low */
	/* set w to low */
	osc_fifo_clock(1);	
}
/* create fft input */
void osc_fft_input(float * dsp_fft_infut,unsigned char chn,unsigned int len)
{
	signed char td;
	unsigned int strcnt = 350;
	const osc_time_def * tm = osc_time_get();
	/* get data */
	for( int i = 0 ; i < len ; i ++ )
	{
		td = (signed char)osc_copy_from_fifo_onebyone(chn,strcnt);
		/* inc */
		strcnt += tm->zoom;
		/* sgge */
		dsp_fft_infut[i] = (float)td;
	}
}
/* create fft lcd buffer */
void osc_fft_to_lcd(float * dsp_fft_input,unsigned short *lcd_buffer,float max_v , unsigned int ind_max)
{
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();		
	/* get max std */
	float base_kd =(float)area->total_pixel_v /  max_v ;	
	/* create the data */
  for( int i = 0 ; i < area->total_pixel_h ; i ++ )
	{
		/* set */
		float f_t = dsp_fft_input[i+1] * base_kd * 0.8f;
		/* set */
		lcd_buffer[i] = area->total_pixel_v - f_t; 
		/* limit */
		if( lcd_buffer[i] > area->total_pixel_v )
		{
			lcd_buffer[i] = area->total_pixel_v;
		}
	}
}
/* wave data select */
int osc_trig_read(unsigned short * ch1_m,unsigned short * ch2_m,int trig_type,int trig_source,int tflag,unsigned int ins,unsigned int deep)
{
	/* extern */
	extern unsigned short dma_trig_ndtr;
	/* get time */
	const osc_time_def * tm = osc_time_get();
	/* read */
	unsigned int dma_ted = dma_trig_ndtr * 4;
	/* fifo */
	unsigned int fifo_deep_ds0 = FIFO_DBS_0 / 2 * 4;
	/* limit */
	if( dma_ted > fifo_deep_ds0 - tm->zoom * 350 )
	{
		dma_ted = fifo_deep_ds0 - tm->zoom * 350 + tm->osc_trig_delay;
	}
	/* head */
	if( dma_ted < tm->zoom * 350 + tm->osc_trig_delay )
	{
		dma_ted = tm->zoom * 700 + tm->osc_trig_delay;
	}
	/* copy data */
	osc_create_analog_data(ch1_m,ch2_m,dma_ted - tm->zoom * 350 - tm->osc_trig_delay);
	/* end */
	return FS_OK;
}
/* tmp bsd */
volatile float ch1_f,ch2_f; 
extern unsigned short * src_fifo0_0;
float rms_buffer_ori0[800];
float rms_buffer_ori1[800];
/* create analog data to display dev */
static void osc_create_analog_data(unsigned short * ch1_m,unsigned short * ch2_m ,unsigned int ins)
{
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	const osc_time_def * tm = osc_time_get();
	/* pos */
	unsigned char ch1_pos;
	unsigned char ch2_pos; 
	unsigned int strcnt = 0;
	/* SET */
	max_ch[0] = 0;
	max_ch[1] = 0;
	min_ch[0] = 0xffff;
	min_ch[1] = 0xffff;
	/* cread */
	osc_set_copy_start(ins);
	/* create the data */
  for( int i = 0 ; i < area->total_pixel_h ; i ++ )
	{
		/* read one data */
		ch1_pos = osc_copy_from_fifo_onebyone(0,strcnt);
		ch2_pos = osc_copy_from_fifo_onebyone(1,strcnt);
		/* check */
		ch1_f = (float)((signed char)(ch1_pos));
		ch2_f = (float)((signed char)(ch2_pos));
		/* set */
		rms_buffer_ori0[i] = ch1_f;
		rms_buffer_ori1[i] = ch2_f;
#if OSC_STM32H750			
		/* create the ddtd */
		ch1_m[i] = ( -400.0f / 256.0f ) * 1.25f * ch1_f + area->total_pixel_v / 2;
		ch2_m[i] = ( -400.0f / 256.0f ) * 1.25f * ch2_f + area->total_pixel_v / 2;
#else
		/* create the ddtd */
		ch1_m[i] = ( -400.0f / 256.0f ) * 1.2f * ch1_f + area->total_pixel_v / 2;
		ch2_m[i] = ( -400.0f / 256.0f ) * 1.2f * ch2_f + area->total_pixel_v / 2;		
#endif
		/* set */
		strcnt += tm->zoom;
		/* check */
		if( ch1_m[i] > max_ch[0] )
		{
			max_ch[0] = ch1_m[i];
		}
		/* max */
		if( ch2_m[i] > max_ch[1] )
		{
			max_ch[1] = ch2_m[i];
		}
		/* check min */
		if( ch1_m[i] < min_ch[0] )
		{
			min_ch[0] = ch1_m[i];
		}
		/* check min */
		if( ch2_m[i] < min_ch[1] )
		{
			min_ch[1] = ch2_m[i];
		}		
		/* flag */
		peek_flag_up[0] = 1;
		peek_flag_up[1] = 1;
	}
}
/* create math data from CH1 and CH2 raw values */
void osc_create_math_data(unsigned short * math_m, unsigned char math_op)
{
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	float math_val;
	float math_val_max;
	/* max display coordinate */
	math_val_max = (float)area->total_pixel_v;
	/* create the data */
	for( int i = 0 ; i < area->total_pixel_h ; i ++ )
	{
		float ch1_f = rms_buffer_ori0[i];
		float ch2_f = rms_buffer_ori1[i];
		/* math operation */
		switch(math_op)
		{
			case 0: /* A + B */
				math_val = ch1_f + ch2_f;
				break;
			case 1: /* A - B */
				math_val = ch1_f - ch2_f;
				break;
			case 2: /* A * B */
				math_val = (ch1_f * ch2_f) / 128.0f;
				break;
			case 3: /* A / B */
				if( ch2_f > 0.1f || ch2_f < -0.1f )
					math_val = (ch1_f / ch2_f) * 128.0f;
				else
					math_val = 0.0f;
				break;
			default:
				math_val = 0.0f;
				break;
		}
		/* convert to display coordinates */
#if OSC_STM32H750
		math_val = ( -400.0f / 256.0f ) * 1.25f * math_val + area->total_pixel_v / 2;
#else
		math_val = ( -400.0f / 256.0f ) * 1.2f * math_val + area->total_pixel_v / 2;
#endif
		/* clamp to valid display range */
		if( math_val < 0.0f )
			math_val = 0.0f;
		if( math_val > math_val_max )
			math_val = math_val_max;
		/* store */
		math_m[i] = (unsigned short)math_val;
	}
}
/* osc api get max and min */
int osc_api_peek(unsigned char chn , unsigned short * max,unsigned short * min)
{
	if( chn >= 2 )
	{
		return FS_ERR;
	}
	/* flags */
	if( peek_flag_up[chn] == 1 )
	{
		/* set data */
		*max = max_ch[chn];
		*min = min_ch[chn];
		/* return OK */
		return FS_OK;
	}
	/* return ERROR */
	return FS_ERR;
}
/* osc output dac */
void osc_voltage_output(unsigned short a,unsigned short b,unsigned short c,unsigned short d)
{
//	/* save buffer */
//	osc_dac_buffer[0] = a;
//	osc_dac_buffer[1] = b;
//	osc_dac_buffer[2] = c;
//	osc_dac_buffer[3] = d;
//	/* update dac */
//  dac_update(a,b,c,d);
}
///* dac event */
//static void osc_dac_update_buf(signed short * buf,signed short * offset)
//{
//	dac_update(buf[0],buf[1],buf[2] + offset[0],buf[3] + offset[1]);
//}
/* set base dac */
void osc_vol_dac(unsigned char chn,unsigned short gain_ctrl)
{
	osc_gain_ctrl(chn,gain_ctrl);
}
/* void osc_dac_offset */
void osc_dac_offset(unsigned char chn,signed short offset_mv)
{
	osc_set_offset_dac(chn,offset_mv);
}
/* fifo clock enable */
void osc_fifo_clock(unsigned short sta)
{
//	hal_write_gpio(FIFO_DIO_TR,sta);
}
/* coune */
int osc_read_rot_idle(unsigned index)
{
	unsigned short ret = 0;
//	/* index */
//	switch(index)
//	{
//		case 0:
//			ret = hal_read_gpio(DIO_ROT_TIME_UP) || hal_read_gpio(DIO_ROT_TIME_DM);
//		break;
//		case 1:
//			ret = hal_read_gpio(DIO_ROT_VOL_UP) || hal_read_gpio(DIO_ROT_VOL_DM);
//		break;
//		case 2:
//			ret = hal_read_gpio(DIO_ROT_TRIG_UP) || hal_read_gpio(DIO_ROT_TRIG_DM);
//		break;		
//		case 3:
//			ret = hal_read_gpio(DIO_ROT_HORI_UP) || hal_read_gpio(DIO_ROT_HORI_DM);
//		break;		
//		default:
//			ret = 1;
//		break;
//	}
	/* get osc data */
	return ret;
}
/* gain ctrl */
void osc_gain_sel(unsigned char chn , unsigned char sta)
{
	/* gain ctrl */
	if( chn == 0 )
	{
		hal_write_gpio(DIO_RLY0,sta);
	}
	else
	{
		hal_write_gpio(DIO_RLY1,sta);
	}
}
/* osc set key addr */
void osc_set_key_addr(unsigned char addr_cnt)
{
//	/* addr A */
//	hal_write_gpio(DIO_CD4051_ADDR_A,addr_cnt & 0x01);
//	/* ADDR B */
//	hal_write_gpio(DIO_CD4051_ADDR_B,addr_cnt & 0x02);
//	/* ADDR C */
//	hal_write_gpio(DIO_CD4051_ADDR_C,addr_cnt & 0x04);
	/* endif */
}
/* osc read com2 sta */
int osc_read_com2(void)
{
	return 0;//hal_read_gpio(DIO_CD4051_COM2) ? 1 : 0;
}
/* osc read com1 sta */
int osc_read_com1(void)
{
	return 0;//hal_read_gpio(DIO_CD4051_COM1) ? 1 : 0;
}
/* void dcac coupling */
void osc_coupling_setting(unsigned char chn,unsigned char dcac)
{
	osc_dcac_gpio_set(chn,dcac);
}
/* pwr */
void osc_power_en(unsigned char mode)
{
//	if( mode == 0 )
//	{
//		hal_write_gpio(DIO_PWR_CTRL,0); // pwr off
//	}
//	else
//	{
//		hal_write_gpio(DIO_PWR_CTRL,1); // pwr on
//	}
}
/* pwr */
void osc_usbs_en(unsigned char mode)
{
//	if( mode == 0 )
//	{
//		hal_write_gpio(DIO_USB_S_ENABLE,0); // pwr off
//	}
//	else
//	{
//		hal_write_gpio(DIO_USB_S_ENABLE,1); // pwr on
//	}
}
/* pwr */
void osc_backlight_en(unsigned short mode)
{
//	if( mode == 0 )
//	{
//		hal_write_gpio(DIO_BACK_LIGHT_EN,0); // light off
//	}
//	else
//	{
//		hal_write_gpio(DIO_BACK_LIGHT_EN,1); // light on
//	}
}