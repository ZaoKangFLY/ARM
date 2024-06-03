#include "uart_init.h"
uint16_t rx_len=0;
uint8_t  recBuffer[recSize]; //���ݻ���
uint8_t  recData[recSize];   //����������
uint8_t framaHeader[2]={0xFA,0xAF};
uint8_t framaTail=0xFE;
float recPosition[4]={0.0f,0.0f,0.0f,0.0f};  //�洢��Ϣ
int16_t  g_jianPosition=0;
int16_t  g_bigPosition=0;
int16_t  g_smallPosition=0;
int16_t  g_wanPosition=0;
uint8_t  g_zhua=0;

uint16_t Ce_Speed=0;
/*�������մ���*/
void uart_enable()
{
  
//	__HAL_UART_ENABLE_IT(&uart232, UART_IT_IDLE);
//    HAL_UART_Receive_DMA(&uart232,recBuffer,recSize);
	
	//HAL_UARTEx_ReceiveToIdle_DMA(&uart232 ,recBuffer,recSize);
	//__HAL_DMA_DISABLE_IT(&uartDMA232,DMA_IT_HT);
	
	//HAL_UART_Receive_IT(&uart232 ,recBuffer,recSize);//δʹ��DMA
}
int8_t validate_data(uint8_t *data, uint16_t length)//���
{
    // ����ʵ���������У���߼�������CRCУ���У���У��
    // ����true��ʾУ��ɹ���false��ʾУ��ʧ��
    uint8_t checksum = 0;
    for (uint8_t k = 0; k < length; k++)
    {
        checksum += data[k];
    }

    // ����У��������ݵ����һ���ֽ�
    return (checksum == data[length]);
}
void process_data(uint8_t *data)//��ֵ
{
     memcpy(recPosition,&data[5],16);//�������λ����Ϣժ����
		g_jianPosition=recPosition[0];//ռ�ձ�
		g_bigPosition=recPosition[1];
		g_smallPosition=recPosition[2];
		g_wanPosition=recPosition[3];
		g_zhua = data[21];
		Ce_Speed=recPosition[0];//ռ�ձ�
}
void handle_receidved_data(uint8_t* recBuffer, uint16_t dataSize)//��������
{
	if (dataSize < 24)//����һ֡
	{
		return;
	}
	for (uint8_t i = 0; i < dataSize ; i++)
	{
		if (recBuffer[i] == framaHeader[0]&&recBuffer[i+1] == framaHeader[1])
		{
			for (uint8_t j = i + 23; j < dataSize; j++)
			{
				if (recBuffer[j] == framaTail)
				{
					// У����յ�������
					if (validate_data(&recBuffer[i], j - i - 1))//sumУ��
					{
						// ����У��ɹ�����������
						process_data(&recBuffer[i]);
						return;
					}
					else
					{
						// ����У��ʧ�ܣ��������
						return;
					}

				}
			}
		}
	}
	

}
void handle_receidved_data_1(uint8_t* recBuffer, uint8_t dataSize)//�̶�֡����
{
	 if (dataSize == recBuffer[3])
		{
			if (recBuffer[0] == framaHeader[0] && recBuffer[1] == framaHeader[1] && recBuffer[dataSize - 1] == framaTail)
			{
				if (validate_data(recBuffer , dataSize-2) )
				{
					process_data(recBuffer);
					return;
				}
				else
				{
//					handle_error();
					return;
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


/*����DMA�жϻص������н��մ���*/
/*void HAL_UARTx_RxEvenCallback(UART_HandleTypeDef *huart,uint16_t Size)
{	
	if(huart == (&uart232) )
    {
		if(RESET != __HAL_UART_GET_FLAG(&uart232, UART_FLAG_IDLE))   //�ж��Ƿ��ǿ����ж�
		{
			 		
			rx_len = recSize - __HAL_DMA_GET_COUNTER(&uartDMA232);   //������յ������ݳ���
		    handle_receidved_data(recBuffer, data_length);
			HAL_UART_Transmit(&uart232,recBuffer,rx_len ,0x200);             //���Ժ����������յ������ݴ�ӡ��ȥ
			
			memset(recBuffer,0,data_length);			// ���û���������������DMA����                                           
			rx_len  = 0;
			HAL_UARTEx_ReceiveToIdle_DMA(&uart232 ,recBuffer,recSize);     //������ʼDMA����
			__HAL_DMA_DISABLE_IT(&uartDMA232,DMA_IT_HT);// ��IRQ.
		 }
	 }
	

}*/



/*�����жϻص������н��մ���*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   
#if PID_ASSISTANT_EN

#else
	if(huart == (&uart232) )
    {

//		if(RESET != __HAL_UART_GET_FLAG(&uart232, UART_FLAG_IDLE))   //�ж��Ƿ��ǿ����ж�
//		{
//			__HAL_UART_CLEAR_IDLEFLAG(&uart232);                       //��������жϱ�־
//			HAL_UART_DMAStop(&uart232);                                //ֹͣ����DMA���� 		
//			 rx_len  = recSize - __HAL_DMA_GET_COUNTER(&uartDMA232);   //������յ������ݳ���
//		    handle_receidved_data_1(recBuffer, rx_len);
//			//HAL_UART_Transmit(&uart232,recBuffer, rx_len,0x200);             //���Ժ����������յ������ݴ�ӡ��ȥ
//			//printf("%d",data_length)
//			memset(recBuffer,0, rx_len);			// ���û���������������DMA����                                           
//			rx_len = 0;                           // ���ý��ճ���
//			HAL_UART_Receive_DMA(&uart232, recBuffer, recSize);     //������ʼDMA����  ��IRQ
//         }
	
	//printf("%4s %4s %6s %7s %5s %4s\n","Ԥ�ڽǶ�", "ʵ�ʽǶ�", "��������", "��������", "ռ�ձ�", "�������");
	//handle_receidved_data(recBuffer, 24); 
	

	}
#endif
	
     
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



