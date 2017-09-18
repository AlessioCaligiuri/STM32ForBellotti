#include "menu.h"
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
MenuEntryStruct menuImpostazioni;
	MenuEntryStruct luceRossa;
	MenuEntryStruct luceVerde;
	MenuEntryStruct luceBlu;
	MenuEntryStruct indietroAImpostazioni;

MenuEntryStruct menuVerifica;
	MenuEntryStruct ricezione;
	MenuEntryStruct trasmissione;
	MenuEntryStruct indietroAVerifica;

MenuEntryStruct crediti;
	MenuEntryStruct alessio;

uint8_t	luceR;
uint8_t	luceV;
uint8_t	luceB;
/* END Entries structs ----------------------------------------------------- */

/**
 * @brief	Create menu entries and fill them fields.
 */
void MyMenu_CreateEntries(void)
{

	Menu_FillEntryWithZeros(&menuImpostazioni);
	strcpy(menuImpostazioni.name,	"      MENU");
	strcpy(menuImpostazioni.surname,"  IMPOSTAZIONI");
	menuImpostazioni.nextEntry = &menuVerifica;
	menuImpostazioni.lowerLevelEntry = &luceRossa;
	menuImpostazioni.onPression = Menu_GoLowerLevel;

	Menu_FillEntryWithZeros(&luceRossa);
	strcpy(luceRossa.name,	"LUCE ROSSA");
	strcpy(luceRossa.surname,"Modifica...");
	luceRossa.nextEntry = &luceVerde;
	luceRossa.param = &luceR;
	luceRossa.onPression = Menu_ModifyParam;

	Menu_FillEntryWithZeros(&luceVerde);
	strcpy(luceVerde.name,	"LUCE VERDE");
	strcpy(luceVerde.surname,"Modifica...");
	luceVerde.nextEntry = &luceBlu;
	luceVerde.previousEntry = &luceRossa;
	luceVerde.param = &luceV;
	luceVerde.onPression = Menu_ModifyParam;

	Menu_FillEntryWithZeros(&luceBlu);
	strcpy(luceBlu.name,	"LUCE BLU");
	strcpy(luceBlu.surname,"Modifica...");
	luceBlu.nextEntry = &indietroAImpostazioni;
	luceBlu.previousEntry = &luceVerde;
	luceBlu.param = &luceB;
	luceBlu.onPression = Menu_ModifyParam;

	Menu_FillEntryWithZeros(&indietroAImpostazioni);
	strcpy(indietroAImpostazioni.name,	"Torna indietro al");
	strcpy(indietroAImpostazioni.surname, "menu principale");
	indietroAImpostazioni.previousEntry = &luceBlu;
	indietroAImpostazioni.upperLevelEntry = &menuImpostazioni;
	indietroAImpostazioni.onPression = Menu_GoUpperLevel;


	Menu_FillEntryWithZeros(&menuVerifica);
	strcpy(menuVerifica.name,	"      MENU");
	strcpy(menuVerifica.surname,"    VERIFICA");
	menuVerifica.previousEntry = &menuImpostazioni;
	menuVerifica.lowerLevelEntry = &ricezione;
	menuVerifica.onPression = Menu_GoLowerLevel;

	Menu_FillEntryWithZeros(&ricezione);
	strcpy(ricezione.name,	"RICEZIONE");
	strcpy(ricezione.surname,"OK, funziona!");
	ricezione.nextEntry = &indietroAVerifica;

	Menu_FillEntryWithZeros(&indietroAVerifica);
	strcpy(indietroAVerifica.name,	"Torna indietro al");
	strcpy(indietroAVerifica.surname,"menu principale");
	indietroAVerifica.previousEntry = &ricezione;
	indietroAVerifica.upperLevelEntry = &menuVerifica;
	indietroAVerifica.onPression = Menu_GoUpperLevel;

	MyMenu_initialEntryPtr = &menuImpostazioni;
}
