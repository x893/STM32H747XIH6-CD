/**
  ******************************************************************************
  * @file    jdata_conf_template.h 
  * @author  MCD Application Team
  * @brief   Header for jdata_conf_template.c module.
  *          This file should be copied to the application folder and modified 
  *          as follows:
  *            - Rename it to 'jdata_conf.c'.
  *            - Implement read/write functions (example of implementation is 
  *              provided based on FatFs)
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

/* Includes ------------------------------------------------------------------*/
#include "ff.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
  Example of implementation based on FatFS
  
  If JFREAD/JFWRITE prototypes are complient with the standard FatFS APIs (f_read/f_write) 
  only APIs re-definition is needed and no need to wrapper APIs defined in "jdata_conf_template.c" file

   #define JFREAD  f_read
   #define JFWRITE f_write

*/


#define JFILE            FIL

#define JMALLOC   malloc    
#define JFREE     free  

size_t read_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf);
size_t write_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf) ;

#define JFREAD(file,buf,sizeofbuf)  \
   read_file (file,buf,sizeofbuf)

#define JFWRITE(file,buf,sizeofbuf)  \
   write_file (file,buf,sizeofbuf)

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
