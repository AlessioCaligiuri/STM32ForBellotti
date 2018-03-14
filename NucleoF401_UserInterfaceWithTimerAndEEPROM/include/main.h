/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  * Author			   : Ale & Ema
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>	//included to use uintXX_t types

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define ENC_B_Pin GPIO_PIN_1
#define ENC_B_GPIO_Port GPIOC
#define LDB_Pin GPIO_PIN_0
#define LDB_GPIO_Port GPIOA
#define LDO_Pin GPIO_PIN_1
#define LDO_GPIO_Port GPIOA
#define USART_TX_PC_Pin GPIO_PIN_2
#define USART_TX_PC_GPIO_Port GPIOA
#define USART_RX_PC_Pin GPIO_PIN_3
#define USART_RX_PC_GPIO_Port GPIOA
#define ENC_SW_Pin GPIO_PIN_4
#define ENC_SW_GPIO_Port GPIOA
#define ENC_A_Pin GPIO_PIN_0
#define ENC_A_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_8
#define LCD_D7_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_10
#define LCD_RS_GPIO_Port GPIOA
#define LCD_E_Pin GPIO_PIN_3
#define LCD_E_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_4
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_5
#define LCD_D4_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_6
#define LCD_BL_GPIO_Port GPIOB
#define DMX_OUT_Pin GPIO_PIN_11
#define DMX_OUT_GPIO_Port GPIOA
#define DMX_IN_Pin GPIO_PIN_12
#define DMX_IN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
extern int timFlag_PeriodElapsed;
void DelayBreaked(void);
/* USER CODE END Private defines */

typedef enum {
	LightMode_DMXControlled = 0,	/*!< Used when lights are DMX controlled. 			*/
	LightMode_Manual = 1			/*!< Used when lights are driven by manual settings.*/
} LightMode_t;


/**
 * @brief	Flag to indicate full on lights mode active.
 */
extern uint8_t fullOnIsActive;

/**
 * @brief	DMX receiving channel for red LED of A stripe.
 */
extern uint16_t dmxCh_redA;

/**
 * @brief	DMX receiving channel for green LED of A stripe.
 */
extern uint16_t dmxCh_greenA;

/**
 * @brief	DMX receiving channel for blue LED of A stripe.
 */
extern uint16_t dmxCh_blueA;

/**
 * @brief	DMX receiving channel for red LED of B stripe.
 */
extern uint16_t dmxCh_redB;

/**
 * @brief	DMX receiving channel for green LED of B stripe.
 */
extern uint16_t dmxCh_greenB;

/**
 * @brief	DMX receiving channel for blue LED of B stripe.
 */
extern uint16_t dmxCh_blueB;

/**
 * @brief	Light mode
 */
extern LightMode_t lightMode;

extern uint8_t dmxThruModeIsActive;
extern uint8_t dmxCheckViaSerial_isOn;
extern uint8_t light_redA;
extern uint8_t light_greenA;
extern uint8_t light_blueA;
extern uint8_t light_redB;
extern uint8_t light_greenB;
extern uint8_t light_blueB;

extern uint8_t dmx_Rx_Data[513];

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
