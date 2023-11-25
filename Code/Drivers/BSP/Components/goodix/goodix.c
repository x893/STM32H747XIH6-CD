/**
  ******************************************************************************
  * @file    goodix.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-June-2015
  * @brief   This file provides a set of functions needed to manage the GT911
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
#include "goodix.h"
#include "stm32h7xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup GT911
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/** @defgroup GT911_Private_Types_Definitions
  * @{
  */

/* Private define ------------------------------------------------------------*/

/** @defgroup GT911_Private_Defines
  * @{
  */

/* Private macro -------------------------------------------------------------*/

/** @defgroup GT911_Private_Macros
  * @{
  */

/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef heval_I2c;
/** @defgroup GT911_Private_Variables
  * @{
  */

/* Touch screen driver structure initialization */
TS_DrvTypeDef Goodix_ts_drv =
{
  Goodix_Init,
  Goodix_ReadID,
  Goodix_Reset,

  Goodix_TS_Start,
  Goodix_TS_DetectTouch,
  Goodix_TS_GetXY,

  Goodix_TS_EnableIT,
  Goodix_TS_ClearIT,
  Goodix_TS_ITStatus,
  Goodix_TS_DisableIT
};

/* Global GT911 handle */
static Goodix_handle_TypeDef Goodix_handle = { GOODIX_I2C_NOT_INITIALIZED, 0, 0};

/**
  * @brief  Return the status of I2C was initialized or not.
  * @param  None.
  * @retval : I2C initialization status.
  */
static uint8_t Goodix_Get_I2C_InitializedStatus(void)
{
  return(Goodix_handle.i2cInitialized);
}

/**
  * @brief  I2C initialize if needed.
  * @param  None.
  * @retval : None.
  */
static void Goodix_I2C_InitializeIfRequired(void)
{
  if(Goodix_Get_I2C_InitializedStatus() == GOODIX_I2C_NOT_INITIALIZED)
  {
    /* Initialize TS IO BUS layer (I2C) */
    I2Cx_Init();
    /* Set state to initialized */
    Goodix_handle.i2cInitialized = GOODIX_I2C_INITIALIZED;
  }
}
void Goodix_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  TP_INT_GPIO_CLK_ENABLE();
	TP_RST_GPIO_CLK_ENABLE();

  /*Configure GPIO pin : TP_RST_Pin */
  GPIO_InitStruct.Pin = TP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TP_RST_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pin : TP_INT_Pin */
  GPIO_InitStruct.Pin = TP_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TP_INT_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(TP_INT_GPIO_Port, TP_INT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_RESET);
	
	HAL_Delay(100);
	HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	
	HAL_GPIO_WritePin(TP_INT_GPIO_Port, TP_INT_Pin, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = TP_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TP_INT_GPIO_Port, &GPIO_InitStruct);	
}
//void Goodix_Config_Int_Pin(uint16_t DeviceAddr)
//{
//	uint8_t Goodix_reg_ID[3] = {0x80,0x4D,0x05};

//	HAL_I2C_Master_Transmit(&heval_I2c, DeviceAddr, Goodix_reg_ID, 3,100);
//}
/**
  * @brief  Initialize the ft5336 communication bus
  *         from MCU to FT5336 : ie I2C channel initialization (if required).
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5336).
  * @retval None
  */
void Goodix_Init(uint16_t DeviceAddr)
{
  /* Wait at least 200ms after power up before accessing registers
   * Trsi timing (Time of starting to report point after resetting) from GT911 datasheet */

	Goodix_IO_Init();
//	HAL_Delay(200);
  /* Initialize I2C link if needed */
  Goodix_I2C_InitializeIfRequired();
//	Goodix_Config_Int_Pin(DeviceAddr);
}

uint16_t Goodix_ReadID(uint16_t DeviceAddr)
{
  uint8_t Goodix_reg_ID[2] = {0x81,0x40};
	uint8_t ID_buffer[4];
	
	/* Initialize I2C link if needed */
  Goodix_I2C_InitializeIfRequired();
	
	HAL_I2C_Master_Transmit(&heval_I2c, DeviceAddr, Goodix_reg_ID, 2,100);
		
	HAL_I2C_Master_Receive(&heval_I2c, DeviceAddr+1, ID_buffer, 4, 100);
	
	return (ID_buffer[0]<<8) | ID_buffer[1];
}

/**
  * @brief  Return if there is touch detected or not.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval Touch detected state.
  */
void Goodix_Clea_Detect(uint16_t DeviceAddr)
{
  uint8_t Goodix_reg_Status_Clea[3] = {0x81,0x4E,0x00};//Write 0x00 to 0x814E REG
	
	HAL_I2C_Master_Transmit(&heval_I2c, DeviceAddr, Goodix_reg_Status_Clea, 3,1000);
}

/**
  * @brief  Return if there is touches detected or not.
  *         Try to detect new touches and forget the old ones (reset internal global
  *         variables).
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval : Number of active touches detected (can be 0, 1 or 2).
  */
