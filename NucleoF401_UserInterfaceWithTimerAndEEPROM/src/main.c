/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "DMX.h"
#include <string.h>

/* USER CODE BEGIN Includes */
#include "userInterface.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint16_t dmxCh_redA = 1;
uint16_t dmxCh_greenA = 2;
uint16_t dmxCh_blueA = 3;
uint16_t dmxCh_redB = 4;
uint16_t dmxCh_greenB = 5;
uint16_t dmxCh_blueB = 6;
uint8_t fullOnIsActive = 0;
uint8_t dmxThruModeIsActive = 1;
LightMode_t lightMode = LightMode_DMXControlled;

/**
 * @brief	Light of red A in manual mode.
 */
uint8_t light_redA;

/**
 * @brief	Light of green A in manual mode.
 */
uint8_t light_greenA;

/**
 * @brief	Light of blue A in manual mode.
 */
uint8_t light_blueA;

/**
 * @brief	Light of red B in manual mode.
 */
uint8_t light_redB;

/**
 * @brief	Light of green B in manual mode.
 */
uint8_t light_greenB;

/**
 * @brief	Light of blue B in manual mode.
 */
uint8_t light_blueB;

uint8_t dmxCheckViaSerial_isOn = 0;

int timerCounterDMX = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
void Serial_SendDMXDataToPC(void);
void PWM_Update(void);
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int timFlag_PeriodElapsed = 0;
/* USER CODE END 0 */

int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init(); //UART for PC (115200bps)
	MX_USART6_UART_Init(); //UART for DMX (250kbps)
	MX_I2C1_Init();
	MX_TIM3_Init();
	MX_TIM1_Init();
	MX_TIM10_Init();

	/* USER CODE BEGIN 2 */

	/* Initialize DMX */
	if (DMX_Init(&huart6) != HAL_OK)
	{
	Error_Handler();
	}

	/* Initialize User Interface */
	UI_Init();

	/* START PWMs TIMERS */
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	/* Start Timer 10, used for PWM update, serial to PC and DMX timeout */
	HAL_TIM_Base_Start_IT(&htim10);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	  UI_Update();
	/* USER CODE END WHILE */
	/* USER CODE BEGIN 3 */

	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/**
 * @brief 	ISR for GPIO interrupt.
 * 			Set encoder flags.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ENC_SW_Pin)
	{
		encoderFlag_ButtonIsPressed = 1;
	}
	if(GPIO_Pin == ENC_A_Pin)
	{
		if(HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin)) //if(ENC_B==SET)
		{
			encoderRotationCount--;
		}
		else //if(ENC_B==RESET)
		{
			encoderRotationCount++;
		}
	}
}

/**
 * @brief	Sends DMX received values to PC over serial (115200 baud).
 */
void Serial_SendDMXDataToPC(void)
{
	const char alignCursor[] = "\033[0;0H";
	const char initialClean[] = "\033[2J";
	int i = 0;
	char toSend[60];
	uint16_t toSendDim;

	/* Clear terminal */

	HAL_UART_Transmit(&huart2, (uint8_t*)alignCursor, strlen(alignCursor), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, (uint8_t*)initialClean, strlen(initialClean), HAL_MAX_DELAY);

//	HAL_UART_Transmit_IT(&huart2, (uint8_t*)alignCursor, strlen(alignCursor));
//	HAL_UART_Transmit_IT(&huart2, (uint8_t*)initialClean, strlen(initialClean));

//	if(DMX_rxData_count < 0) //if no packet received
//	{
//		toSendDim = sprintf(toSend,"* No DMX packet received. DMX_Mode = %d *",
//				DMX_Mode);
//		HAL_UART_Transmit(&huart2, (uint8_t*)toSend, toSendDim, HAL_MAX_DELAY);
//		//HAL_UART_Transmit_IT(&huart2, (uint8_t*)toSend, toSendDim);
//	}
//	else
	{
		/* Print start code */
		toSendDim = sprintf(toSend,"Start code\t%d\r\n", DMX_rxData[0]);
		HAL_UART_Transmit(&huart2, (uint8_t*)toSend, toSendDim, HAL_MAX_DELAY);
		//HAL_UART_Transmit_IT(&huart2, (uint8_t*)toSend, toSendDim);
		//if(DMX_rxData_count > 0)
		if(1)
		{
			/* Print channel values */
//			for(i = 1; i<=DMX_rxData_count; i++ )
			for(i = 1; i<=24; i++ )
			{
				  toSendDim = sprintf(toSend,"Channel %d\t%d\r\n",i,DMX_rxData[i]);
				  HAL_UART_Transmit(&huart2, (uint8_t*)toSend, toSendDim, HAL_MAX_DELAY);
				  //HAL_UART_Transmit_IT(&huart2, (uint8_t*)toSend, toSendDim);
			}
		}
	}
}

