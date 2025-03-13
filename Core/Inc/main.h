/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DIMM_LED_1_Pin GPIO_PIN_14
#define DIMM_LED_1_GPIO_Port GPIOC
#define DIMM_LED_2_Pin GPIO_PIN_15
#define DIMM_LED_2_GPIO_Port GPIOC
#define BTN_4_Pin GPIO_PIN_0
#define BTN_4_GPIO_Port GPIOA
#define TXS_OE_Pin GPIO_PIN_1
#define TXS_OE_GPIO_Port GPIOA
#define SHDN_170V_3V3_Pin GPIO_PIN_4
#define SHDN_170V_3V3_GPIO_Port GPIOA
#define INS_EN_3V3_Pin GPIO_PIN_5
#define INS_EN_3V3_GPIO_Port GPIOA
#define DIN_3V3_Pin GPIO_PIN_6
#define DIN_3V3_GPIO_Port GPIOA
#define OE_3V3_Pin GPIO_PIN_7
#define OE_3V3_GPIO_Port GPIOA
#define RCK_3V3_Pin GPIO_PIN_0
#define RCK_3V3_GPIO_Port GPIOB
#define SCK_3V3_Pin GPIO_PIN_1
#define SCK_3V3_GPIO_Port GPIOB
#define INT_SQW_Pin GPIO_PIN_8
#define INT_SQW_GPIO_Port GPIOA
#define INT_OPT_Pin GPIO_PIN_11
#define INT_OPT_GPIO_Port GPIOA
#define JDY_STAT_Pin GPIO_PIN_12
#define JDY_STAT_GPIO_Port GPIOA
#define JDY_PWRC_Pin GPIO_PIN_15
#define JDY_PWRC_GPIO_Port GPIOA
#define BTN_3_Pin GPIO_PIN_3
#define BTN_3_GPIO_Port GPIOB
#define BTN_2_Pin GPIO_PIN_4
#define BTN_2_GPIO_Port GPIOB
#define BTN_1_Pin GPIO_PIN_5
#define BTN_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define MASK_LEFT					1
#define MASK_ENTER					2
#define MASK_RIGHT					4
#define NUM_BTN_COMBINATIONS		((MASK_LEFT|MASK_RIGHT|MASK_ENTER) + 1)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
