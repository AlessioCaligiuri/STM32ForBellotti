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
#include "DMX.h"

/**
 * @brief	Max time in ms between encoder rotations to turn on "turbo rotation" mode.
 */
#define MAX_ELAPSED_SYSTICK_ROTATION	50

/**
 * @brief	Used in "Modify Parameter" mode to temporary hold the value
 * 			inserted by the user.
 */
int tempParam;

/**
 * @brief True if a parameter has been modified without saving.
 */
uint8_t menuParamIsModified = 0;

/**
 * @brief	Used in DMX Check on LCD to determine which channels have to be shown.
 * 			This variable specifies the first channel shown at left.
 */
uint16_t dmxCheckOnLCD_Ch_1stColumn = 1;

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
	case ParamModify_1_512:
	case ParamModified_1_512:
		if(tempParam != *((uint16_t*)(Menu_currentEntry->param)))
				{
					Menu_currentEntry->isModified = 1; //this menu entry parameter has been modified
					menuParamIsModified = 1;		//some menu entry parameter has been modified
					*((uint16_t*)(Menu_currentEntry->param)) = tempParam; //store the new value into the variable
				}
				menuState = NavigationUpdate;
				LCD_CursorMode(Invisible_Cursor);
				break;

	case ParamModify_0_1:
	case ParamModified_0_1:
		if(tempParam != *((uint8_t*)(Menu_currentEntry->param)))
		{
			Menu_currentEntry->isModified = 1; //this menu entry parameter has been modified
			menuParamIsModified = 1;		//some menu entry parameter has been modified
			*((uint8_t*)(Menu_currentEntry->param)) = tempParam; //store the new value into the variable
		}
		menuState = NavigationUpdate;
		LCD_CursorMode(Invisible_Cursor);
		break;

	case ParamModify_LightMode:
	case ParamModified_LightMode:
	case ParamNotModified_LightMode:
		if(tempParam != *((LightMode_t*)((Menu_currentEntry->param))))
		{
			Menu_currentEntry->isModified = 1; //this menu entry parameter has been modified
			menuParamIsModified = 1;		//some menu entry parameter has been modified
			*((LightMode_t*)(Menu_currentEntry->param)) = (LightMode_t)tempParam; //store the new value into the variable
		}
		menuState = NavigationUpdate;
		LCD_CursorMode(Invisible_Cursor);
		break;

	case ParamModify_0_255:
	case ParamModified_0_255:
		if(tempParam != *((uint8_t*)((Menu_currentEntry->param))))
		{
			Menu_currentEntry->isModified = 1; //this menu entry parameter has been modified
			menuParamIsModified = 1;		//some menu entry parameter has been modified
			*((uint8_t*)((Menu_currentEntry->param))) = (uint8_t)tempParam; //store the new value into the variable
		}
		menuState = NavigationUpdate;
		LCD_CursorMode(Invisible_Cursor);
		break;

	case DMXCheckOnLCD_Entered:
	case DMXCheckOnLCD_Update:
	case DMXCheckOnLCD_SwitchedChannels:
		menuState = NavigationUpdate;
		break;

	case ConfirmRequested:
	case ConfirmRequestedModified:
		if(tempParam)
		{
			LCD_2ndRow();
			LCD_printf("                ");
			LCD_2ndRow();
			LCD_printf("  executing...  ");
			((functionPtr)(Menu_currentEntry->param))();
		}
		menuState = NavigationUpdate;
		LCD_CursorMode(Invisible_Cursor);
		break;

	default:
		break;
	}
}

/**
 * @brief	Used as "onPression" when the menu entry can modify a parameter
 * 			with 1 - 512 bounds.
 * 			Set the menu mode as "Parameter Modify", then load the current
 * 			parameter value into a temporary variable.
 */
void Menu_ModifyParam_1_512(void)
{
	if(Menu_currentEntry->param) //check if pointer is not equal to 0
	{
		menuState = ParamModify_1_512;
		tempParam = *((uint16_t*)(Menu_currentEntry->param));
	}
}

