/*
 * menu.h
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_.

#include "main.h"
#include "buttons.h"
#include "settings.h"

/*
 * Menu structure should look like this:
 *
 *  Menu Name	| Description
 *
 * *1. BOOT 	- Do not have any adjustable items. Shows current versions of:
 * 				- Preview: [FW -> delay 200ms -> compile date]
 *
 * 2. MAIN 		- Shows date, time and temp.
 * 				- Preview: [ Temp <- MM:SS <- HH:MM -> DD:MM -> YYYY ]
 *
 * 4. SETTINGS:	- On/Off ambient light sensor (when off, brightness is adjusted manually)
 * 				- Manual brightness
 * 				- Timer and Alarms
 * 				- Bluetooth on/off/connect/forget
 * 				- Display style
 * 				- Transitions style
 * 				- Tubes life extension algorithm
 */

/*
 * Controls:
 * 1. Left and right short  - always either choose item or change it's value when selected.
 * 2. Enter short - is to go one step down the menu tree and
 */

typedef enum {
	STATE_DEFAULT,

	STATE_BOOT,
	STATE_OFF,
	STATE_ON,
	STATE_DISPLAY,
	STATE_SELECT,
	STATE_ADJUST,
	STATE_ANIMATE,

	NUM_STATES
} StateType;


typedef enum {
	ITEM_DEFAULT,

	ITEM_DISP_YEAR,
	ITEM_DISP_DAY_MONTH,
	ITEM_DISP_HOURS_MINUTES,
	ITEM_DISP_MINUTES_SECONDS,
	ITEM_DISP_TEMPERATURE,

	ITEM_SETTING_DATE_TIME,
	ITEM_SETTING_BRIGHTNESS,
	ITEM_SETTING_AMB_LIGHT_SENS_EN,
	ITEM_SETTING_TEMP_SENS_EN,

	ITEM_ADJUST_YEAR,
	ITEM_ADJUST_MONTH,
	ITEM_ADJUST_DATE,
	ITEM_ADJUST_HOUR,
	ITEM_ADJUST_MINUTE,
	ITEM_ADJUST_BRIGHTNESS,
	ITEM_ADJUST_AMB_LIGHT_SENS_EN,
	ITEM_ADJUST_TEMP_SENS_EN,

	NUM_ITEMS
} ItemType;

extern ItemType current_item;
//typedef struct {
//	ItemType* parent;
//	ItemType* child;
//} Hierarchy_TypeDef;

typedef struct Item {
	void* 			(*data_get)(void);     // Pointer to the data
	const char*		format;
	struct Item*	child;					// Subitem to go into
} Item_TypeDef;

typedef struct Menu {
	struct Menu* 	parent;
	Item_TypeDef*	items;
	Item_TypeDef*	cur_item;
	uint8_t 		num_items;
} Menu_TypeDef;



typedef struct {
	StateType				state;
	Menu_TypeDef*			cur_menu;
} Menu_HandleTypeDef;

extern void (*logic[NUM_ITEMS][NUM_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES])(void);
void init_menu_items(Menu_HandleTypeDef* user_menu, Item_TypeDef* user_items, uint16_t number_of_menus, uint16_t number_of_items);
void init_menu_functions(void);

void item_goto_prev(void);
void item_goto_next(void);

int GetDayMonth(void);

#endif /* INC_MENU_H_ */
