/**
  ******************************************************************************
  * @file    userInterface.c
  * @author  Alessio Caligiuri
  * @version V0.0.1
  * @date    10 September 2017
  * @brief   User interface with HD44780 LCD and rotary encoder + push button.
  ******************************************************************************
  * @details
  *	This code is used to manage a user interface made by an HD44780 LCD (2x16)
  *	and a rotary encoder with push button.
  *	This user interface needs a menu library and a mymenu configuration file to
  *	create ad use the menu.
  *
  ******************************************************************************
  */

#include "menu.h"
#include "userInterface.h"
#include "myMenu.h"
#include "HD44780.h"
#include <string.h>

/**
 * @brief	Encoder pressure flag. 0 if not pressed.
 */
int encoderFlag_ButtonIsPressed;

/**
 * @brief	Encoder rotation flag. 0 if no rotation has occurred.
 */
int encoderFlag_IsRotated;

/**
 * @brief	Encoder rotation direction. 0 if CCW, other if CW.
 */
int encoderFlag_RotationClockwise;

int encoderRotation =  0;

/**
 * @brief	This function is similar to HAL_Delay, but it can be interrupted
 * 			by encoder press or rotation.
 */
void HAL_Delay_Breakable(uint32_t Delay)
{

	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = Delay;

	/* Add a period to guarantee minimum wait */
	if (wait < HAL_MAX_DELAY)
	{
	 wait++;
	}


	while((HAL_GetTick() - tickstart) < wait)
	{
		if(encoderFlag_ButtonIsPressed)
		{
			encoderFlag_ButtonIsPressed = 0;
			break;
		}
		if(encoderFlag_IsRotated)
		{
			encoderFlag_IsRotated = 0;
			break;
		}
	}
}

/**
 * @brief	Wait indefinitely encoder press on rotation.
 */
void Wait_EncoderPressOrRotation(void)
{
	while(1)
	{
		if(encoderFlag_ButtonIsPressed)
		{
			encoderFlag_ButtonIsPressed = 0;
			break;
		}
		if(encoderFlag_IsRotated)
		{
			encoderFlag_IsRotated = 0;
			break;
		}
	}
}

/**
 * @brief	User interface initialization:
 * 			menu creation and LCD initialization.
 */
void UI_Init(void)
{
	/* Initialize LCD display */
	LCD_Init();
	LCD_Backlight_On();

	/* Show initiation message */
	LCD_Clear_Display();
	LCD_printf("Welcome!\nLCD Test...");
	HAL_GPIO_WritePin(LDO_GPIO_Port, LDO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LDB_GPIO_Port, LDB_Pin, GPIO_PIN_SET);
	HAL_Delay_Breakable(5000);

	/* Create entries */
	MyMenu_CreateEntries();

	/* Call menu init */
	Menu_Init(MyMenu_initialEntryPtr);
	Menu_Show();
}

/**
 * @brief	Update and show menu, looking at encoder flags.
 * 			To call in an infinite loop in main.
 */
void UI_Update(void)
{
	int encoderRotCopy;
	int i;

	/* If encoder has been pressed */
	if(encoderFlag_ButtonIsPressed)
	{
		encoderFlag_ButtonIsPressed = 0; //reset flag
		Menu_OnPression();
		Menu_Show();
	}

	/* If encoder has been rotated */
	if(encoderFlag_IsRotated)
	{
		encoderFlag_IsRotated = 0; //reset flag

		encoderRotCopy = encoderRotation;
		encoderRotation = 0;

		if(encoderRotCopy > 0)
		{
			for(i = 0; i<encoderRotCopy; i++)
			{
				Menu_OnRotationCW();
			}
		}
		else
		{
			for(i = 0; i>encoderRotCopy; i--)
			{
				Menu_OnRotationCCW();
			}
		}
		Menu_Show();

//		/* Check direction on rotation */
//		if(encoderFlag_RotationClockwise)
//		{
//			Menu_OnRotationCW();
//			Menu_Show();
//		}
//		else
//		{
//			Menu_OnRotationCCW();
//			Menu_Show();
//		}
	}
}


/**
 * @brief	Show a message on LCD, waiting for delay time or
 * 			encoder press/rotation. Then show menu.
 * @param	message	Message to write.
 * @param	delay	Delay in ms, to wait before showing menu.
 */
void UI_Message(char* message, uint32_t delay)
{
	LCD_Clear_Display();
	LCD_printf(message);
	HAL_Delay_Breakable(delay);
	Menu_Show();
}

/**
 * @brief	Show an error message on LCD and turn on orange light, waiting for
 * 			encoder press/rotation. Then show menu.
 * @param	message	Message to write.
 * @param	delay	Delay in ms, to wait before showing menu.
 */

void UI_Error(char* message)
{
	LCD_Clear_Display();
	LCD_printf(message);
	/* Orange encoder LED ON */
	HAL_GPIO_WritePin(LDB_GPIO_Port, LDB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LDO_GPIO_Port, LDO_Pin, GPIO_PIN_SET);

}
