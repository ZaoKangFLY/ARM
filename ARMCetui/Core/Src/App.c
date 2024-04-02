#include "App.h"

pid_t Motor_Big;    //�������PID�ṹ�� 
//pid_t Motor_Small;  //����С��PID�ṹ�� 
uint8_t    is_motor_en = 0;             // ���ʹ��

/*����ϵͳ��ʼ��*/
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
   
	/*��ʼ���ṹ��*/
	//pid_param_init(&Motor_Big,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.001f);//��ʼ�����PID�ṹ��
	pid_param_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2.1f,1.0f,0.0f,10000.0f,100.0f);//��ʼ��С��PID�ṹ�� 
	//PID_struct_init(&Motor_Small,POSITION_PID,999.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.02f);

#if PID_ASSISTANT_EN
    float pid_temp[3] = {Motor_Small.p, Motor_Small.i, Motor_Small.d};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ�� 1 ���� P I D ֵ
#endif	


    /*����12���ϵ��ʼ��*/
//    Ce1_SETCOMPAER(2000);
//    Ce1_SETCOMPAER(1500);
//    Ce2_SETCOMPAER(2000);
//    Ce2_SETCOMPAER(1500);
#if PID_ASSISTANT_EN

#else
	/*���Իش���PC104����Ҫȥ��*/
    //HAL_UART_Transmit(&huart3, (uint8_t *)"��ʼ����ɣ�", 16 , 0xffff);
    Usart_SendString((uint8_t *)"��ʼ����ɣ�");
#endif
    stop3=0;
	
}


/*���ƴ��δ����*/
void Motor_Big_Set_Position(float _set)
{
    float Motor_Big_K=0.00057f; //������0.00057f
	static int Round_Cnt=0;	
	static int Last_CNT=0;
    static float PWM=0.0f;	
	static float Big_Position_Now=0.0f;
    
	if((int)(Big_Econder_TIM->CNT)-Last_CNT>40000)//Ϊʲô����40000������1000
	{
        Round_Cnt--; 
	}
    else if((int)(Big_Econder_TIM->CNT)-Last_CNT<-40000)
	{
        Round_Cnt++;
	}
    
    Last_CNT=(int)Big_Econder_TIM->CNT; //����ǰ��������ֵ��¼����
	Big_Position_Now=(float)((int)(Big_Econder_TIM->CNT))*3.14f*Motor_Big_K/1000.0f+(float)Round_Cnt*65.535*3.14f*Motor_Big_K;//��
    //Big_Position_Now=(float)((int)(Big_TIM_CNT))*(3.14*2)f*Motor_Big_K/2000.0f+(float)Round_Cnt*65535*(3.14*2)f*Motor_Big_K/2000.0f;//��

	PWM=PID_calc(&Motor_Big,Big_Position_Now,_set);
	
    if(PWM>0.01f)
	{
		
        Big_PWM1_SETCOMPAER((uint32_t)PWM);
        Big_PWM2_SETCOMPAER(0);//�����ת ���Ÿ߸ߵ�ƽ
	}else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
	{
        Big_PWM2_SETCOMPAER((uint32_t)(-PWM));
        Big_PWM1_SETCOMPAER(0);   //�����ת	
	}else
	{
        Big_PWM1_SETCOMPAER(Big_Econder_TIM->ARR);
        Big_PWM2_SETCOMPAER(Big_Econder_TIM->ARR);  //���ͣת
        //stop1=0;//��ת��־
	}
    /*ԭ
    if(PWM>0.01f)
	{
		TIM5->CCR3=(uint32_t)PWM;//ͨ��3ռ�ձ�
		TIM5->CCR4=0;	              //�����ת
	}else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
	{
		TIM5->CCR4=(uint32_t)(-PWM);
		TIM5->CCR3=0;	              //�����ת	
	}else
	{
		TIM5->CCR3=TIM5->ARR;
		TIM5->CCR4=TIM5->ARR;   //���ͣת
	}*/
}