/**
 * @brief	Used as "onPression" when the menu entry can modify a parameter
 * 			with 0 - 255 bounds (uint8_t).
 * 			Set the menu mode as "Parameter Modify", then load the current
 * 			parameter value into a temporary variable.
 */
void Menu_ModifyParam_0_255(void)
{
	if(Menu_currentEntry->param) //check if pointer is not equal to 0
	{
		menuState = ParamModify_0_255;
		tempParam = *((uint8_t*)((Menu_currentEntry->param)));
	}
}

/**
 * @brief	Used as "onPression" when the menu entry can modify a parameter
 * 			with boolean value.
 * 			Set the menu mode as "Parameter Modify", then load the current
 * 			parameter value into a temporary variable.
 */
void Menu_ModifyParam_0_1(void)
{
	if(Menu_currentEntry->param) //check if pointer is not equal to 0
	{
		menuState = ParamModify_0_1;
		tempParam = *((uint8_t*)(Menu_currentEntry->param));
	}
}

/**
 * @brief	Used as "onPression" when the menu entry can modify the parameter
 * 			Light mode.
 * 			Set the menu mode as "Parameter Modify", then load the current
 * 			parameter value into a temporary variable.
 */
void Menu_ModifyParam_LightMode(void)
{
	if(Menu_currentEntry->param) //check if pointer is not equal to 0
	{
		menuState = ParamModify_LightMode;
		tempParam = *((LightMode_t*)((Menu_currentEntry->param)));
	}
}

/**
 * @brief	Show the current menu entry.
 * 			Clear the display and print name and, if necessary, parameter.
 * 			If the parameter has been modified, shows an '*' after the name.
 */
void Menu_Show(void)
{
	/********************** Switch for menu state **********************/
	switch(menuState)
	{
	/******** Switch for menu state: case Navigation Update ************/
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
	/*********** Switch for menu state: case Param Mod. ***************/
	case ParamModify_0_255:
	case ParamModify_1_512: /* If the param is going to be modified */
		LCD_2ndRow();
		LCD_printf("                ");
		LCD_2ndRow();
		LCD_printf("Value: %d  ",tempParam);
		//LCD_Locate(2, 8);
		LCD_CMD(0x10); //shift cursor left
		LCD_CMD(0x10); //shift cursor left
		LCD_CMD(0x10); //shift cursor left
		LCD_CursorMode(BlinkingBlock_Cursor);
		break;

		/*********** Switch for menu state: case Param Mod. ***************/
	/* If the param has been modified, re-print only the parameter value */
	case ParamModified_0_255:
	case ParamModified_1_512:
		LCD_Locate(2, 8);
		LCD_printf("%d  ",tempParam);
		LCD_CMD(0x10); //shift cursor left
		LCD_CMD(0x10); //shift cursor left
		LCD_CMD(0x10); //shift cursor left
		break;

	/*********** Switch for menu state: case Param Mod. ***************/
	case ParamModify_0_1: /* If the param is going to be modified */
		LCD_2ndRow();
		LCD_printf("                ");
		LCD_2ndRow();
		if(tempParam) //if not zero
		{
			LCD_printf("Value: ON ");
		}
		else
		{
			LCD_printf("Value: OFF");
		}
		LCD_Locate(2, 8);
		LCD_CursorMode(BlinkingBlock_Cursor);
		break;

	/*********** Switch for menu state: case Param Mod. ***************/
	/* If the param has been modified, re-print only the parameter value */
	case ParamModified_0_1:
		LCD_Locate(2, 8);
		if(tempParam) //if not zero
		{
			LCD_printf("ON ");
		}
		else
		{
			LCD_printf("OFF");
		}
		LCD_Locate(2, 8);
		break;

	/*********** Switch for menu state: case Param Mod. ***************/
	case ParamModify_LightMode: /* If the param is going to be modified */
		LCD_2ndRow();
		LCD_printf("                ");
		LCD_2ndRow();
		switch(tempParam)
		{
		case LightMode_DMXControlled:
			LCD_printf(" DMX Controlled ");
			LCD_Locate(2, 2);
			break;
		case LightMode_Manual:
			LCD_printf("     Manual     ");
			LCD_Locate(2, 6);
			break;
		default:
			break;
		}

		LCD_CursorMode(BlinkingBlock_Cursor);
		break;

	/*********** Switch for menu state: case Param Mod. ***************/
	/* If the param has been modified, re-print only the parameter value */
	case ParamModified_LightMode:
		LCD_2ndRow();
		switch(tempParam)
		{
		case LightMode_DMXControlled:
			LCD_printf(" DMX Controlled ");
			LCD_Locate(2, 2);
			break;
		case LightMode_Manual:
			LCD_printf("     Manual     ");
			LCD_Locate(2, 6);
			break;
		default:
			break;
		}
		break;

	/*********** Switch for menu state: case Navigation *************/
	case Navigation:
		break;

	/*********** Switch for menu state: case DMX Check **************/
	case DMXCheckOnLCD_Entered:
		LCD_Clear_Display();
		switch(DMX_Mode)
		{
		case DMX_MODE_DATA:
		case DMX_MODE_BREAK:
			LCD_printf("Ch.    ");
			LCD_Locate(1,6);
			LCD_printf("%d",dmxCheckOnLCD_Ch_1stColumn);
			LCD_Locate(1,10);
			LCD_printf("%d",dmxCheckOnLCD_Ch_1stColumn+1);
			LCD_Locate(1,14);
			LCD_printf("%d",dmxCheckOnLCD_Ch_1stColumn+2);
			LCD_2ndRow();
			LCD_printf("Val.");
			LCD_Locate(2,6);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn]);
			LCD_Locate(2,10);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn + 1]);
			LCD_Locate(2,14);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn + 2]);
			break;
		case DMX_MODE_UNKNOWN_SC:
			LCD_printf("Unknown start");
			LCD_2ndRow();
			LCD_printf("code: %d",DMX_rxData[0]);
			break;
