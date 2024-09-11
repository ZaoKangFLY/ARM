#include "motor_control.h"
//uint8_t  g_motorEnable = 0;             // 第一次通信的电机使能
/********************************************尝试版本****************************************************/
/*稳定版以机械臂脉冲数pid控制精度高但稳定性稍差，如有需要可以改为输出轴控制或者机械臂控制*/
void big_set_angle(int16_t _set)//输出机械臂角度pid
{
		 int16_t _get = 0;//实际转动角度
		 float pwm = 0;//控制占空比
         static __IO int32_t get = 0;////实际转过脉冲数		
		get =__HAL_TIM_GET_COUNTER(&Big_Encoder_htim)+ g_bigEncoderOverflowCount*Big_Encoder_htim.Init.Period   ; //总脉冲数	
		_get=get*360/B_ARM_PULSES_PER_REVOLUTION ;//转动角度		
		pwm=PID_calc(&Pid_Big,_get,-_set);//pid计算	
		if(pwm>0)//电机控制
		{
			Big1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Big2_SETCOMPARE(0);	    	
		}
		else 
		{
			Big1_SETCOMPARE(0); 
			Big2_SETCOMPARE((uint16_t)(-pwm));
		}

#if YeHuoPID
		int16_t __get = 0;//实际转动角度
		 __get=-_get;
set_computer_value(SEND_FACT_CMD, CURVES_CH2, &__get, 4);  
#endif
#if	CeShi  
static int m =0; 
if(m==10&&pwm!=0)// ms计算一次 
{
	printf("Big（%d，%d）%d\n",_set,_get,pwm );
	m=0;
}
m++;
#endif
}
void small_set_angle(int16_t _set)//输出机械臂角度pid
{
		 int16_t _get = 0;//实际转动角度
		 float pwm = 0;//控制占空比
		static __IO int32_t get = 0;////实际转过脉冲数
		get = __HAL_TIM_GET_COUNTER(&Small_Encoder_htim)+(g_smallEncoderOverflowCount * Small_Encoder_htim.Init.Period );//读取编码器脉冲数	
		_get=get*360/S_ARM_PULSES_PER_REVOLUTION;	
		pwm=PID_calc(&Pid_Small,_get,-_set);
        if(pwm>0)//电机控制
		{
			Small1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Small2_SETCOMPARE(0);	    	
		}
		else 
		{
			Small1_SETCOMPARE(0); 
			Small2_SETCOMPARE((uint16_t)(-pwm));
		}		
		
#if YeHuoPID
 int16_t __get = 0;//实际转动角度
		 __get=-_get;
set_computer_value(SEND_FACT_CMD, CURVES_CH3, &__get, 4);  
#endif
#if	CeShi  
static int m =0; 
if(m==10&&pwm!=0)// ms计算一次 
{
	printf("Small（%d，%d）%d\n",_set,_get,pwm );
	m=0;
}
m++;
#endif
}
/*尝试速度环内环位置环外环的双环控制small*/
//速度环
void motor_small_speed(int16_t set_speed)//rpm_输出轴
{
		float pwm = 0;//控制占空比
		static __IO int32_t get = 0;////实际转过脉冲数
		static __IO int32_t get_last = 0;////上一次实际转过脉冲数
		float actual_speed=0;
		get = __HAL_TIM_GET_COUNTER(&Small_Encoder_htim)+(g_smallEncoderOverflowCount * Small_Encoder_htim.Init.Period );//读取编码器脉冲数	
		actual_speed=(get- get_last)/S_OUTPUT_SHAFT_PULSES_PER_REVOLUTION*60000;//输出轴速度
		get_last=get;
		pwm=PID_calc(&Pid_Small,actual_speed,set_speed);
		if(pwm>0)//电机控制
		{
			Small1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Small2_SETCOMPARE(0);	    	
		}
		else 
		{
			Small1_SETCOMPARE(0); 
			Small2_SETCOMPARE((uint16_t)(-pwm));
		}
}
//双环外环角度
void motor_small_speed_angle(int16_t set_angle)//rpm_输出轴
{
		float pwm = 0;						//控制占空比
		static __IO int32_t get_angle  = 0;		//实际转过脉冲数
		static __IO int32_t get_angle_last = 0;	//上一次实际转过脉冲数
		float get_speed=0;				//实际速度
		get_angle = __HAL_TIM_GET_COUNTER(&Small_Encoder_htim)+(g_smallEncoderOverflowCount * Small_Encoder_htim.Init.Period );//读取编码器脉冲数	
		get_speed=(get_angle - get_angle_last)/S_OUTPUT_SHAFT_PULSES_PER_REVOLUTION*60000;//输出轴速度
		get_angle_last=get_angle;
		pwm=PID_CascadeCalc(&cas_small_angle ,set_angle,get_angle,get_speed);
		if(pwm>0)//电机控制
		{
			Small1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Small2_SETCOMPARE(0);	    	
		}
		else 
		{
			Small1_SETCOMPARE(0); 
			Small2_SETCOMPARE((uint16_t)(-pwm));
		}

}
/*姿态控制未合并初步想法涉及动力分配*/
void cetui_set_postion(float _get)//当前姿态角度
{
	static int throttle = 1700;//
	
		static float ROL_conval = 0;//控制量
		static uint16_t motor_pwm_1 = 0;//占空比
		static uint16_t motor_pwm_2 = 0;//占空比
		static int _set = 0;//设定0
		_get = deg_to_rad(_get);//当前滚转角转弧度
		ROL_conval = PID_calc(&Pid_Ce,_get,_set); 
		motor_pwm_1 = throttle + ROL_conval;//throttle约等于1700
		motor_pwm_2 = throttle - ROL_conval;
		if(motor_pwm_1>2000){motor_pwm_1=2000;}
		if(motor_pwm_2>2000){motor_pwm_2=2000;}
		if(motor_pwm_1<1500){motor_pwm_1=1500;}
		if(motor_pwm_2<1500){motor_pwm_2=1500;}
		Ce1_SETCOMPARE(motor_pwm_1) ;
		Ce2_SETCOMPARE(motor_pwm_2) ;		



}
/*********************************************稳定版本*******************************************/
/*肩部*/
void jian_set_postion(int16_t _set)
{


		int16_t _get = 0;//实际转动角度
		 int16_t pwm = 0;//控制占空比
	     int32_t set = 0;//设定脉冲数
         static int32_t get = 0;////实际转过脉冲数	
		set=_set*J_ARM_PULSES_PER_REVOLUTION/360 ;	//设定角度对应的脉冲数 
		get =__HAL_TIM_GET_COUNTER(&Jian_Encoder_htim) + (g_jianEncoderOverflowCount * Jian_Encoder_htim.Init.Period);//读取编码器脉冲数	
		pwm=PID_calc(&Pid_Jian,get,set);
     	if(pwm>0)//电机控制
		{
			Jian1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Jian2_SETCOMPARE(0);	    	
		}
		else 
		{
			Jian1_SETCOMPARE(0); 
			Jian2_SETCOMPARE((uint16_t)(-pwm));
		}
		
		_get=get*360/J_ARM_PULSES_PER_REVOLUTION;
	
#if YeHuoPID
		//set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // 给通道 1 发送实际值				
#endif
#if CeShi   
if(pwm!=0)// ms计算一次 
{
	printf("Jian（%d，%d）（%d，%d=%d*T+%d）%d\n",_set,_get,set,get,g_jianEncoderOverflowCount,__HAL_TIM_GET_COUNTER(&Jian_Encoder_htim) ,pwm);	
}
#endif
#if VoFa
printf("%d, %d, %d, %d\n",_set,_get, set, get);
#endif

}
/*大臂*/
void big_set_postion(int16_t _set)
{
	 	
		 int16_t _get = 0;//实际转动角度
		 float pwm = 0;//控制占空比
	     int32_t set = 0;//设定脉冲数
         static __IO int32_t get = 0;////实际转过脉冲数		
		set=-(_set*B_ARM_PULSES_PER_REVOLUTION/360) ;	//设定角度对应的脉冲数 
#if YeHuoPID
//set_computer_value(SEND_FACT_CMD, CURVES_CH4, &set, 2);  // 以两个通道换取显示脉冲记得注释其他同一此通道
#endif
		get =__HAL_TIM_GET_COUNTER(&Big_Encoder_htim)+ g_bigEncoderOverflowCount*Big_Encoder_htim.Init.Period   ; //总脉冲数	
		pwm=PID_calc(&Pid_Big,get,set);//pid计算	
		if(pwm>0)//电机控制
		{
			Big1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Big2_SETCOMPARE(0);	    	
		}
		else 
		{
			Big1_SETCOMPARE(0); 
			Big2_SETCOMPARE((uint16_t)(-pwm));
		}
        _get=get*360/B_ARM_PULSES_PER_REVOLUTION ;//转动角度		
#if YeHuoPID
		//set_computer_value(SEND_FACT_CMD, CURVES_CH2, &_get, 2);  // 给通道 2 发送实际值
		//set_computer_value(SEND_FACT_CMD, CURVES_CH4, &set, 2);  // 以两个通道换取显示脉冲记得注释其他同一此通道
		//set_computer_value(SEND_FACT_CMD, CURVES_CH5, &get, 4);  
#endif
#if	CeShi  
static int n =0; 
if(n==100&&pwm!=0)// ms计算一次 
{
	printf("Big（%d，%d）（%d,%d,%d*T）%.2f\n",_set,_get,set,get,g_bigEncoderOverflowCount,pwm );
	n=0;
}
n++;
#endif
#if VoFa
printf("%d, %d, %d, %d\n",_set,_get, set, get);
#endif

}

