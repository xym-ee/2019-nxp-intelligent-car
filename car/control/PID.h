// Header:
// File Name: 
// Author: Z
// Date:2018/08/03

#ifndef _PID_H
#define _PID_H

#include "system.h"

#define M_PI 3.1415926
#define _fCut   20                     //低通滤波  截止频率
#define RC 1/(2*M_PI*_fCut)
typedef struct
{
    float                _kp;         //P
    float                _ki;         //I
    float                _kd;         //D
    float                _imax;       //积分限幅
    
    float                _pid_out_p;  //KP输出
    float                _pid_out_i;  //KI输出
    float                _pid_out_d;  //KD输出
    float                _pid_out;    //pid输出
    
    float                _integrator; ///< integrator value
    float                _last_error; ///< last error for derivative
    float                _last_derivative;///< last derivative for low-pass filter
    uint32_t             _last_t;    ///< last time  
    float                scaler;      //比例
}_pid_param_t;

typedef struct
{
    float (* get_pid)(_pid_param_t * pid_param, float error);
    void (* reset)(_pid_param_t * pid_param);
    void (* reset_I)(_pid_param_t * pid_param);
}_pid_t;

extern _pid_t pid;
extern _pid_param_t  Motor_pid;
extern _pid_param_t  Servo_pid;

#endif