#ifndef __PID_H
#define __PID_H

#include "App.h"
//#define ABS(x)		((x>0)? (x): (-x)) //取绝对值,运算性能更高
enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    POSITION_PID,//位置式PID,也是数值(3)
    DELTA_PID,//4
};
typedef struct __pid_t{             //标记类型__pid_t
                        float p,i,d;  
                        float set[3],get[3],err[3];
                        float pout,iout,dout;
                        float pos_out,last_pos_out;
                        float delta_u, delta_out,last_delta_out;  
                        float max_err, deadband,maxOutput,integralLimit;
                        uint32_t pid_mode;
                        /*函数指针*/
                        void (*f_param_init)(struct __pid_t *pid,  
											uint32_t Mode, //模式
										    float MaxOutput,//最大输出值
										    float IntergralLimit,//积分限幅
											float 	kp,
											float 	ki,
											float 	kd,
											float Max_err, //最大误差
											float Deadband);//死区值
                        void (*f_pid_reset)(struct __pid_t *pid,
                                            float kp,
                                            float ki,
                                            float kd);
}pid_t;		//结构体别名pid_t

typedef struct _CascadePID
{
	pid_t inner;//内环
	pid_t outer;//外环
	float output;//串级输出，等于inner.output
}CascadePID;
extern pid_t Motor_Big;    //创建大臂PID结构体
extern pid_t Motor_Small; //创建小臂PID结构体 


void PID_struct_init(pid_t* pid,
                    uint32_t Mode,
                    float MaxOutput,
                    float IntergralLimit,
                    float kp,
                    float ki,
                    float kd,
                    float Max_err,
                    float Deadband);

					
//void set_pid_target( int temp_val);
//float get_pid_target(void);
float PID_calc(pid_t* pid, float get, float set);
void PID_CascadeCalc(CascadePID *pid,float angleGet,float speedGet,float angleSet);					
					
void abs_limit(float *a, float ABS_MAX);
void abs_limit_min(float *a, float ABS_MIN);
					

void pid_param_init( pid_t* pid, //结构体指针所以下文->
                            uint32_t Mode, //模式
                            float MaxOutput,//最大输出值
                            float IntergralLimit,//积分限幅
                            float 	kp,
                            float 	ki,
                            float 	kd,
                            float Max_err, //最大误差
                            float Deadband);//死区值
void pid_reset(pid_t *pid, float kp, float ki, float kd);




							

#endif




















