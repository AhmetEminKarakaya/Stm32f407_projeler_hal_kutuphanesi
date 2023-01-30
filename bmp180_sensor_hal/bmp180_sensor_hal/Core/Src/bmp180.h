/*
 * bmp180.h
 *
 *  Created on: Oct 8, 2022
 *      Author: ahmet
 */

#ifndef SRC_BMP180_H_
#define SRC_BMP180_H_

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS  0xEF

#define BMP180_DEVICE_CALIBRATION_START_ADDRESS 0xAA

#define BMP180_CALIBRATION_VALUE_LENGTH 22

int16_t AC1;
int16_t AC2;
int16_t AC3;
int16_t AC4;
int16_t AC5;
int16_t AC6;
int16_t B1;
int16_t B2;
int16_t MB;
int16_t MC;
int16_t MD;


void BMP180_Init();
void BMP180_Get_Calibration();
void BMP180_Get_Calibration_Value();




#endif /* SRC_BMP180_H_ */
