/**
  ******************************************************************************
  * @file    ft5426_reg.h
  * @author  MCD Application Team
  * @brief   Header of ft5426_reg.c
  *          
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FT5426_REG_H
#define FT5426_REG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Macros --------------------------------------------------------------------*/

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Component
 * @{
 */

/** @addtogroup FT5426
 * @{
 */

/* Exported types ------------------------------------------------------------*/

/** @defgroup FT5426_Exported_Types FT5426 Exported Types
 * @{
 */
/************** Generic Function  *******************/

typedef int32_t (*FT5426_Write_Func)(void *, uint8_t, uint8_t*, uint16_t);
typedef int32_t (*FT5426_Read_Func) (void *, uint8_t, uint8_t*, uint16_t);

/**
 * @}
 */

/** @defgroup FT5426_Imported_Globals FT5426 Imported Globals
 * @{
 */
typedef struct
{
  FT5426_Write_Func   WriteReg;
  FT5426_Read_Func    ReadReg;
  void                 *handle;
} ft5426_ctx_t;
/**
 * @}
 */

/** @defgroup FT5426_Exported_Constants FT5426 Exported Constants
 * @{
 */

/* Current mode register of the FT5426 (R/W) */
#define FT5426_DEV_MODE_REG         0x00U

/* Gesture ID register */
#define FT5426_GEST_ID_REG          0x01U

/* Touch Data Status register : gives number of active touch points (0..2) */
#define FT5426_TD_STAT_REG          0x02U

/* P1 X, Y coordinates, weight and misc registers */
#define FT5426_P1_XH_REG            0x03U
#define FT5426_P1_XL_REG            0x04U
#define FT5426_P1_YH_REG            0x05U
#define FT5426_P1_YL_REG            0x06U
#define FT5426_P1_WEIGHT_REG        0x07U
#define FT5426_P1_MISC_REG          0x08U

/* P2 X, Y coordinates, weight and misc registers */
#define FT5426_P2_XH_REG            0x09U
#define FT5426_P2_XL_REG            0x0AU
#define FT5426_P2_YH_REG            0x0BU
#define FT5426_P2_YL_REG            0x0CU
#define FT5426_P2_WEIGHT_REG        0x0DU
#define FT5426_P2_MISC_REG          0x0EU

/* Threshold for touch detection */
#define FT5426_TH_GROUP_REG         0x80U

/* Filter function coefficients */
#define FT5426_TH_DIFF_REG          0x85U		//Non-existent

/* Control register */
#define FT5426_CTRL_REG             0x86U

/* The time period of switching from Active mode to Monitor mode when there is no touching */
#define FT5426_TIMEENTERMONITOR_REG 0x87U

/* Report rate in Active mode */
#define FT5426_PERIODACTIVE_REG     0x88U

/* Report rate in Monitor mode */
#define FT5426_PERIODMONITOR_REG    0x89U

/* The value of the minimum allowed angle while Rotating gesture mode */
#define FT5426_RADIAN_VALUE_REG     0x91U		//Non-existent

/* Maximum offset while Moving Left and Moving Right gesture */
#define FT5426_OFFSET_LR_REG        0x92U		//Non-existent

/* Maximum offset while Moving Up and Moving Down gesture */
#define FT5426_OFFSET_UD_REG        0x93U		//Non-existent

/* Minimum distance while Moving Left and Moving Right gesture */
#define FT5426_DISTANCE_LR_REG      0x94U		//Non-existent

/* Minimum distance while Moving Up and Moving Down gesture */
#define FT5426_DISTANCE_UD_REG      0x95U		//Non-existent

/* Maximum distance while Zoom In and Zoom Out gesture */
#define FT5426_DISTANCE_ZOOM_REG    0x96U		//Non-existent

/* High 8-bit of LIB Version info */
#define FT5426_LIB_VER_H_REG        0xA1U

/* Low 8-bit of LIB Version info */
#define FT5426_LIB_VER_L_REG        0xA2U

/* Chip Selecting */
#define FT5426_CIPHER_REG           0xA3U

/* Interrupt mode register (used when in interrupt mode) */
#define FT5426_GMODE_REG            0xA4U

/* Current power mode the FT5426 system is in (R) */
#define FT5426_PWR_MODE_REG         0xA5U

/* FT5426 firmware version */
#define FT5426_FIRMID_REG           0xA6U

/* FT5426 Chip identification register */
/* Chip vendor ID */
#define FT5426_CHIP_ID_REG          0xA3U		//0xA8U

/* Release code version */
#define FT5426_RELEASE_CODE_ID_REG  0xAFU		//Non-existent

/* Current operating mode the FT5426 system is in (R) */
#define FT5426_STATE_REG            0xBCU		//Non-existent

/**
 * @}
 */

/*******************************************************************************
* Register      : Generic - All
* Address       : Generic - All
* Bit Group Name: None
* Permission    : W
*******************************************************************************/
int32_t ft5426_write_reg(ft5426_ctx_t *ctx, uint8_t reg, uint8_t *pbuf, uint16_t length);
int32_t ft5426_read_reg(ft5426_ctx_t *ctx, uint8_t reg, uint8_t *pbuf, uint16_t length);

/**************** Base Function  *******************/

/*******************************************************************************
* Register      : DEV_MODE
* Address       : 0X00
* Bit Group Name: DEVICE_MODE
* Permission    : RW
*******************************************************************************/
#define   FT5426_DEV_MODE_BIT_MASK        0x70U
#define   FT5426_DEV_MODE_BIT_POSITION    4U
int32_t  ft5426_dev_mode_w(ft5426_ctx_t *ctx, uint8_t value);
int32_t  ft5426_dev_mode_r(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : GEST_ID
* Address       : 0X01
* Bit Group Name: Gesture ID
* Permission    : R
*******************************************************************************/
#define   FT5426_GEST_ID_BIT_MASK        0xFFU
#define   FT5426_GEST_ID_BIT_POSITION    0U
int32_t ft5426_gest_id(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : TD_STATUS
* Address       : 0X02
* Bit Group Name: 
* Permission    : R
*******************************************************************************/
#define   FT5426_TD_STATUS_BIT_MASK        0x0FU
#define   FT5426_TD_STATUS_BIT_POSITION    0U
int32_t ft5426_td_status(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_XH
* Address       : 0X03
* Bit Group Name: First Event Flag
* Permission    : R
* Default value : 0xF0U
*******************************************************************************/
#define   FT5426_P1_XH_EF_BIT_MASK        0xC0U
#define   FT5426_P1_XH_EF_BIT_POSITION    6U
int32_t ft5426_p1_xh_ef(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_XH
* Address       : 0X03
* Bit Group Name: First Touch X Position
* Permission    : R
* Default value : 0x0FU
*******************************************************************************/
#define   FT5426_P1_XH_TP_BIT_MASK        0x0FU
#define   FT5426_P1_XH_TP_BIT_POSITION    0U
int32_t ft5426_p1_xh_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_XL
* Address       : 0X04
* Bit Group Name: First Touch X Position
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P1_XL_TP_BIT_MASK        0xFFU
#define   FT5426_P1_XL_TP_BIT_POSITION    0U
int32_t ft5426_p1_xl_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_YH
* Address       : 0X05
* Bit Group Name: First Touch ID
* Permission    : R
* Default value : 0xF0U
*******************************************************************************/
#define   FT5426_P1_YH_TID_BIT_MASK        0xF0U
#define   FT5426_P1_YH_TID_BIT_POSITION    4U
int32_t ft5426_p1_yh_tid(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_YH
* Address       : 0x06
* Bit Group Name: First Touch Y Position
* Permission    : R
* Default value : 0x0FU
*******************************************************************************/
#define   FT5426_P1_YH_TP_BIT_MASK        0x0FU
#define   FT5426_P1_YH_TP_BIT_POSITION    0U
int32_t ft5426_p1_yh_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_YL
* Address       : 0X06
* Bit Group Name: First Touch Y Position
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P1_YL_TP_BIT_MASK        0xFFU
#define   FT5426_P1_YL_TP_BIT_POSITION    0U
int32_t ft5426_p1_yl_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_WEIGHT
* Address       : 0X07
* Bit Group Name: First Touch Weight(pressure)
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P1_WEIGHT_BIT_MASK        0xFFU
#define   FT5426_P1_WEIGHT_BIT_POSITION    0U
int32_t ft5426_p1_weight(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P1_MISC
* Address       : 0X08
* Bit Group Name: First Touch Area
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P1_MISC_BIT_MASK        0xF0U
#define   FT5426_P1_MISC_BIT_POSITION    4U
int32_t ft5426_p1_misc(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_XH
* Address       : 0X09
* Bit Group Name: Second Event Flag
* Permission    : R
* Default value : 0xF0U
*******************************************************************************/
#define   FT5426_P2_XH_EF_BIT_MASK        0xC0U
#define   FT5426_P2_XH_EF_BIT_POSITION    6U
int32_t ft5426_p2_xh_ef(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_XH
* Address       : 0X09
* Bit Group Name: Second Touch X Position
* Permission    : R
* Default value : 0x0FU
*******************************************************************************/
#define   FT5426_P2_XH_TP_BIT_MASK        0x0FU
#define   FT5426_P2_XH_TP_BIT_POSITION    0U
int32_t ft5426_p2_xh_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_XL
* Address       : 0X0A
* Bit Group Name: Second Touch X Position
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P2_XL_TP_BIT_MASK        0xFFU
#define   FT5426_P2_XL_TP_BIT_POSITION    0U
int32_t ft5426_p2_xl_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_YH
* Address       : 0X0B
* Bit Group Name: Second Touch ID
* Permission    : R
* Default value : 0xF0U
*******************************************************************************/
#define   FT5426_P2_YH_TID_BIT_MASK        0xF0U
#define   FT5426_P2_YH_TID_BIT_POSITION    4U
int32_t ft5426_p2_yh_tid(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_YH
* Address       : 0x0B
* Bit Group Name: Second Touch Y Position
* Permission    : R
* Default value : 0x0FU
*******************************************************************************/
#define   FT5426_P2_YH_TP_BIT_MASK        0x0FU
#define   FT5426_P2_YH_TP_BIT_POSITION    0U
int32_t ft5426_p2_yh_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_YL
* Address       : 0X0C
* Bit Group Name: Second Touch Y Position
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P2_YL_TP_BIT_MASK        0xFFU
#define   FT5426_P2_YL_TP_BIT_POSITION    0U
int32_t ft5426_p2_yl_tp(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_WEIGHT
* Address       : 0X0D
* Bit Group Name: Second Touch Weight(pressure)
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P2_WEIGHT_BIT_MASK        0xFFU
#define   FT5426_P2_WEIGHT_BIT_POSITION    0U
int32_t ft5426_p2_weight(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : P2_MISC
* Address       : 0X0E
* Bit Group Name: Second Touch Area
* Permission    : R
* Default value : 0xFFU
*******************************************************************************/
#define   FT5426_P2_MISC_BIT_MASK        0xF0U
#define   FT5426_P2_MISC_BIT_POSITION    4U
int32_t ft5426_p2_misc(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : TH_GROUP
* Address       : 0X80
* Bit Group Name: Threshold for touch detection
* Permission    : RW
* Default value : None
*******************************************************************************/
#define   FT5426_TH_GROUP_BIT_MASK        0xFFU
#define   FT5426_TH_GROUP_BIT_POSITION    0U
int32_t ft5426_th_group(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : TH_DIFF
* Address       : 0X85
* Bit Group Name: Filter function coefficient
* Permission    : RW
* Default value : None
*******************************************************************************/
#define   FT5426_TH_DIFF_BIT_MASK        0xFFU
#define   FT5426_TH_DIFF_BIT_POSITION    0U
int32_t ft5426_th_diff(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : CTRL
* Address       : 0X86
* Bit Group Name: 
* Permission    : RW
* Default value : 0x01
*******************************************************************************/
#define   FT5426_CTRL_BIT_MASK           0xFFU
#define   FT5426_CTRL_BIT_POSITION       0U
int32_t ft5426_ctrl(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : TIMEENTERMONITOR
* Address       : 0X87
* Bit Group Name: 
* Permission    : RW
* Default value : 0x0A
*******************************************************************************/
#define   FT5426_TIMEENTERMONITOR_BIT_MASK           0xFFU
#define   FT5426_TIMEENTERMONITOR_BIT_POSITION       0U
int32_t ft5426_time_enter_monitor(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : PERIODACTIVE
* Address       : 0X88
* Bit Group Name: 
* Permission    : RW
* Default value : None
*******************************************************************************/
#define   FT5426_PERIODACTIVE_BIT_MASK           0xFFU
#define   FT5426_PERIODACTIVE_BIT_POSITION       0U
int32_t ft5426_period_active(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : PERIODMONITOR
* Address       : 0X89
* Bit Group Name: 
* Permission    : RW
* Default value : 0x28
*******************************************************************************/
#define   FT5426_PERIODMONITOR_BIT_MASK           0xFFU
#define   FT5426_PERIODMONITOR_BIT_POSITION       0U
int32_t ft5426_period_monitor(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : RADIAN_VALUE
* Address       : 0X91
* Bit Group Name: 
* Permission    : RW
* Default value : 0x0A
*******************************************************************************/
#define   FT5426_RADIAN_VALUE_BIT_MASK           0xFFU
#define   FT5426_RADIAN_VALUE_BIT_POSITION       0U
int32_t ft5426_radian_value(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : OFFSET_LEFT_RIGHT
* Address       : 0X92
* Bit Group Name: 
* Permission    : RW
* Default value : 0x19
*******************************************************************************/
#define   FT5426_OFFSET_LR_BIT_MASK           0xFFU
#define   FT5426_OFFSET_LR_BIT_POSITION       0U
int32_t ft5426_offset_left_right(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : OFFSET_UP_DOWN
* Address       : 0X93
* Bit Group Name: 
* Permission    : RW
* Default value : 0x19
*******************************************************************************/
#define   FT5426_OFFSET_UD_BIT_MASK           0xFFU
#define   FT5426_OFFSET_UD_BIT_POSITION       0U
int32_t ft5426_offset_up_down(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : DISTANCE_LEFT_RIGHT
* Address       : 0X94
* Bit Group Name: 
* Permission    : RW
* Default value : 0x19
*******************************************************************************/
#define   FT5426_DISTANCE_LR_BIT_MASK           0xFFU
#define   FT5426_DISTANCE_LR_BIT_POSITION       0U
int32_t  ft5426_disatnce_left_right(ft5426_ctx_t *ctx, uint8_t value);
        
/*******************************************************************************
* Register      : DISTANCE_UP_DOWN
* Address       : 0X95
* Bit Group Name: 
* Permission    : RW
* Default value : 0x19
*******************************************************************************/
#define   FT5426_DISTANCE_UD_BIT_MASK           0xFFU
#define   FT5426_DISTANCE_UD_BIT_POSITION       0U
int32_t ft5426_distance_up_down(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : DISTANCE_ZOOM
* Address       : 0X96
* Bit Group Name: 
* Permission    : RW
* Default value : 0x32
*******************************************************************************/
#define   FT5426_DISTANCE_ZOOM_BIT_MASK           0xFFU
#define   FT5426_DISTANCE_ZOOM_BIT_POSITION       0U
int32_t ft5426_distance_zoom(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : LIB_VER_H
* Address       : 0XA1
* Bit Group Name: 
* Permission    : R
* Default value : None
*******************************************************************************/
#define   FT5426_LIB_VER_H_BIT_MASK           0xFFU
#define   FT5426_LIB_VER_H_BIT_POSITION       0U
int32_t ft5426_lib_ver_high(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : LIB_VER_L
* Address       : 0XA2
* Bit Group Name: 
* Permission    : R
* Default value : None
*******************************************************************************/
#define   FT5426_LIB_VER_L_BIT_MASK           0xFFU
#define   FT5426_LIB_VER_L_BIT_POSITION       0U
int32_t ft5426_lib_ver_low(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : CIPHER
* Address       : 0XA3
* Bit Group Name: 
* Permission    : R
* Default value : 0x06
*******************************************************************************/
#define   FT5426_CIPHER_BIT_MASK           0xFFU
#define   FT5426_CIPHER_BIT_POSITION       0U
int32_t ft5426_cipher(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : G_MODE
* Address       : 0XA4
* Bit Group Name: 
* Permission    : RW
* Default value : 0x01
*******************************************************************************/
#define   FT5426_G_MODE_BIT_MASK           0xFFU
#define   FT5426_G_MODE_BIT_POSITION       0U
int32_t ft5426_g_mode(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : PWR_MODE
* Address       : 0XA5
* Bit Group Name: 
* Permission    : RW
* Default value : 0x00
*******************************************************************************/
#define   FT5426_PWR_MODE_BIT_MASK           0xFFU
#define   FT5426_PWR_MODE_BIT_POSITION       0U
int32_t ft5426_pwr_mode(ft5426_ctx_t *ctx, uint8_t value);

/*******************************************************************************
* Register      : FIRMID
* Address       : 0XA6
* Bit Group Name: 
* Permission    : R
* Default value : None
*******************************************************************************/
#define   FT5426_FIRMID_BIT_MASK           0xFFU
#define   FT5426_FIRMID_BIT_POSITION       0U
int32_t ft5426_firm_id(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : FOCALTECH_ID
* Address       : 0XA8
* Bit Group Name: 
* Permission    : R
* Default value : 0x11
*******************************************************************************/
#define   FT5426_CHIP_ID_BIT_MASK           0xFFU
#define   FT5426_CHIP_ID_BIT_POSITION       0U
int32_t ft5426_chip_id(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : RELEASE_CODE_ID
* Address       : 0XAF
* Bit Group Name: 
* Permission    : R
* Default value : 0x001
*******************************************************************************/
#define   FT5426_RC_ID_BIT_MASK           0xFFU
#define   FT5426_RC_ID_BIT_POSITION       0U
int32_t ft5426_release_code_id(ft5426_ctx_t *ctx, uint8_t *value);

/*******************************************************************************
* Register      : STATE
* Address       : 0XBC
* Bit Group Name: 
* Permission    : RW
* Default value : 0x01
*******************************************************************************/
#define   FT5426_STATE_BIT_MASK           0xFFU
#define   FT5426_STATE_BIT_POSITION       0U
int32_t ft5426_state(ft5426_ctx_t *ctx, uint8_t value);

  /**
   * @}
   */

#ifdef __cplusplus
}
#endif
#endif /* FT5426_REG_H */


/**
 * @}
 */

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
