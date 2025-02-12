/*
 * menu.c
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */
#include "menu.h"
#include "ds3231_for_stm32_hal.h"
#include "nixie_tubes.h"

/*
 * There are 3 layers: Menu
 */

/*
 * Overall, menu structure should look like this.
 *
 *  Menu Name	| Description
 * 1. BOOT 		- Shows current version of FW and compile date. Do not have any adjustable items.
 * 2. MAIN 		- DD:MM & YYYY & HH:MM; or HH:MM & Temp;
 * 3. TIMER 	- Timer and Alarms (and mb other widgets, such as bluetooth)
 * 4. SETTINGS:
 * 				- On/Off ambient light sensor (when off, brightness is adjusted manually)
 * 				- Manual brightness
 * 				- Bluetooth on/off/connect/forget
 * 				- Display style
 * 				- Transitions style
 * 				- Tubes life extension algorithm
 */



extern TIM_HandleTypeDef htim2;



Menu_HandleTypeDef* 	menu;
Item_TypeDef* 			items;

//uint8_t duty_cycles[4] = {90, 90, 90, 90};

uint8_t day_max = 0;

void (*logic[NUM_OF_ITEMS][NUM_OF_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES][NUM_OF_SELECTIONS])(void);

static void decrease_parameter(void) {
//	if(is_selected) {
//		settings_val_dec();
//		settings_save = 1;
//	} else {
//		item_goto(PREVIOUS);
//	}
}


static inline void init_items_hierarchy_horizontal(void) {
	ItemType selected 	= 1;
	ItemType first_item = 0;

	/*
	 * For each menu we need to find ID of the next item
	 * We iterate through each item and compare it's menu type with selected menu type
	 * If it's a match, then write this item as the next
	 */
	for(MenuType target_menu = 0; target_menu < NUM_OF_MENUS; ++target_menu) {
		for(ItemType i = 0; i < NUM_OF_ITEMS; ++i) {
			if(items[i].menu == target_menu) {
				/*
				 * Find the first item in list
				 * if it was not set earlier (item == 0)
				 * And break the current iteration of the cycle
				 */
				if(first_item == 0) {
					first_item = selected = i;
					continue;
				}

				/*
				 * Find the next
				 */
				items[selected].next = i;
				selected = i;
			}
		}
		/*
		 * Also assign the last value to be the first one
		 */
		items[selected].next = first_item;

		/*
		 * Reset values before searching in the next menu
		 */
		selected = first_item = 0;
	}

	/*
	 * To find previous item we need to simply find the array index
	 */
	for(int current = 1; current < NUM_OF_ITEMS; ++current) {
		for(int prev = 1; prev < NUM_OF_ITEMS; ++prev) {
			if(items[prev].next == current)
				items[current].prev = prev;
		}
	}
}

static inline void init_items_hierarchy_vertical(void) {
	for(int i = 0; i < num_of_items; ++i) {
		items[i].parent = NO_ITEM;
		items[i].child 	= NO_ITEM;
	}

	items[YEAR].child 						= menu->items[COMPILE_DATE].child = FACTORY_RESET;
	items[DAY_MONTH].child 					= ITEM_PROFILE;
	items[HOURS_MINUTES].child 				= ITEM_SIGHTING_UNITS;
	items[MINUTES_SECONDS].child 			= ITEM_MARK_TYPE;
	items[TEMPERATURE].child 				= s_ptr[MARK_TYPE].val == 0 ? ITEM_AMMO_TYPE : ITEM_GRID_X; // Need to monitor which mark type we have
	items[BRIGHTNESS_LEVEL].child 			= ITEM_MARK_X;
	items[AMBIENT_LIGHT_SENSOR_EN].child 	= ITEM_BAD_PIXELS_FIX_AUTO;
	items[TEMPERATURE_SENSOR_EN].child 		= ITEM_BAD_PIXELS_FIX_AUTO;

	items[YEAR].parent 						= NO_ITEM;//nnot be any parents (levels above parent level)
	items[DAY_MONTH].parent 				= NO_ITEM;
	items[HOURS_MINUTES].parent 			= NO_ITEM;
	items[MINUTES_SECONDS].parent 			= NO_ITEM;

	items[TEMPERATURE].parent 				= NO_ITEM;
	items[BRIGHTNESS_LEVEL].parent 			= NO_ITEM;
	items[AMBIENT_LIGHT_SENSOR_EN].parent	= NO_ITEM;
	items[TEMPERATURE_SENSOR_EN].parent		= NO_ITEM;




/*
 * menu:
 * 		items list
 * 		level
 * 		entry point (item/s from parent menu)
 *
 */



// чтобы описать иерархию вертикальную нам нужно всего указать одного ребенка,
// а дальше все остальные дети с таким же уровнем меню найдуться

// либо создавать отдельные меню сразу по группам - внутри группы все предметы равны по уровню
// а группы связаны между собой уже по выбору (либо какие-то предметы изначально будут иметь свойство "проводника")


}

