#ifndef _RECEIVE_DATA_H
#define _RECEIVE_DATA_H
#include <stdio.h>
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
#define RX_BUF_1_LENGTH 40//接收长度

typedef struct pos //状态指示
{
    int step;//步长
    uint16_t LenCnt;//数据长度 
	uint16_t LenCnt_2;//2数据长度 
    int aRxBufIndex;//数组位
    uint8_t aRxBuf_1[RX_BUF_1_LENGTH];//缓存数组

	uint16_t sum;//校验和
	uint8_t Done;

}PUartHandle;
float rol_reslving(uint8_t *array);
void Init_data_array(uint8_t *array, int size);
void pUartHandle_init(void);
void Receive_Data(PUartHandle *pUartHandle,uint8_t str, uint8_t *Data);
extern PUartHandle pUartHandle;
#endif
