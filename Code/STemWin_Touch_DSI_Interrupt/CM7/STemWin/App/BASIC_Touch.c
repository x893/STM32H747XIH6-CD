/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : BASIC_HelloWorld.c
Purpose     : Simple demo drawing "Hello world"
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
	
	GUI_PID_STATE State;
	GUI_SetBkColor(GUI_RED);
	GUI_Clear();
	GUI_Delay(1000);
	
	GUI_SetBkColor(GUI_GREEN);
	GUI_Clear();
	GUI_Delay(1000);
	
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_Delay(1000);
	
	
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_SetColor(GUI_BLACK);
	
	GUI_CURSOR_Show();
  while(1)
	{
		GUI_TOUCH_GetState(&State);
    if (State.Pressed == 1) {
      GUI_FillCircle(State.x, State.y, 3);
			GUI_Exec();
    }
		GUI_Delay(10);
	}
}

/*************************** End of file ****************************/
