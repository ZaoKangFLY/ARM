#include "uart_init.h"
uint16_t rx_len=0;
uint8_t  recBuffer[recSize]; //数据缓存
uint8_t  recData[recSize];   //整理后的数据
uint8_t framaHeader[2]={0xFA,0xAF};
uint8_t framaTail=0xFE;
float recPosition[4]={0.0f,0.0f,0.0f,0.0f};  //存储信息
int16_t  g_jianPosition=0;
int16_t  g_bigPosition=0;
int16_t  g_smallPosition=0;
int16_t  g_wanPosition=0;
uint8_t  g_zhua=0;
uint16_t Ce_Speed=0;

/*开启接收串口*/
void uart_enable()
{
#if UVMS
	__HAL_UART_ENABLE_IT(&uart232, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&uart232,recBuffer,recSize);
#endif
#if YeHuoPID
	HAL_UART_Receive_IT(&uart232 ,recBuffer,recSize);//未使用DMA
#endif		
	//HAL_UARTEx_ReceiveToIdle_DMA(&uart232 ,recBuffer,recSize);
	//__HAL_DMA_DISABLE_IT(&uartDMA232,DMA_IT_HT);
}
//求和FA AF 04 18 00 00 00 00 00 00 F0 C1 00 00 F0 C1 00 00 00 00 00 00 27 FE 
int8_t validate_data(uint8_t *data, uint16_t length)//求和
{
    // 这里实现你的数据校验逻辑，例如CRC校验或校验和校验
    // 返回true表示校验成功，false表示校验失败
    uint8_t checksum = 0;//5C减少计算FAAF0418
    for (uint8_t k = 0; k < length; k++)//5
    {
        checksum += data[k];
    }

    // 假设校验和在数据的最后一个字节
    return (checksum == data[length]);
}
void process_data(uint8_t *data)//赋值
{
	//tim_pwm_enable();
	memcpy(recPosition,&data[5],16);//将电机的位置信息摘出来
	g_jianPosition=recPosition[0];//占空比
	g_bigPosition=recPosition[1];
	g_smallPosition=recPosition[2];
	g_wanPosition=recPosition[3];
	g_zhua = data[21];
	Ce_Speed=recPosition[0];//占空比
	switch(g_zhua)
	{
		case 0xAF://启动
			tim_econder_enable();
			tim_basic_enable(); 
			break;
		case 0xBF://重置新起点
			tim_basic_disenable();
			tim_econder_disenable();
			tim_econder_enable();
			break;
		case 0xFF://停止
			tim_basic_disenable(); 
			break;
		case 0xFA://继续
			tim_basic_enable(); 
			break;
		 default:
		 break;
	}
/*	if(g_zhua==0xFF)//停机指令
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
	if(g_zhua==0xAF)//启动
	{
		tim_econder_enable();
		tim_basic_enable(); 	
	}
	if(g_zhua==0xBF)//重置新起点
	{
		//tim_pwm_disable();
		tim_basic_disenable();
		tim_econder_disenable();
		tim_econder_enable();
	}*/

}
void handle_receidved_data(uint8_t* recBuffer, uint16_t dataSize)//遍历查找
{
	if (dataSize < 24)//不够一帧
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
					// 校验接收到的数据
					if (validate_data(&recBuffer[i], j - i - 1))//sum校验
					{
						// 数据校验成功，处理数据
						process_data(&recBuffer[i]);
						return;
					}
					else
					{
						// 数据校验失败，处理错误
						return;
					}

				}
			}
		}
	}
	

}
void handle_receidved_data_1(uint8_t* recBuffer, uint8_t dataSize)//固定帧查找
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

/*****************  发送字符 **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&uart232 , &str, 1, 1000);
}

/*****************  发送字符串 **********************/
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

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&uart232 , (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}
