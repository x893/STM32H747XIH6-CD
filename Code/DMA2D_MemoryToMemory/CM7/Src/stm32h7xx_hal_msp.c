/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemoryToMemory/CM7/Src/stm32h7xx_hal_msp.c
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

/** @addtogroup DMA2D_MemoryToMemory
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
  * @brief DMA2D MSP Initialization
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /*##-2- NVIC configuration  ################################################*/
  /* NVIC configuration for DMA2D transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief DMA2D MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *        - Reset Peripheral  
  *        - Disable the Peripheral's clock
  *        - Deactivate Peripheral IRQ
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{  
  /*##-1- Reset peripherals ##################################################*/
  /* Enable DMA2D reset state */
  __HAL_RCC_DMA2D_FORCE_RESET();

  /* Release DMA2D from reset state */
  __HAL_RCC_DMA2D_RELEASE_RESET();
  
  /* ##-2- Disable DMA2D Clock ##################################################*/
  __HAL_RCC_DMA2D_CLK_DISABLE();
  
  /* ##-3- Disable DMA2D IRQ in NVIC #############################################*/
  HAL_NVIC_DisableIRQ(DMA2D_IRQn);
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
