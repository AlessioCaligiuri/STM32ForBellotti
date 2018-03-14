#include "stm32f4xx_hal.h"
#include "DMX.h"



/**
 * @brief	DMX mode.
 */
DMX_ModeTypeDef DMX_Mode;

int16_t rxCounter = -1;

uint8_t rxBuff[DMX_LENGTH];

/**
 * Pointer to the instance of UART used for DMX.
 */
UART_HandleTypeDef *DMX_huart_ptr;

/**
 * @brief	DMX initialization function
 * @param  	huart: 	pointer to a UART_HandleTypeDef structure that contains
 *                	the configuration information for the specified UART module.
 * @retval 	HAL status
 */
HAL_StatusTypeDef DMX_Init(UART_HandleTypeDef *huart)
{
	uint16_t i; //counter used to reset the rx buffer
	HAL_StatusTypeDef hs = HAL_LIN_Init(huart, UART_LINBREAKDETECTLENGTH_11B);
	DMX_huart_ptr = huart;
	DMX_Mode = DMX_MODE_INIT;
	SET_BIT(huart->Instance->CR1, USART_CR1_RXNEIE); //enable Interrupt on RX
	SET_BIT(huart->Instance->CR2, USART_CR2_LBDIE);  //enable Interrupt on Break

	//reset rx buffer
	for(i=0; i < DMX_LENGTH; i++)
	{
		rxBuff[DMX_LENGTH] = 0;
	}

	return hs; //return HAL status
}

/**
 * @brief	Interrupt Service Routine for DMX.
 * 			To call in USARTx_IRQHandler().
 */
void DMX_IRQHandler(void)
{
	/* Received byte (buffer) */
	uint8_t rxByte = 0;

	/* Get registers from the UART peripheral */
	uint32_t isrflags   = READ_REG(DMX_huart_ptr->Instance->SR);
	uint32_t cr1its     = READ_REG(DMX_huart_ptr->Instance->CR1);
	uint32_t cr2its     = READ_REG(DMX_huart_ptr->Instance->CR2);
	uint32_t errorflags = 0x00U;

	/* RX NOT EMPTY:
	 * it is cleared by a read to the USART_DR register.
	 */

	/* If byte received */
	if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
	{
		rxByte = (uint8_t)(DMX_huart_ptr->Instance->DR & (uint8_t)0x00FF);
	}

	/* OVERRUN ERROR, FRAMING ERROR, NOISE ERROR
	 * These bits are cleared by a software sequence: a read to the
	 * USART_SR register followed by a read to the USART_DR register.
	 */

	/* If break detected */
	if(((isrflags & USART_SR_LBD) != RESET) && ((cr2its & USART_CR2_LBDIE) != RESET))
	{
		DMX_Mode = DMX_MODE_BREAK;
		CLEAR_BIT(DMX_huart_ptr->Instance->SR, USART_SR_LBD); //reset LBD flag
		rxCounter = 0; //reset data counter
		return;
	}

	/* If error (Framing, Overrun, Noise) occurs */
	errorflags = (isrflags & (uint32_t)(USART_SR_FE | USART_SR_ORE | USART_SR_NE));
	if(errorflags != RESET)
	{
		DMX_Mode = DMX_MODE_ERROR;
		return;
	}
	/* ATTENTION! Framing error occours also when there is a Break!
	 */



	switch(DMX_Mode)
	{
	case DMX_MODE_BREAK: //check start code
		if(rxByte == 0) // SC == 0?
		{
			DMX_Mode = DMX_MODE_DATA;
			rxBuff[0] = 0;
		}
		else
		{
			DMX_Mode = DMX_MODE_UNKNOWN_SC;
		}
		break;
	case DMX_MODE_DATA:
		rxCounter++;
		rxBuff[rxCounter] = rxByte;
		break;
	default:
		break;
	}

}
