#include "motor_control.h"
uint8_t  g_motorEnable = 0;             // ���ʹ��
uint8_t  g_cemotorEnable = 0;             // ���ʹ��
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
		static int _get = 0; 		// ʵ��ת���Ƕ�
        static int16_t con_val = 0; // ����ռ�ձ�
        static int32_t set = 0;     // pid�趨������
        static int32_t get = 0;     // ʵ��ת��������
        
        set = _set * motor->pulses_per_revolution / 360; // �趨�Ƕȶ�Ӧ��������
        get = __HAL_TIM_GET_COUNTER(motor->encoder_timer) + 
              (*motor->encoder_overflow_count) * motor->encoder_period; // ��������
        
        con_val = (int16_t)(PID_calc(motor->pid, get, set)); // PID ����
        motor->motor_control_function(con_val);   // ���Ƶ��
        
        _get = get * 360 / motor->pulses_per_revolution; // ת���Ƕ�
#if YeHuoPID
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // ��ͨ�� 1 ����ʵ��ֵ
#else	
		static uint16_t i = 0;
		if(i ==100)// ms����һ�� 
		{
			printf("%8d%18d%8d%8d%8d%8d\r\n",_set,_get,__HAL_TIM_GET_COUNTER(motor->encoder_timer), get,con_val, (*motor->encoder_overflow_count)  );
			i=0;
		}
		i++;	
#endif
    }
}*/
void motor_jian_fun(int16_t pwm)
{
	static int16_t val=0; 
    if(pwm!=val)
	{
		if(pwm>0)//�������
		{
			Jian1_SETCOMPARE((uint16_t)pwm);  //ͬ��ͣת
			Jian2_SETCOMPARE(0);	    	
		}
		else 
		{
			Jian1_SETCOMPARE(0); 
			Jian2_SETCOMPARE((uint16_t)(-pwm));
		}
		val=pwm;
	}
}
void motor_big_fun(int16_t pwm)
{
	static int16_t val=0; 
    if(pwm!=val)
	{
		if(pwm>0)//�������
		{
			Big1_SETCOMPARE((uint16_t)pwm);  //ͬ��ͣת
			Big2_SETCOMPARE(0);	    	
		}
		else 
		{
			Big1_SETCOMPARE(0); 
			Big2_SETCOMPARE((uint16_t)(-pwm));
		}
		val=pwm;
	}
}

void motor_small_fun(int16_t pwm)
{
	static int16_t val=0; 
    if(pwm!=val)
	{
		if(pwm>0)//�������
		{
			Small1_SETCOMPARE((uint16_t)pwm);  //ͬ��ͣת
			Small2_SETCOMPARE(0);	    	
		}
		else 
		{
			Small1_SETCOMPARE(0); 
			Small2_SETCOMPARE((uint16_t)(-pwm));
		}
		val=pwm;
	}
}
void motor_wan_fun(int16_t pwm)
{
	static int16_t val=0; 
    if(pwm!=val)
	{
		if(pwm>0)//�������
		{
			Wan1_SETCOMPARE((uint16_t)pwm);  //ͬ��ͣת
			Wan2_SETCOMPARE(0);	    	
		}
		else 
		{
			Wan1_SETCOMPARE(0); 
			Wan2_SETCOMPARE((uint16_t)(-pwm));
		}
		val=pwm;
	}
}






void zhua_set(uint8_t _set)
{
	static int8_t i = 0;
	if(_set==0x0A&& i<10)//i=10����
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
	     	Zhua1_SETCOMPARE(0);  //ͣת
			Zhua2_SETCOMPARE(0);
	}
}


