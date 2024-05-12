#include "pid.h"

pid_t Motor_Big;    //�������PID�ṹ��
pid_t Motor_Small;  //����С��PID�ṹ�� 
pid_t Motor_Ce;     //����С��PID�ṹ�� 

/* PID����------λ��ʽ������ʽ */
float PID_calc(pid_t* pid, float get, float set)
{
	static int index = 1;
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;//��ǰ���
	if(pid->max_err != 0 && ABS(pid->err[NOW]) >  pid->max_err  )  {return 0;}  //�������ʧ�� 
	if(pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband )	{return 0;} //�����ڲ��ٿ���
	index=((pid->deadband !=0 &&  ABS(pid->err[NOW])>pid->deadband+4) ? 0 : 1); //���ַ���>a���ֲ�������

	
    if(pid->pid_mode == POSITION_PID) //λ��ʽPID
    {
        pid->pout = pid->p * pid->err[NOW];
        pid->iout += pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST] );
        abs_limit(&(pid->iout), pid->integralLimit);//������ֵ
        pid->pos_out = pid->pout + index* pid->iout + pid->dout;
        abs_limit(&(pid->pos_out), pid->maxOutput);//�����ֵ
        pid->last_pos_out = pid->pos_out;	//update last time 
    }
    else if(pid->pid_mode == DELTA_PID)//����ʽPID
    {
        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
        pid->iout = pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);       
        abs_limit(&(pid->iout), pid->integralLimit);
        pid->delta_u = pid->pout + pid->iout + pid->dout;
        pid->delta_out = pid->last_delta_out + pid->delta_u;
        abs_limit(&(pid->delta_out), pid->maxOutput);
        pid->last_delta_out = pid->delta_out;	//update last time
    }    
    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return pid->pid_mode==POSITION_PID ? pid->pos_out : pid->delta_out;
}



void PID_CascadeCalc(CascadePID *pid,float angleGet,float speedGet,float angleSet)
{
	PID_calc(&pid->outer,angleGet,angleSet);//�����⻷(�ǶȻ�)
	PID_calc(&pid->inner,pid->outer.pos_out,speedGet);//�����ڻ�(�ٶȻ�)
	pid->output=pid->inner.pos_out;
}




/*pid�����趨 */
void pid_param_init( pid_t* pid, //�ṹ��ָ����������->
                            uint32_t Mode, //ģʽ
                           float MaxOutput,//������ֵ
                           float IntergralLimit,//�����޷�
                            float 	kp,
                            float 	ki,
                            float 	kd,
                            float Max_err, //������
                            float Deadband)//����ֵ
{   
    pid->integralLimit = IntergralLimit;
    pid->maxOutput = MaxOutput;
    pid->pid_mode = Mode;    
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid->max_err=Max_err;
    pid->deadband=Deadband;

}
/* pid�����ض�(����) */
 void pid_reset(pid_t *pid, float kp, float ki, float kd)
{
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}
/*pid��ʼ������*/
//PID_struct_init(&Motor_Small,POSITION_PID,1000.0f,200.0f,2000.0f,20.5f,50.0f,10000.0f,0.001f);
void PID_struct_init(pid_t* pid,
                    uint32_t Mode,
                    float MaxOutput,
                    float IntergralLimit,
                    float kp,
                    float ki,
                    float kd,
                    float Max_err,
                    float Deadband)
{

    pid->f_param_init = pid_param_init;    //��ȡ������ַ�����������Ǻ�����ַ��������ָ��Ӷ�ָ��ú�����
    pid->f_pid_reset = pid_reset;
    pid->f_param_init(pid, Mode, MaxOutput, IntergralLimit, kp, ki, kd,Max_err,Deadband);   //��ʼ������������������Ϊpid_param_init��a,b����
    pid->f_pid_reset(pid, kp, ki, kd);	


}

/*void set_pid_target( int temp_val)
{
   Small_Position= temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

float get_pid_target(void)
{
  return Small_Position;    // ���õ�ǰ��Ŀ��ֵ
}*/

void abs_limit(float *a, float ABS_MAX)//ȡ�޶���ֵ
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}
void abs_limit_min(float *a, float ABS_MIN)//ȡ�޶���ֵ
{
    if(0<*a < ABS_MIN)
        *a = ABS_MIN;
    else if(0>*a > -ABS_MIN)
        *a = -ABS_MIN;
}

