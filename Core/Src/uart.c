#include "uart.h"
#include <stdio.h>

UART_HandleTypeDef *myHuart;

int rxBufferGp;									//get pointer (read)
int rxBufferPp;									//put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar;

// init device
void initUart(UART_HandleTypeDef *inHuart){
	myHuart = inHuart;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);	//h = handle arrive 3 --> Start interrupt
}

//ASCII Method//
//process received character
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	rxBuffer[rxBufferPp++] = rxChar;
	rxBufferPp %= rxBufferMax;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);
}

//get character from buffer
uint8_t getChar(){
	uint8_t result;
	if(rxBufferGp == rxBufferPp) return 0;
	result = rxBuffer[rxBufferGp++];
	rxBufferGp %= rxBufferMax;
	return result;
}

// binary data transmit , Little Endian method
void binaryTransmit(protocol_t inData){
	uint8_t txBuffer[] = {STX, 0, 0, 0, 0, 0, 0, 0, ETX};
	// data copy
	//memcpy(&txBuffer[1], &inData, 6); // copy six data
	txBuffer[1] = inData.id | 0x80;
	txBuffer[2] = inData.command | 0x80;
	txBuffer[3] = inData.data | 0x80;
	txBuffer[4] = (inData.data >> 7) | 0x80;
	txBuffer[5] = (inData.data >> 14) | 0x80;
	txBuffer[6] = (inData.data >> 21) | 0x80;
	// CRC calculation
	for(int i = 0; i < 7 ; i++){
		txBuffer[7] += txBuffer[i];
	}
	// transmit
	HAL_UART_Transmit(myHuart, txBuffer, sizeof(txBuffer), 10);
}

int _write(int file, char *p, int len) {
   HAL_UART_Transmit(myHuart, p, len, 10);
   return len;
}

