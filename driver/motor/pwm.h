#include "system.h"
#ifndef _PWM_H
#define _PWM_H

#define DUTY_MAX  10000     //duty最大值

/**
 *  初始化电机、舵机pwm输出
 *  ----------------
 */
void pwm_init(void);

/**
 *  设置电机占空比（速度）
 *  ----------------
 *  duty                占空比             -DUTY_MAX ~ DUTY_MAX
 *  ----------------
 *  duty最大值10000定义在DUTY_MAX
 *  
 *  left_motor(-2000)  //左电机反转  占空比  2000 / 10000 
 */
void left_motor(short duty);
void right_motor(short duty);

/**
 *  设置舵机角度
 *  ----------------
 *  duty                占空比             2000-4000
 *  ----------------
 *  舵机中值3000
 */
void servo(uint16_t duty);


void servo_test(void);
void test_motor(void);




#endif 



