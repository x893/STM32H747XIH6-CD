/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"

/* EVAL includes component */
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_sdram.h"
#include "stm32h747i_eval_lcd.h"
#include "stm32h747i_eval_io.h"
#include "stm32h747i_eval_sd.h"
#include "stm32h747i_eval_audio.h"
#include "stm32h747i_eval_qspi.h"

#if defined (USE_FT5426)
	#include "stm32h747i_eval_ft5426_ts.h"
#else
	#include "stm32h747i_eval_tsc2007_ts.h"
#endif
                 
/* FatFs includes components */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include <cmsis_os.h>

   
/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"
#include "ST_GUI_Addons.h"

 /* Kernel includes components */
#include "k_module.h"
#include "storage.h"
#include "k_rtc.h"
#include "k_bsp.h"
#include "k_mfx.h"
#include "k_calibration.h"
   
/* utilities and standard includes components */   
#include "cpu_utils.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/* Dual-core IPC */
#include "cm_ipc.h"

/* Exported types ------------------------------------------------------------*/
typedef enum module_id_e
{
  MODULE_MAIN_MENU        = 0
, MODULE_AUDIO_PLAYER     /* 1 */
, MODULE_VIDEO_PLAYER     /* 2 */
, MODULE_GAME             /* 3 */
, MODULE_ANALOG_CLOCK     /* 4 */
, MODULE_GRAPHIC_EFFECTS  /* 5 */
, MODULE_DUAL_CORE        /* 6 */
, MODULE_COUNT
} module_id_t;

typedef struct Bitmap_Res_s
{
  uint16_t       x;
  uint16_t       y;
} Bitmap_Res_t;

typedef struct Demo_Header_s
{
  uint32_t                demo_id;
  const uint32_t * const  icon_addr;
  Bitmap_Res_t            icon_res;
  const uint32_t * const  logo_addr;
  Bitmap_Res_t            logo_res;
  uint32_t                sw_flags;
  uint32_t                reserved[2];
  unsigned char           demo_descr[32];
} Demo_Header_t;

typedef union
{
  struct
  {
    uint32_t     A1 : 15;
    uint32_t     B1 : 16;
    uint32_t     Reserved : 1;
  }b;
  uint32_t d32;
}CALIBRATION_Data1Typedef;

typedef union
{
  struct
  {
    uint32_t      A2 : 15;
    uint32_t      B2 : 16;
    uint32_t      IsCalibrated : 1;
  }b;
  uint32_t d32;

}CALIBRATION_Data2Typedef;
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SW_FLAG_AUTO_DEMO         (1L << 0)

#define AUTO_DEMO_TIMEOUT_0       20
#define AUTO_DEMO_TIMEOUT_1        5
#define AUTO_DEMO_STEP_TIMEOUT     5
#define BKP_REG_PWR_CFG           (RTC->BKP28R)
#define BKP_REG_SW_CFG            (RTC->BKP27R)
#define BKP_REG_SUBDEMO_ADDRESS   (RTC->BKP26R)
#define BKP_REG_CALIB_DR0         (RTC->BKP25R)
#define BKP_REG_CALIB_DR1         (RTC->BKP24R)

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void Error_Handler( void );
void BSP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
