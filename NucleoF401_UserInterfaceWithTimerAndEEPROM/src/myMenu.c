#include "menu.h"
#include "main.h"
#include "myMenu.h"
#include <string.h>

/**
  ******************************************************************************
  * @file    myMenu.c
  * @author  Alessio Caligiuri
  * @version V0.0.1
  * @date    10 September 2017
  * @brief   Creation of menu entries.
  ******************************************************************************
  * @details
  *
  ******************************************************************************
  */

/**
 * @brief	Initial entry pointer, used by user interface to init menu.
 */
MenuEntryStruct* MyMenu_initialEntryPtr;

/* BEGIN Entries structs --------------------------------------------------- */

/* Level 1 entries */
MenuEntryStruct mesL1_lightMode;
MenuEntryStruct mesL1_dmxSettings;
MenuEntryStruct mesL1_manualSettings;
MenuEntryStruct mesL1_fullOn;
MenuEntryStruct mesL1_dmxCheck;

/* Level 2 of DMX Settings entries */
MenuEntryStruct mesL2_ds_redA;
MenuEntryStruct mesL2_ds_greenA;
MenuEntryStruct mesL2_ds_blueA;
MenuEntryStruct mesL2_ds_redB;
MenuEntryStruct mesL2_ds_greenB;
MenuEntryStruct mesL2_ds_blueB;
MenuEntryStruct mesL2_ds_thru;
MenuEntryStruct mesL2_ds_back;

/* Level 2 of Manual Settings entries */
MenuEntryStruct mesL2_ms_redA;
MenuEntryStruct mesL2_ms_greenA;
MenuEntryStruct mesL2_ms_blueA;
MenuEntryStruct mesL2_ms_redB;
MenuEntryStruct mesL2_ms_greenB;
MenuEntryStruct mesL2_ms_blueB;
MenuEntryStruct mesL2_ms_back;

/* Level 2 of DMX check entries */
MenuEntryStruct mesL2_dc_serial;
MenuEntryStruct mesL2_dc_lcd;

/* END Entries structs ----------------------------------------------------- */

/**
 * @brief	Create menu entries and fill them fields.
 */
void MyMenu_CreateEntries(void)
{
	/*************** Level 1 entries ****************/
	/* Light mode */
	Menu_FillEntryWithZeros(&mesL1_lightMode);
	strcpy(mesL1_lightMode.name,	"   LIGHT MODE   ");
	strcpy(mesL1_lightMode.surname, "press to change ");
	mesL1_lightMode.nextEntry = &mesL1_dmxSettings;
	mesL1_lightMode.param = &lightMode;
	//mesL1_lightMode.onPression = Menu_GoLowerLevel;

	/* DMX Settings */
	Menu_FillEntryWithZeros(&mesL1_dmxSettings);
	strcpy(mesL1_dmxSettings.name,	 "  DMX SETTINGS  ");
	strcpy(mesL1_dmxSettings.surname,"                ");
	mesL1_dmxSettings.previousEntry = &mesL1_lightMode;
	mesL1_dmxSettings.nextEntry = &mesL1_manualSettings;
	mesL1_dmxSettings.lowerLevelEntry = &mesL2_ds_redA;
	mesL1_dmxSettings.onPression = Menu_GoLowerLevel;

	/* Manual Settings */
	Menu_FillEntryWithZeros(&mesL1_manualSettings);
	strcpy(mesL1_manualSettings.name,	 "MANUAL SETTINGS ");
	strcpy(mesL1_manualSettings.surname, "                ");
	mesL1_manualSettings.previousEntry = &mesL1_dmxSettings;
	mesL1_manualSettings.nextEntry = &mesL1_fullOn;
	mesL1_manualSettings.lowerLevelEntry = &mesL2_ms_redA;
	mesL1_manualSettings.onPression = Menu_GoLowerLevel;

	/* Full On */
	Menu_FillEntryWithZeros(&mesL1_fullOn);
	strcpy(mesL1_fullOn.name,	 "    FULL ON     ");
	strcpy(mesL1_fullOn.surname, "press to on/off ");
	mesL1_fullOn.previousEntry = &mesL1_manualSettings;
	mesL1_fullOn.nextEntry = &mesL1_dmxCheck;
	mesL1_fullOn.param = &fullOnIsActive;
	mesL1_fullOn.onPression = Menu_ModifyParam_0_1;

	/*************** Level 2 entries ****************/

	MyMenu_initialEntryPtr = &menuImpostazioni;
}
