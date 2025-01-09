/*
 * callbacks.c
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */
#include "buttons.h"
#include "IN12.h"


//uint8_t actions[7][5] = {
//
//};
extern TIM_HandleTypeDef 		htim2;
extern TIM_HandleTypeDef 		htim21;
extern Buttons_HandleTypeDef 	hbtns;

void btns_callback(uint16_t mask, PressType_TypeDef press_type) {
	shared_mask 		= mask;
	shared_press_type 	= press_type;

	//	switch (mask) {
	//	case MASK_LEFT:
	//		break;
	//	case MASK_RIGHT:
	//		break;
	//
	//	default: break;
	//	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	static uint8_t count_50_ms = 0;
	if(htim == &htim2) {
		bitmask_set(); // restore values
		if(!flag_upd_tubes)
			flag_upd_tubes = 1;
//		IN12_set();
	}

	if(htim == &htim21) {
		flag_upd_time = 1;
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
//		IN12_set();
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
	static uint16_t compare_value = 49;
	if(htim == hbtns.htim) {
		compare_value = compare_value > 999 ? 49 : compare_value + 50;
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, compare_value);
		btns_check(&hbtns);
//		htim->Instance->CCMR1 += 50;
	}
}
