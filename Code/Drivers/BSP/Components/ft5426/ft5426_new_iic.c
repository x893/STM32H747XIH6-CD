/**
  ******************************************************************************
  * @file    ft5426.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-June-2015
  * @brief   This file provides a set of functions needed to manage the FT5426
  *          touch screen devices.
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

/* Includes ------------------------------------------------------------------*/
#include "ft5426_new_iic.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup FT5426
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/** @defgroup FT5426_Private_Types_Definitions
  * @{
  */

/* Private define ------------------------------------------------------------*/

/** @defgroup FT5426_Private_Defines
  * @{
  */
#define FT5426_MAX_INSTANCE  2
/* Private macro -------------------------------------------------------------*/

/** @defgroup FT5426_Private_Macros
  * @{
  */

/* Private variables ---------------------------------------------------------*/

/** @defgroup FT5426_Private_Variables
  * @{
  */

/* Touch screen driver structure initialization */
TS_DrvTypeDef ft5426_ts_drv =
{
  ft5426_Init,
  ft5426_ReadID,
  ft5426_Reset,

  ft5426_TS_Start,
  ft5426_TS_DetectTouch,
  ft5426_TS_GetXY,

  ft5426_TS_EnableIT,
  ft5426_TS_ClearIT,
  ft5426_TS_ITStatus,
  ft5426_TS_DisableIT

};

/* ft6x06 instances by address */
uint8_t ft5426[FT5426_MAX_INSTANCE] = {0};

/* Global ft5426 handle */
static ft5426_handle_TypeDef ft5426_handle = { FT5426_I2C_NOT_INITIALIZED, 0, 0};

/**
  * @}
  */

/** @defgroup ft5426_Private_Function_Prototypes
  * @{
  */

/* Private functions prototypes-----------------------------------------------*/

/**
  * @brief  Return the status of I2C was initialized or not.
  * @param  None.
  * @retval : I2C initialization status.
  */
static uint8_t ft5426_Get_I2C_InitializedStatus(void);

/**
  * @brief  I2C initialize if needed.
  * @param  None.
  * @retval : None.
  */
static void ft5426_I2C_InitializeIfRequired(void);

/**
  * @brief  Basic static configuration of TouchScreen
  * @param  DeviceAddr: FT5426 Device address for communication on I2C Bus.
  * @retval Status FT5426_STATUS_OK or FT5426_STATUS_NOT_OK.
  */
static uint32_t ft5426_TS_Configure(uint16_t DeviceAddr);

//static uint8_t ft5426_GetInstance(uint16_t DeviceAddr);

/** @defgroup ft5426_Private_Functions
  * @{
  */

/** @defgroup ft5426_Public_Function_Body
  * @{
  */

/* Public functions bodies-----------------------------------------------*/


/**
  * @brief  Initialize the ft5426 communication bus
  *         from MCU to FT5426 : ie I2C channel initialization (if required).
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5426).
  * @retval None
  */
//void ft5426_Init(uint16_t DeviceAddr)
//{  
//  uint8_t instance;
//  uint8_t empty;
//	
//	TS_IO_Delay(200);
//  
//  /* Check if device instance already exists */
//  instance = ft5426_GetInstance(DeviceAddr);
//  
//  /* To prevent double initialization */
//  if(instance == 0xFF)
//  {
//    /* Look for empty instance */
//    empty = ft5426_GetInstance(0);
//    
//    if(empty < FT5426_MAX_INSTANCE)
//    {
//      /* Register the current device instance */
//      ft5426[empty] = DeviceAddr;
//      
//      /* Initialize IO BUS layer */
//      TS_IO_Init(); 
//    }
//  }  
//}
void ft5426_Init(uint16_t DeviceAddr)
{
  /* Wait at least 200ms after power up before accessing registers
   * Trsi timing (Time of starting to report point after resetting) from FT5426GQQ datasheet */
  HAL_Delay(200);

  /* Initialize I2C link if needed */
  ft5426_I2C_InitializeIfRequired();
}

/**
  * @brief  Software Reset the ft5426.
  *         @note : Not applicable to FT5426.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5426).
  * @retval None
  */
void ft5426_Reset(uint16_t DeviceAddr)
{
  /* Do nothing */
  /* No software reset sequence available in FT5426 IC */
}

