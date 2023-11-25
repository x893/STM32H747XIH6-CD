/**
  ******************************************************************************
  * @file    lcd1024600.h
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
#ifndef __LCD1024600_H
#define __LCD1024600_H

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
  
/** @addtogroup LCD1024600
  * @{
  */

/** @defgroup LCD1024600_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup LCD1024600_Exported_Constants
  * @{
  */
  
/** 
  * @brief  LCD1024600 Size  
  */    
#define  LCD1024600_WIDTH    ((uint16_t)1024)             /* LCD PIXEL WIDTH            */
#define  LCD1024600_HEIGHT   ((uint16_t)600)              /* LCD PIXEL HEIGHT           */

/** 
  * @brief  LCD1024600 Timing  
  */ 
#define  LCD1024600_HSYNC            ((uint16_t)2)      /* Horizontal synchronization */
#define  LCD1024600_HBP              ((uint16_t)162)     /* Horizontal back porch      */
#define  LCD1024600_HFP              ((uint16_t)210)    /* Horizontal front porch     */
#define  LCD1024600_VSYNC            ((uint16_t)2)      /* Vertical synchronization   */
#define  LCD1024600_VBP              ((uint16_t)25)     /* Vertical back porch        */
#define  LCD1024600_VFP              ((uint16_t)1)     /* Vertical front porch       */

//#define  LCD1024600_HSYNC            ((uint16_t)1)      /* Horizontal synchronization */
//#define  LCD1024600_HBP              ((uint16_t)162)     /* Horizontal back porch      */
//#define  LCD1024600_HFP              ((uint16_t)1)    /* Horizontal front porch     */
//#define  LCD1024600_VSYNC            ((uint16_t)1)      /* Vertical synchronization   */
//#define  LCD1024600_VBP              ((uint16_t)25)     /* Vertical back porch        */
//#define  LCD1024600_VFP              ((uint16_t)1)     /* Vertical front porch       */

//#define  LCD1024600_HSYNC            ((uint16_t)20)      /* Horizontal synchronization */
//#define  LCD1024600_HBP              ((uint16_t)140)     /* Horizontal back porch      */
//#define  LCD1024600_HFP              ((uint16_t)160)    /* Horizontal front porch     */
//#define  LCD1024600_VSYNC            ((uint16_t)3)      /* Vertical synchronization   */
//#define  LCD1024600_VBP              ((uint16_t)20)     /* Vertical back porch        */
//#define  LCD1024600_VFP              ((uint16_t)12)     /* Vertical front porch       */

/** 
  * @brief  LCD1024600 frequency divider  
  */    
#define  LCD1024600_FREQUENCY_DIVIDER     4              /* LCD Frequency divider      */
/**
  * @}
  */
  
/** @defgroup LCD1024600_Exported_Functions
  * @{
  */    

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __LCD1024600_H */
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
