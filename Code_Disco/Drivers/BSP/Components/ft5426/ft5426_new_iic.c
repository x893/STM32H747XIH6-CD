/**
  ******************************************************************************
  * @file    ft5426.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the FT5426
  *          IO Expander devices.
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
#include "ft5426_new_iic.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup FT5426 FT5426
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/** @defgroup FT5426_Exported_Variables FT5426 Exported Variables
  * @{
  */

/* Touch screen driver structure initialization */
FT5426_TS_Drv_t FT5426_TS_Driver =
{
  FT5426_Init,
  FT5426_DeInit,
  FT5426_GestureConfig,
  FT5426_ReadID, 
  FT5426_GetState,
  FT5426_GetMultiTouchState,
  FT5426_GetGesture,
  FT5426_GetCapabilities,
  FT5426_EnableIT,
  FT5426_DisableIT,  
  FT5426_ClearIT,
  FT5426_ITStatus
};
/**
  * @}
  */

/** @defgroup FT5426_Private_Function_Prototypes FT5426 Private Function Prototypes
  * @{
  */
#if (FT5426_AUTO_CALIBRATION_ENABLED == 1)
static int32_t FT5426_TS_Calibration(FT5426_Object_t *pObj);
static int32_t FT5426_Delay(FT5426_Object_t *pObj, uint32_t Delay);
#endif /* FT5426_AUTO_CALIBRATION_ENABLED == 1 */
static int32_t FT5426_DetectTouch(FT5426_Object_t *pObj);
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t* Data, uint16_t Length);
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t* Data, uint16_t Length);

/**
  * @}
  */

/** @defgroup FT5426_Exported_Functions FT5426 Exported Functions
  * @{
  */

/**
  * @brief  Register IO bus to component object
  * @param  Component object pointer
  * @retval error status
  */
int32_t FT5426_RegisterBusIO (FT5426_Object_t *pObj, FT5426_IO_t *pIO)
{
  int32_t ret;
  
  if (pObj == NULL)
  {
    ret = FT5426_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;
    
    pObj->Ctx.ReadReg  = ReadRegWrap;
    pObj->Ctx.WriteReg = WriteRegWrap;
    pObj->Ctx.handle   = pObj;
    
    if(pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = FT5426_ERROR;
    }
  }    
  
  return ret;
}

/**
  * @brief  Get FT5426 sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to FT5426 sensor capabilities
  * @retval Component status
  */
int32_t FT5426_GetCapabilities(FT5426_Object_t *pObj, FT5426_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  /* Store component's capabilities */
  Capabilities->MultiTouch = 1;
  Capabilities->Gesture    = 0;  /* Gesture feature is currently not activated on FW chipset */
  Capabilities->MaxTouch   = FT5426_MAX_NB_TOUCH;
  Capabilities->MaxXl      = FT5426_MAX_X_LENGTH;
  Capabilities->MaxYl      = FT5426_MAX_Y_LENGTH;
  
  return FT5426_OK;
}

/**
  * @brief  Initialize the FT5426 communication bus
  *         from MCU to FT5426 : ie I2C channel initialization (if required).
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t FT5426_Init(FT5426_Object_t *pObj)
{
  int32_t ret = FT5426_OK;
  
  if(pObj->IsInitialized == 0U)
  {    
    /* Initialize IO BUS layer */
    pObj->IO.Init();
    
#if (FT5426_AUTO_CALIBRATION_ENABLED == 1)
    /* Hw Calibration sequence start : should be done once after each power up */
    /* This is called internal calibration of the touch screen                 */
    ret += FT5426_TS_Calibration(pObj);
#endif /* (FT5426_AUTO_CALIBRATION_ENABLED == 1) */    
    /* By default set FT5426 IC in Polling mode : no INT generation on FT5426 for new touch available */
    /* Note TS_INT is active low                                                                      */
    ret += FT5426_DisableIT(pObj);
    
    pObj->IsInitialized = 1;
  }
  
  if(ret != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }
  
  return ret;
}

