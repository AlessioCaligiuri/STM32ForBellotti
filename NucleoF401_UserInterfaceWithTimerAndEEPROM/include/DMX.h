#ifndef __DMX_H
#define __DMX_H

#include "stm32f4xx_hal.h"

#define	DMX_LENGTH 512

/**
 * @brief	Enum type for DMX mode
 */
typedef enum {
	DMX_MODE_INIT,
	DMX_MODE_BREAK,
	DMX_MODE_DATA,
	DMX_MODE_ERROR,
	DMX_MODE_UNKNOWN_SC} DMX_ModeTypeDef;

extern DMX_ModeTypeDef DMX_Mode;

extern uint8_t rxBuff[DMX_LENGTH];

/**
 * @brief	DMX initialization function
 * @param  	huart: 	pointer to a UART_HandleTypeDef structure that contains
 *                	the configuration information for the specified UART module.
 * @retval 	HAL status
 */
HAL_StatusTypeDef DMX_Init(UART_HandleTypeDef *huart);
void DMX_IRQHandler(void);

#endif
