/*
 * IN12.c
 *
 *  Created on: Dec 25, 2024
 *      Author: v_
 */

#include "main.h"
#include "nixie_tubes.h"
#include <math.h>





#define PULSE_DURATION_MS						0 // Debug value 1000ms to see action
#define OUTPUT_ENABLE()							HAL_GPIO_WritePin(OE_3V3_GPIO_Port, OE_3V3_Pin, GPIO_PIN_RESET)

#define DATA_SET(value)							HAL_GPIO_WritePin(DIN_3V3_GPIO_Port, DIN_3V3_Pin, (GPIO_PinState)(value))	// Set data output
#define SHIFT_REG_SET(value)					HAL_GPIO_WritePin(SCK_3V3_GPIO_Port, SCK_3V3_Pin, (GPIO_PinState)(value))	// Set shift register
#define STORAGE_REG_SET(value)					HAL_GPIO_WritePin(RCK_3V3_GPIO_Port, RCK_3V3_Pin, (GPIO_PinState)(value))	// Set storage register
#define CONV_DIGIT_TO_SR_BITMASK_LSB(digit)  	((uint16_t)(1 << digit) & 0b000000001)
#define GET_MOST_SIGNIFICANT_DIGIT(number)		((uint8_t)(number/10))
#define GET_LEAST_SIGNIFICANT_DIGIT(number)		((uint8_t)(number%10))

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

uint8_t flag_upd_tubes;
uint8_t flag_upd_time;
uint8_t flag_upd_dots;

//#define COMPILATION_TIME
//
//char compile_time[]	= __TIME__;

float brightness;

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






//#pragma region Hardware Abstraction
// Implement these based on your specific hardware
//void set_shift_register(uint8_t tube, uint16_t data) {
//    // Your shift register control code
//}

float read_current(uint8_t tube) {
    // ADC read implementation
    return 0.0f; // Return mA
}

float read_temperature() {
    // Temperature sensor read
    return 25.0f; // °C
}

void update_health_stats(NixieClock* clock) {
    for(uint8_t t = 0; t < NUM_TUBES; t++) {
        NixieTube* tube = &clock->tubes[t];

        // Update active time
        if(tube->current_digit < NUM_DIGITS) {
            tube->health.hours_active[tube->current_digit] +=
                HEALTH_CHECK_INTERVAL / 3600;
        }

        // Current monitoring (moving average)
        for(uint8_t d = 0; d < NUM_DIGITS; d++) {
            tube->health.current_samples[d] =
                (tube->health.current_samples[d] * 0.9f) +
                (read_current(t) * 0.1f);
        }

        // Temperature monitoring
        tube->health.temperature = read_temperature();

        // Calculate health score (0-100%)
        float usage_balance = 1.0f;
        float max_hours = 0, min_hours = UINT32_MAX;
        for(uint8_t d = 0; d < NUM_DIGITS; d++) {
            if(tube->health.hours_active[d] > max_hours)
                max_hours = tube->health.hours_active[d];
            if(tube->health.hours_active[d] < min_hours)
                min_hours = tube->health.hours_active[d];
        }
        usage_balance = 1.0f - ((max_hours - min_hours) / (max_hours + 0.001f));

        tube->health.health_score = (usage_balance * 0.7f) +
                                  ((tube->health.temperature < 45.0f) ? 0.3f : 0.0f);
    }
}

void rotate_digits(NixieClock* clock) {
    for(uint8_t t = 0; t < NUM_TUBES; t++) {
        NixieTube* tube = &clock->tubes[t];

        // Find least used digit
        uint8_t least_used = 0;
        uint32_t min_hours = UINT32_MAX;
        for(uint8_t d = 0; d < NUM_DIGITS; d++) {
            if(tube->health.hours_active[d] < min_hours) {
                min_hours = tube->health.hours_active[d];
                least_used = d;
            }
        }

        // Schedule rotation
        if(tube->health.rotation_counter++ >= ROTATION_INTERVAL) {
            tube->target_digit = least_used;
            tube->health.rotation_counter = 0;
        }
    }
}

void anti_poisoning_procedure(NixieClock* clock) {
    static uint32_t last_run = 0;
    if((clock->system_clock - last_run) > 604800000) { // Weekly
        for(uint8_t t = 0; t < NUM_TUBES; t++) {
            for(uint8_t d = 0; d < NUM_DIGITS; d++) {
                set_shift_register(t, (1 << d));
                HAL_Delay(50);
                set_shift_register(t, 0);
                HAL_Delay(150);
            }
        }
        last_run = clock->system_clock;
    }
}


void update_animation(NixieTube* tube) {
    if(tube->animation.duration == 0) return;

    uint32_t elapsed = HAL_GetTick() - tube->animation.start_tick;
    tube->animation.progress = (float)elapsed / tube->animation.duration;

    if(tube->animation.progress >= 1.0f) {
        tube->current_digit = tube->target_digit;
        tube->animation.duration = 0;
        return;
    }

    switch(tube->animation.type) {
        case ANIM_FADE:
            tube->pwm.duty_cycle = tube->animation.progress * brightness; // Duty cycles define reset of tubes inside the ITERRUPT (when nothing is shown)
            break;

        case ANIM_ANTI_POISON:
            // Special handling for anti-poison animation
            break;

        // Add other animation types
        default: break;
    }
}

