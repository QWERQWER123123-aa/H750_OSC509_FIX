/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : touch.c
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
#include "hal_iic.h"
#include "gui.h"
#include "hal_exit.h"
#include "osc_cfg.h"
#include "osc_menu.h"
#include "hal_tim.h"
#include "osc.h"
#include "osc_ui.h"
#include "osc_measure.h"
#include "string.h"
#include "osc_win.h"
#include "main.h"
#include "stdio.h"
#include "GT9xx.h"
/* function */
static int touch_init(void);
static void touch_task_idle(void);
void touch_event_check(void);
void touch_event_handler(unsigned char event_type,unsigned short pox,unsigned short poy);
void touch_slider_v(unsigned short poy);
void touch_slider_h(unsigned short pox);
static void osc_touch_check_click(void);
int touch_item_slider(unsigned short * px,unsigned short * py,unsigned short cnt);
int touch_item_check_apx_area(unsigned short * px,unsigned short * py,unsigned short cnt);
int touch_item_trig_slider(unsigned short * px,unsigned short * py,unsigned short cnt);
extern int osc_msg_box_sta(void);
extern void osc_msgbox_touch(unsigned short xr,unsigned short yr);
extern void osc_msg_touch_release(void);
extern int osc_fine_tune_sta(void);
extern void osc_fine_tune_touch(unsigned short px, unsigned short py);
extern void osc_param_save_noload(void);
/* touch sta */
static int touch_sta = FS_ERR;
unsigned char LCD_INCH = 0;
unsigned char TOUCH_IC = 1 ;// 0 is FT , 1 is GT
unsigned char chn_stop_move = 0;
unsigned char chn_stop_move_f0 = 0;
unsigned char chn_stop_move_f1 = 0;
signed short move_chn_dp[2];
/* touch event */
#define TOUCH_EVENT_CLICK          (0xE0)
#define TOUCH_EVENT_LONG           (0xE1)
#define TOUCH_EVENT_SLIDER_H       (0xE2)
#define TOUCH_EVENT_SLIDER_V       (0xE3)
#define TOUCH_EVENT_ZOOM_H         (0xE4)
#define TOUCH_EVENT_ZOOM_V         (0xE5)
#define TOUCH_EVENT_DOUBLE         (0xE6)
#define TOUCH_EVENT_CLICK0         (0xE7)
#define TOUCH_EVENT_CLICK1         (0xE8)
/* pos */
#define TOUCH_QUEUE_DEEP           (20)
/* register node */
FOS_INODE_REGISTER("touch",touch_init,0,0,4);
/* idle task */
FOS_TSK_REGISTER(touch_task_idle,PRIORITY_4,10);
/* iic test cmd */
#define FT_CMD_WR 				      (0x70)
#define FT_CMD_RD 				      (0x71)
/* reg */
#define FT_DEVIDE_MODE 			    (0x00)
#define FT_REG_NUM_FINGER       (0x02)

#define FT_TP1_REG 				      (0x03)
#define FT_TP2_REG 				      (0x09)
#define FT_TP3_REG 				      (0x0F)
#define FT_TP4_REG 				      (0x15)
#define FT_TP5_REG 				      (0x1B) 
 
#define	FT_ID_G_LIB_VERSION		  (0xA1)
#define FT_ID_G_MODE 			      (0xA4)
#define FT_ID_G_THGROUP			    (0x80) 
#define FT_ID_G_PERIODACTIVE	  (0x88) 
/* tables */
const unsigned short FT5206_TPx_TBL[5] = 
{ FT_TP1_REG , FT_TP2_REG , FT_TP3_REG , 
  FT_TP4_REG , FT_TP5_REG };
const uint16_t GT9xx_TPR_TBL[5] = {GT9xx_TouchPoint1Reg,GT9xx_TouchPoint2Reg,
GT9xx_TouchPoint3Reg,GT9xx_TouchPoint4Reg,GT9xx_TouchPoint5Reg};
/* point */
static unsigned short postx[5] , posty[5];
/* color table */
const unsigned int color[5] = {2,3,4,5,6};
/* touch test var */
static unsigned char t_press_flag = 0;
/* static poxi queue */
static unsigned short tposb0_x[TOUCH_QUEUE_DEEP];
static unsigned short tposb0_y[TOUCH_QUEUE_DEEP];
/* static unsigned short cnt */
static unsigned short tpbs_cnt = 0;
/* mean flag */
static unsigned char mean_flag = 0;
/* static */
static unsigned char slider_chnn_enable = 0;
/* get draw area_tp */
const draw_area_def * area_tp;
static osc_run_msg_def * runmsg_tp;
/* static long fress falsg */
static unsigned int long_fress_cnt = 0;
/* tesf */
static unsigned int event_cnt[15];
/* extern */
extern widget_def btn_ch[4];
extern window_def win_menu[2];
extern widget_def btn[6];
extern widget_def ctrl_btn[7];
extern int osc_msg_box(unsigned char index);
/* press time at us */
static unsigned int tpt_click_us;
/* btn event */
static unsigned short btn_event_id = 0xffff;
/* btn */
static int touch_item_blank_id = FS_ERR;
unsigned int touch_idle_cnt = 0;
/* base interface */
static unsigned char FT5206_WR_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
  /* wr reg */
	unsigned char ret=0;
	/* iic start */
	IIC_Start();
	/* send wr cmd */
	IIC_Send_Byte(FT_CMD_WR);
	/* wait ack */
	IIC_Wait_Ack(); 	 	
  /* send cmd */	
	IIC_Send_Byte(reg&0xFF);
	/* wait ack */
	IIC_Wait_Ack();  
	/* send data  */
	for(int i=0;i<len;i++)
	{	   
		/* sent one byte */
		IIC_Send_Byte(buf[i]);
		/* wait ack */
		ret = IIC_Wait_Ack();
		/* check */
		if(ret)
		{
			break; 
		} 
	}
	/* iic stop */
	IIC_Stop();   
  /* return */	
	return ret; 
}
/* read data */			  
static void FT5206_RD_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
	/* iic start */ 
	IIC_Start();	
	/* send wr cmd */
	IIC_Send_Byte(FT_CMD_WR);
	/* wait ack */
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(reg&0xFF);
	/* wait ack */
	IIC_Wait_Ack();  
	IIC_Start();  	 	   
	IIC_Send_Byte(FT_CMD_RD);
	/* wait ack */	
	IIC_Wait_Ack();	   
	/* read data  */
	for( int i = 0 ; i < len ; i++ )
	{	   
		buf[i] = IIC_Read_Byte( i == ( len - 1 ) ? 0 : 1 );
	} 
	/* iic stop */
	IIC_Stop(); 
	/* end of */
} 
/* init */
static int touch_init(void)
{
	/* tmp */
	unsigned char temp[2];
	unsigned char err_cnt = 0;
	/* return */
	RETRY:
	/* send cmd */
  FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);
	/* sen */
	temp[0] = 22;
	/* valid */
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);
	/* time */
	temp[0]=12;
	/* active */
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	/* read version */
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	/* check */
	if( (temp[0] == 0x30 && temp[1] == 0xF8) )
	{
		/* set touch ok */
		touch_sta = FS_OK;
		/* set inch 5 */
		LCD_INCH = 5;
		/* ok */
		return FS_OK;
	} 
	else if( temp[1] == 0x01 )
	{
		/* set touch ok */
		touch_sta = FS_OK;
		/* set inch 7 */
		LCD_INCH = 7;
		/* ok */
		return FS_OK;		
	}else
	{
		/* GT9xx */
		if( GT9xx_Init() == 1 )
		{
			/* got it */
			TOUCH_IC = 1;
			/* set touch ok */
			touch_sta = FS_OK;
			/* set inch 7 */
			LCD_INCH = 7;
			/* ok */
			return FS_OK;				
		}
	}
	/* hal delay */
	HAL_Delay(500);
	/* err ++ */
	err_cnt ++;
	/* chcek */
	if( err_cnt < 1 )
	{
		/* retry */
		goto RETRY;
	}
	/* set touch ok */
	touch_sta = FS_ERR;	
	/* cannot find the touch pad*/	
	return FS_ERR;
}
/* static touch config */
int osc_touch_config(void)
{
	/* init */
	area_tp = get_draw_area_msg();
	runmsg_tp = get_run_msg();
	/* return */
	return FS_OK;
}
#if 0
/* static cal mean */
static unsigned int osc_mean_cal(unsigned short * mdat , unsigned short len,unsigned short * dst)
{
	/* -- */
	float sum = 0;
	unsigned int mdcnt = 0;
	/* mean */
	unsigned int mean = 0;
	/* sun */
	for( int i = 0 ; i < len ; i ++ )
	{
		sum += (float)mdat[i] / len;
	}
	/* cal mean */
	for( int i = 0 ; i < len ; i ++ )
	{
		mean += (unsigned int)(((float)mdat[i] - sum ) * ((float)mdat[i] - sum ));
	}
	/* check */
	if( mean < 10 )
	{
		/* dfes */
		*dst = sum;
		/* return */
		return mean;		
	}
	/* check same */
  for( int i = 0 ; i < len ; i ++ )
	{
		/* check */
		unsigned short ltmp = mdat[i];
		mdcnt = 0;
		/* check same */
		for( int j = 0 ; j < len ; j ++ )
		{
			if( ltmp == mdat[j] )
			{
				mdcnt ++;
				/* check */
				if( mdcnt >= len / 2 )
				{
					/* dfes */
					*dst = ltmp;		
          /* check */					
					return 0;
				}
			}
		}
	}
	/* return */
	return mean;
}
/* static cal mean */
static unsigned short osc_click_cal(unsigned short * mdat , unsigned short len)
{
	/* -- */
	float sum = 0;
	unsigned short cnfdt = 0;
	/* sun */
	for( int i = 0 ; i < len ; i ++ )
	{
		sum += (float)mdat[i] / len;
	}
	/* cal mean */
	for( int i = 0 ; i < len ; i ++ )
	{
		/* check */
		float diff = ( sum > (float)mdat[i] ) ? ( sum - (float)mdat[i] ) : ( (float)mdat[i] - sum );
		/* check */
		if( diff < 10 )
		{
			cnfdt ++;
			/* check */
			if( cnfdt >= len / 2 )
			{
				/* ok */
				return (unsigned short)sum;
			}
		}
	}
	/* return */
	return 0xffff;
}
#endif
#if 0
static unsigned short py_lt = 0xffff;
static int upder_0 = 0;
static int upder_1 = 0;
static int upder_2 = 0;
static unsigned int diff_sum_x0 = 0;
static unsigned int diff_sum_x1 = 0;
static unsigned int diff_sum_y0 = 0;
static unsigned int diff_sum_y1 = 0;