//		case DMX_MODE_ERROR:
//			LCD_printf("   DMX Error!   ");
//			break;
		case DMX_MODE_INIT:
			LCD_printf("DMX Initialized ");
			break;
		default:
			break;
		}
		menuState = DMXCheckOnLCD_Update;
		break;

	/*********** Switch for menu state: case DMX Check **************/
	case DMXCheckOnLCD_Update:
		switch(DMX_Mode)
		{
		case DMX_MODE_DATA:
		case DMX_MODE_BREAK:
			LCD_Locate(2,6);
			LCD_printf("           ");
			LCD_Locate(2,6);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn]);
			LCD_Locate(2,10);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn + 1]);
			LCD_Locate(2,14);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn + 2]);
			break;
		case DMX_MODE_UNKNOWN_SC:
			LCD_Clear_Display();
			LCD_printf("Unknown start");
			LCD_2ndRow();
			LCD_printf("code: %d",DMX_rxData[0]);
			break;
//		case DMX_MODE_ERROR:
//			LCD_Clear_Display();
//			LCD_printf("   DMX Error!   ");
//			break;
		case DMX_MODE_INIT:
			LCD_Clear_Display();
			LCD_printf("DMX Initialized ");
			break;
		default:
			break;
		}
		break;

	/*********** Switch for menu state: case DMX Check **************/
	case DMXCheckOnLCD_SwitchedChannels:
		switch(DMX_Mode)
		{
		case DMX_MODE_DATA:
		case DMX_MODE_BREAK:
			LCD_Locate(1,1);
			LCD_printf("Ch.  ");
			LCD_Locate(1,6);
			LCD_printf("           ");
			LCD_Locate(1,6);
			LCD_printf("%d",dmxCheckOnLCD_Ch_1stColumn);
			LCD_Locate(1,10);
			LCD_printf("%d",dmxCheckOnLCD_Ch_1stColumn+1);
			LCD_Locate(1,14);
			LCD_printf("%d",dmxCheckOnLCD_Ch_1stColumn+2);
			LCD_2ndRow();
			LCD_printf("Val.");
			LCD_Locate(2,6);
			LCD_printf("           ");
			LCD_Locate(2,6);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn]);
			LCD_Locate(2,10);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn + 1]);
			LCD_Locate(2,14);
			LCD_printf("%d",DMX_rxData[dmxCheckOnLCD_Ch_1stColumn + 2]);
			break;
		case DMX_MODE_UNKNOWN_SC:
			LCD_Clear_Display();
			LCD_printf("Unknown start");
			LCD_2ndRow();
			LCD_printf("code: %d",DMX_rxData[0]);
			break;
