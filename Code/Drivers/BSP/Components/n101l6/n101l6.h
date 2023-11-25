/**
  ******************************************************************************
  * @file    n101l6.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   This file contains all the constants parameters for the ampire640480
  *          LCD component.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __N101L6_H
#define __N101L6_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/  

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup N101L6
  * @{
  */

/** @defgroup N101L6_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup N101L6_Exported_Constants
  * @{
  */
  
/** 
  * @brief  N101L6 Size  
  */    
#define  N101L6_WIDTH    ((uint16_t)1024)             /* LCD PIXEL WIDTH            */
#define  N101L6_HEIGHT   ((uint16_t)600)              /* LCD PIXEL HEIGHT           */

/** 
  * @brief  N101L6 Timing  
  */ 
#define  N101L6_HSYNC            ((uint16_t)1)      /* Horizontal synchronization */
#define  N101L6_HBP              ((uint16_t)46)     /* Horizontal back porch      */
#define  N101L6_HFP              ((uint16_t)210)    /* Horizontal front porch     */
#define  N101L6_VSYNC            ((uint16_t)1)      /* Vertical synchronization   */
#define  N101L6_VBP              ((uint16_t)4)     /* Vertical back porch        */
//#define  N101L6_VBP              ((uint16_t)1)     /* Vertical back porch        */
#define  N101L6_VFP              ((uint16_t)1)     /* Vertical front porch       */

/** 
  * @brief  N101L6 frequency divider  
  */    
#define  N101L6_FREQUENCY_DIVIDER     4              /* LCD Frequency divider      */
/**
  * @}
  */
  
/** @defgroup N101L6_Exported_Functions
  * @{
  */    

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __N101L6_H */
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