/* touch check clear */
void touch_check_clear(void)
{
	upder_0 = 0;
	upder_1 = 0;
	upder_2 = 0;
	py_lt = 0xffff;
}
/* touch check real time */
int touch_check_rt_by(unsigned short py)
{
  /* check */
	if( py_lt != 0xffff )
	{
	  /* check */
		if( py > py_lt )
		{
			upder_0 ++;
			upder_1 = 0;
			/* increme  */
			diff_sum_y0 += ( py - py_lt);
			diff_sum_y1 = 0;
			/*----------*/
		}
		else if( py < py_lt )
		{
			upder_0 = 0;
			upder_1 ++;
			/* increme  */
			diff_sum_y1 += ( py_lt - py);
			diff_sum_y0 = 0;
			/*----------*/			
		}
		else
		{
			upder_2 ++;
		}
	}
	else
	{
		diff_sum_y0 = 0;
		diff_sum_y1 = 0;
	}
	/* if check */
	if( upder_0 >= 5 )
	{
		/* clear */
		touch_check_clear();
		/* return */
		return 1;//down
	}
	/* up */
	if( upder_1 >= 5 )
	{
		/* clear */
		touch_check_clear();
		/* return */		
		return 2;//up		
	}
	/* clear */
  py_lt = py;
	/* return */
	return FS_ERR;
}
#endif
/* flags */
static unsigned short cart[30];
/* check */

