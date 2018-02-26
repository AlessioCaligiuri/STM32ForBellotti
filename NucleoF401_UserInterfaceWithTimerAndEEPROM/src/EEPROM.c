#include "EEPROM.h"
#include "HD44780.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "userInterface.h"

void LoadParam_1_512(uint8_t memAddress, uint16_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag);
void SaveParam_1_512(uint8_t memAddress, uint16_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag);
void LoadParam_0_255(uint8_t memAddress, uint8_t* paramPtr,
		uint8_t* memErrorFlag);
void SaveParam_0_255(uint8_t memAddress, uint8_t* paramPtr,
		uint8_t* memErrorFlag);
void LoadParam_0_1(uint8_t memAddress, uint8_t* paramPtr, uint8_t* memErrorFlag);
void SaveParam_0_1(uint8_t memAddress, uint8_t* paramPtr, uint8_t* memErrorFlag);
void LoadParam_LightMode(uint8_t memAddress, LightMode_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag);
void SaveParam_LightMode(uint8_t memAddress, LightMode_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag);

/**
 * @brief	Loads values from EEPROM and put them into global variables.
 * 			Prints a success or error message.
 */
void EEPROM_LoadFromMemory()
{
	uint8_t somethingBadHappened = 0;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		UI_Error(" I2C or EEPROM  \n     ERROR!     ");
		return;
	}

	/* Load DMX rx channels data */
	LoadParam_1_512(EEPROM_LOCATION_CH_RED_A,&dmxCh_redA,
			" ERROR: invalid \n red A ch value ", &somethingBadHappened);
	LoadParam_1_512(EEPROM_LOCATION_CH_GREEN_A,&dmxCh_greenA,
			" ERROR: invalid \ngreen A ch value", &somethingBadHappened);
	LoadParam_1_512(EEPROM_LOCATION_CH_BLUE_A,&dmxCh_blueA,
			" ERROR: invalid \nblue A ch value ", &somethingBadHappened);
	LoadParam_1_512(EEPROM_LOCATION_CH_RED_B,&dmxCh_redB,
			" ERROR: invalid \n red B ch value ", &somethingBadHappened);
	LoadParam_1_512(EEPROM_LOCATION_CH_GREEN_B,&dmxCh_greenB,
			" ERROR: invalid \ngreen B ch value", &somethingBadHappened);
	LoadParam_1_512(EEPROM_LOCATION_CH_BLUE_B,&dmxCh_blueB,
			" ERROR: invalid \nblue B ch value ", &somethingBadHappened);

	/* Options data */
	LoadParam_0_1(EEPROM_LOCATION_DMX_THRU_MODE_ON_OFF,&dmxThruModeIsActive,
			&somethingBadHappened);
	LoadParam_LightMode(EEPROM_LOCATION_LIGHT_MODE_DMX_MANUAL,&lightMode,
			" ERROR: invalid \nlight mode value",&somethingBadHappened);
	LoadParam_0_1(EEPROM_LOCATION_DMX_CHECK_VIA_SERIAL_ON_OFF,&dmxCheckViaSerial_isOn,
			&somethingBadHappened);

	/* Light in manual mode data */
	LoadParam_0_255(EEPROM_LOCATION_LIGHT_RED_A,&light_redA,
			&somethingBadHappened);
	LoadParam_0_255(EEPROM_LOCATION_LIGHT_GREEN_A,&light_greenA,
			&somethingBadHappened);
	LoadParam_0_255(EEPROM_LOCATION_LIGHT_BLUE_A,&light_blueA,
			&somethingBadHappened);
	LoadParam_0_255(EEPROM_LOCATION_LIGHT_RED_B,&light_redB,
			&somethingBadHappened);
	LoadParam_0_255(EEPROM_LOCATION_LIGHT_GREEN_B,&light_greenB,
			&somethingBadHappened);
	LoadParam_0_255(EEPROM_LOCATION_LIGHT_BLUE_B,&light_blueB,
			&somethingBadHappened);

	if(somethingBadHappened)
	{
		UI_Error(" I2C or EEPROM  \n     ERROR!     ");
	}
	else
	{
		UI_Message("Load completed!",500);
	}
}

/**
 * @brief	Saves values from global variables into the EEPROM.
 * 			Prints a success or error message.
 */
