// Created by Glif.
#ifndef _ST7735_
#define _ST7735_
#include <stm32f4xx.h>

// Screen orientation
// 0 Vertical screen
// 1 Vertical screen rotate 180 degrees
// 2 Horizontal screen
// 3 Horizontal screen Rotate 180 degrees
#ifndef LCD_DIRECTION
#define LCD_DIRECTION 0
#endif

// Screen size
#define LCD_LONG  160
#define LCD_SHORT 128
#ifndef LCD_X
#define LCD_X (LCD_DIRECTION > 1 ? LCD_LONG : LCD_SHORT)
#endif
#ifndef LCD_Y
#define LCD_Y (LCD_DIRECTION > 1 ? LCD_SHORT : LCD_LONG)
#endif

// Common colors
#define COLOR_BLACK   0x0000
#define COLOR_NAVY    0x000F
#define COLOR_DGREEN  0x03E0
#define COLOR_DCYAN   0x03EF
#define COLOR_MAROON  0x7800
#define COLOR_DGRAY   0x7BEF
#define COLOR_BLUE    0x001F
#define COLOR_GREEN   0x07E0
#define COLOR_CYAN    0x07FF
#define COLOR_RED     0xF800
#define COLOR_PURPLE  0x780F
#define COLOR_YELLOW  0xFFE0
#define COLOR_MAGENTA 0xF81F
#define COLOR_OLIVE   0x7BE0
#define COLOR_LGRAY   0xC618
#define COLOR_WHITE   0xFFFF

typedef struct {
    SPI_TypeDef *SPIx;
    GPIO_TypeDef *CS_GPIO_Port;
    GPIO_TypeDef *DC_GPIO_Port;
    GPIO_TypeDef *RST_GPIO_Port;
    uint32_t CS_Pin;
    uint32_t DC_Pin;
    uint32_t RST_Pin;
} lcdSettings;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t u16Width;
    uint16_t u16Height;
    uint16_t u16ZoomWidth;
    uint16_t u16ZoomHeight;
    _Bool bBigEndian;
} lcdMeasurement;

void lcdInit(lcdSettings *lcdsInit);
void lcdSetForeColor(uint16_t u16ForeColor);
void lcdSetBackColor(uint16_t u16BackColor);
void lcdClear(void);
void lcdDrawPoint(uint16_t x, uint16_t y);
void lcdDrawRect(uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by, _Bool bFill);
void lcdDrawChar(uint16_t x, uint16_t y, uint8_t u8Data);
void lcdDrawStr(uint16_t x, uint16_t y, const uint8_t *pu8Data);
void lcdDrawInt8(uint16_t x, uint16_t y, int8_t i8Data);
void lcdDrawUInt8(uint16_t x, uint16_t y, uint8_t u8Data);
void lcdDrawInt16(uint16_t x, uint16_t y, int16_t i16Data);
void lcdDrawUInt16(uint16_t x, uint16_t y, uint16_t u16Data);
void lcdDrawChinese(uint16_t x, uint16_t y, const uint8_t *pu8Data, uint8_t u8Size, uint8_t u8Length);
void lcdSetImageScale(lcdMeasurement *lcdmImage);
void lcdDrawImage(const uint8_t *pu8Data);

#endif
