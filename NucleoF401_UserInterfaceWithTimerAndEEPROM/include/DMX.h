#ifndef __DMX_H
#define __DMX_H

#include "stm32f4xx_hal.h"

#define	DMX_LENGTH 513 //512 + 1 for start code

/**
 * @brief	Enum type for DMX mode
 */
typedef enum {
	DMX_MODE_INIT = 0,
	DMX_MODE_BREAK = 1,
	DMX_MODE_DATA = 2,
	DMX_MODE_ERROR = 3,
	DMX_MODE_UNKNOWN_SC = 4} DMX_ModeTypeDef;

extern DMX_ModeTypeDef DMX_Mode;

extern uint8_t DMX_rxData[DMX_LENGTH];
extern int16_t DMX_rxData_count;

/**
 * @brief	Function to call to reset the timeout (1s) timer.
 * @note	To be implemented somewhere.
 */
extern void DMX_ResetTimer(void);

HAL_StatusTypeDef DMX_Init(UART_HandleTypeDef *huart);
void DMX_IRQHandler(void);

#endif
