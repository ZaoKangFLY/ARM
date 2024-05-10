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
PwmHandle pwmHandle_1;
PwmHandle pwmHandle_2;
void pwmHandle_init(float out) //�ṹ���ʼ������    
{
	pwmHandle_1.outlier=out;
	pwmHandle_2.outlier=out;
    //pwmHandle.con_val[now]=0;	
}

/*��̬����*/
void Motor_CeTui_Set(float _get)//��ǰ��̬�Ƕ�
{
	if (is_cemotor_en == 1)//ʹ��
	{	
		static int i=0;
	    static int throttle = 1550;
		static float ROL_conval = 0;//������
		static uint16_t motor_pwm_1 = 0;//ռ�ձ�1
		static uint16_t motor_pwm_2 = 0;//ռ�ձ�2
		static float _set = 0.00f;//�趨0
		//_get = deg_to_rad(_get);//��ǰ��ת��ת����
		ROL_conval = PID_calc(&Motor_Ce,_get,_set); 
		printf("set-get:%.3f ,pwm:%.2f\n\r",_set-ROL_Angle,ROL_conval);
;
//			   i++;
//	if(i==1)
//	{
//		printf("get-set:%.2f-%.1f ,pwm:%.2f\n\r",ROL_Angle,_set,ROL_conval);
//		i=0;
//	}
		motor_pwm_1 = throttle + (int)(ROL_conval);//throttleԼ����1700
		motor_pwm_2 = throttle - (int)(ROL_conval);
		
	    limit(motor_pwm_1, 1590, 1530);
        limit(motor_pwm_2, 1590, 1530);	
		printf("%d ,%d\n",motor_pwm_1,motor_pwm_2);
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
		LED1_TOGGLE();
		Motor_CeTui_Set(ROL_Angle);         
    }


}

void Control_Pwm(PwmHandle handle, int _set)
{
	handle.con_val[now]=_set;	
	if(1000<=handle.con_val[now]&&handle.con_val[now]<=2000)//�ж�ָ���Ƿ���Ч��Χ
	{
		if(handle.con_val[now]!=handle.con_val[last] )//�ж�ָ���Ƿ������ı�
		{
			if(ABS(handle.con_val[now]-handle.con_val[last])<handle.outlier)
			{
				if(handle.mode==Ce1)
				{Ce1_SETCOMPAER(handle.con_val[now]) ;}
				else 
				{Ce2_SETCOMPAER(handle.con_val[now]) ;}
				handle.con_val[last]= handle.con_val[now];
			}
		}
	}
}