/*
 * callbacks.c
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */
#include "buttons.h"
#include "IN12.h"


extern TIM_HandleTypeDef 		htim2;
extern TIM_HandleTypeDef 		htim21;
extern Buttons_HandleTypeDef 	hbtns;

extern uint8_t duty_cycles[4];


void btns_callback(uint16_t mask, PressType_TypeDef press_type) {
	shared_mask 		= mask;
	shared_press_type 	= press_type;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	static uint32_t count_50_ms = 0;
	if(htim == &htim2) {
		bitmask_set(); // restore values
		if(!flag_upd_tubes)
			flag_upd_tubes = 1;
	}

	if(htim == &htim21) {
		++count_50_ms;
		if (count_50_ms >= 20) {
			flag_upd_time = 1;
			flag_upd_dots = 1;
			count_50_ms = 0;
		}
		if (count_50_ms % 1000 == 0) {
		}
		btns_check(&hbtns);
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim2) {
		switch (htim->Channel) {
			case HAL_TIM_ACTIVE_CHANNEL_1: clock_data.bitmask.tube[0] = 0; break;// E.g. "if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {...}"
			case HAL_TIM_ACTIVE_CHANNEL_2: clock_data.bitmask.tube[1] = 0; break;
			case HAL_TIM_ACTIVE_CHANNEL_3: clock_data.bitmask.tube[2] = 0; break;
			case HAL_TIM_ACTIVE_CHANNEL_4: clock_data.bitmask.tube[3] = 0; break;
			default: break;
		}
		if(!flag_upd_tubes)
			flag_upd_tubes = 1;
	}
}
