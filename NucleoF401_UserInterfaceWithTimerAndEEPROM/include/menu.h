/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENU_H
#define __MENU_H

#include <stdint.h>	//included to use uintXX_t types

/**
 * @brief Maximum name string length (included \0)
 */
#define MAX_TEXT_LENGTH 33

/**
 * Type definition for void --> void functions.
 */
typedef void(*functionPtr)(void);

/**
 * @brief Type definition for menu entry.
 */
typedef struct MenuEntry
{
	char name[MAX_TEXT_LENGTH];			/*!< Menu entry name.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	char surname[MAX_TEXT_LENGTH];		/*!< Menu entry "surname", i.e. 2nd row text
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	struct MenuEntry *previousEntry;	/*!< Pointer to the previous menu entry.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	struct MenuEntry *nextEntry;		/*!< Pointer to the next menu entry.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	struct MenuEntry *upperLevelEntry;	/*!< Pointer to the upper level menu entry.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	struct MenuEntry *lowerLevelEntry ;	/*!< Pointer to the lower level menu entry.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	functionPtr onPression;				/*!< Pointer to the function to be executed,
	 	 	 	 	 	 	 	 	 	 	 	 when the encoder button is pressed. 	 */
	void *param;							/*!< Pointer to parameter modified by this
											 	 entry (generic).						 */
	int isModified;						/*!< True if the pointed parameter was been
	 	 	 	 	 	 	 	 	 	 	 	 modified.								 */
} MenuEntryStruct;

/**
 * @brief Enum for menu state.
 */
typedef enum MenuState {
	Navigation = 0,			/*!< Used when no new entry has to be shown. 				*/
	NavigationUpdate,		/*!< Used when a new entry has to be shown. 				*/
	ParamModify_1_512,		/*!< Used when the user want to modify a parameter (with 1
	 	 	 	 	 			- 512 bounds) but the value hasn't been changed yet. 	*/
	ParamModified_1_512,	/*!< Used when the user has modified a parameter value (with
								1 - 512 bounds).										*/
	ParamModify_0_255,		/*!< Used when the user want to modify a parameter (with 0
	 	 	 	 	 			- 255 bounds) but the value hasn't been changed yet. 	*/
	ParamModified_0_255,	/*!< Used when the user has modified a parameter value (with
								0 - 255 bounds).										*/
	ParamModify_0_1,		/*!< Used when the user want to modify a parameter (with boolean
								value) but the value hasn't been changed yet. 			*/
	ParamModified_0_1,		/*!< Used when the user has modified a parameter (with boolean
								value) value.											*/
	ParamModify_LightMode,	/*!< Used when the user want to modify the parameter "Light
	 	 	 	 	 	 	 	 mode" but the value hasn't been changed yet. 			*/
	ParamModified_LightMode, /*!< Used when the user has modified the parameter "Light
								mode" value.											*/
	ParamNotModified_LightMode, /*!< Used when the user has rotated the encoder but the
										parameter hasn't been modified 					*/
	DMXCheckOnLCD_Entered,		/*!< Used when the LCD displays DMX data because DMX Check
	 	 	 	 	 	 	 	 	 	 has just been entered */
	DMXCheckOnLCD_Update,		/*!< Used when the LCD displays DMX data and the values
										have to be updated */
	DMXCheckOnLCD_SwitchedChannels, /*!< Used when the LCD displays DMX data and the channels
										to show have been modified */
	ConfirmRequested,
	ConfirmRequestedModified
} MenuState_t;

/**
 * @brief Variable that holds the menu state.
 */
MenuState_t menuState;

/**
 * @brief	Pointer to the current menu entry.
 */
MenuEntryStruct* Menu_currentEntry;

/**
 * @brief	Message to show on LCD.
 */
char lcdMessage[MAX_TEXT_LENGTH];

void Menu_Init(MenuEntryStruct *initialMenuEntry);
void Menu_Show(void);
void Menu_FillEntryWithZeros(MenuEntryStruct *menuEntryStruct);
void Menu_GoLowerLevel(void);
void Menu_GoUpperLevel(void);
void Menu_GoPreviousEntry(void);
void Menu_GoNextEntry(void);
void Menu_ModifyParam_1_512(void);
void Menu_ModifyParam_0_255(void);
void Menu_ModifyParam_0_1(void);
void Menu_ModifyParam_LightMode(void);
void Menu_DMXCheckOnLCD(void);
void Menu_OnRotationCW(void);
void Menu_OnRotationCCW(void);
void Menu_OnPression(void);
void Menu_SwitchEntry(MenuEntryStruct* Menu_newEntry);
void Menu_Confirm(void);


#endif /* __MENU_H */
