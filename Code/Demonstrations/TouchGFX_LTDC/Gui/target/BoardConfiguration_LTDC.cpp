
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <STM32H7DMA.hpp>
#include <STM32H7HAL_LTDC.hpp>

#include <STM32H747TouchController.hpp>
#include <STM32H7Instrumentation.hpp>

/***********************************************************
 ******         24 Bits Per Pixel Support            *******
 ***********************************************************
 *
 * The default bit depth of the framebuffer is 16bpp. If you want 24bpp support, define the symbol "USE_BPP" with a value
 * of "24", e.g. "USE_BPP=24". This symbol affects the following:
 *
 * 1. Type of TouchGFX LCD (16bpp vs 24bpp)
 * 2. Bit depth of the framebuffer(s)
 * 3. TFT controller configuration.
 *
 * WARNING: Remember to modify your image formats accordingly in app/config/. Please see the following knowledgebase article
 * for further details on how to choose and configure the appropriate image formats for your application:
 *
 * https://touchgfx.zendesk.com/hc/en-us/articles/206725849
 */

extern "C"
{
#include "stm32h7xx_hal.h"

#ifdef USE_STM32H747I_DISCO
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_sd.h"
#include "stm32h747i_discovery_sdram.h"
#include "stm32h747i_discovery_qspi.h"
#else
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_io.h"
#include "stm32h747i_eval_sd.h"
#include "stm32h747i_eval_sdram.h"
#include "stm32h747i_eval_qspi.h"
#include "../Components/at800480/at800480.h"
#endif

#include "bsp.h"
#include "rtc_app.h"

typedef struct pwr_db
{
  __IO uint32_t t[0x30/4];
  __IO uint32_t PDR1;
}PWDDBG_TypeDef;

#define LCD_BL_Pin 				GPIO_PIN_10
#define LCD_BL_GPIO_Port 	GPIOF

#define PWR_CFG_SMPS  0xCAFECAFE
#define PWR_CFG_LDO   0x5ACAFE5A

static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;

static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define LAYER0_ADDRESS  (0xD0000000)

#if !defined(USE_BPP) || USE_BPP==16
#define HACT            200      /* Note: 16bpp: Screen divided in 4 areas of 200 pixels to avoid DSI tearing */
#elif USE_BPP==24
#define HACT            400      /* Note: 24bpp: Screen divided in 2 areas of 400 pixels to avoid DSI tearing */
#else
#error Unknown USE_BPP
#endif

#define PWDDBG ((PWDDBG_TypeDef*)PWR)
}

#define DEVICE_IS_CUT_2_1()      (HAL_GetREVID() & 0x21ff) ? 1 : 0
#define DEVICE_IS_CUT_2_X()      (HAL_GetREVID() & 0x2fff) ? 1 : 0
#define DEVICE_IS_CUT_1_2()      (HAL_GetREVID() & 0x12ff) ? 1 : 0

uint8_t pCols[4][4] =
{
    {0x00, 0x00, 0x00, 0xC7}, /*   0 -> 199 */
    {0x00, 0xC8, 0x01, 0x8F}, /* 200 -> 399 */
    {0x01, 0x90, 0x02, 0x57}, /* 400 -> 599 */
    {0x02, 0x58, 0x03, 0x1F}, /* 600 -> 799 */
};

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */

uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */

static uint32_t frameBuf0 = (uint32_t)(0xD0000000);

#ifdef USE_STM32H747I_DISCO
#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_TEMPSENSOR

/* ADC handler declaration */
static ADC_HandleTypeDef    AdcHandle;

/**
  * @brief  Configure the ADC.
  * @param  None
  * @retval None
  */
uint32_t TEMP_SENSOR_Init(void)
{
  uint32_t ret = HAL_OK;

  ADC_ChannelConfTypeDef sConfig;

   /* ADCx Periph clock enable */
  ADCx_CLK_ENABLE();

  memset(&AdcHandle, 0, sizeof(AdcHandle));
  memset(&sConfig, 0, sizeof(sConfig));

  /* Configure the ADC peripheral */
  AdcHandle.Instance                      = ADCx;

  /* Clear all control registers */
  ADC_CLEAR_COMMON_CONTROL_REGISTER(&AdcHandle);

  AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;
  AdcHandle.Init.Resolution               = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode             = ENABLE;
  AdcHandle.Init.ContinuousConvMode       = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode    = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T1_CC1;
  AdcHandle.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
  AdcHandle.Init.NbrOfConversion          = 1;
  AdcHandle.Init.NbrOfDiscConversion      = 1;
  AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  AdcHandle.Init.LowPowerAutoWait         = DISABLE;
  AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;

  *((uint32_t *) (ADC12_COMMON_BASE)) |= 0xC00000; // enable TSEN bit in  ADC CCR in common register

  ret = HAL_ADC_Init(&AdcHandle);
  if (ret == HAL_OK)
  {
    /* Configure ADC Temperature Sensor Channel */
    sConfig.Channel      = ADCx_CHANNEL;                /* Sampled channel number */
    sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
    sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;   /* Sampling time (number of clock cycles unit) */
    sConfig.Offset       = 0;                           /* Parameter discarded because offset correction is disabled */
    sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */

    ret = HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
  }

  return ret;
}