/*小臂*/
void small_set_postion(int16_t _set)
{


		 int16_t _get = 0;//实际转动角度
		 float pwm = 0;//控制占空比
	     int32_t set = 0;//设定脉冲数
		static __IO int32_t get = 0;////实际转过脉冲数
		set=-(_set*S_ARM_PULSES_PER_REVOLUTION /360) ;	//设定角度对应的脉冲数 
#if YeHuoPID
set_computer_value(SEND_FACT_CMD, CURVES_CH4, &set, 2);  // 以两个通道换取显示脉冲记得注释其他同一此通道
#endif
		get = __HAL_TIM_GET_COUNTER(&Small_Encoder_htim)+(g_smallEncoderOverflowCount * Small_Encoder_htim.Init.Period );//读取编码器脉冲数	
		pwm=PID_calc(&Pid_Small,get,set);
        if(pwm>0)//电机控制
		{
			Small1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Small2_SETCOMPARE(0);	    	
		}
		else 
		{
			Small1_SETCOMPARE(0); 
			Small2_SETCOMPARE((uint16_t)(-pwm));
		}			
		_get=get*360/S_ARM_PULSES_PER_REVOLUTION;		
#if YeHuoPID
//set_computer_value(SEND_FACT_CMD, CURVES_CH3, &_get, 2);  // 给通道 1 发送实际值
//	set_computer_value(SEND_FACT_CMD, CURVES_CH4, &set, 2);  // 以两个通道换取显示脉冲记得注释其他同一此通道
set_computer_value(SEND_FACT_CMD, CURVES_CH5, &get, 4);  
#endif
#if	CeShi  
static int m =0; 
if(m==100&&pwm!=0)// ms计算一次 
{
	printf("Small（%d，%d）（%d,%d,%d*T）%.2f\n",_set,_get,set,get,g_smallEncoderOverflowCount,pwm );
	m=0;
}
m++;
#endif
#if VoFa
printf("%d, %d, %d, %d\n",_set,_get, set, get);
#endif

}