void init_menu_items(Menu_HandleTypeDef* user_menu, Item_TypeDef* user_items, uint16_t number_of_menus, uint16_t number_of_items) {
	assert_param(user_hmenu == NULL);
	assert_param(user_items == NULL);

	menu 				= user_menu;
	items 				= user_items;
//	num_of_items		= number_of_items;
//	num_of_menus		= number_of_menus;

//	menu->current_item 	= FW_VERSION; // Show FW version and compile date on start

//	init_items_hierarchy_vertical	();
	init_items_hierarchy_horizontal	();

	init_menu_functions();

	volatile HAL_StatusTypeDef status = 0;
	status = HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)&duty_cycles[0], 1);
	status = HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_2, (uint32_t *)&duty_cycles[1], 1);
	status = HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_3, (uint32_t *)&duty_cycles[2], 1);
	status = HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_4, (uint32_t *)&duty_cycles[3], 1);

	status = HAL_TIM_Base_Start_IT(&htim2);

	menu->current_item = HOURS_MINUTES;

	 tubes_data_source_set(&(items[menu->current_item].numbers[0]), &(items[menu->current_item].numbers[1]));
}

void item_goto_prev(void) {
	menu->current_item = items[menu->current_item].prev;
}

void item_goto_next(void) {
	menu->current_item = items[menu->current_item].next;
}
void item_selection_cycle(void) {
//	menu->is_selected = menu->is_selected < NUM_OF_SELECTIONS-1 ? menu->is_selected + 1 : 0;
	switch (menu->is_selected) {
		case DESELECTED:
			if(menu->current_item == YEAR) {
				menu->is_selected = SELECTED_SECOND_NUM;
				duty_cycles[0] = duty_cycles[1] = 30;
				duty_cycles[2] = duty_cycles[3] = 90;
				break;
			} else {
				menu->is_selected = SELECTED_FIRST_NUM;
				duty_cycles[0] = duty_cycles[1] = 90;
				duty_cycles[2] = duty_cycles[3] = 30;
			}
			break;
		case SELECTED_FIRST_NUM:
			menu->is_selected = SELECTED_SECOND_NUM;
			duty_cycles[0] = duty_cycles[1] = 30;
			duty_cycles[2] = duty_cycles[3] = 90;
			break;
		case SELECTED_SECOND_NUM:
			menu->is_selected = DESELECTED;
			duty_cycles[0] = duty_cycles[1] = 90;
			duty_cycles[2] = duty_cycles[3] = 90;
			break;
		default:
			break;
	}
}

void year_decrease(void) {
	items[YEAR].numbers[1] = items[YEAR].numbers[1] > 0 ? items[YEAR].numbers[1] - 1 : 99;
	DS3231_SetYear(2000 + (uint8_t)items[YEAR].numbers[1]);
}
void year_increase(void) {
	items[YEAR].numbers[1] = items[YEAR].numbers[1] < 99 ? items[YEAR].numbers[1] + 1 : 0;
	DS3231_SetYear(2000 + (uint8_t)items[YEAR].numbers[1]);
}

static uint8_t check_max_date() {

	uint8_t year = DS3231_GetYear();
	/*
	 * Calc maximum date for a month
	 */
	if(DS3231_GetMonth()%2 == 0) {
		if(DS3231_GetMonth() == 2) {	// If February
			day_max = (year%4 == 0) && (year%100 != 0) ? 29 : 28; // Check for leap year
		} else {
			day_max = 30;
		}
	} else {
		day_max = 31;
	}
}

void month_decrease(void) {
	items[DAY_MONTH].numbers[1] = items[DAY_MONTH].numbers[1] > 1 ? items[DAY_MONTH].numbers[1] - 1 : 12;
	DS3231_SetMonth((uint8_t)items[DAY_MONTH].numbers[1]);
}
void month_increase(void) {
	items[DAY_MONTH].numbers[1] = items[DAY_MONTH].numbers[1] < 11 ? items[DAY_MONTH].numbers[1] + 1 : 1;
	DS3231_SetMonth((uint8_t)items[DAY_MONTH].numbers[1]);
}

void days_decrease(void) {
	check_max_date();
	items[DAY_MONTH].numbers[0] = items[DAY_MONTH].numbers[0] > 1 ? items[DAY_MONTH].numbers[0] - 1 : day_max;
	DS3231_SetDate((uint8_t)items[DAY_MONTH].numbers[0]);
}
void days_increase(void) {
	check_max_date();
	items[DAY_MONTH].numbers[0] = items[DAY_MONTH].numbers[0] < day_max ? items[DAY_MONTH].numbers[0] + 1 : 1;
	DS3231_SetDate((uint8_t)items[DAY_MONTH].numbers[0]);
}

