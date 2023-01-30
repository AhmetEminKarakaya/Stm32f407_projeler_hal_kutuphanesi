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


	AC1 = (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	AC2 = (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	AC3 = (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	AC4 = (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	AC5 = (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	AC6 = (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	B1 =  (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	B2 =  (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	MB =  (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	MC =  (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;
	MD =  (int16_t)callbuff[a] << 8 | callbuff[a+1]; a+=2;


	if(AC1 == 0x0000  || AC1 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
	if(AC2 == 0x0000  || AC2 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(AC3 == 0x0000  || AC3 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(AC4 == 0x0000  || AC4 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(AC5 == 0x0000  || AC5 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(AC6 == 0x0000  || AC6 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(B1 == 0x0000  || B1 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(B2 == 0x0000  || B2 == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(MB == 0x0000  || MB == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(MC == 0x0000  || MC == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
	if(MD == 0x0000  || MD == 0xFFFF){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}



}

void BMP180_Get_Temperature(){

	BMP180_Get_Calibration_Value();

	tX1 = ((int32_t)unCompTemperature - (int32_t)AC6) * (int32_t) AC5 * ((float) 0.000030517578125);
	tX2 = ((int32_t)MC * 2048) / (tX1 + (int32_t)MD);

	if(tX1 == 0 && MD ==0){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		while(1);
	}

	tB5 = tX1 + tX2;
	Temperature = ((float) (tB5 + 8)) / ((float)160);
}

void BMP180_Get_Uncompansated_Temperature(){

	uint8_t wData[1];
	wData[0] = 0x2E;
	uint8_t rData[2] = {0};

	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(5);

	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 2, 100000);
	unCompTemperature = (int16_t)((rData[0]<<8) | rData[1]);

}


void BMP180_Get_Presure(){

	BMP180_Get_Uncompansated_Presure();

	pB6 = tB5 - 4000;
	pX1 = (B2 * (pB6 * pB6 / 4096)) / 2048;
	pX2 = AC2 * pB6 /2048;
	pX3 = pX1 + pX2;
	pB3 = (((AC1 * 4 + pX3) << (uint8_t)0x03) + 2) /4;
	pX1 = AC3 * pB6 /8192;
	pX2 = (B1 * (pB6 * pB6 / 4096 )) / 65536;
	pX3 = ((pX1 + pX2) + 2) / 4;
	pB4 = AC4 * (unsigned long)(pX3 + 32768) / 32768;
	pB7 = ((unsigned long)unCompPresure - pB3) * (50000 >> (uint8_t)0x03);

	if(pB7 < 0x80000000){
		presure = (pB7 * 2) / pB4;
	}
	else{
		presure = (pB7 / pB8) * 2;
	}

	pX1 = (presure / 256) * (presure /256);
	pX1 = (pX1 * 3038) / 65536;
	pX2 = (-7357 * presure) / 65536;
	presure = presure + (pX1 + pX2 + 3791) / 16;
	presureATM = presure * 0.00000986923;



}


void BMP180_Get_Uncompansated_Presure(){

	uint8_t rData[3] = {0};
	uint8_t wData[1];
	wData[0] = 0x34 | (0x03 << 6);

	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(26);

	HAL_I2C_Mem_Read(&hi2c1,BMP180_DEVICE_READ_REGISTER_ADDRESS,0xF6,1,rData,3,100000);

	unCompPresure = (rData[0]<<16 | rData[1] << 8 | rData[2] >> (8 - (uint8_t )0x03));

}






