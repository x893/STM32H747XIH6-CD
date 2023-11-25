/**
  ******************************************************************************
  * @file    goodix.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-June-2015
  * @brief   This file contains all the functions prototypes for the
  *          ft5336.c Touch screen driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#ifndef __GOODIX_H
#define __GOODIX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Set Multi-touch as supported */
#if !defined(TS_MONO_TOUCH_SUPPORTED)
#define TS_MULTI_TOUCH_SUPPORTED        1
#endif /* TS_MONO_TOUCH_SUPPORTED */

/* Includes ------------------------------------------------------------------*/
#include "../Common/ts.h"

/* Touch screen driver structure */
extern TS_DrvTypeDef Goodix_ts_drv;

/* Macros --------------------------------------------------------------------*/
#define TP_INT_Pin 				GPIO_PIN_15
#define TP_INT_GPIO_Port 	GPIOA
#define TP_INT_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

#define TP_RST_Pin 				GPIO_PIN_10
#define TP_RST_GPIO_Port 	GPIOF
#define TP_RST_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()

/* Possible values of global variable 'TS_I2C_Initialized' */

#define GOODIX_ADDRESS											0xBA

#define GOODIX_I2C_NOT_INITIALIZED          ((uint8_t)0x00)
#define GOODIX_I2C_INITIALIZED              ((uint8_t)0x01)

/* Maximum border values of the touchscreen pad */
#define GOODIX_MAX_WIDTH              			((uint16_t)480)     /* Touchscreen pad max width   */
#define GOODIX_MAX_HEIGHT             			((uint16_t)272)     /* Touchscreen pad max height  */

/* Max detectable simultaneous touches */
#define GOODIX_MAX_DETECTABLE_TOUCH         ((uint8_t)0x05)

typedef struct
{
  uint8_t i2cInitialized;

  /* field holding the current number of simultaneous active touches */
  uint8_t currActiveTouchNb;

  /* field holding the touch index currently managed */
  uint8_t currActiveTouchIdx;

} Goodix_handle_TypeDef;

void Goodix_Init(uint16_t DeviceAddr);
void Goodix_Reset(uint16_t DeviceAddr);
void Goodix_TS_Start(uint16_t DeviceAddr);
void Goodix_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y);
void Goodix_TS_EnableIT(uint16_t DeviceAddr);
void Goodix_TS_ClearIT(uint16_t DeviceAddr);
void Goodix_TS_DisableIT(uint16_t DeviceAddr);
uint8_t Goodix_TS_ITStatus(uint16_t DeviceAddr);
uint8_t Goodix_TS_DetectTouch(uint16_t DeviceAddr);
uint16_t Goodix_ReadID(uint16_t DeviceAddr);

extern void     I2Cx_Init(void);
#ifdef __cplusplus
}
#endif
#endif /* __FT5336_H */


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
