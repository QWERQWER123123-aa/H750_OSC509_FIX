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
#include "dev.h"
#include "gui_ascii.h"
#include "hz_out24.h"
#include "string.h"

#define START_DELAY 1000000;            //¿ª»úÑÓÊ±

/* QR size */
#define QR_SIZE 12
#define QR_X    400 
#define QR_Y    60


const unsigned int qrtab[32] = {
	0,
};

/* line add */
static unsigned short ypos = 32;
/* Includes ------------------------------------------------------------------*/
void gui_char(unsigned short x,unsigned short y,char num,unsigned char size,unsigned int color,unsigned int backcolor,unsigned char show_m)
{
	/* define some function */
	unsigned char temp;
	unsigned short y0 = y;
	/* calbrate the size */
	unsigned char csize = ( size / 8 + ( ( size % 8 ) ? 1 : 0 ) ) * ( size / 2);
	/* get the offset */
	num = num - ' ';
	/* write the char */
	for(int i = 0 ; i < csize ; i ++)
	{   
		/* default is the 2412 font */
		if( size == 16 )
		{
			temp = asc2_1608[num][i];
		}
		else
		{
			temp = asc2_2412[num][i];
		}
		/* draw point */
		for( int j = 0 ; j < 8  ; j ++ )
		{
			/* one by one */
			if( temp & 0x80 )
			{
				set_noload_point( x, y, color);
			}
			else 
			{
				if( show_m )
				{
					set_noload_point( x, y, backcolor);
				}
			}
			/* shift */
			temp <<= 1;
			/* next point */
			y++;
			/* get changed */
			if( ( y - y0 ) == size )
			{
				y = y0;
				x++;
				/* break */
				break;
			}
		}  	 
	}  	    	   	 	  
}
/* show string */
void show_string(char * hzc)
{
	unsigned short x = 0;
	/* show */
	while( *hzc != 0 )
	{
		/* show a char */
		gui_char(x , ypos ,*hzc,24,8,1,0);
		/* channge pos */
		x += 12;
		/* out */
		hzc += 1;
	}	
	/* ypos add */
	ypos += 24;
	/* limit */
	if( ypos >= 480 )
	{
		ypos = 0;
	}
}
/* fe */
void show_string_static(char * hzc)
{
	unsigned short x = 0;
	/* show */
	while( *hzc != 0 )
	{
		/* show a char */
		gui_char(x , ypos ,*hzc,24,8,1,1);
		/* channge pos */
		x += 12;
		/* out */
		hzc += 1;
	}
}
/* show process */
void show_process_des(void)
{
	/* param */
	unsigned int prce = 0;
	unsigned int t = 0;
	/* show provess */
	while(1)
	{
		for(int i = 0 ; i < 16 ; i ++ )
		{
			for( int j = 0 ; j < 12 ; j ++ )
			{
				set_noload_point(prce + j,i + 450,5);
			}
		}
		/* prce */
		prce += 12;
		/* limit */
		if( (prce+12) >= 800 )
		{
			return;
		}
		/* delay */
		t = START_DELAY;
		/* while */
		while(t--);
		/* end */
	}
}
/* show process */
void hide_process(void)
{
	/* param */
	unsigned int prce = 0;
	/* show provess */
	while(1)
	{
		for(int i = 0 ; i < 16 ; i ++ )
		{
			for( int j = 0 ; j < 12 ; j ++ )
			{
				set_noload_point(prce + j,i + 450,1);
			}
		}
		/* prce */
		prce += 12;
		/* limit */
		if( (prce+12) >= 800 )
		{
			return;
		}
	}
}

