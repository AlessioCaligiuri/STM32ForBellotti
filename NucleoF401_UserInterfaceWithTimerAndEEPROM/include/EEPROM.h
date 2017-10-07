#ifndef __EEPROM_H
#define __EEPROM_H

/** Variables to save:
 * 	6 uint16_t for channels
 * 	6 uint8_t for light values
 * 	1 uint8_t thru mode on/off
 * 	1 uint8_t light mode dmx/manual
 * 	1 uint8_t debug mode via serial on/off
 *
 */

/* EEPROM timings in ms */
#define EEPROM_WRITE_TIME	5  //from datasheet of CAT24C04
#define EEPROM_READ_TIME	1  //supposed


/* EEPROM I2C Address */
#define	EEPROM_I2C_ADDRESS_FIRST_HALF	0xA0

/* 1st EEPROM page addresses definitions */
#define	EEPROM_LOCATION_CH_RED_A	0x00
#define	EEPROM_LOCATION_CH_GREEN_A	0x02
#define	EEPROM_LOCATION_CH_BLUE_A	0x04
#define	EEPROM_LOCATION_CH_RED_B	0x06
#define	EEPROM_LOCATION_CH_GREEN_B	0x08
#define	EEPROM_LOCATION_CH_BLUE_B	0x0A

#define EEPROM_LOCATION_DMX_THRU_MODE_ON_OFF		0x0C
#define EEPROM_LOCATION_LIGHT_MODE_DMX_MANUAL		0x0D
#define EEPROM_LOCATION_SERIAL_DEBUG_MODE_ON_OFF	0x0E

/* 2nd EEPROM page addresses definitions */
#define	EEPROM_LOCATION_LIGHT_RED_A		0x10
#define	EEPROM_LOCATION_LIGHT_GREEN_A	0x12
#define	EEPROM_LOCATION_LIGHT_BLUE_A	0x14
#define	EEPROM_LOCATION_LIGHT_RED_B		0x16
#define	EEPROM_LOCATION_LIGHT_GREEN_B	0x18
#define	EEPROM_LOCATION_LIGHT_BLUE_B	0x1A

void EEPROM_LoadFromMemory(void);
void EEPROM_SaveToMemory(void);
void EEPROM_RestoreDefaultMemory(void);

#endif
