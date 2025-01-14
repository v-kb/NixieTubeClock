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


typedef struct Item {
	const char*				name;
	uint8_t					menu;					// Menu, related to the Item
	Setting_TypeDef* 		s_ptr;                  // Pointer to the related setting
	uint16_t 				parent;
	uint16_t 				child;
	uint16_t 				prev;
	uint16_t 				next;
	void					(*action[NUM_OF_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES][2])(void* param);
} Item_TypeDef;

typedef struct {
	Item_TypeDef*			items;
	uint16_t   				current_item;				// Currently selected item
//	uint16_t*   			current_item_ptr;				// Currently selected item
	int						is_selected;			// Shows if current item selected to change it's value
} Menu_HandleTypeDef;

//void (*menu[NUM_OF_ITEMS][MASK_LEFT|MASK_RIGHT|MASK_ENTER][NUM_OF_PRESS_TYPES])(void* param);
void init_menu_items(Menu_HandleTypeDef* user_hmenu, Item_TypeDef* user_items, uint16_t num_of_items);
void init_menu_functions(void);

#endif /* INC_MENU_H_ */