/* clear */
void touch_check_rt_clear(void)
{
	memset(cart,0,sizeof(cart));
}
/* check touch item at area */
int touch_area_item_click(unsigned short * px,unsigned short * py,unsigned short len)
{
	/* check */
	unsigned short sum0 = 0 , sum1 = 0 , sum2 = 0;
	/* for */
	for( int i = 0 ; i < len ; i ++ )
	{
		/* check area */
		if( ( px[i] < area_tp->start_pos_x + area_tp->total_pixel_h / 2 ) && 
			  ( py[i] > area_tp->start_pos_y + area_tp->pixel_vertiacl ) )
		{
			sum0 ++;
		}	
		else if( ( px[i] > area_tp->start_pos_x + area_tp->total_pixel_h / 2 ) && 
			       ( px[i] < area_tp->start_pos_x + area_tp->total_pixel_h - area_tp->pixel_horizontal ) && 
		         ( py[i] > area_tp->start_pos_y + area_tp->pixel_vertiacl ) )
		{
			sum1 ++;
		}
		else
		{
			sum2 ++;
		}
	}
	/* check max */
	if( sum0 > sum1 && sum0 > sum2 )
	{
		return 0;
	}
	else if( sum1 > sum0 && sum1 > sum2 )
	{
		return 1;
	}
	else
	{
		/* error */
		return 2;
	}
}
/* touch check on time */
int touch_area_check_ontime(unsigned short px,unsigned short py)
{
	/* check */
	if( osc_l2_window_sta() )
	{
		/* check */
		osc_l2_touch(px,py);
		/* reutn */
		return FS_ERR;
	}	
	/* check fine tune panel */
	if( osc_fine_tune_sta() )
	{
		osc_fine_tune_touch(px, py);
		return FS_ERR;
	}
	/* check */
	if( osc_msg_box_sta() )
	{
		/* check */
		osc_msgbox_touch(px,py);
		/* reutn */
		return FS_ERR;
	}		
	/* check */
	if( px > area_tp->start_pos_x && px < area_tp->stop_pos_x &&
		  py > area_tp->start_pos_y && py < area_tp->stop_pos_y )
	{
		cart[0] ++;
	}
	else if( px > btn_ch[0].msg.x && px < ( btn_ch[0].msg.x_size + btn_ch[0].msg.x )&&
					 py > btn_ch[0].msg.y && py < ( btn_ch[0].msg.y_size + btn_ch[0].msg.y ))
	{
		cart[10] ++;
	}
	else if( px > btn_ch[1].msg.x && px < ( btn_ch[1].msg.x_size + btn_ch[1].msg.x )&&
					 py > btn_ch[1].msg.y && py < ( btn_ch[1].msg.y_size + btn_ch[1].msg.y ))
	{
		cart[11] ++;
	}
	else if( px > btn_ch[2].msg.x && px < ( btn_ch[2].msg.x_size + btn_ch[2].msg.x )&&
					 py > btn_ch[2].msg.y && py < ( btn_ch[2].msg.y_size + btn_ch[2].msg.y ))
	{
		cart[12] ++;
	}
	else if( px > btn_ch[3].msg.x && px < ( btn_ch[3].msg.x_size + btn_ch[3].msg.x )&&
					 py > btn_ch[3].msg.y && py < ( btn_ch[3].msg.y_size + btn_ch[3].msg.y ))
	{
		cart[13] ++;
	}	
	else if( px > win_menu[0].msg.x && px < ( win_menu[0].msg.x_size + win_menu[0].msg.x )&&
					 py > win_menu[0].msg.y && py < ( win_menu[0].msg.y_size + win_menu[0].msg.y ))
	{
		/* transfer to abs pos */
		unsigned short ads_px ;
		unsigned short ads_py ;
		/* check two menu btn */
		if( osc_ctrl_menu_sta() )
		{
			/* if */
			for( int i = 0 ; i < 6 ; i ++ )
			{
				/* transfer to abs pos */
				ads_px =  btn[i].msg.x + win_menu[0].msg.x;
				ads_py =  btn[i].msg.y + win_menu[0].msg.y;
				/* check */
				if( px > ads_px && px < ( btn[i].msg.x_size + ads_px )&&
						 py > ads_py && py < ( btn[i].msg.y_size + ads_py ))
				{
					cart[14 + i] ++;
				}				
			}
		}
		else
		{
			/* if */
			for( int i = 0 ; i < 7 ; i ++ )
			{
				/* transfer to abs pos */
				ads_px =  ctrl_btn[i].msg.x + win_menu[1].msg.x;
				ads_py =  ctrl_btn[i].msg.y + win_menu[1].msg.y;
				/* check */
				if( px > ads_px && px < ( ctrl_btn[i].msg.x_size + ads_px )&&
						 py > ads_py && py < ( ctrl_btn[i].msg.y_size + ads_py ))
				{
					cart[20 + i] ++;
				}				
			}
		}
	}
	else
	{
		cart[29] ++;
	}
	/* check */
	for( int i = 0 ; i < 30 ; i ++ )
	{
		/* check */
		if( cart[i] >= 2 )
		{
			return i;
		}
	}
	/* return ERR */
	return FS_ERR;
}
/* set */
int touch_check_area_rt(unsigned short px,unsigned short * ppx)
{
  /* check area_tp */
	if( px < area_tp->start_pos_x + area_tp->pixel_horizontal * 4 )
	{
		cart[0] ++;
		cart[1] = 0;
		cart[2] = 0;
		cart[3] = 0;
		*ppx = px;
	}
	else if( ( px > ( area_tp->start_pos_x + area_tp->pixel_horizontal * 6 ) ))
	{
		/* check measure */
		if( px > ( area_tp->start_pos_x + area_tp->pixel_horizontal * 9 ) )
		{
			cart[0] = 0;
			cart[1] = 0;		
			cart[2] ++;	
			cart[3] = 0;	
			*ppx = px;				
		}
		else
		{
			cart[0] = 0;
			cart[1] ++;		
			cart[2] = 0;
			cart[3] = 0;				
			*ppx = px;			
		}
	}
	else
	{
		cart[0] = 0;
		cart[1] = 0;		
		cart[2] = 0;
		cart[3] ++;
		*ppx = px;
	}
	/* check */
	if( cart[0] >= 8 )
	{
		/* clear */
		touch_check_rt_clear();
		/* clear */
		return 1;
	}
	/* check */
	if( cart[1] >= 8 )
	{
		/* clear */
		touch_check_rt_clear();
		/* clear */
		return 2;
	}
	/* check */
	if( cart[2] >= 8 )
	{
		/* clear */
		touch_check_rt_clear();
		/* clear */
		return 3;
	}	
	/* check */
	if( cart[3] >= 8 )
	{
		/* clear */
		touch_check_rt_clear();
		/* clear */
		return 4;
	}	
	/* error */
	return FS_ERR;
}
/* tp time */
volatile unsigned int tp_ims[100];
/* thread */
static void FT5206_Scan(unsigned char mode0)
{
	unsigned char buf[4];
	unsigned char temp;
	unsigned char sta = 0;
	static unsigned int t = 0;
		/* get sem */
	if( hal_iic_sem_get() == 1 )
	{
		return;
	}
	/* set sem */
	hal_iic_sem_set(1);	
	/* freq */
	t++;
	/* enter */
	if(( t % 5 ) == 0 || t < 200 * 5 ) // 5s
	{	
		/* read sta */
		if( TOUCH_IC == 0 )
		{
			FT5206_RD_Reg(FT_REG_NUM_FINGER,&sta,1);
		}
		else
		{
			GT9xx_RdNByte(GT9xx_DevAdr0,GT9xx_TouchStateReg,&sta,1);
		}
		/* release sem */
		hal_iic_sem_set(0);	
		/* check  */
		if( TOUCH_IC == 0 )
		{
			/* check */
			if( ( sta & 0xF ) && ( ( sta & 0xF ) < 6 ))
			{
				/* check */
				temp = 0xFF << ( sta & 0xF );
				/* sta */
				sta = (~temp)|0x80|0x40; 
			}
			else
			{
				sta = 0;
			}
		}
		else
		{
			if( sta & 0x80 )
			{
				if(sta & 0x0F)
				{
					sta = ~ ( 0xFF << ( sta & 0x0F ) );
				}
				else
				{
					/* clear */
					if( TOUCH_IC == 1 )
					{
						sta = 0;
						GT9xx_WrNByte(GT9xx_DevAdr0,GT9xx_TouchStateReg,&sta,1);
					}
				}
			}
			else
			{
				sta = 0;
			}
		}
		/* check */
		if( sta )
		{
			/* read point */
			for(int i = 0 ; i < 1 ; i++ )
			{
				if( sta & ( 1 << i ))
				{
					/* read x y */
					if( TOUCH_IC == 0 )
					{
						FT5206_RD_Reg(FT5206_TPx_TBL[i],buf,4);
					}
					else
					{
						GT9xx_RdNByte(GT9xx_DevAdr0,GT9xx_TPR_TBL[i],buf,4);
					}
					/* clear */
					if( TOUCH_IC == 1 )
					{
						sta = 0;
						GT9xx_WrNByte(GT9xx_DevAdr0,GT9xx_TouchStateReg,&sta,1);
					}					
					/* release sem */
					hal_iic_sem_set(0);							
					/* set point */
					if( LCD_INCH == 5 )
					{
						postx[i]=((unsigned short)(buf[0]&0x0F)<<8)+buf[1];
						posty[i]=((unsigned short)(buf[2]&0x0F)<<8)+buf[3];
					}
					else if( LCD_INCH == 7 )
					{
						/* if */
						if( TOUCH_IC == 0 )
						{
							postx[i]=((unsigned short)(buf[2]&0x0F)<<8)+buf[3];
							posty[i]=((unsigned short)(buf[0]&0x0F)<<8)+buf[1];						
						}
						else
						{
							postx[i] = (((uint16_t)(buf[1]) << 8) + buf[0]);
							posty[i] = ((uint16_t)(buf[3]) << 8) + buf[2];
						}
					}
					else
					{
						postx[i] = 0;
						posty[i] = 0;
					}
					/* filter */
					if( postx[i] > 800 || posty[i] > 480 )
					{
						/* clear */
						if( TOUCH_IC == 1 )
						{
							sta = 0;
							GT9xx_WrNByte(GT9xx_DevAdr0,GT9xx_TouchStateReg,&sta,1);
						}						
						return;
					}
#if 0
						extern void set_noload_point( unsigned short x , unsigned short y , unsigned int color );
						/* set point */
						set_noload_point(postx[0],posty[0],COLOR_TIPS_ERROR);
#endif
					/* set */
					long_fress_cnt ++;
					/* rel */
					touch_idle_cnt = 0;
					/* set rot */
					if( mean_flag == 2 )
					{
						
					}
					else if( mean_flag == 3 )
				{
					if( long_fress_cnt > 10 )
					{
						if( slider_chnn_enable >= 8 && slider_chnn_enable <= 9 )
							touch_slider_h(postx[i]);
						else
							touch_slider_v(posty[i]);
					}
				}
					else
					{
						/* check area */
						if( btn_event_id == 0xffff )
						{
							/* buffer */
							tposb0_x[tpbs_cnt] = postx[0];
							tposb0_y[tpbs_cnt] = posty[0];
							/* check */
							tpbs_cnt ++;
							/* chec */
							if( tpbs_cnt >= TOUCH_QUEUE_DEEP )
							{
								/* clear */
								tpbs_cnt = 0;
							}							
							/* set time */
							if( tpt_click_us == 0 )
							{
								tpt_click_us = hal_sys_time_us();
							}
							/* check */
							int ret = touch_area_check_ontime(postx[0],posty[0]);//touch_check_area_rt(postx[0],&poxdt);
							/* check */
							if(  ret != FS_ERR )
							{
								/* got it */
								if( ret >= 10 && ret <= 13 )
								{
									/* press the button */
									osc_top_color(&btn_ch[ret - 10],1);
									/* set btn event */
									btn_event_id = ret;
								}
								else if( ret >= 14 && ret <= 19)
								{
									osc_ui_focus_btn(ret - 14 , 1);
									/* set btn event */
									btn_event_id = ret;									
								}
								else if( ret >= 20 && ret <= 26 )
								{
									/* filter */
									if( ret != 22 && ret != 25 )
									{
										osc_ui_focus_ctrl_btn(ret-20,1);
										/* set btn event */
										btn_event_id = ret;										
									}
								}
								else if( ret == 0 )
								{
									/* curcul area */
									btn_event_id = ret;
									/* check area field */
									touch_item_blank_id = touch_item_check_apx_area(tposb0_x,tposb0_y,tpbs_cnt);
									/* check  proity */
									if( touch_item_blank_id == 0 ) // vol offset
									{
										/* check vol first */
										touch_item_blank_id = touch_item_slider(tposb0_x,tposb0_y,tpbs_cnt);									
									  /* if no */
										if( touch_item_blank_id == FS_ERR )
										{
											/* check Cursor */
										}
									}
									else if( touch_item_blank_id == 1 )
								  {
										/* trig area */
										/* check vol first */
										touch_item_blank_id = touch_item_trig_slider(tposb0_x,tposb0_y,tpbs_cnt);									
									  /* if no */
										if( touch_item_blank_id == FS_ERR )
										{
											/* check Cursor */
										}
										/*-----------*/
									}
									else
									{
										/* free area */
										/* check vol first */
										touch_item_blank_id = touch_item_slider(tposb0_x,tposb0_y,tpbs_cnt);									
									  /* if no */
										if( touch_item_blank_id == FS_ERR )
										{
											/* check Cursor */
										}										
									}
									/* set time */
								}
								else
								{
									/* error */
								}
							}
						}
						else if( btn_event_id == 0 )
						{
							/* check long press and slider */
							/* ---------------------------- */
						}
						else
						{
							/* nothing to do */
						}
				  }
					/*---------*/
					t_press_flag = 1;
					/* clear */
					t = 0;
					/* set */					
				}			
			}		
		}
		else
		{
			/* GT9xx */			
			if( TOUCH_IC == 1 )
			{	
				/* idle cnt */
				touch_idle_cnt ++;
				/* check */
				if( touch_idle_cnt < 2 )
				{
					return;
				}
			}
			/* release sem */
			hal_iic_sem_set(0);			
			/* check */
			if( t_press_flag == 1 )
			{
				/* ctrl */
				osc_ui_release_ctrl_btn();
				/* clear */
				osc_ui_release_btn();
				/* clear */
				osc_ui_release_top_btn();
				/* clear */
				touch_check_rt_clear();		
				/* check event */
        if( btn_event_id != 0xffff )
				{
					/* check area */
					if( btn_event_id != 0 )
					{
						/* check */
						touch_event_handler(btn_event_id,0,0);
					}
					else
					{
						/* set check */
						if( long_fress_cnt > 1 && long_fress_cnt < 16 )
						{
							osc_touch_check_click();
						}
					}
					/* release */
					btn_event_id = 0xffff;					
				}
				else if( osc_l2_window_sta() )
				{
					osc_l2_touch_release();
				}
				else if( osc_fine_tune_sta() )
				{
					osc_fine_tune_touch_release();
				}
				else if( osc_msg_box_sta() )
				{
					/* release */
					osc_msg_touch_release();
				}
				else
				{
//					osc_touch_check_click();
				}
			}
			/* clear */
			t_press_flag = 0;
			/* clear */
			tpbs_cnt = 0;
			/* clear */
			mean_flag = 0;
			chn_stop_move = 0;
			/* clear */
			long_fress_cnt = 0;
			/* clear */
			tpt_click_us = 0;
			/* idle cnt */
			touch_idle_cnt ++;
			/* save param after touch release */
			osc_param_save_noload();			
		}
	}
	/* release sem */
	hal_iic_sem_set(0);		
}
/* check click */
static void osc_touch_check_click(void)
{
	volatile unsigned int diff_us = hal_sys_time_us() - tpt_click_us;
	/* check long press */
	if( diff_us < 150 * 1000 ) //100ms
	{
		/* get click event check */
		event_cnt[0] ++;
		/* click event */
		int ret = touch_area_item_click(tposb0_x,tposb0_y,tpbs_cnt);
		/* check */
		if( ret == 0 )
		{
			ret = TOUCH_EVENT_CLICK0;
		}
		else if( ret == 1 )
		{
			ret = TOUCH_EVENT_CLICK1;
		}
		else
		{
			/* error */
			ret = 0xffff;
		}
		/* event */
		touch_event_handler(ret,tposb0_x[0],tposb0_y[0]);
		/*----------------*/
	}
	else
	{
		tpt_click_us = 0;
		event_cnt[1] ++;
	}
	/* set time */			
}
/* static idle task for test */
static void touch_task_idle(void)
{
	/* check run mode */
	if( osc_run_mode() == RUN_STOP_MODE )	
	{
		//return;
	}	
	/* check  */
	if( touch_sta == FS_OK )
	{
		FT5206_Scan(0);
		/* check */
	}
#if 0
	else
	{
		static unsigned char init_dtt = 0;
		/* check */
		if( init_dtt ++ >= 100 ) // 1s
		{
			/* CLEAR */
			init_dtt = 0;
			/* INIT AGAIN */
			touch_init();
		}
	}
#endif
}
#if 0
/* touch */
void touch_event_check(void)
{
	/* time */
	static unsigned int tp_time_us[2] = {0,0};
	static unsigned char flag_tp = 0;
	static unsigned char flag_long_press = 0;
	static unsigned int double_click_time = 0;
	static unsigned int click_delay_us = 0;
	/* check */
	if( t_press_flag == 1 )
	{
		/* get time */
		tp_time_us[1] = hal_sys_time_us();
    /* time */
    if(( (tp_time_us[1] - tp_time_us[0] ) > 1000 * 1000) && (long_fress_cnt > 75) )
		{
			/* clear */
			long_fress_cnt = 0;
			/* touch long flag */
			if( flag_long_press == 0 )
			{
				if( !osc_ui_measure_sta() )
				{
					/* long press */
					//touch_event_handler(TOUCH_EVENT_LONG,tp_point[0],tp_point[1]);
				}
				/* flag_long_press */
				//flag_long_press = 1;
			}
		}
//		/* check area */
//		if( flag_tp == 0 )
//		{
//			/* set btn */
//			static int btn = 0;
//			
//			osc_ui_focus_btn(btn,1);
//			
//			btn ++;
//		}
		/* set press flag */
		flag_tp = 1;
	}
	else
	{
		/* check click */
		if( flag_tp == 1 )
		{
			/* clear */
			flag_tp = 0;
			/* check time */
			unsigned int lci_t = tp_time_us[1] - tp_time_us[0];
			/* time */
			if(  lci_t < 100 * 1000 )
			{
				/* click */
				unsigned int double_t = hal_sys_time_us() - double_click_time;				
				/*-----------*/
				tp_point[0] = osc_click_cal(tpos0_x,tps_cnt_click);
				tp_point[1] = osc_click_cal(tpos0_y,tps_cnt_click);
        /* check mean */
        if( tp_point[0] != 0xffff && tp_point[1] != 0xffff )
				{
					/* set time */
					if( double_t < 200 * 1000 )
					{
//						touch_event_handler(TOUCH_EVENT_DOUBLE,tp_point[0],tp_point[1]);
						/* do not exe */
						click_delay_us = 0;
					}
					else
					{
						click_delay_us = hal_sys_time_us();
					}
			  }
				else
				{
					event_cnt[11] ++;
				}
				/* update time */
				double_click_time = hal_sys_time_us();
			}
		}
		tp_time_us[0] = hal_sys_time_us();
		/* clear */
		flag_long_press = 0;
		/* clear */
		slider_chnn_enable = 0;
		/* clear */
		trig_follow[0] = 0xffff;
		trig_follow[1] = 0xffff;			
	}
	/* check */
	if( ( click_delay_us != 0 ) && (( hal_sys_time_us() - click_delay_us ) >= 250 * 1000 ))
	{
//		touch_event_handler(TOUCH_EVENT_CLICK,tp_point[0],tp_point[1]);
		click_delay_us = 0;
	}
}
#endif
/* static sitch to chnn */
static void swi_chnn_touch(osc_run_msg_def * rmd,unsigned char chn,unsigned short px,unsigned short py)
{
//	/* swi chn */
//	if( rmd->chn_focus != chn )
//	{
//		user_swi_shnn(chn);
//	}
//	/* enable */
//	slider_chnn_enable = 1;
//	/* follow */
//	if( rmd->trig_source == chn )
//	{
//		trig_follow[0] = rmd->trig_vol_level_ch[chn];
//		trig_follow[1] = rmd->vol_offset_scale[chn];
//	}
//	else
//	{
//		trig_follow[0] = 0xffff;
//		trig_follow[1] = 0xffff;								
//	}	
}
/* check int */
int osc_ui_cont(gui_info_def * gid,unsigned short px,unsigned short py)
{
	/* check */
	if( ( px > gid->x && px < gid->x + gid->x_size ) && 
      ( py > gid->y && py < gid->y + gid->y_size ) )
	{
		/* return */
		return FS_OK;
	}
	/* error */
	return FS_ERR;
}
#if 0
/* void check livk */
void touch_key_transfer_win(unsigned char event_type,unsigned short pox,unsigned short poy)
{
	/* def */
	gui_info_def * sfe;
	/* check */
	sfe = osc_ui_win_meun_msg();
	/* check event */
	switch(event_type)
	{
		/* click */
		case TOUCH_EVENT_CLICK:
			/* suf */
			if( osc_ui_cont(sfe,pox,poy) == FS_OK )
			{
				/* find botton */
				int btn = osc_ui_find_win_btn(pox,poy);
#if 0				
				/* TEST */
				osc_ui_focus_btn(btn,1);
#endif				
				/* check */
				if( btn != FS_ERR )
				{
					/* switch */
					switch(btn)
					{
						case 0:
							key_runstop_callback();
							break;
						case 1:
							key_auto_callback();
							break;
						case 2:
							key_measure_callback();
							break;
						case 3:
							key_single_callback();
							break;
						case 4:
							key_menu_Longfress_callback();
							break;
						default:
							break;
					}
				}
			}
			else
			{
				//key_menu_callback();
			}	
			/* end */
			break;
		 /* long */
		 case TOUCH_EVENT_LONG:
			/* suf */
			if( osc_ui_cont(sfe,pox,poy) == FS_OK )
			{
				/* find botton */
				int btn = osc_ui_find_win_btn(pox,poy);
				/* check */
				if( btn != FS_ERR )
				{
					/* switch */
					switch(btn)
					{
#if 0						
						case 0:
							key_runstop_callback();
							break;
						case 1:
							key_auto_callback();
							break;
						case 2:
							key_measure_callback();
							break;
						case 3:
							key_single_callback();
							break;					
						case 4:
							key_menu_Longfress_callback();
#endif						
							break;
						default:
							break;
					}
				}
			}
			/* end */
			break;	
		default:
			break;					
	}
}
#endif
/* check trig area */
int touch_item_check_apx_area(unsigned short * px,unsigned short * py,unsigned short cnt)
{
	/* check ped area */
	unsigned short ga_px;
	//unsigned short ga_py;	
	/* clear */
	touch_check_rt_clear();
	/* transfer to grid axis */
	for( int i = 0 ; i < cnt ; i ++ )
	{
		/* transfer to abs axis */
		ga_px = px[i] - area_tp->start_pos_x;
		//ga_py = py[i] - area_tp->start_pos_y;
		/* check ads */
		if( ga_px < area_tp->pixel_horizontal * 3 )
		{
			cart[0] ++;//voltage
		}
		else if( ga_px >= area_tp->pixel_horizontal * 12 )
		{
			cart[1] ++;//trig
		}
		else
		{
			cart[2] ++;//error area
		}
	}	
	/* check max */
	unsigned short max = cart[0];
	unsigned char  max_id = 0;
	/* check */
	for( int i = 0 ; i < 3 ; i ++ )
	{
		/* check */
		if( cart[i] > max )
		{
			max = cart[i];
			/* check */
			max_id = i;
		}
	}
	/* return */
	return max_id;
	/*--------*/
}
/* check touch trig slider */
int touch_item_trig_slider(unsigned short * px,unsigned short * py,unsigned short cnt)
{
	/* check */
	/* check item */
	unsigned short ga_py;
	unsigned short diff0 = 0;	
	/* check */
	unsigned short cnt_get = 0;
	unsigned short cnt_noget = 0;	
	/* check */
  unsigned short gy_trig_abs = osc_trig_source() == 0 ? runmsg_tp->trig_vol_level_ch[0] : runmsg_tp->trig_vol_level_ch[1];
	/* check */
	/* ret */
	int ret = FS_ERR;
	/* 幅度光标检测 */
	if( osc_param_sys_get(runmsg_tp->sys_menu_set, 16) )
	{
		unsigned short cur_ay = area_tp->start_pos_y + runmsg_tp->amp_cursor_a;
		unsigned short cur_by = area_tp->start_pos_y + runmsg_tp->amp_cursor_b;
		unsigned short cnt_a = 0, cnt_b = 0;
		unsigned short sum_a = 0, sum_b = 0;
		for( int i = 0 ; i < cnt ; i ++ )
		{
			unsigned short diff = (cur_ay > py[i]) ? (cur_ay - py[i]) : (py[i] - cur_ay);
			if( diff <= area_tp->pixel_vertiacl * 3 ) { cnt_a ++; sum_a += diff; }
			diff = (cur_by > py[i]) ? (cur_by - py[i]) : (py[i] - cur_by);
			if( diff <= area_tp->pixel_vertiacl * 3 ) { cnt_b ++; sum_b += diff; }
		}
		if( cnt_a >= 2 && cnt_b < 2 ) { slider_chnn_enable = 6; mean_flag = 3; return FS_OK; }
		else if( cnt_b >= 2 && cnt_a < 2 ) { slider_chnn_enable = 7; mean_flag = 3; return FS_OK; }
		else if( cnt_a >= 2 && cnt_b >= 2 ) { slider_chnn_enable = (sum_a < sum_b) ? 6 : 7; mean_flag = 3; return FS_OK; }
	}
	/* 时间光标检测 */
	if( osc_param_sys_get(runmsg_tp->sys_menu_set, 17) )
	{
		unsigned short cur_ax = area_tp->start_pos_x + runmsg_tp->time_cursor_a;
		unsigned short cur_bx = area_tp->start_pos_x + runmsg_tp->time_cursor_b;
		unsigned short cnt_a = 0, cnt_b = 0;
		unsigned short sum_a = 0, sum_b = 0;
		for( int i = 0 ; i < cnt ; i ++ )
		{
			unsigned short diff = (cur_ax > px[i]) ? (cur_ax - px[i]) : (px[i] - cur_ax);
			if( diff <= area_tp->pixel_horizontal * 3 ) { cnt_a ++; sum_a += diff; }
			diff = (cur_bx > px[i]) ? (cur_bx - px[i]) : (px[i] - cur_bx);
			if( diff <= area_tp->pixel_horizontal * 3 ) { cnt_b ++; sum_b += diff; }
		}
		if( cnt_a >= 2 && cnt_b < 2 ) { slider_chnn_enable = 8; mean_flag = 3; return FS_OK; }
		else if( cnt_b >= 2 && cnt_a < 2 ) { slider_chnn_enable = 9; mean_flag = 3; return FS_OK; }
		else if( cnt_a >= 2 && cnt_b >= 2 ) { slider_chnn_enable = (sum_a < sum_b) ? 8 : 9; mean_flag = 3; return FS_OK; }
	}
	/* transfer to grid axis */
	for( int i = 0 ; i < cnt ; i ++ )
	{
		/* check */
		ga_py = py[i] - area_tp->start_pos_y;
		/* check channel */
		diff0 = ( gy_trig_abs > ga_py ) ? ( gy_trig_abs - ga_py ) : ( ga_py - gy_trig_abs );
		/* check ch1 */
		if( diff0 <= area_tp->pixel_vertiacl * 2 )
		{
			cnt_get ++;
		}	
		else
		{
			cnt_noget ++;
		}			
	}		
	/* check cnt */
	if( cnt_get >= 2 )
	{
		ret = 0;
	}	
	else
	{
		/* error */
		ret = FS_ERR;
	}
	/* ok release */
	/* check area proice */
	if( ret == 0 )
	{
		/* check */
		if( osc_trig_source() == 0 )
		{
			/* trig ch1 */
			slider_chnn_enable = 3;
		}
		else
		{
			/* trig ch2 */
			slider_chnn_enable = 4;			
		}
		/* enable the slider */
		mean_flag = 3;
	}
	else
	{
		/* nothing */
		mean_flag = 0;
	}	
  /*------------*/
	return ret;
}
/* check what item to slider */
int touch_item_slider(unsigned short * px,unsigned short * py,unsigned short cnt)
{
	/* 幅度光标检测 */
	if( osc_param_sys_get(runmsg_tp->sys_menu_set, 16) )
	{
		unsigned short cur_ay = area_tp->start_pos_y + runmsg_tp->amp_cursor_a;
		unsigned short cur_by = area_tp->start_pos_y + runmsg_tp->amp_cursor_b;
		unsigned short cnt_a = 0, cnt_b = 0;
		unsigned short sum_a = 0, sum_b = 0;
		for( int i = 0 ; i < cnt ; i ++ )
		{
			unsigned short diff = (cur_ay > py[i]) ? (cur_ay - py[i]) : (py[i] - cur_ay);
			if( diff <= area_tp->pixel_vertiacl * 3 ) { cnt_a ++; sum_a += diff; }
			diff = (cur_by > py[i]) ? (cur_by - py[i]) : (py[i] - cur_by);
			if( diff <= area_tp->pixel_vertiacl * 3 ) { cnt_b ++; sum_b += diff; }
		}
		if( cnt_a >= 2 && cnt_b < 2 ) { slider_chnn_enable = 6; mean_flag = 3; return FS_OK; }
		else if( cnt_b >= 2 && cnt_a < 2 ) { slider_chnn_enable = 7; mean_flag = 3; return FS_OK; }
		else if( cnt_a >= 2 && cnt_b >= 2 ) { slider_chnn_enable = (sum_a < sum_b) ? 6 : 7; mean_flag = 3; return FS_OK; }
	}
	/* 时间光标检测 */
	if( osc_param_sys_get(runmsg_tp->sys_menu_set, 17) )
	{
		unsigned short cur_ax = area_tp->start_pos_x + runmsg_tp->time_cursor_a;
		unsigned short cur_bx = area_tp->start_pos_x + runmsg_tp->time_cursor_b;
		unsigned short cnt_a = 0, cnt_b = 0;
		unsigned short sum_a = 0, sum_b = 0;
		for( int i = 0 ; i < cnt ; i ++ )
		{
			unsigned short diff = (cur_ax > px[i]) ? (cur_ax - px[i]) : (px[i] - cur_ax);
			if( diff <= area_tp->pixel_horizontal * 3 ) { cnt_a ++; sum_a += diff; }
			diff = (cur_bx > px[i]) ? (cur_bx - px[i]) : (px[i] - cur_bx);
			if( diff <= area_tp->pixel_horizontal * 3 ) { cnt_b ++; sum_b += diff; }
		}
		if( cnt_a >= 2 && cnt_b < 2 ) { slider_chnn_enable = 8; mean_flag = 3; return FS_OK; }
		else if( cnt_b >= 2 && cnt_a < 2 ) { slider_chnn_enable = 9; mean_flag = 3; return FS_OK; }
		else if( cnt_a >= 2 && cnt_b >= 2 ) { slider_chnn_enable = (sum_a < sum_b) ? 8 : 9; mean_flag = 3; return FS_OK; }
	}
	/* check item */
	unsigned short ga_py;
	unsigned short diff0 = 0;
	unsigned short diff1 = 0;
	/* ds */
	unsigned short sum0 = 0;
	unsigned short sum1 = 0;
	/* check */
	unsigned short cnt_ch1 = 0;
	unsigned short cnt_ch2 = 0;
	/* ret */
	int ret = FS_ERR;
	/* transfer to grid axis */
	for( int i = 0 ; i < cnt ; i ++ )
	{
		/* check */
		ga_py = py[i] - area_tp->start_pos_y;
		/* check proity */
		/* check channel */
		diff0 = ( runmsg_tp->vol_offset_scale[0] > ga_py ) ? ( runmsg_tp->vol_offset_scale[0] - ga_py ) :
																																	( ga_py - runmsg_tp->vol_offset_scale[0] );
		diff1 = ( runmsg_tp->vol_offset_scale[1] > ga_py ) ? ( runmsg_tp->vol_offset_scale[1] - ga_py ) :
																																	( ga_py - runmsg_tp->vol_offset_scale[1] );
		/* check ch1 */
		if( diff0 <= area_tp->pixel_vertiacl )
		{
			cnt_ch1 ++;
		}
		/* check */
		if( diff1 <= area_tp->pixel_vertiacl )
		{
			cnt_ch2 ++;
		}
		/* sum ++ */
		sum0 += diff0;
		sum1 += diff1;
	}
	/* check */
	if( cnt_ch1 >= 2 && cnt_ch2 < 2 )
	{
		ret =  0;
	}
	else if( cnt_ch2 >= 2 && cnt_ch1 < 2 )
	{
		ret = 1;
	}
	else if( cnt_ch1 >= 2 && cnt_ch2 >= 2 )
	{
		/* same */
		if( sum0 > sum1 )
		{
			ret = 1;
		}
		else 
		{
			ret = 0;
		}
	}
	else
	{
		/* get nothing */
	}
	/* check area proice */
	if( ret == 0 )
	{
		/* ch1 */
		slider_chnn_enable = 1;
		mean_flag = 3;
	}
	else if( ret == 1 )
	{
		/* ch2 */
		slider_chnn_enable = 2;
		mean_flag = 3;
	}
	else
	{
		/* nothing */
		mean_flag = 0;
	}		
	/* return */
	return ret;
}
/* slider check */
int touch_slider_v_check(unsigned char vid , unsigned short pox,unsigned short poy)
{
	/* check area_tp */
	int ret = FS_ERR;
	/* ret */
	switch(vid)
	{
		case 1:
		/* two enable */
		if(osc_enable_chn(0) == 0 && osc_enable_chn(1) == 0 )
		{
			/* check channel */
			unsigned short diff0 = ( runmsg_tp->vol_offset_scale[0] > poy ) ? ( runmsg_tp->vol_offset_scale[0] - poy ) :
																																		( poy - runmsg_tp->vol_offset_scale[0] );
			unsigned short diff1 = ( runmsg_tp->vol_offset_scale[1] > poy ) ? ( runmsg_tp->vol_offset_scale[1] - poy ) :
																																		( poy - runmsg_tp->vol_offset_scale[1] );
			/* check near */
			if( ( diff0 < area_tp->pixel_vertiacl ) && ( diff1 < area_tp->pixel_vertiacl ) )
			{
				/* chech chn */
				if( diff0 < diff1 )
				{
					/* swi to ch1 */
					swi_chnn_touch(runmsg_tp,0,pox,poy);
					/* break */
					return FS_OK;							
				}
				else
				{
					/* swi to ch2 */
					swi_chnn_touch(runmsg_tp,1,pox,poy);					
					/* break */
					return FS_OK;								
				}
			}
			else
			{
				event_cnt[6] ++;
			}
			/* runmsg_tp */					
		}
		/* check */
		if( osc_enable_chn(0) == 0 )
		{
			/* check channel */
			unsigned short diff = ( runmsg_tp->vol_offset_scale[0] > poy ) ? ( runmsg_tp->vol_offset_scale[0] - poy ) :
																																		( poy - runmsg_tp->vol_offset_scale[0] );
			/* runmsg_tp */
			if( diff < area_tp->pixel_vertiacl )
			{
				/* swi to ch1 */
				swi_chnn_touch(runmsg_tp,0,pox,poy);
				/* break */
				return FS_OK;
			}
			else
			{
				event_cnt[5] ++;
			}
		}
		/* check ch2 */
		if(osc_enable_chn(1) == 0 )
		{
			/* check channel */
			unsigned short diff = ( runmsg_tp->vol_offset_scale[1] > poy ) ? ( runmsg_tp->vol_offset_scale[1] - poy ) :
																																		( poy - runmsg_tp->vol_offset_scale[1] );
			/* runmsg_tp */
			if( diff <  area_tp->pixel_vertiacl )
			{
				/* swi to ch2 */
				swi_chnn_touch(runmsg_tp,1,pox,poy);				
				/* break */
				return FS_OK;						
			}
      else
			{
				event_cnt[7] ++;				
		  }
		}
		/* return ERR */
		break;
	/* next */
	case 3:
		/* check */
	  slider_chnn_enable = 5;
		/* ret */
		ret = FS_OK;
	  /* break */
		break;		
	/* next */
	case 2:
		/* enable */
		if( !osc_ui_measure_sta() )
		{
			/* def */
			gui_info_def * sfe = osc_ui_measure_meun_msg();
			/* suf */
			if( osc_ui_cont(sfe,pox,poy) == FS_OK )
			{
				slider_chnn_enable = 3;//measure
			}
			else
			{
				slider_chnn_enable = 2;		
			}
		}
		else if( !osc_ui_menu_sta() )
		{
			/* def */
			gui_info_def * sfe = osc_ui_win_meun_msg();
			/* suf */
			if( osc_ui_cont(sfe,pox,poy) == FS_OK )
			{
				slider_chnn_enable = 4;//menu
			}		
			else
			{
				slider_chnn_enable = 2;		
			}					
		}
		else
		{
			event_cnt[10] ++;
			slider_chnn_enable = 2;		
		}
		/* ret */
		ret = FS_OK;
		/* break */		
		break;
	case 4:
		break;
	default:
		break;
	}	
	/* return err */
	return ret;	
}
extern void osc_usbh_save_start(void);
extern int sd_card_sta;
unsigned char run_mode_fk = 0;
char range_file_name[64];
char msg_range_file_name[64];
unsigned int us_to_name = 0;
unsigned int us_bk = 0;
/* RUN and STOP */
void osc_run_and_stop_trs(void)
{
	if( run_mode_fk == 1 )
	{
		/* check */
		if( osc_run_mode() == RUN_STOP_MODE )
		{
			osc_set_run_mode(RUN_RUN_MODE);
			/* show */
			osc_run_stop(osc_run_mode());	
			/* run as usual */
			run_mode_fk = 0;
		}	
	}
}
extern int osc_msg_box(unsigned char index);
extern unsigned char chn_stop_move_f0;
extern unsigned char chn_stop_move_f1;
extern unsigned char mcd;
/* touch event handler */
void touch_event_handler(unsigned char event_id,unsigned short pox,unsigned short poy)
{
	/* switch id */
	switch( event_id )
	{
		case 14:
		case 20:
			osc_ctrl_change();
			break;
		case 15:
			/* check */
		  if( osc_run_mode() == RUN_RUN_MODE )
			{
				osc_set_run_mode(RUN_STOP_MODE);
			}
			else
			{
				osc_set_run_mode(RUN_RUN_MODE);
				chn_stop_move_f0 = 0;
				chn_stop_move_f1 = 0;
				mcd = 0;
			}
			/* set */
			osc_run_stop(osc_run_mode());
			/*-----*/
			break;
		case 16:
		/* auto */
		key_auto_callback();
		/*-----*/
		break;
		case 17:
			/* single trigger mode */
			{
				osc_run_msg_def * rmsg = get_run_msg();
				/* set trig type to SINGLE */
				osc_param_sys_set(&rmsg->sys_menu_set, RUN_TRIG_SINGLE, 15);
				/* update UI trig type */
				osc_ui_set_trig_type(RUN_TRIG_SINGLE);
				/* update global trt */
				extern unsigned char trt;
				trt = RUN_TRIG_SINGLE;
				/* if STOP mode, switch to RUN to wait for trigger */
				if( osc_run_mode() == RUN_STOP_MODE )
				{
					osc_set_run_mode(RUN_RUN_MODE);
					osc_run_stop(RUN_RUN_MODE);
				}
			}
			break;
		case 18:
			osc_create_measure_math_menu();
			break;
		case 19:
			/* sd card check */
		  if( sd_card_sta != 0 )
			{
				/* show */
				osc_msg_box(22);
				/* SD error */
				break;
			}
			/* check */
		  if( osc_run_mode() == RUN_RUN_MODE )
			{
				osc_set_run_mode(RUN_STOP_MODE);
				osc_run_stop(osc_run_mode());
				/* run as usual */
				run_mode_fk = 1;
			}
			/* show msg */
			if( us_to_name == 0 )
			{
				us_to_name = hal_sys_time_us() % 100000;
			}
			/* create name */
			sprintf(range_file_name,"0:osc%d_%d.bmp",us_to_name,us_bk);
			sprintf(msg_range_file_name,"Save:%s",range_file_name);
			/* inc */
			us_bk ++;
			/* show */
			osc_msg_box(19);
			/*----------*/
#if 1				
			osc_usbh_save_start();
#endif		
			/* end of */
			break;
		case 21:
			osc_vol_scale_thread(0,1);
		  osc_ui_vol_scale(2,0);
			break;
		case 23:
			osc_vol_scale_thread(0,0);
			osc_ui_vol_scale(2,0);
			break;
		case 24:
			osc_vol_scale_thread(1,1);
			osc_ui_vol_scale(3,0);
			break;
		case 26:
			osc_vol_scale_thread(1,0);
			osc_ui_vol_scale(3,0);
			break;		
		case 13:
			osc_create_system_menu();
			break;
		case 11:
			osc_create_ch12t_menu(1);
			break;
		case 12:
			osc_create_ch12t_menu(2);
			break;
		case 10:
			osc_create_ch12t_menu(0);
			break;
		case TOUCH_EVENT_CLICK0:
			/* time up at run mode */
		  osc_scan_thread(1);
			break;
		case TOUCH_EVENT_CLICK1:
			/* time up at run mode */
		  osc_scan_thread(0);			
			break;
		default :
			break;
	}
}
static unsigned short move_base[2];
/* void slider event */
void touch_slider_v(unsigned short poy)
{
	/* ch1 versig */
	if( slider_chnn_enable == 1 )
	{
		/*--------*/
		if( osc_run_mode() == RUN_STOP_MODE )
		{
			if( chn_stop_move_f0 == 0 )
			{
				chn_stop_move_f0 = 1;
				move_base[0] = runmsg_tp->vol_offset_scale[0];
			}
			/* ser */
			chn_stop_move = 1;
		}		
		/* set */
		runmsg_tp->vol_offset_scale[0] = poy - area_tp->start_pos_y;
		/* update */
		osc_offset_scale_thread(0);
		/* chec */
		if( chn_stop_move == 1 )
		{
			/* set */
			move_chn_dp[0] = runmsg_tp->vol_offset_scale[0] - move_base[0];			
		}
	}
	else if( slider_chnn_enable == 2 )
	{
		/*--------*/
		if( osc_run_mode() == RUN_STOP_MODE )
		{
			if( chn_stop_move_f1 == 0 )
			{
				chn_stop_move_f1 = 1;
				move_base[1] = runmsg_tp->vol_offset_scale[1];
			}
			/* set */
			move_chn_dp[1] = runmsg_tp->vol_offset_scale[1] - move_base[1];			
			/* ser */
			chn_stop_move = 2;
		}		
		/* set */
		runmsg_tp->vol_offset_scale[1] = poy - area_tp->start_pos_y;
		/* update */
		osc_offset_scale_thread(1);
	}
	else if( slider_chnn_enable == 3 )
	{
		runmsg_tp->trig_vol_level_ch[0] = poy - area_tp->start_pos_y;
		osc_trig_scale_thread(0);
	}
	else if( slider_chnn_enable == 4 )
	{
		runmsg_tp->trig_vol_level_ch[1] = poy - area_tp->start_pos_y;
		osc_trig_scale_thread(1);
	}
	else if( slider_chnn_enable == 5 )
	{
		//osc_ui_move_mlines(1,poy);	
	}
	else if( slider_chnn_enable == 6 )
	{
		unsigned short old_y = runmsg_tp->amp_cursor_a;
		runmsg_tp->amp_cursor_a = poy - area_tp->start_pos_y;
		osc_move_cursor_h_redraw(old_y);
	}
	else if( slider_chnn_enable == 7 )
	{
		unsigned short old_y = runmsg_tp->amp_cursor_b;
		runmsg_tp->amp_cursor_b = poy - area_tp->start_pos_y;
		osc_move_cursor_h_redraw(old_y);
	}
	else
	{
		
	}
//	/* slider_chnn_enable */
//	if( slider_chnn_enable == 1 )
//	{
//		osc_rot_set(OSC_VOL_OFFSET_SCALE,poy);	
//		/* check */
//		if( trig_follow[0] != 0xffff && trig_follow[1] != 0xffff )
//		{
//			/* chnn */
//			unsigned char chnn = runmsg_tp->chn_focus;
//			/*------ */
//			osc_rot_set(OSC_TRIG_SCALE,trig_follow[0] + runmsg_tp->vol_offset_scale[chnn] - trig_follow[1] );	
//		}
//	}
//	else if( slider_chnn_enable == 2 )
//	{
//		osc_rot_set(OSC_TRIG_SCALE,poy);	
//	}
//	else if( slider_chnn_enable == 3 )
//	{
//		/* find botton */
//		static unsigned int btn_last = 0xffff;
//		/* check */
//		int btn = osc_ui_find_measure_btn_by(poy);
//		/* ifche */
//		if( btn != btn_last )
//		{
//			/* TEST */
//			osc_ui_release_measure_btn();
//			/* set btn */
//			osc_ui_focus_measure_btn(btn,1);	
//			/* clear */
//			long_fress_cnt = 0;
//		}			
//		/* update */
//		btn_last = btn;
//	}
//	else if( slider_chnn_enable == 5 )
//	{
//		osc_ui_move_mlines(0,poy);	
//	}
//	else
//	{
//		
//	}
}
/* 时间光标水平拖动 */
void touch_slider_h(unsigned short pox)
{
	if( slider_chnn_enable == 8 )
	{
		unsigned short old_x = runmsg_tp->time_cursor_a;
		runmsg_tp->time_cursor_a = pox - area_tp->start_pos_x;
		osc_move_cursor_v_redraw(old_x);
	}
	else if( slider_chnn_enable == 9 )
	{
		unsigned short old_x = runmsg_tp->time_cursor_b;
		runmsg_tp->time_cursor_b = pox - area_tp->start_pos_x;
		osc_move_cursor_v_redraw(old_x);
	}
}
/* set focus */
void touch_measure_key_focus_mem(unsigned char index)
{
	/* check */
	int btn = osc_ui_find_measure_btn_mem();
	/* ifche */
	if( btn != index )
	{
		/* TEST */
		osc_ui_release_measure_btn();
		/* set btn */
		osc_ui_focus_measure_btn(index,1);	
		/* clear */
		long_fress_cnt = 0;
	}
}
/*-------*/