/*����С��*/
void Motor_Small_Set_Position(float _set)
{
	 if (is_motor_en == 1)
	 {
		//static int Round_Cnt=0;
		//static int Last_CNT=0;
		//float Motor_Small_K=0.00057f; //������(�����������Լ����ٱ�)
		static float PWM=0.0f;//��������ռ�ձȣ����ڵ��й�
		static float _get=0.0f;//��ǰλ��
		static int32_t Small_Position_Now=0;//��ǰλ��	 
		_set=Convert_angle(1,_set);//ת������
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
		//Last_CNT=(int)Small_Econder_TIM->CNT; //����ǰ��������ֵ��¼����
		PWM=PID_calc(&Motor_Small,_get,_set);
		Small_Position_Now=Convert_angle(0,_get);		
		//Get_Test=Position_Now;	
	
#if 0
/*������ӵĿ���*/
	//////////1��-1�͵����
		if(PWM>0.01f)//�����ת
		{
			float a=(PWM<245) ? PWM+60 : PWM;
			//float a= PWM;
			Small_PWM1_SETCOMPAER((uint32_t)a);//�͵�->�ߵ�
			Small_PWM2_SETCOMPAER(0);		    //�ߵ�
		}
		else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
		{
			//float b= PWM;
			float b=(PWM>-245) ? PWM-60 : PWM;
			Small_PWM2_SETCOMPAER((uint32_t)(-b));
			Small_PWM1_SETCOMPAER(0); //�����ת
		}
		else
		{
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);  //����ϵ�ͣת
			//stop2=0;
			is_motor_en = 0;
		}
# endif
#if 0
	//////////1��-1�ߵ����
		float a=0;
		float b=0;
		if(PWM>10.01f)
		{
			a=Small_PWM_TIM->ARR-PWM;//�����ת
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM2_SETCOMPAER((uint32_t)(a));	  
		}
		else if(PWM<-10.01f)  //���ݴ洢�е�ƫ��
		{
			b=Small_PWM_TIM->ARR+PWM;//�����ת
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR);
			Small_PWM1_SETCOMPAER((uint32_t)(b)); 
		}
		else
		{
			Small_PWM1_SETCOMPAER(Small_PWM_TIM->ARR); //����ϵ�ͣת
			Small_PWM2_SETCOMPAER(Small_PWM_TIM->ARR); 
			//stop2=0;
		}
#endif		
#if 0	
/*������ӵĿ���*/	
		if(PWM>0.01f)
		{
			Small_PWM1_SETCOMPAER(0);//�����ת
			Small_PWM2_SETCOMPAER((uint32_t)PWM);
		   
		}
		else if(PWM<-0.01f)  //���ݴ洢�е�ƫ��
		{
			Small_PWM1_SETCOMPAER((uint32_t)(-PWM)); //�����ת
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
		static int32_t Capture_Count=0; // ��ǰʱ���ܼ���ֵ
#if PID_ASSISTANT_EN
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Small_Position_Now, 1);                // ��ͨ�� 1 ����ʵ��ֵ
#else
		i++;
		if(i == 1000)/* 100ms����һ�� */
		{
			/* ��ǰʱ���ܼ���ֵ = ������ֵ + ����������� * ENCODER_TIM_PERIOD  */
			Capture_Count =__HAL_TIM_GET_COUNTER(&htim2) + (Encoder_Overflow_Count *4294967295);
			/* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
			Shaft_Speed = (float)(Capture_Count - Last_Count) / 2000;
			printf("****************************\r\n�趨�Ƕ�%.3f\r\n��ǰ�Ƕ�%.3f\r\nCNT%d\r\nPWM%.3f\r\n",Convert_angle(0,_set),Convert_angle(0,_get),Small_Econder_TIM->CNT,PWM );/* ��λʱ�����ֵ = ��ǰʱ���ܼ���ֵ - ��һʱ���ܼ���ֵ */   
//			printf("��λʱ������Ч����ֵ��%d\r\n", Capture_Count - Last_Count);/* ��λʱ�����ֵ = ��ǰʱ���ܼ���ֵ - ��һʱ���ܼ���ֵ */
//			printf("���ת�ᴦת�٣�%.2f ת/�� \r\n", Shaft_Speed);
//			printf("��������ת�٣�%.2f ת/�� \r\n", Shaft_Speed/REDUCTION_RATIO);/* �����ת�� = ת��ת�� / ���ٱ� */
			/* ��¼��ǰ�ܼ���ֵ������һʱ�̼���ʹ�� */
			Last_Count = Capture_Count;
			i=0;
		}
