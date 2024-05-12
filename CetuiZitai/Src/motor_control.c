#include "motor_control.h"
/*控制侧推*/
uint8_t is_cemotor_en = 0;
//void Motor_CeTui_Set_Speed(int _set)
//{
////     static float y=0;
//     static uint16_t con_val=1500; 
//     if(is_cemotor_en == 1)//接收到侧推指令
//    {
//        if(1000<=_set&&_set<=2000)//判断指令是否有效范围
//        {
//            if(_set!=con_val )//判断指令是否发生过改变
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
//              //  printf("\r\npwm:%d;占空比:%.1f%%;PWM:%.1fms;推力:%.2fN;",_set,((float)con_val)/200,((float)con_val)/20000*20,10*y);
//            }
//            else
//            {
//               // printf("\r\n输入侧推占空比无变化"); 
//            }

//        }
//        else
//        {
//            //printf("\r\n输入侧推占空比不在范围"); 
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

//void Control_Pwm(PwmHandle* pwm, int _set)//传参可移植性好，测试简单
//{
//	pwm->Con_val[NOW]=_set;	
//	if(1000<=pwm->Con_val[NOW]&&pwm->Con_val[NOW]<=2000)//判断指令是否有效范围
//	{
//		if(pwm->Con_val[NOW]!=pwm->Con_val[LAST] )//判断指令是否发生过改变
//		{
//			if(ABS(pwm->Con_val[NOW]-pwm->Con_val[LAST])<pwm->Outlier)//是否跳变
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
uint16_t Control_Pwm(PwmHandle* pwm, int _set)//传参可移植性好，测试简单
{
    limit(_set, pwm->Max, pwm->Min);	
	if(_set!=pwm->Con_val[LAST] )//判断指令是否发生过改变
	{
		if(ABS(_set-pwm->Con_val[LAST])<pwm->Outlier)//是否跳变
		{
			pwm->Con_val[LAST]= _set;
			return _set;	
		}	
	}
	return pwm->Con_val[LAST];
}





/*姿态控制*/
void Motor_CeTui_Set(float _get)//当前姿态角度
{
	if (is_cemotor_en == 1)//使能
	{	 
		LED1_TOGGLE();
		static int i=0;
	    static int throttle = 1520;
		static float ROL_conval = 0;//控制量
		static uint16_t motor_pwm_1 = 0;//占空比1
		static uint16_t motor_pwm_2 = 0;//占空比2
		static float _set = 0.00f;//设定稳定水平
		ROL_conval = PID_calc(&Motor_Ce,_get,_set); 
		//printf("误差角度:%.2f ,初始ROL_conval:%.2f\n\r",_set-ROL_Angle,ROL_conval);
		motor_pwm_1 = throttle + (int)(ROL_conval);//throttle约等于1700
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
/*定时器中断回调函数中进行PWM*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
     if(htim==(&Basic_htim))//10ms进一次中断
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

//void Control_Pwm(PwmHandle* pwm, int _set)更省一部分内存
//{
//	if(1000<=_set&&_set<=2000)//判断指令是否有效范围
//	{
//		if(_set!=pwm->Con_val[LAST] )//判断指令是否发生过改变
//		{
//			if(ABS(_set-pwm->Con_val[LAST])<pwm->Outlier)//是否跳变
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