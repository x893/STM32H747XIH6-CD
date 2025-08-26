/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"

/* USER CODE BEGIN 0 */

/* Reset Operations */
#define SRAM_CMD_RST_EN         0x66
#define SRAM_CMD_RST            0x99

/* 4-byte Address Mode Operations */
#define SRAM_CMD_QUAD_ON		0x35
#define SRAM_CMD_QUAD_OFF		0xF5

#define SRAM_CMD_QUAD_WRITE     0x38

#define SRAM_CMD_READ_ID        0x9F
#define SRAM_CMD_QUAD_READ      0xEB

static void short_delay(void)
{
	uint32_t i = 9000;
	while (i-- != 0)
		__NOP();
}

/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;
MDMA_HandleTypeDef hmdma_quadspi_fifo_th;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */
	hqspi.Instance = QUADSPI;
	HAL_QSPI_DeInit(&hqspi);

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = (QSPI_PRESCALE-1);
  hqspi.Init.FifoThreshold = 2;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = QSPI_FLASH_SIZE;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_2;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

	if (QSPI_PSRAM_Init() != HAL_OK)
	{
		Error_Handler();
	}

  /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PG9     ------> QUADSPI_BK2_IO2
    PC11     ------> QUADSPI_BK2_NCS
    PG14     ------> QUADSPI_BK2_IO3
    PH2     ------> QUADSPI_BK2_IO0
    PH3     ------> QUADSPI_BK2_IO1
    PB2     ------> QUADSPI_CLK
    */
    GPIO_InitStruct.Pin = QSPI_BK2_IO2_Pin|QSPI_BK2_IO3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK2_NCS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_BK2_NCS_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK2_IO0_Pin|QSPI_BK2_IO1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_CLK_GPIO_Port, &GPIO_InitStruct);

    /* QUADSPI MDMA Init */
    /* QUADSPI_FIFO_TH Init */
    hmdma_quadspi_fifo_th.Instance = MDMA_Channel0;
    hmdma_quadspi_fifo_th.Init.Request = MDMA_REQUEST_QUADSPI_FIFO_TH;
    hmdma_quadspi_fifo_th.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
    hmdma_quadspi_fifo_th.Init.Priority = MDMA_PRIORITY_LOW;
    hmdma_quadspi_fifo_th.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    hmdma_quadspi_fifo_th.Init.SourceInc = MDMA_SRC_INC_BYTE;
    hmdma_quadspi_fifo_th.Init.DestinationInc = MDMA_DEST_INC_DISABLE;
    hmdma_quadspi_fifo_th.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
    hmdma_quadspi_fifo_th.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
    hmdma_quadspi_fifo_th.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
    hmdma_quadspi_fifo_th.Init.BufferTransferLength = 4;
    hmdma_quadspi_fifo_th.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
    hmdma_quadspi_fifo_th.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
    hmdma_quadspi_fifo_th.Init.SourceBlockAddressOffset = 0;
    hmdma_quadspi_fifo_th.Init.DestBlockAddressOffset = 0;
    if (HAL_MDMA_Init(&hmdma_quadspi_fifo_th) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_MDMA_ConfigPostRequestMask(&hmdma_quadspi_fifo_th, 0, 0) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(qspiHandle,hmdma,hmdma_quadspi_fifo_th);

    /* QUADSPI interrupt Init */
    HAL_NVIC_SetPriority(QUADSPI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PG9     ------> QUADSPI_BK2_IO2
    PC11     ------> QUADSPI_BK2_NCS
    PG14     ------> QUADSPI_BK2_IO3
    PH2     ------> QUADSPI_BK2_IO0
    PH3     ------> QUADSPI_BK2_IO1
    PB2     ------> QUADSPI_CLK
    */
    HAL_GPIO_DeInit(GPIOG, QSPI_BK2_IO2_Pin|QSPI_BK2_IO3_Pin);

    HAL_GPIO_DeInit(QSPI_BK2_NCS_GPIO_Port, QSPI_BK2_NCS_Pin);

    HAL_GPIO_DeInit(GPIOH, QSPI_BK2_IO0_Pin|QSPI_BK2_IO1_Pin);

    HAL_GPIO_DeInit(QSPI_CLK_GPIO_Port, QSPI_CLK_Pin);

    /* QUADSPI MDMA DeInit */
    HAL_MDMA_DeInit(qspiHandle->hmdma);

    /* QUADSPI interrupt Deinit */
    HAL_NVIC_DisableIRQ(QUADSPI_IRQn);
  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void HAL_QSPI_ErrorCallback(QSPI_HandleTypeDef *hqspi)
{
	__BKPT(0);
}
void HAL_QSPI_AbortCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	__BKPT(0);
}
void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	__BKPT(0);
}
void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
//	__BKPT(0);
}
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
//	__BKPT(0);
}
void HAL_QSPI_FifoThresholdCallback(QSPI_HandleTypeDef *hqspi)
{
	__BKPT(0);
}
void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef *hqspi)
{
	__BKPT(0);
}
void HAL_QSPI_TimeOutCallback(QSPI_HandleTypeDef *hqspi)
{
	__BKPT(0);
}


