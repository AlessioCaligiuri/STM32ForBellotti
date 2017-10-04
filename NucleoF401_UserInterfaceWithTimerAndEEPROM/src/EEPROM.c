#include "EEPROM.h"
#include "HD44780.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"

void EEPROM_LoadFromMemory()
{
	char rmsg;
	HAL_I2C_Mem_Read(&hi2c1, 0xA0, 0, 1, (uint8_t*)&rmsg, 1, HAL_MAX_DELAY);
	while(HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, HAL_MAX_DELAY) != HAL_OK);
	LCD_Clear_Display();
	LCD_printf("Load completed %c",rmsg);
 	HAL_Delay(500);
}

void EEPROM_SaveToMemory()
{
	char wmsg = '?';
	HAL_I2C_Mem_Write(&hi2c1, 0xA0, 0, 1, (uint8_t*)&wmsg, 1, HAL_MAX_DELAY);
	while(HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, HAL_MAX_DELAY) != HAL_OK);
	LCD_Clear_Display();
	LCD_printf("Save completed!");
 	HAL_Delay(500);
}

void EEPROM_RestoreDefaultMemory()
{
	HAL_Delay(1000);
}