/**
  * @brief  Read the ft5426 device ID, pre initialize I2C in case of need to be
  *         able to read the FT5426 device ID, and verify this is a FT5426.
  * @param  DeviceAddr: I2C FT5426 Slave address.
  * @retval The Device ID (two bytes).
  */
uint16_t ft5426_ReadID(uint16_t DeviceAddr)
{
  volatile uint8_t ucReadId = 0;
	
	/* Initialize I2C link if needed */
//  TS_IO_Init();
	ft5426_I2C_InitializeIfRequired();
	
	//The code change from FT5336,FT5426 do not have ID
	ucReadId = 0x51;
  /* Return the device ID value */
  return (ucReadId);
}

/**
  * @brief  Configures the touch Screen IC device to start detecting touches
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address).
  * @retval None.
  */
void ft5426_TS_Start(uint16_t DeviceAddr)
{
  /* Minimum static configuration of FT5426 */
  FT5426_ASSERT(ft5426_TS_Configure(DeviceAddr));

  /* By default set FT5426 IC in Polling mode : no INT generation on FT5426 for new touch available */
  /* Note TS_INT is active low                                                                      */
  ft5426_TS_DisableIT(DeviceAddr);
}

/**
  * @brief  Return if there is touches detected or not.
  *         Try to detect new touches and forget the old ones (reset internal global
  *         variables).
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval : Number of active touches detected (can be 0, 1 or 2).
  */
uint8_t ft5426_TS_DetectTouch(uint16_t DeviceAddr)
{
  uint8_t nbTouch = 0;

  /* Read register FT5426_TD_STAT_REG to check number of touches detection */
//  nbTouch = TS_IO_Read(DeviceAddr, FT5426_TD_STAT_REG);
	BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, FT5426_TD_STAT_REG, &nbTouch, 1);
  nbTouch &= FT5426_TD_STAT_MASK;

  if(nbTouch > FT5426_MAX_DETECTABLE_TOUCH)
  {
    /* If invalid number of touch detected, set it to zero */
    nbTouch = 0;
  }

  /* Update ft5426 driver internal global : current number of active touches */
  ft5426_handle.currActiveTouchNb = nbTouch;

  /* Reset current active touch index on which to work on */
  ft5426_handle.currActiveTouchIdx = 0;

  return(nbTouch);
}

/**
  * @brief  Get the touch screen X and Y positions values
  *         Manage multi touch thanks to touch Index global
  *         variable 'ft5426_handle.currActiveTouchIdx'.
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  X: Pointer to X position value
  * @param  Y: Pointer to Y position value
  * @retval None.
  */
