/**
  ********************************************************************************
  * @file    HD44780.h
  * @author  MCD Application Team, Alessio Caligiuri
  * @version V0.9.1
  * @date    10 September 2017
  * @brief   This file contains all functions prototypes and macros for the 
  * 		 HD44780 LCD controller.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HD44780_H
#define __HD44780_H


/* Includes ------------------------------------------------------------------*/

/** MODIFY the include directive with the correct STM32 family HAL, like
  * #include "stm32TYxx_hal.h" where T is f or l and Y is 0,1,2,3,4,.... 
  */

#include "stm32f4xx_hal.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/* HD44780 CGRAM address start */
#define CGRAM_address_start 0x40

/**
 * @brief Enum for cursor state.
 */
typedef enum CursorMode {
	Invisible_Cursor = 0x0C,
	BlinkingBlock_Cursor = 0x0F,
	Underline_Cursor = 0x0E} CursorMode_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @addtogroup HD44780_Exported_Functions
  * @{
  */
void LCD_Load_CGRAM(char tab[], uint8_t charnum);
void LCD_Backlight_On (void);
void LCD_Backlight_Off (void);
void LCD_Init(void);
void LCD_printchar(unsigned char ascode);
void LCD_printstring(unsigned char *text);
void LCD_CMD(unsigned char data);
void LCD_Clear_Display(void);
void LCD_2ndRow(void);
void LCD_3rdRow(void);
void LCD_4thRow(void);
void LCD_Home(void);
void LCD_LShift(void);
void LCD_RShift(void);
void LCD_Display_On(void);
void LCD_Display_Off(void);
void LCD_Locate(uint8_t row, uint8_t column);
void LCD_printf(const char *fmt, ...);
void LCD_CursorMode(CursorMode_t cursorMode);

#ifdef LCD_RW_Pin
#ifdef LCD_RW_GPIO_Port
uint8_t LCD_Read_BF_AC(void);
uint8_t LCD_Read_Data(void);
#endif
#endif


#endif /* __HD44780_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