void jian_set_postion(int16_t _set)
{
	 if (g_motorEnable == 1)
	 {	
		static int16_t _get = 0;//ʵ��ת���Ƕ�
		static int16_t con_val = 0;//����ռ�ձ�
	    static int32_t set = 0;//�趨������
        static int32_t get = 0;////ʵ��ת��������	
		set=_set*J_ARM_PULSES_PER_REVOLUTION/360 ;	//�趨�Ƕȶ�Ӧ�������� 
		get =__HAL_TIM_GET_COUNTER(&Jian_Encoder_htim) + (g_jianEncoderOverflowCount * J_ENCODER_PERIOD);//��ȡ������������	
		con_val=PID_calc(&Pid_Jian,get,set);
        motor_jian_fun(con_val);	
		_get=get*360/J_ARM_PULSES_PER_REVOLUTION;		
#if YeHuoPID
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // ��ͨ�� 1 ����ʵ��ֵ				
#endif
#if ShangweiJi 
	static uint16_t i = 0;
	if(i ==200)// ms����һ�� 
	{
		printf("%8d%8d%8d%8d%8d%8d\r\n",_set,_get,set,get,con_val,g_jianEncoderOverflowCount );
		i=1;
	}
	i++;
#endif

	}

}
/*���*/
void big_set_postion(int16_t _set)//�ջ����ƣ��趨�Ƕ�
{
	 
	 if (g_motorEnable == 1)
	 {	
		static int _get = 0;//ʵ��ת���Ƕ�
		static int16_t con_val = 0;//����ռ�ձ�
	    static int32_t set = 0;//�趨������
        static int32_t get = 0;////ʵ��ת��������		
		set=_set*B_ARM_PULSES_PER_REVOLUTION/360 ;	//�趨�Ƕȶ�Ӧ�������� 
		get =__HAL_TIM_GET_COUNTER(&Big_Encoder_htim)+ g_bigEncoderOverflowCount  *  B_ENCODER_PERIOD    ; //��������	
		con_val=PID_calc(&Pid_Big,get,set);//pid����	
        motor_big_fun(con_val);	
        _get=get*360/B_ARM_PULSES_PER_REVOLUTION ;//ת���Ƕ�		
#if YeHuoPID
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // ��ͨ�� 1 ����ʵ��ֵ
#endif

#if	ShangweiJi 
		static uint16_t i = 0;
		if(i ==100)// ms����һ�� 
		{
			printf("%8d%8d%8d%8d%8d%8d\r\n",_set,_get,set,get,con_val,g_bigEncoderOverflowCount );
			i=1;
		}
		i++;	
#endif
	}

}

/*С��*/
void small_set_postion(int16_t _set)//_set�趨�Ƕȣ��۵����¸��Ƕ�
{

	 if (g_motorEnable == 1)
	 {	
		static int16_t _get = 0;//ʵ��ת���Ƕ�
		static int16_t con_val = 0;//����ռ�ձ�
	    static int32_t set = 0;//�趨������
        static int32_t get = 0;////ʵ��ת��������	
		set=_set*S_ARM_PULSES_PER_REVOLUTION/360 ;	//�趨�Ƕȶ�Ӧ�������� 
		get =__HAL_TIM_GET_COUNTER(&Small_Encoder_htim) + (g_smallEncoderOverflowCount * S_ENCODER_PERIOD);//��ȡ������������	
		con_val=PID_calc(&Pid_Small,get,set);
        motor_small_fun(con_val);	
		_get=get*360/S_ARM_PULSES_PER_REVOLUTION;		
#if YeHuoPID
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // ��ͨ�� 1 ����ʵ��ֵ
#endif
#if	ShangweiJi 
		static uint16_t i = 0;
		if(i ==100)// ms����һ�� 
		{
			printf("%8d%8d%8d%8d%8d%8d\r\n",_set,_get,set,get,con_val,g_smallEncoderOverflowCount );
			i=1;
		}
		i++;	
#endif
	}

}

void wan_set_postion(int16_t _set)
{
	 if (g_motorEnable == 1)
	 {	
		static int16_t _get = 0;//ʵ��ת���Ƕ�
		static int16_t con_val = 0;//����ռ�ձ�
	    static int32_t set = 0;//�趨������
        static int32_t get = 0;////ʵ��ת��������	
		set=_set*W_ARM_PULSES_PER_REVOLUTION/360 ;	//�趨�Ƕȶ�Ӧ�������� 
		get =__HAL_TIM_GET_COUNTER(&Wan_Encoder_htim) + (g_wanEncoderOverflowCount * W_ENCODER_PERIOD);//��ȡ������������	
		con_val=PID_calc(&Pid_Wan,get,set);
        motor_wan_fun(con_val);	
		_get=get*360/W_ARM_PULSES_PER_REVOLUTION;		
#if YeHuoPID
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);  // ��ͨ�� 1 ����ʵ��ֵ
#endif
#if ShangweiJi 
		static uint16_t i = 0;
		if(i ==200)// ms����һ�� 
		{
			printf("%8d%8d%8d%8d%8d%8d\r\n",_set,_get,set,get,con_val,g_wanEncoderOverflowCount );
			i=1;
		}
		i++;	
#endif
	}

}

/*���Ʋ���*/
void motor_cetui_set_postion(int _set)
{
     static uint16_t con_val=1500; 
     if(g_cemotorEnable == 1)//���յ�����ָ��
    {
        if(1000<=_set&&_set<=2000)//�ж�ָ���Ƿ���Ч��Χ
        {
            if(_set!=con_val )//�ж�ָ���Ƿ������ı�
            {
                if(con_val<_set)
                {
                    while(con_val!=_set)
                    {
						Ce1_SETCOMPARE(con_val) ;//1600
						Ce2_SETCOMPARE(3000-con_val) ;//1400
                        HAL_Delay(5);
                        con_val++;
                    }
                }
                else if(con_val>_set)
                {
                   while(_set!=con_val)
                   {
					   	Ce1_SETCOMPARE(con_val) ;//1400
						Ce2_SETCOMPARE(3000-con_val);//1600
                       HAL_Delay(5);
                       con_val--;
                   }
                }
              // y=COUNT(_set);
              //  printf("\r\npwm:%d;ռ�ձ�:%.1f%%;PWM:%.1fms;����:%.2fN;",_set,((float)con_val)/200,((float)con_val)/20000*20,10*y);
            }
            else
            {
               // printf("\r\n�������ռ�ձ��ޱ仯"); 
            }

        }
        else
        {
            //printf("\r\n�������ռ�ձȲ��ڷ�Χ"); 
        }
		g_cemotorEnable = 0;
       
    }
}

