/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/CM7/Src/file_operations.c
  * @author  MCD Application Team
  * @brief   Write/read file on the disk.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
FATFS USBH_fatfs;
FIL MyFile;
FRESULT res;
uint32_t bytesWritten;
uint8_t rtext[200];
uint8_t wtext[] = "USB Host Library : Mass Storage application";

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Files operations: Read/Write and compare
  * @param  None
  * @retval None
  */
void MSC_File_Operations(void)
{
  uint16_t bytesread;

  LCD_UsrLog("INFO : FatFs Initialized \n");

  if (f_open(&MyFile, "0:USBHost.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    LCD_ErrLog("Cannot Open 'USBHost.txt' file \n");
  }
  else
  {
    LCD_UsrLog("INFO : 'USBHost.txt' opened for write  \n");
    res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&bytesWritten);
    f_close(&MyFile);

    if ((bytesWritten == 0) || (res != FR_OK))  /* EOF or Error */
    {
      LCD_ErrLog("Cannot Write on the  'USBHost.txt' file \n");
    }
    else
    {
      if (f_open(&MyFile, "0:USBHost.txt", FA_READ) != FR_OK)
      {
        LCD_ErrLog("Cannot Open 'USBHost.txt' file for read.\n");
      }
      else
      {
        LCD_UsrLog("INFO : Text written on the 'USBHost.txt' file \n");

        res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);

        if ((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
        {
          LCD_ErrLog("Cannot Read from the  'USBHost.txt' file \n");
        }
        else
        {
          LCD_UsrLog("Read Text : \n");
          LCD_DbgLog((char *)rtext);
          LCD_DbgLog("\n");
        }
        f_close(&MyFile);
      }
      /* Compare read data with the expected data */
      if ((bytesread == bytesWritten))
      {
        LCD_UsrLog("INFO : FatFs data compare SUCCES");
        LCD_UsrLog("\n");
      }
      else
      {
        LCD_ErrLog("FatFs data compare ERROR");
        LCD_ErrLog("\n");
      }
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
