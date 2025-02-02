/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DCMI_D4_Pin LL_GPIO_PIN_4
#define DCMI_D4_GPIO_Port GPIOE
#define DCMI_D6_Pin LL_GPIO_PIN_5
#define DCMI_D6_GPIO_Port GPIOE
#define DCMI_D7_Pin LL_GPIO_PIN_6
#define DCMI_D7_GPIO_Port GPIOE
#define DCMI_HSNYC_Pin LL_GPIO_PIN_4
#define DCMI_HSNYC_GPIO_Port GPIOA
#define DCMI_PIXCLK_Pin LL_GPIO_PIN_6
#define DCMI_PIXCLK_GPIO_Port GPIOA
#define LED_D3_Pin LL_GPIO_PIN_7
#define LED_D3_GPIO_Port GPIOA
#define FLASH_CS_Pin LL_GPIO_PIN_0
#define FLASH_CS_GPIO_Port GPIOB
#define LCD_BL_Pin LL_GPIO_PIN_1
#define LCD_BL_GPIO_Port GPIOB
#define LCD_DC_Pin LL_GPIO_PIN_10
#define LCD_DC_GPIO_Port GPIOB
#define LCD_RST_Pin LL_GPIO_PIN_11
#define LCD_RST_GPIO_Port GPIOB
#define LCD_CS_Pin LL_GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SCK_Pin LL_GPIO_PIN_13
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_MOSI_Pin LL_GPIO_PIN_15
#define LCD_MOSI_GPIO_Port GPIOB
#define CAM_PDN_Pin LL_GPIO_PIN_11
#define CAM_PDN_GPIO_Port GPIOD
#define DCMI_D0_Pin LL_GPIO_PIN_6
#define DCMI_D0_GPIO_Port GPIOC
#define DCMI_D1_Pin LL_GPIO_PIN_7
#define DCMI_D1_GPIO_Port GPIOC
#define WLAN_D0_Pin LL_GPIO_PIN_8
#define WLAN_D0_GPIO_Port GPIOC
#define WLAN_D1_Pin LL_GPIO_PIN_9
#define WLAN_D1_GPIO_Port GPIOC
#define CAM_RST_Pin LL_GPIO_PIN_12
#define CAM_RST_GPIO_Port GPIOA
#define WLAN_PDN_Pin LL_GPIO_PIN_15
#define WLAN_PDN_GPIO_Port GPIOA
#define WLAN_D2_Pin LL_GPIO_PIN_10
#define WLAN_D2_GPIO_Port GPIOC
#define WLAN_D3_Pin LL_GPIO_PIN_11
#define WLAN_D3_GPIO_Port GPIOC
#define WLAN_CK_Pin LL_GPIO_PIN_12
#define WLAN_CK_GPIO_Port GPIOC
#define WLAN_CMD_Pin LL_GPIO_PIN_2
#define WLAN_CMD_GPIO_Port GPIOD
#define FLASH_SCK_Pin LL_GPIO_PIN_3
#define FLASH_SCK_GPIO_Port GPIOB
#define FLASH_MISO_Pin LL_GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin LL_GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB
#define DCMI_D5_Pin LL_GPIO_PIN_6
#define DCMI_D5_GPIO_Port GPIOB
#define DCMI_VSYNC_Pin LL_GPIO_PIN_7
#define DCMI_VSYNC_GPIO_Port GPIOB
#define SCCB_SCL_Pin LL_GPIO_PIN_8
#define SCCB_SCL_GPIO_Port GPIOB
#define SCCB_SDA_Pin LL_GPIO_PIN_9
#define SCCB_SDA_GPIO_Port GPIOB
#define DCMI_D2_Pin LL_GPIO_PIN_0
#define DCMI_D2_GPIO_Port GPIOE
#define DCMI_D3_Pin LL_GPIO_PIN_1
#define DCMI_D3_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
