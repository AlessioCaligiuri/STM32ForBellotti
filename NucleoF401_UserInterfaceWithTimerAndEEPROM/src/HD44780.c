/**
  ******************************************************************************
  * @file    HD44780.c
  * @author  MCD Application Team, Alessio Caligiuri
  * @version V0.9.1
  * @date    10 September 2017
  * @brief   This file contains all function implementations for the HD44780
  * 		 LCD controller.
  *			 The display is intended to be used with 4-bit interface 
  *          (D4, D5, D6, D7 pins) and E, RS pins.
  *			 If the pin RW is not used (no read operations required), it must be
  *			 connected to the GND.
   ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * FOR MORE INFORMATION PLEASE READ CAREFULLY THE LICENSE AGREEMENT FILE
  * LOCATED IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  *
  * This library is modded by Alessio Caligiuri in order to use STM32 HAL 
  * libraries and to allow the usage of different pins for every communication 
  * line. Moreover, 20x4 LCD display is now supported and if R/W pin is used, 
  * the LCD is now readable.
  *	Functions for backlight management are included.
  * 
  * USAGE: Pin names refers to main.h, where the user has to define:
  * #define LCD_D4_Pin 
  * #define LCD_D4_GPIO_Port 
  * #define LCD_D5_Pin
  * #define LCD_D5_GPIO_Port
  * #define LCD_D6_Pin
  * #define LCD_D6_GPIO_Port
  * #define LCD_D7_Pin
  * #define LCD_D7_GPIO_Port
  *
  * #define LCD_BL_Pin
  * #define LCD_BL_GPIO_Port
  * #define LCD_RS_Pin
  * #define LCD_RS_GPIO_Port
  * #define LCD_E_Pin
  * #define LCD_E_GPIO_Port
  *
  * and IF AND ONLY IF R/W pin is used:
  * #define LCD_RW_Pin
  * #define LCD_RW_GPIO_Port
  *
  * This pin and port names definitions are easy to implement by STM32 CubeMX,
  * simply naming pins as LCD_D4 ... LCD_D7, LCD_BL, LCD_RS, LCD_E, 
  * and, if used, LCD_RW .
  *
  * The user have also to modify the #include directive with the correct STM32 
  * family HAL header.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "HD44780.h"
#include "main.h" //header where find ports and pins definitions
#include "stdarg.h"
#include "stdio.h"

/** MODIFY the include directive with the correct STM32 family HAL, like
  * #include "stm32TYxx_hal.h" where T is f or l and Y is 0,1,2,3,4,.... 
  */
#include "stm32f4xx_hal.h"

/** @attention
  * LCD_DataPins_As_Input() and LCD_DataPins_As_Output() functions are used to
  * change the GPIO pins directions, in order to read the LCD. These functions
  * are used IF AND ONLY IF the RW pin is used. */
#ifdef LCD_RW_Pin
#ifdef LCD_RW_GPIO_Port
/**
 * @brief Used in "read" functions to configure GPIO Data Pins (D7 -- D4) as inputs.
 */
void LCD_DataPins_As_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pin */
	GPIO_InitStruct.Pin = LCD_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D7_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_D6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D6_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_D5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D5_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D4_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief Used in "read" functions to re-configure GPIO Data Pins (D7 -- D4) as outputs.
 */
void LCD_DataPins_As_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*Configure GPIO pin */
	GPIO_InitStruct.Pin = LCD_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D7_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_D6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D6_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_D5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D5_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D4_GPIO_Port, &GPIO_InitStruct);
}
#endif
#endif

/**
  * @addtogroup HD44780_Functions
  * @{
  */

/**
  * @brief  Initializes Character Generator CGRAM with custom characters data
  * 		CGRAM contains at most 8 custom characters (in 5x8 dots mode).
  * 		DDRAM address is after set to 0x00.
  * 		Each character can be printed by calling LCD_printchar(0 --- 7).
  * @param  Array containing characters definition values, made by 8-bytes blocks.
  * 		Each 8-bytes block is a character, where each byte is a row of the character.
  * 		A row byte must be in the range 0x00 (all dots off) to 0x1F (all dots on).
  * @param  Number of characters defined in the table (from 1 to 7)
  * @retval None
  */
void LCD_Load_CGRAM(char tab[], uint8_t charNum)
{
  uint8_t index;

  /* Each character contains 8 definition values,
   * because each character is a 8-bytes block. */
  charNum = charNum * 8;

  /* Set CGRAM initial address */
  LCD_CMD(CGRAM_address_start);

  for (index = 0; index < charNum; index++)
  {
    /* Store values in LCD CGRAM */
    LCD_printchar(tab[index]);
    HAL_Delay(1); //1 millisecond delay
  }

  /* Set DDRAM initial address */
  LCD_CMD(0x80);
}

/**
  * @brief  Activate Pin that supplies LCD backlight
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_Backlight_On(void)
{
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Disable Pin that supplies LCD backlight
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_Backlight_Off (void)
{
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Activate Enable Pin of LCD, then disable it after 2ms
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_ENABLE (void)
{
  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
  HAL_Delay(2); //2ms delay
  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Command data sent to LCD module
  * @param  command value to be sent
  */
