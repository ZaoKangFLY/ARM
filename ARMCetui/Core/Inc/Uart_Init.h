#ifndef __UART_INIT_H__
#define __UART_INIT_H__
#include "App.h"


#define recSize  48
#define uartDMA232         		hdma_usart3_rx
#define uart232               huart3
//#define Uart_485                huart2
extern uint16_t rx_len;
extern  uint8_t  recBuffer[recSize]; //Êý¾Ý»º´æ
extern int16_t  g_jianPosition;
extern int16_t  g_smallPosition;
extern int16_t  g_bigPosition;
extern int16_t  g_wanPosition;
extern uint8_t  g_zhua;
extern uint16_t Ce_Speed;

void uart_enable(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
void handle_receidved_data(uint8_t* recBuffer, uint16_t dataSize);
void handle_receidved_data_1(uint8_t* recBuffer, uint8_t dataSize);
#endif /* __UART_INIT_H__ */