/**
  * @brief  De-Initialize the FT5426 communication bus
  *         from MCU to FT5426 : ie I2C channel initialization (if required).
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t FT5426_DeInit(FT5426_Object_t *pObj)
{
  if(pObj->IsInitialized == 1U)
  {
    pObj->IsInitialized = 0;
  }
  
  return FT5426_OK;
}

/**
  * @brief  Configure the FT5426 gesture
  *         from MCU to FT5426 : ie I2C channel initialization (if required).
  * @param  pObj  Component object pointer
  * @param  GestureInit Gesture init structure
  * @retval Component status
  */
int32_t FT5426_GestureConfig(FT5426_Object_t *pObj, FT5426_Gesture_Init_t *GestureInit)
{
  int32_t ret;
  
  ret = ft5426_radian_value(&pObj->Ctx, (uint8_t)GestureInit->Radian);
  ret += ft5426_offset_left_right(&pObj->Ctx, (uint8_t)GestureInit->OffsetLeftRight);
  ret += ft5426_offset_up_down(&pObj->Ctx, (uint8_t)GestureInit->OffsetUpDown);
  ret += ft5426_disatnce_left_right(&pObj->Ctx, (uint8_t)GestureInit->DistanceLeftRight);
  ret += ft5426_distance_up_down(&pObj->Ctx, (uint8_t)GestureInit->DistanceUpDown);
  ret += ft5426_distance_zoom(&pObj->Ctx, (uint8_t)GestureInit->DistanceZoom);
  
  if(ret != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }
  
  return ret;   
}

/**
  * @brief  Read the FT5426 device ID, pre initialize I2C in case of need to be
  *         able to read the FT5426 device ID, and verify this is a FT5426.
  * @param  pObj Component object pointer
  * @param  Id Pointer to component's ID
  * @retval Component status
  */
int32_t FT5426_ReadID(FT5426_Object_t *pObj, uint32_t *Id)
{
  int32_t ret;
  uint8_t ft5426_id;

  ret = ft5426_chip_id(&pObj->Ctx, &ft5426_id);
  *Id = (uint32_t) ft5426_id;

  return ret;
}

/**
  * @brief  Get the touch screen X and Y positions values
  * @param  pObj Component object pointer
  * @param  State Single Touch structure pointer
  * @retval Component status.
  */
int32_t FT5426_GetState(FT5426_Object_t *pObj, FT5426_State_t *State)
{
  int32_t ret = FT5426_OK;
  uint8_t  data[4];
  
  State->TouchDetected = (uint32_t)FT5426_DetectTouch(pObj);
  if(ft5426_read_reg(&pObj->Ctx, FT5426_P1_XH_REG, data, (uint16_t)sizeof(data)) != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }
  else
  {
    /* Send back first ready X position to caller */
    State->TouchX = (((uint32_t)data[0] & FT5426_P1_XH_TP_BIT_MASK) << 8) | ((uint32_t)data[1] & FT5426_P1_XL_TP_BIT_MASK);
    /* Send back first ready Y position to caller */
    State->TouchY = (((uint32_t)data[2] & FT5426_P1_YH_TP_BIT_MASK) << 8) | ((uint32_t)data[3] & FT5426_P1_YL_TP_BIT_MASK);
  }
  
  return ret;
}

/**
  * @brief  Get the touch screen Xn and Yn positions values in multi-touch mode
  * @param  pObj Component object pointer
  * @param  State Multi Touch structure pointer
  * @retval Component status.
  */
