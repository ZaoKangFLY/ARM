#include "tim_init.h"
__IO  int16_t g_jianEncoderOverflowCount = 0;
__IO  int16_t g_bigEncoderOverflowCount = 0;
__IO  int16_t g_smallEncoderOverflowCount = 0;
__IO  int16_t g_wanEncoderOverflowCount = 0;

/* ʹ�ܶ�ӦPWMͨ����ʱ�� */
void tim_pwm_enable(void)
{
#if YeHuoPID
//	g_motorEnable=1;
#endif
	Jian1_PWM_ENABLE();
    Jian2_PWM_ENABLE();
    Big1_PWM_ENABLE();
    Big2_PWM_ENABLE();
    Small1_PWM_ENABLE();
    Small2_PWM_ENABLE();
	Wan1_PWM_ENABLE();
    Wan2_PWM_ENABLE();
    Ce1_PWM_ENABLE() ;    
    Ce2_PWM_ENABLE() ;  

}
/* ʧ�ܶ�ӦPWMͨ����ʱ�� */
void tim_pwm_disable(void)
{
//	g_motorEnable=0;
	Jian1_PWM_DISABLE();
    Jian2_PWM_DISABLE();
	Big1_PWM_DISABLE();
    Big2_PWM_DISABLE();
    Small1_PWM_DISABLE();
    Small2_PWM_DISABLE();
	Wan1_PWM_DISABLE();
    Wan2_PWM_DISABLE();
//	Ce1_PWM_DISABLE();
//	Ce2_PWM_DISABLE();

}
/* ʹ�ܱ�������ʱ�� */
void tim_econder_enable(void)
{
	g_jianEncoderOverflowCount = 0;
	g_bigEncoderOverflowCount = 0;
	g_smallEncoderOverflowCount = 0;
	g_wanEncoderOverflowCount = 0;
	Jian_TIM_SETCOUNTER();          
	Big_TIM_SETCOUNTER() ;              
	Small_TIM_SETCOUNTER() ;        
	Wan_TIM_SETCOUNTER() ; 
	/*��ձ�־λ*/
    __HAL_TIM_CLEAR_IT(&Jian_Encoder_htim,TIM_IT_UPDATE);
	__HAL_TIM_CLEAR_IT(&Big_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&Small_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&Wan_Encoder_htim,TIM_IT_UPDATE);
	/*��ʱ�������ж�*/
	__HAL_TIM_ENABLE_IT(&Jian_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&Big_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&Small_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&Wan_Encoder_htim,TIM_IT_UPDATE);	
	
	Jian_Encoder_ENABLE(); 
	Big_Encoder_ENABLE(); 
    Small_Encoder_ENABLE();
	Wan_Encoder_ENABLE(); 

   
}
void tim_econder_disenable(void)
{
	Big_Encoder_DISENABLE()  ;                
	Small_Encoder_DISENABLE() ;	
   
}
/*ʹ�ܶ�ʱ���ж�*/	
void tim_basic_enable(void)
{
	Basic_TIM_ENABLE();	
#if YeHuoPID
    uint32_t temp = GET_BASIC_TIM_PERIOD();     // �������ڣ���λms  
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH2, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH3, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH4, &temp, 1);     // ��ͨ�� 1 ����Ŀ��ֵ
#endif
}
void tim_basic_disenable(void)
{
	 Basic_TIM_DISENABLE();
}




/*��ʱ���жϻص������н���PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	if(htim==(&Basic_htim))//1ms��һ���ж�
	{  	
//		if(g_motorEnable == 1)
//		{
			/*static int c =0; 
			if(c==100)// ms����һ�� 
			{
				printf("\n%d----%d\n",g_bigPosition,g_smallPosition);
				c=0;
			}
			c++;*/
			
			//jian_set_postion(g_jianPosition);
			big_set_postion(g_bigPosition);
			//small_set_postion(g_smallPosition);
			//wan_set_postion(g_wanPosition);
			//zhua_set_postion( g_zhua);
			//big_set_angle(g_bigPosition);
			small_set_angle(g_smallPosition);
//		}

	}
	else if(htim==(&Jian_Encoder_htim))
    {
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Jian_Encoder_htim))
        {   /* ���� */
            g_jianEncoderOverflowCount--;
        }
        else
        {   /* ���� */
            g_jianEncoderOverflowCount++;
        }	
    }  
    else if(htim==(&Big_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Big_Encoder_htim))
        {   /* ���� */
            g_bigEncoderOverflowCount--;
        }
        else
        {   /* ���� */
            g_bigEncoderOverflowCount++;
        }	
    }  
    else if(htim==(&Small_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Small_Encoder_htim))
        {   /* ���� */
            g_smallEncoderOverflowCount--;
        }
        else
        {   /* ���� */
            g_smallEncoderOverflowCount++;
        }	
    } 
	else if(htim==(&Wan_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* �жϵ�ǰ�������������� */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Wan_Encoder_htim))
        {   /* ���� */
            g_wanEncoderOverflowCount--;
        }
        else
        {   /* ���� */
            g_wanEncoderOverflowCount++;
        }	
    }  	
	       


}



