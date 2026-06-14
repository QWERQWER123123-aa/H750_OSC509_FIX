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
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dev.h"
#include "gui.h"
#include "boot.h"
#include "crc.h"
/* USER CODE END Includes */
#define ENABLE_INT()   __set_PRIMASK(0)
#define DISABLE_INT()  __set_PRIMASK(1)


unsigned int head_app_0 = 0xfabc2747;
unsigned int head_app_1 = 0xb546ee89;
	
/* decfe */
extern const unsigned int __FS_version_export[3];
/* decode */
unsigned int usb_write_active = 0;

static unsigned char dnd_state = 0;
static unsigned int dnd_tick = 0;
static unsigned int dnd_idle_tick = 0;
static unsigned int dnd_fsize = 0;
static unsigned int dnd_faddr = 0;
static __attribute__((aligned(32))) unsigned int dnd_buf[256];
static FIL dnd_file;
static DIR dnd_dir;
static FILINFO dnd_finfo;


/* USER CODE BEGIN 0 */

/* JumpToBootloader */
void JumpToapp(void)
{
	/* define */
	unsigned int i=0;
	void (*SysMemBootJump)(void); 
	__IO unsigned int BootAddr = APP_FLASH_ADDR; /* STM32H7  APP  */

	/* DISABLE INT */
	DISABLE_INT();

	/* SYSTICK DISABLE */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
  /* RCC DeInit */
	//HAL_RCC_DeInit();
  /* disable all it */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}
  /* enable int */
	ENABLE_INT();

	/* bootLoader MSP + 4 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));
	/* set MSP */
	__set_MSP(*(unsigned int *)BootAddr);
	__set_CONTROL(0);
  /* jump */
	SysMemBootJump();
	/* never arrival here */
	while (1)
	{
		
	}
}
/* app check */
unsigned int app_code_check(void)
{
	/* get version addr */
	unsigned int * cde = (unsigned int *)APP_FLASH_ADDR;
	/* check full APP region */
	for( int i = 0 ; i < 0x40000 / 4 ; i ++ )
	{
		/* get app version */
		if( cde[i] == head_app_0 && cde[i+2] == head_app_1)
		{
			return cde[i+1];
		}
	}
	/* return error */
	return 0;
}
/* boot loader thread */

void usb_dnd_flash_init(void)
{
	dnd_state = 0;
	dnd_tick = 0;
	dnd_idle_tick = 0;
	dnd_fsize = 0;
	dnd_faddr = 0;
	usb_write_active = 0;
}