/*腕部*/
void wan_set_postion(int16_t _set)
{
		int16_t _get = 0;//实际转动角度
		 int16_t pwm = 0;//控制占空比
	     int32_t set = 0;//设定脉冲数
         static int32_t get = 0;////实际转过脉冲数		
		set=_set*W_ARM_PULSES_PER_REVOLUTION/360 ;	//设定角度对应的脉冲数 
		get =__HAL_TIM_GET_COUNTER(&Wan_Encoder_htim) + (g_wanEncoderOverflowCount * Wan_Encoder_htim.Init.Period);//读取编码器脉冲数	
		pwm=PID_calc(&Pid_Wan,get,set);
        if(pwm>0)//电机控制
		{
			Wan1_SETCOMPARE((uint16_t)pwm);  //同高停转
			Wan2_SETCOMPARE(0);	    	
		}
		else 
		{
			Wan1_SETCOMPARE(0); 
			Wan2_SETCOMPARE((uint16_t)(-pwm));
		}
		_get=get*360/W_ARM_PULSES_PER_REVOLUTION;		
#if YeHuoPID
		//set_computer_value(SEND_FACT_CMD, CURVES_CH4, &_get, 4);  // 给通道 4 发送实际值
#endif
#if CeShi   
if( pwm!=0)// ms计算一次 
{
	printf("Wan（%d，%d）（%d，%d=%d*T+%d）%d\n",_set,_get,set,get,g_wanEncoderOverflowCount ,__HAL_TIM_GET_COUNTER(&Wan_Encoder_htim),pwm);
}		
#endif
#if VoFa
printf("%d, %d, %d, %d\n",_set,_get, set, get);
#endif
}
/*抓*/
void zhua_set_postion(uint8_t _set)
{
	static int16_t i = 0;
	if(_set==0x0A&& i<10)//i=10待测
	{      
		Zhua1_SETCOMPARE(999);  
		Zhua2_SETCOMPARE(0);
		i++;	
	}
	else if(_set==0x0F && i>0)
	{
		Zhua1_SETCOMPARE(0);  
		Zhua2_SETCOMPARE(999);	
		i--;
	}
	else
	{
		Zhua1_SETCOMPARE(0);  //停转
		Zhua2_SETCOMPARE(0);
	}
}

