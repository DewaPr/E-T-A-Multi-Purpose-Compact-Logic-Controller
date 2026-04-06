/*
 * UART_Parser.c
 *
 *  Created on: Apr 6, 2026
 *      Author: dewap
 */

#include "main.h"
#include "mainSystem.h"
#include "string.h"
#include "stdio.h"

// ------------------------------------------- UART Controller
// UART Variables
extern UART_HandleTypeDef huart1;

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

// Multi-byte CRC8 (Polynomial 0x07)
uint8_t Calculate_CRC8(uint8_t *data, uint16_t length) {
    uint8_t crc = 0x00;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint8_t RxData[16];
uint8_t currentOutputMode = UART_OUT_TEXT; // Default to human-readable

// UART_Initialize
void UART_Init(){
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxData, 16);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (Size == 0) return;

	// Check for [DE] [XX] [CRC] [A1]
	if (Size == 4 && RxData[0] == 0xDE && RxData[3] == 0xA1) {
	uint8_t payload = RxData[1];
	uint8_t received_crc = RxData[2];

	// Validate the payload
	if (Calculate_CRC8((uint8_t*)&payload, 1) == received_crc) {
		uint8_t command_bits = payload & 0x0F;

		// Access the GPIO
		for(uint8_t i = 0; i < 4; i++){
			GPIO_Write_Pin(i, command_bits & (i << i));
		}

		return; // Exit function
		}
	}

	// Check for text mode

	//  embedding safety null pointer
	if (Size < 16) RxData[Size] = '\0';
	else RxData[15] = '\0';

	// Command: "uart out bits"
	if (strncmp((char*)RxData, "uart out bits", 13) == 0) {
		currentOutputMode = UART_OUT_BITS;
	}
	// Command: "uart out text"
	else if (strncmp((char*)RxData, "uart out text", 13) == 0) {
		currentOutputMode = UART_OUT_TEXT;
	}
	// Command: "enable output X" (15 characters total)
	else if (strncmp((char*)RxData, "enable output ", 14) == 0) {
		int pinTarget = RxData[14] - '0'; // Convert ASCII number to integer
		if (pinTarget >= 0 && pinTarget <= 4) {
			GPIO_Write_Pin(pinTarget, 1); // Turn ON
		}
	}
	// Command: "disable output X" (16 characters total)
	else if (strncmp((char*)RxData, "disable output ", 15) == 0) {
		int pinTarget = RxData[15] - '0'; // Convert ASCII number to integer
		if (pinTarget >= 0 && pinTarget <= 4) {
			GPIO_Write_Pin(pinTarget, 0); // Turn OFF
		}
	}
}

void sendUART(void) {
	uint8_t digitalInputModeFlag = getPackedFreqDetectFlag();
	uint8_t current_inputs       = getPackedDigitalInputData();
	uint8_t current_outputs      = getPackedDigitalOutputData();

	if (currentOutputMode == UART_OUT_TEXT) {
		char txBuffer[256];
		int offset = 0;

		// Build Digital Inputs text (4 Pins to match the 12-bit data)
		for (int i = 0; i < 4; i++) {
			uint8_t bit_state = (current_inputs >> i) & 0x01;
			uint8_t mode_flag = (digitalInputModeFlag >> i) & 0x01;

			offset += snprintf(txBuffer + offset, sizeof(txBuffer) - offset,
												 "Digital Input %d: %-4s | Mode: %s\r\n",
												 i,
												 bit_state ? "High" : "Low",
												 mode_flag ? "Freq" : "Basic");
		}

		// Build Digital Outputs text
		for (int i = 0; i < 4; i++) {
			uint8_t bit_state = (current_outputs >> i) & 0x01;

			offset += snprintf(txBuffer + offset, sizeof(txBuffer) - offset,
												 "Digital Output %d: %s\r\n",
												 i,
												 bit_state ? "High" : "Low");
		}

		offset += snprintf(txBuffer + offset, sizeof(txBuffer) - offset, "-----------------------------\r\n");

		HAL_UART_Transmit(&huart1, (uint8_t*)txBuffer, offset, 100);

	}
	else {
		uint8_t txBuffer[5];

		// Pack 12 bits into a 16-bit integer:
		// [ 0000 | Mode(4) | DI_State(4) | DO_State(4) ]
		uint16_t packed_data = ((digitalInputModeFlag & 0x0F) << 8) |
													 ((current_inputs & 0x0F) << 4) |
													 (current_outputs & 0x0F);

		txBuffer[0] = 0xDF;                            // Header
		txBuffer[1] = (packed_data >> 8) & 0xFF;       // Payload High Byte
		txBuffer[2] = (packed_data & 0xFF);            // Payload Low Byte

		// Calculate CRC only over the payload bytes (txBuffer[1] and txBuffer[2])
		txBuffer[3] = Calculate_CRC8(&txBuffer[1], 2);

		txBuffer[4] = 0xA1;                            // Tail

		// Transmit exactly 5 bytes
		HAL_UART_Transmit(&huart1, txBuffer, 5, 100);
	}
}

uint8_t uartTimeTracker = 0;
void UART_Loop(){
	if(HAL_GetTick() - uartTimeTracker > 1000){
		uartTimeTracker = HAL_GetTick();
		sendUART();
	}
}