int32_t FT5426_GetMultiTouchState(FT5426_Object_t *pObj, FT5426_MultiTouch_State_t *State)
{
  int32_t ret = FT5426_OK;  
  uint8_t  data[12];
  
  State->TouchDetected = (uint32_t)FT5426_DetectTouch(pObj);
  
  if(ft5426_read_reg(&pObj->Ctx, FT5426_P1_XH_REG, data, (uint16_t)sizeof(data)) != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }
  else
  {  
    /* Send back first ready X position to caller */
    State->TouchX[0] = (((uint32_t)data[0] & FT5426_P1_XH_TP_BIT_MASK) << 8) | ((uint32_t)data[1] & FT5426_P1_XL_TP_BIT_MASK);
    /* Send back first ready Y position to caller */
    State->TouchY[0] = (((uint32_t)data[2] & FT5426_P1_YH_TP_BIT_MASK) << 8) | ((uint32_t)data[3] & FT5426_P1_YL_TP_BIT_MASK);
    /* Send back first ready Event to caller */  
    State->TouchEvent[0] = (((uint32_t)data[0] & FT5426_P1_XH_EF_BIT_MASK) >> FT5426_P1_XH_EF_BIT_POSITION);
    /* Send back first ready Weight to caller */  
    State->TouchWeight[0] = ((uint32_t)data[4] & FT5426_P1_WEIGHT_BIT_MASK);
    /* Send back first ready Area to caller */  
    State->TouchArea[0] = ((uint32_t)data[5] & FT5426_P1_MISC_BIT_MASK) >> FT5426_P1_MISC_BIT_POSITION;
    
    /* Send back first ready X position to caller */
    State->TouchX[1] = (((uint32_t)data[6] & FT5426_P2_XH_TP_BIT_MASK) << 8) | ((uint32_t)data[7] & FT5426_P2_XL_TP_BIT_MASK);
    /* Send back first ready Y position to caller */
    State->TouchY[1] = (((uint32_t)data[8] & FT5426_P2_YH_TP_BIT_MASK) << 8) | ((uint32_t)data[9] & FT5426_P2_YL_TP_BIT_MASK);
    /* Send back first ready Event to caller */  
    State->TouchEvent[1] = (((uint32_t)data[6] & FT5426_P2_XH_EF_BIT_MASK) >> FT5426_P2_XH_EF_BIT_POSITION);
    /* Send back first ready Weight to caller */  
    State->TouchWeight[1] = ((uint32_t)data[10] & FT5426_P2_WEIGHT_BIT_MASK);
    /* Send back first ready Area to caller */  
    State->TouchArea[1] = ((uint32_t)data[11] & FT5426_P2_MISC_BIT_MASK) >> FT5426_P2_MISC_BIT_POSITION;
  }
  
  return ret;  
}

/**
  * @brief  Get Gesture ID
  * @param  pObj Component object pointer
  * @param  GestureId gesture ID
  * @retval Component status
  */
int32_t FT5426_GetGesture(FT5426_Object_t *pObj, uint8_t *GestureId)
{  
  return ft5426_gest_id(&pObj->Ctx, GestureId);
}

/**
  * @brief  Configure the FT5426 device to generate IT on given INT pin
  *         connected to MCU as EXTI.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t FT5426_EnableIT(FT5426_Object_t *pObj)
{
  return ft5426_g_mode(&pObj->Ctx, FT5426_G_MODE_INTERRUPT_TRIGGER);
}

/**
  * @brief  Configure the FT5426 device to stop generating IT on the given INT pin
  *         connected to MCU as EXTI.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t FT5426_DisableIT(FT5426_Object_t *pObj)
{
  return ft5426_g_mode(&pObj->Ctx, FT5426_G_MODE_INTERRUPT_POLLING);
}

/**
  * @brief  Get IT status from FT5426 interrupt status registers
  *         Should be called Following an EXTI coming to the MCU to know the detailed
  *         reason of the interrupt.
  *         @note : This feature is not supported by FT5426.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t FT5426_ITStatus(FT5426_Object_t *pObj)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  /* Always return FT5426_OK as feature not supported by FT5426 */
  return FT5426_OK;
}

/**
  * @brief  Clear IT status in FT5426 interrupt status clear registers
  *         Should be called Following an EXTI coming to the MCU.
  *         @note : This feature is not supported by FT5426.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t FT5426_ClearIT(FT5426_Object_t *pObj)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  /* Always return FT5426_OK as feature not supported by FT5426 */
  return FT5426_OK;
}

/**
  * @}
  */

/** @defgroup FT5426_Private_Functions FT5426 Private Functions
  * @{
  */
