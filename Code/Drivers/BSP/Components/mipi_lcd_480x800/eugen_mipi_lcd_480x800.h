/**
  ******************************************************************************
  * @file    mipi_lcd_480x800.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    27-January-2017
  * @brief   
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EUGEN_MIPI_LCD_480X800_H
#define __EUGEN_MIPI_LCD_480X800_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include <stdint.h>
#include "stm32h7xx_hal.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup MIPI_LCD
  * @{
  */

/** @addtogroup MIPI_LCD_Exported_Variables
  * @{
  */

#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */
#endif /* __GNUC__ */

#define MIPI_LCD_ID  ((uint32_t) 0)

#define MIPI_DCS_EXIT_SLEEP_MODE 0x11
#define MIPI_DCS_SET_DISPLAY_ON	 0x29
      
/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define MIPI_LCD_ORIENTATION_PORTRAIT    ((uint32_t)0x00) /* Portrait orientation choice of LCD screen  */
#define MIPI_LCD_ORIENTATION_LANDSCAPE   ((uint32_t)0x01) /* Landscape orientation choice of LCD screen */

/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */
#define MIPI_LCD_FORMAT_RGB888    ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
#define MIPI_LCD_FORMAT_RGB565    ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

/**
  * @brief  MIPI_LCD_480X800 Size
  */

/* Width and Height in Portrait mode */
#define  MIPI_LCD_480X800_WIDTH             ((uint16_t)480)     /* LCD PIXEL WIDTH   */
#define  MIPI_LCD_480X800_HEIGHT            ((uint16_t)800)     /* LCD PIXEL HEIGHT  */

/**
  * @brief  MIPI_LCD_480X800 Timing parameters for Portrait orientation mode
  */
#define  MIPI_LCD_480X800_HSYNC             ((uint16_t)20)      /* Horizontal synchronization */
#define  MIPI_LCD_480X800_HBP               ((uint16_t)20)     /* Horizontal back porch      */
#define  MIPI_LCD_480X800_HFP               ((uint16_t)20)     /* Horizontal front porch     */
#define  MIPI_LCD_480X800_VSYNC             ((uint16_t)4)      /* Vertical synchronization   */
#define  MIPI_LCD_480X800_VBP               ((uint16_t)18)      /* Vertical back porch        */
#define  MIPI_LCD_480X800_VFP               ((uint16_t)16)      /* Vertical front porch       */

/**
  * @brief  MIPI_LCD_480X800 Timing parameters for Landscape orientation mode
  *         Same values as for Portrait mode in fact.
  */

#define MIPI_LCD_CMD_CASET                 0x2A  /* Column address set command */
#define MIPI_LCD_CMD_PASET                 0x2B  /* Page address set command */

#define MIPI_LCD_CMD_DISPOFF               0x28  /* Display Off command */
#define MIPI_LCD_CMD_DISPON                0x29  /* Display On command */

/* CABC Management : ie : Content Adaptive Back light Control in IC OTM8009a */
#define MIPI_LCD_CMD_WRDISBV               0x51  /* Write Display Brightness command          */
#define MIPI_LCD_CMD_WRCTRLD               0x53  /* Write CTRL Display command                */
#define MIPI_LCD_CMD_WRCABC                0x55  /* Write Content Adaptive Brightness command */
#define MIPI_LCD_CMD_WRCABCMB              0x5E  /* Write CABC Minimum Brightness command     */

/**
  * @brief  MIPI_LCD_480X800 frequency divider
  */
#define MIPI_LCD_480X800_FREQUENCY_DIVIDER  2   /* LCD Frequency divider      */

/**
  * @}
  */
   
/* Exported macro ------------------------------------------------------------*/
   
/** @defgroup MIPI_LCD_Exported_Macros MIPI_LCD Exported Macros
  * @{
  */ 

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
  
/** @addtogroup MIPI_LCD_Exported_Functions
  * @{
  */
//void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);
void DSI_WriteCmd(uint8_t cmd, uint8_t data);
uint8_t Eugen_MIPI_480x800_Init(uint32_t ColorCoding, uint32_t orientation);
extern void MIPI_LCD_IO_Delay(uint32_t Delay);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __MIPI_LCD_480X800_H */
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