void LCD_CMD(unsigned char cmd_data)
{
	/* RS=0 => Control mode */
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

	/* Put binary data on output pins */
	/* Most significant nibble */
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, cmd_data & 0x80); //bit 7 --> D7 (MSB)
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, cmd_data & 0x40); //bit 6 --> D6
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, cmd_data & 0x20); //bit 5 --> D5
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, cmd_data & 0x10); //bit 4 --> D4    
	LCD_ENABLE();
	/* Least significant nibble */
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, cmd_data & 0x08); //bit 3 --> D7 (MSB)
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, cmd_data & 0x04); //bit 2 --> D6
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, cmd_data & 0x02); //bit 1 --> D5
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, cmd_data & 0x01); //bit 0 --> D4
	LCD_ENABLE();

	HAL_Delay(2); //2ms delay
}

/**
  * @brief  Initializes HD44780 LCD module in 4-bit mode.
			Display is set to ON, cursor is OFF
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_Init(void)
{

	/* If Read/Write pin is used, hold it at GND during initialization */
	#ifdef LCD_RW_Pin
	#ifdef LCD_RW_GPIO_Port
		HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	#endif
	#endif

	HAL_Delay(15); //Wait 15ms to get stable VCC 5V

	//Initialization of HD44780-based LCD (4-bit HW)
	LCD_CMD(0x33);
	HAL_Delay(4);
	LCD_CMD(0x32);
	HAL_Delay(4);
	//Function Set 4-bit mode, 2 lines, 5x7 dots
	LCD_CMD(0x28);
	//Display On, Cursor Off Control
	LCD_CMD(0x0C);
	//Entry mode set, Increment
	LCD_CMD(0x06);
	LCD_Clear_Display();
	//Minimum delay to wait before driving LCD module
	HAL_Delay(200);
}

/**
  * @brief  Print Character on LCD module
  * @param  Ascii value of character
  * @param  None
  * @retval None
  */
void LCD_printchar(unsigned char ascode)
{
	/* RS=1 => Data mode */
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	/* Put binary data on output pins */
	/* Most significant nibble */
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, ascode & 0x80); //bit 7 --> D7 (MSB)
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, ascode & 0x40); //bit 6 --> D6
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, ascode & 0x20); //bit 5 --> D5
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, ascode & 0x10); //bit 4 --> D4
	LCD_ENABLE();
	/* Least significant nibble */
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, ascode & 0x08); //bit 3 --> D7
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, ascode & 0x04); //bit 2 --> D6
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, ascode & 0x02); //bit 1 --> D5
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, ascode & 0x01); //bit 0 (LSB) --> D4
	LCD_ENABLE();
	
	HAL_Delay(2); //2ms delay
}

/**
  * @brief  Display of a characters string,
			until a "new line" or "string termination" character is found.
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
void LCD_printstring(unsigned char *text)
{
	do
	{
	LCD_printchar(*text++);
	}
	while ((*text != '\n') && (*text != 0));
}

/**
  * @brief  Clear LCD module display
  */
void LCD_Clear_Display(void)
{
	LCD_CMD(0x01);
	HAL_Delay(2);
}

/**
  * @brief  Set Cursor on second row 1st digit
  */
void LCD_2ndRow(void)
{
	LCD_CMD(0xC0);
}

/**
  * @brief  Set Cursor on third row 1st digit (only for 20x4 LCD)
  */
void LCD_3rdRow(void)
{
	LCD_CMD(0x94);
}

/**
  * @brief  Set Cursor on forth row 1st digit (only for 20x4 LCD)
  */
void LCD_4thRow(void)
{
	LCD_CMD(0xD4);
}

/**
  * @brief  Set Cursor to Home position
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_Home(void)
{
	LCD_CMD(0x02);
	HAL_Delay(2);
}

/**
  * @brief  Shift display to left
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_LShift(void)  
{
	LCD_CMD(0x18);
}

/**
  * @brief  Shift display to right
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
void LCD_RShift(void)
{
	LCD_CMD(0x1C);
}

/**
  * @brief  Set Display On
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_Display_On(void)
{
	LCD_CMD(0x0C);
}

/**
  * @brief  Set Display Off
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_Display_Off(void)
{
	LCD_CMD(0x08);
}

/**
  * @brief  Set Cursor to a specified location given by row and column information
  * @param  Row Number (1 to 2 for 16x2 LCD, or 1 to 4 for 20x4 LCD)
  * @param  Column Number (1 to 16 for 16x2 LCD, or 1 to 20 for 20x4 LCD)
  * @retval None
  */
void LCD_Locate(uint8_t row, uint8_t column)
{
	column--;
	/* To set the command "DDRAM Address" the MSB must be 1, so the 0x80 or */
	switch (row)
	{
	case 1:
		/* Set cursor to 1st row address and add index */
		LCD_CMD(column | 0x80);
		break;
	case 2:
		/* Set cursor to 2nd row address and add index */
		LCD_CMD(column | 0x40 | 0x80);
		break;
	case 3:
		/* Set cursor to 3rd row address and add index */
		LCD_CMD((column + 0x14) | 0x80);
		break;
	case 4:
		/* Set cursor to 4th row address and add index */
		LCD_CMD((column + 0x54) | 0x80);
		break;
	default:
	  break;
	}
}