/*控制侧推*/
void motor_cetui_set_postion(int _set)
{
     static uint16_t con_val=1500; 
  
	if(1000<=_set&&_set<=2000)//判断指令是否有效范围
	{
		if(_set!=con_val )//判断指令是否发生过改变
		{
		
			Ce1_SETCOMPARE(3000-(int)(_set*1.164-201.44)) ;//蓝_反浆
			Ce2_SETCOMPARE(_set) ;//红
		}
		else if(_set==1500)
		{
			Ce1_SETCOMPARE(1500) ;//
			Ce2_SETCOMPARE(1500);//
		}
	
	
//		if(_set!=con_val )//判断指令是否发生过改变
//		{
//			if(con_val<_set)
//			{
//				while(con_val!=_set)
//				{
//					Ce1_SETCOMPARE(3000-con_val) ;//
//					Ce2_SETCOMPARE(con_val) ;//
//					HAL_Delay(5);
//					con_val++;
//				}
//			}
//			else if(con_val>_set)
//			{
//			   while(_set!=con_val)
//			   {
//					Ce1_SETCOMPARE(3000-con_val) ;//
//					Ce2_SETCOMPARE(con_val);//
//				   HAL_Delay(5);
//				   con_val--;
//			   }
//			}
//		  // y=COUNT(_set);
//		  //  printf("\r\npwm:%d;占空比:%.1f%%;PWM:%.1fms;推力:%.2fN;",_set,((float)con_val)/200,((float)con_val)/20000*20,10*y);
//		}
//		else if(_set==1500)
//		{
//			Ce1_SETCOMPARE(1500) ;//
//			Ce2_SETCOMPARE(1500);//
//		}
	}   
    
}



// 弧度转角度
float rads_to_deg(float radians) {
    return radians * 180.0f / PI;
}
// 角度转弧度
float deg_to_rad(float degrees) {
    return degrees * PI / 180.0f;
}

/*侧推占空比对应力的函数计算*/
float count(float x) 
{
    const float coefficients[] = {
								-111.911,
								1017.465,
								-3802.421,
								7472.746,
								-8141.241,
								4663.670,
								-1100.139};

    float y = 0;
    float x_pow = 1.0;  // x^0
    x /= 1000;  // 将 x 缩小 1000 倍
    for (int i = 0; i < sizeof(coefficients) / sizeof(coefficients[0]); i++) 
	{
        y += coefficients[i] * x_pow;
        x_pow *= x;
    }
    return y;
}

#if 0
/*motor_t Motor_Jian;
motor_t Motor_Big;
motor_t Motor_Small;
motor_t Motor_Wan;
void motor_init(motor_t *motor, TIM_HandleTypeDef *encoder_timer, pid_t *pid, 
                int16_t *encoder_overflow_count, uint32_t encoder_period, 
                uint32_t pulses_per_revolution, void (*motor_control_function)(int16_t)) {
    motor->encoder_timer = encoder_timer;
    motor->pid = pid;
    motor->encoder_overflow_count = encoder_overflow_count;
    motor->encoder_period = encoder_period;
    motor->pulses_per_revolution = pulses_per_revolution;
    motor->motor_control_function = motor_control_function;
}
 void set_position(motor_t *motor, int16_t _set) 
{
    if (g_motorEnable == 1)
    {   
		static int _get = 0; 		// 实际转动角度
        static int16_t con_val = 0; // 控制占空比
        static int32_t set = 0;     // pid设定脉冲数
        static int32_t get = 0;     // 实际转过脉冲数
        
        set = _set * motor->pulses_per_revolution / 360; // 设定角度对应的脉冲数
        get = __HAL_TIM_GET_COUNTER(motor->encoder_timer) + 
              (*motor->encoder_overflow_count) * motor->encoder_period; // 总脉冲数
        
        con_val = (int16_t)(PID_calc(motor->pid, get, set)); // PID 计算
        motor->motor_control_function(con_val);   // 控制电机
        
        _get = get * 360 / motor->pulses_per_revolution; // 转动角度
#if YeHuoPID
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // 给通道 1 发送实际值
#else	
		static uint16_t i = 0;
		if(i ==100)// ms计算一次 
		{
			printf("%8d%18d%8d%8d%8d%8d\r\n",_set,_get,__HAL_TIM_GET_COUNTER(motor->encoder_timer), get,con_val, (*motor->encoder_overflow_count)  );
			i=0;
		}
		i++;	
#endif
    }
}*/

