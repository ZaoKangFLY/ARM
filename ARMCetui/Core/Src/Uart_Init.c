#include "uart_init.h"

uint8_t  recBuffer[recSize]; //数据缓存
uint8_t  recData[recSize];   //整理后的数据
uint8_t framaHeader[2]={0xFA,0xAF};
uint8_t framaTail=0xFE;
float recPosition[4]={0.0f,0.0f,0.0f,0.0f};  //存储信息
int16_t  g_jianPosition=0;
int16_t  g_bigPosition=0;
int16_t  g_smallPosition=0;
int16_t  g_wanPosition=0;
uint8_t     g_zhua=0;

uint16_t Ce_Speed=0;

/*开启接收串口*/
void uart_enable()
{
	__HAL_UART_ENABLE_IT(&uart232, UART_IT_IDLE);
	//HAL_UART_Receive_IT(&uart232 ,Uart_Rx_Buffer,Uart_Rx_Information_Size);//未使用DMA
    HAL_UART_Receive_DMA(&uart232,recBuffer,recSize);
}
int8_t validate_data(uint8_t *data, uint8_t length)
{
    // 这里实现你的数据校验逻辑，例如CRC校验或校验和校验
    // 返回true表示校验成功，false表示校验失败
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }

    // 假设校验和在数据的最后一个字节
    return (checksum == data[length]);
}
void process_data(uint8_t *data)
{
     memcpy(recPosition,&data[5],16);//将电机的位置信息摘出来
		g_jianPosition=recPosition[0];//占空比
		g_bigPosition=recPosition[1];
		g_smallPosition=recPosition[2];
		g_wanPosition=recPosition[3];
		g_zhua = data[21];
		// Ce_Speed=recPosition[0];//占空比
}
void handle_receidved_data(uint8_t* recBuffer, uint8_t dataSize)//遍历
{
	if (dataSize < 24)//不够一帧
	{
		return;
	}
	for (uint8_t i = 0; i < dataSize - 1; i++)
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
void handle_receidved_data_1(uint8_t* recBuffer, uint8_t dataSize)//固定帧
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
static void Uart_Filter_Data(uint8_t* _header,uint8_t* _input,uint8_t* _output,uint8_t _size)   //防止数据错位
{
    // 帧头位置正确，直接复制数据
    if (_input[0] == _header[0] && _input[1] == _header[1])
    {
        memcpy(_output, _input, _size);//C 库函数 void *memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字节到存储区 str1。
    }
    // 帧头在最后一位的情况
    else if (_input[_size - 1] == _header[0] && _input[0] == _header[1])
    {
        _output[0] = _header[0];
        _output[1] = _header[1];
        memcpy(_output + 2, _input+1, _size - 2);
    }
    // 在循环中查找帧头的位置
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


/*串口中断回调函数中接收处理*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == (&uart232) )
    {
#if 	PID_ASSISTANT_EN

#else
		if(RESET != __HAL_UART_GET_FLAG(&uart232, UART_FLAG_IDLE))   //判断是否是空闲中断
		{
			__HAL_UART_CLEAR_IDLEFLAG(&uart232);                       //清除空闲中断标志
			HAL_UART_DMAStop(&uart232);                                //停止本次DMA传输 		
			uint8_t data_length  = recSize - __HAL_DMA_GET_COUNTER(&uartDMA232);   //计算接收到的数据长度
		    handle_receidved_data(recBuffer, data_length);
			//HAL_UART_Transmit(&uart232,recBuffer,data_length,0x200);             //测试函数：将接收到的数据打印出去
			memset(recBuffer,0,data_length);			// 重置缓冲区并重新启动DMA传输                                           
			data_length = 0;
			HAL_UART_Receive_DMA(&uart232, recBuffer, recSize);      //重启开始DMA传输
         }                    
/*		g_motorEnable = 1;
		g_cemotorEnable= 1;
		Uart_Filter_Data(Frama_Header,recBuffer,recData,recSize);//将接收的数据筛选出来（未做SUM校验）
        memcpy(recPosition,&recData[5],16);//将电机的位置信息摘出来
		g_jianPosition=recPosition[0];//占空比
		g_bigPosition=recPosition[1];
		g_smallPosition=recPosition[2];
		g_wanPosition=recPosition[3];
		 Ce_Speed=recPosition[0];//占空比
        printf("*****************************新数据********************************\r\n");*/
#endif
		//HAL_UART_Receive_IT(&uart232 ,recBuffer,recSize);//该函数会开启接收中断：标志位 UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	}
//	else if(huart == (&Uart_485))
//	{
//		 //ROL_Angle=;
//	
//	}
     
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



