/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_ULPM_DataClock/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to operate the DSI ULPM (Ultra Low Power Mode)
  *          on data lanes and clock lane in a use case with display in WVGA Landscape
  *          of size (800x480) using the STM32H7xx HAL API and BSP.
  *          This is the main program for Cortex-M7.  
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
#include "image_320x240_argb8888.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup LCD_DSI_ULPM_DataClock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DSI_HandleTypeDef hdsi_eval;
extern LTDC_HandleTypeDef  hltdc_eval;
static DMA2D_HandleTypeDef   hdma2d;

uint32_t frameCnt = 0;
char str_display[60] = "";

__IO uint32_t ltdc_lowpower_request = 0;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint8_t CheckForUserInput(void);
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7) 
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */

  /* Configure the MPU attributes as Write Through for SDRAM*/
  MPU_Config();
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* This sample code displays a fixed image 800x480 on LCD KoD in */
  /* orientation mode landscape and DSI mode video burst           */

  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* When system initialization is finished, Cortex-M7 could wakeup (when needed) the Cortex-M4  by means of 
     HSEM notification or by any D2 wakeup source (SEV,EXTI..)   */  
	 
  /* Initialize used Leds */
  BSP_LED_Init(LED1);

  /* Configure Tamper push-button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

   /* Initialize the LCD DSI in Video Burst mode with LANDSCAPE orientation */
   /* Initialize the LCD   */
  if(BSP_LCD_Init() != LCD_OK)
  {
    Error_Handler();
  }
  /* Program a line event at line 0 */
  HAL_LTDC_ProgramLineEvent(&hltdc_eval, 0);
  
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);

  BSP_LCD_Clear(LCD_COLOR_BLACK);  

  /* Copy texture to be displayed on LCD from Flash to SDRAM */
  CopyPicture((uint32_t *)&image_320x240_argb8888, (uint32_t *)LCD_FB_START_ADDRESS, 240, 100, 320, 240);
  
  /* Prepare area to display frame number in the image displayed on LCD */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 400, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);

  /* Display title */
  BSP_LCD_DisplayStringAt(0, 420, (uint8_t *) "LCD_DSI_ULPM_DataClock example", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 440, (uint8_t *) "Press TAMPER button to enter ULPM", CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
      
  /* Infinite loop */
  while (1)
  {
    /* Clear previous line */
    BSP_LCD_ClearStringLine(460);
    
    /* New text to display */
    sprintf(str_display, ">> Frame Nb : %lu", frameCnt);
    
    /* Print updated frame number */
    BSP_LCD_DisplayStringAt(0, 460, (uint8_t *)str_display, CENTER_MODE);
    
    if (CheckForUserInput() > 0)
    {
      /* Clear previous line */
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_ClearStringLine(440);
      BSP_LCD_DisplayStringAt(0, 440, (uint8_t *) "          Enter ULPM - switch Off LCD 6 seconds          ", CENTER_MODE);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      
      HAL_Delay(1000); 
      /* Display Off with ULPM management Data and clock lane integrated */
      BSP_LCD_DisplayOff();

      ltdc_lowpower_request = 1;
      /* wait for vertical blanking period to proceed with disabling the LTDC clock and entering DSI to Ultra Low Power Mode */
      /* Note that the flag ltdc_lowpower_request is reset and the LTDC is disabled within the HAL_LTDC_LineEventCallback */
      while(ltdc_lowpower_request != 0)
      {
      }      
      
      /* Enter ultra low power mode (data and clock lanes) */
      /* This means : laneByteClock is derived from PLL2.PLL2Q : ie ck_plldsi @ 80 MHz = 800 MHz / PLLQ=10 */
      /* DSI State machine start and wait until ULPM state for Data and Clock lanes, then when reached, switch off PLL DPHY */
      HAL_DSI_EnterULPM(&hdsi_eval);
      BSP_LED_On(LED1);
      
      HAL_Delay(6000);
      
      BSP_LCD_ClearStringLine(440);
      BSP_LCD_DisplayStringAt(0, 440, (uint8_t *) " Exited ULPM with success - Press To enter Again ULPM. ", CENTER_MODE);
      
      /* Exit ultra low power mode (data and clock lanes)
      * This means : PLL DPHY is first restarted and wait until locked with its initial settings
      * DSI State machine ULPM Out start and wait until ULPM state is Out for Data and Clock lanes
      * laneByteClock is then switched back on PLL DPHY
      */
      HAL_DSI_ExitULPM(&hdsi_eval);
      BSP_LED_Off(LED1);

      /* Switch On bit LTDCEN */
      __HAL_LTDC_ENABLE(&hltdc_eval);
      
      /* Display On with ULPM exit Data and clock lane integrated */
      BSP_LCD_DisplayOn();

      /* Program a line event at line 0 */
      HAL_LTDC_ProgramLineEvent(&hltdc_eval, 0);  
      
    }
  }
}

/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
  if(ltdc_lowpower_request == 0)
  {
    /* Increment frame count */
    frameCnt++;
    HAL_LTDC_ProgramLineEvent(hltdc, 0);
  }
  else
  {
    /* Application asks to enter DSI to Ultra low power mode */
    
    /* Switch Off bit LTDCEN */
    __HAL_LTDC_DISABLE(&hltdc_eval);

    /* Reset the ltdc_lowpower_request*/
    ltdc_lowpower_request = 0;    
  }
}


/**
  * @brief  Check for user input.
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
static uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_RESET)
  {
    HAL_Delay(10);
    do
    {
      /* Clear previous line */
      BSP_LCD_ClearStringLine(460);

      /* New text to display */
      sprintf(str_display, ">> Frame Nb : %lu", frameCnt);

      /* Print updated frame number */
      BSP_LCD_DisplayStringAt(0, 460, (uint8_t *)str_display, CENTER_MODE);

    } while (BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_RESET);

    return 1 ;
  }
  return 0;
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode   
  * @retval None
  */
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{   
  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;
  
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 800 - xsize;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */   
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */  

  hdma2d.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
  }   
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CM7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (CM4 CPU, AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO);

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
  
/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

 /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)       
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
 */
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is 0xD0000000 .
  *         The Configured Region Size is 32MB because same as SDRAM size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief Error Handler
  * @retval None
  */
static void Error_Handler(void)
{
  while(1) { ; } /* Blocking on error */
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
