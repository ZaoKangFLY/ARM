#ifndef __UART_INIT_H__
#define __UART_INIT_H__
#include "App.h"


#define Uart_Rx_Information_Size  24
#define UartHandle                huart3
#define DEBUG_USART               USART3

extern uint8_t  Uart_Rx_Buffer[]; //用于UART3的数据接收缓存
extern uint8_t  Uart_Rx_Data[];   //用于UART3的整理后的数据
extern float Receive_Position[];  //存储接收到的信息

extern float Big_Position;
extern float Small_Position;

extern float Small_Speed;
extern float Big_Speed;
extern int Ce_Speed;

void Uartx_enable(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
#endif /* __UART_INIT_H__ */

