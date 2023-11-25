/*----------------------------------------------------------------------------------------------------------*/
/*                                                                                                          */
/* Copyright by Huang Ying Xin                                                          */
/* All rights reserved.                                                                                     */
/*                                                                                                          */
/*------------------------------------------------------------------------------------------------------------
  File Name        : tsc2007.h
  Version          : V0.1
  Date[mm/dd/yyyy] : 06/11/2016
  Description      : The header file of tsc2007.c module.
------------------------------------------------------------------------------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __TSC2007_H
#define __TSC2007_H

#include "stdint.h"
#include "../Common/ts.h"
#include "../Common/io.h"

/* Includes ------------------------------------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------------------------------------*/
#define TSC2007_8_bit             0x02
#define TSC2007_12_bit		        0x00

#define Measure_X_Select          0xC0
#define Measure_Y_Select          0xD0
#define Measure_Z1_Select         0xE0
#define Measure_Z2_Select         0xF0

#define Converter_Mode            TSC2007_8_bit
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void tsc2007_Init(void);
uint16_t tsc2007_MeasureXpos(void);
uint16_t tsc2007_MeasureYpos(void);
uint16_t tsc2007_MeasureZ1pos(void);
uint16_t tsc2007_MeasureZ2pos(void);
uint8_t  tsc2007_TS_DetectTouch(void);

#endif /* __TSC2007_H ---------------------------------------------------------------------------------*/
