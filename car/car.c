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

const car_calculate_operations_t car_calculate_ops = {
        .speed = car_speed_calculate,
        .differential = car_speed_differential,
};

const car_control_operations_t car_control_ops = {
        .speed = car_speed_control,
        .direction = car_direction_control,

};


/* 外部操作接口 */
const car_device_t Car = {
        .calculate = &car_calculate_ops,
        .control = &car_control_ops,
};



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

/* 依据曲率的舵机转角控制 */
static void car_direction_control(void)
{
//  double x0,y0,x1,y1;
//  static double alpha,last_alpha;
//  /* 实际点位置 */
//  x0 = Loc_change(90,187).x;
//  y0 = Loc_change(90,187).y;
//  
//  /* 目标点位置 */
//  x1 = Loc_change(90,midline[90]).x;
//  y1 = Loc_change(90,midline[90]).y;
//  alpha = atan(x1/y1);
//  uint16_t pwm;
//  pwm = (uint16_t)(3000 + 600*alpha);
//  servo(pwm);

}

