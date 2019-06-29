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

#ifndef _MPID_H
#define _MPID_H

typedef struct
{
  short     _kp;            //P
  short     _ki;            //I
  short     _kd;            //D
  short     _pid_out;       //pid输出
  short     _err;           //偏差
  short     _err1;          //上次偏差
  short     _err2;          //上上次偏差
  short     _sume;
}_pid_t;

extern short speedvalue;
extern _pid_t leftpid;
extern _pid_t rightpid;

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
void car_speed(short value);
void pid_value_init(_pid_t* base);
void pid_change(_pid_t* base,short kp,short ki,short kd);

#endif


