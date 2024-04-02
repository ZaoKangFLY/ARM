#include "App.h"

pid_t Motor_Big;    //创建大臂PID结构体 
//pid_t Motor_Small;  //创建小臂PID结构体 
uint8_t    is_motor_en = 0;             // 电机使能

/*控制系统初始化*/
void System_Init()
{
#if PID_ASSISTANT_EN
	TIMx_PWM_disable();
#else	
    TIMx_PWM_enable();
#endif 
	Uartx_enable();
	TIMx_econder_enable();
    TIMx_basic_enable();
   
	/*初始化结构体*/
	//pid_param_init(&Motor_Big,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.001f);//初始化大臂PID结构体
	pid_param_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//初始化小臂PID结构体 
	//PID_struct_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.02f);

#if PID_ASSISTANT_EN
    float pid_temp[3] = {Motor_Small.p, Motor_Small.i, Motor_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // 给通道 1 发送 P I D 值
#endif	


    /*侧推12的上电初始化*/
//    Ce1_SETCOMPAER(2000);
//    Ce1_SETCOMPAER(1500);
//    Ce2_SETCOMPAER(2000);
//    Ce2_SETCOMPAER(1500);
#if PID_ASSISTANT_EN

#else
	/*调试回传，PC104可能要去掉*/
    //HAL_UART_Transmit(&huart3, (uint8_t *)"初始化完成！", 16 , 0xffff);
    Usart_SendString((uint8_t *)"初始化完成！");
#endif
    stop3=0;
	
}


/*控制大臂未改完*/
void Motor_Big_Set_Position(float _set)
{
    float Motor_Big_K=0.00057f; //传动比0.00057f
	static int Round_Cnt=0;	
	static int Last_CNT=0;
    static float PWM=0.0f;	
	static float Big_Position_Now=0.0f;
    
	if((int)(Big_Econder_TIM->CNT)-Last_CNT>40000)//为什么大于40000而不是1000
	{
        Round_Cnt--; 
	}
    else if((int)(Big_Econder_TIM->CNT)-Last_CNT<-40000)
	{
        Round_Cnt++;
	}
    
    Last_CNT=(int)Big_Econder_TIM->CNT; //将当前编码器的值记录下来
	Big_Position_Now=(float)((int)(Big_Econder_TIM->CNT))*3.14f*Motor_Big_K/1000.0f+(float)Round_Cnt*65.535*3.14f*Motor_Big_K;//度
    //Big_Position_Now=(float)((int)(Big_TIM_CNT))*(3.14*2)f*Motor_Big_K/2000.0f+(float)Round_Cnt*65535*(3.14*2)f*Motor_Big_K/2000.0f;//度

	PWM=PID_calc(&Motor_Big,Big_Position_Now,_set);
	
    if(PWM>0.01f)
	{
		
        Big_PWM1_SETCOMPAER((uint32_t)PWM);
        Big_PWM2_SETCOMPAER(0);//电机正转 引脚高高电平
	}else if(PWM<-0.01f)  //数据存储有点偏差
	{
        Big_PWM2_SETCOMPAER((uint32_t)(-PWM));
        Big_PWM1_SETCOMPAER(0);   //电机反转	
	}else
	{
        Big_PWM1_SETCOMPAER(Big_Econder_TIM->ARR);
        Big_PWM2_SETCOMPAER(Big_Econder_TIM->ARR);  //电机停转
        //stop1=0;//运转标志
	}
    /*原
    if(PWM>0.01f)
	{
		TIM5->CCR3=(uint32_t)PWM;//通道3占空比
		TIM5->CCR4=0;	              //电机正转
	}else if(PWM<-0.01f)  //数据存储有点偏差
	{
		TIM5->CCR4=(uint32_t)(-PWM);
		TIM5->CCR3=0;	              //电机反转	
	}else
	{
		TIM5->CCR3=TIM5->ARR;
		TIM5->CCR4=TIM5->ARR;   //电机停转
	}*/
}





/*控制小臂*/
void Motor_Small_Set_Position(float _set)
{
	 if (is_motor_en == 1)
	 {
		//static int Round_Cnt=0;
		//static int Last_CNT=0;
		//float Motor_Small_K=0.00057f; //传动比(包含传动比以及减速比)
		static float PWM=0.0f;//控制量：占空比；周期等有关
		static float _get=0.0f;//当前位置
		static int32_t Small_Position_Now=0;//当前位置	 
		_set=Convert_angle(1,_set);//转换弧度
		/*
		 if((int)(Small_Econder_TIM->CNT)-Last_CNT>40000)     
		{
			Round_Cnt--;
		}
		else if((int)(Small_Econder_TIM->CNT)-Last_CNT<-40000)
		{
			Round_Cnt++;
		}*/
		
		//Small_Position_Now=(float)((int)(Small_Econder_TIM->CNT))*3.14f*Motor_Small_K/1000.0f;//+(float)Round_Cnt*4294967.295f*3.14f*Motor_Small_K;
		_get=(float)((int)(Small_Econder_TIM->CNT)/ENCODER_TOTAL_RESOLUTION)*2*3.1415926f*Motor_Small_K;
		//Last_CNT=(int)Small_Econder_TIM->CNT; //将当前编码器的值记录下来
		PWM=PID_calc(&Motor_Small,_get,_set);
		Small_Position_Now=Convert_angle(0,_get);		
		//Get_Test=Position_Now;	
	
#if 0
/*电机反接的控制*/
	//////////1高-1低电控制
		if(PWM>0.01f)//电机正转
		{
			float a=(PWM<245) ? PWM+60 : PWM;
			//float a= PWM;
			Small_PWM1_SETCOMPAER((uint32_t)a);//低电->高电
			Small_PWM2_SETCOMPAER(0);		    //高电
		}
		else if(PWM<-0.01f)  //数据存储有点偏差
		{
			//float b= PWM;
			float b=(PWM>-245) ? PWM-60 : PWM;
			Small_PWM2_SETCOMPAER((uint32_t)(-b));
			Small_PWM1_SETCOMPAER(0); //电机反转
		}
		else
		{
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);  //电机断电停转
			//stop2=0;
			is_motor_en = 0;
		}
# endif
#if 0
	//////////1低-1高电控制
		float a=0;
		float b=0;
		if(PWM>10.01f)
		{
			a=Small_PWM_TIM->ARR-PWM;//电机正转
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM2_SETCOMPAER((uint32_t)(a));	  
		}
		else if(PWM<-10.01f)  //数据存储有点偏差
		{
			b=Small_PWM_TIM->ARR+PWM;//电机反转
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM1_SETCOMPAER((uint32_t)(b)); 
		}
		else
		{
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR); //电机断电停转
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR); 
			//stop2=0;
		}