void EEPROM_SaveToMemory()
{
	uint8_t somethingBadHappened = 0;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		UI_Error(" I2C or EEPROM  \n     ERROR!     ");
		return;
	}

	/* Save DMX rx channels data */
	SaveParam_1_512(EEPROM_LOCATION_CH_RED_A,&dmxCh_redA,
			" ERROR: invalid \n red A ch value ", &somethingBadHappened);
	SaveParam_1_512(EEPROM_LOCATION_CH_GREEN_A,&dmxCh_greenA,
			" ERROR: invalid \ngreen A ch value", &somethingBadHappened);
	SaveParam_1_512(EEPROM_LOCATION_CH_BLUE_A,&dmxCh_blueA,
			" ERROR: invalid \nblue A ch value ", &somethingBadHappened);
	SaveParam_1_512(EEPROM_LOCATION_CH_RED_B,&dmxCh_redB,
			" ERROR: invalid \n red B ch value ", &somethingBadHappened);
	SaveParam_1_512(EEPROM_LOCATION_CH_GREEN_B,&dmxCh_greenB,
			" ERROR: invalid \ngreen B ch value", &somethingBadHappened);
	SaveParam_1_512(EEPROM_LOCATION_CH_BLUE_B,&dmxCh_blueB,
			" ERROR: invalid \nblue B ch value ", &somethingBadHappened);

	/* Options data */
	SaveParam_0_1(EEPROM_LOCATION_DMX_THRU_MODE_ON_OFF,&dmxThruModeIsActive,
			&somethingBadHappened);
	SaveParam_LightMode(EEPROM_LOCATION_LIGHT_MODE_DMX_MANUAL,&lightMode,
			" ERROR: invalid \nlight mode value",&somethingBadHappened);
	SaveParam_0_1(EEPROM_LOCATION_DMX_CHECK_VIA_SERIAL_ON_OFF,&dmxCheckViaSerial_isOn,
			&somethingBadHappened);

	/* Light in manual mode data */
	SaveParam_0_255(EEPROM_LOCATION_LIGHT_RED_A,&light_redA,
			&somethingBadHappened);
	SaveParam_0_255(EEPROM_LOCATION_LIGHT_GREEN_A,&light_greenA,
			&somethingBadHappened);
	SaveParam_0_255(EEPROM_LOCATION_LIGHT_BLUE_A,&light_blueA,
			&somethingBadHappened);
	SaveParam_0_255(EEPROM_LOCATION_LIGHT_RED_B,&light_redB,
			&somethingBadHappened);
	SaveParam_0_255(EEPROM_LOCATION_LIGHT_GREEN_B,&light_greenB,
			&somethingBadHappened);
	SaveParam_0_255(EEPROM_LOCATION_LIGHT_BLUE_B,&light_blueB,
			&somethingBadHappened);

	if(somethingBadHappened)
	{
		UI_Error(" I2C or EEPROM  \n     ERROR!     ");
	}
	else
	{
		UI_Message("Save completed!",500);
	}
}

/**
 * @brief	Reprograms EEPROM memory with default values.
 */
void EEPROM_RestoreDefaultMemory()
{
	UI_Message("Not done!",500);
}

/**
 * @brief	Used to load a parameter in 1-512 range (uint16_t type)
 * @param	memAddress	Initial EEPROM location where to find data
 * @param	paramPtr	Pointer to the RAM variable where to store data
 * @param	errorMsg	String to write if the parameter is out of 1 - 512 range
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void LoadParam_1_512(uint8_t memAddress, uint16_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag)
{
	uint16_t temp;
	HAL_StatusTypeDef halStatus;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Read from memory
	halStatus = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)&temp, sizeof(uint16_t), EEPROM_READ_TIME);

	//If something went wrong during reading
	if(halStatus != HAL_OK)
	{
		*memErrorFlag = 1;
		return;
	}

	//Check read parameter
	if(temp > 0 && temp <= 512)
	{
		*paramPtr = temp;
	}
	else
	{
		UI_Error(errorMsg);
	}
}


/**
 * @brief	Used to save a parameter in 1-512 range (uint16_t type)
 * @param	memAddress	Initial EEPROM location where to store data
 * @param	paramPtr	Pointer to the RAM variable where to catch data
 * @param	errorMsg	String to write if the parameter is out of 1 - 512 range
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void SaveParam_1_512(uint8_t memAddress, uint16_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag)
{
	uint16_t temp = *paramPtr;
	HAL_StatusTypeDef halStatus;

	/* Check if the passed data is in 1 - 512 range */
	if(temp == 0 || temp > 512)
	{
		UI_Error(errorMsg);
		return;
	}

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Try to write
	halStatus = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)paramPtr, sizeof(uint16_t), EEPROM_WRITE_TIME);

	if(halStatus != HAL_OK) //if something went wrong during writing
	{
		*memErrorFlag = 1;
		return;
	}
}


