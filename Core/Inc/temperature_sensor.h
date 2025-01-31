/*
 * temperature_sensor.h
 *
 *  Created on: Jan 31, 2025
 *      Author: v_
 */

#ifndef INC_TEMPERATURE_SENSOR_H_
#define INC_TEMPERATURE_SENSOR_H_


#include "main.h"

#define TMP75_ADDR				0x90
#define FTMP75_TEMP_AD_STEP		0.0625


void Tmp75_Init(I2C_HandleTypeDef* handle);
void Write_Register(uint8_t register_pointer, uint16_t register_value);
void Read_Register(uint8_t register_pointer, uint8_t* receive_buffer);
void Read_TempCelsius(float* receive_buffer);
void Read_TempEeprom(uint16_t* receive_buffer);
void One_ShotTemp(void);


#endif /* INC_TEMPERATURE_SENSOR_H_ */
