/**
  ******************************************************************************
  * @file    stm32h747i_eval_lcd.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    29-December-2017
  * @brief   This file includes the driver for Liquid Crystal Display (LCD) module
  *          mounted on stm32h747I-EVAL evaluation board.
  @verbatim
  How To use this driver:
  -----------------------
   - This driver is used to drive directly an LCD TFT using the LTDC controller.
   - This driver selects dynamically the mounted LCD, AMPIRE 640x480 LCD mounted
     on MB1063 or AMPIRE 480x272 LCD mounted on MB1046 daughter board,
     and uses the adequate timing and setting for the specified LCD using
     device ID of the STMPE811 mounted on MB1046 daughter board.

  Driver description:
  ------------------
  + Initialization steps:
     o Initialize the LCD using the BSP_LCD_Init() function.
     o Apply the Layer configuration using the BSP_LCD_LayerDefaultInit() function.
     o Select the LCD layer to be used using the BSP_LCD_SelectLayer() function.
     o Enable the LCD display using the BSP_LCD_DisplayOn() function.

  + Options
     o Configure and enable the colour keying functionality using the
       BSP_LCD_SetColorKeying() function.
     o Modify in the fly the transparency and/or the frame buffer address
       using the following functions:
       - BSP_LCD_SetTransparency()
       - BSP_LCD_SetLayerAddress()

  + Display on LCD
     o Clear the whole LCD using BSP_LCD_Clear() function or only one specified string
       line using the BSP_LCD_ClearStringLine() function.
     o Display a character on the specified line and column using the BSP_LCD_DisplayChar()
       function or a complete string line using the BSP_LCD_DisplayStringAtLine() function.
     o Display a string line on the specified position (x,y in pixel) and align mode
       using the BSP_LCD_DisplayStringAtLine() function.
     o Draw and fill a basic shapes (dot, line, rectangle, circle, ellipse, .. bitmap)
       on LCD using the available set of functions.
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_eval_lcd_ltdc.h"

#include "../Components/lq035nc111/lq035nc111.h"
#include "../Components/ampire480272/ampire480272.h"
#include "../Components/ampire640480/ampire640480.h"
#include "../Components/at800480/at800480.h"
#include "../Components/n101l6/n101l6.h"
#include "../Components/ht12x21/ht12x21.h"
#include "../Components/g104age/g104age.h"

/**
  * @brief  Clock Config.
  * @param  hltdc: LTDC handle
  * @note   This API is called by BSP_LCD_Init()
  * @retval None
  */
void BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

#ifdef USE_320x240
	/* The LQ035NC111 LCD 320x240 is selected */
	/* LCD clock configuration */	
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/37 = 6 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 6 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 56;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
#ifdef USE_480x272
	/* The AMPIRE LCD 480x272 is selected */
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/37 = 9.08 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 9.08 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 37;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
#ifdef USE_640x480
  /* The 640x480 LCD is selected */
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/14 = 24 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 24 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 14;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
#ifdef USE_800x480
	/* The 800x480 LCD is selected */
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/14 = 24 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 24 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 14;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
#ifdef USE_800x600
	/* The 800x480 LCD is selected */
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/14 = 24 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 24 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 14;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
#ifdef USE_1024x600
	/* The 1024x600 LCD is selected */
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/14 = 24 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 24 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 14;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
#ifdef USE_1024x768
	/* The 1024x768 LCD is selected */
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 25Mhz/5 = 5 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N  = 5*160 = 800 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/25 = 32 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 32 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 5;
	periph_clk_init_struct.PLL3.PLL3N = 160;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 2;
	periph_clk_init_struct.PLL3.PLL3R = 25;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
	
	/* LCD clock configuration */
	/* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
	/* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 336 Mhz */
	/* PLLLCDCLK = PLL3_VCO Output/PLL3R = 336/10 = 33.6 Mhz */
	/* LTDC clock frequency = PLLLCDCLK = 33.6 Mhz */
	/* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 48 Mhz */
	periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	periph_clk_init_struct.PLL3.PLL3M = 25;
	periph_clk_init_struct.PLL3.PLL3N = 336;
	periph_clk_init_struct.PLL3.PLL3P = 2;
	periph_clk_init_struct.PLL3.PLL3Q = 7;
	periph_clk_init_struct.PLL3.PLL3R = 10;
	HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
#endif
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

