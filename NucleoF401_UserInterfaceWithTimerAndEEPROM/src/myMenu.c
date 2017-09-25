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
	mesL1_lightMode.param = (int*)&lightMode;
	mesL1_lightMode.onPression = Menu_ModifyParam_LightMode;

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

	/* Dmx Check */
	Menu_FillEntryWithZeros(&mesL1_dmxCheck);
	strcpy(mesL1_dmxCheck.name,	 	"   DMX CHECK    ");
	strcpy(mesL1_dmxCheck.surname, 	"                ");
	mesL1_dmxCheck.onPression = Menu_DMXCheckOnLCD;
	mesL1_dmxCheck.previousEntry = &mesL1_fullOn;

	/*************** Level 2 entries ****************/
	/**** Level 2 of Dmx Settings *****/
	/* Red A Ch. */
	Menu_FillEntryWithZeros(&mesL2_ds_redA);
	strcpy(mesL2_ds_redA.name,	 "    Red A Ch.   ");
	strcpy(mesL2_ds_redA.surname,"press to modify ");
	mesL2_ds_redA.previousEntry = &mesL2_ds_back;
	mesL2_ds_redA.nextEntry = &mesL2_ds_greenA;
	mesL2_ds_redA.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_redA.param = &dmxCh_redA;
	mesL2_ds_redA.onPression = Menu_ModifyParam_0_511;

	/* Green A Ch. */
	Menu_FillEntryWithZeros(&mesL2_ds_greenA);
	strcpy(mesL2_ds_greenA.name,	"   Green A Ch.  ");
	strcpy(mesL2_ds_greenA.surname,	"press to modify ");
	mesL2_ds_greenA.previousEntry = &mesL2_ds_redA;
	mesL2_ds_greenA.nextEntry = &mesL2_ds_blueA;
	mesL2_ds_greenA.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_greenA.param = &dmxCh_greenA;
	mesL2_ds_greenA.onPression = Menu_ModifyParam_0_511;

	/* Blue A Ch. */
	Menu_FillEntryWithZeros(&mesL2_ds_blueA);
	strcpy(mesL2_ds_blueA.name,	  "   Blue A Ch.   ");
	strcpy(mesL2_ds_blueA.surname,"press to modify ");
	mesL2_ds_blueA.previousEntry = &mesL2_ds_greenA;
	mesL2_ds_blueA.nextEntry = &mesL2_ds_redB;
	mesL2_ds_blueA.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_blueA.param = &dmxCh_blueA;
	mesL2_ds_blueA.onPression = Menu_ModifyParam_0_511;

	/* Red B Ch. */
	Menu_FillEntryWithZeros(&mesL2_ds_redB);
	strcpy(mesL2_ds_redB.name,	 "    Red B Ch.   ");
	strcpy(mesL2_ds_redB.surname,"press to modify ");
	mesL2_ds_redB.previousEntry = &mesL2_ds_blueA;
	mesL2_ds_redB.nextEntry = &mesL2_ds_greenB;
	mesL2_ds_redB.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_redB.param = &dmxCh_redB;
	mesL2_ds_redB.onPression = Menu_ModifyParam_0_511;

	/* Green B Ch. */
	Menu_FillEntryWithZeros(&mesL2_ds_greenB);
	strcpy(mesL2_ds_greenB.name,	"   Green B Ch.  ");
	strcpy(mesL2_ds_greenB.surname,	"press to modify ");
	mesL2_ds_greenB.previousEntry = &mesL2_ds_redB;
	mesL2_ds_greenB.nextEntry = &mesL2_ds_blueB;
	mesL2_ds_greenB.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_greenB.param = &dmxCh_greenB;
	mesL2_ds_greenB.onPression = Menu_ModifyParam_0_511;

	/* Blue B Ch. */
	Menu_FillEntryWithZeros(&mesL2_ds_blueB);
	strcpy(mesL2_ds_blueB.name,	  "   Blue B Ch.   ");
	strcpy(mesL2_ds_blueB.surname,"press to modify ");
	mesL2_ds_blueB.previousEntry = &mesL2_ds_greenB;
	mesL2_ds_blueB.nextEntry = &mesL2_ds_thru;
	mesL2_ds_blueB.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_blueB.param = &dmxCh_blueB;
	mesL2_ds_blueB.onPression = Menu_ModifyParam_0_511;

	/* Thru Mode */
	Menu_FillEntryWithZeros(&mesL2_ds_thru);
	strcpy(mesL2_ds_thru.name,	  "   Thru Mode    ");
	strcpy(mesL2_ds_thru.surname, "press to on/off ");
	mesL2_ds_thru.previousEntry = &mesL2_ds_blueB;
	mesL2_ds_thru.nextEntry = &mesL2_ds_back;
	mesL2_ds_thru.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_thru.param = &dmxThruModeIsActive;
	mesL2_ds_thru.onPression = Menu_ModifyParam_0_1;

	/* Back */
	Menu_FillEntryWithZeros(&mesL2_ds_back);
	strcpy(mesL2_ds_back.name,	  "  Back to main  ");
	strcpy(mesL2_ds_back.surname, "      menu      ");
	mesL2_ds_back.previousEntry = &mesL2_ds_thru;
	mesL2_ds_back.nextEntry = &mesL2_ds_redA;
	mesL2_ds_back.upperLevelEntry = &mesL1_dmxSettings;
	mesL2_ds_back.onPression = Menu_GoUpperLevel;


	/**** Level 2 of Manual Settings *****/
	/* Red A Light */
	Menu_FillEntryWithZeros(&mesL2_ms_redA);
	strcpy(mesL2_ms_redA.name,	 "  Red A Light   ");
	strcpy(mesL2_ms_redA.surname,"press to modify ");
	mesL2_ms_redA.previousEntry = &mesL2_ms_back;
	mesL2_ms_redA.nextEntry = &mesL2_ms_greenA;
	mesL2_ms_redA.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_redA.param = &light_redA;
	mesL2_ms_redA.onPression = Menu_ModifyParam_0_255;

	/* Green A Light */
	Menu_FillEntryWithZeros(&mesL2_ms_greenA);
	strcpy(mesL2_ms_greenA.name,	" Green A Light  ");
	strcpy(mesL2_ms_greenA.surname,	"press to modify ");
	mesL2_ms_greenA.previousEntry = &mesL2_ms_redA;
	mesL2_ms_greenA.nextEntry = &mesL2_ms_blueA;
	mesL2_ms_greenA.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_greenA.param = &light_greenA;
	mesL2_ms_greenA.onPression = Menu_ModifyParam_0_255;

	/* Blue A Light */
	Menu_FillEntryWithZeros(&mesL2_ms_blueA);
	strcpy(mesL2_ms_blueA.name,	  "  Blue A Light  ");
	strcpy(mesL2_ms_blueA.surname,"press to modify ");
	mesL2_ms_blueA.previousEntry = &mesL2_ms_greenA;
	mesL2_ms_blueA.nextEntry = &mesL2_ms_redB;
	mesL2_ms_blueA.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_blueA.param = &light_blueA;
	mesL2_ms_blueA.onPression = Menu_ModifyParam_0_255;

	/* Red B Light */
	Menu_FillEntryWithZeros(&mesL2_ms_redB);
	strcpy(mesL2_ms_redB.name,	 "  Red B Light   ");
	strcpy(mesL2_ms_redB.surname,"press to modify ");
	mesL2_ms_redB.previousEntry = &mesL2_ms_blueA;
	mesL2_ms_redB.nextEntry = &mesL2_ms_greenB;
	mesL2_ms_redB.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_redB.param = &light_redB;
	mesL2_ms_redB.onPression = Menu_ModifyParam_0_255;

	/* Green B Light */
	Menu_FillEntryWithZeros(&mesL2_ms_greenB);
	strcpy(mesL2_ms_greenB.name,	" Green B Light  ");
	strcpy(mesL2_ms_greenB.surname,	"press to modify ");
	mesL2_ms_greenB.previousEntry = &mesL2_ms_redB;
	mesL2_ms_greenB.nextEntry = &mesL2_ms_blueB;
	mesL2_ms_greenB.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_greenB.param = &light_greenB;
	mesL2_ms_greenB.onPression = Menu_ModifyParam_0_255;

	/* Blue B Light */
	Menu_FillEntryWithZeros(&mesL2_ms_blueB);
	strcpy(mesL2_ms_blueB.name,	  "  Blue B Light  ");
	strcpy(mesL2_ms_blueB.surname,"press to modify ");
	mesL2_ms_blueB.previousEntry = &mesL2_ms_greenB;
	mesL2_ms_blueB.nextEntry = &mesL2_ms_back;
	mesL2_ms_blueB.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_blueB.param = &light_blueB;
	mesL2_ms_blueB.onPression = Menu_ModifyParam_0_255;

	/* Back */
	Menu_FillEntryWithZeros(&mesL2_ms_back);
	strcpy(mesL2_ms_back.name,	  "  Back to main  ");
	strcpy(mesL2_ms_back.surname, "      menu      ");
	mesL2_ms_back.previousEntry = &mesL2_ms_blueB;
	mesL2_ms_back.nextEntry = &mesL2_ms_redA;
	mesL2_ms_back.upperLevelEntry = &mesL1_manualSettings;
	mesL2_ms_back.onPression = Menu_GoUpperLevel;

	MyMenu_initialEntryPtr = &mesL1_lightMode;
}
