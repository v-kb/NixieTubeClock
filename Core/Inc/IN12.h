/*
 * IN12.h
 *
 *  Created on: Dec 25, 2024
 *      Author: v_
 */

#ifndef INC_IN12_H_
#define INC_IN12_H_

#include <stdlib.h>
#include <time.h>
#include "ds3231_for_stm32_hal.h"
#include <stdbool.h>
#include <string.h>

#define OUTPUT_DISABLE()						HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_SET)
#define NUM_OF_DIGITS 							10
#define NUM_OF_TUBES 							4

typedef struct {
//	RTC_TimeTypeDef time; 									// Hours, minutes, seconds - this will be in settings...
//	RTC_DateTypeDef date;									// Year, month, date
	bool is_digit_on[NUM_OF_TUBES][NUM_OF_DIGITS];			// True means digit X in a tube Y is now turned on
	union {
		uint16_t tube[NUM_OF_TUBES];
		uint64_t fused;										// Tube #1 mask + Tube #2 mask + ...
	} bitmask;
	uint8_t duty_cycles[NUM_OF_TUBES];
} ClockData_HandleTypeDef;

extern ClockData_HandleTypeDef clock_data;
extern uint8_t flag_upd_tubes;
extern uint8_t flag_upd_time;

void time_update(void);
void IN12_init(void);
//void IN12_set_digit_pairs(uint16_t first_number, uint16_t second_number);
void IN12_set(void);
void bitmask_set(void);

#endif /* INC_IN12_H_ */
