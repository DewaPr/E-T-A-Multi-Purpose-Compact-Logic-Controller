/*
 * mainSystem.h
 *
 *  Created on: Apr 6, 2026
 *      Author: dewap
 */

#ifndef SRC_MAINSYSTEMMASTER_MAINSYSTEM_H_
#define SRC_MAINSYSTEMMASTER_MAINSYSTEM_H_



// ------------------------------------------- Main System
void mainSystemBegin();
void mainSystemLoop();

// ------------------------------------------- Analog Handler
void Analog_Init();
uint16_t Analog_Read_Pin(uint16_t _gpio);

// ------------------------------------------- UART Parser
#define UART_OUT_TEXT				0
#define UART_OUT_BITS				1

void UART_Init();
void UART_Loop();

// ------------------------------------------- IO Handler
#define INPUT_DIGITAL_FLAG	0
#define INPUT_FREQ_FLAG			1

#define INPUT_INACTIVE			0
#define INPUT_DIGITAL_MODE	1
#define INPUT_FREQ_MODE			2

#define INPUT_REQUEST_ERROR	255

#define NATURAL_FREQ_DETECTOR		10 //(Hz)
#define NATURAL_FREQ_TOLERANCE	10 //(%)

#define NATURAL_FREQ_UPPER_TH		((float)NATURAL_FREQ_DETECTOR - (float)(NATURAL_FREQ_TOLERANCE / 100.0))
#define NATURAL_FREQ_LOWER_TH		((float)NATURAL_FREQ_DETECTOR + (float)(NATURAL_FREQ_TOLERANCE / 100.0))

#define FREQ_TO_MILLIS(a)	((uint32_t)(1000 / a))

uint8_t GPIO_Read_Freq_Detect(uint16_t _gpio);
uint8_t getPackedFreqDetectFlag();
uint8_t GPIO_Read_Input_Mode(uint8_t _gpio);
void GPIO_Input_Init(uint16_t _gpio, uint8_t _mode);
void GPIO_Input_Sampling_Time(uint32_t _millis);
uint8_t GPIO_Read_Pin(uint16_t _gpio);
uint8_t getPackedDigitalInputData();
void GPIO_Write_Pin(uint16_t _gpio, GPIO_PinState _state);
uint8_t getPackedDigitalOutputData();
void GPIO_Input_Loop();

#endif /* SRC_MAINSYSTEMMASTER_MAINSYSTEM_H_ */
