/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com:He0L1w  NJUST >
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "system.h"

/*----------------------- 全局变量 ----------------------------*/

/* 车基速给定 */
short CarBaseSpeed = 0;


/*----------------------- 方法声明 ----------------------------*/


/* 依据曲率的目标车速计算 */
static void car_speed_calculate(void);

/* 依据曲率的后轮差速计算 */
static void car_speed_differential(void);

/* 依据电机转速计算值的PID控制 */
static void car_speed_control(void);

/* 依据曲率的舵机转角控制 */
static void car_direction_control(void);




/*----------------------- 接口定义 ----------------------------*/



/* 外部操作接口 */
//const car_operations_t car = {
//
//};



/*----------------------- 方法实现 ----------------------------*/


/* 依据曲率的目标车速计算 */
static void car_speed_calculate(void)
{



}


/* 依据曲率的后轮差速计算 */
static void car_speed_differential(void)
{


}

/* 依据电机转速计算值的PID控制 */
static void car_speed_control(void)
{


}


#define servo_mid 3000
typedef struct _pwm
{
	double Left_PWM;
	double Right_PWM;
}pwm_t;

pwm_t servo_PWM(double Ackman_R)
{
	pwm_t servo_pwm;

	if (Ackman_R > 200)		/* 半径大，是直路 */
		servo_pwm.Left_PWM = servo_pwm.Right_PWM = servo_mid;

	else if (Ackman_R>30 && Ackman_R<200)	
	{
		servo_pwm.Left_PWM = 0.0003 * pow(Ackman_R, 3.0) - 0.09*pow(Ackman_R, 2.0) + 11.20 * Ackman_R + 2390.0;
		servo_pwm.Right_PWM = -0.0003 * pow(Ackman_R, 3.0) + 0.09*pow(Ackman_R, 2.0) - 11.20 * Ackman_R + 3610.0;
	}

	return servo_pwm;
}



/* 依据曲率的舵机转角控制 */
static void car_direction_control(void)
{

}