void ft5426_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y)
{
  uint8_t ucReadData = 0;
  static uint16_t coord;
  uint8_t regAddressXLow = 0;
  uint8_t regAddressXHigh = 0;
  uint8_t regAddressYLow = 0;
  uint8_t regAddressYHigh = 0;

  if(ft5426_handle.currActiveTouchIdx < ft5426_handle.currActiveTouchNb)
  {
    switch(ft5426_handle.currActiveTouchIdx)
    {
    case 0 :
      regAddressXLow  = FT5426_P1_XL_REG;
      regAddressXHigh = FT5426_P1_XH_REG;
      regAddressYLow  = FT5426_P1_YL_REG;
      regAddressYHigh = FT5426_P1_YH_REG;
      break;

    case 1 :
      regAddressXLow  = FT5426_P2_XL_REG;
      regAddressXHigh = FT5426_P2_XH_REG;
      regAddressYLow  = FT5426_P2_YL_REG;
      regAddressYHigh = FT5426_P2_YH_REG;
      break;

    case 2 :
      regAddressXLow  = FT5426_P3_XL_REG;
      regAddressXHigh = FT5426_P3_XH_REG;
      regAddressYLow  = FT5426_P3_YL_REG;
      regAddressYHigh = FT5426_P3_YH_REG;
      break;

    case 3 :
      regAddressXLow  = FT5426_P4_XL_REG;
      regAddressXHigh = FT5426_P4_XH_REG;
      regAddressYLow  = FT5426_P4_YL_REG;
      regAddressYHigh = FT5426_P4_YH_REG;
      break;

    case 4 :
      regAddressXLow  = FT5426_P5_XL_REG;
      regAddressXHigh = FT5426_P5_XH_REG;
      regAddressYLow  = FT5426_P5_YL_REG;
      regAddressYHigh = FT5426_P5_YH_REG;
      break;

    case 5 :
      regAddressXLow  = FT5426_P6_XL_REG;
      regAddressXHigh = FT5426_P6_XH_REG;
      regAddressYLow  = FT5426_P6_YL_REG;
      regAddressYHigh = FT5426_P6_YH_REG;
      break;

    case 6 :
      regAddressXLow  = FT5426_P7_XL_REG;
      regAddressXHigh = FT5426_P7_XH_REG;
      regAddressYLow  = FT5426_P7_YL_REG;
      regAddressYHigh = FT5426_P7_YH_REG;
      break;

    case 7 :
      regAddressXLow  = FT5426_P8_XL_REG;
      regAddressXHigh = FT5426_P8_XH_REG;
      regAddressYLow  = FT5426_P8_YL_REG;
      regAddressYHigh = FT5426_P8_YH_REG;
      break;

    case 8 :
      regAddressXLow  = FT5426_P9_XL_REG;
      regAddressXHigh = FT5426_P9_XH_REG;
      regAddressYLow  = FT5426_P9_YL_REG;
      regAddressYHigh = FT5426_P9_YH_REG;
      break;

    case 9 :
      regAddressXLow  = FT5426_P10_XL_REG;
      regAddressXHigh = FT5426_P10_XH_REG;
      regAddressYLow  = FT5426_P10_YL_REG;
      regAddressYHigh = FT5426_P10_YH_REG;
      break;

    default :
      break;

    } /* end switch(ft5426_handle.currActiveTouchIdx) */

    /* Read low part of X position */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressXLow);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressXLow, &ucReadData, 1);
    coord = (ucReadData & FT5426_TOUCH_POS_LSB_MASK) >> FT5426_TOUCH_POS_LSB_SHIFT;

    /* Read high part of X position */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressXHigh);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressXHigh, &ucReadData, 1);
    coord |= ((ucReadData & FT5426_TOUCH_POS_MSB_MASK) >> FT5426_TOUCH_POS_MSB_SHIFT) << 8;

    /* Send back ready X position to caller */
    *X = coord;

    /* Read low part of Y position */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressYLow);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressYLow, &ucReadData, 1);
    coord = (ucReadData & FT5426_TOUCH_POS_LSB_MASK) >> FT5426_TOUCH_POS_LSB_SHIFT;

    /* Read high part of Y position */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressYHigh);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressYHigh, &ucReadData, 1);
    coord |= ((ucReadData & FT5426_TOUCH_POS_MSB_MASK) >> FT5426_TOUCH_POS_MSB_SHIFT) << 8;

    /* Send back ready Y position to caller */
    *Y = coord;

    ft5426_handle.currActiveTouchIdx++; /* next call will work on next touch */

  } /* of if(ft5426_handle.currActiveTouchIdx < ft5426_handle.currActiveTouchNb) */
}

/**
  * @brief  Configure the FT5426 device to generate IT on given INT pin
  *         connected to MCU as EXTI.
  * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of FT5426).
  * @retval None
  */
void ft5426_TS_EnableIT(uint16_t DeviceAddr)
{
   uint8_t regValue = 0;
   regValue = (FT5426_G_MODE_INTERRUPT_TRIGGER & (FT5426_G_MODE_INTERRUPT_MASK >> FT5426_G_MODE_INTERRUPT_SHIFT)) << FT5426_G_MODE_INTERRUPT_SHIFT;

   /* Set interrupt trigger mode in FT5426_GMODE_REG */
   TS_IO_Write(DeviceAddr, FT5426_GMODE_REG, regValue);
}

/**
  * @brief  Configure the FT5426 device to stop generating IT on the given INT pin
  *         connected to MCU as EXTI.
  * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of FT5426).
  * @retval None
  */