uint8_t Goodix_TS_DetectTouch(uint16_t DeviceAddr)
{
  uint8_t nbTouch = 0;
	uint8_t Goodix_Status;
	uint8_t Goodix_reg_Status[2] = {0x81,0x4E};

  /* Read register 0x814E to check number of touches detection */
	HAL_I2C_Master_Transmit(&heval_I2c, DeviceAddr, Goodix_reg_Status, 2,100);
	HAL_I2C_Master_Receive(&heval_I2c, DeviceAddr+1, &Goodix_Status, 1, 100);
	
	if(Goodix_Status > 0x80)
	{
		nbTouch = Goodix_Status & 0x0F;

		if(nbTouch > GOODIX_MAX_DETECTABLE_TOUCH)
		{
			/* If invalid number of touch detected, set it to zero */
			nbTouch = 0;
		}

		/* Update ft5336 driver internal global : current number of active touches */
		Goodix_handle.currActiveTouchNb = nbTouch;

		/* Reset current active touch index on which to work on */
		Goodix_handle.currActiveTouchIdx = 0;
	}
	if((Goodix_Status & 0x80) == 0x80)
		Goodix_Clea_Detect(GOODIX_ADDRESS);

  return(nbTouch);
}

/**
  * @brief  Get the touch screen X and Y positions values
  *         Manage multi touch thanks to touch Index global
  *         variable 'ft5336_handle.currActiveTouchIdx'.
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  X: Pointer to X position value
  * @param  Y: Pointer to Y position value
  * @retval None.
  */
void Goodix_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y)
{
	uint8_t Goodix_reg_Pointx[2];
	uint8_t Goodix_Point_value[4];

  if(Goodix_handle.currActiveTouchIdx < Goodix_handle.currActiveTouchNb)
  {
		switch(Goodix_handle.currActiveTouchIdx)
    {
    case 0 :
      Goodix_reg_Pointx[0] = 0x81;
			Goodix_reg_Pointx[1] = 0x50;
      break;

    case 1 :
      Goodix_reg_Pointx[0] = 0x81;
			Goodix_reg_Pointx[1] = 0x58;
      break;

    case 2 :
      Goodix_reg_Pointx[0] = 0x81;
			Goodix_reg_Pointx[1] = 0x60;
      break;

    case 3 :
      Goodix_reg_Pointx[0] = 0x81;
			Goodix_reg_Pointx[1] = 0x68;
      break;

    case 4 :
      Goodix_reg_Pointx[0] = 0x81;
			Goodix_reg_Pointx[1] = 0x70;
      break;

    default :
			Goodix_reg_Pointx[0] = 0x81;
			Goodix_reg_Pointx[1] = 0x50;
      break;

    } /* end switch(ft5336_handle.currActiveTouchIdx) */

    //Read Point 1 Coordinate
		HAL_I2C_Master_Transmit(&heval_I2c, DeviceAddr, Goodix_reg_Pointx, 2,100);
		HAL_I2C_Master_Receive(&heval_I2c, DeviceAddr+1, Goodix_Point_value, 4, 100);

    /* Send back ready X position to caller */
    *X = (Goodix_Point_value[1]<<8) | Goodix_Point_value[0];

    /* Send back ready Y position to caller */
    *Y = (Goodix_Point_value[3]<<8) | Goodix_Point_value[2];

    Goodix_handle.currActiveTouchIdx++; /* next call will work on next touch */

  } /* of if(Goodix_handle.currActiveTouchIdx < Goodix_handle.currActiveTouchNb) */
}

/**
  * @brief  Software Reset the gt911.
  *         @note : Not applicable to GT911.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of GT911).
  * @retval None
  */
void Goodix_Reset(uint16_t DeviceAddr)
{

}

/**
  * @brief  Configures the touch Screen IC device to start detecting touches
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address).
  * @retval None.
  */
void Goodix_TS_Start(uint16_t DeviceAddr)
{
  /* By default set GT911 IC in Polling mode : no INT generation on GT911 for new touch available */
  /* Note TS_INT is active low                                                                    */
//  Goodix_TS_DisableIT(DeviceAddr);
}

/**
  * @brief  Configure the GT911 device to generate IT on given INT pin
  *         connected to MCU as EXTI.
  * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of GT911).
  * @retval None
  */
void Goodix_TS_EnableIT(uint16_t DeviceAddr)
{

}

/**
  * @brief  Configure the GT911 device to stop generating IT on the given INT pin
  *         connected to MCU as EXTI.
  * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of GT911).
  * @retval None
  */
void Goodix_TS_DisableIT(uint16_t DeviceAddr)
{

}

/**
  * @brief  Get IT status from GT911 interrupt status registers
  *         Should be called Following an EXTI coming to the MCU to know the detailed
  *         reason of the interrupt.
  *         @note : This feature is not applicable to GT911.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of GT911).
  * @retval TS interrupts status : always return 0 here
  */
uint8_t Goodix_TS_ITStatus(uint16_t DeviceAddr)
{
  /* Always return 0 as feature not applicable to GT911 */
  return 0;
}

/**
  * @brief  Clear IT status in GT911 interrupt status clear registers
  *         Should be called Following an EXTI coming to the MCU.
  *         @note : This feature is not applicable to GT911.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of FT5336).
  * @retval None
  */
void Goodix_TS_ClearIT(uint16_t DeviceAddr)
{
  /* Nothing to be done here for FT5336 */
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

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