void usb_dnd_flash_thread(void)
{
	FRESULT fr;
	UINT br;
	char bff[64];

	switch (dnd_state)
	{
	case 0:
		dnd_tick++;
		if (dnd_tick < 500000)
			break;
		dnd_tick = 0;

		if (usb_write_active)
		{
			dnd_idle_tick = 0;
			usb_write_active = 0;
			break;
		}

		dnd_idle_tick++;
		if (dnd_idle_tick < 3)
			break;
		dnd_idle_tick = 0;

		f_mount(0, USERPath, 0);
		fr = f_mount(&USERFatFS, USERPath, 1);
		if (fr != FR_OK)
		{
			clear_screen(1);  //清屏
			sprintf(bff, "DND: mount err %d", fr);
			show_string_static(bff);
			break;
		}

		fr = f_opendir(&dnd_dir, "");
		if (fr != FR_OK)
		{
			clear_screen(1);  //清屏
			sprintf(bff, "DND: opendir err %d", fr);
			show_string_static(bff);
			break;
		}

		dnd_state = 1;
		break;

	case 1:
		fr = f_readdir(&dnd_dir, &dnd_finfo);
		if (fr != FR_OK || dnd_finfo.fname[0] == 0)
		{
			f_closedir(&dnd_dir);
			clear_screen(1);  //清屏
			show_string_static("DND: no .bin found");
			dnd_state = 0;
			dnd_tick = 0;
			break;
		}

		if ((dnd_finfo.fattrib & AM_DIR) == 0 && dnd_finfo.fsize > 1024)
		{
			char *p = dnd_finfo.fname;
			while (*p) p++;
			if (p - dnd_finfo.fname >= 4)
			{
				p -= 4;
				if (p[0] == '.' && (p[1] == 'B' || p[1] == 'b') &&
				    (p[2] == 'I' || p[2] == 'i') && (p[3] == 'N' || p[3] == 'n'))
				{
					dnd_fsize = dnd_finfo.fsize;
						dnd_faddr = 0;

						if (dnd_fsize > APP_FLASH_SIZE)
						{
							clear_screen(1);
							sprintf(bff, "DND: file too big %dKB > %dKB", dnd_fsize / 1024, APP_FLASH_SIZE / 1024);
							show_string_static(bff);
							f_closedir(&dnd_dir);
							dnd_state = 0;
							dnd_tick = 0;
							break;
						}

						HAL_FLASH_Unlock();
					{
						unsigned int sec_cnt = (dnd_fsize + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE;
						for (unsigned int s = 0; s < sec_cnt && (APP_START_SECTOR + s) <= APP_END_SECTOR; s++)
						{
							FLASH_Erase_Sector(APP_START_SECTOR + s, FLASH_BANK_1, FLASH_VOLTAGE_RANGE_3);
						}
						FLASH_WaitForLastOperation(50000U, FLASH_BANK_1);
					}

					fr = f_open(&dnd_file, dnd_finfo.fname, FA_READ);
					if (fr != FR_OK)
					{
						clear_screen(1);  //清屏
						sprintf(bff, "DND: open err %d", fr);
						show_string_static(bff);
						f_closedir(&dnd_dir);
						dnd_state = 0;
						dnd_tick = 0;
						break;
					}

					clear_screen(1);  //清屏
					sprintf(bff, "DND: %s %dKB", dnd_finfo.fname, dnd_fsize / 1024);
					show_string(bff);
					show_string_static("DND: programming...");

					f_closedir(&dnd_dir);
					dnd_state = 2;
					break;
				}
			}
		}
		break;

	case 2:
	{
		unsigned int chunk = 512;
		if (dnd_faddr + chunk > dnd_fsize)
			chunk = dnd_fsize - dnd_faddr;
		if (chunk == 0)
		{
			f_close(&dnd_file);
			HAL_FLASH_Lock();
			
			/* Clean D-Cache for the entire APP region to ensure fresh instruction fetch */
			//SCB_CleanInvalidateDCache_by_Addr((uint32_t *)APP_FLASH_ADDR, APP_FLASH_SIZE);
			
			clear_screen(1);  //清屏
			show_string("DND: flash complete!");
			sprintf(bff, "DND: %d bytes flashed", dnd_fsize);
			show_string(bff);
			dnd_state = 3;
			dnd_tick = 0;
			break;
		}

		fr = f_read(&dnd_file, dnd_buf, chunk, &br);
		if (fr != FR_OK || br == 0)
		{
			f_close(&dnd_file);
			clear_screen(1);  //清屏
			if (br == 0)
				show_string("DND: read EOF err!");
			else
			{
				sprintf(bff, "DND: read err %d", fr);
				show_string_static(bff);
			}
			dnd_state = 0;
			dnd_tick = 0;
			break;
		}

		/* Clean D-Cache before programming, size aligned to 32 bytes (cache line) */
		//unsigned int cache_size = (br + 31U) & ~31U;
		//SCB_CleanDCache_by_Addr((uint32_t *)dnd_buf, cache_size);

		int prog_err = 0;
		for (unsigned int i = 0; i < (br + 31U)/32; i++)  // br/32 rounded up
		{
			if (i * 32 >= br)
				break;  // don't write beyond what we read
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,
			                      APP_FLASH_ADDR + dnd_faddr + i * 32,
			                      (unsigned int)&dnd_buf[i * 8]) != HAL_OK)
			{
				uint32_t err = FLASH->SR1;
				f_close(&dnd_file);
				HAL_FLASH_Lock();
				clear_screen(1);  //清屏
				sprintf(bff, "DND: prog err @0x%08X", APP_FLASH_ADDR + dnd_faddr + i * 32);
				show_string_static(bff);
				sprintf(bff, "SR1=0x%08X", (unsigned int)err);
				show_string_static(bff);
				dnd_state = 0;
				dnd_tick = 0;
				prog_err = 1;
				break;
			}
		}
		
		if (prog_err)
			break;

		/* Wait for all operations to complete and check errors */
		if (FLASH_WaitForLastOperation(50000U, FLASH_BANK_1) != HAL_OK)
		{
			uint32_t err = HAL_FLASH_GetError();
			f_close(&dnd_file);
			HAL_FLASH_Lock();
			clear_screen(1);  //清屏
			sprintf(bff, "DND: wait err 0x%08X", (unsigned int)err);
			show_string_static(bff);
			dnd_state = 0;
			dnd_tick = 0;
			break;
		}

		dnd_faddr += br;

		if ((dnd_faddr & 0x1FFF) == 0 || dnd_faddr >= dnd_fsize)
		{
			clear_screen(1);  //清屏
			sprintf(bff, "DND: %d/%d KB", dnd_faddr / 1024, dnd_fsize / 1024);
			show_string_static(bff);
		}
		break;
	}

	case 3:
		dnd_tick++;
		if (dnd_tick > 1500000)
		{
			dnd_tick = 0;
			dnd_state = 4;
			clear_screen(1);  //清屏
			show_string_static("DND: jumping to APP...");
		}
		break;

	case 4:
		dnd_tick++;
		if (dnd_tick > 500000)
		{
			JumpToapp();
		}
		break;

	default:
		dnd_state = 0;
		break;
	}
}