void ft5426_TS_DisableIT(uint16_t DeviceAddr)
{
  uint8_t regValue = 0;
  regValue = (FT5426_G_MODE_INTERRUPT_POLLING & (FT5426_G_MODE_INTERRUPT_MASK >> FT5426_G_MODE_INTERRUPT_SHIFT)) << FT5426_G_MODE_INTERRUPT_SHIFT;

  /* Set interrupt polling mode in FT5426_GMODE_REG */
  TS_IO_Write(DeviceAddr, FT5426_GMODE_REG, regValue);
}

/**
  * @brief  Get IT status from FT5426 interrupt status registers
  *         Should be called Following an EXTI coming to the MCU to know the detailed
  *         reason of the interrupt.
  *         @note : This feature is not applicable to FT5426.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5426).
  * @retval TS interrupts status : always return 0 here
  */
uint8_t ft5426_TS_ITStatus(uint16_t DeviceAddr)
{
  /* Always return 0 as feature not applicable to FT5426 */
  return 0;
}

/**
  * @brief  Clear IT status in FT5426 interrupt status clear registers
  *         Should be called Following an EXTI coming to the MCU.
  *         @note : This feature is not applicable to FT5426.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5426).
  * @retval None
  */
void ft5426_TS_ClearIT(uint16_t DeviceAddr)
{
  /* Nothing to be done here for FT5426 */
}

/**** NEW FEATURES enabled when Multi-touch support is enabled ****/

#if (TS_MULTI_TOUCH_SUPPORTED == 1)

/**
  * @brief  Get the last touch gesture identification (zoom, move up/down...).
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5426).
  * @param  pGestureId : Pointer to get last touch gesture Identification.
  * @retval None.
  */
void ft5426_TS_GetGestureID(uint16_t DeviceAddr, uint32_t * pGestureId)
{
  uint8_t ucReadData = 0;

//  ucReadData = TS_IO_Read(DeviceAddr, FT5426_GEST_ID_REG);
	BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, FT5426_GEST_ID_REG, &ucReadData, 1);
  * pGestureId = ucReadData;
}

/**
  * @brief  Get the touch detailed informations on touch number 'touchIdx' (0..1)
  *         This touch detailed information contains :
  *         - weight that was applied to this touch
  *         - sub-area of the touch in the touch panel
  *         - event of linked to the touch (press down, lift up, ...)
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5426).
  * @param  touchIdx : Passed index of the touch (0..1) on which we want to get the
  *                    detailed information.
  * @param  pWeight : Pointer to to get the weight information of 'touchIdx'.
  * @param  pArea   : Pointer to to get the sub-area information of 'touchIdx'.
  * @param  pEvent  : Pointer to to get the event information of 'touchIdx'.

  * @retval None.
  */