/**
  * @brief  LCD printf function
  * @param  string with standard defined formats
  * @param
  * @retval None
  */
void LCD_printf(const char *fmt, ...)
{
	int32_t i; //counter

	uint32_t text_size, letter;

	uint8_t currentRow = 1; //current row (1,2,3,4)

	static char text_buffer[32]; //formatted string to print
	va_list args;

	va_start(args, fmt);
	text_size = vsprintf(text_buffer, fmt, args);

	// Process the string
	for (i = 0; i < text_size; i++)
	{
		letter = text_buffer[i];

		/* If new line character is found */
		if (letter == '\n')
		{
			switch(currentRow)
			{
			case 1:
				LCD_2ndRow();
				currentRow++;
				break;
			case 2:
				LCD_3rdRow();
				currentRow++;
				break;
			case 3:
				LCD_4thRow();
				currentRow++;
				break;
			case 4:
			default:
				return;
			}
		}
		else
		{
			/* If the current letter is a printable character */
			if ((letter > 0x1F) && (letter < 0x80))
			LCD_printchar(letter);
		}
	}
}


/**
 * @brief	Update cursor mode.
 * @param	Desired cursor mode, of @ref CursorMode_t type
 */
void LCD_CursorMode(CursorMode_t cursorMode)
{
	LCD_CMD(cursorMode);
}

/**
 * @attention	LCD_Read functions are available only if RW pin is used.
 */
#ifdef LCD_RW_Pin
#ifdef LCD_RW_GPIO_Port
/**
 * @brief	Read busy flag and address counter
 * @retval	an 8-bit register containing |BF|     AC      |
 */
uint8_t LCD_Read_BF_AC(void)
{
	uint8_t readData = 0;

	LCD_DataPins_As_Input();

	//RW to 1 (read mode)
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_SET);

	//RS to 0 (register selection)
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

	//Enable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);

	//Read data (Most significant nibble)
	if(HAL_GPIO_ReadPin(LCD_D7_GPIO_Port, LCD_D7_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b10000000;
	}
	if(HAL_GPIO_ReadPin(LCD_D6_GPIO_Port, LCD_D6_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b01000000;
	}
	if(HAL_GPIO_ReadPin(LCD_D5_GPIO_Port, LCD_D5_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00100000;
	}
	if(HAL_GPIO_ReadPin(LCD_D4_GPIO_Port, LCD_D4_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00010000;
	}

	//Disable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);

	HAL_Delay(2);

	//Enable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);

	//Read data (last significant nibble)
	if(HAL_GPIO_ReadPin(LCD_D7_GPIO_Port, LCD_D7_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00001000;
	}
	if(HAL_GPIO_ReadPin(LCD_D6_GPIO_Port, LCD_D6_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00000100;
	}
	if(HAL_GPIO_ReadPin(LCD_D5_GPIO_Port, LCD_D5_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00000010;
	}
	if(HAL_GPIO_ReadPin(LCD_D4_GPIO_Port, LCD_D4_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00000001;
	}

	//Disable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);

	//RW to 0
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);

	LCD_DataPins_As_Output();

	return readData;
}

/**
 * @brief	Read data from display, after DDRAM or CGRAM address setting
 * @retval	Read data from DDRAM or CGRAM
 */
uint8_t LCD_Read_Data(void)
{
	uint8_t readData = 0;

	LCD_DataPins_As_Input();

	//RW to 1 (read mode)
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_SET);

	//RS to 1 (data selection)
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	//Enable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);

	//Read data (Most significant nibble)
	if(HAL_GPIO_ReadPin(LCD_D7_GPIO_Port, LCD_D7_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b10000000;
	}
	if(HAL_GPIO_ReadPin(LCD_D6_GPIO_Port, LCD_D6_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b01000000;
	}
	if(HAL_GPIO_ReadPin(LCD_D5_GPIO_Port, LCD_D5_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00100000;
	}
	if(HAL_GPIO_ReadPin(LCD_D4_GPIO_Port, LCD_D4_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00010000;
	}

	//Disable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);

	HAL_Delay(2);

	//Enable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);

	//Read data (last significant nibble)
	if(HAL_GPIO_ReadPin(LCD_D7_GPIO_Port, LCD_D7_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00001000;
	}
	if(HAL_GPIO_ReadPin(LCD_D6_GPIO_Port, LCD_D6_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00000100;
	}
	if(HAL_GPIO_ReadPin(LCD_D5_GPIO_Port, LCD_D5_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00000010;
	}
	if(HAL_GPIO_ReadPin(LCD_D4_GPIO_Port, LCD_D4_Pin) == GPIO_PIN_SET)
	{
		readData = readData | 0b00000001;
	}

	//Disable
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);

	//RW to 0
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);

	LCD_DataPins_As_Output();

	return readData;
}
#endif
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
