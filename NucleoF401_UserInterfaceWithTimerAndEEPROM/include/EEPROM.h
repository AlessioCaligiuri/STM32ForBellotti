#ifndef __EEPROM_H
#define __EEPROM_H

/** Variables to save:
 * 	6 uint16_t for channels
 * 	6 uint8_t for light values
 * 	1 uint8_t thru mode on/off
 * 	1 uint8_t light mode on/off
 * 	1 uint8_t debug mode via serial on/off
 *
 */

void EEPROM_LoadFromMemory(void);
void EEPROM_SaveToMemory(void);
void EEPROM_RestoreDefaultMemory(void);

#endif
