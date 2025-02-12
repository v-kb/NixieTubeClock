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

typedef struct {
	ItemType* parent;
	ItemType* child;
	ItemType* prev;
	ItemType* next;
} Hierarchy_TypeDef;

typedef struct Item {
//	const char*				name;
	MenuType				menu;					// Menu, related to the Item
	int* 					data_src;                  // Pointer to the related setting
	const char*				format;
	void					(*action[NUM_OF_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES])(void);
} Item_TypeDef;

typedef struct {
	Item_TypeDef*			items;
	ItemType*   			current_item;				// Currently selected item
	ItemType*				next_item;			// Shows if current item selected to change it's value
	SelectionType			selection;
} Menu_HandleTypeDef;

extern void (*logic[NUM_OF_ITEMS][NUM_OF_BTN_COMBINATIONS][NUM_OF_PRESS_TYPES][NUM_OF_SELECTIONS])(void);
void init_menu_items(Menu_HandleTypeDef* user_menu, Item_TypeDef* user_items, uint16_t number_of_menus, uint16_t number_of_items);
void init_menu_functions(void);

void item_goto_prev(void);
void item_goto_next(void);

#endif /* INC_MENU_H_ */