uint32_t TEMP_SENSOR_Start(void)
{
  return HAL_ADC_Start(&AdcHandle);
}

uint32_t TEMP_SENSOR_Stop(void)
{
  return HAL_ADC_Stop(&AdcHandle);
}

/**
  * @brief  Get JunctionTemp level in 12 bits.
  * @retval JunctionTemp level(0..0xFFF) / 0xFFFFFFFF : Error
  */
uint32_t TEMP_SENSOR_GetValue(void)
{
  uint32_t JTemp = -1;
  uint32_t ConvertedValue = HAL_ADC_GetValue(&AdcHandle);

#ifdef Calibration_ON
  /* 110 �C - 30�C
  -----------------  *  (ConvertedValue - TS_CAL1 )+30 �C
  TS_CAL2-TS_CAL1
  TS_CAL2 :the temperature sensor calibration value acquired at 110�C stored at TS1_T0[1:0]
  TS_CAL1 :the temperature sensor calibration value acquired at 30�C stored at TS1_T0[1:0]
  */
#define VREFANALOG_VOLTAGE  3300
  JTemp= __LL_ADC_CALC_TEMPERATURE(VREFANALOG_VOLTAGE,\
  ConvertedValue,\
  ADC_RESOLUTION_12B)                              \
  (((( ((int32_t)((__LL_ADC_CONVERT_DATA_RESOLUTION((ConvertedValue),     \
  (ADC_RESOLUTION_12B),          \
  LL_ADC_RESOLUTION_16B)         \
  * (VREFANALOG_VOLTAGE))                                     \
  / TEMPSENSOR_CAL_VREFANALOG)                                     \
  - (int32_t) *TEMPSENSOR_CAL1_ADDR)                                         \
  ) * (int32_t)(TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP)                    \
  ) / (int32_t)((int32_t)*TEMPSENSOR_CAL2_ADDR - (int32_t)*TEMPSENSOR_CAL1_ADDR) \
  ) + TEMPSENSOR_CAL1_TEMP                                                        \
  );
#else
  /* Compute the Junction Temperature value */
  /* 110-30
  ---------  *  (ConvertedValue - 0 )+30
  4095-0                               */
  JTemp=((80*ConvertedValue)/4095)+30;
#endif

  return JTemp;
}
#endif

extern "C" uint32_t LCD_GetXSize()
{
    return 800;//OTM8009A_800X480_WIDTH;
}

extern "C" uint32_t LCD_GetYSize()
{
    return 480;//OTM8009A_800X480_HEIGHT;
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_LayerInit(uint16_t LayerIndex, uint32_t Address)
{
    LTDC_LayerCfgTypeDef Layercfg;

    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = LCD_GetYSize();
    Layercfg.FBStartAdress = Address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.ImageHeight = LCD_GetYSize();

#if !defined(USE_BPP) || USE_BPP==16
    Layercfg.WindowX1 = LCD_GetXSize(); 	//Note: Div4 due to screen being divided into 4 areas.
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    Layercfg.ImageWidth = LCD_GetXSize(); //Note: Div4 due to screen being divided into 4 areas.
#elif USE_BPP==24
    Layercfg.WindowX1 = LCD_GetXSize(); 	//Note: Div2 due to screen being divided into 2 areas.
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = LCD_GetXSize(); //Note: Div2 due to screen being divided into 2 areas.
#else
#error Unknown USE_BPP
#endif

    HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, LayerIndex);
}

/**
  * @brief  LCD backligh 
  * @retval None
  */
void HAL_LCD_BackLighInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LCD_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
    /* DeInit */
    HAL_LTDC_DeInit(&hltdc);

    /* LTDC Config */
    /* Timing and polarity */
		hltdc.Init.HorizontalSync = (AT800480_HSYNC - 1);
		hltdc.Init.VerticalSync = (AT800480_VSYNC - 1);
		hltdc.Init.AccumulatedHBP = (AT800480_HSYNC + AT800480_HBP - 1);
		hltdc.Init.AccumulatedVBP = (AT800480_VSYNC + AT800480_VBP - 1);  
		hltdc.Init.AccumulatedActiveH = (AT800480_HEIGHT + AT800480_VSYNC + AT800480_VBP - 1);
		hltdc.Init.AccumulatedActiveW = (AT800480_WIDTH + AT800480_HSYNC + AT800480_HBP - 1);
		hltdc.Init.TotalHeigh = (AT800480_HEIGHT + AT800480_VSYNC + AT800480_VBP + AT800480_VFP - 1);
		hltdc.Init.TotalWidth = (AT800480_WIDTH + AT800480_HSYNC + AT800480_HBP + AT800480_HFP - 1);

    /* background value */
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;

    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Instance = LTDC;

    HAL_LTDC_Init(&hltdc);
}