#endif		
#if 0	
/*电机正接的控制*/	
		if(PWM>0.01f)
		{
			Small_PWM1_SETCOMPAER(0);//电机正转
			Small_PWM2_SETCOMPAER((uint32_t)PWM);
		   
		}
		else if(PWM<-0.01f)  //数据存储有点偏差
		{
			Small_PWM1_SETCOMPAER((uint32_t)(-PWM)); //电机反转
			Small_PWM2_SETCOMPAER(0);
		}
		else
		{
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);  
			//stop2=0;
		}
#endif	
		
		static uint16_t i = 0;
		float Shaft_Speed = 0.0f;
		static int  Last_Count=0;
		static int32_t Capture_Count=0; // 当前时刻总计数值
#if PID_ASSISTANT_EN
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Small_Position_Now, 1);                // 给通道 1 发送实际值
#else
		i++;
		if(i == 1000)/* 100ms计算一次 */
		{
			/* 当前时刻总计数值 = 计数器值 + 计数溢出次数 * ENCODER_TIM_PERIOD  */
			Capture_Count =__HAL_TIM_GET_COUNTER(&htim2) + (Encoder_Overflow_Count *4294967295);
			/* 转轴转速 = 单位时间内的计数值 / 编码器总分辨率 * 时间系数  */
			Shaft_Speed = (float)(Capture_Count - Last_Count) / 2000;
			printf("****************************\r\n设定角度%.3f\r\n当前角度%.3f\r\nCNT%d\r\nPWM%.3f\r\n",Convert_angle(0,_set),Convert_angle(0,_get),Small_Econder_TIM->CNT,PWM );/* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */   
//			printf("单位时间内有效计数值：%d\r\n", Capture_Count - Last_Count);/* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */
//			printf("电机转轴处转速：%.2f 转/秒 \r\n", Shaft_Speed);
//			printf("电机输出轴转速：%.2f 转/秒 \r\n", Shaft_Speed/REDUCTION_RATIO);/* 输出轴转速 = 转轴转速 / 减速比 */
			/* 记录当前总计数值，供下一时刻计算使用 */
			Last_Count = Capture_Count;
			i=0;
		}
