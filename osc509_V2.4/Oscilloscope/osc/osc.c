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
#include "display_dev.h"
#include "math.h"
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "string.h"
#include "osc_menu.h"
/* Private includes ----------------------------------------------------------*/
#include "osc_icon.h"
#include "osc_win.h"
/* functions */
extern int osc_touch_config(void);
/* draw area defines */
static draw_area_def draw_area;
/* define some nes message */
static unsigned short EN,EM;
#if !HARDWARE_ACCEL_SUPPLY /* no use without hardware accel */
static unsigned short ENR,EMRR;
#endif
/* this param is for test that will be deleted soon */
static unsigned char grid_init_flag = 0;
/* 
	create grid_grobal_data 
	Computing grid data according to different resolutions 
*/
static int grid_grobal_data(unsigned short width, unsigned short height)
{
	/* insc */
	width -=RIGHT_REMAIN_PIXEL;
	/* horizonal data Find the greatest common divisor for 5 and 10 */
	EN = ( width - LEFT_REMAIN_PIXEL - 2 ) / ( HORIZONTAL_GRID_TOTAL );
#if !HARDWARE_ACCEL_SUPPLY	
  /* The remaining pixels on the far right of the screen */
	ENR = width - LEFT_REMAIN_PIXEL - 1 - EN * (HORIZONTAL_GRID_TOTAL) - 1;
#endif
  /* for verital data Find the greatest common divisor for 5 and 8 */
	EM = ( height - TOP_REMAIN_PIXEL - 2 - BOTTOM_REMAIN_PIXEL ) / (VERTICAL_GRID_TOTAL);
  /* The remaining pixels on the far bottom of the screen */
#if !HARDWARE_ACCEL_SUPPLY	
	EMRR = height - EM * (VERTICAL_GRID_TOTAL) - TOP_REMAIN_PIXEL - 1 - 1;
#endif
	/* create ok or not */
	if( EN == 0 || EM == 0 )
	{
		/* oh no , feels not good , What seems to be the problem */
		return (-1);
	}
	/* set area data  , start pos */
	draw_area.start_pos_x = LEFT_REMAIN_PIXEL + 1;
	draw_area.start_pos_y = TOP_REMAIN_PIXEL + 1;
	draw_area.stop_pos_x  = LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL);
	draw_area.stop_pos_y = TOP_REMAIN_PIXEL  + 1 + EM * (VERTICAL_GRID_TOTAL);
	/* num grid of horizontal and vertical */
	draw_area.num_horizontal = HORIZONTAL_GRID_NUM;
	draw_area.num_vertical = VERTICAL_GRID_NUM;
	/* num pixel for all horizontal */
	draw_area.pixel_horizontal = LITTLE_GRIG_NUM * EN;
	draw_area.pixel_vertiacl = LITTLE_GRIG_NUM * EM;
	/* num of little grid */
	draw_area.little_grid = LITTLE_GRIG_NUM;
	/* total pixel */
	draw_area.total_pixel_h = HORIZONTAL_GRID_NUM * LITTLE_GRIG_NUM * EN;
	draw_area.total_pixel_v = VERTICAL_GRID_NUM * LITTLE_GRIG_NUM * EM;
	/* endding */
	return 0; // OK
}
/* get grid data struct for other apps and threads */
draw_area_def * get_draw_area_msg(void)
{
  /* create init or not */
	if( grid_init_flag == 0 )
	{
		/* reinit */
		return (draw_area_def *)(0) ; // not init yet 
	}
	/* return bat */
	return &draw_area;
}
/* create grid data */
static void create_grid_data(window_def * win)
{	
  /* get gui dev */
	/* set grid data */
	int ret = grid_grobal_data( win->dev->width , win->dev->height );
	/* ok or not */
	if( ret != 0 )
	{
		/* oh no , feels not good , What seems to be the problem */
		return;		
	}
  /* sraw */
#if HARDWARE_ACCEL_SUPPLY
	/* clear all area with one color */
	win->dev->clear_display_dev(COLOR_BACK_GROUND);
	/* if doesn't has the hardware accel */
#else
	for (unsigned int j = 0; j < win->dev->height; j++)
	{
		for (int i = 0; i < LEFT_REMAIN_PIXEL; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,i, j, COLOR_BACK_GROUND);
		}

		for (int i = 0; i < ENR + RIGHT_REMAIN_PIXEL; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,i + LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) +1 +1, j, COLOR_BACK_GROUND);
		}
	}
  /* draw other grid data */
	for (unsigned int j = 0; j < win->dev->width; j++)
	{
		for (int i = 0; i < TOP_REMAIN_PIXEL; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,j, i, COLOR_BACK_GROUND);
		}
    /* draw other grid data */
		for (int i = 0; i < EMRR - 1; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,j, i + TOP_REMAIN_PIXEL + 1 + 1 + 1 + EM * (VERTICAL_GRID_TOTAL), COLOR_BACK_GROUND);
		}
	}
#endif
/* create the data area background color */
#if HARDWARE_ACCEL_SUPPLY
/* full the rect area with some color */
  win->dev->fill_rect(LEFT_REMAIN_PIXEL + 1,TOP_REMAIN_PIXEL + 1,
	               LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) , 
								 TOP_REMAIN_PIXEL  + EM * (VERTICAL_GRID_TOTAL),COLOR_GRID_AREA_BG);
#else
#if 1
	for (int j = 0; j < EN * (HORIZONTAL_GRID_TOTAL) ; j++)
	{
		for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1 ; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + j, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_AREA_BG);
		}
	}
#endif
#endif
  /* draw other grid data */
	for(int j = 0 ; j < ( VERTICAL_GRID_NUM + 1 ); j++ )
	{
		for (int i = 0 ; i < ( HORIZONTAL_GRID_TOTAL + 1 ); i++ )
		{
			win->dev->set_point(&win->msg.mark_flag, LEFT_REMAIN_PIXEL + 1 + i * EN, TOP_REMAIN_PIXEL + 1 + j * EM * LITTLE_GRIG_NUM , COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int j = 0 ; j < ( HORIZONTAL_GRID_NUM + 1 ) ; j++ )
	{
		for (int i = 0 ; i < (VERTICAL_GRID_TOTAL + 1); i++ )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + j * EN * LITTLE_GRIG_NUM , TOP_REMAIN_PIXEL + 1 + i * EM, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */		//绘制上方横线的点
	for( int i = 0 ; i < EN * ( HORIZONTAL_GRID_TOTAL ) ; i++ )
	{
	  /* draw other grid data */ 
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( i % EN ) == 0 )				
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 2, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 3, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */	//绘制下方横线的点
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EN)) == 0 )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 2, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 3, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */	//绘制左右侧横线的点
	for( int i = 0 ; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL + 1  + i, COLOR_GRID_POINT);
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		/* draw other grid data */
		if((( i % EM )) == 0 )
		{
			/* draw other grid data */
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 3, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			/* draw other grid data */
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 1 , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 2 , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 3, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */	//绘制水平中线两侧的点
	for( int i = 0; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2), COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( (i % EN) ) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 1, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 2, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 1, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 2, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */	//绘制水平中线两侧的点
	for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2), TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EM)) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 1, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 1, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
//    /* draw other grid data */
//		if (((i % (EM * LITTLE_GRIG_NUM))) == 0)
//		{
//			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 8, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
//			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 7, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
//		}
	}
  /* draw other grid data */
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL, COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	/* create the title */
	//osc_create_TITLE(win);
	/* for test */
#if 0
  /* draw other grid data */
	double sin_x = 0;
	/* sinx */
	for( int i = 0 ; i < 50 ; i ++ )
	{
	 double te = 15 * sin( sin_x ) + 15;

		short tm = (short)( te );

		double tce = 15 * cos( sin_x ) + 15;

		short tcm = (short)( tce );

		win->dev->set_point(&win->msg.mark_flag, i , tm + 5, COLOR_CH1);

		win->dev->set_point(&win->msg.mark_flag, i , tcm  + 5, COLOR_CH2);

		sin_x += (6.28) / (double)(50.0f) * 3;
		
	}
#endif
/* set the flag */
	grid_init_flag = 1;
	/* get lcd msg */
	osc_touch_config();	
