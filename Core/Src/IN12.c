/*
 * IN12.c
 *
 *  Created on: Dec 25, 2024
 *      Author: v_
 */

#include "IN12.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;


#define PULSE_DURATION_MS						0 // Debug value 1000ms to see action
#define OUTPUT_ENABLE()							HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_RESET)
#define OUTPUT_DISABLE()						HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_SET)
#define DATA_SET(value)							HAL_GPIO_WritePin(DIN_3V3_GPIO_Port, DIN_3V3_Pin, (GPIO_PinState)(value))	// Set data output
#define SHIFT_REG_SET(value)					HAL_GPIO_WritePin(SCK_3V3_GPIO_Port, SCK_3V3_Pin, (GPIO_PinState)(value))	// Set shift register
#define STORAGE_REG_SET(value)					HAL_GPIO_WritePin(RCK_3V3_GPIO_Port, RCK_3V3_Pin, (GPIO_PinState)(value))	// Set storage register
#define CONV_DIGIT_TO_SR_BITMASK_LSB(digit)  	((uint16_t)(1 << digit) & 0b000000001)
#define NUM_OF_DIGITS 							10

//#define COMPILATION_TIME
//
//char compile_time[]	= __TIME__;
//
//struct {
//
//};

uint16_t digit_bitmask[NUM_OF_DIGITS] = {
	0b0000000001,	// 0
	0b0000000010,	// 1
	0b0000000100,	// 2
	0b0000001000,	// 3
	0b0000010000,	// 4
	0b0000100000,	// 5
	0b0001000000,	// 6
	0b0010000000,	// 7
	0b0100000000,	// 8
	0b1000000000 	// 9
};

static void shift_reg_pulse_srclk(uint32_t delay_ms) {
	SHIFT_REG_SET(1);
	if (delay_ms > 0) HAL_Delay(delay_ms - 1);
	SHIFT_REG_SET(0);
	if (delay_ms > 0) HAL_Delay(delay_ms - 1);
}
static void shift_reg_pulse_rclk(uint32_t delay_ms) {
	STORAGE_REG_SET(1);
	if (delay_ms > 0) HAL_Delay(delay_ms - 1);
	STORAGE_REG_SET(0);
	if (delay_ms > 0) HAL_Delay(delay_ms - 1);
}


// data size is 16 bit, but only 10 are actually connected to a nixie lamp
static void shift_reg_send(uint16_t data) {
	for (int8_t bit = 15; bit >= 0; --bit) {	// Send least significant bit first
		/*
		 * Push data into shift register
		 */
		DATA_SET((data >> bit) & 1);

		/*
		 * Set SER pin according to currently transmitted bit
		 * Toggle clock n times to set all N bits
		 */
		shift_reg_pulse_srclk(PULSE_DURATION_MS);
	}
}


void IN12_set_digit_pairs(uint16_t first_number, uint16_t second_number) {
	IN12_set_digits_separately(first_number/10, first_number%10, second_number/10, second_number%10);
}

void IN12_set_digits_separately(uint8_t first_digit, uint8_t second_digit, uint8_t third_digit, uint8_t forth_digit) {
	OUTPUT_DISABLE();

	/*
	 * Prepare bitmasks to be sent
	 */
	uint16_t first_digit_data 	= digit_bitmask[first_digit];
	uint16_t second_digit_data 	= digit_bitmask[second_digit];
	uint16_t third_digit_data 	= digit_bitmask[third_digit];
	uint16_t forth_digit_data 	= digit_bitmask[forth_digit];

	/*
	 * Send data
	 */
	shift_reg_send(forth_digit_data);
	shift_reg_send(third_digit_data);
	shift_reg_send(second_digit_data);
	shift_reg_send(first_digit_data);

	/*
	 * Toggle latch
	 */
	shift_reg_pulse_rclk(PULSE_DURATION_MS);

	OUTPUT_ENABLE();
}

void IN12_init(void) {
	rtc_time.Hours 		= DS3231_GetHour();
	rtc_time.Minutes 	= DS3231_GetMinute();
	rtc_time.Seconds 	= DS3231_GetSecond();

	rtc_date.Year 		= DS3231_GetYear();
	rtc_date.Month 		= DS3231_GetMonth();
	rtc_date.Date 		= DS3231_GetDate();
	rtc_date.WeekDay 	= DS3231_GetDayOfWeek();

	if(!rtc_time.Hours && !rtc_time.Minutes && !rtc_time.Seconds) {
		// todo: go to the "time set mode" automaticaly
		DS3231_SetFullTime(21, 34, 00);
	}


	DS3231_GetDate();
	DS3231_EnableOscillator(DS3231_ENABLED);
}
