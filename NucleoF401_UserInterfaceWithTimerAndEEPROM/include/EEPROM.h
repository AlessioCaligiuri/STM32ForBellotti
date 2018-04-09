/**
  ******************************************************************************
  * @file    EEPROM.h
  * @author  Alessio Caligiuri, Emanuele Anfuso
  * @version V0.9
  * @date    26 March 2018
  * @brief	 Functions to handle the EEPROM.
  ******************************************************************************
  */

#ifndef __EEPROM_H
#define __EEPROM_H

/** Variables to save:
 * 	6 uint16_t for channels
 * 	6 uint8_t for light values
 * 	1 uint8_t thru mode on/off
 * 	1 uint8_t light mode dmx/manual
 * 	1 uint8_t debug mode via serial on/off
 */

/* EEPROM timings in ms */
#define EEPROM_WRITE_TIME	5  //from datasheet of CAT24C04
#define EEPROM_READ_TIME	1  //supposed

#define	EEPROM_TIMEOUT_MS	10 //ms to wait in IsDeviceReady
#define	EEPROM_TRIALS		60 //trials to do in IsDeviceReady
/** @attention
 * HAL_I2C_IsDeviceReady polls the slave device to be ready by
 * sending the address and sampling the "ACK" bit. This is done continually
 * and the timeout refers to the availability of the MCU's I2C peripheral.
 * So, in general, the timeout is not the delay between two subsequent trials!
 * In our application, the I2C memory (24C04) has a maximum write time of 5ms
 * (see Atmel's datasheet) and the I2C speed is 100kbit/s by configuration.
 * Each time we make a trial, this requires 10 bits to be sent (1 start,
 * 7 address, 1 ack, 1 stop) and so 0.1ms elapse. To be sure that at least 5ms
 * elapsed, we make 60 trials (at maximum 60*0.1ms = 6ms).
 */


/* AT24C04 I2C memory, internally organized in 16-bytes pages */

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
#define EEPROM_LOCATION_DMX_CHECK_VIA_SERIAL_ON_OFF	0x0E

/* 2nd EEPROM page addresses definitions */
#define	EEPROM_LOCATION_LIGHT_RED_A		0x10
#define	EEPROM_LOCATION_LIGHT_GREEN_A	0x12
#define	EEPROM_LOCATION_LIGHT_BLUE_A	0x14
#define	EEPROM_LOCATION_LIGHT_RED_B		0x16
#define	EEPROM_LOCATION_LIGHT_GREEN_B	0x18
#define	EEPROM_LOCATION_LIGHT_BLUE_B	0x1A

/* Default EEPROM values */
#define	EEPROM_DEFAULT_CH_RED_A			1
#define	EEPROM_DEFAULT_CH_GREEN_A		2
#define	EEPROM_DEFAULT_CH_BLUE_A		3
#define	EEPROM_DEFAULT_CH_RED_B			4
#define	EEPROM_DEFAULT_CH_GREEN_B		5
#define	EEPROM_DEFAULT_CH_BLUE_B		6
#define EEPROM_DEFAULT_DMX_THRU_MODE_ON_OFF			1
#define EEPROM_DEFAULT_LIGHT_MODE_DMX_MANUAL		0
#define EEPROM_DEFAULT_DMX_CHECK_VIA_SERIAL_ON_OFF	0
#define	EEPROM_DEFAULT_LIGHT_RED_A		0x7F
#define	EEPROM_DEFAULT_LIGHT_GREEN_A	0x7F
#define	EEPROM_DEFAULT_LIGHT_BLUE_A		0x7F
#define	EEPROM_DEFAULT_LIGHT_RED_B		0x7F
#define	EEPROM_DEFAULT_LIGHT_GREEN_B	0x7F
#define	EEPROM_DEFAULT_LIGHT_BLUE_B		0x7F

void EEPROM_LoadFromMemory(void);
void EEPROM_SaveToMemory(void);
void EEPROM_RestoreDefaultMemory(void);

#endif
