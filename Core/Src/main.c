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
#include "temperature_sensor.h"
#include "opt3001.h"
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

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim21;
DMA_HandleTypeDef hdma_tim2_ch1;
DMA_HandleTypeDef hdma_tim2_ch2;
DMA_HandleTypeDef hdma_tim2_ch3;
DMA_HandleTypeDef hdma_tim2_ch4;

/* USER CODE BEGIN PV */
extern RTC_TimeTypeDef rtc_time;

Menu_HandleTypeDef 		hmenu;
Buttons_HandleTypeDef 	hbtns;
volatile float t = 0;
volatile float lux = 0;
Button_InitTypeDef btns_list[] = {
		{"Left", 	BTN_1_GPIO_Port, BTN_1_Pin, GPIO_PIN_RESET},
		{"Enter", 	BTN_2_GPIO_Port, BTN_2_Pin, GPIO_PIN_RESET},
		{"Right", 	BTN_3_GPIO_Port, BTN_3_Pin, GPIO_PIN_RESET},
};
Setting_TypeDef s_ptr[] = {
		// Name				Current value	  Default	Delta	  Min		Max				Need to save		  Cyclic change
		//		{"FW_VERSION",				.val = 0, .def = 0, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 0, .is_change_cyclic = 0},
		//		{"COMPILE_DATE",			.val = 0, .def = 0, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 0, .is_change_cyclic = 0},
		//		{"YEAR",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 2077, 	.is_need_to_save = 0, .is_change_cyclic = 1},
		//		{"MONTH",					.val = 0, .def = 0, .del = 1, .min = 1, . max = 12,		.is_need_to_save = 0, .is_change_cyclic = 1},
		//		{"DAY",						.val = 0, .def = 0, .del = 1, .min = 1, . max = 31,		.is_need_to_save = 0, .is_change_cyclic = 1},
		//		{"HOURS",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 23,		.is_need_to_save = 0, .is_change_cyclic = 1},
		//		{"MINUTES",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 59,		.is_need_to_save = 0, .is_change_cyclic = 1},
		//		{"SECONDS",					.val = 0, .def = 0, .del = 1, .min = 0, . max = 59,		.is_need_to_save = 0, .is_change_cyclic = 1},
		{"BRIGHTNESS",				.val = 0, .def = 0, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 1, .is_change_cyclic = 0},
		{"AMBIENT_LIGHT_SENSOR",	.val = 0, .def = 1, .del = 1, .min = 0, . max = 1, 		.is_need_to_save = 1, .is_change_cyclic = 1},
		{"TEMPERATURE_SENSOR",		.val = 0, .def = 1, .del = 1, .min = 0, . max = 1, 		.is_need_to_save = 1, .is_change_cyclic = 1},
		//		{"BLUETOOTH",				.val = 0, .def = 1, .del = 0, .min = 0, . max = 0, 		.is_need_to_save = 1, .is_change_cyclic = 1},
};

//const Item_TypeDef item_temperature 			= {.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {0, 0}};
//const Item_TypeDef item_brightness 			= {.menu = SETTINGS, 	.s_ptr = &s_ptr[0], 	.numbers = {1, 3}};
//const Item_TypeDef item_ambient_light_sensor 	= {.menu = SETTINGS, 	.s_ptr = &s_ptr[1], 	.numbers = {2, 4}};
//const Item_TypeDef item_temperature_sensor 	= {.menu = SETTINGS, 	.s_ptr = &s_ptr[2], 	.numbers = {3, 5}};
Item_TypeDef items_list[] = {
		{.menu = NO_MENU, 	.s_ptr = NULL, 			.numbers = {0, 0}}, 	// "NO_ITEM",
		//		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {0, 5}}, 	// "FW VERSION",
		//		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {22, 05}}, 	// "COMPILE DATE",
		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {20, 25}}, 	// "YEAR",
		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {31, 01}}, 	// "DAY_MONTH",
		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {99, 99}}, 	// "HOURS_MINUTES",
		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {8, 30}}, 	// "MINUTES_SECONDS",
		{.menu = MAIN, 		.s_ptr = NULL, 			.numbers = {25, 3}},		// Temperature
		{.menu = SETTINGS, 	.s_ptr = &s_ptr[0], 	.numbers = {1, 3}},
		{.menu = SETTINGS, 	.s_ptr = &s_ptr[1], 	.numbers = {2, 4}},
		{.menu = SETTINGS, 	.s_ptr = &s_ptr[2], 	.numbers = {3, 5}},
		//		item_brightness, // "TUBES BRIGHTNESS ADJUST",
		//		item_ambient_light_sensor, // "AMBIENT LIGHT SENSOR",
		//		item_temperature_sensor, // "TEMPERATURE SENSOR",
		//		{.menu = SETTINGS, 	.s_ptr = NULL, .numbers = {4, 6}}, // "BLUETOOTH",
};
uint16_t settings_size = sizeof(s_ptr)/sizeof(s_ptr[0]);
uint8_t num_of_btns = sizeof(btns_list)/sizeof(btns_list[0]);
uint8_t num_of_items = sizeof(items_list)/sizeof(items_list[0]);







