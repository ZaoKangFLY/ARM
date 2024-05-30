#ifndef __UART_INIT_H__
#define __UART_INIT_H__
#include "App.h"


#define recSize  24
#define uartDMA232         hdma_usart3_rx
#define uart232               huart3
//#define Uart_485                huart2




extern int16_t Big_Position;
extern int16_t Small_Position;


extern int16_t  g_bigPosition;
extern int16_t  g_smallPosition;

extern uint16_t Ce_Speed;

void uart_enable(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
#endif /* __UART_INIT_H__ */

