#ifndef __PID_H
#define __PID_H
#include "main.h"

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
                        float maxOutput, fenli_err,integralLimit, max_err, deadband;
                        uint32_t pid_mode;
                        /*����ָ��*/
                        void (*f_param_init)(struct __pid_t *pid,  
											uint32_t Mode, //ģʽ
										    float MaxOutput,//������ֵ�޷�
											float FenLi_err,
										    float IntergralLimit,//�����޷�
											float 	kp,
											float 	ki,
											float 	kd,
											float Max_err, //������
											float Deadband);//����ֵ
                        void (*f_pid_reset)(struct __pid_t *pid,
                                            float kp,
                                            float ki,
                                            float kd);
}pid_t;		//�ṹ�����pid_t
void PID_struct_init(pid_t* pid,
                    uint32_t Mode,
                    float MaxOutput,
					float FenLi_err,
                    float IntergralLimit,
                    float kp,
                    float ki,
                    float kd,
                    float Max_err,
                    float Deadband);
typedef struct _CascadePID
{
	pid_t inner;//�ڻ�
	pid_t outer;//�⻷
	float output;//�������������inner.output
}CascadePID;

extern pid_t Pid_Jian;    //�������PID�ṹ��
extern pid_t Pid_Big;    //�������PID�ṹ��
extern pid_t Pid_Small; //����С��PID�ṹ�� 
extern pid_t Pid_Wan;    //�������PID�ṹ��
extern pid_t Pid_Ce;     //����С��PID�ṹ�� 



float PID_calc(pid_t* pid, float get, float set);
void PID_CascadeCalc(CascadePID *pid,float angleGet,float speedGet,float angleSet);					
									

/*void pid_param_init( pid_t* pid, //�ṹ��ָ����������->
                            uint32_t Mode, //ģʽ
                            float MaxOutput,//������ֵ
                            float IntergralLimit,//�����޷�
                            float 	kp,
                            float 	ki,
                            float 	kd,
                            float Max_err, //������
                            float Deadband);//����ֵ
void pid_reset(pid_t *pid, float kp, float ki, float kd);*/


//void set_pid_target( int temp_val);
//float get_pid_target(void);

							

#endif




