//		case DMX_MODE_ERROR:
//			LCD_Clear_Display();
//			LCD_printf("   DMX Error!   ");
//			break;
		case DMX_MODE_INIT:
			LCD_Clear_Display();
			LCD_printf("DMX Initialized ");
			break;
		default:
			break;
		}
		menuState = DMXCheckOnLCD_Update;
		break;

	/************* Switch for menu state: confirm req. **************/
	case ConfirmRequested:
		LCD_2ndRow();
		LCD_printf("                ");
		LCD_2ndRow();
		LCD_printf("Are you sure? %c",tempParam? 'Y' : 'N');
		LCD_Locate(2,15);
		LCD_CursorMode(BlinkingBlock_Cursor);
		break;
	case ConfirmRequestedModified:
		LCD_Locate(2,15);
		LCD_printf("%c",tempParam? 'Y' : 'N');
		LCD_Locate(2,15);
		break;

	/************* Switch for menu state: default case **************/
	default:
		break;
	}
}

/**
 * @brief	Update menu on clockwise rotation.
 */
void Menu_OnRotationCW(void)
{
	static uint32_t prevSystick = 0; /*!< Previous Systick value for encoder
											rotation speed 					*/

	uint32_t currSystick = HAL_GetTick(); /*!< Current Systick value for
												encoder rotation speed 		*/


	/* If the encoder is rotated quickly, turboRotation is on */
	int turboRotationIsOn = ((currSystick - prevSystick) < MAX_ELAPSED_SYSTICK_ROTATION);

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

	case ParamModify_1_512:
		if(tempParam < 512)
		{
			if(turboRotationIsOn & (tempParam < 508))
			{
				tempParam+=5;
			}
			else
			{
				tempParam++;
			}
		}
		menuState = ParamModified_1_512;
		break;

	case ParamModified_1_512:
		if(tempParam < 512)
		{
			if(turboRotationIsOn & (tempParam < 508))
			{
				tempParam+=5;
			}
			else
			{
				tempParam++;
			}
		}
		break;

	case ParamModify_0_255:
		if(tempParam < 255)
		{
			if(turboRotationIsOn & (tempParam < 250))
			{
				tempParam+=5;
			}
			else
			{
				tempParam++;
			}
		}
		menuState = ParamModified_0_255;
		break;

	case ParamModified_0_255:
		if(tempParam < 255)
		{
			if(turboRotationIsOn & (tempParam < 250))
			{
				tempParam+=5;
			}
			else
			{
				tempParam++;
			}
		}
		break;

	case ParamModify_0_1:
		if(tempParam) //if not zero
		{
		}
		else
		{
			tempParam=1; //if zero, put 1
		}
		menuState = ParamModified_0_1;
		break;

	case ParamModified_0_1:
		if(tempParam) //if zero
		{
		}
		else
		{
			tempParam=1; //if zero, put 1
		}
		break;

	case ParamModify_LightMode:
	case ParamNotModified_LightMode:
	case ParamModified_LightMode:
		if(tempParam == LightMode_DMXControlled)
		{
			tempParam = LightMode_Manual;
			menuState = ParamModified_LightMode;
		}
		else
		{
			menuState = ParamNotModified_LightMode;
		}
		break;

	case DMXCheckOnLCD_Entered:
	case DMXCheckOnLCD_Update:
	case DMXCheckOnLCD_SwitchedChannels:
		if(dmxCheckOnLCD_Ch_1stColumn < 510)
		{
			if(turboRotationIsOn && (dmxCheckOnLCD_Ch_1stColumn < 508))
			{
				dmxCheckOnLCD_Ch_1stColumn+=3;
			}
			else
			{
				dmxCheckOnLCD_Ch_1stColumn++;
			}
			menuState = DMXCheckOnLCD_SwitchedChannels;
		}
		break;

	case ConfirmRequested:
		if(!tempParam)
		{
			tempParam = 1;
			menuState = ConfirmRequestedModified;
		}
		break;

	case ConfirmRequestedModified:
			if(!tempParam)
			{
				tempParam = 1;
			}
			break;

	default:
		break;
	}

	prevSystick = currSystick; //update previous value with current
}

