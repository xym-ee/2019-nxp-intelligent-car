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


/* 电机速度类型定义 */
typedef struct _motor_speed
{
    short     left;
    short     right;
    short     enc_left;
    short     enc_right;
} motor_speed_t;


/* 单个电机的pid参数类型定义 */
typedef struct __motor_pid
{
  float     kp;
  float     ki;
  float     kd;
  float     ut;
  float     err;
  float     err1;
  float     err2;
  float     int_err;
} _motor_pid_t; 

/* pid参数类型定义 */
typedef struct _motor_pid
{
  _motor_pid_t *left;
  _motor_pid_t *right;
} motor_pid_t;


typedef struct _mpid_operations mpid_operations_t;  

struct _mpid_operations
{
  void (*init)(void);
  void (*pidclear)(motor_pid_t *base); 
  void (*pidchange)(motor_pid_t* base,float p,float i,float d);
  void (*pidcontrol)(motor_speed_t *speed);
  void (*pidtest)(void);
};


extern const mpid_operations_t motor;
extern motor_pid_t pid;
extern motor_speed_t motor_speed;


#endif