#if (FT5426_AUTO_CALIBRATION_ENABLED == 1)
/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param pObj pointer to component object
  * @param Delay specifies the delay time length, in milliseconds
  * @retval Component status
  */
static int32_t FT5426_Delay(FT5426_Object_t *pObj, uint32_t Delay)
{  
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
  return FT5426_OK;
}

/**
  * @brief  Start TouchScreen calibration phase
  * @param pObj pointer to component object
  * @retval Component status
  */
static int32_t FT5426_TS_Calibration(FT5426_Object_t *pObj)
{
  int32_t ret = FT5426_OK;
  uint32_t nbr_attempt;
  uint8_t read_data;
  uint8_t end_calibration = 0;
  
  /* Switch FT5426 back to factory mode to calibrate */
  if(ft5426_dev_mode_w(&pObj->Ctx, FT5426_DEV_MODE_FACTORY) != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }/* Read back the same register FT5426_DEV_MODE_REG */
  else if(ft5426_dev_mode_r(&pObj->Ctx, &read_data) != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }
  else
  {
    (void)FT5426_Delay(pObj, 300); /* Wait 300 ms */
    
    if(read_data != FT5426_DEV_MODE_FACTORY )
    {
      /* Return error to caller */
      ret = FT5426_ERROR;
    }
    else
    {
      /* Start calibration command */
      read_data= 0x04;
      if(ft5426_write_reg(&pObj->Ctx, FT5426_TD_STAT_REG, &read_data, 1) != FT5426_OK)
      {
        ret = FT5426_ERROR;
      }
      else
      {
        (void)FT5426_Delay(pObj, 300); /* Wait 300 ms */
        
        /* 100 attempts to wait switch from factory mode (calibration) to working mode */
        for (nbr_attempt=0; ((nbr_attempt < 100U) && (end_calibration == 0U)) ; nbr_attempt++)
        {
          if(ft5426_dev_mode_r(&pObj->Ctx, &read_data) != FT5426_OK)
          {
            ret = FT5426_ERROR;
            break;
          }
          if(read_data == FT5426_DEV_MODE_WORKING)
          {
            /* Auto Switch to FT5426_DEV_MODE_WORKING : means calibration have ended */
            end_calibration = 1; /* exit for loop */
          }
          
          (void)FT5426_Delay(pObj, 200); /* Wait 200 ms */
        } 
      }
    }
  }
  
  return ret;
}
#endif /* FT5426_AUTO_CALIBRATION_ENABLED == 1 */

/**
  * @brief  Return if there is touches detected or not.
  *         Try to detect new touches and forget the old ones (reset internal global
  *         variables).
  * @param  pObj Component object pointer
  * @retval Number of active touches detected (can be 0, 1 or 2) or FT5426_ERROR
  *         in case of error
  */
static int32_t FT5426_DetectTouch(FT5426_Object_t *pObj)
{
  int32_t ret;
  uint8_t nb_touch;
  
  /* Read register FT5426_TD_STAT_REG to check number of touches detection */
  if(ft5426_td_status(&pObj->Ctx, &nb_touch) != FT5426_OK)
  {
    ret = FT5426_ERROR;
  }
  else
  {
    if(nb_touch > FT5426_MAX_NB_TOUCH)
    {
      /* If invalid number of touch detected, set it to zero */
      ret = 0;
    }
    else
    {
      ret = (int32_t)nb_touch;
    }
  }
  return ret;
}

/**
  * @brief  Wrap IO bus read function to component register red function
  * @param  handle Component object handle
  * @param  Reg The target register address to read
  * @param  pData The target register value to be read
  * @param  Length buffer size to be read
  * @retval Component status.
  */
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t* pData, uint16_t Length)
{
  FT5426_Object_t *pObj = (FT5426_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap IO bus write function to component register write function
  * @param  handle Component object handle
  * @param  Reg The target register address to write
  * @param  pData The target register value to be written
  * @param  Length buffer size to be written
  * @retval Component status.
  */
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t* pData, uint16_t Length)
{
  FT5426_Object_t *pObj = (FT5426_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
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