void start_animation(NixieTube* tube, AnimationType type, uint32_t duration) {
    tube->animation = (TubeAnimation){
        .type = type,
        .start_tick = HAL_GetTick(),
        .duration = duration,
        .from_digit = tube->current_digit,
        .to_digit = tube->target_digit,
        .progress = 0.0f
    };
}

//void check_emergency(NixieClock* clock) {
//    for(uint8_t t = 0; t < NUM_TUBES; t++) {
//        // Overcurrent protection
//        float total_current = 0.0f;
//        for(uint8_t d = 0; d < NUM_DIGITS; d++) {
//            total_current += clock->tubes[t].health.current_samples[d];
//        }
//
//        if(total_current > MAX_CURRENT_MA * 1.5f) {
//            clock->emergency_state = 1;
//            return;
//        }
//
//        // Overtemperature protection
//        if(clock->tubes[t].health.temperature > 50.0f) {
//            clock->emergency_state = 2;
//            return;
//        }
//    }
//}
//
//void emergency_handler(NixieClock* clock) {
//    if(clock->emergency_state == 0) return;
//
//    // Disable all tubes
//    for(uint8_t t = 0; t < NUM_TUBES; t++) {
//        clock->tubes[t].is_enabled = 0;
//        set_shift_register(t, 0);
//    }
//
//    // Log error (implement your preferred method)
//    switch(clock->emergency_state) {
//        case 1: printf("EMERGENCY: Overcurrent!\n"); break;
//        case 2: printf("EMERGENCY: Overtemperature!\n"); break;
//    }
//
//    // Requires manual reset
//    while(1) { __WFI(); }
//}


void nixie_clock_update(NixieClock* clock) {
    clock->system_clock = HAL_GetTick();

    // Update all subsystems
    update_health_stats(clock);
    rotate_digits(clock);
    anti_poisoning_procedure(clock);

    for(uint8_t t = 0; t < NUM_TUBES; t++) {
        update_animation(&clock->tubes[t]);

        // Update shift register if needed
        uint16_t data = digit_bitmask[clock->tubes[t].current_digit] * clock->tubes[t].is_enabled;
		set_shift_register_single(data);
    }

    check_emergency(clock);
    emergency_handler(clock);
}

void nixie_clock_init(NixieClock* clock) {
    memset(clock, 0, sizeof(NixieClock));

    for(uint8_t t = 0; t < NUM_TUBES; t++) {
        clock->tubes[t].pwm.duty_cycle = 0.75f;
        clock->tubes[t].is_enabled = 1;
    }

    // Initialize hardware timers
    // Your PWM timer initialization code here
}

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
static void shift_reg_send_single(uint16_t data) {
	for (int8_t bit = 15; bit >= 0; --bit) {	// Send least significant bit first
		/*
		 * Push data into shift register
		 */
		DATA_SET((data >> bit) & 1);

		/*
		 * Set SER pin according to currently transmitted bit
		 * Toggle clock n times to set all N bits
		 */
//		shift_reg_pulse_srclk(PULSE_DURATION_MS);
		SHIFT_REG_SET(1);
		SHIFT_REG_SET(0);
	}
}


//void IN12_set_digit_pairs(uint16_t first_number, uint16_t second_number) {
//	IN12_set(first_number/10, first_number%10, second_number/10, second_number%10);
//}

void IN12_set(NixieClock* clock) {
	OUTPUT_DISABLE();

	/*
	 * Send data
	 */
	union data{
		uint64_t combined;
		uint16_t per_tube[4];
	} data_u;
	uint16_t data = 0;
	for (int t = 3; t >= 0; --t) {
		data_u.per_tube[t] = digit_bitmask[clock->tubes[t].target_digit];
		data = digit_bitmask[clock->tubes[t].target_digit];
		shift_reg_send_single(data);
	}


	/*
	 * Toggle latch
	 */
	shift_reg_pulse_rclk(PULSE_DURATION_MS);

	OUTPUT_ENABLE();
}

void bitmask_set(NixieClock* clock) {
	memset(clock_data.is_digit_on, 0, sizeof(bool)*NUM_OF_TUBES*NUM_OF_DIGITS);
	clock_data.bitmask.fused = 0;

//	digit[4] = {
//			*clock_data.first_number,
//			*clock_data.first_number,
//	};

	clock_data.is_digit_on[0][GET_MOST_SIGNIFICANT_DIGIT(*clock_data.first_number)] 	= true;
	clock_data.is_digit_on[1][GET_LEAST_SIGNIFICANT_DIGIT(*clock_data.first_number)] 	= true;
	clock_data.is_digit_on[2][GET_MOST_SIGNIFICANT_DIGIT(*clock_data.second_number)] 	= true;
	clock_data.is_digit_on[3][GET_LEAST_SIGNIFICANT_DIGIT(*clock_data.second_number)] 	= true;

	for (int t = 0; t < NUM_OF_TUBES; ++t) {
		clock->tubes[t].current_digit
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

void tubes_data_source_set(uint16_t* first_number_src, uint16_t* second_number_src) {
	clock_data.first_number = first_number_src;
	clock_data.second_number = second_number_src;
}
