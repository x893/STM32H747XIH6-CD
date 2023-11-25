/**
  ******************************************************************************
  * @file    QSPI/QSPI_MemoryMappedDual/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
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
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_qspi_mxic.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for QSPI clock resources */
#define QSPI_CLK_ENABLE()              __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()             __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_CS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_BK1_D0_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D3_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

#define QSPI_BK2_D0_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOH_CLK_ENABLE()
#define QSPI_BK2_D1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOH_CLK_ENABLE()
#define QSPI_BK2_D2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_BK2_D3_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()

#define QSPI_MDMA_CLK_ENABLE()         __HAL_RCC_MDMA_CLK_ENABLE()

#define QSPI_FORCE_RESET()             __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()           __HAL_RCC_QSPI_RELEASE_RESET()

/* Definition for QSPI Pins */
#define QSPI_CS_PIN                    GPIO_PIN_6
#define QSPI_CS_GPIO_PORT              GPIOG
#define QSPI_CLK_PIN                   GPIO_PIN_2
#define QSPI_CLK_GPIO_PORT             GPIOB
#define QSPI_BK1_D0_PIN                GPIO_PIN_8
#define QSPI_BK1_D0_GPIO_PORT          GPIOF
#define QSPI_BK1_D1_PIN                GPIO_PIN_9
#define QSPI_BK1_D1_GPIO_PORT          GPIOF
#define QSPI_BK1_D2_PIN                GPIO_PIN_7
#define QSPI_BK1_D2_GPIO_PORT          GPIOF
#define QSPI_BK1_D3_PIN                GPIO_PIN_6
#define QSPI_BK1_D3_GPIO_PORT          GPIOF

#define QSPI_BK2_D0_PIN                GPIO_PIN_2
#define QSPI_BK2_D0_GPIO_PORT          GPIOH
#define QSPI_BK2_D1_PIN                GPIO_PIN_3
#define QSPI_BK2_D1_GPIO_PORT          GPIOH
#define QSPI_BK2_D2_PIN                GPIO_PIN_9
#define QSPI_BK2_D2_GPIO_PORT          GPIOG
#define QSPI_BK2_D3_PIN                GPIO_PIN_14
#define QSPI_BK2_D3_GPIO_PORT          GPIOG


/* Size of buffers */
#define BUFFERSIZE                 (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