#endif
	}

}



void Motor_Small_Set_Speed(float _set)//速度环控制
{
	 if (is_motor_en == 1)
	 {	
		//float Motor_Small_K=0.01425f;//传动比
		//float K=0.01425f/REDUCTION_RATIO; //(包含传动比以及减速比)0.00057=k/REDUCTION_RATIO 
		//static int Round_Cnt = 0; // 当前时刻总计数值
		//static int Last_Cnt = 0;
		static float con_val = 0;//控制量
		static uint16_t PWM = 0;//占空比；周期等有关
		static int set = 0;//当前速度
		static int get = 0;//当前速度
		static int _get = 0;//当前速度
		_set=_set*CIRCLE_Samll/360;	//臂转角度反应的脉冲数
		 set=_set;
//#if PID_ASSISTANT_EN
//		set_computer_value(SEND_FACT_CMD, CURVES_CH2, &set, 1); 
//#endif			 
		get =__HAL_TIM_GET_COUNTER(&htim2)+ Encoder_Overflow_Count *4294967295  ;
//#if PID_ASSISTANT_EN
//                // 给通道 1 发送实际值
//		set_computer_value(SEND_FACT_CMD, CURVES_CH3, &get, 1);
//#endif	
//		 
		con_val=PID_calc(&Motor_Small,get,set);//输入为臂速度	

/*电机反接的控制*/	//////////1高-1低电控制			 
		if(con_val>0)//电机正转
		{
			PWM = con_val;
			Small_PWM1_SETCOMPAER(PWM);
			Small_PWM2_SETCOMPAER(0);		    
		}
		else //电机反转
		{
			PWM = -con_val;
			Small_PWM1_SETCOMPAER(0); 
			Small_PWM2_SETCOMPAER(PWM);
		}
		 _get=(int)(get*360/CIRCLE_Samll);		
#if PID_ASSISTANT_EN
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);                // 给通道 1 发送实际值
		//set_computer_value(SEND_FACT_CMD, CURVES_CH2, &set, 1); 
		//set_computer_value(SEND_FACT_CMD, CURVES_CH3, &get, 1);
#endif
	}

}
/*控制侧推*/
//void Motor_CeTui_Set_Position(int _set)
//{
//     static float y=0;
//     static uint16_t PWM=1500; 
//     if(stop3==1)//接收到侧推指令
//    {
//        if(1000<=_set&&_set<=2000)//判断指令是否有效范围
//        {
//            if(_set!=PWM )//判断指令是否发生过改变
//            {
//                if(PWM<_set)
//                {
//                    while(PWM!=_set)
//                    {
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, PWM);/* 修改比较值控制占空比 */
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, PWM);
//                        HAL_Delay(5);
//                        PWM++;
//                    }
//                }
//                else if(PWM>_set)
//                {
//                   while(_set!=PWM)
//                   {
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, PWM);/* 修改比较值控制占空比 */
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, PWM);
//                       HAL_Delay(5);;
//                       PWM--;
//                   }
//                }
//               y=COUNT(_set);
//                printf("\r\npwm:%d;占空比:%.1f%%;PWM:%.1fms;推力:%.2fN;",_set,((float)PWM)/200,((float)PWM)/20000*20,10*y);
//            }
//            else
//            {
//                printf("\r\n输入侧推占空比无变化"); 
//            }

//        }
//        else
//        {
//            printf("\r\n输入侧推占空比不在范围"); 
//        }
//        stop3=0;
//    }
//}





/*弧度转角度计算*/
float Convert_angle(int j, float x)//1转角度0转弧度
{
	if(j)
	{
		x=x*3.1415926/180;
	}
	else
	{
		x=x*180/3.1415926;
	}
	return x;
}

/*侧推占空比对应力的函数计算*/
float COUNT(float x) 
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











