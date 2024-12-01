/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PULSE_DURATION_MS						10 // Debug value 1000ms to see action
#define OUTPUT_ENABLE()							HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_RESET)
#define OUTPUT_DISABLE()						HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_SET)
#define DATA_SET(value)							HAL_GPIO_WritePin(DIN_3V3_GPIO_Port, DIN_3V3_Pin, (GPIO_PinState)(value))	// Set data output
#define SHIFT_REG_SET(value)					HAL_GPIO_WritePin(SCK_3V3_GPIO_Port, SCK_3V3_Pin, (GPIO_PinState)(value))	// Set shift register
#define STORAGE_REG_SET(value)					HAL_GPIO_WritePin(RCK_3V3_GPIO_Port, RCK_3V3_Pin, (GPIO_PinState)(value))	// Set storage register
#define CONV_DIGIT_TO_SR_BITMASK_LSB(digit)  	((uint16_t)(1 << digit) & 0b000000001)
#define NUM_OF_DIGITS 							10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart2;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
//typedef enum {
//	ZERO 	= 0,
//	ONE 	= 0b100000000,
//	TWO		= 0b010000000,
//	THREE	= 0b001000000,
//	FOUR	= 0b000100000,
//	FIVE	= 0b000010000,
//	SIX		= 0b000001000,
//	SEVEN	= 0b000000100,
//	EIGHT	= 0b000000010,
//	NINE	= 0b000000001
//} NumberBitsMSB;

uint16_t digit_to_bits[10] = {
	0b000000000,
	0b100000000,
	0b010000000,
	0b001000000,
	0b000100000,
	0b000010000,
	0b000001000,
	0b000000100,
	0b000000010,
	0b000000001,
};

typedef struct {
	uint16_t reversed;
	uint16_t normal;
} DigitBits_TypeDef;

