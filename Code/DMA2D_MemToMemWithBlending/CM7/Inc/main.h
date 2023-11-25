/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_lcd.h"

#define LAYER_SIZE_X      240
#define LAYER_SIZE_Y      130
#define LAYER_BYTE_PER_PIXEL  2


#define LCD_FRAME_BUFFER  0xD0000000

#ifdef USE_320x240
	#define LCD_RES_X  320
	#define LCD_RES_Y  240
#endif
#ifdef USE_480x272
	#define LCD_RES_X  480
	#define LCD_RES_Y  272
#endif
#ifdef USE_640x480
	#define LCD_RES_X  640
	#define LCD_RES_Y  480
#endif
#ifdef USE_800x480
	#define LCD_RES_X  800
	#define LCD_RES_Y  480
#endif
#ifdef USE_800x600
	#define LCD_RES_X  800
	#define LCD_RES_Y  600
#endif
#ifdef USE_1024x600
	#define LCD_RES_X  1024
	#define LCD_RES_Y  600
#endif
#ifdef USE_1280x720
	#define LCD_RES_X  720
	#define LCD_RES_Y  1280
#endif

#define ARGB8888_BYTE_PER_PIXEL  4
#define RGB565_BYTE_PER_PIXEL    2

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
