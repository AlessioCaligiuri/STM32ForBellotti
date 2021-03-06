/**
  ******************************************************************************
  * @file    userInterface.h
  * @author  Alessio Caligiuri, Emanuele Anfuso
  * @version V0.9
  * @date    10 September 2017
  * @brief   User interface with HD44780 LCD and rotary encoder + push button.
  ******************************************************************************
  * @note
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

extern int encoderFlag_ButtonIsPressed;

extern int encoderRotationCount;

void UI_CreateEntries(void);
void UI_Init(void);
void UI_Update(void);
void UI_Message(char* message, uint32_t delay);
void UI_Error(char* message);


#endif