/**
 * @brief	Updates PWMs values, linearizing human light perception.
 * On the basis of chosen light mode, uses DMX values or "manual light" values.
 */
void PWM_Update(void)
{
	/* LUT for DMX to PWM exponential conversion, in order to linearize human
	 * light perception */
	const uint16_t dmxToPwm[256]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13,
			14, 15, 16, 17, 18, 20, 21, 22, 23, 24, 26, 27, 28, 29, 31, 32,
			33, 35, 36, 37, 39, 40, 41, 43, 44, 45, 47, 48, 50, 51, 53, 54,
			56, 57, 59, 60, 62, 63, 65, 67, 68, 70, 71, 73, 75, 76, 78, 80,
			82, 83, 85, 87, 89, 91, 92, 94, 96, 98, 100, 102, 104, 106, 108,
			110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 133, 135,
			137, 139, 142, 144, 146, 149, 151, 153, 156, 158, 161, 163, 165,
			168, 171, 173, 176, 178, 181, 184, 186, 189, 192, 194, 197, 200,
			203, 206, 208, 211, 214, 217, 220, 223, 226, 229, 232, 236, 239,
	        242, 245, 248, 252, 255, 258, 261, 265, 268, 272, 275, 279, 282,
			286, 289, 293, 297, 300, 304, 308, 312, 316, 319, 323, 327, 331,
			335, 339, 343, 347, 352, 356, 360, 364, 369, 373, 377, 382, 386,
			391, 395, 400, 405, 409, 414, 419, 424, 428, 433, 438, 443, 448,
			453, 458, 464, 469, 474, 479, 485, 490, 496, 501, 507, 512, 518,
			524, 529, 535, 541, 547, 553, 559, 565, 571, 577, 584, 590, 596,
			603, 609, 616, 622, 629, 636, 642, 649, 656, 663, 670, 677, 684,
			692, 699, 706, 714, 721, 729, 736, 744, 752, 760, 768, 775, 784,
			792, 800, 808, 816, 825, 833, 842, 851, 859, 868, 877, 886, 895,
			904, 913, 923, 932, 941, 951, 961, 970, 980, 990, 999};


	switch(lightMode)
	{
	case LightMode_DMXControlled:
		/* Copy data from DMX to PWMs */
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dmxToPwm[DMX_rxData[dmxCh_redA]]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dmxToPwm[DMX_rxData[dmxCh_greenA]]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dmxToPwm[DMX_rxData[dmxCh_blueA]]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, dmxToPwm[DMX_rxData[dmxCh_redB]]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dmxToPwm[DMX_rxData[dmxCh_greenB]]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dmxToPwm[DMX_rxData[dmxCh_blueB]]);
		break;
	case LightMode_Manual:
		/* Copy data from manual lights to PWMs */
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dmxToPwm[light_redA]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dmxToPwm[light_greenA]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dmxToPwm[light_blueA]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, dmxToPwm[light_redB]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dmxToPwm[light_greenB]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dmxToPwm[light_blueB]);
		break;
	default:
		break;
	}
}

/**
 * @brief	Called when a timer expires.
 * For TIM10 (20ms period), update PWMs, increment the DMX timeout counter,
 * transmit DMX data to PC over serial every 1s.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int timerCounterSerial = 0; //counter for serial PC transmission timing

	if(htim->Instance == TIM10) //20ms Timer
	{
		PWM_Update();

		/* DMX Timeout */
		if(DMX_Mode != DMX_MODE_INIT)
		{
			timerCounterDMX++;
			if(timerCounterDMX >= 50) //1s elapsed: reset DMX state
			{
				DMX_Mode = DMX_MODE_INIT;
				DMX_rxData_count = -1;
				timerCounterDMX = 0;
			}
		}

		/* Periodic send of DMX data to PC over serial */
		if(dmxCheckViaSerial_isOn)
		{
			timerCounterSerial++;
			if(timerCounterSerial >= 5) //100ms elapsed
			{
				Serial_SendDMXDataToPC();
				//HAL_Delay(10);
				timerCounterSerial = 0;
			}
		}
	}
}

/**
 * @brief	Resets DMX timeout timer.
 */
void DMX_ResetTimer(void)
{
	timerCounterDMX = 0;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
