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
//extern TIM_HandleTypeDef 		htim22;
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

	if(htim == hbtns.htim) {
		btns_check(&hbtns);

		++count_50_ms;
		if(count_50_ms >= 1000/50) {
			count_50_ms = 0;
			if(!flag_upd_time)
				flag_upd_time = 1;
//			time_update();
		}
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim2) {
		clock_data.bitmask.tube[htim->Channel - 1] = 0; // E.g. "if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {...}"
		if(!flag_upd_tubes)
			flag_upd_tubes = 1;
//		IN12_set();
	}
}