uint8_t always_update = 1;
volatile uint8_t is_update_battery, is_update_distance, is_update_temp_and_press, is_update_hall, is_update_gui, is_update_ntm_flags, is_show_menu;
volatile

typedef struct {
	uint8_t* 	start_condition;	// Time to pass in ms to set flag to update
	uint32_t 	period;				// Time to pass after since last_upd update to trigger the update request func
	uint32_t 	last_upd;			// Tick in ms when the last update happened
	void  		(*upd_func)(void); 	// Instead of using flag we can directly use custom function
} update_request; // Milliseconds passed to update certain parameters
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM21_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void update_flags(void) {
	is_update_battery 			= 0;//hntm.standby.status == OFF && is_battery_value_ready;
	is_update_distance 			= 0;//hntm.standby.status == OFF && (hrangefinder.status == DISTANCE_ACQUIRED);
	is_update_temp_and_press	= 0;//hntm.standby.status == OFF && (auto_pressure == ON || auto_temperature == ON);
	is_update_gui				= 0;//hntm.standby.status == OFF;
	is_update_ntm_flags 		= 0;//(!settings.power_saving) && (settings.power_saving_timer > 0);
	//	is_show_menu				= 0;//action != NONE &&
}



static void collapse_menu(void) {
}

static void update_running_time(void) {
}

static void update_battery(void) {
	//	adc_convert();
	//	get_cpu_temp(); // todo: remove?
	//
	//	is_battery_value_ready = 0;
}

static void update_distance(void) {
}

static void update_temp_and_press(void) {
}

static void update_angles(void) {
}

static void update_hall(void) {
}

static void update_ntm_flags(void) {
}

static HAL_StatusTypeDef sensors_check(void) {
}

static void system_update(update_request* secondary_tasks[], uint8_t num_of_tasks) {
	/*
	 * Update flags and timers for all parameters
	 */
	for (int i = 0; i < num_of_tasks; ++i) {
		update_request* request = secondary_tasks[i];

		if(*(request->start_condition) == 1) {
			uint32_t current_tick = HAL_GetTick();

			if(current_tick - request->last_upd > request->period) {
				if(request->upd_func != NULL) {
					(request->upd_func)();
					request->last_upd = current_tick;
				} else {
					Error_Handler();
				}
			}
		}
	}
}
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
	MX_DMA_Init();
	MX_I2C1_Init();
	MX_LPUART1_UART_Init();
	MX_USART2_UART_Init();
	MX_RTC_Init();
	MX_TIM21_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	settings_init(s_ptr, settings_size);
	IN12_init();
	DS3231_Init(&hi2c1);
	init_menu_items(&hmenu, items_list, NUM_OF_MENUS, NUM_OF_ITEMS);
	btns_init(&hbtns, btns_list, num_of_btns, &htim21, PRESSED);
	Tmp75_Init(&hi2c1);
	opt3001_init();

//	update_request gui_collapse 	= {&flag_show_full_menu, 		15000, 	0, collapse_menu};
	update_request running_time 	= {&always_update, 				1000, 	0, update_running_time};
	update_request temp_and_press 	= {&is_update_temp_and_press, 	100, 	0, update_temp_and_press};
	update_request distance 		= {&is_update_distance, 		100, 	0, update_distance};
