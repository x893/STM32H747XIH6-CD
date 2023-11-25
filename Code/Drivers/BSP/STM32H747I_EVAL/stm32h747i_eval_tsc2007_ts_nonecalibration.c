/**
  ******************************************************************************
  * @file    stm32f7xx_eval_tsc2007_ts.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    22-April-2016
  * @brief   This file provides a set of functions needed to manage the Touch 
  *          Screen on STM32756G-EVAL and STM32746G-EVAL evaluation boards.
  @verbatim
  How To use this driver:
  -----------------------
   - This driver is used to drive the touch screen module of the STM32756G-EVAL
     evaluation board on the AMPIRE 640x480 LCD mounted on MB1063 or AMPIRE 
     480x272 LCD mounted on MB1046 daughter board.
   - If the AMPIRE 640x480 LCD is used, the TS3510 or EXC7200 component driver
     must be included according to the touch screen driver present on this board.
   - If the AMPIRE 480x272 LCD is used, the STMPE811 IO expander device component 
     driver must be included in order to run the TS module commanded by the IO 
     expander device, the MFXSTM32L152 IO expander device component driver must be
     also included in case of interrupt mode use of the TS.

  Driver description:
  ------------------
  + Initialization steps:
     o Initialize the TS module using the BSP_TS_Init() function. This 
       function includes the MSP layer hardware resources initialization and the
       communication layer configuration to start the TS use. The LCD size properties
       (x and y) are passed as parameters.
     o If TS interrupt mode is desired, you must configure the TS interrupt mode
       by calling the function BSP_TS_ITConfig(). The TS interrupt mode is generated
       as an external interrupt whenever a touch is detected. 
       The interrupt mode internally uses the IO functionalities driver driven by
       the IO expander, to configure the IT line.
  
  + Touch screen use
     o The touch screen state is captured whenever the function BSP_TS_GetState() is 
       used. This function returns information about the last LCD touch occurred
       in the TS_StateTypeDef structure.
     o If TS interrupt mode is used, the function BSP_TS_ITGetStatus() is needed to get
       the interrupt status. To clear the IT pending bits, you should call the 
       function BSP_TS_ITClear().
     o The IT is handled using the corresponding external interrupt IRQ handler,
       the user IT callback treatment is implemented on the same external interrupt
       callback.
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "stm32h743i_eval_tsc2007_ts.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32756G_EVAL
  * @{
  */ 
  
/** @defgroup STM32756G_EVAL_TS
  * @{
  */   

/** @defgroup STM32756G_EVAL_TS_Private_Types_Definitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32756G_EVAL_TS_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32756G_EVAL_TS_Private_Macros
  * @{
  */ 
/**
  * @}
  */  

/** @defgroup STM32756G_EVAL_TS_Private_Variables
  * @{
  */ 
//static TS_DrvTypeDef *tsDriver; 
/**
  * @}
  */ 

/** @defgroup STM32756G_EVAL_TS_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32756G_EVAL_TS_Private_Functions
  * @{
  */ 

/**
  * @brief  Initializes and configures the touch screen functionalities and 
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  xSize: Maximum X size of the TS area on LCD
  *         ySize: Maximum Y size of the TS area on LCD  
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_Init(uint16_t xSize, uint16_t ySize)
{
  uint8_t status = TS_OK;
  
  tsc2007_Init();
  
  return status;
}

/**
  * @brief  DeInitializes the TouchScreen.
  * @retval TS state
  */
uint8_t BSP_TS_DeInit(void)
{ 
  /* Actually ts_driver does not provide a DeInit function */
  return TS_OK;
}

/**
  * @brief  Configures and enables the touch screen interrupts.
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_ITConfig(void)
{
  return TS_OK;  
}

/**
  * @brief  Gets the touch screen interrupt status.
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_ITGetStatus(void)
{
  /* Return the TS IT status */
  return TS_OK;
}

/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TS_State: Pointer to touch screen current state structure
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
#if defined (USE_320x240) || defined (USE_480x272) || defined (USE_800x480)  || defined (USE_800x600)
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
	TS_State->TouchDetected = tsc2007_TS_DetectTouch();
  
  if(TS_State->TouchDetected)
  {
//		TS_State->x = tsc2007_MeasureXpos();
//		TS_State->y = tsc2007_MeasureYpos();
		TS_State->x = tsc2007_MeasureXpos();
		TS_State->y = tsc2007_MeasureYpos();
	}
  return TS_OK;
}
#endif
#if defined (USE_640x480)
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
	TS_State->TouchDetected = tsc2007_TS_DetectTouch();
  
  if(TS_State->TouchDetected)
  {
		//Swap x and y
//		TS_State->x = tsc2007_MeasureYpos();
//		TS_State->y = tsc2007_MeasureXpos();
		TS_State->x = tsc2007_MeasureYpos();
		TS_State->y = tsc2007_MeasureXpos();
	}
  return TS_OK;
}
#endif
/**
  * @brief  Clears all touch screen interrupts.
  * @retval None
  */
void BSP_TS_ITClear(void)
{
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
