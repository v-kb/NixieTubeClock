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

void time_update(void);
void IN12_init(void);
void IN12_set_digit_pairs(uint16_t first_number, uint16_t second_number);
void IN12_set_digits_separately(uint8_t first_digit, uint8_t second_digit, uint8_t third_digit, uint8_t forth_digit);

#endif /* INC_IN12_H_ */
