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
#include "crc.h"
/*-------fmlink--------*/
const unsigned char CRC16_TABLE[256] = {
	50, 124, 137, 0, 237, 217, 104, 119, 0, 0, 0, 
	89,  0,  0,  0,  0,  0,  0,  0,  0, 214,    
	159, 220, 168, 24, 23, 170, 144, 67, 115, 39,   
	246, 185, 104, 237, 244, 222, 212, 9, 254, 230, 
	28, 28, 132, 221, 232, 11, 153, 41, 39, 0,    
	0, 0, 0, 15, 3, 0, 230, 0, 0, 0,   
	153, 183, 51, 82, 118, 148, 21, 0, 243, 124,
	0, 0, 38, 20, 158, 152, 143, 0, 0, 0,
    106, 49, 22, 143, 140, 5, 150, 0, 231, 183,
	63, 54, 0, 0, 0, 0, 0, 0, 0, 175,
	102, 158, 208, 56, 93, 138, 108, 32, 185, 84,
	34, 0, 124, 237, 4, 76, 128, 56, 116, 134,
	237, 203, 250, 87, 203, 220, 25, 226, 46, 29,
	223, 85, 6, 229, 203, 1,  0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 154, 49, 0, 134,
	219, 208, 188, 84, 22, 19, 21, 134, 0, 78,
	68, 189, 127, 154, 21, 21, 144, 1, 234, 73,
	181, 22, 83, 167, 138, 234, 240, 47, 189, 52,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,  0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 204, 49, 170, 44, 83, 46,  0};

//************************************************************************************
/*
 *  if the crc data will be  same as the data calc,then return 0;
 *
 *  else return fail none-true
 *
 */
unsigned short holder_check_crc(unsigned char *data,int len)
{
	/* resave it now */
	unsigned short crc;
	/* crc */
	crc = crc16_accumulate_buffer(len,data);
	/* crc */
	return crc;
}
//************************************************************************************
unsigned short crc16_accumulate(unsigned char data,unsigned short crc_data)
{
	/* end fif */
	unsigned char ch;
	/* chee */
	ch = (unsigned char)(data ^ (unsigned char)(crc_data & 0x00ff));
	ch = (unsigned char)(ch ^ (ch << 4));
	/* fdfef */
	crc_data = ((crc_data >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
	/* fesf */
	return crc_data;
}
//************************************************************************************

//************************************************************************************
unsigned short crc16_accumulate_buffer(unsigned short buffer_length,unsigned char* data_buffer)
{
	/* crcdfsf */
	unsigned short crc = 0xf00f;
	/* int */
	for( int i = 1 ; i < buffer_length ; i++ )
	{
		crc = crc16_accumulate(data_buffer[i],crc );
	}
	/* crc */
	crc = crc16_accumulate( CRC16_TABLE[data_buffer[5]],crc);
	/* resf */
	return crc;
}