/**
 * @brief	Update menu on counter-clockwise rotation.
 */
void Menu_OnRotationCCW(void)
{

	static uint32_t prevSystick = 0; /*!< Previous Systick value for encoder
											rotation speed 					*/

	uint32_t currSystick = HAL_GetTick(); /*!< Current Systick value for
												encoder rotation speed 		*/


	/* If the encoder is rotated quickly, turboRotation is on */
	int turboRotationIsOn = ((currSystick - prevSystick) < MAX_ELAPSED_SYSTICK_ROTATION);

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

	case ParamModify_1_512:
		if(tempParam > 1)
		{
			if(turboRotationIsOn & (tempParam > 5))
			{
				tempParam-=5;
			}
			else
			{
				tempParam--;
			}
		}
		menuState = ParamModified_1_512;
		break;

	case ParamModified_1_512:
		if(tempParam > 1)
		{
			if(turboRotationIsOn & (tempParam > 5))
			{
				tempParam-=5;
			}
			else
			{
				tempParam--;
			}
		}
		break;

	case ParamModify_0_255:
		if(tempParam > 0)
		{
			if(turboRotationIsOn & (tempParam > 5))
			{
				tempParam-=5;
			}
			else
			{
				tempParam--;
			}
		}
		menuState = ParamModified_0_255;
		break;

	case ParamModified_0_255:
		if(tempParam > 0)
		{
			if(turboRotationIsOn & (tempParam > 5))
			{
				tempParam-=5;
			}
			else
			{
				tempParam--;
			}
		}
		break;

	case ParamModify_0_1:
		if(tempParam) //if not zero
		{
			tempParam = 0;
		}
		menuState = ParamModified_0_1;
		break;

	case ParamModified_0_1:
		if(tempParam) //if not zero
		{
			tempParam = 0;
		}
		break;

	case ParamModify_LightMode:
	case ParamNotModified_LightMode:
	case ParamModified_LightMode:
		if(tempParam == LightMode_Manual)
		{
			tempParam = LightMode_DMXControlled;
			menuState = ParamModified_LightMode;
		}
		else
		{
			menuState = ParamNotModified_LightMode;
		}
		break;

	case DMXCheckOnLCD_Entered:
	case DMXCheckOnLCD_Update:
	case DMXCheckOnLCD_SwitchedChannels:
		if(dmxCheckOnLCD_Ch_1stColumn > 1)
		{
			if(turboRotationIsOn && (dmxCheckOnLCD_Ch_1stColumn > 3))
			{
				dmxCheckOnLCD_Ch_1stColumn-=3;
			}
			else
			{
				dmxCheckOnLCD_Ch_1stColumn--;
			}
			menuState = DMXCheckOnLCD_SwitchedChannels;
		}
		break;

	case ConfirmRequested:
		if(tempParam)
		{
			tempParam = 0;
			menuState = ConfirmRequestedModified;
		}
		break;

	case ConfirmRequestedModified:
			if(tempParam)
			{
				tempParam = 0;
			}
			break;

	default:
		break;
	}
	prevSystick = currSystick; //update previous value with current
}

void Menu_DMXCheckOnLCD()
{
	menuState = DMXCheckOnLCD_Entered;
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

void Menu_Confirm()
{
	menuState = ConfirmRequested;
	tempParam = 0; //assume NO as default choose
}
