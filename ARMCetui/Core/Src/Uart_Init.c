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
  
//	__HAL_UART_ENABLE_IT(&uart232, UART_IT_IDLE);
//    HAL_UART_Receive_DMA(&uart232,recBuffer,recSize);
	
	//HAL_UARTEx_ReceiveToIdle_DMA(&uart232 ,recBuffer,recSize);
	//__HAL_DMA_DISABLE_IT(&uartDMA232,DMA_IT_HT);
	
	//HAL_UART_Receive_IT(&uart232 ,recBuffer,recSize);//未使用DMA
}
int8_t validate_data(uint8_t *data, uint16_t length)//求和
{
    // 这里实现你的数据校验逻辑，例如CRC校验或校验和校验
    // 返回true表示校验成功，false表示校验失败
    uint8_t checksum = 0;
    for (uint8_t k = 0; k < length; k++)
    {
        checksum += data[k];
    }

    // 假设校验和在数据的最后一个字节
    return (checksum == data[length]);
}
void process_data(uint8_t *data)//赋值
{
     memcpy(recPosition,&data[5],16);//将电机的位置信息摘出来
		g_jianPosition=recPosition[0];//占空比
		g_bigPosition=recPosition[1];
		g_smallPosition=recPosition[2];
		g_wanPosition=recPosition[3];
		g_zhua = data[21];
		Ce_Speed=recPosition[0];//占空比
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


/*串口DMA中断回调函数中接收处理*/
/*void HAL_UARTx_RxEvenCallback(UART_HandleTypeDef *huart,uint16_t Size)
{	
	if(huart == (&uart232) )
    {
		if(RESET != __HAL_UART_GET_FLAG(&uart232, UART_FLAG_IDLE))   //判断是否是空闲中断
		{
			 		
			rx_len = recSize - __HAL_DMA_GET_COUNTER(&uartDMA232);   //计算接收到的数据长度
		    handle_receidved_data(recBuffer, data_length);
			HAL_UART_Transmit(&uart232,recBuffer,rx_len ,0x200);             //测试函数：将接收到的数据打印出去
			
			memset(recBuffer,0,data_length);			// 重置缓冲区并重新启动DMA传输                                           
			rx_len  = 0;
			HAL_UARTEx_ReceiveToIdle_DMA(&uart232 ,recBuffer,recSize);     //重启开始DMA传输
			__HAL_DMA_DISABLE_IT(&uartDMA232,DMA_IT_HT);// 放IRQ.
		 }
	 }
	

}*/



/*串口中断回调函数中接收处理*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   
#if PID_ASSISTANT_EN

#else
	if(huart == (&uart232) )
    {

//		if(RESET != __HAL_UART_GET_FLAG(&uart232, UART_FLAG_IDLE))   //判断是否是空闲中断
//		{
//			__HAL_UART_CLEAR_IDLEFLAG(&uart232);                       //清除空闲中断标志
//			HAL_UART_DMAStop(&uart232);                                //停止本次DMA传输 		
//			 rx_len  = recSize - __HAL_DMA_GET_COUNTER(&uartDMA232);   //计算接收到的数据长度
//		    handle_receidved_data_1(recBuffer, rx_len);
//			//HAL_UART_Transmit(&uart232,recBuffer, rx_len,0x200);             //测试函数：将接收到的数据打印出去
//			//printf("%d",data_length)
//			memset(recBuffer,0, rx_len);			// 重置缓冲区并重新启动DMA传输                                           
//			rx_len = 0;                           // 重置接收长度
//			HAL_UART_Receive_DMA(&uart232, recBuffer, recSize);     //重启开始DMA传输  放IRQ
//         }
	
	//printf("%4s %4s %6s %7s %5s %4s\n","预期角度", "实际角度", "编码器数", "总脉冲数", "占空比", "溢出次数");
	//handle_receidved_data(recBuffer, 24); 
	

	}
#endif
	
     
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



