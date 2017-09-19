/**
  ******************************************************************************
  * @file    menu.c
  * @author  Alessio Caligiuri
  * @version V0.0.1
  * @date    22 June 2017
  * @brief   This code is used to build a menu, composed by menu entries.
  * 		 This code is intended for use with a rotary encoder with button
  * 		 and a HD44780 LCD display.
  * 		 Each menu entry can perform a specific task when the button is
  * 		 pressed. This task is specified by the field "onPressed", that is
  * 		 a function pointer.
  ******************************************************************************
  * @attention
  *

  *
  ******************************************************************************
  */

#include "menu.h"
#include "HD44780.h"

/**
 * @brief	Used in "Modify Parameter" mode to temporary hold the value
 * 			inserted by the user.
 */
uint8_t tempParam;

/**
 * @brief True if a parameter has been modified without saving.
 */
int menuParamIsModified = 0;

/**
 * @brief	Menu initialization.
 * 			Set the first menu entry and the menu state as "navigation".
 * 			Initialize the LCD display.
 * @param	initialMenuEntry	Pointer to the initial menu entry.
 */
void Menu_Init(MenuEntryStruct *initialMenuEntry)
{
	//LCD_Init();
	//LCD_Backlight_On();
	Menu_currentEntry = initialMenuEntry;
	menuState = NavigationUpdate;
}

/**
 * @brief	Fill all entry fields with 0.
 */
void Menu_FillEntryWithZeros(MenuEntryStruct *menuEntryStruct)
{
	menuEntryStruct->isModified = 0;
	menuEntryStruct->lowerLevelEntry = 0;
	menuEntryStruct->name[0] = 0;
	menuEntryStruct->surname[0] = 0;
	menuEntryStruct->nextEntry = 0;
	menuEntryStruct->onPression = 0;
	menuEntryStruct->param = 0;
	menuEntryStruct->previousEntry = 0;
	menuEntryStruct->upperLevelEntry = 0;
}

/**
 * @brief	Go to lower level menu entry.
 */
void Menu_GoLowerLevel(void)
{
	Menu_SwitchEntry(Menu_currentEntry->lowerLevelEntry);
}

/**
 * @brief	Go to upper level menu entry.
 */
void Menu_GoUpperLevel(void)
{
	Menu_SwitchEntry(Menu_currentEntry->upperLevelEntry);
}

/**
 * @brief 	Go to previous menu entry in this level.
 */
void Menu_GoPreviousEntry(void)
{
	Menu_SwitchEntry(Menu_currentEntry->previousEntry);
}

/**
 * @brief	Go to next menu entry in this level.
 */
void Menu_GoNextEntry(void)
{
	Menu_SwitchEntry(Menu_currentEntry->nextEntry);
}

/**
 * @brief	To call when the button is pressed.
 * 			In "Navigation" mode, perform the menu entry task.
 * 			In "Modify Parameter" mode, save the new value and set
 * 			"isModified" flag for this entry and the entire menu, then set
 * 			the menu mode as "Navigation".
 */
void Menu_OnPression(void)
{
	switch(menuState)
	{
	case Navigation:
		if(Menu_currentEntry->onPression) //if the pointer is not null
			Menu_currentEntry->onPression();
		break;
	case ParamModify:
	case ParamModified:
		if(tempParam != *((uint8_t*)(Menu_currentEntry->param)))
		{
			Menu_currentEntry->isModified = 1; //this menu entry parameter has been modified
			menuParamIsModified = 1;		//some menu entry parameter has been modified
			*((uint8_t*)(Menu_currentEntry->param)) = tempParam; //store the new value into the variable
		}
		menuState = NavigationUpdate;
		LCD_CursorMode(Invisible_Cursor);
		break;
	default:
		break;
	}
}

/**
 * @brief	Used as "onPression" when the menu entry can modify a parameter.
 * 			Set the menu mode as "Parameter Modify", then load the current
 * 			parameter value into a temporary variable.
 */
void Menu_ModifyParam(void)
{
	if(Menu_currentEntry->param) //check if pointer is not equal to 0
	{
		menuState = ParamModify;
		tempParam = *((uint8_t*)(Menu_currentEntry->param));
	}

}

/**
 * @brief	Show the current menu entry.
 * 			Clear the display and print name and, if necessary, parameter.
 * 			If the parameter has been modified, shows an '*' after the name.
 */
void Menu_Show(void)
{
	switch(menuState)
	{

	case NavigationUpdate: /* A new entry has to be shown */
		LCD_Clear_Display();
		LCD_printf(Menu_currentEntry->name);
		LCD_2ndRow();
		LCD_printf(Menu_currentEntry->surname);

		if(Menu_currentEntry->isModified)
		{
			LCD_printchar('*');
		}
		menuState = Navigation;
		break;

	case ParamModify: /* If the param is going to be modified */
		LCD_2ndRow();
		LCD_printf("                ");
		LCD_2ndRow();
		LCD_printf("Value: %d  ",tempParam);
		LCD_Locate(2, 8);
		LCD_CursorMode(BlinkingBlock_Cursor);
		break;

	/* If the param has been modified, re-print only the parameter value */
	case ParamModified:
		LCD_Locate(2, 8);
		LCD_printf("%d  ",tempParam);
		LCD_CMD(0x10); //shift cursor left
		LCD_CMD(0x10); //shift cursor left
		LCD_CMD(0x10); //shift cursor left
		break;

	case Navigation:
		break;
	default:
		break;
	}
}

/**
 * @brief	Update menu on clockwise rotation.
 */
void Menu_OnRotationCW(void)
{
	/**
	 * If the menu state is "navigation", then switch to the next menu entry.
	 * If the menu state is "parameter modify", then increment the temporary
	 * variable for parameter.
	 */
	switch(menuState)
	{
	case Navigation:
		Menu_GoNextEntry();
		break;
	case ParamModify:
		if(tempParam < 255)
		{
			tempParam++;
		}
		menuState = ParamModified;
		break;
	case ParamModified:
		if(tempParam < 255)
		{
			tempParam++;
		}
		break;
	default:
		break;
	}
}

/**
 * @brief	Update menu on counter-clockwise rotation.
 */
void Menu_OnRotationCCW(void)
{
	/**
	 * If the menu state is "navigation", then switch to the next menu entry.
	 * If the menu state is "parameter modify", then decrement the temporary
	 * variable for parameter.
	 */
	switch(menuState)
	{
	case Navigation:
		Menu_GoPreviousEntry();
		break;
	case ParamModify:
		if(tempParam > 0)
		{
			tempParam--;
		}
		menuState = ParamModified;
		break;
	case ParamModified:
		if(tempParam > 0)
		{
			tempParam--;
		}
		break;
	default:
		break;
	}
}

/**
 * @brief	Switch the current menu entry.
 *
 * 			Check if the passed pointer is not 0 and if the new entry is
 * 			different from the current; if yes, update the current entry
 * 			pointer with the new entry pointer and update the menu state
 * 			as "Navigation Update".
 *
 * 	@param	Menu_newEntry	Pointer to the new entry.
 *
 */
void Menu_SwitchEntry(MenuEntryStruct* Menu_newEntry)
{
	if(Menu_newEntry) //fails if Menu_newEntry points to 0
	{
//		if(Menu_currentEntry == Menu_newEntry)
//			return;
		Menu_currentEntry = Menu_newEntry;
		menuState = NavigationUpdate;
	}
}
