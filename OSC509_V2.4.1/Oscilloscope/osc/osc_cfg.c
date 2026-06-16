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
#include "hal_tim.h"
#include "osc_cfg.h"
#include "osc.h"
#include "hal_exit.h"
#include "osc_ui.h"
#include "osc_api.h"
#include "hal_dac.h"
#include "stdio.h"
#include "osc_menu.h"
#include "osc_win.h"
/* out dac part */
static signed short trig_dac_part_rot = 0;
/* hold time */
static unsigned short trig_lines_hold_time_s = 0;
/* create cfg task gui detecter task run as 100 ms */
FOS_TSK_REGISTER(osc_cfg_task,PRIORITY_4,1000);
/* Private includes ----------------------------------------------------------*/
const osc_time_def osc_tim[] = 
{
#if 1	
	{
		.str = "50ns ",
		.osc_time = 50 , /*  base on 750ns */
		.osc_unit = OSC_UINT_NS,
		.osc_trig_delay = 120,
		.zoom = 1,
		.osc_zoom_factor = 10,
	},
	{
		.str = "100ns",
		.osc_time = 100 , /*  base on 750ns */
		.osc_unit = OSC_UINT_NS,
		.osc_trig_delay = 120,
		.zoom = 1,
		.osc_zoom_factor = 5,
	},	
	{
		.str = "250ns",
		.osc_time = 250 , /*  base on 750ns */
		.osc_unit = OSC_UINT_NS,
		.osc_trig_delay = 120,
		.zoom = 1,	
		.osc_zoom_factor = 2,
	},
#endif
	{
		.str = "500ns",
		.osc_time = 500 , /*  base on 750ns */
		.osc_unit = OSC_UINT_NS,
		.osc_trig_delay = 120,
		.zoom = 1,
		.osc_zoom_factor = 1,
	},
#if 1
	{
		.str = "750ns",
		.osc_time = 750 , /* 750 ns */
		.osc_unit = OSC_UINT_NS,
		.osc_trig_delay = 120,
		.zoom = 1,
		.osc_zoom_factor = 1,
	},
#endif
	{
		.str = "1.0us ",
		.osc_time = 1.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 55,
		.zoom = 1,
		.osc_zoom_factor = 1,
	},
	{
		.str = "2.5us ",
		.osc_time = 2.5f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 1,
		.osc_zoom_factor = 1,
	},
	{
		.str = "5us   ",
		.osc_time = 2.5f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 2,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "10us  ",
		.osc_time = 2.5f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 4,
		.osc_zoom_factor = 1,
	},
	{
		.str = "25us  ",
		.osc_time = 2.5f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 10,
		.osc_zoom_factor = 1,
	},
	{
		.str = "50us  ",
		.osc_time = 2.5f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 20,
		.osc_zoom_factor = 1,
	},
	{
		.str = "100us ",
		.osc_time = 2.5f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 40,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "200us ",
		.osc_time = 10.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 20,
		.osc_zoom_factor = 1,
	},
	{
		.str = "500us ",
		.osc_time = 10.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 50,
		.osc_zoom_factor = 1,
	},	
#if 1
	{
		.str = "1ms   ",
		.osc_time = 25.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 40,
		.osc_zoom_factor = 1,
	},
	{
		.str = "2.5ms ",
		.osc_time = 50.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 50,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "5ms   ",
		.osc_time = 100.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 50,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "10ms  ",
		.osc_time = 200.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 50,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "25ms  ",
		.osc_time = 500.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 50,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "50ms  ",
		.osc_time = 1000.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 50,
		.osc_zoom_factor = 1,
	},	
#endif	
	{
		.str = "100ms ",
		.osc_time = 1000.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 100,
		.osc_zoom_factor = 1,
	},		
	{
		.str = "200ms ",
		.osc_time = 1000.0f , /* 2.5us */
		.osc_unit = OSC_UNIT_US,
		.osc_trig_delay = 20,
		.zoom = 200,
		.osc_zoom_factor = 1,
	},
#if 0	
	{
		.str = "500ms ",
		.osc_time = 500 , /* 500ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_trig_delay = 0,
	},
  {
		.str = "1.0s   ",
		.osc_time = 1 , /* 1ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
		.osc_trig_delay = 0,
	},
	{
		.str = "2.5s   ",
		.osc_time = 2.5 , /* 2.5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
		.osc_trig_delay = 0,
	},	
	{
		.str = "5s    ",
		.osc_time = 5 , /* 5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
		.osc_trig_delay = 0,
	},	
#endif
#if 0	
	{
		.str = "10s  ",
		.osc_time = 10 , /* 10ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "25s ",
		.osc_time = 25 , /* 25ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "50s  ",
		.osc_time = 50 , /* 50ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},		
	#endif
};
/* voltage gain */
osc_vol_scale_def osc_vol_scale_chn[] = 
{
	/* 1 */
	{
		.str = "10mV",
		.strX10 = "100mV",
		.mv_int = 10,
		.gain_sel = 1,
		.gain_ctrl_index[0] = 4,
		.gain_ctrl_index[1] = 5,
	},
	/* 2 */
	{
		.str = "20mV",
		.strX10 = "200mV",
		.mv_int = 20,
		.gain_sel = 1,
		.gain_ctrl_index[0] = 6,
		.gain_ctrl_index[1] = 7,
	},
	/* 4 */
	{
		.str = "50mV",
		.strX10 = "500mV",
		.mv_int = 50,
		.gain_sel = 1,
		.gain_ctrl_index[0] = 1,
		.gain_ctrl_index[1] = 0,
	},
	/* 5 */
	{
		.str = "100mV",
		.strX10 = "1V",
		.mv_int = 100,
		.gain_sel = 1,
		.gain_ctrl_index[0] = 3,
		.gain_ctrl_index[1] = 1,
	},
	/* 6 */
	{
		.str = "200mV",
		.strX10 = "2V",
		.mv_int = 200,
		.gain_sel = 1,
		.gain_ctrl_index[0] = 0,
		.gain_ctrl_index[1] = 2,
	},
/*---------------------------------------------------------------*/
	/* 1 */
	{
		.str = "500mV",
		.strX10 = "5V",
		.mv_int = 500,
		.gain_sel = 0,
		.gain_ctrl_index[0] = 4,
		.gain_ctrl_index[1] = 5,
	},
	/* 2 */
	{
		.str = "1V",
		.strX10 = "10V",
		.mv_int = 1000,
		.gain_sel = 0,
		.gain_ctrl_index[0] = 6,
		.gain_ctrl_index[1] = 7,
	},
	/* 3 */
	{
		.str = "2V",
		.strX10 = "20V",
		.mv_int = 2000,
		.gain_sel = 0,
		.gain_ctrl_index[0] = 2,
		.gain_ctrl_index[1] = 3,
	},
	/* 4 */
	{
		.str = "5V",
		.strX10 = "50V",
		.mv_int = 5000,
		.gain_sel = 0,
		.gain_ctrl_index[0] = 3,
		.gain_ctrl_index[1] = 1,
	},
	/* 5 */
	{
		.str = "10V",
		.strX10 = "100V",
		.mv_int = 10000,
		.gain_sel = 0,
		.gain_ctrl_index[0] = 0,
		.gain_ctrl_index[1] = 2,
	},	
};
/* */
static void osc_cfg_task(void)
{
	/* create */
	if( trig_lines_hold_time_s == 0 )
	{
		/* hide two lines */
		osc_ui_trig_lines_show(0,0);
		osc_ui_trig_lines_show(1,0);
		/* end */
	}
	else
	{
		trig_lines_hold_time_s --;
	}
}
/* set scan clock */
const osc_time_def * osc_scan_time(unsigned int index)
{
	/* over */
	if( index >= sizeof(osc_tim) / sizeof(osc_tim[0]) )
	{
		/* return */
		return 0;
	}
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* psc */
	unsigned int psc = 0;
	/* get the */
	if( osc_tim[index].osc_unit == OSC_UNIT_US )
	{
		/* calbrate the psc num */
		psc = (unsigned int)((float)OSC_BASE_CLOCK * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UNIT_MS )
	{
		psc = (unsigned int)((float)OSC_BASE_CLOCK * 1000 * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UINT_NS )
	{
		/* return */
		psc	= 1;
	}
	else if( osc_tim[index].osc_unit == OSC_UNIT_S )
	{
		psc = (unsigned int)((float)OSC_BASE_CLOCK * 1000000 * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else
	{
		/* unit = s , can not supply now */
		return 0;
	}
	/* set psc */
	hal_tim_psc(psc);
	/* return OK */
	return &osc_tim[index];
}
/* get scan_time leng */
int osc_time_scan_leng(void)
{
	return sizeof(osc_tim) / sizeof(osc_tim[0]);
}
/* get */
int osc_vol_scale_leng(void)
{
	return sizeof(osc_vol_scale_chn) / sizeof(osc_vol_scale_chn[0]);
} 
/* get */
const osc_vol_scale_def * osc_get_vol_scale(unsigned char chn)
{
	/* get run msg for osc */
	osc_run_msg_def * runmsg = get_run_msg();
	/* index */
	unsigned char index = runmsg->vol_scale_ch[chn];
	/* return OK */
	return &osc_vol_scale_chn[index];
}
/* get osc vol scale */
osc_vol_scale_def * osc_vol_scale_s(unsigned char index)
{
	return &osc_vol_scale_chn[index];
}
/* get scanf now */
const osc_time_def * osc_time_get(void)
{
	/* get run msg for osc */
	osc_run_msg_def * runmsg = get_run_msg();	
	/* scan time */
	unsigned short osc_t = runmsg->time_scale;
	/* get len */
	int osc_ts_leng = osc_time_scan_leng();
	/* check */
	if( osc_t >= osc_ts_leng )
	{
		osc_t =  osc_ts_leng - 1;
	}
	/* return */
	const osc_time_def * osc_time_sw = &osc_tim[osc_t];
	/* set time text */
	return osc_time_sw;
	/* update */
}
/* osc_cfg_thread */
void osc_scan_thread(unsigned char inc)
{
	static unsigned char ste = 0;	
	osc_run_msg_def * runmsg = get_run_msg();
	static unsigned char precise_cnt = 0;
	if( osc_param_sys_get(runmsg->sys_menu_set, 18) )
	{
		precise_cnt ++;
		if( precise_cnt & 1 ) return;
	}
	else
	{
		precise_cnt = 0;
	}
	/* ceck */	
	signed short osc_rot = runmsg->time_scale;
	/* inc */
	osc_rot = ( inc == 0 ) ? ( osc_rot - 1 ) : ( osc_rot + 1 );
	/* get leng */
	int osc_ts_leng = osc_time_scan_leng();
	/* check */
	if( osc_rot >= osc_ts_leng )
	{
		/* over the max */
		osc_rot = osc_ts_leng - 1;
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("扫描时间已经到达最大值");
		}
	}
	else if( osc_rot < 0 )
	{
		/* min */
		osc_rot = 0;
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("扫描时间已经到达最小值");
		}
	}
	else
	{
		if( osc_rot != 0 && osc_rot != ( osc_ts_leng - 1) )
		{
			/* once flag */
			if( ste == 0 )
			{
				ste = 1;
				osc_ui_tips_show(0);//hide
			}
		}
	}
	/* set time text */
	osc_ui_time_str(osc_tim[osc_rot].str);	
	/* scan time */
	const osc_time_def * osc_time_sw = osc_scan_time(osc_rot);
	/* update */
	runmsg->time_scale = osc_rot;
	/* return */
}
/* voltage thread */
void osc_offset_scale_thread(unsigned char chn)
{
	/* run msg */
	osc_run_msg_def * runmsg = get_run_msg();
  /* void offset thread */
	signed short vol_offset_scale = runmsg->vol_offset_scale[chn];
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* little */
	unsigned short max_scale = area->total_pixel_v;
	/* get data */
	if( vol_offset_scale > max_scale )
	{
		/* set max offset */
		vol_offset_scale = max_scale;
		/* set vol_offset_scale */
		runmsg->vol_offset_scale[chn] = vol_offset_scale;
		/*----------------------*/
	}
	else if( vol_offset_scale < 6 )
	{
		/* set max offset */
		vol_offset_scale = 6;
		/* set vol_offset_scale */
		runmsg->vol_offset_scale[chn] = vol_offset_scale;
		/*----------------------*/
	}
	else
	{
		/* to do nothing */
	}
	/* change */
	unsigned short new_pos = vol_offset_scale + area->start_pos_y - 6 ;
	/* channel 1 */
	osc_ui_move_offset_arrow(chn,new_pos);
	/* calbrate the offset voltage */
	short out_dac_mv = (float)( vol_offset_scale - max_scale / 2 ) * 500.0f / (float)(max_scale / 2);
	/* calbrate the trig_dac_part_offset */
	signed short vol_t = ( osc_trig_source() == 0 ) ? runmsg->vol_offset_scale[0] : runmsg->vol_offset_scale[1];
	/* out dac for test */
	osc_dac_offset(chn,out_dac_mv);
	/* update dac trig */
	osc_set_dac(trig_dac_part_rot);
	/* show */
}
///* show vertical vol */
//static void osc_vertical_offset_tips(unsigned char chn,signed short mv)
//{
//	/* test */
//	static char buf[32];
//	/* get rotation data */
//	signed short osc_vs = osc_rot_sta(OSC_VOL_SCALE);
//	/* transfer to vol */
//	float fmv = (float)mv / (-128.0f) * (float)osc_vol_scale_chn[osc_vs].mv_int;
//	/* change unit */
//	if( fmv >= 1000 || fmv <= -1000 )
//	{
//		/* create data */
//		sprintf(buf,"通道%d 垂直位置 % 4.2f%s",chn+1,fmv / 1000,"V     ");
//	}
//	else
//	{
//		/* create data */
//		sprintf(buf,"通道%d 垂直位置 % 4.2f%s",chn+1,fmv,"mV    ");
//	}
//	/* show */
//	osc_ui_tips_str_dir(buf);	
//}
/* voltage thread */
void osc_trig_scale_thread(unsigned char chn)
{
	/* run msg */
	osc_run_msg_def * runmsg = get_run_msg();	
  /* void offset thread */
	signed short vol_trig_scale = runmsg->trig_vol_level_ch[chn];
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* little */
	unsigned short max_scale = area->total_pixel_v;
	/* get data */
	if( vol_trig_scale > max_scale )
	{
		/* set max offset */
		vol_trig_scale = max_scale;
		/* set vol_trig_scale */
		runmsg->trig_vol_level_ch[chn] = vol_trig_scale;
	}
	else if( vol_trig_scale < 1 )
	{
		/* set max offset */
		vol_trig_scale = 1;
		/* set vol_trig_scale */
		runmsg->trig_vol_level_ch[chn] = vol_trig_scale;
	}
	else
	{
		/* to do nothing */
	}
	/* pos */
	unsigned short new_pos = vol_trig_scale  + area->start_pos_y - 6 ;
	/* channel 1 */
	osc_ui_move_trig_arrow(chn,new_pos );
	/* show lines */
	osc_ui_trig_lines_show(chn,1);
	/* show hide */
	if( chn == 0 )
	{
		osc_ui_trig_lines_show(1,0);
	}
	else
	{
		osc_ui_trig_lines_show(0,0);
	}
	/* set lines hold time */
	trig_lines_hold_time_s = 3;
	/* mode trig lines */
	osc_ui_move_trig_lines(chn,new_pos + 6);
	/* calbrate the offset voltage */
	signed  short out_dac = (float)( max_scale / 2 - vol_trig_scale ) * 500.0f / (float)(max_scale / 2);
	/* calbrate trig_dac_part_rot */
	trig_dac_part_rot = out_dac;
	/* calbrate the offset voltage */
	osc_set_dac(trig_dac_part_rot);
	/* out dac for test */
}
/* vol scale thread */
void osc_vol_scale_thread(unsigned char chn,unsigned char inc)
{
	/* defines */
	static unsigned char ste = 0;	
	/* get rotation data */
	if( chn >=2 )
	{
		return;
	}
	/* run msg */
	osc_run_msg_def * runmsg = get_run_msg();
	if( osc_param_sys_get(runmsg->sys_menu_set, 18) )
	{
		signed short offset = runmsg->vol_offset_scale[chn];
		offset = (inc == 0) ? (offset - 1) : (offset + 1);
		if( offset < 0 ) offset = 0;
		if( offset > 399 ) offset = 399;
		runmsg->vol_offset_scale[chn] = offset;
		osc_offset_scale_thread(chn);
		return;
	}
  /* check */	
	signed short osc_vs_t = runmsg->vol_scale_ch[chn];
	/* get leng */
	int osc_ts_leng = osc_vol_scale_leng();
	/* check */
	osc_vs_t = ( inc == 0 ) ? ( -- osc_vs_t ) : ( ++ osc_vs_t );
	/* check */
	if( osc_vs_t >= osc_ts_leng )
	{
		/* over the max */
		osc_vs_t = osc_ts_leng - 1;
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("垂直电压已到达最大值");
		}
	}
	else if( osc_vs_t < 0 )
	{
		/* min */
		osc_vs_t = 0;
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("垂直电压已到达最小值");
		}
	}
	else
	{
		if( osc_vs_t != 0 && osc_vs_t != ( osc_ts_leng - 1) )
		{
			/* once flag */
			if( ste == 0 )
			{
				ste = 1;
				osc_ui_tips_show(0);//hide
			}
		}
	}
	/* reware */
	runmsg->vol_scale_ch[chn] = osc_vs_t;
	/* set string */
	if( osc_X10X1_get(chn) == 0 )
	{
		osc_ui_vol_scale(chn,osc_vol_scale_chn[osc_vs_t].strX10);
	}
	else
	{
		osc_ui_vol_scale(chn,osc_vol_scale_chn[osc_vs_t].str);
	}
	/* set gain ctrl */
	osc_gain_sel(chn,osc_vol_scale_chn[osc_vs_t].gain_sel);
	/* set dac */
	osc_vol_dac(chn,osc_vol_scale_chn[osc_vs_t].gain_ctrl_index[chn]);
	/* end */
}
/* set vol */
void osc_vol_scale_set_dir(unsigned char chn,unsigned char ind)
{
	/* check */
	if( chn >= 2 )
	{
		return;
	}
	/* check */
	if( ind >= osc_vol_scale_leng() )
	{
		return;
	}
	/* set string */
	if( osc_X10X1_get(chn) == 0 )
	{
		osc_ui_vol_scale(chn,osc_vol_scale_chn[ind].strX10);
	}
	else
	{
		osc_ui_vol_scale(chn,osc_vol_scale_chn[ind].str);
	}
	/* set gain ctrl */
	osc_gain_sel(chn,osc_vol_scale_chn[ind].gain_sel);
	/* set dac */
	osc_vol_dac(chn,osc_vol_scale_chn[ind].gain_ctrl_index[chn]);
	/* end */	
}