/**
  * @brief  BSP LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and desactivating it later.
  */
static void LCD_Reset(void)
{
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
static void LCD_MspInit(void)
{
    GPIO_InitTypeDef gpio_init_structure;
		/** @brief Enable the LTDC clock */
    __HAL_RCC_LTDC_CLK_ENABLE();

    /** @brief Toggle Sw reset of LTDC IP */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();

    /** @brief Enable the DMA2D clock */
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /** @brief Toggle Sw reset of DMA2D IP */
    __HAL_RCC_DMA2D_FORCE_RESET();
    __HAL_RCC_DMA2D_RELEASE_RESET();

		/* Enable GPIOs clock */
    __HAL_RCC_GPIOI_CLK_ENABLE();
		__HAL_RCC_GPIOJ_CLK_ENABLE();
		__HAL_RCC_GPIOK_CLK_ENABLE();

    /*** LTDC Pins configuration ***/
    /* GPIOI configuration */
		gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
		gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
		gpio_init_structure.Pull      = GPIO_NOPULL;
		gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
		gpio_init_structure.Alternate = GPIO_AF14_LTDC;  
		HAL_GPIO_Init(GPIOI, &gpio_init_structure);

		/* GPIOJ configuration */  
		gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
																		GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
																		GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
																		GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
		gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
		gpio_init_structure.Pull      = GPIO_NOPULL;
		gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
		gpio_init_structure.Alternate = GPIO_AF14_LTDC;  
		HAL_GPIO_Init(GPIOJ, &gpio_init_structure);  

		/* GPIOK configuration */  
		gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
																		GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
		gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
		gpio_init_structure.Pull      = GPIO_NOPULL;
		gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
		gpio_init_structure.Alternate = GPIO_AF14_LTDC;  
		HAL_GPIO_Init(GPIOK, &gpio_init_structure);

    HAL_LCD_BackLighInit();
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);//turn on backlight

    /** @brief NVIC configuration for LTDC interrupt that is now enabled */
    HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);

    /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
    HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief  Initializes the DSI LCD.
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
    LCD_Reset();
    LCD_MspInit();

#if !defined(USE_BPP) || USE_BPP==16
    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 625 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 625/10 = 62.5 Mhz */
    /* LTDC clock frequency = PLLLCDCLK = 62.5 Mhz */
    /* USB uses same pll3 as clock frequency and PLL3Q as devider: USB clock frequency = 625 / 13 = 48.07 Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 5;
    PeriphClkInitStruct.PLL3.PLL3N = 125;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 13;
    PeriphClkInitStruct.PLL3.PLL3R = 10;
#elif USE_BPP==24
//    /* LCD clock configuration */
//    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
//    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
//    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/21 = 38.095 Mhz */
//    /* LTDC clock frequency = PLLLCDCLK = 38.095 Mhz */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
//    PeriphClkInitStruct.PLL3.PLL3M = 5;
//    PeriphClkInitStruct.PLL3.PLL3N = 160;
//    PeriphClkInitStruct.PLL3.PLL3P = 2;
//    PeriphClkInitStruct.PLL3.PLL3Q = 2;
//    PeriphClkInitStruct.PLL3.PLL3R = 40;
		
		/* LCD clock configuration */
    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/21 = 38.095 Mhz */
    /* LTDC clock frequency = PLLLCDCLK = 38.095 Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 5;
    PeriphClkInitStruct.PLL3.PLL3N = 160;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 21;
#else
#error Unknown USE_BPP
#endif
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    /* Initialize LTDC */
    LTDC_Init();
    __HAL_LTDC_DISABLE(&hltdc);

    /* Initialize LTDC layer 0 iused for Hint */
    LCD_LayerInit(0, LAYER0_ADDRESS);

    /* Update pitch: drawing is done on the whole physical X Size */
    HAL_LTDC_SetPitch(&hltdc, LCD_GetXSize(), 0);

    /* Finally enable the LTDC IP */
    __HAL_LTDC_ENABLE(&hltdc);

    return 0;
}

