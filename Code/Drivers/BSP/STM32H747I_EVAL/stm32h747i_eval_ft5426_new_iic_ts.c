/**
  ******************************************************************************
  * @file    stm32h747i_eval_ft5426_new_iic_ts.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the Touch
  *          Screen on STM32H747I_EVAL board.
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

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the FT5426 touch screen module
     mounted over TFT-LCD on the STM32H747I_EVAL board.

2. Driver description:
---------------------
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
       in the TS_State_t structure.
     o The IT is handled using the corresponding external interrupt IRQ handler,
       the user IT callback treatment is implemented on the same external interrupt
       callback.

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_eval_ft5426_new_iic_ts.h"
#include "stm32h747i_eval_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_EVAL
  * @{
  */

/** @defgroup STM32H747I_EVAL_TS TS
  * @{
  */

/** @defgroup STM32H747I_EVAL_TS_Private_Types_Definitions Private Types Definitions
  * @{
  */
typedef void (* BSP_EXTI_LineCallback) (void);
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_TS_Private_Defines Private Types Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_TS_Private_Macros Private Macros
  * @{
  */
#define TS_MIN(a,b) ((a > b) ? b : a)
/**
  * @}
  */
/** @defgroup STM32H747I_EVAL_TS_Private_Function_Prototypes Private Function Prototypes
  * @{
  */
static int32_t FT5426_Probe(uint32_t Instance);
//static void TS_EXTI_Callback(void);
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_TS_Privates_Variables Privates Variables
  * @{
  */
//static EXTI_HandleTypeDef hts_exti[TS_INSTANCES_NBR] = {0};
static TS_Drv_t           *Ts_Drv = NULL;
void               *Ts_CompObj[TS_INSTANCES_NBR] = {0};
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_TS_Exported_Variables Exported Variables
  * @{
  */
TS_Ctx_t           Ts_Ctx[TS_INSTANCES_NBR] = {0};
/**
  * @}
  */

/** @addtogroup STM32H747I_EVAL_TS_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes and configures the touch screen functionalities and
  *         configures all necessary hardware resources (GPIOs, I2C, clocks..).
  * @param  Instance TS instance. Could be only 0.
  * @param  TS_Init  TS Init structure
  * @retval BSP status
  */
int32_t BSP_TS_Init(uint32_t Instance, TS_Init_t *TS_Init)
{
  int32_t ret = BSP_ERROR_NONE;

  if((Instance >=TS_INSTANCES_NBR) || (TS_Init->Width == 0U) ||( TS_Init->Width > TS_MAX_WIDTH) ||\
                         (TS_Init->Height == 0U) ||( TS_Init->Height > TS_MAX_HEIGHT) ||\
                         (TS_Init->Accuracy > TS_MIN((TS_Init->Width), (TS_Init->Height))))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(FT5426_Probe(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else
    {
      TS_Capabilities_t Capabilities;
      Ts_Ctx[Instance].Width             = TS_Init->Width;
      Ts_Ctx[Instance].Height            = TS_Init->Height;
      Ts_Ctx[Instance].Orientation       = TS_Init->Orientation;
      Ts_Ctx[Instance].Accuracy          = TS_Init->Accuracy;

      if (Ts_Drv->GetCapabilities(Ts_CompObj[Instance], &Capabilities) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        /* Store maximum X and Y on context */
        Ts_Ctx[Instance].MaxX = Capabilities.MaxXl;
        Ts_Ctx[Instance].MaxY = Capabilities.MaxYl;
        /* Initialize previous position in order to always detect first touch */
        Ts_Ctx[Instance].PreviousX[0] = TS_Init->Width + TS_Init->Accuracy + 1U;
        Ts_Ctx[Instance].PreviousY[0] = TS_Init->Height + TS_Init->Accuracy + 1U;
      }
    }
  }

  return ret;
}

/**
  * @brief  De-Initializes the touch screen functionalities
  * @param  Instance TS instance. Could be only 0.
  * @retval BSP status
  */
int32_t BSP_TS_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >=TS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Ts_Drv->DeInit(Ts_CompObj[Instance]) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  return ret;
}

/**
  * @brief  Get Touch Screen instance capabilities
  * @param  Instance Touch Screen instance
  * @param  Capabilities pointer to Touch Screen capabilities
  * @retval BSP status
  */
int32_t BSP_TS_GetCapabilities(uint32_t Instance, TS_Capabilities_t *Capabilities)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >=TS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    (void)Ts_Drv->GetCapabilities(Ts_CompObj[Instance], Capabilities);
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Configures and enables the touch screen interrupts.
  * @param  Instance TS instance. Could be only 0.
  * @retval BSP status
  */
int32_t BSP_TS_EnableIT(uint32_t Instance)
{
  int32_t ret  = BSP_ERROR_NONE;

  return ret;
}

/**
  * @brief  Disables the touch screen interrupts.
  * @param  Instance TS instance. Could be only 0.
  * @retval BSP status
  */
int32_t BSP_TS_DisableIT(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  return ret;
}

/**
  * @brief  BSP TS Callback.
  * @retval None.
  */
__weak void BSP_TS_Callback(uint32_t Instance)
{

  /* This function should be implemented by the user application.
     It is called into this driver when an event on TS touch detection */
}

/**
  * @brief  Returns positions of a single touch screen.
  * @param  Instance  TS instance. Could be only 0.
  * @param  TS_State  Pointer to touch screen current state structure
  * @retval BSP status
  */
int32_t BSP_TS_GetState(uint32_t Instance, TS_State_t *TS_State)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t x_oriented, y_oriented;
  uint32_t x_diff, y_diff;

  if(Instance >= TS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    FT5426_State_t state;

    /* Get each touch coordinates */
    if(Ts_Drv->GetState(Ts_CompObj[Instance], &state) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check and update the number of touches active detected */
    else if(state.TouchDetected != 0U)
    {
      x_oriented = state.TouchX;
      y_oriented = state.TouchY;

      if((Ts_Ctx[Instance].Orientation & TS_SWAP_XY) == TS_SWAP_XY)
      {
        x_oriented = state.TouchY;
        y_oriented = state.TouchX;
      }

      if((Ts_Ctx[Instance].Orientation & TS_SWAP_X) == TS_SWAP_X)
      {
        x_oriented = Ts_Ctx[Instance].MaxX - x_oriented - 1UL;
      }

      if((Ts_Ctx[Instance].Orientation & TS_SWAP_Y) == TS_SWAP_Y)
      {
        y_oriented = Ts_Ctx[Instance].MaxY - y_oriented - 1UL;
      }

      /* Apply boundary */
      TS_State->TouchX = (x_oriented * Ts_Ctx[Instance].Width) / Ts_Ctx[Instance].MaxX;
      TS_State->TouchY = (y_oriented * Ts_Ctx[Instance].Height) / Ts_Ctx[Instance].MaxY;
      /* Store Current TS state */
      TS_State->TouchDetected = state.TouchDetected;

      /* Check accuracy */
      x_diff = (TS_State->TouchX > Ts_Ctx[Instance].PreviousX[0])?
               (TS_State->TouchX - Ts_Ctx[Instance].PreviousX[0]):
               (Ts_Ctx[Instance].PreviousX[0] - TS_State->TouchX);

      y_diff = (TS_State->TouchY > Ts_Ctx[Instance].PreviousY[0])?
               (TS_State->TouchY - Ts_Ctx[Instance].PreviousY[0]):
               (Ts_Ctx[Instance].PreviousY[0] - TS_State->TouchY);


      if ((x_diff > Ts_Ctx[Instance].Accuracy) || (y_diff > Ts_Ctx[Instance].Accuracy))
      {
        /* New touch detected */
        Ts_Ctx[Instance].PreviousX[0] = TS_State->TouchX;
        Ts_Ctx[Instance].PreviousY[0] = TS_State->TouchY;
      }
      else
      {
        TS_State->TouchX = Ts_Ctx[Instance].PreviousX[0];
        TS_State->TouchY = Ts_Ctx[Instance].PreviousY[0];
      }
    }
    else
    {
      TS_State->TouchDetected = 0U;
      TS_State->TouchX = Ts_Ctx[Instance].PreviousX[0];
      TS_State->TouchY = Ts_Ctx[Instance].PreviousY[0];
    }
  }

  return ret;
}

#if (USE_TS_MULTI_TOUCH > 0)
/**
  * @brief  Returns positions of multi touch screen.
  * @param  Instance  TS instance. Could be only 0.
  * @param  TS_State  Pointer to touch screen current state structure
  * @retval BSP status
  */
int32_t BSP_TS_Get_MultiTouchState(uint32_t Instance, TS_MultiTouch_State_t *TS_State)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t x_oriented[2], y_oriented[2];
  uint32_t x_diff, y_diff;
  uint32_t index;
  if(Instance >= TS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    TS_MultiTouch_State_t state;
    /* Get each touch coordinates */
    if(Ts_Drv->GetMultiTouchState(Ts_CompObj[Instance], &state) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check and update the number of touches active detected */
    else if(state.TouchDetected != 0U)
    {
      for(index = 0; index < state.TouchDetected; index++)
      {
        x_oriented[index] = state.TouchX[index];
        y_oriented[index] = state.TouchY[index];

        if((Ts_Ctx[Instance].Orientation & TS_SWAP_XY) == TS_SWAP_XY)
        {
          x_oriented[index] = state.TouchY[index];
          y_oriented[index] = state.TouchX[index];
        }

        if((Ts_Ctx[Instance].Orientation & TS_SWAP_X) == TS_SWAP_X)
        {
          x_oriented[index] = Ts_Ctx[Instance].MaxX - x_oriented[index] - 1UL;
        }

        if((Ts_Ctx[Instance].Orientation & TS_SWAP_Y) == TS_SWAP_Y)
        {
          y_oriented[index] = Ts_Ctx[Instance].MaxY - y_oriented[index] - 1UL;
        }

       /* Apply boundary */
       TS_State->TouchX[index] = (x_oriented[index] * Ts_Ctx[Instance].Width) / Ts_Ctx[Instance].MaxX;
       TS_State->TouchY[index] = (y_oriented[index] * Ts_Ctx[Instance].Height) / Ts_Ctx[Instance].MaxY;
       /* Store Current TS state */
       TS_State->TouchDetected = state.TouchDetected;

        /* Check accuracy */
        x_diff = (TS_State->TouchX[index] > Ts_Ctx[Instance].PreviousX[0])?
          (TS_State->TouchX[index] - Ts_Ctx[Instance].PreviousX[0]):
          (Ts_Ctx[Instance].PreviousX[0] - TS_State->TouchX[index]);

        y_diff = (TS_State->TouchY[index] > Ts_Ctx[Instance].PreviousY[0])?
          (TS_State->TouchY[index] - Ts_Ctx[Instance].PreviousY[0]):
          (Ts_Ctx[Instance].PreviousY[0] - TS_State->TouchY[index]);

        if ((x_diff > Ts_Ctx[Instance].Accuracy) || (y_diff > Ts_Ctx[Instance].Accuracy))
        {
          /* New touch detected */
          Ts_Ctx[Instance].PreviousX[index] = TS_State->TouchX[index];
          Ts_Ctx[Instance].PreviousY[index] = TS_State->TouchY[index];
        }
        else
        {
          TS_State->TouchX[index] = Ts_Ctx[Instance].PreviousX[index];
          TS_State->TouchY[index] = Ts_Ctx[Instance].PreviousY[index];
        }
      }
    }
    else
    {
      TS_State->TouchDetected = 0U;
      for(index = 0; index < TS_TOUCH_NBR; index++)
      {
        TS_State->TouchX[index] = Ts_Ctx[Instance].PreviousX[index];
        TS_State->TouchY[index] = Ts_Ctx[Instance].PreviousY[index];
      }
    }
  }

  return ret;
}
#endif /* USE_TS_MULTI_TOUCH > 0*/

#if (USE_TS_GESTURE > 0)
/**
  * @brief  Update gesture Id following a touch detected.
  * @param  Instance      TS instance. Could be only 0.
  * @param  GestureConfig Pointer to gesture configuration structure
  * @retval BSP status
  */
int32_t BSP_TS_GestureConfig(uint32_t Instance, TS_Gesture_Config_t *GestureConfig)
{
  int32_t ret;

  if(Instance >=TS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(Ts_Drv->GestureConfig(Ts_CompObj[Instance], GestureConfig) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Update gesture Id following a touch detected.
  * @param  Instance   TS instance. Could be only 0.
  * @param  GestureId  Pointer to gesture ID
  * @retval BSP status
  */
int32_t BSP_TS_GetGestureId(uint32_t Instance, uint32_t *GestureId)
{
  uint8_t tmp = 0;
  int32_t ret;

  if(Instance >=TS_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Get gesture Id */
    Ts_Drv->GetGesture(Ts_CompObj[Instance], &tmp);

    /* Remap gesture Id to a TS_Gesture_Id_t value */
    switch(tmp)
    {
    case FT5426_GEST_ID_NO_GESTURE :
      *GestureId = GESTURE_ID_NO_GESTURE;
      break;
    case FT5426_GEST_ID_MOVE_UP :
      *GestureId = GESTURE_ID_MOVE_UP;
      break;
    case FT5426_GEST_ID_MOVE_RIGHT :
      *GestureId = GESTURE_ID_MOVE_RIGHT;
      break;
    case FT5426_GEST_ID_MOVE_DOWN :
      *GestureId = GESTURE_ID_MOVE_DOWN;
      break;
    case FT5426_GEST_ID_MOVE_LEFT :
      *GestureId = GESTURE_ID_MOVE_LEFT;
      break;
    case FT5426_GEST_ID_ZOOM_IN :
      *GestureId = GESTURE_ID_ZOOM_IN;
      break;
    case FT5426_GEST_ID_ZOOM_OUT :
      *GestureId = GESTURE_ID_ZOOM_OUT;
      break;
    default :
      *GestureId = GESTURE_ID_NO_GESTURE;
      break;
    }

    ret = BSP_ERROR_NONE;
  }

  return ret;
}
#endif /* USE_TS_GESTURE > 0 */

/**
  * @brief  Set TS orientation
  * @param  Instance TS instance. Could be only 0.
  * @param  Orientation Orientation to be set
  * @retval BSP status
  */
int32_t BSP_TS_Set_Orientation(uint32_t Instance, uint32_t Orientation)
{
  Ts_Ctx[Instance].Orientation = Orientation;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get TS orientation
  * @param  Instance TS instance. Could be only 0.
  * @param  Orientation Current Orientation to be returned
  * @retval BSP status
  */
int32_t BSP_TS_Get_Orientation(uint32_t Instance, uint32_t *Orientation)
{
  *Orientation = Ts_Ctx[Instance].Orientation;
  return BSP_ERROR_NONE;
}

/**
  * @brief  This function handles EXTI_LINE_8 interrupt request.
  * @retval None
  */
//void BSP_TS_IRQHandler(uint32_t Instance)
//{
//  HAL_EXTI_IRQHandler(&hts_exti[Instance]);
//}

/**
  * @}
  */
/** @defgroup STM32H747I_EVAL_TS_Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval BSP status
  */
static int32_t FT5426_Probe(uint32_t Instance)
{
  int32_t ret ;
  FT5426_IO_t              IOCtx;
  static FT5426_Object_t   FT5426Obj;
  FT5426_Capabilities_t    Cap;
  uint32_t id, i;
  uint32_t const i2c_address[] = {DEV_I2C_ADDRESS, DEV_I2C_ADDRESS_A02};

  /* Configure the touch screen driver */
  IOCtx.Init        = BSP_I2C1_Init;
  IOCtx.DeInit      = BSP_I2C1_DeInit;
  IOCtx.ReadReg     = BSP_I2C1_ReadReg;
  IOCtx.WriteReg    = BSP_I2C1_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  for(i = 0; i < 2UL; i++)
  {
    IOCtx.Address     = (uint16_t)i2c_address[i];

    if(FT5426_RegisterBusIO (&FT5426Obj, &IOCtx) != FT5426_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else if(FT5426_ReadID(&FT5426Obj, &id) != FT5426_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if(id != FT5426_ID)
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    else
    {

      (void)FT5426_GetCapabilities(&FT5426Obj, &Cap);
      Ts_CompObj[Instance] = &FT5426Obj;
      Ts_Drv = (TS_Drv_t *) &FT5426_TS_Driver;

      if(Ts_Drv->Init(Ts_CompObj[Instance]) != FT5426_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
        break;
      }
    }
  }

  return ret;
}


/**
  * @brief  TS EXTI touch detection callbacks.
  * @retval None
  */
//static void TS_EXTI_Callback(void)
//{
//  BSP_TS_Callback(0);
//}
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