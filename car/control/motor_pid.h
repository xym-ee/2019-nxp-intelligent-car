#ifndef _MPID_H
#define _MPID_H


typedef struct
{
  short     _kp;         //P
  short     _ki;         //I
  short     _kd;         //D
  short     _pid_out;    //pid输出
  short     _err;       //偏差
  short     _err1;      //上次偏差
  short     _err2;      //上上次偏差
  short     _sume;
}_pid_t;


void pid_value_init(_pid_t* base);
void pid_change(_pid_t* base,short kp,short ki,short kd);
short pid_control(_pid_t* base,short targer,short real);



/**
 *  车模速度控制初始化
 *  ----------------
 *  采用了PID控制算法
 */
void pid_control_init(void);
/**
 *  车模速度设置
 *  ----------------
 */
void speedcontrol(short value);


#endif