////////////////
/*控制大臂未改完*/
//void Motor_Big_Set_Position(float _set)
//{
//    float Motor_Big_K=0.00057f; //传动比0.00057f
//	static int Round_Cnt=0;	
//	static int Last_CNT=0;
//    static float PWM=0.0f;	
//	static float Big_Position_Now=0.0f;
//    
//	if((int)(Big_Econder_TIM->CNT)-Last_CNT>40000)//为什么大于40000而不是1000
//	{
//        Round_Cnt--; 
//	}
//    else if((int)(Big_Econder_TIM->CNT)-Last_CNT<-40000)
//	{
//        Round_Cnt++;
//	}
//    
//    Last_CNT=(int)Big_Econder_TIM->CNT; //将当前编码器的值记录下来
//	Big_Position_Now=(float)((int)(Big_Econder_TIM->CNT))*3.14f*Motor_Big_K/1000.0f+(float)Round_Cnt*65.535*3.14f*Motor_Big_K;//度
//    //Big_Position_Now=(float)((int)(Big_TIM_CNT))*(3.14*2)f*Motor_Big_K/2000.0f+(float)Round_Cnt*65535*(3.14*2)f*Motor_Big_K/2000.0f;//度

//	PWM=PID_calc(&Motor_Big,Big_Position_Now,_set);
//	
//    if(PWM>0.01f)
//	{
//		
//        Big_PWM1_SETCOMPAER((uint32_t)PWM);
//        Big_PWM2_SETCOMPAER(0);//电机正转 引脚高高电平
//	}else if(PWM<-0.01f)  //数据存储有点偏差
//	{
//        Big_PWM2_SETCOMPAER((uint32_t)(-PWM));
//        Big_PWM1_SETCOMPAER(0);   //电机反转	
//	}else
//	{
//        Big_PWM1_SETCOMPAER(Big_Econder_TIM->ARR);
//        Big_PWM2_SETCOMPAER(Big_Econder_TIM->ARR);  //电机停转
//        //stop1=0;//运转标志
//	}
//    /*原
//    if(PWM>0.01f)
//	{
//		TIM5->CCR3=(uint32_t)PWM;//通道3占空比
//		TIM5->CCR4=0;	              //电机正转
//	}else if(PWM<-0.01f)  //数据存储有点偏差
//	{
//		TIM5->CCR4=(uint32_t)(-PWM);
//		TIM5->CCR3=0;	              //电机反转	
//	}else
//	{
//		TIM5->CCR3=TIM5->ARR;
//		TIM5->CCR4=TIM5->ARR;   //电机停转
//	}*/
//}





