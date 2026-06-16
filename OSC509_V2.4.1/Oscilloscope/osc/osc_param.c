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
#include "osc_menu.h"
#include "string.h"
#include "osc_cfg.h"
#include "osc_param.h"
#include "k24c02.h"
#include "osc.h"
#include "hal_iic.h"
#include "main.h"
#include "fatfs.h"
/* Private includes ----------------------------------------------------------*/
#define OSC_FLASH_EEPROM_ADDR  ( 0x08000000 + 0x20000 * 7 )

/* SD card param storage with integrity check */
typedef struct
{
	unsigned int magic;
	unsigned int crc;
	osc_run_msg_def data;
}osc_sd_param_def;

/* CRC32 table */
static const unsigned int crc32_table[256] = {
	0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,
	0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
	0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,
	0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
	0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,
	0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
	0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
	0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
	0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,
	0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
	0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
	0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
	0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
	0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,
	0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
	0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,
	0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
	0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,
	0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
	0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
	0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
	0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,
	0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB30A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
	0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
	0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
	0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,
	0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x61052BD3,0x166CFE45,
	0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C3,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,
	0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x478B2C7F,0x30B5FFE9,
	0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,
	0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
};

static unsigned int osc_crc32(const unsigned char *buf, unsigned int len)
{
	unsigned int crc = 0xFFFFFFFF;
	for(unsigned int i = 0; i < len; i++)
		crc = (crc >> 8) ^ crc32_table[(crc ^ buf[i]) & 0xFF];
	return crc ^ 0xFFFFFFFF;
}

static int osc_sd_param_check(osc_sd_param_def *sp)
{
	if(sp->magic != OSC_PARAM_MAGIC)
		return FS_ERR;
	unsigned int crc_calc = osc_crc32((unsigned char *)&sp->data, OSC_PARAM_SIZE);
	if(crc_calc != sp->crc)
		return FS_ERR;
	return FS_OK;
}

