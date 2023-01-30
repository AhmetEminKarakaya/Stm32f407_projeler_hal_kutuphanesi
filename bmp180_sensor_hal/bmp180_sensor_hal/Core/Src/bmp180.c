/*
 * bmp180.c
 *
 *  Created on: Oct 8, 2022
 *      Author: ahmet
 */

#include "bmp180.h"

void BMP180_Init(){

	if(HAL_I2C_IsDeviceReady(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 1, 100000) != HAL_OK){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}

	BMP180_Get_Calibration_Value();

}

void BMP180_Get_Calibration_Value(){

	int a=0;

	uint8_t callbuff[BMP180_CALIBRATION_VALUE_LENGTH] = {0};

	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, BMP180_DEVICE_CALIBRATION_START_ADDRESS, 1, callbuff, BMP180_CALIBRATION_VALUE_LENGTH, 100000);


	AC1 = callbuff[a] << 8 | callbuff[a+1]; a++;
	AC2 = callbuff[a] << 8 | callbuff[a+1]; a++;
	AC3 = callbuff[a] << 8 | callbuff[a+1]; a++;
	AC4 = callbuff[a] << 8 | callbuff[a+1]; a++;
	AC5 = callbuff[a] << 8 | callbuff[a+1]; a++;
	AC6 = callbuff[a] << 8 | callbuff[a+1]; a++;
	B1 = callbuff[a] << 8 | callbuff[a+1]; a++;
	B2 = callbuff[a] << 8 | callbuff[a+1]; a++;
	MB = callbuff[a] << 8 | callbuff[a+1]; a++;
	MC = callbuff[a] << 8 | callbuff[a+1]; a++;
	MD = callbuff[a] << 8 | callbuff[a+1]; a++;
}
