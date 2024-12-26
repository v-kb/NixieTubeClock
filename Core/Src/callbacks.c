/*
 * callbacks.c
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */
#include "buttons.h"


uint8_t actions[7][5] = {

};


void btns_callback(uint16_t mask, PressType_TypeDef press_type)
{
	switch (mask) {
	case MASK_LEFT:
		break;
	case MASK_RIGHT:
		break;

	default: break;
	}
}

