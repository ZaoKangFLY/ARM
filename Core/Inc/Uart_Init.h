#ifndef __UART_INIT_H__
#define __UART_INIT_H__
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "App.h"
#include "protocol.h"

#define Uart_Rx_Information_Size  24
#define UartHandle                huart3
#define DEBUG_USART               USART3

extern uint8_t  Uart_Rx_Buffer[]; //����UART3�����ݽ��ջ���
extern uint8_t  Uart_Rx_Data[];   //����UART3������������
extern float Receive_Position[];  //�洢���յ�����Ϣ
extern int Ce_Speed;
extern float Big_Position;
extern float Small_Position;
extern int Small_Speed;
extern int stop1;//��ת��־
extern int stop2;
extern int stop3;

void Uartx_enable(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
#endif /* __UART_INIT_H__ */

