/*********************************************************************
*          Portions COPYRIGHT 2019 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.40 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/* Define the available number of bytes available for the GUI */

//#define GUI_NUMBYTES  (210) *  1024 

/* Define the memory table to use by STemWin */
//U32 aMemory[GUI_NUMBYTES / 4];
#define GUI_NUMBYTES  (210) *  1024 

#if defined ( __ICCARM__ ) /* !< IAR Compiler */
U32 aMemory[GUI_NUMBYTES / 4];

#elif defined ( __CC_ARM ) /* !< Keil Compiler */
U32 aMemory[GUI_NUMBYTES / 4] __attribute__((at(0x24020000)));

#elif defined ( __GNUC__ ) /* !< GNU Compiler */
U32 aMemory[GUI_NUMBYTES / 4] __attribute__((section(".GUI_NUMBYTES_section")));
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void) 
{
	GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
}
//void GUI_X_Config(void) 
//{
//  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
//}

/*************************** End of file ****************************/
