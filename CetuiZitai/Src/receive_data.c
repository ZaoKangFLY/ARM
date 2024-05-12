#include <stdio.h>
#include "receive_data.h"
#include <stdlib.h>
#include <string.h>
#define _head_1 0xFA        //֡ͷ
#define _head_2 0xAF        //֡ͷ
#define  _mode_1 0xF1        //
#define  _mode_2 0xF2       //
PUartHandle pUartHandle;

void Init_data_array(uint8_t *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0; // ��ÿ��Ԫ�ض�����Ϊ0
    }
}


void pUartHandle_init(void) //�ṹ���ʼ������    
{

    pUartHandle.step = 0;

    pUartHandle.LenCnt = 0;
	pUartHandle.LenCnt_2 = 0;

    pUartHandle.aRxBufIndex = 0;
	
	pUartHandle.sum = 0;
	
	
}
/*
���պ���
��һ�������ǽ��ܵ����ַ����ڶ���Ϊ��������
����������Ҫ�û��Զ���
�Һ������ڽ����ж��������У���֤ʵʱ��
*/
void Receive_Data(PUartHandle *pUartHandle,uint8_t str, uint8_t* Data)
{

    switch (pUartHandle->step)
    {
    case 0:
        if (str == _head_1)   //֡ͷ����
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str; /*���뻺��������Ӧλ*/
			
        }
        break;
		
    case 1:
        if (str == _head_2)  //�ڶ���֡ͷ����
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;

        }
        else if (str == _head_1) //��ֹ֡ͷ�������ظ�FA FA 
		{  
			pUartHandle->step = 1;
		}
        else
        {
            pUartHandle_init();
        }
        break;

    case 2:
        if (str == _head_1)//��ֹ֡ͷ�������ظ�FA AF FA
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
        if (str == _head_1)//��ֹ֡ͷ�������ظ�FA AF F1 FA
        {
            pUartHandle->step = 1;
            pUartHandle->aRxBufIndex = 1;
        }
        else
        {
			
            pUartHandle->LenCnt = (int)str; //�����ݳ��ȸ�ֵ��TMPCNT
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;                           /*��ʼ��������*/
            if (((17 - pUartHandle->aRxBufIndex) < str+1) || (str == 0x00)) //��ֹ������������յ�����ֵ
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
        if (pUartHandle->LenCnt == 0) //˵�����ݶν������
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
//            Init_data_array(pUartHandle.aRxBuf_1);  //ÿ����һ֡���ݺ󻺳������㡣��ֹ���
//            pUartHandle_init();                       //��ʼ��
//			pUartHandle.Done=1;
//        }
//        else
//        {
//            Init_data_array(pUartHandle.aRxBuf_1);  //һ֡���ݲ���,������ȫ�����㡣
//            pUartHandle_init();                      //��ʼ��
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
            //Init_data_array(pUartHandle.aRxBuf_1);  //һ֡���ݲ���,������ȫ�����㡣
            pUartHandle_init();                      //��ʼ��
        }
        break;
    case 6:
        if (str == _head_1)   //֡ͷ����
        {
            pUartHandle->step++;
            pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str; /*���뻺��������Ӧλ*/
        }
		 else
        {
            //Init_data_array(pUartHandle.aRxBuf_1);  //һ֡���ݲ���,������ȫ�����㡣
            pUartHandle_init();                      //��ʼ��
        }
        break;
		
    case 7:
        if (str == _head_2)  //�ڶ���֡ͷ����
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
		pUartHandle->LenCnt_2 = (int)str+1; //�����ݳ��ȸ�ֵ��CNT+1У��
        pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;                           /*��ʼ��������*/
        break;
  
    case 10:
		pUartHandle->LenCnt_2--;//19
		pUartHandle->aRxBuf_1[pUartHandle->aRxBufIndex++] = str;                           /*��ʼ��������*/
//        if (pUartHandle.aRxBufIndex > RX_BUF_1_LENGTH)//��ֹ������о���û��
//        {
//            pUartHandle_init();
//        }
        if (pUartHandle->LenCnt_2 == 0) //˵���������
        {
//          strcpy(Data, pUartHandle.aRxBuf_1);
			Init_data_array(pUartHandle->aRxBuf_1, RX_BUF_1_LENGTH);    //ÿ����һ֡���ݺ󻺳������㡣��ֹ���
            pUartHandle_init();                       //��ʼ��
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




