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
#include "gui.h"
#include "gui_dev.h"
#include "string.h"
#include "gui_dev.h"
/* Includes ------------------------------------------------------------------*/
static window_def * original_win = 0;
/* gui event */
static unsigned int GUI_EVENT = 0;
/* init a */
FOS_INODE_REGISTER("gui",gui_dev_init,gui_static_creater,0,5);
/*----------------------------------------------------------------------------*/
FOS_TSK_REGISTER(gui_thread,PRIORITY_4,10); /* gui detecter task run as 100 ms*/
/* gui task */
static void gui_thread(void)
{
	int ret = 0;
	/* check event */
	if( GUI_EVENT != 0 )
	{
		/* show data */
		ret = gui_static_creater();
		/* if */
		if( ret != 0 )
		{
			GUI_EVENT -= ret;
		}
	}
}
/* int gui draw */
static int gui_static_creater(void)
{
	/* int ret */
	int ret = 0;
	/* from zero to end creating the pic */
	for( window_def * base = original_win ; base != 0 ; base = base->win_child )
	{
		/* if use the default draw function */
		if((!CHECH_DRAWED(base->msg.wflags)) && ( base->draw != 0 ) && (!CHECK_HIDE(base->msg.wflags)))
		{
			base->draw(base);
			/* set flag */
			SET_DRAWED(base->msg.wflags);
			/* clear the mark */
			base->msg.mark_flag = 0;
			/* event */
			ret ++;
		}
		else
		{
			/* check hide or not */
			if( CHECK_HIDE(base->msg.wflags) )
			{
				continue; /* skip this window */
			}
		}
		/* create widget */
		for( widget_def * wbase = base->wchild ; wbase != 0 ; wbase = wbase->peer_linker)
		{
			/* if use the default draw function */
			if((!CHECH_DRAWED(wbase->msg.wflags)) && ( wbase->draw != 0 ) && ((!CHECK_HIDE(wbase->msg.wflags)) || CHECK_REHIDE(wbase->msg.wflags)))
			{
				wbase->draw(wbase);
				/* set flag */
				SET_DRAWED(wbase->msg.wflags);
				/* def */
				if(CHECK_REHIDE(wbase->msg.wflags))
				{
					/* clear flag */
					CLEAR_REHIDE(wbase->msg.wflags);
					/* end */
				}
				/* event */
				ret ++;				
			}
			else
			{
				/* can not supply now*/
			}
		}
	}
	/* return */
	return ret;
}/* int gui draw */
int gui_show_win_noload(window_def * ori)
{
	/* from zero to end creating the pic */
	window_def * base = ori ;
	/* if use the default draw function */
	if( base->draw != 0 )
	{
		base->draw(base);
		/* set flag */
		SET_DRAWED(base->msg.wflags);
	}
	else
	{
		/* check hide or not */
		//return FS_ERR;
	}
	/* create widget */
	for( widget_def * wbase = base->wchild ; wbase != 0 ; wbase = wbase->peer_linker)
	{
		/* if use the default draw function */
		if( wbase->draw != 0 )
		{
			wbase->draw(wbase);
		}
		else
		{
			/* can not supply now*/
			return FS_ERR;
		}
	}
	/* return */
	return FS_OK;
}
/* create and init a win */
int gui_win_creater(window_def * win)
{
	/* juddge wherere it is the ORIGINAL_PARENT */
	if( original_win == ORIGINAL_PARENT )
	{
		/* return error . can not find the original win */
		original_win = win;
		/* ok */
		return FS_OK;
	}
	/* link with parent */
	window_def * base;
	/* same mode or */
	/* set */
	for( base = original_win ; base->win_child != 0 ; base = base->win_child )
	{
		/* find the pos . nothing to do*/
	}		
	/* set */
	base->win_child = win;
	/* return OK */
	return FS_OK;
}
/* create widget */
int gui_widget_creater(widget_def * widget )
{
	/* juddge wherere it is the ORIGINAL_PARENT */
	if( widget->parent == ORIGINAL_PARENT )
	{
		/* return error */
		return FS_ERR;
	}
	/* set the original linker */
	if( widget->parent->wchild == ORIGINAL_PARENT )
	{
		/* set */
		widget_def ** base = &widget->parent->wchild;
		/* set base */
		*base = widget;
		/* return OK */
		return FS_OK;
	}
	/* get the linker tail */
	widget_def * wbase = widget->parent->wchild;
	/* set up widget */
	for(  ; wbase->peer_linker != 0 ; wbase = wbase->peer_linker )
	{
		/* find the pos . nothing to do*/
	}
	/* set linker */
	wbase->peer_linker = widget;
	/* return OK */
	return FS_OK;
}
/* clear all drawed */
int gui_clear_all(void)
{
	/* from zero to end creating the pic */
	for( window_def * base = original_win ; base != 0 ; base = base->win_child )
	{
		/* clear */
		CLEAR_DRAWED(base->msg.wflags);
		/* create widget */
		for( widget_def * wbase = base->wchild ; wbase != 0 ; wbase = wbase->peer_linker)
		{
			/* set flag */
			CLEAR_DRAWED(wbase->msg.wflags);
			/* def */
		}
	}
	/* return */
	return FS_OK;
}
/* find the connect */
int gui_find_connect(window_def * ori,unsigned short x,unsigned short y)
{
	/* from zero to end creating the pic */
	for( window_def * base = ori->win_child ; base != 0 ; base = base->win_child )
	{
		if( base->msg.wflags & GUI_HIDE )
		{
			continue;
		}
		if( x >= base->msg.x && x <= (base->msg.x + base->msg.x_size) )
		{
			if( y >= base->msg.y && y <= (base->msg.y + base->msg.y_size) )
			{
				return 1;
			}
		}
	}
	/* return */
	return 0;
}
/* set widget to undrawed */
static int gui_clear_widget(window_def * win)
{
	/* find the wchild and clear */
  for( widget_def * wbase = win->wchild ; wbase != 0 ; wbase = wbase->peer_linker)	
	{
		/* clear the redraw flag */
		CLEAR_DRAWED(wbase->msg.wflags);
	}
	/* return */
	return FS_OK;
}
/* gui hide window */
int gui_hide_win(window_def * win)
{
	/* get hide flag */
	if( CHECK_HIDE(win->msg.wflags) )
	{
		return FS_OK;//already hide
	}
	/* got xy */
	int gotxy = 0;
	/* from zero to end creating the pic */
	for( window_def * base = original_win ; base != 0 ; base = base->win_child )
	{
		/* skip the base */
		if( win == base )
		{
			continue;//skip this win
		}
		/* clear */
		gotxy = 0;
		/* get the cross data */
		if((( win->msg.x <= base->msg.x ) && ( win->msg.x + win->msg.x_size ) >= base->msg.x ) || 
			 (( win->msg.x >= base->msg.x ) && ( win->msg.x <= ( base->msg.x + base->msg.x_size ))))
		{
		  /* got x */
			gotxy = 1;
		}
		/* get the cross data y */
		if((( win->msg.y <= base->msg.y ) && ( win->msg.y + win->msg.y_size ) >= base->msg.y ) || 
			 (( win->msg.y >= base->msg.y ) && ( win->msg.y <= ( base->msg.y + base->msg.y_size ))))
		{
		  /* got x */
			if( gotxy == 1 )
			{
			  /* got y */
				base->msg.mark_flag = 1;
				/* set other */
				base->msg.mx = win->msg.x;
				base->msg.my = win->msg.y;
				base->msg.mxstop = win->msg.x_size + win->msg.x;
				base->msg.mystop = win->msg.y_size + win->msg.y;
				/* clear the redraw flag */
				CLEAR_DRAWED(base->msg.wflags);
				/* set hide */
				SET_HIDE(win->msg.wflags);
				/* create gui event */
				CREATE_GUI_EVENT(GUI_EVENT);
				/* endo if */
			}
		}		
	}
	/* return ERROR */
	return FS_OK;
}
/* gui show a win*/
int gui_show_win(window_def * win)
{
	/* get hide flag */
	if( !CHECK_HIDE(win->msg.wflags) )
	{
		return FS_OK;//already show
	}	
	/* clear */
	CLEAR_HIDE(win->msg.wflags);
	/* set loce */
	CLEAR_DRAWED(win->msg.wflags);
	/* clear flag */
	gui_clear_widget(win);
	/* create gui event */
	CREATE_GUI_EVENT(GUI_EVENT);	
	/* return ERROR */
	return FS_OK;	
}
/* gui hide a widget */
int gui_hide_widget(widget_def * wid)
{
	/* check hide */
	if( !CHECK_HIDE(wid->parent->msg.wflags) )
	{
		/* check hide */
		if( !CHECK_HIDE(wid->msg.wflags) )
		{			
			/* widget set rehide */
			SET_REHIDE(wid->msg.wflags);
			/* set the hide */
			SET_HIDE(wid->msg.wflags);
			/* clear */
			CLEAR_DRAWED(wid->msg.wflags);
			/* create gui event */
			CREATE_GUI_EVENT(GUI_EVENT);			
			/* return OK */
			return FS_OK;
		}
	}
	else
	{
		if( !CHECK_HIDE(wid->msg.wflags) )
		{
			/* set the hide */
			SET_HIDE(wid->msg.wflags);	
		}
	}
	/* return ERROR */
	return FS_ERR;
}
/* gui hide a widget */
int gui_show_widget(widget_def * wid)
{
	/* check hide */
	if( !CHECK_HIDE(wid->parent->msg.wflags) && CHECK_HIDE(wid->msg.wflags) )
	{
		CLEAR_HIDE(wid->msg.wflags);
		/* create gui event */
		CREATE_GUI_EVENT(GUI_EVENT);
		/* return OK */
		return FS_OK;
	}
	/* return ERROR */
	return FS_ERR;
}
/* set window text */
int gui_set_wid_text(widget_def * wid,char * data)
{
	/* set win txt */
	if( !CHECK_OVERM(wid->msg.wflags))
	{
		/* set data */
		wid->msg.pri_data = data;
		/* hide ? */
		if( !CHECK_HIDE(wid->msg.wflags) && !CHECK_HIDE(wid->parent->msg.wflags) )
		{
			/* set OVERM */
			SET_OVERM(wid->msg.wflags);
			/* clear */
			CLEAR_DRAWED(wid->msg.wflags);
			/* create gui event */
			CREATE_GUI_EVENT(GUI_EVENT);
			/* return */
			return FS_OK;
		}
	}
	/* return ERROR */
	return FS_ERR;
}
/* gui move a widget */
int gui_move_wid(widget_def * wid,unsigned short px,unsigned short py)
{
	/* calbrate the new posion */
	wid->msg.mx = px;
	wid->msg.my = py;
	/* check hide */
	if( !CHECK_HIDE(wid->parent->msg.wflags) && !CHECK_HIDE(wid->msg.wflags) )
	{
		/* do not need notice to gui thread */
		wid->msg.mark_flag = 2;// move
		/* clear */
		CLEAR_DRAWED(wid->msg.wflags);
		/* create gui event */
		CREATE_GUI_EVENT(GUI_EVENT);
		/* return */
		return FS_OK;
	}
  /* return OK */
	return FS_OK;
}
/* gui get connect */
int gui_wid_inline( widget_def * wid , unsigned short * upd)
{
	/* get parent window 's pointer */
	for( widget_def * wd = wid->parent->wchild ; wd != 0 ; wd = wd->peer_linker)
	{
		/* check . skip current widget */
		if( wd != wid )
		{
			/* check range */
			if(( wid->msg.x >= wd->msg.x && wid->msg.x < (  wd->msg.x +  wd->msg.x_size ) ) && 
				 ( wid->msg.y >= wd->msg.y && wid->msg.y < (  wd->msg.y +  wd->msg.y_size ) ))
			{
				/* set upd */
				*upd = wd->msg.upd;
				/* return FS_OK */
				return FS_OK;
			}
		}
	}
	/* error */
	return FS_ERR;
}


























































