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
#include "IN12.h"
#include "menu.h"
#include "buttons.h"
#include "settings.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
Menu_HandleTypeDef 		hmenu;
Buttons_HandleTypeDef 	hbtns;

Button_InitTypeDef btns_list[] = {
		{"Left", 	BTN_1_GPIO_Port, BTN_1_Pin, GPIO_PIN_RESET},
		{"Right", 	BTN_2_GPIO_Port, BTN_2_Pin, GPIO_PIN_RESET},
		{"Enter", 	BTN_3_GPIO_Port, BTN_3_Pin, GPIO_PIN_RESET}
};
Item_TypeDef items_list[NUM_OF_ITEMS] = {
		{"FW VERSION",  			MAIN, 		NULL,     							2},
		{"COMPILE DATE",			MAIN, 		NULL,     							2},
		{"YEAR",     				MAIN, 		NULL,     							2},
		{"MONTH",     				MAIN, 		NULL,     							0},
		{"DAY",     				MAIN, 		NULL,								1},
		{"HOURS",     				MAIN, 		NULL,								1},
		{"MINUTES",     			MAIN, 		NULL,								1},
		{"SECONDS",     			MAIN, 		NULL,								1},
		{"TUBES BRIGHTNESS ADJUST", SETTINGS, 	&s_ptr[TUBES_BRIGHTNESS_ADJUST],	0},
		{"AMBIENT LIGHT SENSOR",    SETTINGS, 	&s_ptr[AMBIENT_LIGHT_SENSOR], 		1},
		{"TEMPERATURE SENSOR",     	SETTINGS, 	&s_ptr[TEMPERATURE_SENSOR], 		1},
		{"BLUETOOTH",     			SETTINGS, 	&s_ptr[BLUETOOTH], 					1},
};
Setting_TypeDef s_ptr[] = {
		// Name				Current value	  Default	Delta	  Min		Max				Need to save		  Cyclic change
		{"FW_VERSION",				.val = 0, .def = 0, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 0, .is_change_cyclic = 0},
		{"COMPILE_DATE",			.val = 0, .def = 0, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 0, .is_change_cyclic = 0},
		{"YEAR",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 2077, 	.is_need_to_save = 0, .is_change_cyclic = 1},
		{"MONTH",					.val = 0, .def = 0, .del = 1, .min = 1, . max = 12,		.is_need_to_save = 0, .is_change_cyclic = 1},
		{"DAY",						.val = 0, .def = 0, .del = 1, .min = 1, . max = 31,		.is_need_to_save = 0, .is_change_cyclic = 1},
		{"HOURS",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 23,		.is_need_to_save = 0, .is_change_cyclic = 1},
		{"MINUTES",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 59,		.is_need_to_save = 0, .is_change_cyclic = 1},
		{"SECONDS",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 59,		.is_need_to_save = 0, .is_change_cyclic = 1},
		{"TUBES_BRIGHTNESS_ADJUST",	.val = 0, .def = 0, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 1, .is_change_cyclic = 0},
		{"AMBIENT_LIGHT_SENSOR",	.val = 0, .def = 1, .del = 1, .min = 0, . max = 1, 		.is_need_to_save = 1, .is_change_cyclic = 1},
		{"TEMPERATURE_SENSOR",		.val = 0, .def = 1, .del = 1, .min = 0, . max = 1, 		.is_need_to_save = 1, .is_change_cyclic = 1},
		{"BLUETOOTH",				.val = 0, .def = 1, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 1, .is_change_cyclic = 1},
};
uint16_t settings_size = sizeof(s_ptr)/sizeof(s_ptr[0]);
uint8_t num_of_btns = sizeof(btns_list)/sizeof(btns_list[0]);








/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

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
  settings_init(s_ptr, settings_size);
  IN12_init();
  DS3231_Init(&hi2c1);

  init_menu_items(&hmenu, items_list);
  btns_init(&hbtns, btns_list, num_of_btns, htim2, PRESSED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // 7-15 Volts input
  while (1)
  {
	  time_update();
	  extern RTC_TimeTypeDef rtc_time;
	  IN12_set_digit_pairs(rtc_time.Minutes, rtc_time.Seconds);

	  HAL_GPIO_TogglePin(INS_EN_3V3_GPIO_Port, INS_EN_3V3_Pin);
	  HAL_Delay(500);
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