void hours_decrease(void) {
	items[HOURS_MINUTES].numbers[0] = items[HOURS_MINUTES].numbers[0] > 0 ? items[HOURS_MINUTES].numbers[0] - 1 : 23;
	DS3231_SetHour((uint8_t)items[HOURS_MINUTES].numbers[0]);
}
void hours_increase(void) {
	items[HOURS_MINUTES].numbers[0] = items[HOURS_MINUTES].numbers[0] < 23 ? items[HOURS_MINUTES].numbers[0] + 1 : 0;
	DS3231_SetHour((uint8_t)items[HOURS_MINUTES].numbers[0]);
}
void minutes_decrease(void) {
	items[HOURS_MINUTES].numbers[1] = items[HOURS_MINUTES].numbers[1] > 0 ? items[HOURS_MINUTES].numbers[1] - 1 : 59;
	DS3231_SetMinute((uint8_t)items[HOURS_MINUTES].numbers[1]);
	DS3231_SetSecond(0);
}
void minutes_increase(void) {
	items[HOURS_MINUTES].numbers[1] = items[HOURS_MINUTES].numbers[1] < 59 ? items[HOURS_MINUTES].numbers[1] + 1 : 0;
	DS3231_SetMinute((uint8_t)items[HOURS_MINUTES].numbers[1]);
	DS3231_SetSecond(0);
}

static void numbers_show(void) {
	HAL_GPIO_WritePin(SHDN_170V_3V3_GPIO_Port, SHDN_170V_3V3_Pin, GPIO_PIN_RESET);
}

static void numbers_hide(void) {
	HAL_GPIO_WritePin(SHDN_170V_3V3_GPIO_Port, SHDN_170V_3V3_Pin, GPIO_PIN_SET);
}

static void numbers_on_off(void) {
	static int status = 0;

	if(status == 0) {
		status = 1;
		numbers_show();
	} else {
		status = 0;
		numbers_hide();
	}

//	TOGGLE_170V();
}

static menu_toggle(void) {

}

void init_menu_functions(void) {
	for(int item = 0; item < NUM_OF_ITEMS; ++item) {
		for(int mask = 0; mask < NUM_OF_BTN_COMBINATIONS; ++mask) {
			for(int press = 0; press < NUM_OF_PRESS_TYPES; ++press) {
				for(int selection = 0; selection < NUM_OF_SELECTIONS; ++selection) {
					logic[item][mask][press][selection] 	= NULL;
				}
			}
		}
		logic[item][MASK_ENTER]	[CLICK][DESELECTED] = numbers_on_off; //item_selection_cycle;
		logic[item][MASK_ENTER]	[HOLD][DESELECTED] = item_selection_cycle;

		logic[item][MASK_LEFT]	[CLICK][DESELECTED] = item_goto_prev; // Goes to a previous item when "is_selected == 0" and decreases parameter
		logic[item][MASK_RIGHT]	[CLICK][DESELECTED] = item_goto_next; // Goes to a previous item when "is_selected == 0" and decreases parameter
		logic[item][MASK_ENTER]	[CLICK][SELECTED_FIRST_NUM] = item_selection_cycle;
		logic[item][MASK_ENTER]	[CLICK][SELECTED_SECOND_NUM] = item_selection_cycle;
//		logic[item][MASK_ENTER]	[HOLD] 	= confirm_parameter_value;
	}

	logic[YEAR][MASK_LEFT]	[CLICK][SELECTED_SECOND_NUM] = year_decrease; // Goes to a previous item when "is_selected == 0" and decreases parameter
	logic[YEAR][MASK_RIGHT][CLICK][SELECTED_SECOND_NUM] = year_increase;

	logic[DAY_MONTH][MASK_LEFT]	[CLICK][SELECTED_FIRST_NUM] = days_decrease; // Goes to a previous item when "is_selected == 0" and decreases parameter
	logic[DAY_MONTH][MASK_RIGHT][CLICK][SELECTED_FIRST_NUM] = days_increase;
	logic[DAY_MONTH][MASK_LEFT]	[CLICK][SELECTED_SECOND_NUM] = month_decrease; // Goes to a previous item when "is_selected == 0" and decreases parameter
	logic[DAY_MONTH][MASK_RIGHT][CLICK][SELECTED_SECOND_NUM] = month_increase;

	logic[HOURS_MINUTES][MASK_LEFT]	[CLICK][SELECTED_FIRST_NUM] = hours_decrease; // Goes to a previous item when "is_selected == 0" and decreases parameter
	logic[HOURS_MINUTES][MASK_RIGHT][CLICK][SELECTED_FIRST_NUM] = hours_increase;
	logic[HOURS_MINUTES][MASK_LEFT]	[CLICK][SELECTED_SECOND_NUM] = minutes_decrease; // Goes to a previous item when "is_selected == 0" and decreases parameter
	logic[HOURS_MINUTES][MASK_RIGHT][CLICK][SELECTED_SECOND_NUM] = minutes_increase;
}

//void item_displayed_numbers_update(void) {
//	switch(menu->current_item) {
//	case FW_VERSION: break;
//	case COMPILE_DATE: break;
//	case YEAR: break;
//	case DAY_MONTH: break;
//	case HOURS_MINUTES: break;
//	case MINUTES_SECONDS: break;
//	case BRIGHTNESS_ADJUST: break;
//	case AMBIENT_LIGHT_SENSOR_EN: break;
//	case TEMPERATURE_SENSOR_EN: break;
//	case FACTORY_RESET: break;
//
//	default: break;
//	}
//
//
//}