DigitBits_TypeDef digit_to_shift_reg_data[NUM_OF_DIGITS] = {
		{0b000000000, 0b000000000},	// 0
		{0b000000001, 0b100000000},	// 1
		{0b000000010, 0b010000000},	// 2
		{0b000000100, 0b001000000},	// 3
		{0b000001000, 0b000100000},	// 4
		{0b000010000, 0b000010000},	// 5
		{0b000100000, 0b000001000},	// 6
		{0b001000000, 0b000000100},	// 7
		{0b010000000, 0b000000010},	// 8
		{0b100000000, 0b000000001},	// 9
};
//   	#1				#2				#3			#4
//   0123 4567 89   0123 4567 89   0123 4567 89   0123 4567 89
// 0b1000 0000 00 0b0100 0000 00 0b0010 0000 00 0b0001 0000 00
uint16_t n[4] = {1 << 9, 1 << 8, 1 << 7, 1 << 6};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
static void shift_reg_pulse_srclk(uint32_t delay_ms);
static void shift_reg_send(uint16_t data);
static void nixie_test(uint16_t number);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_LPUART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  OUTPUT_DISABLE();
  STORAGE_REG_SET(0);
  SHIFT_REG_SET(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  srand(time(NULL));   // Initialization, should only be called once.
  uint16_t random_number = 0;      // Returns a pseudo-random integer between 0 and RAND_MAX.
  uint16_t number = 0;      // Returns a pseudo-random integer between 0 and RAND_MAX.

  HAL_Delay(1000);
  while (1)
  {
	  // RESET
	  OUTPUT_DISABLE();

//	  for (uint8_t i = 0, led = 0; i < 10; ++i) {
//		  shift_reg_send(i);
//
//		  HAL_GPIO_TogglePin(INS_EN_3V3_GPIO_Port, INS_EN_3V3_Pin);
//		  HAL_GPIO_TogglePin(DIMM_LED_1_GPIO_Port, DIMM_LED_1_Pin);
//		  HAL_GPIO_TogglePin(DIMM_LED_2_GPIO_Port, DIMM_LED_2_Pin);
//		  HAL_Delay(1000);
//	  }

	  random_number = rand();
	  number = 1234;
	  nixie_test(number);
	  HAL_Delay(10);
	  OUTPUT_ENABLE();

	  HAL_Delay(1000);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 209700;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_7B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DIMM_LED_1_Pin|DIMM_LED_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TXS_OE_GPIO_Port, TXS_OE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SHDN_170V_3V3_Pin|INS_EN_3V3_Pin|DIN_3V3_Pin|OE_3V3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RCK_3V3_Pin|SCK_3V3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DIMM_LED_1_Pin DIMM_LED_2_Pin */
  GPIO_InitStruct.Pin = DIMM_LED_1_Pin|DIMM_LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_4_Pin INT_SQW_Pin INT_OPT_Pin JDY_STAT_Pin
                           JDY_PWRC_Pin */
  GPIO_InitStruct.Pin = BTN_4_Pin|INT_SQW_Pin|INT_OPT_Pin|JDY_STAT_Pin
                          |JDY_PWRC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TXS_OE_Pin */
  GPIO_InitStruct.Pin = TXS_OE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TXS_OE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SHDN_170V_3V3_Pin */
  GPIO_InitStruct.Pin = SHDN_170V_3V3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SHDN_170V_3V3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : INS_EN_3V3_Pin DIN_3V3_Pin OE_3V3_Pin */
  GPIO_InitStruct.Pin = INS_EN_3V3_Pin|DIN_3V3_Pin|OE_3V3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RCK_3V3_Pin SCK_3V3_Pin */
  GPIO_InitStruct.Pin = RCK_3V3_Pin|SCK_3V3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_3_Pin BTN_2_Pin BTN_1_Pin */
  GPIO_InitStruct.Pin = BTN_3_Pin|BTN_2_Pin|BTN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void shift_reg_pulse_srclk(uint32_t delay_ms) {
	SHIFT_REG_SET(1);
	HAL_Delay(delay_ms);
	SHIFT_REG_SET(0);
	HAL_Delay(delay_ms);
}
static void shift_reg_pulse_rclk(uint32_t delay_ms) {
	STORAGE_REG_SET(1);
	HAL_Delay(delay_ms);
	STORAGE_REG_SET(0);
	HAL_Delay(delay_ms);
}



// numbers[0] = MSB aka "hours > 9"

//   	#1				#2				#3			#4
//   0123 4567 89   0123 4567 89   0123 4567 89   0123 4567 89
// 0b1000 0000 00 0b0100 0000 00 0b0010 0000 00 0b0001 0000 00

// data size is 16 bit, but only 10 are actually connected to a nixie lamp
static void shift_reg_send(uint16_t data) {
//	/*
//	 * Disable latch
//	 */
//	STORAGE_REG_SET(0);

	/*
	 * Set SER pin according to currently transmitted bit
	 * Toggle clock n times to set all N bits
	 */
	for (uint8_t bit = 15; bit >= 0; --bit) {
		DATA_SET((data >> bit) & 1);					// Send least significant bit first
		shift_reg_pulse_srclk(PULSE_DURATION_MS);
	}

//	/*
//	 * Set latch
//	 */
//	STORAGE_REG_SET(1);
}

static void nixie_show_time(uint32_t h, uint32_t m, uint32_t s) {

}
static void nixie_test(uint16_t number) {
	//   	#1				#2				#3			#4
	//   0123 4567 89   0123 4567 89   0123 4567 89   0123 4567 89
	// 0b1000 0000 00 0b0100 0000 00 0b0010 0000 00 0b0001 0000 00


	uint16_t digit_data[4] = {
			digit_to_shift_reg_data[	(number/1000)	%10000	].reversed,
			digit_to_shift_reg_data[	(number/100)	%1000	].reversed,
			digit_to_shift_reg_data[	(number/10)		%100	].reversed,
			digit_to_shift_reg_data[	(number/1)		%10		].reversed,
	};

	/*
	 * Disable latch
	 */
//	STORAGE_REG_SET(0);

	/*
	 * Send data
	 */
	for (int digit = 0; digit < 4; ++digit) {
		shift_reg_send(digit_data[digit]);
	}
//		n[i] = n[i] >> 1;
//		if (n[i] == 0)
//			n[i] = 1 << 9;

	/*
	 * Set latch
	 */
//	STORAGE_REG_SET(1);

	shift_reg_pulse_rclk(PULSE_DURATION_MS);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
