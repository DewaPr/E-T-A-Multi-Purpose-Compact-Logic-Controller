/*
 * IO_Handler.c
 *
 *  Created on: Apr 6, 2026
 *      Author: dewap
 */

#include "main.h"
#include "mainSystem.h"

// ------------------------------------------- Digital Input Controller
// Digital Input Variables
uint8_t gpioAsFreqDetectorFlag = 0;

void setGpioAsFreqDetectFlag(uint8_t _bit, uint8_t _flag){
	if(_flag) gpioAsFreqDetectorFlag |= (1 << _bit);
	else gpioAsFreqDetectorFlag &= ~(1 << _bit);
}

uint8_t getPackedFreqDetectFlag(){
	return gpioAsFreqDetectorFlag;
}

uint8_t getGpioAsFreqDetectFlag(uint16_t _gpio){
	if(_gpio == DI0_Pin) return (gpioAsFreqDetectorFlag & (1 << 0));
	else if(_gpio == DI1_Pin) return (gpioAsFreqDetectorFlag & (1 << 1));
	else if(_gpio == DI2_Pin) return (gpioAsFreqDetectorFlag & (1 << 2));
	else if(_gpio == DI3_Pin) return (gpioAsFreqDetectorFlag & (1 << 3));
	else return INPUT_REQUEST_ERROR;
}

uint8_t GPIO_Read_Input_Mode(uint8_t _gpio){
	return getGpioAsFreqDetectFlag(_gpio)? INPUT_FREQ_MODE : INPUT_DIGITAL_MODE;
}

uint32_t gpioEXTILastTracker[4];
uint32_t gpioEXTIDiffTracker[4];

// Digital Input Interrupt Control
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if((GPIO_Pin == DI0_Pin) && getGpioAsFreqDetectFlag(GPIO_Pin)){
		uint32_t _nowTick = HAL_GetTick();
		gpioEXTIDiffTracker[0] = _nowTick - gpioEXTILastTracker[0];
		gpioEXTILastTracker[0] = _nowTick;
	}
	else if((GPIO_Pin == DI1_Pin) && getGpioAsFreqDetectFlag(GPIO_Pin)){
		uint32_t _nowTick = HAL_GetTick();
		gpioEXTIDiffTracker[1] = _nowTick - gpioEXTILastTracker[1];
		gpioEXTILastTracker[1] = _nowTick;
	}
	else if((GPIO_Pin == DI2_Pin) && getGpioAsFreqDetectFlag(GPIO_Pin)){
		uint32_t _nowTick = HAL_GetTick();
		gpioEXTIDiffTracker[2] = _nowTick - gpioEXTILastTracker[2];
		gpioEXTILastTracker[2] = _nowTick;
	}
	else if((GPIO_Pin == DI3_Pin) && getGpioAsFreqDetectFlag(GPIO_Pin)){
		uint32_t _nowTick = HAL_GetTick();
		gpioEXTIDiffTracker[3] = _nowTick - gpioEXTILastTracker[3];
		gpioEXTILastTracker[3] = _nowTick;
	}
}

uint8_t freqDetectData;
void setFreqDetectData(uint8_t _bit, uint8_t _state){
	if(_state) freqDetectData |= (1 << _bit);
	else freqDetectData &= ~(1 << _bit);
}

uint8_t GPIO_Read_Freq_Detect(uint16_t _gpio){
	if(_gpio == DI0_Pin && getGpioAsFreqDetectFlag(DI0_Pin)) return (freqDetectData & (1 << 0));
	else if(_gpio == DI1_Pin && getGpioAsFreqDetectFlag(DI0_Pin)) return (freqDetectData & (1 << 1));
	else if(_gpio == DI2_Pin && getGpioAsFreqDetectFlag(DI0_Pin)) return (freqDetectData & (1 << 2));
	else if(_gpio == DI3_Pin && getGpioAsFreqDetectFlag(DI0_Pin)) return (freqDetectData & (1 << 3));
	else return INPUT_REQUEST_ERROR;
}

void GPIO_Input_Init(uint16_t _gpio, uint8_t _mode){
	if(_gpio == DI0_Pin) setGpioAsFreqDetectFlag(0, _mode == INPUT_FREQ_MODE);
	else if(_gpio == DI1_Pin) setGpioAsFreqDetectFlag(1, _mode == INPUT_FREQ_MODE);
	else if(_gpio == DI2_Pin) setGpioAsFreqDetectFlag(2, _mode == INPUT_FREQ_MODE);
	else if(_gpio == DI3_Pin) setGpioAsFreqDetectFlag(3, _mode == INPUT_FREQ_MODE);
}