/*��̬����δ�ϲ������뷨�漰��������*/
void cetui_set_postion(float _get)//��ǰ��̬�Ƕ�
{
	static int throttle = 1700;//
	if (g_cemotorEnable == 1)//ʹ��
	{	
		static float ROL_conval = 0;//������
		static uint16_t motor_pwm_1 = 0;//ռ�ձ�
		static uint16_t motor_pwm_2 = 0;//ռ�ձ�
		static int _set = 0;//�趨0
		_get = deg_to_rad(_get);//��ǰ��ת��ת����
		ROL_conval = PID_calc(&Pid_Ce,_get,_set); 
		motor_pwm_1 = throttle + ROL_conval;//throttleԼ����1700
		motor_pwm_2 = throttle - ROL_conval;
		if(motor_pwm_1>2000){motor_pwm_1=2000;}
		if(motor_pwm_2>2000){motor_pwm_2=2000;}
		if(motor_pwm_1<1500){motor_pwm_1=1500;}
		if(motor_pwm_2<1500){motor_pwm_2=1500;}
		Ce1_SETCOMPARE(motor_pwm_1) ;
		Ce2_SETCOMPARE(motor_pwm_2) ;		
#if YeHuoPID

#endif
	}

}

// ����ת�Ƕ�
float rads_to_deg(float radians) {
    return radians * 180.0f / PI;
}
// �Ƕ�ת����
float deg_to_rad(float degrees) {
    return degrees * PI / 180.0f;
}

/*����ռ�ձȶ�Ӧ���ĺ�������*/
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
    x /= 1000;  // �� x ��С 1000 ��
    for (int i = 0; i < sizeof(coefficients) / sizeof(coefficients[0]); i++) 
	{
        y += coefficients[i] * x_pow;
        x_pow *= x;
    }
    return y;
}

#if 0
/*���ƴ��δ����*/
//void Motor_Big_Set_Position(float _set)
//{
//    float Motor_Big_K=0.00057f; //������0.00057f
//	static int Round_Cnt=0;	
//	static int Last_CNT=0;
//    static float PWM=0.0f;	
//	static float Big_Position_Now=0.0f;
//    
//	if((int)(Big_Econder_TIM->CNT)-Last_CNT>40000)//Ϊʲô����40000������1000
//	{
//        Round_Cnt--; 
//	}
//    else if((int)(Big_Econder_TIM->CNT)-Last_CNT<-40000)
//	{
//        Round_Cnt++;
//	}
//    
//    Last_CNT=(int)Big_Econder_TIM->CNT; //����ǰ��������ֵ��¼����
//	Big_Position_Now=(float)((int)(Big_Econder_TIM->CNT))*3.14f*Motor_Big_K/1000.0f+(float)Round_Cnt*65.535*3.14f*Motor_Big_K;//��
//    //Big_Position_Now=(float)((int)(Big_TIM_CNT))*(3.14*2)f*Motor_Big_K/2000.0f+(float)Round_Cnt*65535*(3.14*2)f*Motor_Big_K/2000.0f;//��

//	PWM=PID_calc(&Motor_Big,Big_Position_Now,_set);
//	
//    if(PWM>0.01f)
//	{
//		
//        Big_PWM1_SETCOMPAER((uint32_t)PWM);
//        Big_PWM2_SETCOMPAER(0);//�����ת ���Ÿ߸ߵ�ƽ
//	}else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
//	{
//        Big_PWM2_SETCOMPAER((uint32_t)(-PWM));
//        Big_PWM1_SETCOMPAER(0);   //�����ת	
//	}else
//	{
//        Big_PWM1_SETCOMPAER(Big_Econder_TIM->ARR);
//        Big_PWM2_SETCOMPAER(Big_Econder_TIM->ARR);  //���ͣת
//        //stop1=0;//��ת��־
//	}
//    /*ԭ
//    if(PWM>0.01f)
//	{
//		TIM5->CCR3=(uint32_t)PWM;//ͨ��3ռ�ձ�
//		TIM5->CCR4=0;	              //�����ת
//	}else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
//	{
//		TIM5->CCR4=(uint32_t)(-PWM);
//		TIM5->CCR3=0;	              //�����ת	
//	}else
//	{
//		TIM5->CCR3=TIM5->ARR;
//		TIM5->CCR4=TIM5->ARR;   //���ͣת
//	}*/
//}





