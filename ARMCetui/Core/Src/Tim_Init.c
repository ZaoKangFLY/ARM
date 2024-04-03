#include "Tim_Init.h"

int16_t Encoder_Overflow_Count = 0;
/* ʹ�ܶ�ӦPWMͨ����ʱ�� */
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
/* ʧ�ܶ�ӦPWMͨ����ʱ�� */
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
/* ʹ�ܱ�������ʱ�� */
void TIMx_econder_enable(void)
{
    //Big_Encoder_ENABLE(); 
    Small_Encoder_ENABLE();
    /* ��������� */
    Small_TIM_SETCOUNTER();
   // Big_TIM_SETCOUNTER();
}
/*ʹ�ܶ�ʱ���ж�*/	
void TIMx_basic_enable(void)
{
	Basic_TIM_ENABLE();	
#if PID_ASSISTANT_EN
  uint32_t temp = GET_BASIC_TIM_PERIOD();     // �������ڣ���λms  
  set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
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

/*��ʱ���жϻص������н���PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	
    if(htim->Instance==Basic_TIM)//1ms��һ���ж�
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
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Small_Encoder_htim))
        {   /* ���� */
            Encoder_Overflow_Count--;
        }
        else
        {   /* ���� */
            Encoder_Overflow_Count++;
        }
    }  
}