static int osc_sd_param_save(osc_run_msg_def *ormd)
{
	osc_sd_param_def sp;
	sp.magic = OSC_PARAM_MAGIC;
	memcpy(&sp.data, ormd, OSC_PARAM_SIZE);
	sp.crc = osc_crc32((unsigned char *)&sp.data, OSC_PARAM_SIZE);

	FIL file;
	if(f_open(&file, OSC_SD_PARAM_FILE, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
		return FS_ERR;
	UINT bw;
	if(f_write(&file, &sp, sizeof(sp), &bw) != FR_OK || bw != sizeof(sp))
	{
		f_close(&file);
		return FS_ERR;
	}
	f_close(&file);
	return FS_OK;
}

static int osc_sd_param_load(osc_run_msg_def *ormd)
{
	osc_sd_param_def sp;
	FIL file;
	if(f_open(&file, OSC_SD_PARAM_FILE, FA_READ) != FR_OK)
		return FS_ERR;
	UINT br;
	if(f_read(&file, &sp, sizeof(sp), &br) != FR_OK || br != sizeof(sp))
	{
		f_close(&file);
		return FS_ERR;
	}
	f_close(&file);
	if(osc_sd_param_check(&sp) != FS_OK)
		return FS_ERR;
	memcpy(ormd, &sp.data, OSC_PARAM_SIZE);
	return FS_OK;
}

static int osc_sd_card_ready(void)
{
	extern int sd_card_sta;
	return (sd_card_sta == FR_OK) ? 1 : 0;
}
/* Includes ------------------------------------------------------------------*/
FOS_TSK_REGISTER(osc_param_thread,PRIORITY_IDLE,100); /* gui detecter task run as idle */
FOS_INODE_REGISTER("osc_thread",osc_param_heap,osc_param_init,0,4);
/* FOS task */
/* iic sta change */
FOS_TSK_REGISTER(hal_iic_sta,PRIORITY_4,100);
/* tmp param */
static osc_run_msg_def osc_m_tmp;
/* dfe */
static osc_run_msg_def * runmsg;
/* sta */
extern void osc_param_sys_set(unsigned int * src , unsigned char b_data,unsigned char index);
static int osc_flash_eeprom_read(osc_run_msg_def * ormd,unsigned int *);
static int osc_flash_eeprom_write(osc_run_msg_def * orew);
extern unsigned int hal_sys_time_us(void);
extern unsigned int touch_idle_cnt;
extern unsigned char cal_step_start;
/* heap init */
static int osc_param_heap(void)
{
	return FS_OK;
}
/* check sum */
static unsigned char osc_param_check(unsigned char * dat,unsigned int len)
{
	/* sum */
	unsigned char xor_t = 0;
	/* ce */
	for( int i = 0 ; i < len ; i ++ )
	{
		xor_t += dat[i];
	}
	/* return */
	return xor_t;
}
/* write flash */
static int osc_write_flash_block(unsigned int addr,void * d)
{
	/* check*/
	unsigned char * dfeee = (unsigned char *)d;
	HAL_StatusTypeDef status;
	/* need erase flash */
	HAL_FLASH_Unlock();
	/* resd */
	for( int i = 0 ; i < 4 ; i ++ )
	{
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,(unsigned int)addr + i * 32 ,(unsigned int)&dfeee[i*32]);
		if( status != HAL_OK )
		{
			HAL_FLASH_Lock();
			return FS_ERR;
		}
	}
	HAL_FLASH_Lock();
	return FS_OK;
}
/* first check */
static int osc_flash_eeprom_init(void)
{
	unsigned int dep = 0;
	/* sey */
	int ret = osc_flash_eeprom_read(&osc_m_tmp,&dep);
	/* check */
	if( ret == FS_OK )
	{
		if( dep > 1000 )
		{
			/* need erase flash */
			HAL_FLASH_Unlock();
			/* set */
#if OSC_STM32H750	
			/* set */
			FLASH_Erase_Sector(7,FLASH_BANK_1,FLASH_VOLTAGE_RANGE_3);	
#else
			for( int i = 0 ; i < 16 ; i ++ )
			{
				FLASH_Erase_Sector(7*16+i,FLASH_BANK_1,0x20);	
			}
#endif		
			/* check */
			osc_flash_eeprom_write(&osc_m_tmp);
			HAL_FLASH_Lock();
		}
		/* progmer */
		return FS_OK;
	}
	/* ok */
	return FS_ERR;
}
/* osc param read */
static int osc_flash_eeprom_read(osc_run_msg_def * ormd,unsigned int * ind)
{
	/* tm 0*/
	unsigned char rd[128];
	osc_run_msg_def * or_f;
	unsigned int t_i = 0xffff;
		/* check */
	unsigned char * flash_eeprom_addr_base = (unsigned char *)OSC_FLASH_EEPROM_ADDR;
	/* read */
	for( int i = 0 ; i < 1024 ; i ++ )
	{
		unsigned char * tpf = &flash_eeprom_addr_base[i * 128];
		/* read */
		memcpy(rd,tpf,128);
		/* check */
		or_f = ( osc_run_msg_def * )rd;
		/* check */
		if( or_f->check == osc_param_check((unsigned char *)or_f,82))
		{
			/* ok . next */
			t_i = i;
		}
	}
	/* return last valid entry found */
	if( t_i != 0xffff )
	{
		unsigned char * tpf = &flash_eeprom_addr_base[t_i * 128];
		memcpy(ormd,tpf,sizeof(osc_run_msg_def));
		*ind = t_i;
		return FS_OK;
	}
	/* error */
	return FS_ERR;
}
/* static write */
static int osc_flash_eeprom_write(osc_run_msg_def * orew)
{
	/* get */
	unsigned char wd[128];
		/* check */
	unsigned char * flash_eeprom_addr_base = (unsigned char *)OSC_FLASH_EEPROM_ADDR;
	/* read */
	for( int i = 0 ; i < 1024 ; i ++ )
	{
		/* read */
		memcpy(wd,&flash_eeprom_addr_base[i * 128] , 128);
		/* check 0xFF */
		int j;
		/* check 0xff */
		for( j = 0 ; j < 128 ; j ++ )
		{
			if( wd[j] != 0xff )
			{
				break;
			}
		}
		/* check */
		if( j == 128 )
		{
			/* ok get write */
			if( osc_write_flash_block((unsigned int)flash_eeprom_addr_base + i * 128,orew) != FS_OK )
			{
				return FS_ERR;
			}
			/* return */
			return FS_OK;
		}
	}
	/* sector full, erase and write to slot 0 */
	HAL_FLASH_Unlock();
#if OSC_STM32H750	
	FLASH_Erase_Sector(7,FLASH_BANK_1,FLASH_VOLTAGE_RANGE_3);	
#else
	for( int i = 0 ; i < 16 ; i ++ )
	{
		FLASH_Erase_Sector(7*16+i,FLASH_BANK_1,0x20);	
	}
#endif
	if( osc_write_flash_block((unsigned int)flash_eeprom_addr_base,orew) != FS_OK )
	{
		return FS_ERR;
	}
	return FS_OK;
}
/* osc_param_init */
static int osc_param_init(void)
{
	runmsg = get_run_msg();
	int sd_ok = osc_sd_card_ready();

	if(sd_ok)
	{
		if(osc_sd_param_load(&osc_m_tmp) == FS_OK)
		{
			memcpy(runmsg, &osc_m_tmp, sizeof(osc_m_tmp));
			return FS_OK;
		}
	}
	else
	{
		int rett = osc_flash_eeprom_init();
		if(rett == FS_OK)
		{
			memcpy(runmsg, &osc_m_tmp, sizeof(osc_m_tmp));
			return FS_OK;
		}
	}

	memset(runmsg, 0, sizeof(osc_run_msg_def));
	runmsg->vol_offset_scale[0] = 400 / 2;
	runmsg->vol_offset_scale[1] = 400 / 2 + 50;
	runmsg->vol_scale_ch[0] = 6;
	runmsg->vol_scale_ch[1] = 6;
	runmsg->trig_vol_level_ch[0] = 400 / 2 - 50;
	runmsg->trig_vol_level_ch[1] = 400 / 2 + 50;
	runmsg->time_scale = 5;
	osc_param_sys_set(&runmsg->sys_menu_set, 1, 7);
	osc_param_sys_set(&runmsg->sys_menu_set, 1, 10);
	runmsg->measure_item[0] = (1<<0)|(1<<2);
	runmsg->measure_item[1] = (1<<0)|(1<<2);
	const unsigned short cal_buffer0[14] = {6432,5982,5528,5079,4627,4175,3728,6437,5987,5534,5085,4632,4180,3735};
	memcpy(runmsg->pos_zero_pwm_ch1, cal_buffer0, sizeof(runmsg->pos_zero_pwm_ch1));
	memcpy(runmsg->pos_zero_pwm_ch2, cal_buffer0, sizeof(runmsg->pos_zero_pwm_ch2));
	runmsg->back_light_per = 60;
	runmsg->check = osc_param_check((unsigned char *)runmsg, 82);
	if(sd_ok)
		osc_sd_param_save(runmsg);
	else
		osc_flash_eeprom_write(runmsg);
	memcpy(&osc_m_tmp, runmsg, sizeof(osc_m_tmp));
	return FS_OK;
}
/* thread */

void osc_param_thread(void)
{
	static unsigned int pt0 = 0;
	if( hal_sys_time_us() - pt0 < 2 * 1000 * 1000 )
		return;
	pt0 = hal_sys_time_us();
	if( touch_idle_cnt < 150 )
		return;
	if( cal_step_start )
		return;
	touch_idle_cnt = 0;
	unsigned char * tmp = (unsigned char *)&osc_m_tmp;
	unsigned char * rum = (unsigned char *)runmsg;
	if( memcmp(tmp, rum, sizeof(osc_m_tmp)) != 0 )
	{
		runmsg->check = osc_param_check(rum, 82);
		if(osc_sd_card_ready())
			osc_sd_param_save(runmsg);
		else
			osc_flash_eeprom_write(runmsg);
		memcpy(tmp, rum, sizeof(osc_m_tmp));
	}
}
/* param save */
void osc_param_save_noload(void)
{
	unsigned char * tmp = (unsigned char *)&osc_m_tmp;
	unsigned char * rum = (unsigned char *)runmsg;
	if( memcmp(tmp, rum, sizeof(osc_m_tmp)) != 0 )
	{
		runmsg->check = osc_param_check(rum, 82);
		if(osc_sd_card_ready())
			osc_sd_param_save(runmsg);
		else
			osc_flash_eeprom_write(runmsg);
		memcpy(tmp, rum, sizeof(osc_m_tmp));
	}
}
/* restor the default param */
void osc_reset_param(void)
{
	/* need erase flash */
	HAL_FLASH_Unlock();
#if OSC_STM32H750	
	/* set */
	FLASH_Erase_Sector(7,FLASH_BANK_1,FLASH_VOLTAGE_RANGE_3);	
#else
	for( int i = 0 ; i < 16 ; i ++ )
	{
		FLASH_Erase_Sector(7*16+i,FLASH_BANK_1,0x20);	
	}
#endif	
	/* reset */
	NVIC_SystemReset();		
}