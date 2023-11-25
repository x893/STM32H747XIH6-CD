/**
  ******************************************************************************
  * @file    LwIP/LwIP_UDPTCP_Echo_Server_Netconn_RTOS/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval.h"
#include "common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_LCD

/*Static IP ADDRESS*/
#define IP_ADDR0   ((uint8_t)192U)
#define IP_ADDR1   ((uint8_t)168U)
#define IP_ADDR2   ((uint8_t)1U)
#define IP_ADDR3   ((uint8_t)108U)
   
/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t)255U)
#define NETMASK_ADDR1   ((uint8_t)255U)
#define NETMASK_ADDR2   ((uint8_t)255U)
#define NETMASK_ADDR3   ((uint8_t)0U)

/*Gateway Address*/
#define GW_ADDR0   ((uint8_t)192U)
#define GW_ADDR1   ((uint8_t)168U)
#define GW_ADDR2   ((uint8_t)1U)
#define GW_ADDR3   ((uint8_t)1U)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
