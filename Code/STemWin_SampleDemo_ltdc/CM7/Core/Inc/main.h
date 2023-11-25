/**
  ******************************************************************************
  * @file    StemWin/StemWin_fonts/CM7/Core/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "GUI.h"

/* EVAL includes component */
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_lcd.h"
#include "stm32h747i_eval_sdram.h"
#if defined (USE_FT5426) 
	#include "stm32h747i_eval_ft5426_ts.h"
#elif defined (USE_FT5336) 
  #include "stm32h747i_eval_ft5336_ts.h"
#elif defined (USE_FT5316) 
  #include "stm32h747i_eval_ft5316_ts.h"
#elif defined (USE_GOODIX) 
  #include "stm32h747i_eval_goodix_ts.h"
#else
	#include "stm32h747i_eval_tsc2007_ts.h"
#endif
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