///*控制小臂*/
//void Motor_Small_Set_Position(float _set)
//{
//	 if (is_motor_en == 1)
//	 {
//		//static int Round_Cnt=0;
//		//static int Last_CNT=0;
//		//float Motor_Small_K=0.00057f; //传动比(包含传动比以及减速比)
//		static float PWM=0.0f;//控制量：占空比；周期等有关
//		static float _get=0.0f;//当前位置
//		static int32_t Small_Position_Now=0;//当前位置	 
//		_set=Convert_angle(1,_set);//转换弧度
//		/*
//		 if((int)(Small_Econder_TIM->CNT)-Last_CNT>40000)     
//		{
//			Round_Cnt--;
//		}
//		else if((int)(Small_Econder_TIM->CNT)-Last_CNT<-40000)
//		{
//			Round_Cnt++;
//		}*/
//		
//		//Small_Position_Now=(float)((int)(Small_Econder_TIM->CNT))*3.14f*Motor_Small_K/1000.0f;//+(float)Round_Cnt*4294967.295f*3.14f*Motor_Small_K;
//		_get=(float)((int)(Small_Econder_TIM->CNT)/ENCODER_TOTAL_RESOLUTION)*2*3.1415926f*Motor_Small_K;
//		//Last_CNT=(int)Small_Econder_TIM->CNT; //将当前编码器的值记录下来
//		PWM=PID_calc(&Motor_Small,_get,_set);
//		Small_Position_Now=Convert_angle(0,_get);		
//		//Get_Test=Position_Now;	
//	
//#if 0
///*电机反接的控制*/
//	//////////1高-1低电控制
//		if(PWM>0.01f)//电机正转
//		{
//			float a=(PWM<245) ? PWM+60 : PWM;
//			//float a= PWM;
//			Small_PWM1_SETCOMPAER((uint32_t)a);//低电->高电
//			Small_PWM2_SETCOMPAER(0);		    //高电
//		}
//		else if(PWM<-0.01f)  //数据存储有点偏差
//		{
//			//float b= PWM;
//			float b=(PWM>-245) ? PWM-60 : PWM;
//			Small_PWM2_SETCOMPAER((uint32_t)(-b));
//			Small_PWM1_SETCOMPAER(0); //电机反转
//		}
//		else
//		{
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);  //电机断电停转
//			//stop2=0;
//			is_motor_en = 0;
//		}
//# endif
//#if 0
//	//////////1低-1高电控制
//		float a=0;
//		float b=0;
//		if(PWM>10.01f)
//		{
//			a=Small_PWM_TIM->ARR-PWM;//电机正转
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM2_SETCOMPAER((uint32_t)(a));	  
//		}
//		else if(PWM<-10.01f)  //数据存储有点偏差
//		{
//			b=Small_PWM_TIM->ARR+PWM;//电机反转
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM1_SETCOMPAER((uint32_t)(b)); 
//		}
//		else
//		{
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR); //电机断电停转
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR); 
//			//stop2=0;
//		}
//#endif		
//#if 0	
///*电机正接的控制*/	
//		if(PWM>0.01f)
//		{
//			Small_PWM1_SETCOMPAER(0);//电机正转
//			Small_PWM2_SETCOMPAER((uint32_t)PWM);
//		   
//		}
//		else if(PWM<-0.01f)  //数据存储有点偏差
//		{
//			Small_PWM1_SETCOMPAER((uint32_t)(-PWM)); //电机反转
//			Small_PWM2_SETCOMPAER(0);
//		}
//		else
//		{
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);  
//			//stop2=0;
//		}
//#endif	
//		
//		static uint16_t i = 0;
//		float Shaft_Speed = 0.0f;
//		static int  Last_Count=0;
//		static int32_t Capture_Count=0; // 当前时刻总计数值
//#if YeHuoPID
//		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Small_Position_Now, 1);                // 给通道 1 发送实际值
//#else
//		i++;
//		if(i == 1000)/* 100ms计算一次 */
//		{
//			/* 当前时刻总计数值 = 计数器值 + 计数溢出次数 * ENCODER_TIM_PERIOD  */
//			Capture_Count =__HAL_TIM_GET_COUNTER(&htim2) + (Encoder_Overflow_Count *4294967295);
//			/* 转轴转速 = 单位时间内的计数值 / 编码器总分辨率 * 时间系数  */
//			Shaft_Speed = (float)(Capture_Count - Last_Count) / 2000;
//			printf("****************************\r\n设定角度%.3f\r\n当前角度%.3f\r\nCNT%d\r\nPWM%.3f\r\n",Convert_angle(0,_set),Convert_angle(0,_get),Small_Econder_TIM->CNT,PWM );/* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */   
////			printf("单位时间内有效计数值：%d\r\n", Capture_Count - Last_Count);/* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */
////			printf("电机转轴处转速：%.2f 转/秒 \r\n", Shaft_Speed);
////			printf("电机输出轴转速：%.2f 转/秒 \r\n", Shaft_Speed/REDUCTION_RATIO);/* 输出轴转速 = 转轴转速 / 减速比 */
//			/* 记录当前总计数值，供下一时刻计算使用 */
//			Last_Count = Capture_Count;
//			i=0;
//		}
//#endif
//	}

//}

#endif







