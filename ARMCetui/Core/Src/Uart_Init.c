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
#if UVMS
	__HAL_UART_ENABLE_IT(&uart232, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&uart232,recBuffer,recSize);
#endif
#if YeHuoPID
	HAL_UART_Receive_IT(&uart232 ,recBuffer,recSize);//δʹ��DMA
#endif		
	//HAL_UARTEx_ReceiveToIdle_DMA(&uart232 ,recBuffer,recSize);
	//__HAL_DMA_DISABLE_IT(&uartDMA232,DMA_IT_HT);
}
//���FA AF 04 18 00 00 00 00 00 00 F0 C1 00 00 F0 C1 00 00 00 00 00 00 27 FE 
int8_t validate_data(uint8_t *data, uint16_t length)//���
{
    // ����ʵ���������У���߼�������CRCУ���У���У��
    // ����true��ʾУ��ɹ���false��ʾУ��ʧ��
    uint8_t checksum = 0;//5C���ټ���FAAF0418
    for (uint8_t k = 0; k < length; k++)//5
    {
        checksum += data[k];
    }

    // ����У��������ݵ����һ���ֽ�
    return (checksum == data[length]);
}
void process_data(uint8_t *data)//��ֵ
{
	//tim_pwm_enable();
	memcpy(recPosition,&data[5],16);//�������λ����Ϣժ����
	g_jianPosition=recPosition[0];//ռ�ձ�
	g_bigPosition=recPosition[1];
	g_smallPosition=recPosition[2];
	g_wanPosition=recPosition[3];
	g_zhua = data[21];
	Ce_Speed=recPosition[0];//ռ�ձ�
	switch(g_zhua)
	{
		case 0xAF://����
			tim_econder_enable();
			tim_basic_enable(); 
			break;
		case 0xBF://���������
			tim_basic_disenable();
			tim_econder_disenable();
			tim_econder_enable();
			break;
		case 0xFF://ֹͣ
			tim_basic_disenable(); 
			break;
		case 0xFA://����
			tim_basic_enable(); 
			break;
		 default:
		 break;
	}
/*	if(g_zhua==0xFF)//ͣ��ָ��
	{
		//g_motorEnable = 0;
		//tim_pwm_disable();
		tim_basic_disenable(); 
		//Ce_Speed=1500;
	}
	if(g_zhua==0xFA)
	{
		tim_basic_enable(); 
	}
	if(g_zhua==0xAF)//����
	{
		tim_econder_enable();
		tim_basic_enable(); 	
	}
	if(g_zhua==0xBF)//���������
	{
		//tim_pwm_disable();
		tim_basic_disenable();
		tim_econder_disenable();
		tim_econder_enable();
	}*/

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
	 //HAL_UART_Transmit_DMA(&uart232 ,(uint8_t *)(str + k) ,1);
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
