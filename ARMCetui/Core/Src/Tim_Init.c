#include "tim_init.h"
__IO  int16_t g_jianEncoderOverflowCount = 0;
__IO  int16_t g_bigEncoderOverflowCount = 0;
__IO  int16_t g_smallEncoderOverflowCount = 0;
__IO  int16_t g_wanEncoderOverflowCount = 0;

/* 使能对应PWM通道定时器 */
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
/* 失能对应PWM通道定时器 */
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
/* 使能编码器定时器 */
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
	/*清空标志位*/
    __HAL_TIM_CLEAR_IT(&Jian_Encoder_htim,TIM_IT_UPDATE);
	__HAL_TIM_CLEAR_IT(&Big_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&Small_Encoder_htim,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&Wan_Encoder_htim,TIM_IT_UPDATE);
	/*计时器更新中断*/
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
/*使能定时器中断*/	
void tim_basic_enable(void)
{
	Basic_TIM_ENABLE();	
#if YeHuoPID
    uint32_t temp = GET_BASIC_TIM_PERIOD();     // 计算周期，单位ms  
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &temp, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH2, &temp, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH3, &temp, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH4, &temp, 1);     // 给通道 1 发送目标值
#endif
}
void tim_basic_disenable(void)
{
	 Basic_TIM_DISENABLE();
}




/*定时器中断回调函数中进行PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	if(htim==(&Basic_htim))//1ms进一次中断
	{  	
//		if(g_motorEnable == 1)
//		{
			/*static int c =0; 
			if(c==100)// ms计算一次 
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
        /* 判断当前计数器计数方向 */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Jian_Encoder_htim))
        {   /* 下溢 */
            g_jianEncoderOverflowCount--;
        }
        else
        {   /* 上溢 */
            g_jianEncoderOverflowCount++;
        }	
    }  
    else if(htim==(&Big_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* 判断当前计数器计数方向 */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Big_Encoder_htim))
        {   /* 下溢 */
            g_bigEncoderOverflowCount--;
        }
        else
        {   /* 上溢 */
            g_bigEncoderOverflowCount++;
        }	
    }  
    else if(htim==(&Small_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* 判断当前计数器计数方向 */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Small_Encoder_htim))
        {   /* 下溢 */
            g_smallEncoderOverflowCount--;
        }
        else
        {   /* 上溢 */
            g_smallEncoderOverflowCount++;
        }	
    } 
	else if(htim==(&Wan_Encoder_htim))
    {
		//printf("************************************\r\n");
        /* 判断当前计数器计数方向 */
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&Wan_Encoder_htim))
        {   /* 下溢 */
            g_wanEncoderOverflowCount--;
        }
        else
        {   /* 上溢 */
            g_wanEncoderOverflowCount++;
        }	
    }  	
	       


}