#endif
	}

}



void Motor_Small_Set_Speed(float _set)//�ٶȻ�����
{
	 if (is_motor_en == 1)
	 {	
		//float Motor_Small_K=0.01425f;//������
		//float K=0.01425f/REDUCTION_RATIO; //(�����������Լ����ٱ�)0.00057=k/REDUCTION_RATIO 
		//static int Round_Cnt = 0; // ��ǰʱ���ܼ���ֵ
		//static int Last_Cnt = 0;
		static float con_val = 0;//������
		static uint16_t PWM = 0;//ռ�ձȣ����ڵ��й�
		static int set = 0;//��ǰ�ٶ�
		static int get = 0;//��ǰ�ٶ�
		static int _get = 0;//��ǰ�ٶ�
		_set=_set*CIRCLE_Samll/360;	//��ת�Ƕȷ�Ӧ��������
		 set=_set;
//#if PID_ASSISTANT_EN
//		set_computer_value(SEND_FACT_CMD, CURVES_CH2, &set, 1); 
//#endif			 
		get =__HAL_TIM_GET_COUNTER(&htim2)+ Encoder_Overflow_Count *4294967295  ;
//#if PID_ASSISTANT_EN
//                // ��ͨ�� 1 ����ʵ��ֵ
//		set_computer_value(SEND_FACT_CMD, CURVES_CH3, &get, 1);
//#endif	
//		 
		con_val=PID_calc(&Motor_Small,get,set);//����Ϊ���ٶ�	

/*������ӵĿ���*/	//////////1��-1�͵����			 
		if(con_val>0)//�����ת
		{
			PWM = con_val;
			Small_PWM1_SETCOMPAER(PWM);
			Small_PWM2_SETCOMPAER(0);		    
		}
		else //�����ת
		{
			PWM = -con_val;
			Small_PWM1_SETCOMPAER(0); 
			Small_PWM2_SETCOMPAER(PWM);
		}
		 _get=(int)(get*360/CIRCLE_Samll);		
#if PID_ASSISTANT_EN
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &_get, 1);                // ��ͨ�� 1 ����ʵ��ֵ
		//set_computer_value(SEND_FACT_CMD, CURVES_CH2, &set, 1); 
		//set_computer_value(SEND_FACT_CMD, CURVES_CH3, &get, 1);
#endif
	}

}
/*���Ʋ���*/
//void Motor_CeTui_Set_Position(int _set)
//{
//     static float y=0;
//     static uint16_t PWM=1500; 
//     if(stop3==1)//���յ�����ָ��
//    {
//        if(1000<=_set&&_set<=2000)//�ж�ָ���Ƿ���Ч��Χ
//        {
//            if(_set!=PWM )//�ж�ָ���Ƿ������ı�
//            {
//                if(PWM<_set)
//                {
//                    while(PWM!=_set)
//                    {
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, PWM);/* �޸ıȽ�ֵ����ռ�ձ� */
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, PWM);
//                        HAL_Delay(5);
//                        PWM++;
//                    }
//                }
//                else if(PWM>_set)
//                {
//                   while(_set!=PWM)
//                   {
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, PWM);/* �޸ıȽ�ֵ����ռ�ձ� */
//                        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, PWM);
//                       HAL_Delay(5);;
//                       PWM--;
//                   }
//                }
//               y=COUNT(_set);
//                printf("\r\npwm:%d;ռ�ձ�:%.1f%%;PWM:%.1fms;����:%.2fN;",_set,((float)PWM)/200,((float)PWM)/20000*20,10*y);
//            }
//            else
//            {
//                printf("\r\n�������ռ�ձ��ޱ仯"); 
//            }

//        }
//        else
//        {
//            printf("\r\n�������ռ�ձȲ��ڷ�Χ"); 
//        }
//        stop3=0;
//    }
//}





/*����ת�Ƕȼ���*/
float Convert_angle(int j, float x)//1ת�Ƕ�0ת����
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

/*����ռ�ձȶ�Ӧ���ĺ�������*/
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
    x /= 1000;  // �� x ��С 1000 ��
    for (int i = 0; i < sizeof(coefficients) / sizeof(coefficients[0]); i++) 
	{
        y += coefficients[i] * x_pow;
        x_pow *= x;
    }
    return y;
}











