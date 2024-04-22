#include "Tim_Init.h"

int16_t Encoder1_Overflow_Count = 0;
int16_t Encoder2_Overflow_Count = 0;
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
	/* ��������� */
	Big_TIM_SETCOUNTER();
    Small_TIM_SETCOUNTER();
	/*��ձ�־λ*/
    __HAL_TIM_CLEAR_IT(&Big_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&Small_Encoder_htim,TIM_IT_UPDATE);
	/*��ʱ�������ж�*/
    __HAL_TIM_ENABLE_IT(&Big_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&Small_Encoder_htim,TIM_IT_UPDATE);	
	
	Big_Encoder_ENABLE(); 
    Small_Encoder_ENABLE();


   
}
/*ʹ�ܶ�ʱ���ж�*/	
void TIMx_basic_enable(void)
{
	Basic_TIM_ENABLE();	
#if PID_ASSISTANT_EN
    uint32_t temp = GET_BASIC_TIM_PERIOD();     // �������ڣ���λms  
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH2, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH3, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH4, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
#endif
}

/*��ʱ���жϻص������н���PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	if(htim==(&Big_Encoder_htim))
    {
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Big_Encoder_htim))
        {   /* ���� */
            Encoder1_Overflow_Count--;
        }
        else
        {   /* ���� */
            Encoder1_Overflow_Count++;
        }	
    }  
    else if(htim==(&Small_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Small_Encoder_htim))
        {   /* ���� */
            Encoder2_Overflow_Count--;
        }
        else
        {   /* ���� */
            Encoder2_Overflow_Count++;
        }	
    }  
    else if(htim==(&Basic_htim))//1ms��һ���ж�
	{       
		Motor_Big_Set_Speed(Big_Speed);
		Motor_Small_Set_Speed(Small_Speed);
//		Motor_CeTui_Set(ROL_Angle);     
		
//Motor_Big_Set_Position(Big_Position);
// Motor_Small_Set_Position(Small_Position);
		
//		static int i =0;
//		i++;
//		if(i==1000)
//		{
//			Usart_SendString((uint8_t *)"1");
//			i=0;
//		}
	       
    }


}



