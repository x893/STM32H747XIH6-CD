/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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


#include "stm32h7xx_it.h"

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma2d.h"
//#include "stm32h7xx_hal_dsi.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_jpeg.h"

#include <stdbool.h>

#include "hwjpeg_decode.h"

#include <cmsis_os.h>

#ifdef USE_STM32H747I_EVAL
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_io.h"
#endif

extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef  hltdc;
extern HCD_HandleTypeDef   hhcd;
extern SD_HandleTypeDef    uSdHandle;

extern struct HwJPEG_Context_s HwJPEG_Context;

static volatile int overrunCnt;

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  SysTick interrupt handler
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  osSystickHandler();
}

/**
  * @brief  DMA2D interrupt handler
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&hdma2d);
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hltdc);
}

/**
  * @brief  LTDC_ER interrupt handler
  * @param  None
  * @retval None
  */
void LTDC_ER_IRQHandler(void)
{
  if (LTDC->ISR & 2)
  {
    LTDC->ICR = 2;
    overrunCnt++;
  }
}

/**
  * @brief  This function handles USB-On-The-Go HS global interrupt request.
  * @param  None
  * @retval None
  */
void OTG_HS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&hhcd);
}

void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);
}

void JPEG_IRQHandler(void)
{
  HAL_JPEG_IRQHandler(&HwJPEG_Context.hjpeg);
}

void MDMA_IRQHandler(void)
{
  HAL_MDMA_IRQHandler(HwJPEG_Context.hjpeg.hdmain);
  HAL_MDMA_IRQHandler(HwJPEG_Context.hjpeg.hdmaout);
}

/**
 * @brief  This function handles SDMMC interrupt request.
 * @param  None
 * @retval None
 */
void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}
