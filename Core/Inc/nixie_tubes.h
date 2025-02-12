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
//#include <string.h>
//#include <stdint.h>


#define TOGGLE_170V()							HAL_GPIO_TogglePin(SHDN_170V_3V3_GPIO_Port, SHDN_170V_3V3_Pin)
#define OUTPUT_DISABLE()						HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_SET)
#define NUM_OF_DIGITS 							10
#define NUM_OF_TUBES 							4
#define NUM_TUBES 4
#define NUM_DIGITS 10
#define PWM_FREQUENCY 200       // Hz
#define MAX_CURRENT_MA 2.5f     // Per digit
#define ROTATION_INTERVAL 3600  // Seconds (1 hour)
#define HEALTH_CHECK_INTERVAL 60


//typedef struct {
////	RTC_TimeTypeDef time; 									// Hours, minutes, seconds - this will be in settings...
////	RTC_DateTypeDef date;									// Year, month, date
//	uint16_t* first_number;
//	uint16_t* second_number;
//	bool is_digit_on[NUM_OF_TUBES][NUM_OF_DIGITS];			// True means digit X in a tube Y is now turned on
//	union {
//		uint16_t tube[NUM_OF_TUBES];
//		uint64_t fused;										// Tube #1 mask + Tube #2 mask + ...
//	} bitmask;
//	uint8_t duty_cycles[NUM_OF_TUBES];
//} ClockData_HandleTypeDef;

typedef struct {
    float duty_cycle;
    uint32_t period_ticks;
    uint32_t pulse_ticks;
    TIM_HandleTypeDef* timer;
} PWM_Control;

typedef enum {
    ANIM_FADE,
    ANIM_SLIDE,
    ANIM_SPARK,
    ANIM_ANTI_POISON
} AnimationType;

typedef struct {
    AnimationType type;
    uint32_t start_tick;
    uint32_t duration;
    uint8_t from_digit;
    uint8_t to_digit;
    float progress;
} TubeAnimation;

typedef struct {
    uint32_t hours_active[NUM_DIGITS];
    uint16_t current_samples[NUM_DIGITS];
    float temperature;
    float health_score;
    uint8_t rotation_counter;
} TubeHealth;

typedef struct {
    uint8_t current_digit;
    uint8_t target_digit;
    PWM_Control pwm;
    TubeAnimation animation;
    TubeHealth health;
    uint8_t is_enabled;
} NixieTube;

typedef struct {
    NixieTube tubes[NUM_TUBES];
    uint32_t system_clock;
    float brightness;
    uint8_t emergency_state;
} NixieClock;

//extern ClockData_HandleTypeDef clock_data;
extern uint8_t flag_upd_tubes;
extern uint8_t flag_upd_time;
extern uint8_t flag_upd_dots;

void time_update(void);
void IN12_init(void);
//void IN12_set_digit_pairs(uint16_t first_number, uint16_t second_number);
void IN12_set(NixieClock* clock);
void bitmask_set(void);
void tubes_data_source_set(uint16_t* first_number_src, uint16_t* second_number_src);

#endif /* INC_IN12_H_ */