namespace touchgfx
{
void hw_init()
{
    if(!DEVICE_IS_CUT_2_X())
    {
        if(!DEVICE_IS_CUT_1_2())
        {
            *((uint32_t*)0x51046108) = 1; /* DMA2D issue with cut 1.1 - fixed in cut 1.2 */
        }
        *((uint32_t*)0x51008108) = 1; /* AXI SRAM issue with cut1.2 - fixed in cut 2.0 */
    }

    /* Enable RTC back-up registers access */
    HAL_PWR_EnableBkUpAccess();

    MPU_Config();
    HAL_Init();

    SystemClock_Config();

    __HAL_RCC_CRC_CLK_ENABLE();

    /* Initialize the QSPI */
    BSP_QSPI_Init();
    BSP_QSPI_EnableMemoryMappedMode();
    HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

    /* Initialize external SDRAM */
    BSP_SDRAM_Init();

    /* Disable FMC Bank1 to avoid speculative/cache accesses */
    FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;

#ifdef USE_STM32H747I_DISCO
    /* Init TEMP Sensor */
    TEMP_SENSOR_Init();
#else
    BSP_POTENTIOMETER_Init();
#endif

    /* Initialized RTC */
    RTC_CalendarBkupInit();

    GPIO::init();

    /* Enable CPU L1 Cache */
    CPU_CACHE_Enable();

    /* Initialize display */
    LCD_Init();
}

STM32H7DMA dma;
STM32H747TouchController tc;
STM32H7Instrumentation mcuInstr;

#if !defined(USE_BPP) || USE_BPP==16
static LCD16bpp display;
static uint16_t bitDepth = 16;
#elif USE_BPP==24
static LCD24bpp display;
static uint16_t bitDepth = 24;
#else
#error Unknown USE_BPP
#endif

void touchgfx_init()
{
    uint16_t dispWidth  = 800;
    uint16_t dispHeight = 480;
    uint32_t offset     = (dispWidth * dispHeight * (bitDepth/8)) * 2; /* start address after the snapshot buffer */
    uint8_t *bmpCache   =  reinterpret_cast<uint8_t*>(frameBuf0) + offset;

    HAL& hal = touchgfx_generic_init<STM32H7HAL>(dma, display, tc, dispWidth, dispHeight,
                                                     (uint16_t*)bmpCache, 4 * 1024 * 1024, 128);

    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitDepth, false, true);

    hal.setTouchSampleRate(1);
    hal.setFingerSize(5);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}
}

void OneTimeInitializeDefaultLayer()
{

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
#if defined(USE_PWR_LDO_SUPPLY)
    uint32_t power_config = PWR_CFG_LDO;
#else
    uint32_t power_config = PWR_CFG_SMPS;
#endif /* USE_PWR_LDO_SUPPLY */
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
#if defined(USE_STM32H747I_EVAL_OOB)
    HAL_PWR_EnableBkUpAccess();
    power_config = READ_REG(BKP_REG_PWR_CFG);
    HAL_PWR_DisableBkUpAccess();
#endif /* USE_STM32H747I_EVAL_OOB */

    if(power_config == PWR_CFG_LDO)
    {
        HAL_PWREx_ConfigSupply(PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO);
    }
    else
    {
      if(DEVICE_IS_CUT_2_1() == 0)
      {
          /* WA to avoid loosing SMPS regulation in run mode */
          PWDDBG->PDR1 = 0xCAFECAFE;
          __DSB();
          PWDDBG->PDR1 |= (1<<5 | 1<<3);
          __DSB();
      }
      HAL_PWREx_ConfigSupply(PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO);
    }

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
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;

    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(ret != HAL_OK)
    {
        while (1) { ; }
    }

    /* PLL3 for USB Clock */
    PeriphClkInitStruct.PLL3.PLL3M = 25;
    PeriphClkInitStruct.PLL3.PLL3N = 336;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 7;

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

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
        while (1) { ; }
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
* @brief  Configure the MPU attributes as Write Through for Internal D1SRAM.
* @note   None
* @param  None
* @retval None
*/
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    HAL_MPU_Disable();

   /* Configure unused area of QSPI region as strongly ordered.
    * This is *important* to avoid unintentional fetches from illegal
    * addresses due to cache/speculation which would halt the MCU.
    */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as WT for QSPI */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup SDRAM in Write-through (framebuffer) */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0xD0000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup AXI SRAM, SRAM1 and SRAM2 in Write-through */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE/*MPU_ACCESS_BUFFERABLE*/;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE/*MPU_INSTRUCTION_ACCESS_DISABLE*/;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x10000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER4;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup SRAM3 in Write-through */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x10040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER5;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable D-Cache */
  SCB_EnableDCache();

  /* Enable I-Cache */
  SCB_EnableICache();
}