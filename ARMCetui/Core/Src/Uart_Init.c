#include "Uart_Init.h"

uint8_t  Uart_Rx_Buffer[Uart_Rx_Information_Size]; //���ݻ���
uint8_t  Uart_Rx_Data[Uart_Rx_Information_Size];   //����������
uint8_t Frama_Header[2]={0xFA,0xAF};
float Receive_Position[4]={0.0f,0.0f,0.0f,0.0f};  //�洢��Ϣ
int Ce_Speed=0;
float Big_Position=0.0f;
float Small_Position=0.0f;
int Small_Speed=0;
int stop1=0;//��ת��־
int stop2=0;
int stop3=0;
/*�������մ���*/
void Uartx_enable()
{
	HAL_UART_Receive_IT(&UartHandle,Uart_Rx_Buffer,Uart_Rx_Information_Size);
   //HAL_UART_Receive_DMA(&huart3,Uart_Rx_Buffer,Uart_Rx_Information_Size);
}

/*����λ*/
static void Uart_Filter_Data(uint8_t* _header,uint8_t* _input,uint8_t* _output,uint8_t _size)   //��ֹ���ݴ�λ
{
    if(_input[0]==_header[0]&&_input[1]==_header[1])  //֡ͷλ����ȷ
    {
        memcpy(_output,_input,_size);//������ȫ�����ƹ���
    }
    else
    {
        if(_input[_size-1]==_header[0]&&_input[0]==_header[1])//֡ͷ�����һλ�����
        {
            _output[_size-1]=_input[0];
            memcpy(_output,_input+1,_size-1);//��֡ͷ��ʼ�������ݸ��Ƶ�ǰ����
        }
        else
        {
            uint8_t i=1;
            for ( i = 1; i < _size-1; ++i)
            {
                if(_input[i]==_header[0]&&_input[i+1]==_header[1])
                {
                    memcpy(_output,_input+i,_size-i);//��֡ͷ��ʼ�������ݸ��Ƶ�ǰ��
                    memcpy(_output+_size-i,_input,i); //��֡ͷǰ�����ݸ��Ƶ�����
                    break;
                }
            }
        }
    }
}
/*
static void Uart_Filter_Data(uint8_t* _header,uint8_t* _input,uint8_t* _output,uint8_t _size)   //��ֹ���ݴ�λ
{
    // ֡ͷλ����ȷ��ֱ�Ӹ�������
    if (_input[0] == _header[0] && _input[1] == _header[1])
    {
        memcpy(_output, _input, _size);//C �⺯�� void *memcpy(void *str1, const void *str2, size_t n) �Ӵ洢�� str2 ���� n ���ֽڵ��洢�� str1��
    }
    // ֡ͷ�����һλ�����
    else if (_input[_size - 1] == _header[0] && _input[0] == _header[1])
    {
        _output[0] = _header[0];
        _output[1] = _header[1];
        memcpy(_output + 2, _input+1, _size - 2);
    }
    // ��ѭ���в���֡ͷ��λ��
    else
    {
        for (uint8_t i = 1; i < _size - 1; i++)
        {
            if (_input[i] == _header[0] && _input[i + 1] == _header[1])
            {
                memcpy(_output, _input + i, _size - i);
                memcpy(_output + _size - i, _input, i);
                break;; 
            }
        }
    }
}*/


/*�����жϻص������н��մ���*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if(huart->Instance == DEBUG_USART)
    {
#if 	PID_ASSISTANT_EN
	
	
#else
		//stop1=1;
        //stop2=1;
        //stop3=1;
		is_motor_en = 1;
		Uart_Filter_Data(Frama_Header,Uart_Rx_Buffer,Uart_Rx_Data,Uart_Rx_Information_Size);//�����յ�����ɸѡ������δ��SUMУ�飩
        memcpy(Receive_Position,&Uart_Rx_Data[5],16);//�������λ����Ϣժ����
        Ce_Speed=Receive_Position[0];//ռ�ձ�
        Big_Position=Receive_Position[1];//��
        Small_Position=Receive_Position[2];
		Small_Speed=Receive_Position[3];
        printf("*****************************������********************************\r\n");
		  // HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
#endif
		HAL_UART_Receive_IT(&UartHandle,Uart_Rx_Buffer,Uart_Rx_Information_Size);//�ú����Ὺ�������жϣ���־λ UART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	}
     
}










/*****************  �����ַ� **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&UartHandle, &str, 1, 1000);
}

/*****************  �����ַ��� **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}



