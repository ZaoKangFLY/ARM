#include "uart_init.h"

uint8_t  Uart_Rx_Buffer[Uart_Rx_Information_Size]; //数据缓存
uint8_t  Uart_Rx_Data[Uart_Rx_Information_Size];   //整理后的数据
uint8_t Frama_Header[2]={0xFA,0xAF};
float receivePosition[4]={0.0f,0.0f,0.0f,0.0f};  //存储信息

int16_t  g_bigPosition=0;
int16_t  g_smallPosition=0;
uint16_t Ce_Speed=0;

/*开启接收串口*/
void uart_enable()
{
	HAL_UART_Receive_IT(&Uart_232 ,Uart_Rx_Buffer,Uart_Rx_Information_Size);//未使用DMA
    //HAL_UART_Receive_DMA(&huart3,Uart_Rx_Buffer,Uart_Rx_Information_Size);
}

/*防错位*/
static void Uart_Filter_Data(uint8_t* _header,uint8_t* _input,uint8_t* _output,uint8_t _size)   //防止数据错位
{
    if(_input[0]==_header[0]&&_input[1]==_header[1])  //帧头位置正确
    {
        memcpy(_output,_input,_size);//将数据全部复制过来
    }
    else
    {
        if(_input[_size-1]==_header[0]&&_input[0]==_header[1])//帧头在最后一位的情况
        {
            _output[_size-1]=_input[0];
            memcpy(_output,_input+1,_size-1);//将帧头开始处的数据复制到前面来
        }
        else
        {
            uint8_t i=1;
            for ( i = 1; i < _size-1; ++i)
            {
                if(_input[i]==_header[0]&&_input[i+1]==_header[1])
                {
                    memcpy(_output,_input+i,_size-i);//将帧头开始处的数据复制到前面
                    memcpy(_output+_size-i,_input,i); //将帧头前的数据复制到后面
                    break;
                }
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

    if(huart == (&Uart_232) )
    {
#if 	PID_ASSISTANT_EN
	
	
#else
		g_motorEnable = 1;
		g_cemotorEnable= 1;
		Uart_Filter_Data(Frama_Header,Uart_Rx_Buffer,Uart_Rx_Data,Uart_Rx_Information_Size);//将接收的数据筛选出来（未做SUM校验）
        memcpy(receivePosition,&Uart_Rx_Data[5],16);//将电机的位置信息摘出来
		//g_jianPosition=receivePosition[0];//占空比
		g_bigPosition=receivePosition[1];
		g_smallPosition=receivePosition[2];
		//g_wanPosition=receivePosition[3];
		 Ce_Speed=receivePosition[0];//占空比
       // printf("*****************************新数据********************************\r\n");
#endif
		HAL_UART_Receive_IT(&Uart_232 ,Uart_Rx_Buffer,Uart_Rx_Information_Size);//该函数会开启接收中断：标志位 UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
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
  HAL_UART_Transmit(&Uart_232 , &str, 1, 1000);
}

/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&Uart_232 ,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
}

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&Uart_232 , (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}



