#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

#define rxBufferMax							255
#define STX 										0x02 //start code (binary version)
#define ETX 										0x03 //end code (binary version)

void initUart(UART_HandleTypeDef *inHuart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t getChar();

//id, command data --> struct
typedef struct {
	uint8_t id;
	uint8_t command;
	uint32_t data;
} protocol_t;



#endif /* INC_UART_H_ */
