#include <stdio.h>
#include "receive_data.h"
#include <stdlib.h>
#include <string.h>
#define _head_1 0xFA        //帧头
#define _head_2 0xAF        //帧头
#define  _mode_1 0xF1        //
#define  _mode_2 0xF2       //
PUartHandle pUartHandle;

void Init_data_array(uint8_t *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0; // 将每个元素都设置为0
    }
}


void pUartHandle_init(void) //结构体初始化函数    
{

    pUartHandle.step = 0;

    pUartHandle.LenCnt = 0;
	pUartHandle.LenCnt_2 = 0;

    pUartHandle.aRxBufIndex = 0;
	
	pUartHandle.sum = 0;
	
	
}
/*
接收函数
第一个参数是接受到的字符，第二个为接受数组
接受数组需要用户自定义
且函数放在接受中断里面运行，保证实时性
*/
void Receive_Data(PUartHandle *pUartHandle,uint8_t str, uint8_t* Data)
{

    switch (pUartHandle->step)
    {
    case 0:
        if (str == _head_1)   //帧头检验
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str; /*存入缓存数组相应位*/
			
        }
        break;
		
    case 1:
        if (str == _head_2)  //第二个帧头检验
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;

        }
        else if (str == _head_1) //防止帧头与数据重复FA FA 
		{  
			pUartHandle->step = 1;
		}
        else
        {
            pUartHandle_init();
        }
        break;

    case 2:
        if (str == _head_1)//防止帧头与数据重复FA AF FA
        {
            pUartHandle->step = 1;
            pUartHandle->aRxBufIndex = 1;
        }
        else if (str == _mode_1)
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;
        }
        else
        {
            pUartHandle_init();
        }
        break;

    case 3:
        if (str == _head_1)//防止帧头与数据重复FA AF F1 FA
        {
            pUartHandle->step = 1;
            pUartHandle->aRxBufIndex = 1;
        }
        else
        {
			
            pUartHandle->LenCnt = (int)str; //将数据长度赋值给TMPCNT
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;                           /*开始存入数据*/
            if (((17 - pUartHandle->aRxBufIndex) < str+1) || (str == 0x00)) //防止数据溢出或者收到错误值
            {
                pUartHandle_init();
            }
        }
        break;

    case 4:
        pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;
        if (pUartHandle->LenCnt-- > 0)
        {
            pUartHandle->sum += pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex-1];
        }
        if (pUartHandle->LenCnt == 0) //说明数据段接收完成
        {
			
            pUartHandle->sum = _head_1 + _head_2 + pUartHandle->aRxBuf_1[2] + pUartHandle->aRxBuf_1[3]+ pUartHandle->sum;
            pUartHandle->sum = pUartHandle->sum & 0x00FF;
            pUartHandle->step++;
        }
        break;
//    case 5:
//        if (str == pUartHandle.sum)
//        {
//            pUartHandle.aRxBuf_1[pUartHandle.aRxBufIndex] = str;
//            memcpy(Data, pUartHandle.aRxBuf_1, sizeof(pUartHandle.aRxBuf_1));//sizeof(pUartHandle.aRxBuf_1)=17
//            Init_data_array(pUartHandle.aRxBuf_1);  //每收完一帧数据后缓冲区清零。防止溢出
//            pUartHandle_init();                       //初始化
//			pUartHandle.Done=1;
//        }
//        else
//        {
//            Init_data_array(pUartHandle.aRxBuf_1);  //一帧数据不对,缓冲区全部清零。
//            pUartHandle_init();                      //初始化
//        }
//        break;
		    case 5:
        if (str == pUartHandle->sum)
        {
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;
            memcpy(Data, pUartHandle->aRxBuf_1, pUartHandle->aRxBufIndex);//pUartHandle.aRxBufIndex=17
			pUartHandle->step++;
			pUartHandle->Done=1;
        }
        else
        {
            //Init_data_array(pUartHandle.aRxBuf_1);  //一帧数据不对,缓冲区全部清零。
            pUartHandle_init();                      //初始化
        }
        break;
    case 6:
        if (str == _head_1)   //帧头检验
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str; /*存入缓存数组相应位*/
        }
		 else
        {
            //Init_data_array(pUartHandle.aRxBuf_1);  //一帧数据不对,缓冲区全部清零。
            pUartHandle_init();                      //初始化
        }
        break;
		
    case 7:
        if (str == _head_2)  //第二个帧头检验
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;

        }
        else
        {
            pUartHandle_init();
        }
        break;
    case 8:		
		if (str == _mode_2)
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;
        }
        else
        {
            pUartHandle_init();
        }
        break;			
    case 9:
	    pUartHandle->step++;
		pUartHandle->LenCnt_2 = (int)str+1; //将数据长度赋值给CNT+1校验
        pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;                           /*开始存入数据*/
        break;
  
    case 10:
		pUartHandle->LenCnt_2--;//19
		pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;                           /*开始存入数据*/
//        if (pUartHandle.aRxBufIndex > RX_BUF_1_LENGTH)//防止溢出，感觉这没用
//        {
//            pUartHandle_init();
//        }
        if (pUartHandle->LenCnt_2 == 0) //说明接收完成
        {
//          strcpy(Data, pUartHandle.aRxBuf_1);
			Init_data_array(pUartHandle->aRxBuf_1, RX_BUF_1_LENGTH);    //每收完一帧数据后缓冲区清零。防止溢出
            pUartHandle_init();                       //初始化
        }
        break;

    default:
        pUartHandle_init();
        break;
    }
}
		
float rol_reslving(uint8_t *array)
{
	    float angle;
		angle=(array[4] << 8) | array[5];
	    return (angle < 32767) ? angle * 0.01 : (angle - 65536) * 0.01;;
}