void ft5426_TS_GetTouchInfo(uint16_t   DeviceAddr,
                            uint32_t   touchIdx,
                            uint32_t * pWeight,
                            uint32_t * pArea,
                            uint32_t * pEvent)
{
  uint8_t ucReadData = 0;
  uint8_t regAddressXHigh = 0;
  uint8_t regAddressPWeight = 0;
  uint8_t regAddressPMisc = 0;

  if(touchIdx < ft5426_handle.currActiveTouchNb)
  {
    switch(touchIdx)
    {
    case 0 :
      regAddressXHigh   = FT5426_P1_XH_REG;
      regAddressPWeight = FT5426_P1_WEIGHT_REG;
      regAddressPMisc   = FT5426_P1_MISC_REG;
      break;

    case 1 :
      regAddressXHigh   = FT5426_P2_XH_REG;
      regAddressPWeight = FT5426_P2_WEIGHT_REG;
      regAddressPMisc   = FT5426_P2_MISC_REG;
      break;

    case 2 :
      regAddressXHigh   = FT5426_P3_XH_REG;
      regAddressPWeight = FT5426_P3_WEIGHT_REG;
      regAddressPMisc   = FT5426_P3_MISC_REG;
      break;

    case 3 :
      regAddressXHigh   = FT5426_P4_XH_REG;
      regAddressPWeight = FT5426_P4_WEIGHT_REG;
      regAddressPMisc   = FT5426_P4_MISC_REG;
      break;

    case 4 :
      regAddressXHigh   = FT5426_P5_XH_REG;
      regAddressPWeight = FT5426_P5_WEIGHT_REG;
      regAddressPMisc   = FT5426_P5_MISC_REG;
      break;

    case 5 :
      regAddressXHigh   = FT5426_P6_XH_REG;
      regAddressPWeight = FT5426_P6_WEIGHT_REG;
      regAddressPMisc   = FT5426_P6_MISC_REG;
      break;

    case 6 :
      regAddressXHigh   = FT5426_P7_XH_REG;
      regAddressPWeight = FT5426_P7_WEIGHT_REG;
      regAddressPMisc   = FT5426_P7_MISC_REG;
      break;

    case 7 :
      regAddressXHigh   = FT5426_P8_XH_REG;
      regAddressPWeight = FT5426_P8_WEIGHT_REG;
      regAddressPMisc   = FT5426_P8_MISC_REG;
      break;

    case 8 :
      regAddressXHigh   = FT5426_P9_XH_REG;
      regAddressPWeight = FT5426_P9_WEIGHT_REG;
      regAddressPMisc   = FT5426_P9_MISC_REG;
      break;

    case 9 :
      regAddressXHigh   = FT5426_P10_XH_REG;
      regAddressPWeight = FT5426_P10_WEIGHT_REG;
      regAddressPMisc   = FT5426_P10_MISC_REG;
      break;

    default :
      break;

    } /* end switch(touchIdx) */

    /* Read Event Id of touch index */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressXHigh);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressXHigh, &ucReadData, 1);
    * pEvent = (ucReadData & FT5426_TOUCH_EVT_FLAG_MASK) >> FT5426_TOUCH_EVT_FLAG_SHIFT;

    /* Read weight of touch index */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressPWeight);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressPWeight, &ucReadData, 1);
    * pWeight = (ucReadData & FT5426_TOUCH_WEIGHT_MASK) >> FT5426_TOUCH_WEIGHT_SHIFT;

    /* Read area of touch index */
//    ucReadData = TS_IO_Read(DeviceAddr, regAddressPMisc);
		BSP_I2C1_ReadReg(FT5426_I2C_SLAVE_ADDRESS, regAddressPMisc, &ucReadData, 1);
    * pArea = (ucReadData & FT5426_TOUCH_AREA_MASK) >> FT5426_TOUCH_AREA_SHIFT;

  } /* of if(touchIdx < ft5426_handle.currActiveTouchNb) */
}

#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

/** @defgroup ft5426_Static_Function_Body
  * @{
  */

/* Static functions bodies-----------------------------------------------*/


/**
  * @brief  Return the status of I2C was initialized or not.
  * @param  None.
  * @retval : I2C initialization status.
  */
static uint8_t ft5426_Get_I2C_InitializedStatus(void)
{
  return(ft5426_handle.i2cInitialized);
}

/**
  * @brief  I2C initialize if needed.
  * @param  None.
  * @retval : None.
  */
static void ft5426_I2C_InitializeIfRequired(void)
{
  if(ft5426_Get_I2C_InitializedStatus() == FT5426_I2C_NOT_INITIALIZED)
  {
    /* Initialize TS IO BUS layer (I2C) */
    BSP_I2C1_Init();
		
//		TS_IO_Delay(100);

    /* Set state to initialized */
    ft5426_handle.i2cInitialized = FT5426_I2C_INITIALIZED;
  }
}

/**
  * @brief  Basic static configuration of TouchScreen
  * @param  DeviceAddr: FT5426 Device address for communication on I2C Bus.
  * @retval Status FT5426_STATUS_OK or FT5426_STATUS_NOT_OK.
  */
static uint32_t ft5426_TS_Configure(uint16_t DeviceAddr)
{
  uint32_t status = FT5426_STATUS_OK;

  /* Nothing special to be done for FT5426 */

  return(status);
}

/**
  * @brief  Check if the device instance of the selected address is already registered
  *         and return its index  
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval Index of the device instance if registered, 0xFF if not.
  */
//static uint8_t ft5426_GetInstance(uint16_t DeviceAddr)
//{
//  uint8_t idx = 0;
//  
//  /* Check all the registered instances */
//  for(idx = 0; idx < FT5426_MAX_INSTANCE ; idx ++)
//  {
//    if(ft5426[idx] == DeviceAddr)
//    {
//      return idx; 
//    }
//  }
//  
//  return 0xFF;
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

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
