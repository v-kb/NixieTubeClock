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




typedef struct Item {
	const char*				name;
	MenuType				menu;					// Menu, related to the Item
	Setting_TypeDef* 		s_ptr;                  // Pointer to the related setting
//	Settings_IDs 			s_id;
//	uint8_t					cyclic_change;
	uint16_t 				parent;
	uint16_t 				child;
	uint16_t 				prev;
	uint16_t 				next;
	void					(*action[NUM_OF_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES])(void* param);
} Item_TypeDef;

typedef struct {
	Item_TypeDef*			items;
	ItemType   				current_item;				// Currently selected item
	ItemType*   			current_item_ptr;				// Currently selected item
	int						is_selected;			// Shows if current item selected to change it's value
} Menu_HandleTypeDef;

//void (*menu[NUM_OF_ITEMS][MASK_LEFT|MASK_RIGHT|MASK_ENTER][NUM_OF_PRESS_TYPES])(void* param);
void init_menu_items(Menu_HandleTypeDef* user_hmenu, Item_TypeDef* user_items, uint16_t num_of_items);
void init_menu_functions(void);

#endif /* INC_MENU_H_ */
