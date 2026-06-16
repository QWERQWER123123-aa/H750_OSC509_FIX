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

#ifndef __OSC_CFG_H__
#define __OSC_CFG_H__
/* Includes ------------------------------------------------------------------*/
#define OSC_UNIT_US   (0)
#define OSC_UNIT_MS   (1)
#define OSC_UNIT_S    (2)
#define OSC_UINT_NS   (3)

#define OSC_BASE_CLOCK  (200)

/* Private includes ----------------------------------------------------------*/
/* MACROS */
#define OSC_TIME_ROT           (0)
#define OSC_VOL_SCALE          (1)
#define OSC_VOL_OFFSET_SCALE   (2)
#define OSC_TRIG_SCALE         (3)

/* osc_ time config */
typedef struct{
	/* show title */
  char * str;
	/* scan time */
  float osc_time;
	/* unit */
	unsigned int osc_unit;
	/* ins */
	unsigned int osc_trig_delay;	
	/* ins */
	unsigned int zoom;
	/* zm */
	unsigned int osc_zoom_factor;
	/* end if */
}osc_time_def;
/* voltage scale */
typedef struct
{
	/* show * str */
	char * str;
	/* X10 */
	char * strX10;	
	/* gain dac */
	unsigned short gain_dac[2];
	/* gain_offset ch1*/
	signed short gain_offset_ch[2];
	/* gain_sel */
	unsigned short gain_sel;
	/* mv */
	unsigned short mv_int;
	/* sel */
	unsigned short gain_ctrl_index[2];
  /* end of */	
}osc_vol_scale_def;

/* set scan time */
void osc_scan_thread(unsigned char);
void osc_offset_scale_thread(unsigned char chn);
void osc_trig_scale_thread(unsigned char chn);
void osc_vol_scale_thread(unsigned char chn,unsigned char inc);
static void osc_cfg_task(void);
static void osc_vertical_offset_tips(unsigned char chn,signed short mv);
const osc_vol_scale_def * osc_get_vol_scale(unsigned char chn);
osc_vol_scale_def * osc_vol_scale_s(unsigned char index);
void osc_trig_flush(void);
const osc_time_def * osc_time_get(void);
void osc_vol_scale_set_dir(unsigned char chn,unsigned char ind);
const osc_time_def * osc_scan_time(unsigned int index);
void osc_trig_lines_self(unsigned char chn);
int osc_time_scan_leng(void);
int osc_vol_scale_leng(void);
#endif