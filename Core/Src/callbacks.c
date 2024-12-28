/*
 * callbacks.c
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */
#include "buttons.h"


//uint8_t actions[7][5] = {
//
//};
extern TIM_HandleTypeDef 		htim22;
extern Buttons_HandleTypeDef 	hbtns;

void btns_callback(uint16_t mask, PressType_TypeDef press_type)
{

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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim == &htim22) {




  }
  if(htim == hbtns.htim) {
	  btns_check(&hbtns);
  }
}