uint32_t samplingTime = 10;
uint32_t samplingTracker = 0;
void GPIO_Input_Sampling_Time(uint32_t _millis){
	samplingTime = _millis;
}

uint8_t digitalInputData;
void setDigitalInputData(uint8_t _bit, GPIO_PinState _state){
	if(_state) digitalInputData |= (1 << _bit);
	else digitalInputData &= ~(1 << _bit);
}

uint8_t getPackedDigitalInputData(){
	return digitalInputData;
}

uint8_t GPIO_Read_Pin(uint16_t _gpio){
	if((_gpio == DI0_Pin) && !getGpioAsFreqDetectFlag(DI0_Pin)) return (digitalInputData & (1 << 0));
	else if((_gpio == DI1_Pin) && !getGpioAsFreqDetectFlag(DI1_Pin)) return (digitalInputData & (1 << 1));
	else if((_gpio == DI2_Pin) && !getGpioAsFreqDetectFlag(DI2_Pin)) return (digitalInputData & (1 << 2));
	else if((_gpio == DI3_Pin) && !getGpioAsFreqDetectFlag(DI3_Pin)) return (digitalInputData & (1 << 3));
	else return INPUT_REQUEST_ERROR;
}

uint8_t digitalOutputData;
void setDigitalOutputData(uint8_t _bit, GPIO_PinState _state){
	if(_state) digitalOutputData |= (1 << _bit);
	else digitalOutputData &= ~(1 << _bit);
}

uint8_t getPackedDigitalOutputData(){
	return digitalOutputData;
}

void GPIO_Write_Pin(uint16_t _gpio, GPIO_PinState _state){
	if(_gpio == DI0_Pin){
		HAL_GPIO_WritePin(DI0_GPIO_Port, DI0_Pin, _state);
		setDigitalOutputData(0, _state);
	}
	else if(_gpio == DI1_Pin){
		HAL_GPIO_WritePin(DI1_GPIO_Port, DI1_Pin, _state);
		setDigitalOutputData(1, _state);
	}
	else if(_gpio == DI2_Pin){
		HAL_GPIO_WritePin(DI2_GPIO_Port, DI2_Pin, _state);
		setDigitalOutputData(2, _state);
	}
	else if(_gpio == DI3_Pin){
		HAL_GPIO_WritePin(DI3_GPIO_Port, DI3_Pin, _state);
		setDigitalOutputData(3, _state);
	}
}

void GPIO_Input_Loop(){
	// GPIO Sampling Tracker
	if(HAL_GetTick() - samplingTracker > samplingTime){
		samplingTracker = HAL_GetTick();

		if(getGpioAsFreqDetectFlag(DI0_Pin)) setDigitalInputData(0, HAL_GPIO_ReadPin(DI0_GPIO_Port, DI0_Pin));
		else setDigitalInputData(0, 0);

		if(getGpioAsFreqDetectFlag(DI1_Pin)) setDigitalInputData(1, HAL_GPIO_ReadPin(DI1_GPIO_Port, DI1_Pin));
		else setDigitalInputData(1, 0);

		if(getGpioAsFreqDetectFlag(DI2_Pin)) setDigitalInputData(2, HAL_GPIO_ReadPin(DI2_GPIO_Port, DI2_Pin));
		else setDigitalInputData(2, 0);

		if(getGpioAsFreqDetectFlag(DI3_Pin)) setDigitalInputData(3, HAL_GPIO_ReadPin(DI3_GPIO_Port, DI3_Pin));
		else setDigitalInputData(3, 0);
	}

	// Frequency Detect Tracker
	for(uint8_t i = 0; i < 4; i++){
		if(HAL_GetTick() - gpioEXTILastTracker[i] < 1000){
			if(gpioEXTIDiffTracker[i] > FREQ_TO_MILLIS(NATURAL_FREQ_LOWER_TH) && gpioEXTIDiffTracker[i] < FREQ_TO_MILLIS(NATURAL_FREQ_UPPER_TH)){
				setFreqDetectData(i, 1);
			}
		}
		else setFreqDetectData(i, 0);
	}
}
