/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_ULPM_DataClock/CM7/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module for Cortex-M7.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */
/**
  * @brief  Initializes the DSI MSP.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_MspInit(DSI_HandleTypeDef* hdsi)
{
  RCC_PeriphCLKInitTypeDef   PeriphClkInitStruct;
  
      
  /* Enable  PLL2. */
  __HAL_RCC_PLL2_DISABLE();
  
  /* DSI clock configuration */
  /* PLL2_VCO Input = HSE_VALUE/PLL2M = 5 Mhz */
  /* PLL2_VCO Output = PLL2_VCO Input * PLL2N = 800 Mhz */
  /* PLLDSICLK = PLL2_VCO Output/PLL2Q = 800/10 = 80 Mhz */
  PeriphClkInitStruct.DsiClockSelection = RCC_DSICLKSOURCE_PLL2;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DSI;
  PeriphClkInitStruct.PLL2.PLL2M = 5;		
  PeriphClkInitStruct.PLL2.PLL2N = 160;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 10;	
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  __HAL_RCC_DSI_CONFIG(RCC_PERIPHCLK_DSI);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
