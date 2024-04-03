#include "Tim_Init.h"

int16_t Encoder_Overflow_Count = 0;
/* 使能对应PWM通道定时器 */
void TIMx_PWM_enable(void)
{
#if PID_ASSISTANT_EN
	is_motor_en=1;
#endif
    Big1_PWM_ENABLE();
    Big2_PWM_ENABLE();
    Small1_PWM_ENABLE();
    Small2_PWM_ENABLE();
	
#if 0   
    Ce_PWM1_ENABLE();    
    Ce_PWM2_ENABLE();  
#endif
}
/* 失能对应PWM通道定时器 */
void TIMx_PWM_disable(void)
{
	is_motor_en=0;
	Big1_PWM_DISABLE();
    Big2_PWM_DISABLE();
    Small1_PWM_DISABLE();
    Small2_PWM_DISABLE();
	
#if 0   
	Ce_PWM1_DISABLE() 
	Ce_PWM2_DISABLE() 
#endif
}
/* 使能编码器定时器 */
void TIMx_econder_enable(void)
{
    //Big_Encoder_ENABLE(); 
    Small_Encoder_ENABLE();
    /* 清零计数器 */
    Small_TIM_SETCOUNTER();
   // Big_TIM_SETCOUNTER();
}
/*使能定时器中断*/	
void TIMx_basic_enable(void)
{
	Basic_TIM_ENABLE();	
#if PID_ASSISTANT_EN
  uint32_t temp = GET_BASIC_TIM_PERIOD();     // 计算周期，单位ms  
  set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &temp, 1);     // 给通道 1 发送目标值
#endif
}

//void Big_PWM_SETCOMPAER(uint32_t channel,int compare)
//{
//		switch(channel)
//	{
//		case TIM_CHANNEL_1:   __HAL_TIM_SET_COMPARE(&Big_PWM_htim,TIM_CHANNEL_1,compare);break;
//		case TIM_CHANNEL_2:	  __HAL_TIM_SET_COMPARE(&Big_PWM_htim,TIM_CHANNEL_2,compare);break;
//		case TIM_CHANNEL_3:	  __HAL_TIM_SET_COMPARE(&Big_PWM_htim,TIM_CHANNEL_3,compare);break;
//		case TIM_CHANNEL_4:	  __HAL_TIM_SET_COMPARE(&Big_PWM_htim,TIM_CHANNEL_4,compare);break;
//	}
//}

//void Small_PWM_SETCOMPAER(uint32_t channel,int compare)
//{
//		switch(channel)
//	{
//		case TIM_CHANNEL_1:   __HAL_TIM_SET_COMPARE(&Small_PWM_htim,TIM_CHANNEL_1,compare);break;
//		case TIM_CHANNEL_2:	  __HAL_TIM_SET_COMPARE(&Small_PWM_htim,TIM_CHANNEL_2,compare);break;
//		case TIM_CHANNEL_3:	  __HAL_TIM_SET_COMPARE(&Small_PWM_htim,TIM_CHANNEL_3,compare);break;
//		case TIM_CHANNEL_4:	  __HAL_TIM_SET_COMPARE(&Small_PWM_htim,TIM_CHANNEL_4,compare);break;
//	}
//}

/*定时器中断回调函数中进行PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	
    if(htim->Instance==Basic_TIM)//1ms进一次中断
	{       
      
		//Motor_Big_Set_Position(Big_Position);
       // Motor_Small_Set_Position(Small_Position);
		
		Motor_Small_Set_Speed(Small_Speed);
		//Motor_Big_Set_Speed(Big_Speed);
	
/*      if(stop2==0)
        {
            HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
            //__HAL_RCC_TIM6_CLK_DISABLE();
        }*/
/*      if(stop1==0&&stop2==0)
        {
            HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        }*/

          
    }
    if(htim->Instance==Small_Econder_TIM)
    {
        /* 判断当前计数器计数方向 */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Small_Encoder_htim))
        {   /* 下溢 */
            Encoder_Overflow_Count--;
        }
        else
        {   /* 上溢 */
            Encoder_Overflow_Count++;
        }
    }  
}



