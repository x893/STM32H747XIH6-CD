/*----------------------------------------------------------------------------------------------------------*/
/*                                                                                 */
/* Copyright by huang ying xin                                                          */
/* All rights reserved.                                                                                     */
/*                                                                                                          */
/*------------------------------------------------------------------------------------------------------------
  File Name        : tsc2007.c
  Version          : V0.1
  Date[mm/dd/yyyy] : 06/11/2016
  Description      : Touch funciton driver.
------------------------------------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "tsc2007.h"
#include "stm32h747i_eval.h"
#include "stm32h7xx_hal.h"

uint16_t z1_position;

extern I2C_HandleTypeDef heval_I2c;
extern void I2Cx_Init(void);
/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------------------------------------*/

#define TSC2007_ADDRESS        0x90//0x4A//

/* Private macro -------------------------------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------
  Function Name  : tsc2007_Init
  Description    : Initializes peripherals used by the TSC2007 driver.
  Input          : None
  Output         : None
  Return         : None
------------------------------------------------------------------------------------------------------------*/
void tsc2007_Init(void)
{
	/* Initialize IO BUS layer */
  I2Cx_Init(); 
}

/*------------------------------------------------------------------------------------------------------------
  Function Name  : tsc2007_MeasureXpos
  Description    : Measure the X position of touch point.
  Input          : -Mode: Conversion mode.                    
  Output         : The X position of touch point.
  Return         : None
------------------------------------------------------------------------------------------------------------*/
uint16_t tsc2007_MeasureXpos(void)
{
	uint8_t Xpos[2];
	uint16_t position;
	uint8_t command;
	command = Measure_X_Select | TSC2007_12_bit;
	
	HAL_I2C_Master_Transmit(&heval_I2c, TSC2007_ADDRESS, &command, 1,100);
		
	HAL_I2C_Master_Receive(&heval_I2c, TSC2007_ADDRESS, Xpos, 2, 100);
	
	position = ((Xpos[0]<<4) | (Xpos[1]>>4));
	return position;
}
//uint8_t tsc2007_MeasureXpos(void)
//{
////	uint8_t Xpos[2];
//	uint8_t command;
//	command = Measure_X_Select | TSC2007_8_bit;
//	
////	I2Cx_ReadMultiple(TSC2007_ADDRESS, (uint16_t)(Measure_X_Select | TSC2007_8_bit), I2C_MEMADD_SIZE_8BIT, Xpos, 2);
//	HAL_I2C_Master_Transmit(&hEvalI2c, TSC2007_ADDRESS, &command, 1,100);
//		
//	HAL_I2C_Master_Receive(&hEvalI2c, TSC2007_ADDRESS, Xpos, 2, 100);
//	
//	return Xpos[0];
//}

/*------------------------------------------------------------------------------------------------------------
  Function Name  : tsc2007_MeasureYpos
  Description    : Measure the Y position of touch point.
  Input          : -Mode: Conversion mode.                    
  Output         : The Y position of touch point.
  Return         : None
------------------------------------------------------------------------------------------------------------*/
uint16_t tsc2007_MeasureYpos(void)
{
	uint8_t Ypos[2];
	uint16_t position;
	uint8_t command;
	command = Measure_Y_Select | TSC2007_12_bit;
	
	HAL_I2C_Master_Transmit(&heval_I2c, TSC2007_ADDRESS, &command, 1,100);
	
	HAL_I2C_Master_Receive(&heval_I2c, TSC2007_ADDRESS, Ypos, 2, 100);
	
	position = ((Ypos[0]<<4) | (Ypos[1]>>4));
	return position;
} 
//uint8_t tsc2007_MeasureYpos(void)
//{
////	uint8_t Ypos[2];
//	uint8_t command;
//	command = Measure_Y_Select | TSC2007_8_bit;
//	
////	I2Cx_ReadMultiple(TSC2007_ADDRESS, (uint16_t)(Measure_Y_Select | TSC2007_8_bit), I2C_MEMADD_SIZE_8BIT, Ypos, 2);
//	HAL_I2C_Master_Transmit(&hEvalI2c, TSC2007_ADDRESS, &command, 1,100);
//	
//	HAL_I2C_Master_Receive(&hEvalI2c, TSC2007_ADDRESS, Ypos, 2, 100);
//	
//	return Ypos[0];
//}
/*------------------------------------------------------------------------------------------------------------
  Function Name  : tsc2007_MeasureZ1pos
  Description    : Measure the Z1 position of touch point.
  Input          : -Mode: Conversion mode.                    
  Output         : The Z1 position of touch point.
  Return         : None
------------------------------------------------------------------------------------------------------------*/
uint16_t tsc2007_MeasureZ1pos(void)
{
	uint8_t Z1pos[2];
	uint16_t position;
	uint8_t command;
	command = Measure_Z1_Select | TSC2007_12_bit;
	
	HAL_I2C_Master_Transmit(&heval_I2c, TSC2007_ADDRESS, &command, 1,100);
	
	HAL_I2C_Master_Receive(&heval_I2c, TSC2007_ADDRESS, Z1pos, 2, 100);
	
	position = ((Z1pos[0]<<4) | (Z1pos[1]>>4));
	return position;
}

/*------------------------------------------------------------------------------------------------------------
  Function Name  : tsc2007_MeasureZ2pos
  Description    : Measure the Z2 position of touch point.
  Input          : -Mode: Conversion mode.                    
  Output         : The Z2 position of touch point.
  Return         : None
------------------------------------------------------------------------------------------------------------*/
uint16_t tsc2007_MeasureZ2pos(void)
{
	uint8_t Z2pos[2];
	uint16_t position;
	uint8_t command;
	command = Measure_Z2_Select | TSC2007_12_bit;
	
	HAL_I2C_Master_Transmit(&heval_I2c, TSC2007_ADDRESS, &command, 1,100);
	
	HAL_I2C_Master_Receive(&heval_I2c, TSC2007_ADDRESS, Z2pos, 2, 100);
	
	position = ((Z2pos[0]<<4) | (Z2pos[1]>>4));
	return position;
}

/**
  * @brief  Return if there is touch detected or not.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval Touch detected state.
  */
uint8_t tsc2007_TS_DetectTouch()
{
  uint8_t ret = 0;
//	uint16_t z1_position;
	
	z1_position = tsc2007_MeasureZ1pos();
	if(z1_position > 0x000F)
		ret = 1;
	return ret;
}

