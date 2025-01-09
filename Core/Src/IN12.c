/*
 * IN12.c
 *
 *  Created on: Dec 25, 2024
 *      Author: v_
 */

#include "IN12.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

struct {
	uint8_t is_on; // 1 = lamp is turned on
} lamp_params;


#define PULSE_DURATION_MS						0 // Debug value 1000ms to see action
#define OUTPUT_ENABLE()							HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_RESET)

#define DATA_SET(value)							HAL_GPIO_WritePin(DIN_3V3_GPIO_Port, DIN_3V3_Pin, (GPIO_PinState)(value))	// Set data output
#define SHIFT_REG_SET(value)					HAL_GPIO_WritePin(SCK_3V3_GPIO_Port, SCK_3V3_Pin, (GPIO_PinState)(value))	// Set shift register
#define STORAGE_REG_SET(value)					HAL_GPIO_WritePin(RCK_3V3_GPIO_Port, RCK_3V3_Pin, (GPIO_PinState)(value))	// Set storage register
#define CONV_DIGIT_TO_SR_BITMASK_LSB(digit)  	((uint16_t)(1 << digit) & 0b000000001)
#define GET_MOST_SIGNIFICANT_DIGIT(number)		((uint8_t)(number/10))
#define GET_LEAST_SIGNIFICANT_DIGIT(number)		((uint8_t)(number%10))

uint8_t flag_upd_tubes;
uint8_t flag_upd_time;
//#define COMPILATION_TIME
//
//char compile_time[]	= __TIME__;
//
//struct {
//
//};
ClockData_HandleTypeDef clock_data;

const uint16_t digit_bitmask[NUM_OF_DIGITS] = {
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
static void shift_reg_send(uint64_t data) {
	for (int8_t bit = 63; bit >= 0; --bit) {	// Send least significant bit first
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
//static void shift_reg_send(uint16_t data) {
//	for (int8_t bit = 15; bit >= 0; --bit) {	// Send least significant bit first
//		/*
//		 * Push data into shift register
//		 */
//		DATA_SET((data >> bit) & 1);
//
//		/*
//		 * Set SER pin according to currently transmitted bit
//		 * Toggle clock n times to set all N bits
//		 */
//		shift_reg_pulse_srclk(PULSE_DURATION_MS);
//	}
//}


//void IN12_set_digit_pairs(uint16_t first_number, uint16_t second_number) {
//	IN12_set(first_number/10, first_number%10, second_number/10, second_number%10);
//}

void IN12_set(void) {
	OUTPUT_DISABLE();


//	uint16_t first_digit_data 	= digit_bitmask[first_digit];
//	uint16_t second_digit_data 	= digit_bitmask[second_digit];
//	uint16_t third_digit_data 	= digit_bitmask[third_digit];
//	uint16_t forth_digit_data 	= digit_bitmask[forth_digit];

	/*
	 * Send data
	 */
//	shift_reg_send(forth_digit_data);
//	shift_reg_send(third_digit_data);
//	shift_reg_send(second_digit_data);
//	shift_reg_send(first_digit_data);

//	shift_reg_send(clock_data.bitmask.tube[3]);
//	shift_reg_send(clock_data.bitmask.tube[2]);
//	shift_reg_send(clock_data.bitmask.tube[1]);
//	shift_reg_send(clock_data.bitmask.tube[0]);
	shift_reg_send(clock_data.bitmask.fused);

	/*
	 * Toggle latch
	 */
	shift_reg_pulse_rclk(PULSE_DURATION_MS);

	OUTPUT_ENABLE();
}

void bitmask_set(void) {
	memset(clock_data.is_digit_on, 0, sizeof(bool)*NUM_OF_TUBES*NUM_OF_DIGITS);
	clock_data.bitmask.fused = 0;

	clock_data.is_digit_on[0][GET_MOST_SIGNIFICANT_DIGIT(rtc_time.Hours)] 		= true;
	clock_data.is_digit_on[1][GET_LEAST_SIGNIFICANT_DIGIT(rtc_time.Hours)] 		= true;
	clock_data.is_digit_on[2][GET_MOST_SIGNIFICANT_DIGIT(rtc_time.Seconds)] 	= true;
	clock_data.is_digit_on[3][GET_LEAST_SIGNIFICANT_DIGIT(rtc_time.Seconds)] 	= true;

	for (int t = 0; t < NUM_OF_TUBES; ++t) {
		for (int d = 0; d < NUM_OF_DIGITS; ++d) {
			clock_data.bitmask.tube[t] |= digit_bitmask[d]*clock_data.is_digit_on[t][d];
		}
	}
}

void time_update(void) {
	// Need to stop PWM first so there is no flickering
	rtc_time.Hours 		= DS3231_GetHour();
	rtc_time.Minutes 	= DS3231_GetMinute();
	rtc_time.Seconds 	= DS3231_GetSecond();


}

static void date_update(void) {
	rtc_date.Year 		= DS3231_GetYear();
	rtc_date.Month 		= DS3231_GetMonth();
	rtc_date.Date 		= DS3231_GetDate();
	rtc_date.WeekDay 	= DS3231_GetDayOfWeek();
}

void IN12_init(void) {
	if(DS3231_IsOscillatorStopped()) {
		// todo: go to the "time set mode" automaticaly
		DS3231_SetFullTime(12, 34, 56);
		DS3231_EnableOscillator(DS3231_ENABLED);
	}

	time_update();
	date_update();
}