/* end of func */	
}
/* osc redraw the grid */
void osc_redraw_grid(window_def * win)
{
	/* remark error */
  /* draw other grid data */
	for(int j = 37 ; j < ( VERTICAL_GRID_NUM + 1 ); j++ )
	{
		for (int i = 0 ; i < ( HORIZONTAL_GRID_TOTAL + 1 ); i++ )
		{
			win->dev->set_point(&win->msg.mark_flag, LEFT_REMAIN_PIXEL + 1 + i * EN, TOP_REMAIN_PIXEL + 1 + j * EM * LITTLE_GRIG_NUM , COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int j = 0 ; j < ( HORIZONTAL_GRID_NUM + 1 ) ; j++ )
	{
		for (int i = 37 ; i < (VERTICAL_GRID_TOTAL + 1); i++ )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + j * EN * LITTLE_GRIG_NUM , TOP_REMAIN_PIXEL + 1 + i * EM, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EN)) == 0 )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 3, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 6, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 350 ; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL + 1  + i, COLOR_GRID_POINT);
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		/* draw other grid data */
		if((( i % EM )) == 0 )
		{
			/* draw other grid data */
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			/* draw other grid data */
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 2 , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for (int i = 350; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2), TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EM)) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 1, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 4, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
    /* draw other grid data */
		if (((i % (EM * LITTLE_GRIG_NUM))) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 8, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 7, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL, COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);	
}
/* draw group win */
static void draw_group_win(window_def * win)
{
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
  /* calutie and ending pos */
	unsigned short  pos_x = win->msg.x , pos_y = win->msg.y;
  /* calutie and ending pos */
	unsigned short pos_x_end = win->msg.x_size , pos_end_y = win->msg.y_size;
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
				win->dev->set_point(&win->msg.mark_flag, j + pos_x + 1 ,pos_y + i + 1,COLOR_GROUP_BG);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x ,i / 4 + pos_y,COLOR_GROUP_LINE);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_GROUP_LINE);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_GROUP_LINE);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_GROUP_LINE);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x + 4 + i , pos_y , COLOR_GROUP_LINE);
		win->dev->set_point(&win->msg.mark_flag,pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_GROUP_LINE);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x , pos_y + 4 + i , COLOR_GROUP_LINE);
		win->dev->set_point(&win->msg.mark_flag,pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_GROUP_LINE);
	}	
	/* create icon */
	if( win->msg.wflags & 0x0100 )
	{
		/* calbrate pos */
		unsigned short eh = ( win->msg.x_size - 4 ) / 3;
		unsigned short v_start = ( win->msg.y_size - 16 * 2 ) / 3;
		
		/* create icon */
		osc_create_chn_icon(win,2,v_start,1);
		osc_create_chn_icon(win,2 + eh,v_start,1);
		osc_create_chn_icon(win,2 + eh*2,v_start,1);
		/* ch2 */
		osc_create_chn_icon(win,2,v_start * 2 + 16,2);
		osc_create_chn_icon(win,2 + eh,v_start * 2 + 16,2);
		osc_create_chn_icon(win,2 + eh*2,v_start * 2 + 16,2);	
	}
	else if( win->msg.wflags & 0x0200 )
	{
		/* calbrate pos */
		unsigned short eh = ( win->msg.x_size - 4 ) / 2;
		unsigned short v_start = ( win->msg.y_size - 16 ) / 2;		
		/* create icon */
		osc_create_chn_icon(win,2,v_start,1);	
		osc_create_chn_icon(win,2 + eh ,v_start,2);	
	}
	else if( win->msg.wflags & 0x0800 )
	{
		/* nothing to do */
	}
}
/* clear win without judge */
void osc_clear_win(window_def * win)
{
	unsigned short sx = win->msg.x;
	unsigned short sy = win->msg.y + win->wchild->msg.y_size + 3;
	/* clear */
	unsigned short ysize = win->msg.y_size - win->wchild->msg.y_size - 3;
	/* set cler */
	for( int i = 0 ; i < ysize ; i ++ )
	{
		for( int j = 0 ; j < win->msg.x_size ; j ++ )
		{
			win->dev->set_noload_point( sx + j , i + sy ,COLOR_BACK_GROUND);
		}
	}
}
/* gImage_auto gImage_td */
void draw_sys_settings(struct widget * win , unsigned char pos_id,unsigned char show_id)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;	
	/* start x y */
	unsigned short ox,oy;
	/* char */
	unsigned short color;
	/* char */
	if( pos_id == 0 )
	{
		ox = 42;
		oy = 23;		
	}
	else if( pos_id == 1 )
	{
		ox = 42;
		oy = 5;				
	}
	else if( pos_id == 2 )
	{
		ox = 42 + 30 + 6;
		oy = 5;				
	}
	else if( pos_id == 3 )
	{
		ox = 42 + 30 + 6;
		oy = 23;				
	}
	else
	{
		
	}
	/* auto */
	unsigned short xs = 30;
	unsigned short ys = 14;
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* 0 , 1 , 2 , 5 */
	const unsigned char * set_char[13] = 
	{
		gImage_fft,gImage_fft,gImage_xy,gImage_yt,gImage_math,
		gImage_ab0,gImage_ab1,gImage_ad2,gImage_ad3,gImage_ai,
		gImage_dvy,gImage_dtx,gImage_adj
	};
	const unsigned short color_table[13] = 
	{
		COLOR_CH1_G,COLOR_CH2_G,COLOR_SYS_BG,
		COLOR_SYS_BG,COLOR_SYS_BG,COLOR_SYS_BG,
	  COLOR_SYS_BG,COLOR_SYS_BG,COLOR_SYS_BG,
	  COLOR_SYS_BG,COLOR_CH1_G,COLOR_CH2_G,
	  COLOR_SYS_BG
	};
	/* show */
	/* clear */
	if( show_id < 13 )
	{
		const unsigned char * set_tmpch = set_char[show_id];
		/* set color */
		color = color_table[show_id];
		/* show */
		for( int i = 0 ; i < ys ; i ++ )
		{
			/* unsigned char */
			for( int j = 0 ; j < xs ; j ++ )
			{
				/* show */
				if( ( set_tmpch[i*yow+ j / 8] << (j % 8)) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox , pos_y + oy + i ,COLOR_GRID_AREA_BG);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox, pos_y + oy + i ,color);
				}
			}
		}	
	}
	else
	{
		/* show */
		for( int i = 0 ; i < ys ; i ++ )
		{
			/* unsigned char */
			for( int j = 0 ; j < xs ; j ++ )
			{
				/* show */
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox , pos_y + oy + i ,COLOR_BUTTON);
			}
		}			
	}
}
/* gImage_auto gImage_td */
void draw_trig_td(struct widget * win , unsigned char show)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;	
	const unsigned char * set_char;
	/* start x y */
	unsigned short ox = 96;
	unsigned short oy = 5;
	/* auto */
	unsigned short xs = 16;
	unsigned short ys = 32;
	set_char = gImage_td;
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* 0 , 1 , 2 , 5 */
	if( show )
	{
		/* show */
		for( int i = 0 ; i < ys ; i ++ )
		{
			/* unsigned char */
			for( int j = 0 ; j < xs ; j ++ )
			{
				/* show */
				if( ( set_char[i*yow+ j / 8] << (j % 8)) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox , pos_y + oy + i ,COLOR_GRID_AREA_BG);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox, pos_y + oy + i ,COLOR_TRIG_TD);
				}
			}
		}	
	}
	else
	{
		/* hide */
		for( int i = 0 ; i < ys ; i ++ )
		{
			/* unsigned char */
			for( int j = 0 ; j < xs ; j ++ )
			{
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox , pos_y + oy + i ,COLOR_BUTTON);
			}
		}			
	}
}
/* trig type */
void draw_trig_type(struct widget * win , unsigned char trig_type)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;	
	/* get trig_type */
	unsigned short xs,ys;
	const unsigned char * set_char;
	/* start x y */
	unsigned short ox = 42;
	unsigned short oy = 6;
	/* trig type */
  if( trig_type == 0 )
	{
		/* auto */
		xs = 29;
		ys = 10;
		set_char = gImage_auto;
	}
	else if( trig_type == 1 )
	{
		/* normal */
		xs = 45;
		ys = 10;
		set_char = gImage_normal;		
	}
	else if( trig_type == 2 || trig_type == 3 )
	{
		/* single */
		xs = 37;
		ys = 13;
		set_char = gImage_single;		
	}	
	else
	{
		/* auto */
		xs = 29;
		ys = 10;
		set_char = gImage_auto;		
	}
	/* clear */
	for( int i = 0 ; i < 13 ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < 45 ; j ++ )
		{
			/* show */
			win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox, pos_y + oy + i ,COLOR_BUTTON);
		}
	}	
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < ys ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* show */
			if( ( set_char[i*yow+ j / 8] << (j % 8)) & 0x80 )
			{
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox , pos_y + oy + i ,COLOR_BUTTON);
			}
			else
			{
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox, pos_y + oy + i ,COLOR_CHAR);
			}
		}
	}	
}
/* osc release a area */
void osc_release_area(widget_def * wid,unsigned char ddt)
{
	/* get area */
	unsigned short sx = wid->msg.x;
	unsigned short sy = wid->msg.y;
	unsigned short sx_size = wid->msg.x_size;
	unsigned short sy_size = wid->msg.y_size;
	/* check */
	draw_area_def * area_re = get_draw_area_msg();
	/* back color */
	unsigned char color = ddt == 0 ? COLOR_BACK_GROUND : COLOR_GRID_AREA_BG;
	/* check */
	for( int i = 0 ; i < sy_size ; i ++ )
	{
		/* check */
		for( int j = 0 ; j < sx_size ; j ++ )
		{
			/* re p */
			unsigned short rt_x = sx + j;
			unsigned short rt_y = sy + i;
			/* out range */
			wid->dev->set_noload_point( rt_x , rt_y , color); 
			/* colse */
		}
	}
}
/* gImage_auto */
void draw_measure_item(struct widget * win,unsigned char chn,unsigned char item_id)
{
  extern const char * measure_item_chinese[10];
	extern const char * measure_item_english_simple[13];
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
  /* calutie and ending pos */
	unsigned short pos_x = win->msg.x;
	unsigned short pos_y = win->msg.y;
  /* calutie and ending pos */
	unsigned short pos_x_end = win->msg.x_size , pos_end_y = win->msg.y_size;	
	/* get trig_type */
	unsigned short xs = 19 , ys = 19;
	const unsigned char * set_char;
	unsigned short color = 0;
	/* if */
	if( chn == 0 )
	{
		set_char = gImage_mch1;
		color = COLOR_CH1_G;
	}
	else
	{
		set_char = gImage_mch2;
		color = COLOR_CH2_G;
	}
	/* start x y */
	unsigned short ox = 3;
	unsigned short oy = 3;
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
				win->dev->set_noload_point( j + pos_x + 1 ,pos_y + i + 1,COLOR_BUTTON);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x ,i / 4 + pos_y,COLOR_BUTTON);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_BUTTON);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_BUTTON);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_BUTTON);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		win->dev->set_noload_point(pos_x + 4 + i , pos_y , COLOR_BUTTON);
		win->dev->set_noload_point(pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_BUTTON);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		win->dev->set_noload_point(pos_x , pos_y + 4 + i , COLOR_BUTTON);
		win->dev->set_noload_point(pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_BUTTON);
	}	
	#if 1
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < ys ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* show */
			if( ( set_char[i*yow+ j / 8] << (j % 8)) & 0x80 )
			{
				win->dev->set_noload_point(pos_x + j + ox , pos_y + oy + i ,COLOR_GRID_AREA_BG);
			}
			else
			{
				win->dev->set_noload_point(pos_x + j + ox, pos_y + oy + i ,color);
			}
		}
	}	
	/* show HZ */
	/* show title */
	unsigned short px = pos_x + 3 + 19 + 3;
	unsigned short py = pos_y + 4;
	/* show */
	const char * tmpch = (osc_language_get() == 0 || item_id >= 10) ? measure_item_english_simple[item_id] : measure_item_chinese[item_id];
	/* show */
	unsigned short td_len = strlen(tmpch);
	/* check */
	if( osc_language_get() == 0 || item_id >= 10 )
	{
		osc_l1_string(win->dev,px,py,tmpch,COLOR_MEASURE_CHAR,0,0);
		/* show ‘:’ */
		gui_char(win->dev,px + td_len * 8 + 2, py,':',16,COLOR_MEASURE_CHAR,0,0);
		/* set */
		win->msg.mxstop = td_len * 8 + 2 + 8;
		/* ------------------------------- */
	}
	else
	{
		/* simple chinese */
		draw_hz(win->dev,(unsigned char *)&tmpch[0],px,py,COLOR_MEASURE_CHAR,0,0);
		draw_hz(win->dev,(unsigned char *)&tmpch[2],px+16,py,COLOR_MEASURE_CHAR,0,0);
		/* if */
		if ( tmpch[4] != NULL )
		{
			draw_hz(win->dev,(unsigned char *)&tmpch[4],px+32,py,COLOR_MEASURE_CHAR,0,0);
			gui_char(win->dev,px+32+16,py,':',16,COLOR_MEASURE_CHAR,0,0);
			/* set */
			win->msg.mxstop = px+32+16+8+2;
		}
		else
		{
			/* set */
			gui_char(win->dev,px+32,py,':',16,COLOR_MEASURE_CHAR,0,0);		
			/* set */
			win->msg.mxstop = px+32+8+2;		
		}
	}
	/* set */
	win->msg.upd = 0xE0;//has drawed
	/* set chn and item id */
	win->msg.mx = chn;
	win->msg.my = item_id;
	/* test */
	#else
	osc_draw_offset_arrow(win->dev,pos_x + ox ,  pos_y + oy , chn);
	#endif
}
/* gImage_auto */
void draw_trig_icon(struct widget * win , unsigned char trig_type)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;	
	/* get trig_type */
	unsigned short xs,ys;
	const unsigned char * set_char;
	/* start x y */
	unsigned short ox = 77;
	unsigned short oy = 21;
	/* trig type */
  if( trig_type == 1 )
	{
		/* auto */
		xs = 7;
		ys = 16;
		set_char = gImage_rf;
	}
	else if( trig_type == 0 )
	{
		/* normal */
		xs = 7;
		ys = 16;
		set_char = gImage_rt;		
	}
	else if( trig_type == 2 )
	{
		/* single */
		xs = 12;
		ys = 16;
		set_char = gImage_pulse;		
	}	
	else
	{
		/* auto */
		xs = 7;
		ys = 16;
		set_char = gImage_rf;		
	}
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < ys ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* show */
			if( ( set_char[i*yow+ j / 8] << (j % 8)) & 0x80 )
			{
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox , pos_y + oy + i ,COLOR_BUTTON);
			}
			else
			{
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox, pos_y + oy + i ,COLOR_CHAR);
			}
		}
	}	
}
/* draw x1 x10 double chn */
void draw_x1_x10(struct widget * win ,unsigned char x1_x10)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;	
	/* draw x10 */
	if( x1_x10 )
	{
		/* x1 */
		for( int i = 0 ; i < 9 ; i ++ )		
		{
			/* shift 2bits */
			unsigned short tx1 = ( char_X1[i] & 0xff00 ) | (( char_X1[i] & 0xff ) >> 2 );
			/* shift */			
			for( int j = 0 ; j < 16 ; j ++ )
			{
				if( ( tx1 << j ) & 0x8000 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 26 + i ,COLOR_CHAR);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 26 + i ,COLOR_BUTTON);
				}
			}
			/* set lan */
			for( int j = 0 ; j < 8 ; j ++ )
			{
				win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 16 , pos_y + 26 + i ,COLOR_BUTTON);
			}
		}	
	}
	else
	{
		/* x10 */
		for( int i = 0 ; i < 9 ; i ++ )		
		{
			/* shift 2bits */
			unsigned short tx1 = char_X1[i];
			/* shift */			
			for( int j = 0 ; j < 16 ; j ++ )
			{
				if( ( tx1 << j ) & 0x8000 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 26 + i ,COLOR_CHAR);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 26 + i ,COLOR_BUTTON);
				}
			}
			/* 0 */
			for( int j = 0 ; j < 8 ; j ++ )
			{
				if( ( char_X10[i] << j ) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 16 , pos_y + 26 + i ,COLOR_CHAR);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 16 , pos_y + 26 + i ,COLOR_BUTTON);
				}
			}			
		}			
	}
}
/* show vol trig */
void draw_vol_scale(struct widget * win,char * scale)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;		
	/* base pos */
	unsigned short ox;
	unsigned short oy;
	/* color */
	unsigned short color = COLOR_BUTTON;
	/* check */
	if( win->msg.mx == 1 || win->msg.mx == 2 )
	{
		ox = ( win->msg.x_size - 24 - 22 ) / 2;
		oy = ( win->msg.y_size - 12 ) / 2 + 1;
		color = COLOR_BACK_GROUND;
	}
	else
	{
		ox = 68;
		oy = 16;
		color = COLOR_BUTTON;
	}
	/* start pox */
	unsigned short stx = 0;
	unsigned char  type = 0;
	/* char */
	const unsigned char * set_char;
	/* clear area */
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < 10 ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < 50 ; j ++ )
		{
			/* show */
			win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox + stx , pos_y + oy + i ,color);
		}
	}	
	/* check */
	if( color == COLOR_BACK_GROUND)
	{
		int len = strlen(scale);
		/* reset the stx */
		ox = ( win->msg.x_size - len * 8 ) / 2;
	}
	/* check */
	while( *scale!= 0 )
	{
		/* check */
		if( *scale == '0' )
		{
			set_char = gImage_0;
		}
		else if( *scale == '1' )
		{
			set_char = gImage_1;
		}			
		else if( *scale == '2' )
		{
			set_char = gImage_2;
		}
		else if( *scale == '5' )
		{
			set_char = gImage_5;
		}
		else if( *scale == 'm' )
		{
			set_char = gImage_mv;
			type = 1;
		}
		else if( *scale == 'V' )
		{
			set_char = gImage_v;
			type = 2;
		}
		else
		{
			set_char = gImage_0;
		}		
		/* show */
		if( type == 0 )
		{
			/* 0 , 1 , 2 , 5 */
			for( int i = 0 ; i < 10 ; i ++ )
			{
				/* unsigned char */
				for( int j = 0 ; j < 8 ; j ++ )
				{
					/* show */
					if( !((set_char[i] << j ) & 0x80) )
					{
						win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox + stx , pos_y + oy + i ,COLOR_CHAR);
					}
				}
			}		
		}
		else if( type == 1 )
		{
			/* 0 , 1 , 2 , 5 */
			for( int i = 0 ; i < 10 ; i ++ )
			{
				/* unsigned char */
				for( int j = 0 ; j < 22 ; j ++ )
				{
					/* show */
					if( !(( set_char[i*3+ j / 8] << (j % 8)) & 0x80))
					{
						win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox + stx , pos_y + oy + i ,COLOR_CHAR);
					}
				}
			}
			/* over */
      break;			
		}
		else if( type == 2 )
		{
			/* 0 , 1 , 2 , 5 */
			for( int i = 0 ; i < 10 ; i ++ )
			{
				/* unsigned char */
				for( int j = 0 ; j < 10 ; j ++ )
				{
					/* show */
					if( !(( set_char[i*2+ j / 8] << (j % 8)) & 0x80 ))
					{
						win->dev->set_point(&win->msg.mark_flag,pos_x + j + ox + stx , pos_y + oy + i ,COLOR_CHAR);
					}
				}
			}
			/* over */
      break;			
		}	
		/* ++ */
		stx += 8;
		scale++;			
	}
}
/* dc ac */
void draw_dcac(struct widget * win,unsigned char daca)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;		
	/* check */
	if( daca == 0 )
	{
		/* dc */
		for( int i = 0 ; i < 9 ; i ++ )		
		{
			for( int j = 0 ; j < 8 ; j ++ )
			{
				if( ( char_d[i] << j ) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 7 + i ,COLOR_CHAR);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 7 + i ,COLOR_BUTTON);
				}
				/* set */
				if( ( char_c[i] << j ) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 1 + 8 , pos_y + 7 + i ,COLOR_CHAR);
				}	
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 1 + 8 , pos_y + 7 + i ,COLOR_BUTTON);
				}
			}
		}	
	}
	else
	{
		/* ac */
		for( int i = 0 ; i < 9 ; i ++ )		
		{
			for( int j = 0 ; j < 8 ; j ++ )
			{
				if( ( char_a[i] << j ) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 7 + i ,COLOR_CHAR);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 7 + i ,COLOR_BUTTON);
				}
				/* set */
				if( ( char_c[i] << j ) & 0x80 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 1 + 8 , pos_y + 7 + i ,COLOR_CHAR);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 + 1 + 8 , pos_y + 7 + i ,COLOR_BUTTON);
				}
			}
		}				
	}
}
/* dc ac */
void draw_trig_source(struct widget * win,unsigned char chn)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;		
	/* check */
	if( chn == 0 )
	{
		/* ch1 */
		for( int i = 0 ; i < 16 ; i ++ )		
		{
			for( int j = 0 ; j < 29 ; j ++ )
			{
				if( gImage_tch1[i*29+j] == 0xD8 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 21 + i ,COLOR_CH1_G);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 21 + i ,COLOR_BACK_GROUND);
				}
			}
		}	
	}
	else
	{
		/* ch2 */
		for( int i = 0 ; i < 16 ; i ++ )		
		{
			for( int j = 0 ; j < 29 ; j ++ )
			{
				if( gImage_tch2[i*29+j] == 0xD8 )
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 21 + i ,COLOR_CH2_G);
				}
				else
				{
					win->dev->set_point(&win->msg.mark_flag,pos_x + j + 42 , pos_y + 21 + i ,COLOR_BACK_GROUND);
				}
			}
		}				
	}
}
/* show languase */
void osc_redraw_languse(widget_def * win)
{
	/* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;	
	/* show title */
	unsigned short px = pos_x + 15;
	unsigned short py = pos_y + 17;
	/* show if( osc_language_get() < 2) */
	const char * tmpch = osc_language_get() == 0 ? win->msg_response : win->msg.pri_data;
	/* clear area */
	/* clear 	if( win->msg.upd == COLOR_BUTTON || win->msg.upd == COLOR_BTN_CTRL ) */
	unsigned char color;
	/* -- */
	if( win->msg.upd == COLOR_BTN_CTRL )
	{
		color = COLOR_BTN_CTRL;
	}
	else
	{
		color = COLOR_BACK_GROUND;
	}	
	/* set df */
	for( int i = 2 ; i < win->msg.x_size - 4; i ++ )
	{
		/* low */
		for( int j = 15 ; j < 60 ; j ++ )
		{
			win->dev->set_noload_point(pos_x + i , pos_y + j , color);
		}
	}
	/* check */
	if( osc_language_get() == 0 )
	{
		/* get len */
		unsigned int slen = strlen(tmpch);
		/* get */
		if( strstr(tmpch,"\n") == NULL )
		{
			px = pos_x + ( win->msg.x_size - slen * 8 ) / 2 ;
			py = pos_y + ( win->msg.y_size - 24 ) / 2 ;
		}
		else
		{
			px = pos_x + ( win->msg.x_size - 7 * 8 ) / 2 ;
			py = pos_y + ( win->msg.y_size - 40 ) / 2 ;
		}
		/* show */
		osc_l1_string(win->dev,px,py,(char *)tmpch,COLOR_CHAR,0,0);
	}
	else
	{	
		/* shoow */
		draw_hz(win->dev,(unsigned char *)&tmpch[0],px,py,COLOR_CHAR,0,0);
		draw_hz(win->dev,(unsigned char *)&tmpch[2],px + 16 + 5,py,COLOR_CHAR,0,0);
		draw_hz(win->dev,(unsigned char *)&tmpch[4],px,py + 16 + 9,COLOR_CHAR,0,0);
		draw_hz(win->dev,(unsigned char *)&tmpch[6],px + 16 + 5,py + 16 + 9,COLOR_CHAR,0,0);		
	}	
}
/* draw wroup wid */
static void draw_group_wid(struct widget * win)
{
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
  /* calutie and ending pos */
	unsigned short  pos_x = win->msg.x + win->parent->msg.x, pos_y = win->msg.y + win->parent->msg.y;
  /* calutie and ending pos */
	unsigned short pos_x_end = win->msg.x_size , pos_end_y = win->msg.y_size;
	/* color */
	unsigned short color_button;
	unsigned short color_line;
	/* unsigned char */
	unsigned char back_flag = 1;
	/* start */
	static unsigned char ctrl_redraw_flag = 0;
	/* get color */
	if( win->msg.upd == COLOR_BUTTON )
	{
		color_button = COLOR_BG_BTN;
		color_line = COLOR_BTN_LINE_NEW;
		back_flag = 0;
	}
	else if( win->msg.upd == COLOR_CH1 )
	{
		color_button = COLOR_BUTTON;
		color_line = COLOR_BUTTON;
	}
	else if( win->msg.upd == COLOR_CH2 )
	{
		color_button = COLOR_BUTTON;
		color_line = COLOR_BUTTON;
	}
	else if( win->msg.upd == COLOR_TRIG_G )
	{
		color_button = COLOR_BUTTON;
		color_line = COLOR_BUTTON;		
	}
	else if( win->msg.upd == COLOR_SYS )
	{
		color_button = COLOR_BUTTON;
		color_line = COLOR_BUTTON;		
	}
	else if( win->msg.upd == COLOR_BTN_CTRL )
	{
		/* check */
		if( ctrl_redraw_flag )
		{
			return;
		}		
		color_button = COLOR_BTN_CTRL;
		color_line = COLOR_BTN_LINE_NEW;		
		ctrl_redraw_flag = 1;
	}
	else if( win->msg.upd == COLOR_CTRL_CH1)
	{
		color_button = COLOR_BG_BTN;
		color_line = COLOR_BTN_LINE_NEW;			
		back_flag = 0;		
	}
	else if( win->msg.upd == COLOR_CTRL_CH2 )
	{
		color_button = COLOR_BG_BTN;
		color_line = COLOR_BTN_LINE_NEW;		
		back_flag = 0;
	}
	else
	{
		color_button = COLOR_BUTTON;
		color_line = COLOR_BUTTON;			
	}
	/* check */
	if( back_flag )
	{
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
					win->dev->set_noload_point( j + pos_x + 1 ,pos_y + i + 1,color_button);
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
			win->dev->set_noload_point( i % 4 + pos_x ,i / 4 + pos_y,color_line);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,color_line);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,color_line);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			win->dev->set_noload_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,color_line);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		win->dev->set_noload_point(pos_x + 4 + i , pos_y , color_line);
		win->dev->set_noload_point(pos_x + 4 + i , pos_y + pos_end_y - 1, color_line);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		win->dev->set_noload_point(pos_x , pos_y + 4 + i , color_line);
		win->dev->set_noload_point(pos_x + pos_x_end - 1, pos_y + 4 + i , color_line);
	}
	/* show title */
	if( win->msg.upd == COLOR_BUTTON || win->msg.upd == COLOR_BTN_CTRL )
	{
		/* show title */
		unsigned short px = pos_x + 15;
		unsigned short py = pos_y + 17;
		/* show if( osc_language_get() < 2) */
		const char * tmpch = osc_language_get() == 0 ? win->msg_response : win->msg.pri_data;
		/* check */
		if( osc_language_get() == 0 )
		{
			/* get len */
	    unsigned int slen = strlen(tmpch);
			/* get */
			if( strstr(tmpch,"\n") == NULL )
			{
				px = pos_x + ( win->msg.x_size - slen * 8 ) / 2 ;
				py = pos_y + ( win->msg.y_size - 24 ) / 2 ;
			}
			else
			{
				px = pos_x + ( win->msg.x_size - 7 * 8 ) / 2 ;
				py = pos_y + ( win->msg.y_size - 40 ) / 2 ;
			}
			/* show */
			osc_l1_string(win->dev,px,py,(char *)tmpch,COLOR_CHAR,0,0);
		}
		else
		{
			/* shoow */
			draw_hz(win->dev,(unsigned char *)&tmpch[0],px,py,COLOR_CHAR,0,0);
			draw_hz(win->dev,(unsigned char *)&tmpch[2],px + 16 + 5,py,COLOR_CHAR,0,0);
			draw_hz(win->dev,(unsigned char *)&tmpch[4],px,py + 16 + 9,COLOR_CHAR,0,0);
			draw_hz(win->dev,(unsigned char *)&tmpch[6],px + 16 + 5,py + 16 + 9,COLOR_CHAR,0,0);		
		}
	}
	else if( win->msg.upd == COLOR_CH1 )
	{
		/* line left and right */
		for( int i = 0 ; i < 32 ; i ++ )
		{
			for( int j = 0 ; j < 31 ; j ++ )
			{
				win->dev->set_noload_point(pos_x + j + 5 , pos_y + 5 + i , gImage_CH1[i*31+j] == 0x6C ? COLOR_CH1_G : COLOR_BACK_GROUND);
			}
		}	
	  /* draw x10 */		
		draw_x1_x10(win,1);
		/* DC */
		draw_dcac(win,1);
		/* scale */
		//draw_vol_scale(win,"500mV");
	}
	else if( win->msg.upd == COLOR_CH2 )
	{
		/* line left and right */
		for( int i = 0 ; i < 32 ; i ++ )
		{
			for( int j = 0 ; j < 31 ; j ++ )
			{
				win->dev->set_noload_point(pos_x + j + 5 , pos_y + 5 + i , gImage_CH2[i*31+j] == 0x0D ? COLOR_CH2_G : COLOR_BACK_GROUND);
			}
		}
		/* draw x10 */
		draw_x1_x10(win,1);
		/* DC */
		draw_dcac(win,0);	
		/* scale */
		//draw_vol_scale(win,"10V");		
	}
	else if( win->msg.upd == COLOR_TRIG_G )
	{
				/* line left and right */
		for( int i = 0 ; i < 32 ; i ++ )
		{
			for( int j = 0 ; j < 31 ; j ++ )
			{
				win->dev->set_noload_point(pos_x + j + 5 , pos_y + 5 + i , gImage_tt[i*31+j] == 0x10 ? COLOR_TRIG_G : COLOR_BACK_GROUND);
			}
		}		
		/* default trig */
		draw_trig_source(win,1);
		/* trig source */
		draw_trig_type(win,0);
		/* trig icon */
		draw_trig_icon(win,0);
		/* show t'd */
		draw_trig_td(win,1);
	}	
	else if( win->msg.upd == COLOR_SYS )
	{
		/* line left and right */
		for( int i = 0 ; i < 32 ; i ++ )
		{
			for( int j = 0 ; j < 31 ; j ++ )
			{
				win->dev->set_noload_point(pos_x + j + 5 , pos_y + 5 + i , gImage_sys[i*31+j] == 0x85 ? COLOR_SYS : COLOR_BACK_GROUND);
			}
		}
		/* show system settings */
		if( osc_format() == 0 )
		{
			draw_sys_settings(win,0,3);
		}
		else
	  {
			draw_sys_settings(win,0,2);
		}
		/* check */
		if( osc_math_item() )	
		{
			/* get */
			unsigned char ind = osc_math_item() & 0x3;
			/* set */
			draw_sys_settings(win,2,5 + ind);
		}			
		//draw_sys_settings(win,1,0);
		//draw_sys_settings(win,2,5);
		osc_run_msg_def * runmsg = get_run_msg();
		if( osc_param_sys_get(runmsg->sys_menu_set,16) )
		{
			draw_sys_settings(win,1,10);
		}
		else if( osc_param_sys_get(runmsg->sys_menu_set,17) )
		{
			draw_sys_settings(win,1,11);
		}
		else if( osc_param_sys_get(runmsg->sys_menu_set,18) )
		{
			draw_sys_settings(win,1,12);
		}
		else if( osc_smartAI() == 0 )
		{
			draw_sys_settings(win,1,9);
		}
		/* check FFT */
		unsigned char fft_t = osc_fft_sta() & 0x3;
		/* check */
		if( fft_t != 0 )
		{
			draw_sys_settings(win,3,fft_t - 1);
		}
		//draw_sys_settings(win,3,9);
		/*----------------------*/
	}
	else if( ( win->msg.upd == COLOR_CTRL_CH1 || win->msg.upd == COLOR_CTRL_CH2 ) && ( win->msg.mx == 0 ))
	{
		/* show title */
		unsigned short px = pos_x + 17;
		unsigned short py = pos_y + 17;
		/* show */
		unsigned char * tmpch = win->msg.pri_data;
		/* shoow */
		gui_char(win->dev,px,py,tmpch[0],16,COLOR_CHAR,COLOR_BACK_GROUND,1);		
		gui_char(win->dev,px + 12,py,tmpch[1],16,COLOR_CHAR,COLOR_BACK_GROUND,1);	
		gui_char(win->dev,px + 24,py,tmpch[2],16,COLOR_CHAR,COLOR_BACK_GROUND,1);	
		gui_char(win->dev,px + 9,py + 20,tmpch[3],24,COLOR_CHAR,COLOR_BACK_GROUND,1);	
	}
	else
	{
		
	}	
	/* check */
	if( win->msg.mx == 1 )
	{
		draw_vol_scale(win,"500mV");
	}
	else if( win->msg.mx == 2 )
	{
		draw_vol_scale(win,"200mV");
	}
	else
	{
		
	}
}
#if 0
/* draw the menu window */
static void draw_menu_win(window_def * win)
{
	/* transfer the menu size */
	unsigned short MENU_WIDTH = win->msg.x_size;//107;
	unsigned short MENU_HEIGHT = win->msg.y_size;//430;
	/* start pos */
	unsigned short pos_x_m = win->msg.x; //dev->width - MENU_WIDTH;
	unsigned short pos_y_m = win->msg.y; //0
	 /*fill the rect */
#if HARDWARE_ACCEL_SUPPLY
  /* fill rect */
	win->dev->fill_rect( pos_x_m , pos_y_m , 
	                pos_x_m + MENU_WIDTH - 1 , pos_y_m + MENU_HEIGHT - 1,
	                COLOR_BACK_GROUND );//background color
#else
	/* draw points one by one */
	for (int i = 0; i < MENU_WIDTH; i++)
	{
		for (int j = 0; j < MENU_HEIGHT; j++)
		{
			win->dev->set_point(&win->msg.mark_flag,pos_x_m + i , pos_y_m + j, COLOR_BACK_GROUND);
		}
	}	
#endif
	/* top line */
	for (int i = 0; i < MENU_WIDTH; i++)
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + 1, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + 2, COLOR_MENU_ONE);
    /* bottom line */
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + MENU_HEIGHT - 1, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + MENU_HEIGHT - 2 , COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + MENU_HEIGHT - 3, COLOR_MENU_ONE);
	}
	/* left lines */
	for (int i = 0; i < MENU_HEIGHT - 6; i++)
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x_m, pos_y_m + i + 3, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + 1, pos_y_m + i + 3, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + 2, pos_y_m + i + 3, COLOR_MENU_ONE);
	}
}
#endif
#if 0
/* create icon */
static void osc_create_TITLE(window_def * win)
{
	/* color tmp */
	unsigned char * color = (unsigned char *)gImage_gt;
	/* create */
	for( int i = 0 ; i < 42 ; i ++ )
	{
		for( int j = 0 ; j < 129 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			if( color[i*129+j] == 0x24 )
			{
				win->dev->set_point(&win->msg.mark_flag,j , i , COLOR_TITLE_BG);
			}
			else if( color[i*129+j] == 0xff )
			{
				win->dev->set_point(&win->msg.mark_flag,j , i , COLOR_TITLE_BTN);
			}
			else
			{
				win->dev->set_point(&win->msg.mark_flag,j , i , COLOR_TITLE_BTN);
			}
#else
			unsigned short tm = color[i*32+j];

			win->dev->set_point(&win->msg.mark_flag,parent_x + pos_x + j , parent_y + pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}
}
#endif
/* draw offset arrow */
static void osc_draw_offset_arrow(gui_dev_def * dev,unsigned short pos_x,unsigned short pos_y,unsigned short chn)
{
	/* color table */
	const unsigned char ch_common_table[16] = {0x01,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x1f,0x1f,0x0f,0x07,0x01};
	const unsigned char chn1_tab[10] = {0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x00};
	const unsigned char chn2_tab[10] = {0x78,0xFC,0x8C,0x0C,0x1C,0x38,0x70,0xE0,0xfC,0xFC};
	/* chn */
	unsigned short color = ( chn == 1 ) ? COLOR_CH1_G : COLOR_CH2_G;
	const unsigned char * chnn = ( chn == 1 ) ? chn1_tab : chn2_tab;
	/*-------------*/
	for( int i = 0 ; i < 16 ; i ++ )
	{
		for( int j = 0 ; j < 32 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			if( j < 8 )
			{
				if( ( ch_common_table[i] << j ) & 0x80 )
				{
					dev->set_noload_point( pos_x + j , pos_y + i , color);
				}
			}
			else if( j >= 24 )
			{
       if( ( ch_common_table[i] >> (j-24) ) & 0x01 )
			 {
				 dev->set_noload_point( pos_x + j , pos_y + i , color);
			 }
			}
		  else
			{
				dev->set_noload_point( pos_x + j , pos_y + i , color);
			}			
		  /* 1 or 2 */
			if( i >= 3 && i < 13)
			{
	       if( j >= 13 && j < 21 )
				 {
					 if( (chnn[i-3] << (j-13)) & 0x80 )
					 {
						 dev->set_noload_point( pos_x + j , pos_y + i , COLOR_GRID_AREA_BG);
					 }
				 }					 
			}
#else
			unsigned short tm = color[i*32+j];

			widget->dev->set_noload_point(parent_x + pos_x + j , parent_y + pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}
}
/* create icon */
static void osc_create_chn_icon(window_def * parent_win,unsigned short x,unsigned short y,unsigned short chn)
{
	/* widget pos */
	unsigned short pos_x = x + parent_win->msg.x;
	unsigned short pos_y = y + parent_win->msg.y;
	/* create */
  osc_draw_offset_arrow(parent_win->dev,pos_x,pos_y,chn);
	/* end if */
}
#if 0
/* create button */
static void osc_create_button(struct widget * widget)
{
	/* create btn */
	/* define the btn size */
  unsigned short MENU_BTN_WIDTH = widget->msg.x_size;//102;
	unsigned short MENU_BTN_HEIGHT = widget->msg.y_size;//79;
	/* define btn start pos */
	unsigned short pos_btn_x = widget->msg.x ;//5
	unsigned short pos_btn_y = widget->msg.y ;//24;
	/* parent win size */
	unsigned short pos_x_m = widget->parent->msg.x;
	unsigned short pos_y_m = widget->parent->msg.y;
	/* parent win size */
#if 0
	unsigned short parent_size_x = widget->parent->msg.x_size;
	unsigned short parent_size_y = widget->parent->msg.y_size;
#endif
	/* create the background */
#if HARDWARE_ACCEL_SUPPLY
 /* fill rect */
	widget->dev->fill_rect( pos_x_m + pos_btn_x + 2 , pos_y_m + pos_btn_y + 2 , 
	                pos_x_m + pos_btn_x + 2 + MENU_BTN_WIDTH - 1 - 2,
					        pos_y_m + pos_btn_y + MENU_BTN_HEIGHT + 2 - 1 - 4,
	                COLOR_BUTTON );
#else
	/* create one by one */
	for (int i = 0; i < MENU_BTN_WIDTH - 2; i++)
	{
		for (int j = 0; j < MENU_BTN_HEIGHT - 4; j++)
		{
			widget->dev->set_noload_point(pos_x_m + pos_btn_x + 2 + i, pos_y_m + pos_btn_y + j + 2, COLOR_BUTTON);
		}
	}
#endif
}
#endif
/* create win_main_pos */
void osc_calculate_main_size(gui_dev_def * dev,window_def * win,unsigned short wf)
{
	/* create the win */
	win->msg.x = 0;
	win->msg.y = 0;
	win->msg.x_size = dev->width;
	win->msg.y_size = dev->height;
	win->dev = dev;
	/* set callback */
	win->draw = create_grid_data;
	/* wf */
	win->msg.wflags = wf | 0xE0;
	/* ------- */
	/* create win creater */
	gui_win_creater(win);	
}
/* static cal the osc grid fast fast */
static int osc_grid_fast(gui_dev_def * tdev,unsigned short * total_ph,unsigned short * total_pv)
{
	/* add right pixel */
	unsigned short twidth = tdev->width - RIGHT_REMAIN_PIXEL;
	/* vertical */
	unsigned short em_t = ( tdev->height - TOP_REMAIN_PIXEL - 2 - BOTTOM_REMAIN_PIXEL ) / (VERTICAL_GRID_TOTAL);
	/* horizonal */
	unsigned short en_t = ( twidth - LEFT_REMAIN_PIXEL - 2 ) / ( HORIZONTAL_GRID_TOTAL );
	
	/* if */
	if( !em_t || !en_t )
	{
		return FS_ERR;
	}
	/* cal hori */
	if( total_ph != 0 )
	{
		*total_ph = en_t * HORIZONTAL_GRID_NUM * LITTLE_GRIG_NUM;
	}
	/* vertical */
	if( total_pv != 0 )
	{
		*total_pv = em_t * VERTICAL_GRID_NUM * LITTLE_GRIG_NUM;
	}
	/* return ok */
	return FS_OK;
}
/* static group pos */
void osc_calculate_sg_size(gui_dev_def * dev,window_def * win0,unsigned int num)
{
	/* def*/
	unsigned short vertical_pixel_total;
	/* get msg */
	if( osc_grid_fast(dev,0,&vertical_pixel_total) == FS_ERR )
	{
		/* creater fail */
		return;
	}
	/* bad data */
	if( num < 4 )
	{
		return;
	}
	/* create the win */
	win0[0].msg.x = dev->width - 450 - 1 ;
	win0[0].msg.y = vertical_pixel_total + TOP_REMAIN_PIXEL + 2 + 5;
	win0[0].msg.x_size = 450;
	win0[0].msg.y_size = dev->height - vertical_pixel_total - TOP_REMAIN_PIXEL - 2 - 10;
	win0[0].dev = dev;
	win0[0].msg.wflags = 0x0100 | 0xC0;
	/* set callback */
	win0[0].draw = draw_group_win; 
	/* create the group */
	win0[1].msg.x = 1;//win0[0].msg.x + win0[0].msg.x_size + 2;
	win0[1].msg.y = win0[0].msg.y;
	win0[1].msg.x_size = ( dev->width - win0[0].msg.x_size ) * 2 / 3;
	win0[1].msg.y_size = (dev->height - vertical_pixel_total - TOP_REMAIN_PIXEL - 2 - 8) / 2 - 1;
	win0[1].dev = dev;
	win0[1].msg.wflags = 0x0200 | 0xC0;
	/* set callback */
	win0[1].draw = draw_group_win;
	/* create the group */
	win0[2].msg.x = win0[1].msg.x;
	win0[2].msg.y = win0[1].msg.y + win0[1].msg.y_size + 1;
	win0[2].msg.x_size = dev->width - win0[0].msg.x_size - 5;
	win0[2].msg.y_size = win0[1].msg.y_size - 1;
	win0[2].dev = dev;
	win0[2].msg.wflags = 0x0400 | 0xC0;
	/* set callback */
	win0[2].draw = draw_group_win;
	/* create the group */
	win0[3].msg.x = win0[1].msg.x + win0[1].msg.x_size + 2;
	win0[3].msg.y = win0[1].msg.y;
	win0[3].msg.x_size = dev->width - win0[0].msg.x_size - win0[1].msg.x_size - 6;
	win0[3].msg.y_size = win0[1].msg.y_size;
	win0[3].dev = dev;
	win0[3].msg.wflags = 0x0800 | 0xC0;
	/* set callback */
	win0[3].draw = draw_group_win;
  /* ok create the win */
	gui_win_creater(&win0[0]);
	gui_win_creater(&win0[1]);
  gui_win_creater(&win0[2]);
  gui_win_creater(&win0[3]);	
}
/* create win_menu_pos */
void osc_calculate_menu_size(gui_dev_def * dev,window_def * win,unsigned short wf)
{
	/* def*/
	unsigned short vertical_pixel_total;
	/* get msg */
	if( osc_grid_fast(dev,0,&vertical_pixel_total) == FS_ERR )
	{
		/* creater fail */
		return;
	}	
	/* create the win */
	win->msg.x_size = 70;
	win->msg.y_size = 480;
	win->msg.x = dev->width - 70;
	win->msg.y = 0;
	win->dev = dev;
	/* set wflags d*/
	win->msg.wflags = wf | 0xE0;
	/* set callback */
	win->draw = 0;
	/* ok */
	/* create */
	gui_win_creater(win);		
}
/* change button color */
void osc_btn_color(window_def * win,widget_def *wd,unsigned char mode)
{
	/* cdte */
	gui_dev_def * dev = wd->dev;
	/* transfer to abs postion */
	unsigned short abpx = win->msg.x + wd->msg.x;
	unsigned short abpy = win->msg.y + wd->msg.y;
	/* start flag */
	unsigned char sfg = 0;
	/* change color */
	for( int i = abpx ; i < abpx + wd->msg.x_size ; i ++ )
	{
		/* clear */
		sfg = 0;
		/* next */
		for( int j = abpy ; j < abpy + wd->msg.y_size ; j ++ )
		{
			/* color */
			unsigned char tcoler = dev->read_point(i,j);
			/* check */
			if( tcoler == COLOR_BTN_LINE_NEW )
			{
				sfg = 1;
			}
			/* check end */
			if( sfg == 1 && tcoler == COLOR_BACK_GROUND )
			{
				/* check */
				if( ( j - abpy ) > wd->msg.y_size / 2 )
				{
					if( dev->read_point(i,j-1) == COLOR_BTN_LINE_NEW)
					{
						/* clear */
						sfg = 0;							
					}
				}			
			}
			/* check */
			if( sfg == 0 )
			{
				continue;
			}
			/* tes */
			if( mode == 1 )
			{
				if( tcoler == COLOR_BACK_GROUND )
				{
					dev->set_noload_point(i,j,COLOR_BG_FUCUS_BTN);
				}
				else if( tcoler == COLOR_BTN_CTRL )
				{
					dev->set_noload_point(i,j,COLOR_BG_FUCUS_CTRL);
				}
				/* set backgroud color */
				wd->msg.my= COLOR_BG_FUCUS_BTN;
				/*---------------------*/
		  }
			else
			{
				if( tcoler == COLOR_BG_FUCUS_BTN )
				{
					dev->set_noload_point(i,j,COLOR_BACK_GROUND);
				}
				else if( tcoler == COLOR_BG_FUCUS_CTRL )
				{
					dev->set_noload_point(i,j,COLOR_BTN_CTRL);
				}
				/* set backgroud color */
				wd->msg.my = COLOR_BACK_GROUND;
				/*---------------------*/				
			}
		}
	}
}
/* change button color */
void osc_top_color(widget_def *wd,unsigned char mode)
{
	/* cdte */
	gui_dev_def * dev = wd->dev;
	/* transfer to abs postion */
	unsigned short abpx = wd->msg.x;
	unsigned short abpy = wd->msg.y;
	/* check */
	if( mode )
	{
		if( wd->msg.upd == COLOR_BTN_FOCUS )
		{
			return;
		}
	}
	else
	{
		if( wd->msg.upd == COLOR_BUTTON )
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
			unsigned char tcoler = dev->read_point(i,j);
			/* tes */
			if( mode == 1 )
			{
				if( tcoler == COLOR_BUTTON )
				{
					dev->set_noload_point(i,j,COLOR_BTN_FOCUS);
				}
				/* set backgroud color */
				wd->msg.upd = COLOR_BTN_FOCUS;
				/*---------------------*/
		  }
			else
			{
				if( tcoler == COLOR_BTN_FOCUS )
				{
					dev->set_noload_point(i,j,COLOR_BUTTON);
				}	
				/* set backgroud color */
				wd->msg.upd = COLOR_BUTTON;
				/*---------------------*/				
			}
		}
	}
}
/* create win_btn_pos */
void osc_calculate_ctrl_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,char ** title,char ** title_eng,unsigned short btn_num)
{
	/* button ypos */
	unsigned short btn_tpos = 3;
	/* calculate x size and y size */
	unsigned short x_size_g = win->msg.x_size - 4;
	unsigned short y_size_g = win->msg.y_size / 6 - 4;
	/* unsigned short */
	unsigned short y_st = y_size_g;
	/* create btn */
	for( int i = 0 ; i < btn_num ; i ++ )
	{
		/* check */
		if( i == 2 || i == 5 )
		{
			y_st = 24;
		}
		else
		{
			y_st = y_size_g;
		}		
		/* set det */
		wd[i].msg.x = 2;
		wd[i].msg.y = btn_tpos;
		wd[i].msg.x_size = x_size_g;
		wd[i].msg.y_size = y_st;
		wd[i].dev = dev;
		wd[i].draw = draw_group_wid;
		wd[i].parent = win;
		wd[i].msg_response = title_eng[i];
		/* check */
		if( i == 0 )
		{
			wd[i].msg.upd = COLOR_BTN_CTRL;
		}
		else if( i == 1 )
		{
			wd[i].msg.upd = COLOR_CTRL_CH1;
		}
		else if( i == 2 )
		{
			wd[i].msg.upd = COLOR_CTRL_CH1;
			wd[i].msg.mx = 1;
		}
		else if( i == 3 )
		{
			wd[i].msg.upd = COLOR_CTRL_CH1;
		}
		else if( i == 4 )
		{
			wd[i].msg.upd = COLOR_CTRL_CH2;
		}		
		else if( i == 5 )
		{
			wd[i].msg.upd = COLOR_CTRL_CH2;
			wd[i].msg.mx = 2;
		}
		else
		{
			wd[i].msg.upd = COLOR_CTRL_CH2;
		}
		/* title */
		wd[i].msg.pri_data = title[i];
		/* create btn */
		if( i != 0 )
		{
			gui_widget_creater(&wd[i]);
		}
		/* calculate the y size */
		btn_tpos += y_st + 3;
	}
}
/* create win_btn_pos */
void osc_calculate_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,char ** title,char ** eng_title,unsigned short btn_num)
{
	/* button ypos */
	unsigned short btn_tpos = 0;
	/* calculate x size and y size */
	unsigned short x_size_g = win->msg.x_size - 4;
	unsigned short y_size_g = win->msg.y_size / btn_num - 4;
	/* create btn */
	for( int i = 0 ; i < btn_num ; i ++ )
	{
		/* calculate the y size */
		btn_tpos = ( y_size_g + 3 ) * i + 3;
		/* set det */
		wd[i].msg.x = 2;
		wd[i].msg.y = btn_tpos;
		wd[i].msg.x_size = x_size_g;
		wd[i].msg.y_size = y_size_g;
		wd[i].dev = dev;
		wd[i].draw = draw_group_wid;
		wd[i].parent = win;
		wd[i].msg_response = eng_title[i];
		/* check */
		if( i == 0 )
		{
			wd[i].msg.upd = COLOR_BTN_CTRL;
		}
		else
		{
			wd[i].msg.upd = COLOR_BUTTON;
		}
		/* title */
		wd[i].msg.pri_data = title[i];
		/* create btn */
		gui_widget_creater(&wd[i]);
	}
}
static void draw_bat_icon(struct widget * win)
{
	/* create */
	unsigned short start_px = win->msg.x;
	unsigned short start_py = win->msg.y;
	/* table */
	for( int i = 0 ; i < win->msg.y_size ; i ++ )
	{
		/* lines */
		for( int j = 0 ; j < win->msg.x_size ; j ++ )
		{
			/* if */
			if( ( bat_icon_shape[i] << j ) & 0x8000 )
			{
				win->dev->set_noload_point( start_px + j , start_py + i , COLOR_GRID_AREA_BG);
			}
			else
			{
				win->dev->set_noload_point( start_px + j , start_py + i , COLOR_CHAR);
			}
		}
	}
	/* set */
	osc_ui_draw_bat(win,2);
}
/* draw */
void osc_ui_draw_bat(struct widget * win,unsigned char level)
{
	unsigned short pox = win->msg.x + 2;
	unsigned short poy = win->msg.y + 6 + 4*5;
	/* color */
	unsigned short color;
	/* clear */
	for( int i = 0 ; i < 4 ; i ++ )
	{
		/* level */
		for( int j = 0 ; j < 4 ; j ++ )
		{
			/* dfe */
			win->dev->set_noload_point(  win->msg.x + 6 + j , win->msg.y + 2 + i , COLOR_CHAR );
		}
	}	
	/* check */
	if( level == 1 )
	{
		color = COLOR_BAT_LOW;
	}
	else if( level == 2 )
	{
		color = COLOR_BAT_HALF;
	}
	else
	{
		color = COLOR_BAT_CAP;
	}
	/* draw */
	for( int i = 0 ; i < 5 ; i ++ )
	{
		/* level */
		for( int j = 0 ; j < 5 ; j ++ )
		{
			/* dfe */
			for( int t = 0 ; t < 12 ; t ++ )
			{
				win->dev->set_noload_point( pox + t , poy + j , ( level > i ) ? color : COLOR_CHAR);
			}
		}
		/* ins */
		poy -= 5;
	}
	if( level == 5 )
	{
		/* clear */
		for( int i = 0 ; i < 4 ; i ++ )
		{
			/* level */
			for( int j = 0 ; j < 4 ; j ++ )
			{
				/* dfe */
				win->dev->set_noload_point(  win->msg.x + 6 + j , win->msg.y + 2 + i , COLOR_BAT_CAP );
			}
		}		
  }
}
/* show */
void osc_ui_measure_bat(struct widget * win)
{
//	const unsigned char enable_bb[10] = {0,0,0,0,0,1,1,1,1,1};
//	/* static */
//	static unsigned char inde = 0;
//	/* chn */
//	draw_measure_item(win,enable_bb[inde],inde);
//	
//	inde ++;
}
/* create measure item */
void osc_calculate_measure_item(gui_dev_def * dev,window_def * win,widget_def *wd)
{
	/* check */
	for( int i = 0 ; i < 10 ; i ++ )
	{	
		if( i < 5 )
		{
			/* set det */
			wd[i].msg.x = 20 + ( 137 + 4 ) * i;
			/* if */
			wd[i].msg.y = win->msg.y_size - 27;
		}
		else
		{
			/* set det */
			wd[i].msg.x = 20 + ( 137 + 4 ) * (i-5);
			/* if */
			wd[i].msg.y = win->msg.y_size - 27 - 27;			
		}
		wd[i].msg.x_size = 137;
		wd[i].msg.y_size = 25;
		wd[i].dev = dev;
		wd[i].draw = osc_ui_measure_bat;
		wd[i].parent = win;
		/* create btn */
		gui_widget_creater(&wd[i]);
	}
}
/* create win_bat_pos */
void osc_calculate_bat_icon_size(gui_dev_def * dev,window_def * win,widget_def *wd)
{
	/* set det */
	wd->msg.x = 700 + 4;
	wd->msg.y = 7;
	wd->msg.x_size = 16;
	wd->msg.y_size = 32;
	wd->dev = dev;
	wd->draw = draw_bat_icon;
	wd->parent = win;
	/* chn */
	wd->msg.upd = COLOR_CHAR;
	/* create btn */
	gui_widget_creater(wd);
	/* calculate the y size */
}
/* create win_btn_pos */
void osc_calculate_chn_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,unsigned short btn_num)
{
	/* button ypos */
	unsigned short btn_tpos = 129 + 16 + 32 + 16;
	/* calculate x size and y size */
	unsigned short x_size_g = 120;//129 + 16 + 32 + 5;
	unsigned short y_size_g = 42;
	/* create btn */
	for( int i = 0 ; i < btn_num ; i ++ )
	{
		/* set det */
		wd[i].msg.x = btn_tpos;
		wd[i].msg.y = 2;
		wd[i].msg.x_size = x_size_g;
		wd[i].msg.y_size = y_size_g;
		wd[i].dev = dev;
		wd[i].draw = draw_group_wid;
		wd[i].parent = win;
		/* chn */
		if( i == 0 )
		{
			wd[i].msg.upd = COLOR_CH1;
		}
		else if ( i == 1 )
		{
			wd[i].msg.upd = COLOR_CH2;
		}
		else if ( i == 2 )
		{
			wd[i].msg.upd = COLOR_TRIG_G;
		}
		else if( i == 3 )
		{
			wd[i].msg.upd = COLOR_SYS;
			wd[i].msg.x = 4;
		}
		else
		{
			
		}
		/* create btn */
		gui_widget_creater(&wd[i]);
		/* calculate the y size */
		if( i == 1 )
		{
			btn_tpos += x_size_g + 20 + 100;
		}
		else
		{
			btn_tpos += x_size_g + 20 ;		
		}
	}
}
/* create win_btn_pos */
void osc_calculate_right_icon(gui_dev_def * dev,window_def * win,widget_def *btn_wd,widget_def * icon_wd,unsigned short btn_num)
{
	/* calculate x size and y size */
	unsigned short x_size_g = win->msg.x_size - 5;
	unsigned short y_size_g = (win->msg.y_size - 24 - 1 ) / btn_num - 2;
	/* create btn */
	for( int i = 0 ; i < btn_num ; i ++ )
	{
		/* set det */
		icon_wd[i].msg.x = btn_wd[i].msg.x + btn_wd[i].parent->msg.x + 3;;
		icon_wd[i].msg.y = btn_wd[i].msg.y + btn_wd[i].parent->msg.y + btn_wd->msg.y_size / 2 - 6;;
		icon_wd[i].msg.x_size = x_size_g;
		icon_wd[i].msg.y_size = y_size_g;
		icon_wd[i].dev = dev;
		icon_wd[i].draw = osc_draw_right_icon;
		icon_wd[i].parent = win;
		icon_wd[i].msg.upd = COLOR_BUTTON;
		icon_wd[i].msg.wflags = GUI_HIDE;
		/* create btn */
		gui_widget_creater(&icon_wd[i]);
	}
}
/* create the voltage title */
void osc_calculate_volage_string(window_def * pwin,widget_def *wd,int num,char * ch1,char * ch2)
{
	/* if num is not epue */
	if( num != 2 )
	{
		return;//reutrn,bad data
	}
	/* calbrate the pox */
	unsigned short ev = ( pwin->msg.y_size - 24/* char height */) / 2 ; 
	unsigned short eh = pwin->msg.x_size / 2 ;
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* default is 24 size */
		wd[i].msg.wflags |= 0x1000;
		/* channel */
		wd[i].msg.wflags |= ( 0x2000 << i );	
		/* set ch1 */
		wd[i].msg.x = eh * i + 32 + 5;
		wd[i].msg.y = ev;
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		if( i == 0 )
		{
		  wd[i].msg.pri_data = ch1;
		}
		else
		{
			wd[i].msg.pri_data = ch2;
		}
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create the voltage title */
void osc_calculate_time_string(window_def * pwin,widget_def *wd,int num,char * M,char * time)
{
	/* if num is not epue */
	if( num != 2 )
	{
		return;//reutrn,bad data
	}
	/* calbrate the pox */
	unsigned short ev = ( pwin->msg.y_size - 24/* char height */) / 2 ; 
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* default is 24 size */
		wd[i].msg.wflags |= 0x1000;
		/* set ch1 */
		wd[i].msg.x = 12 * i + i * 5 + 10;
		wd[i].msg.y = ev;
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		if( i == 0 )
		{
		  wd[i].msg.pri_data = M;
		}
		else
		{
			wd[i].msg.pri_data = time;
		}
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create the voltage title */
void osc_calculate_measure_ch(window_def * pwin,widget_def *wd,int num,char ** item,unsigned char ch)
{
	/* if num is not epue */
	if( num != 6 )
	{
		return;//reutrn,bad data
	}
	/* calbrate the pox */
	unsigned short eh = pwin->msg.x_size / 3;
	unsigned short v_start = ( pwin->msg.y_size - 16 * 2 ) / 3;
	/* strlen */
	int strl = 0;
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* default is 16 size */
		wd[i].msg.wflags &=~ 0x1000;
		/* channel */
		if( ch == 1 )
		{
			wd[i].msg.wflags |= 0x2000;
		}
		else
		{
			wd[i].msg.wflags |= 0x4000;
		}
		/* set ch1 */
		if( i % 2 )
		{
			wd[i].msg.x = eh * (i/2) + 32 + 16 * strl / 2 + 4;
		}
		else
		{
			wd[i].msg.x = eh * i / 2 + 32 + 2;
			/* get strlen len */
			strl = 6;//strlen(item[i]);
		}
		/* vertical */
		if( ch == 1 )
		{
			wd[i].msg.y = v_start;
		}
		else
		{
			wd[i].msg.y = v_start * 2 + 16;
		}
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		/* item */
		wd[i].msg.pri_data = item[i];
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create menu text size */
static int osc_hz_len(char * chd,int * enter)
{
	/* match \n or \0*/
	int i = 0;
	/* get */
	while(*chd != 0 )
	{
		if( *chd == '\n' )
		{
			/* en*/
			*enter = 1;
			/* return */
			return i;
		}
		/* next */
		i++;
		chd++;
	}
	/* reuturn */
	return i;
}
/* create menu text size */
void osc_calculate_menu(window_def * pwin,widget_def *wd,int num,char ** item)
{
	/* if num is not epue */
	if( num == 0 )
	{
		return;//reutrn,bad data
	}
	/* strlen */
	int strl = strlen(item[0]);
	/* font size */
	unsigned char size = (wd[0].msg.wflags & 0x1000) ? 24 : 16;
	/* calbrate the pox */
	unsigned short title_x = ( pwin->msg.x_size - strl / 2 * size ) / 2;
	unsigned short title_y = ( 24 - size ) / 2 + 3; 
	/* other */
	unsigned short eh = pwin->msg.x_size - 5;
	unsigned short v_start = ( pwin->msg.y_size - 24 - 1 ) / ( num - 1 ) - 2;
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* set system title */
		if( i == 0 )
		{
			wd[i].msg.x = title_x;
			/* vertical */
			wd[i].msg.y = title_y;
		  /* char default is 16 size */
		  wd[i].msg.wflags &=~ 0x1000;			
		}
		else
		{
			/* char default is 24 size */
			wd[i].msg.wflags |= 0x1000;
			/* get len */
			int enter_flag = 0;
			int lenc = osc_hz_len(item[i],&enter_flag);
			/* get len */
			wd[i].msg.x = ( eh - lenc / 2 * 16 ) / 2 + 5;
			/* calculate the y size */
      wd[i].msg.y = ( v_start + 2 ) * ( i - 1 ) + 24 + v_start / 2 - ( enter_flag ? 16 : 8 );
		}
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		/* item */
		wd[i].msg.pri_data = item[i];
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create msg group */
void osc_calculate_tips(window_def * pwin,widget_def *wd,unsigned short level,char * tip)
{
	/* calbrate the pox */
	unsigned short ev = ( pwin->msg.y_size - 16/* char height */) / 2 ; 
	/* setting */
	/* default is 16 size */
	wd->msg.wflags &=~ 0x1000;
	/* wd */
	wd->msg.wflags &=~ 0xE000;
	/* get color */
	wd->msg.wflags |= level;
	/* set ch1 */
	wd->msg.x = pwin->msg.x + 5;
	wd->msg.y = ev;
	/* not supply size now */
#if 0
	wd->msg.x_size = 0;
	wd->msg.y_size = 0;
#endif
	/* size */
	wd->msg.pri_data = tip;
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = gui_dynamic_string;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);
}
/* draw a chown */
void osc_draw_arrow_noload(widget_def * wid,unsigned short pos_y,unsigned short chn)
{
	/* chn */
	unsigned short color = ( chn == 0 ) ? COLOR_CH1 : COLOR_CH2;
	const unsigned char * chnn = ( chn == 0 ) ? gImage_ch1_arrow : gImage_ch2_arrow;
	/* set check */
	unsigned char text_color = ( chn == 0 ) ? COLOR_TEXT_ARROW_CH1 : COLOR_TEXT_ARROW_CH2;
	unsigned short old = 0;
	/* unsigned short */
	unsigned short xs = 19;
	/* yow */
	unsigned short yow = ( xs % 8 ) ? ( xs / 8 + 1 ) : ( xs / 8 );
	/* check */
	if( wid->msg.y == 0xfff )
	{
		/* first init */
		old = 0xffff;
	}
	else
	{
		/* clear the old pos */
		old = wid->msg.y;	
	}
	/* 0 , 1 , 2 , 5 */
	for( int i = 0 ; i < 12 ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* show */
			if( !(( chnn[ i*yow + j / 8 ] << (j % 8)) & 0x80) )
			{
				unsigned char colofd = wid->dev->read_point(j , pos_y + i);
				/* check */
				if( colofd ==  COLOR_BACK_GROUND || colofd ==  text_color)
				{
					wid->dev->set_noload_point(j , pos_y + i ,color);
				}
			}
			else
			{
				unsigned char colofd = wid->dev->read_point(j , pos_y + i);
				/* check */
				if( colofd ==  COLOR_BACK_GROUND || colofd ==  color)
				{					
					wid->dev->set_noload_point(j , pos_y + i ,text_color);
				}
			}
		}
	}		
	/* set */
	wid->msg.y = pos_y;
	/* clear */
	if( old == 0xffff )
	{
		return;
	}
	/* clear */
	unsigned short dif = 0xffff;
	unsigned short end;
	/* chhec */
	if( old > pos_y )
	{
		dif = old - pos_y;
		/* check */
		if( dif > 12 )
		{
			dif = 0;
			end = 12;
		}
		else
		{
			dif = 12 - dif;
			end = 12;
		}
	}
	else if( old < pos_y )
	{
		dif = pos_y - old;
		/* check */
		if( dif > 12 )
		{
			dif = 0;
			end = 12;
		}		
		else
		{
			end = dif;
			dif = 0;
		}
	}
	else
	{
		/* same */
		dif = 0xffff;
	}
	/* if old */
	if( dif == 0xffff )
	{
		return;//same
	}
	/* */
#if 0
	/* check outch */
	osc_run_msg_def * runmsg_tp = get_run_msg();
	/* check */
	if( chn == 0 )
	{
		/* check */
		if( dif + old > runmsg_tp->vol_offset_scale[1] - 6 && dif + old < runmsg_tp->vol_offset_scale[1] + 6 )
		{
			dif = runmsg_tp->vol_offset_scale[1] + 6 - old;
		}
		/* check end */
		if( end + old > runmsg_tp->vol_offset_scale[1] - 6 && end + old < runmsg_tp->vol_offset_scale[1] + 6 )
		{
			end = runmsg_tp->vol_offset_scale[1] - 6 - old;
		}		
	}
#endif
	/* 0 , 1 , 2 , 5 */
	for( int i = dif ; i < end ; i ++ )
	{
		/* unsigned char */
		for( int j = 0 ; j < xs ; j ++ )
		{
			/* read */
			unsigned char colofd = wid->dev->read_point(j , old + i);
			/* show */
			if( colofd ==  color || colofd == text_color )
			{
				wid->dev->set_noload_point(j , old + i ,COLOR_BACK_GROUND);
			}
		}
	}		
}
/* create the base voltage icon */
static void osc_draw_trig_arrow(widget_def * wd)
{
	/* color tmp */
	const unsigned char trig_shape[13] = 
	{ 0x02,0x06,0x0E,0x1E,0x3E,0x7f,0xff,0x7f,0x3E,0x1E,0x0E,0x06,0x02};
	unsigned short color;
	unsigned short backcolor;
	unsigned char rehide_flag = 0;
	/* dir */
	unsigned char dir_arrow = 0;	
	/* switch chn */
	switch(wd->msg.wflags & 0xC000)
	{
		/* ch1 */
		case 0xC000:
			color = COLOR_CH1;
			dir_arrow = 0;
		break;
		/* ch2 */
		case 0x0000:
			color = COLOR_CH2;
			dir_arrow = 0;
		break;
		/* ch3 */
		case 0x4000:
			color = COLOR_POINT0;
			dir_arrow = 1;
		break;
		/* ch4 */
		case 0x8000:
			color = COLOR_POINT0;
			dir_arrow = 1;
		break;
		/* of */
		default:
			color = COLOR_CH1;
			break;
	}

	/* check rehide */
	if( CHECK_REHIDE(wd->msg.wflags) || wd->msg.mark_flag == 2 )
	{
		/* get parent color */
		backcolor = gui_color((wd->parent->msg.wflags & 0x00E0) << 8);
		/* set */
		rehide_flag = 1;
		/* set deft */
		if( CHECK_REHIDE(wd->msg.wflags) )
		{
			wd->msg.mark_flag = 1;
		}
		/* clear */
		CLEAR_REHIDE(wd->msg.wflags);		
	}
	else
	{
		wd->msg.mark_flag = 1;
	}
	/* move */
	while(wd->msg.mark_flag)
	{
		/* widget pos */
		unsigned short pos_x = wd->msg.x;
		unsigned short pos_y = wd->msg.y;
		/* create */
		for( int i = 0 ; i < 13 ; i ++ )
		{
			for( int j = 0 ; j < 8 ; j ++ )
			{
#ifndef _VC_SIMULATOR_
				if( rehide_flag == 0 )
				{
					if( dir_arrow == 0 )
					{
						/* check point */
						if( ( trig_shape[i] << j ) & 0x80 )
						{
							wd->dev->set_noload_point(pos_x + j , pos_y + i , color);
						}
				  }
					else
					{
						/* check point */
						if( ( trig_shape[i] >> j ) & 0x01 )
						{
							wd->dev->set_noload_point(pos_x + j , pos_y + i , color);
						}
					}
				}
				else
				{
					wd->dev->set_noload_point(pos_x + j , pos_y + i , backcolor);
				}
#else
				unsigned short tm = color[i*20+j];

				widget->dev->set_noload_point(pos_x + j , pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
			}
		}
		/* fe */
		wd->msg.mark_flag --;
		/* check */
		if( wd->msg.mark_flag )
		{
			/* repos */
			wd->msg.x = wd->msg.mx;
			wd->msg.y = wd->msg.my;
			/* pos */
			pos_x = wd->msg.x;
			pos_x = wd->msg.y;
			/* redraw */
			rehide_flag = 0;
			/* reduce */
		}
  }
}
///* void osc draw rightd */
//void osc_draw_right_icon(widget_def * btn_wd )
//{
////	/* create the base voltage icon */
////	const unsigned short right_icon_table[12] = {0x0000,0x0001,0x0003,0x0007,
////		0x400E,0xE01C,0x7038,0x3870,0x1CE0,0x0FC0,0x0780,0x0300};
////	/* check pox */
////	unsigned short start_px = btn_wd->msg.x ;
////	unsigned short start_py = btn_wd->msg.y ;
////	/* check */
////	unsigned int color;
////	/* check */
////	color = COLOR_RIGHT_ICON;
////	/* table */
////	for( int i = 0 ; i < 12 ; i ++ )
////	{
////		/* lines */
////		for( int j = 0 ; j < 16 ; j ++ )
////		{
////			/* if */
////			if( ( right_icon_table[i] << j ) & 0x8000 )
////			{
////				set_l2_point_noload(btn_wd->parent->msg.x_size,start_px + j , start_py + i , color);
////			}
////		}
////	}
//}
/* void osc draw rightd */
static void osc_draw_stop_icon(widget_def * stop_wd )
{
	/* create the base voltage icon */
	const unsigned short right_icon_table[12] = {0x07E0,0x1FF8,0x3FFC,0x3FFC,
		0x7FFE,0x7FFE,0x7FFE,0x7FFE,0x3FFC,0x3FFC,0x1FF8,0x07E0};
	/* check pox */
	unsigned short start_px = stop_wd->msg.x ;
	unsigned short start_py = stop_wd->msg.y ;
	/* check */
	unsigned int color;
	/* check */
  if( CHECK_HIDE(stop_wd->msg.wflags) )
	{
		color = stop_wd->dev->read_point(start_px,start_py);
	}
  else
	{		
		color = COLOR_STOP_ICON;
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
				stop_wd->dev->set_noload_point( start_px + j , start_py + i , color);
			}
		}
	}
}
/* create stop icon */
void osc_create_stop_icon(gui_dev_def * dev,window_def * win,widget_def *stop_wd,unsigned short px,unsigned short py)
{
	/* set det */
	stop_wd->msg.x = px;
	stop_wd->msg.y = py;
	stop_wd->msg.x_size = 0;
	stop_wd->msg.y_size = 0;
	stop_wd->dev = dev;
	stop_wd->draw = osc_draw_stop_icon;
	stop_wd->parent = win;
	stop_wd->msg.upd = COLOR_BUTTON;
	stop_wd->msg.wflags = GUI_HIDE;
	/* create btn */
	gui_widget_creater(stop_wd);
}	
/* */
void osc_calculate_base_arrow(window_def * pwin,widget_def *wd,int chn)
{
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = 0;
	/* msg */
	wd->msg.y = 0xfff;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);	
}
/* trig */
void osc_calculate_trig_arrow(window_def * pwin,widget_def *wd,int chn)
{
	/* set param */
	/* def*/
	unsigned short vertical_pixel_total,hori_pixel;
	/* get msg */
	if( osc_grid_fast(pwin->dev,&hori_pixel,&vertical_pixel_total) == FS_ERR )
	{
		return;
	}		
	/* calbrate the pox */
	/* get chn */
	if( chn == 1 )
	{
	  wd->msg.wflags |= 0xC000;
	}
	else if( chn == 2 )
	{
		wd->msg.wflags &=~ 0xC000;
	}
	else if( chn == 3 )
	{
		wd->msg.wflags &=~ 0xC000;
		/* set flags */
		wd->msg.wflags |= 0x4000;
	}
	else
	{
		wd->msg.wflags &=~ 0xC000;
		/* set flags */
		wd->msg.wflags |= 0x8000;		
	}
	/* check chn */
	if( chn == 1 || chn == 2 )
	{
		/* set ch1 */
		wd->msg.x = pwin->msg.x + hori_pixel + LEFT_REMAIN_PIXEL + 3;
		wd->msg.y = vertical_pixel_total / 2 + TOP_REMAIN_PIXEL - 6 + 1; /* 20 * 12 */
		wd->msg.my = wd->msg.y;
	}
	else
	{
		/* set ch1 */
		wd->msg.x = pwin->msg.x_size / 8 * ( chn - 3 + 1 ) - 13;
		wd->msg.y = TOP_REMAIN_PIXEL / 2 - 6 ; /* 20 * 12 */		
	}
	/* not supply size now */
#if 0
	wd->msg.x_size = 0;
	wd->msg.y_size = 0;
#endif
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = osc_draw_trig_arrow;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);	
}
/* draw run stop ready */
void osc_draw_rsr(widget_def * wid,unsigned char index)
{
	/* dtat */
	extern const char * mert43_eng[6];
	/* check */
	unsigned short px = wid->msg.x;
	unsigned short py = wid->msg.y;
	/* check */
	if( osc_language_get() == 0 )
	{
		osc_l1_string(wid->dev,px,py,mert43_eng[index],COLOR_CHAR,COLOR_GRID_AREA_BG,1);
	}
	else
	{
		unsigned char * hz_ret = ( unsigned char *)mert43_eng[index + 3];
		/* show */
		draw_hz(wid->dev,&hz_ret[0],px,py,COLOR_CHAR,COLOR_GRID_AREA_BG,1);
		draw_hz(wid->dev,&hz_ret[2],px + 16,py,COLOR_CHAR,COLOR_GRID_AREA_BG,1);
		/* clear */
		gui_char(wid->dev,px+32,py,' ',16,COLOR_CHAR,COLOR_GRID_AREA_BG,1);
	}
}
/* create the voltage title */
void osc_calculate_title_string(window_def * pwin,widget_def *wd,int chn,char ** fast_title)
{
	/* if num is not epue */
	if( chn == 0 )
	{
		return;//reutrn,bad data
	}	
	/* set param */
	/* def*/
	for( int i = 0 ; i < chn ; i ++ )
	{
	  /* color */
		wd[i].msg.wflags |= 0x0000;
		/* set ch1 */
		if( i == 0 )
		{
			wd[i].msg.x = 129 + 16;
			/* set y */
			wd[i].msg.y = 15;			
			/* default is 16 size */
			wd[i].msg.wflags &=~ 0x1000;
		}
		else if( i == 1 )
		{
			wd[i].msg.x = 475;
			wd[i].msg.wflags |= 0x1000;
			/* set y */
			wd[i].msg.y = 11;			
		}
		else if( i == 2 )
		{
			wd[i].msg.x = 475 + 16;
			wd[i].msg.wflags |= 0x1000;
			/* set y */
			wd[i].msg.y = 11;
		}
		else
		{
			wd[i].msg.x = 475 + 2;
		}

		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		wd[i].msg.pri_data = fast_title[i];
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create the base voltage icon */
static void osc_draw_trig_line(widget_def * wd)
{
	/* get area */
	unsigned char rehide_flag = 0;
	draw_area_def * area = get_draw_area_msg();
	/* color table */
	const unsigned char color_table_trig_lines[6][2] = 
	{
		{
			COLOR_CH1_GR_TRIG,
			COLOR_CH1_BG_TRIG,
		},
		{
			COLOR_CH2_GR_TRIG,
			COLOR_CH2_BG_TRIG,			
		},
		{
			COLOR_MEASURE_LINE_H0_0,
			COLOR_MEASURE_LINE_H0_1,			
		},
		{
			COLOR_MEASURE_LINE_H1_0,
			COLOR_MEASURE_LINE_H1_1,			
		},
		{
			COLOR_MEASURE_LINE_V0_0,
			COLOR_MEASURE_LINE_V0_1,			
		},
		{
			COLOR_MEASURE_LINE_V1_0,
			COLOR_MEASURE_LINE_V1_1,			
		}
	};
	/* check rehide */
	if( CHECK_REHIDE(wd->msg.wflags) || wd->msg.mark_flag == 2 )
	{
		/* set */
		rehide_flag = 1;
		/* set deft */
		if( CHECK_REHIDE(wd->msg.wflags) )
		{
			wd->msg.mark_flag = 1;
		}
		/* clear */
		CLEAR_REHIDE(wd->msg.wflags);		
	}
	else
	{
		wd->msg.mark_flag = 1;
	}	
	/* get color */
	int color_index =  wd->msg.wflags >> 12;
	/* set label */
  const unsigned char * chn =  color_table_trig_lines[color_index];
	/* while */
	while(wd->msg.mark_flag)
	{
		/* chec */
		unsigned char xy = ( color_index >= 4 ) ? 1 : 0;
		unsigned short start_xy = ( color_index >= 4 ) ? area->start_pos_y : area->start_pos_x;
		unsigned short stop_xy = ( color_index >= 4 ) ? area->stop_pos_y : area->stop_pos_x;
		/* set point */
		for( int i = start_xy ; i < stop_xy ; i +=2 )
		{
			/* get point */
			unsigned char piox;
			/* check */
			if( xy == 0 )
			{
				piox = wd->dev->read_point(i,wd->msg.y);
			}
			else
			{
				piox = wd->dev->read_point(wd->msg.x,i);
			}
			/* rehide */
			if( rehide_flag == 0 )
			{
				if( xy == 0 )
				{
					/* chn */
					if( piox == COLOR_GRID_POINT ) // chn1
					{
						wd->dev->set_noload_point(i,wd->msg.y,chn[0]);
					}
					else if( piox == COLOR_GRID_AREA_BG )
					{
						wd->dev->set_noload_point(i,wd->msg.y,chn[1]);
					}
					else
					{
						/* draw no point */
					}
			  }
				else
				{
					/* chn */
					if( piox == COLOR_GRID_POINT ) // chn1
					{
						wd->dev->set_noload_point(wd->msg.x,i,chn[0]);
					}
					else if( piox == COLOR_GRID_AREA_BG )
					{
						wd->dev->set_noload_point(wd->msg.x,i,chn[1]);
					}
					else
					{
						/* draw no point */
					}					
				}
			}
			else
			{
				if( xy == 0 )
				{
					/* chn */
					if( piox == chn[0] )
					{
						wd->dev->set_noload_point(i,wd->msg.y,COLOR_GRID_POINT);
					}
					else if( piox == chn[1] )
					{
						wd->dev->set_noload_point(i,wd->msg.y,COLOR_GRID_AREA_BG);
					}
					else
					{
						/* draw no point */
					}			
			  }
				else
				{
					/* chn */
					if( piox == chn[0] )
					{
						wd->dev->set_noload_point(wd->msg.x,i,COLOR_GRID_POINT);
					}
					else if( piox == chn[1] )
					{
						wd->dev->set_noload_point(wd->msg.x,i,COLOR_GRID_AREA_BG);
					}
					else
					{
						/* draw no point */
					}						
				}
			}
			/* set point */
		}
		/* fe */
		wd->msg.mark_flag --;
		/* check */
		if( wd->msg.mark_flag )
		{
			/* repos */
			if( xy == 0 )
			{
				wd->msg.y = wd->msg.my;
			}
			else
			{
				wd->msg.x = wd->msg.mx;
			}
			/* redraw */
			rehide_flag = 0;
			/* reduce */
		}	
	}
}
/* create the trig lines */
void osc_calculate_trig_line(window_def * pwin,widget_def *wd,int chn)
{
	/* clear */
	wd->msg.wflags &=~ 0xF000;
	/* get chn */
	if( chn == 1 )
	{
	  wd->msg.wflags |= 0x0000;
		wd->msg.y = 100; /* for test */
	}
	else if( chn == 2 )
	{
		wd->msg.wflags |= 0x1000;
		wd->msg.y = 0x16A + 6; /* for test */
	}
	else if( chn == 3 )
	{
		wd->msg.wflags |= 0x2000;
		wd->msg.y = 75; /* for test */
	}
	else if( chn == 4 )
	{
		wd->msg.wflags |= 0x3000;
		wd->msg.y = 300; /* for test */
	}	
	else if( chn == 5 )
	{
		wd->msg.wflags |= 0x4000;
		wd->msg.x = 300; /* for test */
	}
	else if( chn == 6 )
	{
		wd->msg.wflags |= 0x5000;
		wd->msg.x = 500; /* for test */
	}
	else
	{
		
	}
	if( chn < 3 )
	{
		/* set default is hide */
		SET_HIDE(wd->msg.wflags);
	}
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = osc_draw_trig_line;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);	
}