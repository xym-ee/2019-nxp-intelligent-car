// Header:
// File Name: 
// Author: Z
// Date:2018/08/03


#include "PID.h"

//函数功能： PID 积分置零
//函数参数： PID参数结构体 
//函数返回值： 无
void reset_I(_pid_param_t * pid_param)
{
    pid_param->_integrator = 0;
	// we use NAN (Not A Number) to indicate that the last 
	// derivative value is not valid
    pid_param->_last_derivative = NAN;
}
//函数功能： PID全部参数置零
//函数参数： PID参数结构体 
//函数返回值： 无
void reset(_pid_param_t * pid_param) 
{
    memset(&pid_param, 0, sizeof(pid_param));
}

//函数功能： 得到PID输出结果
//函数参数： PID参数结构体   误差
//函数返回值： PID输出值
float get_pid(_pid_param_t * pid_param, float error)
{
    uint32_t tnow = _systime.get_time_ms();
    uint32_t dt = tnow - pid_param->_last_t;
    float output            = 0;
    float delta_time;
    
    if (pid_param->_last_t == 0 || dt > 6) {       //限制超时时间6ms 自行修改 
        dt = 0;
        
		// if this PID hasn't been used for a full second then zero
		// the intergator term. This prevents I buildup from a
		// previous fight mode from causing a massive return before
		// the integrator gets a chance to correct itself
        reset_I(pid_param);
    }
    pid_param->_last_t = tnow;
    
    delta_time = (float)dt / 1000.0f;
    
    // Compute proportional component
    pid_param->_pid_out_p = error * pid_param->_kp;
    output += pid_param->_pid_out_p;
    
    // Compute derivative component if time has elapsed
    if ((fabsf(pid_param->_kd) > 0) && (dt > 0)) {
        float derivative;
        
		if (isnan(pid_param->_last_derivative)) {
			// we've just done a reset, suppress the first derivative
			// term as we don't want a sudden change in input to cause
			// a large D output change			
			derivative = 0;
			pid_param->_last_derivative = 0;
		} else {
			derivative = (error - pid_param->_last_error) / delta_time;
		}
        
        // discrete low pass filter, cuts out the
        // high frequency noise that can drive the controller crazy
        derivative = pid_param->_last_derivative + ((delta_time / (RC + delta_time)) *(derivative - pid_param->_last_derivative));
        
        // update state
        pid_param->_last_error             = error;
        pid_param->_last_derivative        = derivative;
        
        // add in derivative component
        pid_param->_pid_out_d = pid_param->_kd * derivative;
        output += pid_param->_pid_out_d;
    }
    
    // scale the P and D components
    output *= pid_param->scaler;
    pid_param->_pid_out_d *= pid_param->scaler;
    pid_param->_pid_out_p *= pid_param->scaler;
    
    // Compute integral component if time has elapsed
    if ((fabsf(pid_param->_ki) > 0) && (dt > 0)) {
        pid_param->_integrator             += (error * pid_param->_ki) * pid_param->scaler * delta_time;
        if (pid_param->_integrator < -pid_param->_imax) 
        {
            pid_param->_integrator = -pid_param->_imax;
        } 
        else if (pid_param->_integrator > pid_param->_imax) 
        {
            pid_param->_integrator = pid_param->_imax;
        }
        pid_param->_pid_out_i = pid_param->_integrator;
        output += pid_param->_integrator;
    }
    pid_param->_pid_out = output;
    return output;
}

_pid_t pid =
{
    get_pid,
    reset,
    reset_I,
};