//	update_request hall 			= {&settings.power_saving, 		100, 	0, update_hall};
	update_request angles			= {&always_update, 				50, 	0, update_angles};
	update_request battery 			= {&is_update_battery, 			50, 	0, update_battery};
	update_request flags 			= {&always_update, 				10, 	0, update_flags};
//	update_request gui 				= {&is_update_gui, 				10, 	0, draw_all};
	update_request ntm_flags 		= {&is_update_ntm_flags, 		10, 	0, update_ntm_flags};

	update_request* requests[] = {
//			&gui_collapse,   	// Hide most of menu icons in main mode
			&running_time, 		// Once in a second obviously
			&temp_and_press, 	// Can be once every 10ms (100 Hertz refresh rate)
			&distance, 			// Can be once every 10ms (100 Hertz refresh rate)
//			&hall, 				// Can be once every 10ms (100 Hertz refresh rate)
			&angles, 			// Can be once every 10ms (100 Hertz refresh rate)
			&battery, 			// It has it's own timer actually so we base update on different variable
			&flags, 			// Update as fast as the fastest other parameter
			&ntm_flags, 		// Update as fast as the fastest other parameter
//			&gui, 				// Update as fast as the fastest other parameter
	};
	uint8_t requests_num = sizeof(requests)/sizeof(requests[0]);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	// 7-15 Volts input

	// No need to fix interrupt being called right after the starting timer
	// Here it serve the purpose of updating important variables and setting bits in control registers
	while (1)
	{
		// must update when digits are active
		if(flag_upd_time) {
			time_update();
			items_list[YEAR].numbers[0] = 20;
			items_list[YEAR].numbers[1] = DS3231_GetYear() - 2000;
			items_list[DAY_MONTH].numbers[0] = DS3231_GetDate();
			items_list[DAY_MONTH].numbers[1] = DS3231_GetMonth();
			items_list[HOURS_MINUTES].numbers[0] = DS3231_GetHour();
			items_list[HOURS_MINUTES].numbers[1] = DS3231_GetMinute();
			items_list[MINUTES_SECONDS].numbers[0] = items_list[HOURS_MINUTES].numbers[1];
			items_list[MINUTES_SECONDS].numbers[1] = DS3231_GetSecond();

			Read_TempCelsius(&t);
			items_list[TEMPERATURE].numbers[0] = t;
			items_list[TEMPERATURE].numbers[1] = ((int)(t*100))%100;

			flag_upd_time = 0;
		}

		if(flag_upd_tubes) {
			IN12_set();
			flag_upd_tubes = 0;
		}

		if(flag_upd_dots) {
			flag_upd_dots = 0;
			HAL_GPIO_TogglePin(INS_EN_3V3_GPIO_Port, INS_EN_3V3_Pin);
		}

		//	  if(hmenu.current_item == )

		if(logic[hmenu.current_item][shared_mask][shared_press_type][hmenu.is_selected] != NULL) {
			(*logic[hmenu.current_item][shared_mask][shared_press_type][hmenu.is_selected])();
			tubes_data_source_set(&(items_list[hmenu.current_item].numbers[0]), &(items_list[hmenu.current_item].numbers[1]));
			shared_mask = 0;
			shared_press_type = 0;
		}

		lux = calculate_lux();

		system_update(requests, requests_num);
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
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 3200-1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 100-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM21 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM21_Init(void)
{

	/* USER CODE BEGIN TIM21_Init 0 */

	/* USER CODE END TIM21_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM21_Init 1 */

	/* USER CODE END TIM21_Init 1 */
	htim21.Instance = TIM21;
	htim21.Init.Prescaler = 32000-1;
	htim21.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim21.Init.Period = 50-1;
	htim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim21.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim21) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim21, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim21, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM21_Init 2 */

	/* USER CODE END TIM21_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel2_3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	/* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

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
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : RCK_3V3_Pin SCK_3V3_Pin */
	GPIO_InitStruct.Pin = RCK_3V3_Pin|SCK_3V3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
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
