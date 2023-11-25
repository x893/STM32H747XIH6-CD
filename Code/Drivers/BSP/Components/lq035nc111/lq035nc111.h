/**
  ******************************************************************************
  * @file    lq035nc111.h
  * @author  MCD Application Team
  * @brief   This file contains all the constants parameters for the lq035nc111
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
#ifndef __LQ035NC111_H
#define __LQ035NC111_H

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
  
/** @addtogroup LQ035NC111320x240
  * @{
  */

/** @defgroup LQ035NC111320x240_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup LQ035NC111320x240_Exported_Constants
  * @{
  */
  
/** 
  * @brief  LQ035NC111320x240 Size  
  */     
#define  LQ035NC111320x240_WIDTH    ((uint16_t)320)          /* LCD PIXEL WIDTH            */
#define  LQ035NC111320x240_HEIGHT   ((uint16_t)240)          /* LCD PIXEL HEIGHT           */

/** 
  * @brief  LQ035NC111320x240 Timing  
  */     
#define  LQ035NC111320x240_HSYNC            ((uint16_t)30)   /* Horizontal synchronization */
#define  LQ035NC111320x240_HBP              ((uint16_t)38)    /* Horizontal back porch      */ 
#define  LQ035NC111320x240_HFP              ((uint16_t)20)    /* Horizontal front porch     */
#define  LQ035NC111320x240_VSYNC            ((uint16_t)3)   /* Vertical synchronization   */
#define  LQ035NC111320x240_VBP              ((uint16_t)15)    /* Vertical back porch        */
#define  LQ035NC111320x240_VFP              ((uint16_t)4)    /* Vertical front porch       */

/** 
  * @brief  LQ035NC111320x240 frequency divider  
  */    
#define  LQ035NC111320x240_FREQUENCY_DIVIDER    7            /* LCD Frequency divider      */
/**
  * @}
  */
  
/** @defgroup LQ035NC111320x240_Exported_Functions
  * @{
  */    

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __LQ035NC111_H */
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
