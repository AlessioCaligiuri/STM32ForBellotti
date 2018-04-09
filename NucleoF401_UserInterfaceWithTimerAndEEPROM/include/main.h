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
//#define TMS_Pin GPIO_PIN_13
//#define TMS_GPIO_Port GPIOA
//#define TCK_Pin GPIO_PIN_14
//#define TCK_GPIO_Port GPIOA
//#define SWO_Pin GPIO_PIN_3
//#define SWO_GPIO_Port GPIOB

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

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
