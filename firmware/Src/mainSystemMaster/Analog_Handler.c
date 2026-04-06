/*
 * Analog_Handler.c
 *
 *  Created on: Apr 6, 2026
 *      Author: dewap
 */


#include "main.h"
#include "mainSystem.h"

uint16_t ADC_Val[4];
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim14;

void Analog_Init(){
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Val, 4);
}

uint16_t Analog_Read_Pin(uint16_t _gpio){
	if(_gpio == AI0_Pin) return ADC_Val[3];
	else if(_gpio == AI1_Pin) return ADC_Val[2];
	else if(_gpio == AI2_Pin) return ADC_Val[1];
	else if(_gpio == AI3_Pin) return ADC_Val[0];
	else return 0;
}