/**
  * @brief  This function reset the QSPI memory.
  * @param	None
  * @retval None
  */
HAL_StatusTypeDef QSPI_PSRAM_Init(void)
{
	uint8_t receive[8];

	QSPI_CommandTypeDef qcmd = {0};

	// exit qspi
	qcmd.Instruction = SRAM_CMD_QUAD_OFF;
	qcmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	qcmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	qcmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	qcmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;

	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	HAL_Delay(10);

	// reset
	qcmd.Instruction = SRAM_CMD_RST_EN;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	HAL_Delay(10);

	qcmd.Instruction = SRAM_CMD_RST;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	HAL_Delay(10);

	// check ID
	qcmd.Instruction = SRAM_CMD_READ_ID;
	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.Address = 0;
	qcmd.DataMode = QSPI_DATA_1_LINE;
	qcmd.NbData = 8;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	if (HAL_QSPI_Receive(&hqspi, receive, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	if ((receive[0] != 0x0D) || (receive[1] != 0x5D))
		return HAL_ERROR;

	// enable QSPI
	qcmd.Instruction = SRAM_CMD_QUAD_ON;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.NbData = 0;
	return HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
}

/**
 * @brief  This function write to QSPI memory.
 * @param	addr:
 * @param	data:
 * @param	len:
 * @retval None
 */
HAL_StatusTypeDef QSPI_PSRAM_Write(uint32_t addr, uint8_t* data, uint32_t len)
{
	QSPI_CommandTypeDef qcmd = {0};

	qcmd.Instruction = SRAM_CMD_QUAD_WRITE;
	qcmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	qcmd.Address = addr;
	qcmd.AddressMode = QSPI_ADDRESS_4_LINES;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.DataMode = QSPI_DATA_4_LINES;
	qcmd.NbData = len;
	qcmd.SIOOMode = QSPI_SIOO_INST_ONLY_FIRST_CMD;
	qcmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	qcmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	// return HAL_QSPI_Transmit(&hqspi, data), HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
	return HAL_QSPI_Transmit_DMA(&hqspi, data);
}

HAL_StatusTypeDef QSPI_PSRAM_Read(uint32_t addr, uint8_t* data, uint32_t len)
{
	QSPI_CommandTypeDef qcmd = {0};

	qcmd.Instruction = SRAM_CMD_QUAD_READ;
	qcmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	qcmd.Address = addr;
	qcmd.AddressMode = QSPI_ADDRESS_4_LINES;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.DataMode = QSPI_DATA_4_LINES;
	qcmd.NbData = len;
	qcmd.DummyCycles = 6;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		return HAL_ERROR;

	// return HAL_QSPI_Receive(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
	return HAL_QSPI_Receive_DMA(&hqspi, data);
}

HAL_StatusTypeDef QSPI_PSRAM_StartMemoryMapped(void)
{
	QSPI_CommandTypeDef qcmd = {0};
	QSPI_MemoryMappedTypeDef qcfg = {0};

	qcmd.Instruction = SRAM_CMD_QUAD_READ;
	qcmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	qcmd.AddressMode = QSPI_ADDRESS_4_LINES;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.DataMode = QSPI_DATA_4_LINES;
	qcmd.DummyCycles = 6;

	qcfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
	return HAL_QSPI_MemoryMapped(&hqspi, &qcmd, &qcfg);
}

HAL_StatusTypeDef QSPI_PSRAM_StopMemoryMapped(void)
{
	return HAL_QSPI_Abort(&hqspi);
}

/* USER CODE END 1 */
