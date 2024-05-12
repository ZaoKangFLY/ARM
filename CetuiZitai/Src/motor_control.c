#include "motor_control.h"
/*���Ʋ���*/
uint8_t is_cemotor_en = 0;
//void Motor_CeTui_Set_Speed(int _set)
//{
////     static float y=0;
//     static uint16_t con_val=1500; 
//     if(is_cemotor_en == 1)//���յ�����ָ��
//    {
//        if(1000<=_set&&_set<=2000)//�ж�ָ���Ƿ���Ч��Χ
//        {
//            if(_set!=con_val )//�ж�ָ���Ƿ������ı�
//            {
//                if(con_val<_set)
//                {
//                    while(con_val!=_set)
//                    {
//						Ce1_SETCOMPAER(_set) ;
//						Ce2_SETCOMPAER(_set) ;
//                        HAL_Delay(5);
//                        con_val++;
//                    }
//                }
//                else if(con_val>_set)
//                {
//                   while(_set!=con_val)
//                   {
//					   	Ce1_SETCOMPAER(_set) ;
//						Ce2_SETCOMPAER(_set) ;
//                       HAL_Delay(5);;
//                       con_val--;
//                   }
//                }
//              // y=COUNT(_set);
//              //  printf("\r\npwm:%d;ռ�ձ�:%.1f%%;PWM:%.1fms;����:%.2fN;",_set,((float)con_val)/200,((float)con_val)/20000*20,10*y);
//            }
//            else
//            {
//               // printf("\r\n�������ռ�ձ��ޱ仯"); 
//            }

//        }
//        else
//        {
//            //printf("\r\n�������ռ�ձȲ��ڷ�Χ"); 
//        }
//       
//    }
//}
//PwmHandle pwmHandle_1;
//PwmHandle pwmHandle_2;

void pwmHandle_Init(PwmHandle* pwm,float out,uint16_t max,uint16_t min)
{
	pwm->Outlier=out;
	pwm->Max=max;
	pwm->Min=min;
}

//void Control_Pwm(PwmHandle* pwm, int _set)//���ο���ֲ�Ժã����Լ�
//{
//	pwm->Con_val[NOW]=_set;	
//	if(1000<=pwm->Con_val[NOW]&&pwm->Con_val[NOW]<=2000)//�ж�ָ���Ƿ���Ч��Χ
//	{
//		if(pwm->Con_val[NOW]!=pwm->Con_val[LAST] )//�ж�ָ���Ƿ������ı�
//		{
//			if(ABS(pwm->Con_val[NOW]-pwm->Con_val[LAST])<pwm->Outlier)//�Ƿ�����
//			{
//				limit(_set, pwm->Max, pwm->Min);

//				if(pwm->Mode==CE1)
//				{
//					Ce1_SETCOMPAER(pwm->Con_val[NOW]);
//				}
//				else if(pwm->Mode==CE2)
//				{
//					Ce2_SETCOMPAER(pwm->Con_val[NOW]);
//				}
//				pwm->Con_val[LAST]= pwm->Con_val[NOW];
//			}
//		}
//	}
//}
uint16_t Control_Pwm(PwmHandle* pwm, int _set)//���ο���ֲ�Ժã����Լ�
{
    limit(_set, pwm->Max, pwm->Min);	
	if(_set!=pwm->Con_val[LAST] )//�ж�ָ���Ƿ������ı�
	{
		if(ABS(_set-pwm->Con_val[LAST])<pwm->Outlier)//�Ƿ�����
		{
			pwm->Con_val[LAST]= _set;
			return _set;	
		}	
	}
	return pwm->Con_val[LAST];
}





/*��̬����*/
void Motor_CeTui_Set(float _get)//��ǰ��̬�Ƕ�
{
	if (is_cemotor_en == 1)//ʹ��
	{	 
		LED1_TOGGLE();
		static int i=0;
	    static int throttle = 1520;
		static float ROL_conval = 0;//������
		static uint16_t motor_pwm_1 = 0;//ռ�ձ�1
		static uint16_t motor_pwm_2 = 0;//ռ�ձ�2
		static float _set = 0.00f;//�趨�ȶ�ˮƽ
		ROL_conval = PID_calc(&Motor_Ce,_get,_set); 
		//printf("���Ƕ�:%.2f ,��ʼROL_conval:%.2f\n\r",_set-ROL_Angle,ROL_conval);
		motor_pwm_1 = throttle + (int)(ROL_conval);//throttleԼ����1700
		motor_pwm_2 = throttle - (int)(ROL_conval);
//		Control_Pwm(&pwmHandle_1, motor_pwm_1);
//		Control_Pwm(&pwmHandle_2, motor_pwm_2);
        limit(motor_pwm_1,1530, 1510);
	    limit(motor_pwm_2,1530, 1510);
		//printf("%d ,%d\n",motor_pwm_1,motor_pwm_2);
	    Ce1_SETCOMPAER(motor_pwm_1);
	    Ce2_SETCOMPAER(motor_pwm_2);		
    is_cemotor_en = 0;
	}

}
/*��ʱ���жϻص������н���PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
     if(htim==(&Basic_htim))//10ms��һ���ж�
	{   
		Motor_CeTui_Set(ROL_Angle);   
//		static int a=0;
//		a++;
//		if(a==10)
//		{
		
//			a=0;
//		}
      
    }

}

//void Control_Pwm(PwmHandle* pwm, int _set)��ʡһ�����ڴ�
//{
//	if(1000<=_set&&_set<=2000)//�ж�ָ���Ƿ���Ч��Χ
//	{
//		if(_set!=pwm->Con_val[LAST] )//�ж�ָ���Ƿ������ı�
//		{
//			if(ABS(_set-pwm->Con_val[LAST])<pwm->Outlier)//�Ƿ�����
//			{
//				limit(_set, pwm->Max, pwm->Min);
//				if(pwm->Mode==CE1)
//				{Ce1_SETCOMPAER(_set) ;}
//				else if(pwm->Mode==CE2)
//				{Ce2_SETCOMPAER(_set) ;}
//				pwm->Con_val[LAST]= _set;
//			}
//		}
//	}
//}