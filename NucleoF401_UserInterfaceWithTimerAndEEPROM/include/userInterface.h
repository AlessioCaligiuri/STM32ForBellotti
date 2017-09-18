/**
  ******************************************************************************
  * @file    userInterface.h
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

#ifndef __USERINTERFACE_H
#define __USERINTERFACE_H

#include "menu.h"

/**
 * @brief	Encoder pressure flag. 0 if not pressed.
 */
extern int encoderFlag_ButtonIsPressed;

/**
 * @brief	Encoder rotation flag. 0 if no rotation has occurred.
 */
extern int encoderFlag_IsRotated;

/**
 * @brief	Encoder rotation direction. 0 if CCW, other if CW.
 */
extern int encoderFlag_RotationClockwise;


void UI_CreateEntries(void);
void UI_Init(void);
void UI_Update(void);
void UI_Message(char* message, uint32_t delay);
void UI_Error(char* message);


#endif
