/*
 * mainSystem.c
 *
 *  Created on: Apr 6, 2026
 *      Author: dewap
 */

#include "main.h"
#include "mainSystem.h"

void mainSystemBegin(){
	GPIO_Input_Init(DI0_Pin, INPUT_DIGITAL_MODE); // Set the DI0 as a basic digital input
	GPIO_Input_Init(DI1_Pin, INPUT_DIGITAL_MODE); // Set the DI1 as a basic digital input
	GPIO_Input_Init(DI2_Pin, INPUT_FREQ_MODE); // Set the DI2 as a frequency input
	GPIO_Input_Init(DI3_Pin, INPUT_INACTIVE); // Set the DI3 as inactive

	GPIO_Input_Sampling_Time(10); //Optional, Default: 10ms
}

void mainSystemLoop(){
	// Example of read the INPUT pin
	if(GPIO_Read_Pin(DI0_Pin)){
		//TODO: something if DI0 High
	}
	else{
		//TODO: something if DI0 High
	}

	// Example of read the INPUT Mode
	if(GPIO_Read_Input_Mode(DI2_Pin) == INPUT_FREQ_MODE){
		//TODO: something if DI2 is set to be frequency mode
	}
	else{
		//TODO: something if DI2 is set to be basic digital mode
	}

	// Example of read the INPUT Frequency
	if(GPIO_Read_Freq_Detect(DI2_Pin)){
		//TODO: something if DI2 pulsing right at 10Hz±10%
	}
	else{
		//TODO: something if DI2 pulse is miss matched
	}

	// Example of driving the Output
	GPIO_Write_Pin(DO0_Pin, 0);
	GPIO_Write_Pin(DO1_Pin, 1);
	GPIO_Write_Pin(DO2_Pin, 0);
	GPIO_Write_Pin(DO3_Pin, 1);

	// Example of read the analog value
	uint16_t val0, val1, val2, val3;

	val0 = Analog_Read_Pin(AI0_Pin);
	val1 = Analog_Read_Pin(AI1_Pin);
	val2 = Analog_Read_Pin(AI2_Pin);
	val3 = Analog_Read_Pin(AI3_Pin);
}
