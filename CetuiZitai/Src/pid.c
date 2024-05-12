#include "pid.h"

pid_t Motor_Big;    //创建大臂PID结构体
pid_t Motor_Small;  //创建小臂PID结构体 
pid_t Motor_Ce;     //创建小臂PID结构体 

/* PID控制------位置式和增量式 */
float PID_calc(pid_t* pid, float get, float set)
{
	static int index = 1;
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;//当前误差
	if(pid->max_err != 0 && ABS(pid->err[NOW]) >  pid->max_err  )  {return 0;}  //过大误差失能 
	if(pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband )	{return 0;} //死区内不再控制
	index=((pid->deadband !=0 &&  ABS(pid->err[NOW])>pid->deadband+4) ? 0 : 1); //积分分离>a积分不起作用

	
    if(pid->pid_mode == POSITION_PID) //位置式PID
    {
        pid->pout = pid->p * pid->err[NOW];
        pid->iout += pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST] );
        abs_limit(&(pid->iout), pid->integralLimit);//积分限值
        pid->pos_out = pid->pout + index* pid->iout + pid->dout;
        abs_limit(&(pid->pos_out), pid->maxOutput);//输出限值
        pid->last_pos_out = pid->pos_out;	//update last time 
    }
    else if(pid->pid_mode == DELTA_PID)//增量式PID
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
	PID_calc(&pid->outer,angleGet,angleSet);//计算外环(角度环)
	PID_calc(&pid->inner,pid->outer.pos_out,speedGet);//计算内环(速度环)
	pid->output=pid->inner.pos_out;
}




/*pid参数设定 */
void pid_param_init( pid_t* pid, //结构体指针所以下文->
                            uint32_t Mode, //模式
                           float MaxOutput,//最大输出值
                           float IntergralLimit,//积分限幅
                            float 	kp,
                            float 	ki,
                            float 	kd,
                            float Max_err, //最大误差
                            float Deadband)//死区值
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
/* pid参数重定(调试) */
 void pid_reset(pid_t *pid, float kp, float ki, float kd)
{
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}
/*pid初始化函数*/
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

    pid->f_param_init = pid_param_init;    //获取函数地址（函数名就是函数地址赋给函数指针从而指向该函数）
    pid->f_pid_reset = pid_reset;
    pid->f_param_init(pid, Mode, MaxOutput, IntergralLimit, kp, ki, kd,Max_err,Deadband);   //初始化函数参数（可以视为pid_param_init（a,b））
    pid->f_pid_reset(pid, kp, ki, kd);	


}

/*void set_pid_target( int temp_val)
{
   Small_Position= temp_val;    // 设置当前的目标值
}

float get_pid_target(void)
{
  return Small_Position;    // 设置当前的目标值
}*/

void abs_limit(float *a, float ABS_MAX)//取限定最值
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}
void abs_limit_min(float *a, float ABS_MIN)//取限定最值
{
    if(0<*a < ABS_MIN)
        *a = ABS_MIN;
    else if(0>*a > -ABS_MIN)
        *a = -ABS_MIN;
}

