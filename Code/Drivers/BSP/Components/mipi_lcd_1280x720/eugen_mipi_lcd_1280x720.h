
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EUGEN_MIPI_LCD_1280X720_H
#define __EUGEN_MIPI_LCD_1280X720_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include <stdint.h>
#include "stm32h7xx_hal.h"

#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */
#endif /* __GNUC__ */

#define MIPI_LCD_ID  ((uint32_t) 0)

#define MIPI_DCS_EXIT_SLEEP_MODE 0x11
#define MIPI_DCS_SET_DISPLAY_ON	 0x29
      
/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define MIPI_LCD_ORIENTATION_PORTRAIT    ((uint32_t)0x00) /* Portrait orientation choice of LCD screen  */
#define MIPI_LCD_ORIENTATION_LANDSCAPE   ((uint32_t)0x01) /* Landscape orientation choice of LCD screen */

/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */
#define MIPI_LCD_FORMAT_RGB888    ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
#define MIPI_LCD_FORMAT_RGB565    ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

/**
  * @brief  MIPI_LCD_720X1280 Size
  */

/* Width and Height in Portrait mode */
#define  MIPI_LCD_720X1280_WIDTH             ((uint16_t)720)     /* LCD PIXEL WIDTH   */
#define  MIPI_LCD_720X1280_HEIGHT            ((uint16_t)1280)     /* LCD PIXEL HEIGHT  */

/* Width and Height in Landscape mode */
#define  MIPI_LCD_1280X720_WIDTH             ((uint16_t)1280)     /* LCD PIXEL WIDTH   */
#define  MIPI_LCD_1280X720_HEIGHT            ((uint16_t)720)     /* LCD PIXEL HEIGHT  */

/**
  * @brief  MIPI_LCD_720X1280 Timing parameters for Portrait orientation mode
  */
#define  MIPI_LCD_720X1280_HSYNC             ((uint16_t)1)      /* Horizontal synchronization */
#define  MIPI_LCD_720X1280_HBP               ((uint16_t)10)     /* Horizontal back porch      */
#define  MIPI_LCD_720X1280_HFP               ((uint16_t)1)     /* Horizontal front porch     */
#define  MIPI_LCD_720X1280_VSYNC             ((uint16_t)1)      /* Vertical synchronization   */
#define  MIPI_LCD_720X1280_VBP               ((uint16_t)20)      /* Vertical back porch        */
#define  MIPI_LCD_720X1280_VFP               ((uint16_t)1)      /* Vertical front porch       */

//#define  MIPI_LCD_720X1280_HSYNC             ((uint16_t)2)      /* Horizontal synchronization */
//#define  MIPI_LCD_720X1280_HBP               ((uint16_t)46)     /* Horizontal back porch      */
//#define  MIPI_LCD_720X1280_HFP               ((uint16_t)38)     /* Horizontal front porch     */
//#define  MIPI_LCD_720X1280_VSYNC             ((uint16_t)1)      /* Vertical synchronization   */
//#define  MIPI_LCD_720X1280_VBP               ((uint16_t)16)      /* Vertical back porch        */
//#define  MIPI_LCD_720X1280_VFP               ((uint16_t)12)      /* Vertical front porch       */

/**
  * @brief  MIPI_LCD_1280X720 Timing parameters for Landscape orientation mode
  *         Same values as for Portrait mode in fact.
  */
#define  MIPI_LCD_1280X720_HSYNC             MIPI_LCD_720X1280_VSYNC  /* Horizontal synchronization */
#define  MIPI_LCD_1280X720_HBP               MIPI_LCD_720X1280_VBP    /* Horizontal back porch      */
#define  MIPI_LCD_1280X720_HFP               MIPI_LCD_720X1280_VFP    /* Horizontal front porch     */
#define  MIPI_LCD_1280X720_VSYNC             MIPI_LCD_720X1280_HSYNC  /* Vertical synchronization   */
#define  MIPI_LCD_1280X720_VBP               MIPI_LCD_720X1280_HBP    /* Vertical back porch        */
#define  MIPI_LCD_1280X720_VFP               MIPI_LCD_720X1280_HFP    /* Vertical front porch       */

#define MIPI_LCD_CMD_CASET                 0x30  /* Column address set command */
//#define MIPI_LCD_CMD_PASET                 0x2B  /* Page address set command */

#define MIPI_LCD_CMD_DISPOFF               0x28  /* Display Off command */
#define MIPI_LCD_CMD_DISPON                0x29  /* Display On command */

#define MIPI_LCD_CMD_TEEON                 0x35  /* Tearing Effect Line On command : command with 1 parameter 'TELOM' */

/* CABC Management : ie : Content Adaptive Back light Control in IC */
#define MIPI_LCD_CMD_WRDISBV               0x51  /* Write Display Brightness command          */
#define MIPI_LCD_CMD_WRCTRLD               0x53  /* Write CTRL Display command                */
#define MIPI_LCD_CMD_WRCABC                0x55  /* Write Content Adaptive Brightness command */
#define MIPI_LCD_CMD_WRCABCMB              0x5E  /* Write CABC Minimum Brightness command     */

void DSI_WriteCmd(uint8_t cmd, uint8_t data);
uint8_t Eugen_MIPI_1280x720_Init(uint32_t ColorCoding, uint32_t orientation);
extern void MIPI_LCD_IO_Delay(uint32_t Delay);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __EUGEN_MIPI_LCD_1280X720_H */
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
