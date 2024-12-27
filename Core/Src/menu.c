/*
 * menu.c
 *
 *  Created on: Dec 26, 2024
 *      Author: v_
 */
#include "menu.h"

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



Menu_HandleTypeDef* 	menu;
Item_TypeDef* 			items;
ItemType				item_parent	[200];	// Items lists, describes menu hierarchy: 	up one level,
ItemType				item_child	[200];	//											down one level (submenu),
ItemType				item_prev	[200];	//											previous item,
ItemType				item_next	[200];	//

//uint16_t 				num_of_items;

//void (*menu[NUM_OF_ITEMS][NUM_OF_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES])(void* param);

static void decrease_parameter(void* param) {
//	if(is_selected) {
//		settings_val_dec();
//		settings_save = 1;
//	} else {
//		item_goto(PREVIOUS);
//	}
}

static inline void init_items_hierarchy_horizontal(uint16_t num_of_menus) {
	ItemType current 	= 1;
	ItemType first_item = 0;

	/*
	 * For each menu we need to find ID of the next item
	 * We iterate through each item and compare it's menu type with selected menu type
	 * If it's a match, then write this item as the next
	 */
//	for(MenuType target_menu = 0; target_menu < num_of_menus; ++target_menu) {
//		for(ItemType next = 0; next < num_of_items; ++next) {
//			if(menu->items[next].menu == target_menu) {
//				/*
//				 * Find the first item in list
//				 * And break the current iteration of the cycle
//				 */
//				if(first_item == 0) {
//					first_item = current = next;
//					continue;
//				}
//
//				/*
//				 * Find the next
//				 */
//				item_next[current] = next;
//				current = next;
//			}
//		}
//		/*
//		 * Also assign the last value to be the first one
//		 */
//		item_next[current] = first_item;
//
//		/*
//		 * Reset values before searching in the next menu
//		 */
//		current = first_item = 0;
//	}
//
//	/*
//	 * To find previous item we need to simply find the array index
//	 */
//	for(ItemType current = 1; current < num_of_items; ++current) {
//		for(ItemType prev = 1; prev < num_of_items; ++prev) {
//			if(item_next[prev] == current)
//				item_prev[current] = prev;
//		}
//	}
}

static inline void init_items_hierarchy_vertical(void) {
//	for(ItemType i = 0; i < num_of_items; ++i) {
//		menu->items[i].parent 	= NO_ITEM;
//		menu->items[i].child 	= NO_ITEM;
//	}

//	item_child[ITEM_MAIN_SCREEN] 			= ITEM_ZOOM;
//	item_child[ITEM_SETTINGS_ENTER] 		= ITEM_PROFILE;
//	item_child[ITEM_PROFILE] 				= ITEM_SIGHTING_UNITS;
//	item_child[ITEM_SIGHTING] 				= ITEM_MARK_TYPE;
//	item_child[ITEM_MARK_TYPE] 				= s_ptr[MARK_TYPE].val == 0 ? ITEM_AMMO_TYPE : ITEM_GRID_X; // Need to monitor which mark type we have
//	item_child[ITEM_AMMO_TYPE] 				= ITEM_MARK_X;
//	item_child[ITEM_BAD_PIXELS_FIX_ENTER] 	= ITEM_BAD_PIXELS_FIX_AUTO;
//
//	item_parent[ITEM_ZOOM] 		        	= ITEM_MAIN_SCREEN;
//	item_parent[ITEM_SIGHTING_UNITS] 		= ITEM_PROFILE;
//	item_parent[ITEM_IDE] 					= ITEM_PROFILE;
//	item_parent[ITEM_CALIBRATION] 			= ITEM_PROFILE;
//	item_parent[ITEM_MARK_TYPE] 			= ITEM_SIGHTING;
//	item_parent[ITEM_AMMO_TYPE] 			= ITEM_MARK_TYPE;
//	item_parent[ITEM_MARK_X] 				= ITEM_AMMO_TYPE;
//	item_parent[ITEM_MARK_Y] 				= ITEM_AMMO_TYPE;
//	item_parent[ITEM_GRID_X] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_Y] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_2] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_3] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_4] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_5] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_6] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_7] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_8] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_GRID_9] 				= ITEM_MARK_TYPE;
//	item_parent[ITEM_BAD_PIXELS_FIX_AUTO] 	= ITEM_BAD_PIXELS_FIX_ENTER;
//	item_parent[ITEM_SETTINGS_EXIT] 		= ITEM_SETTINGS_ENTER;
}

void init_menu_items(Menu_HandleTypeDef* user_hmenu, Item_TypeDef* user_items, uint16_t number_of_items) {
	assert_param(user_hmenu == NULL);
	assert_param(user_items == NULL);

	menu 				= user_hmenu;
	menu->items 		= user_items;
//	num_of_items		= number_of_items;

//	menu->current_item 	= user_hmenu->items[FW_VERSION]; // Show FW version and compile date on start

	init_items_hierarchy_vertical	();
	init_items_hierarchy_horizontal	(NUM_OF_MENUS);
}

void init_menu_functions(void) {
//	for(int item = 0; item < number_of_items; ++item) {
//		for(int mask = 0; mask < NUM_OF_BTN_COMBINATIONS; ++mask) {
//			for(int press = 0; press < NUM_OF_PRESS_TYPES; ++press) {
//				menu[item][mask][press] = NULL;
//			}
//		}
//
////		menu[item][MASK_LEFT]	[CLICK] = decrease_parameter; // Goes to a previous item when "is_selected == 0" and decreases parameter
////		menu[item][MASK_RIGHT]	[CLICK] = increase_parameter;
////		menu[item][MASK_ENTER]	[CLICK] = toggle_item_selection;
////		menu[item][MASK_ENTER]	[HOLD] 	= confirm_parameter_value;
//	}

	/*
	 *
	YEAR,
	MONTH,
	DAY,
//	WEEKDAY,
	HOURS,
	MINUTES,
	SECONDS,
	TUBES_BRIGHTNESS_ADJUST,
	AMBIENT_LIGHT_SENSOR,
	TEMPERATURE_SENSOR,
	BLUETOOTH,
	 *
	 *
	 *
	 */
//	menu[YEAR][MASK_LEFT]
}