///*����С��*/
//void Motor_Small_Set_Position(float _set)
//{
//	 if (is_motor_en == 1)
//	 {
//		//static int Round_Cnt=0;
//		//static int Last_CNT=0;
//		//float Motor_Small_K=0.00057f; //������(�����������Լ����ٱ�)
//		static float PWM=0.0f;//��������ռ�ձȣ����ڵ��й�
//		static float _get=0.0f;//��ǰλ��
//		static int32_t Small_Position_Now=0;//��ǰλ��	 
//		_set=Convert_angle(1,_set);//ת������
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
//		//Last_CNT=(int)Small_Econder_TIM->CNT; //����ǰ��������ֵ��¼����
//		PWM=PID_calc(&Motor_Small,_get,_set);
//		Small_Position_Now=Convert_angle(0,_get);		
//		//Get_Test=Position_Now;	
//	
//#if 0
///*������ӵĿ���*/
//	//////////1��-1�͵����
//		if(PWM>0.01f)//�����ת
//		{
//			float a=(PWM<245) ? PWM+60 : PWM;
//			//float a= PWM;
//			Small_PWM1_SETCOMPAER((uint32_t)a);//�͵�->�ߵ�
//			Small_PWM2_SETCOMPAER(0);		    //�ߵ�
//		}
//		else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
//		{
//			//float b= PWM;
//			float b=(PWM>-245) ? PWM-60 : PWM;
//			Small_PWM2_SETCOMPAER((uint32_t)(-b));
//			Small_PWM1_SETCOMPAER(0); //�����ת
//		}
//		else
//		{
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);  //����ϵ�ͣת
//			//stop2=0;
//			is_motor_en = 0;
//		}
//# endif
//#if 0
//	//////////1��-1�ߵ����
//		float a=0;
//		float b=0;
//		if(PWM>10.01f)
//		{
//			a=Small_PWM_TIM->ARR-PWM;//�����ת
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM2_SETCOMPAER((uint32_t)(a));	  
//		}
//		else if(PWM<-10.01f)  //���ݴ洢�е�ƫ��
//		{
//			b=Small_PWM_TIM->ARR+PWM;//�����ת
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);
//			Small_PWM1_SETCOMPAER((uint32_t)(b)); 
//		}
//		else
//		{
//			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR); //����ϵ�ͣת
//			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR); 
//			//stop2=0;
//		}
//#endif		
//#if 0	
///*������ӵĿ���*/	
//		if(PWM>0.01f)
//		{
//			Small_PWM1_SETCOMPAER(0);//�����ת
//			Small_PWM2_SETCOMPAER((uint32_t)PWM);
//		   
//		}
//		else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
//		{
//			Small_PWM1_SETCOMPAER((uint32_t)(-PWM)); //�����ת
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
//		static int32_t Capture_Count=0; // ��ǰʱ���ܼ���ֵ
//#if YeHuoPID
//		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Small_Position_Now, 1);                // ��ͨ�� 1 ����ʵ��ֵ
//#else
//		i++;
//		if(i == 1000)/* 100ms����һ�� */
//		{
//			/* ��ǰʱ���ܼ���ֵ = ������ֵ + ����������� * ENCODER_TIM_PERIOD  */
//			Capture_Count =__HAL_TIM_GET_COUNTER(&htim2) + (Encoder_Overflow_Count *4294967295);
//			/* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
//			Shaft_Speed = (float)(Capture_Count - Last_Count) / 2000;
//			printf("****************************\r\n�趨�Ƕ�%.3f\r\n��ǰ�Ƕ�%.3f\r\nCNT%d\r\nPWM%.3f\r\n",Convert_angle(0,_set),Convert_angle(0,_get),Small_Econder_TIM->CNT,PWM );/* ��λʱ�����ֵ = ��ǰʱ���ܼ���ֵ - ��һʱ���ܼ���ֵ */   
////			printf("��λʱ������Ч����ֵ��%d\r\n", Capture_Count - Last_Count);/* ��λʱ�����ֵ = ��ǰʱ���ܼ���ֵ - ��һʱ���ܼ���ֵ */
////			printf("���ת�ᴦת�٣�%.2f ת/�� \r\n", Shaft_Speed);
////			printf("��������ת�٣�%.2f ת/�� \r\n", Shaft_Speed/REDUCTION_RATIO);/* �����ת�� = ת��ת�� / ���ٱ� */
//			/* ��¼��ǰ�ܼ���ֵ������һʱ�̼���ʹ�� */
//			Last_Count = Capture_Count;
//			i=0;
//		}
//#endif
//	}

//}

#endif







