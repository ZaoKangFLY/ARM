#include "uart_init.h"

uint8_t  recBuffer[recSize]; //���ݻ���
uint8_t  recData[recSize];   //����������
uint8_t Frama_Header[2]={0xFA,0xAF};
float recPosition[4]={0.0f,0.0f,0.0f,0.0f};  //�洢��Ϣ

int16_t  g_bigPosition=0;
int16_t  g_smallPosition=0;
uint16_t Ce_Speed=0;

/*�������մ���*/
void uart_enable()
{

	__HAL_UART_ENABLE_IT(&uart232, UART_IT_IDLE);
	//HAL_UART_Receive_IT(&uart232 ,Uart_Rx_Buffer,Uart_Rx_Information_Size);//δʹ��DMA
    HAL_UART_Receive_DMA(&uart232,recBuffer,recSize);
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
	

    if(huart == (&uart232) )
    {
#if 	PID_ASSISTANT_EN
	
	
#else
	if(RESET != __HAL_UART_GET_FLAG(&uart232, UART_FLAG_IDLE))   //�ж��Ƿ��ǿ����ж�
	{
		__HAL_UART_CLEAR_IDLEFLAG(&uart232);                       //��������жϱ�־
		HAL_UART_DMAStop(&uart232);                                //ֹͣ����DMA����    
		uint8_t data_length  = recSize - __HAL_DMA_GET_COUNTER(&uartDMA232);   //������յ������ݳ���
		HAL_UART_Transmit(&uart232,recBuffer,data_length,0x200);             //���Ժ����������յ������ݴ�ӡ��ȥ
		memset(recBuffer,0,data_length);                                            
		data_length = 0;
		HAL_UART_Receive_DMA(&uart232, recBuffer, recSize);                    //������ʼDMA����
    }                    
		g_motorEnable = 1;
		g_cemotorEnable= 1;
		Uart_Filter_Data(Frama_Header,recBuffer,recData,recSize);//�����յ�����ɸѡ������δ��SUMУ�飩
        memcpy(recPosition,&recData[5],16);//�������λ����Ϣժ����
		//g_jianPosition=receivePosition[0];//ռ�ձ�
		g_bigPosition=recPosition[1];
		g_smallPosition=recPosition[2];
		//g_wanPosition=recPosition[3];
		 Ce_Speed=recPosition[0];//ռ�ձ�
       // printf("*****************************������********************************\r\n");
#endif
		HAL_UART_Receive_IT(&uart232 ,recBuffer,recSize);//�ú����Ὺ�������жϣ���־λ UART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	}
//	else if(huart == (&Uart_485))
//	{
//		 //ROL_Angle=;
//	
//	}
     
}






/*****************  �����ַ� **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&uart232 , &str, 1, 1000);
}

/*****************  �����ַ��� **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&uart232 ,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&uart232 , (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}