/**
 * @brief	Used to load a parameter in 0-255 range (uint8_t type)
 * @param	memAddress	EEPROM location where to find data
 * @param	paramPtr	Pointer to the RAM variable where to store data
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void LoadParam_0_255(uint8_t memAddress, uint8_t* paramPtr,
		uint8_t* memErrorFlag)
{
	uint8_t temp;
	HAL_StatusTypeDef halStatus;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Read from memory
	halStatus = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)&temp, sizeof(uint8_t), EEPROM_READ_TIME);

	//If something went wrong during reading
	if(halStatus != HAL_OK)
	{
		*memErrorFlag = 1;
		return;
	}

	*paramPtr = temp;
}

/**
 * @brief	Used to save a parameter in 0-255 range (uint8_t type)
 * @param	memAddress	EEPROM location where to store data
 * @param	paramPtr	Pointer to the RAM variable where to catch data
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void SaveParam_0_255(uint8_t memAddress, uint8_t* paramPtr,
		uint8_t* memErrorFlag)
{
	HAL_StatusTypeDef halStatus;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Try to write
	halStatus = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)paramPtr, sizeof(uint8_t), EEPROM_WRITE_TIME);

	if(halStatus != HAL_OK) //if something went wrong during writing
	{
		*memErrorFlag = 1;
		return;
	}
}

/**
 * @brief	Used to load a boolean parameter (uint8_t type)
 * @param	memAddress	Initial EEPROM location where to find data
 * @param	paramPtr	Pointer to the RAM variable where to store data
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void LoadParam_0_1(uint8_t memAddress, uint8_t* paramPtr,
		uint8_t* memErrorFlag)
{
	uint8_t temp;
	HAL_StatusTypeDef halStatus;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Try to read
	halStatus = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)&temp, sizeof(uint8_t), EEPROM_READ_TIME);

	if(halStatus != HAL_OK) //if error during reading
	{
		*memErrorFlag = 1;
		return;
	}

	*paramPtr = temp;

}


/**
 * @brief	Used to save a boolean parameter (uint8_t type)
 * @param	memAddress	Initial EEPROM location where to store data
 * @param	paramPtr	Pointer to the RAM variable where to catch data
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void SaveParam_0_1(uint8_t memAddress, uint8_t* paramPtr,
		uint8_t* memErrorFlag)
{
	HAL_StatusTypeDef halStatus;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Try to write
	halStatus = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)paramPtr, sizeof(uint16_t), EEPROM_WRITE_TIME);

	if(halStatus != HAL_OK) //if error during writing
	{
		*memErrorFlag = 1;
		return;
	}

}


/**
 * @brief	Used to load a parameter of "lightMode" type
 * @param	memAddress	Initial EEPROM location where to find data
 * @param	paramPtr	Pointer to the RAM variable where to store data
 * @param	errorMsg	String to write if not recognized enum value
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void LoadParam_LightMode(uint8_t memAddress, LightMode_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag)
{
	LightMode_t temp;
	HAL_StatusTypeDef halStatus;

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Try to read
	halStatus = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)&temp, sizeof(LightMode_t), EEPROM_READ_TIME);

	if(halStatus != HAL_OK) //if error during reading
	{
		*memErrorFlag = 1;
		return;
	}

	switch(temp)
	{
	case LightMode_DMXControlled:
	case LightMode_Manual:
		*paramPtr = temp;
		break;

	default: //if the loaded value is not a known enum
		UI_Error(errorMsg);
		break;
	}

}


/**
 * @brief	Used to save a parameter of "lightMode" type
 * @param	memAddress	Initial EEPROM location where to store data
 * @param	paramPtr	Pointer to the RAM variable where to catch data
 * @param	errorMsg	String to write if not recognized enum value
 * @param	memErrorFlag	Pointer to the I2C or EEPROM error flag
 * 							(0 = no error)
 */
void SaveParam_LightMode(uint8_t memAddress, LightMode_t* paramPtr, char* errorMsg,
		uint8_t* memErrorFlag)
{
	LightMode_t temp = *paramPtr;
	HAL_StatusTypeDef halStatus;

	/* Check if the passed data is a known enum value */
	switch(temp)
	{
	case LightMode_DMXControlled:
	case LightMode_Manual:
		break;
	default:
		UI_Error(errorMsg);
		return;
	}

	//Wait for the I2C memory to be ready (max EEPROM_TRIALS * EEPROM_TIMEOUT_MS)
	if(HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,EEPROM_TRIALS, EEPROM_TIMEOUT_MS))
	{
		*memErrorFlag = 1;
		return;
	}

	//Try to write
	halStatus = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDRESS_FIRST_HALF,
		memAddress, 1, (uint8_t*)paramPtr, sizeof(LightMode_t), EEPROM_WRITE_TIME);

	if(halStatus != HAL_OK) //if error during writing
	{
		*memErrorFlag = 1;
		return;
	}

}