/* find the hz */
static unsigned char * find_hz(unsigned char * hzw)
{
	/* get the hz pos fast*/
	unsigned short * hsz = (unsigned short *)hzw;
	/* get pos */
	for( int i = 0 ; i < sizeof(HZ_INDEX) / sizeof(HZ_INDEX[0]) ; i ++ )
	{
		/* match the data */
		if( HZ_INDEX[i] == *hsz )
		{
			/* ok */
			return (unsigned char *)HZ_DATAX[i];
		}
	}
	/* can not find the hz */
	return (unsigned char *)HZ_DATAX[0];
}
/* show one hz */
static void draw_hz(unsigned char * hzd,unsigned short x,unsigned short y,unsigned int color,unsigned int backcolor,unsigned char show_m)
{
	/* find the hz */
	unsigned char * hzc = find_hz(hzd);
	/* show a hz */
	int ti = 0;
	/* draw a point  */
  for( int i = 0 ; i < 72 ; i ++ )
	{
		for( int j = 0 ; j < 8 ; j ++ )
		{
			if( ( hzc[i] << j ) & 0x80 )
			{
				set_noload_point( x + ti / 24 , y + ti % 24 , color);
			}
			else
			{
				if( show_m )
				{
					set_noload_point( x + ti / 24 , y + ti % 24 , backcolor);
				}
			}
			/* inc */
			ti++;
		}
	}
}
#if 0
/* get color */
unsigned short gui_color(unsigned short colo)
{
	/* chea */
	unsigned short color;
	/* front color */
	switch(colo)
	{
		case 0x0000:
			color = COLOR_CHAR;
		break;
		case 0x2000:
			color = COLOR_CH1;
			break;
		case 0x4000:
			color = COLOR_CH2;
			break;		
		case 0x6000:
			color = COLOR_TIPS_ERROR;
			break;			
		case 0x8000:
			color = COLOR_TIPS_WARNING;
			break;
		case 0xA000:
			color = COLOR_TIPS_NORMAL;
			break;		
		case 0xC000:
			color = COLOR_GRID_AREA_BG;
			break;			
		case 0xE000:
			color = COLOR_BACK_GROUND;
			break;
		default:
			color = COLOR_GRID_AREA_BG;
			break;
	}
	/* return */
	return color;
}
#endif
/* show a mix string */
void gui_dynamic_string(unsigned short px,unsigned short py,char * hzc,unsigned int color)
{
	/* temp */
	unsigned short x = 0;
	/* judge Is this a char or hz */
	while( *hzc != 0 )
	{
		/* show a hz or char */
		if( (unsigned char)(*hzc) > 0x80 )
		{
			/* show a hz */
			draw_hz((unsigned char *)hzc,px + x , py, color,0,0);
			/* channge pos */
			x += 24;
			hzc += 2;
		}
		else
		{
			/* show a char */
			gui_char(px + x , py,*hzc,24,color,0,0);
		  /* channge pos */
			if( 24 == 16 )
			{
			  x += 8;
			}
			else
			{
				x += 12;
			}
			/* out */
			hzc += 1;
		}
	}
}
/* void console */
void console_out(char * co)
{
	gui_dynamic_string(0,ypos,co,8);
	
	ypos += 24;
}

/* grounp */
static void set_grounp(unsigned int x, unsigned int y, unsigned int color)
{
	/* incremeate */
	x *= QR_SIZE;
	y *= QR_SIZE;
  /* incremeate */ 
	for (int i = 0; i < QR_SIZE; i++)
	{
		for (int j = 0; j < QR_SIZE; j++)
		{
			set_noload_point(j+x + QR_X , i+y + QR_Y , color);
		}
	}
}
/* create the bootloader ui */
void create_boot_ui(void)
{
	/* set title */
	display_info_def * gui_dev = get_display_dev_info();
	/* set */
  memset((void *)gui_dev->gram_addr,2,800*32);
	/* show title */
	gui_dynamic_string(240,3,"STM32H750 BOOTLOADER V1.0.0",0);
	/* show QRcode */
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 31; j++)
		{
			if ((qrtab[i] >> j ) & 0x01)
			{
				set_grounp(j, i, 3);
			}
			else
			{
				set_grounp(j, i,0 );
			}
		}
	}
	//gui_dynamic_string(500,35,"HELLO",8);
}





























