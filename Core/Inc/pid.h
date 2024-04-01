#ifndef __PID_H
#define __PID_H
#include "stdio.h"
#include "stm32f4xx.h"
#include <math.h>

//#define ABS(x)		((x>0)? (x): (-x)) //ȡ����ֵ,�������ܸ���
enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    POSITION_PID,//λ��ʽPID,Ҳ����ֵ(3)
    DELTA_PID,//4
};
typedef struct __pid_t{             //�������__pid_t
                        float p,i,d;  
                        float set[3],get[3],err[3];
                        float pout,iout,dout;
                        float pos_out,last_pos_out;
                        float delta_u, delta_out,last_delta_out;  
                        float max_err, deadband,maxOutput,integralLimit;
                        uint32_t pid_mode;
                        /*����ָ��*/
                        void (*f_param_init)(struct __pid_t *pid,  
                                            uint32_t pid_mode,
                                           float maxOutput,
                                           float integralLimit,
                                            float p,
                                            float i,
                                            float d,
                                            float Max_err,
                                            float Deadband);
                        void (*f_pid_reset)(struct __pid_t *pid,
                                            float p,
                                            float i,
                                            float d);
}pid_t;		//�ṹ�����pid_t

typedef struct _CascadePID
{
	pid_t inner;//�ڻ�
	pid_t outer;//�⻷
	float output;//�������������inner.output
}CascadePID;




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
					
extern pid_t Motor_Small; //����С��PID�ṹ�� 

void pid_param_init( pid_t* pid, //�ṹ��ָ����������->
                            uint32_t Mode, //ģʽ
                            float MaxOutput,//������ֵ
                            float IntergralLimit,//�����޷�
                            float 	kp,
                            float 	ki,
                            float 	kd,
                            float Max_err, //������
                            float Deadband);//����ֵ
void pid_reset(pid_t *pid, float kp, float ki, float kd);




							

#endif




















