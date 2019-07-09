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

#ifndef __CODESTYLE_H
#define __CODESTYLE_H


#include "system.h"

   
/* 类型前置声明 */

//数据类型定义
typedef struct _xxx_data
{
    uint8_t     data1;
    uint8_t     data2;
    uint8_t     data3;
} xxx_data_t;


//操作类型定义
typedef struct _xxx_operations xxx_operations_t;  

struct _xxx_operations
{
    void (*operations1)(uint8_t d1,uint8_t d2);
    void (*operations2)(uint8_t d1,uint8_t d2);
    void (*operations3)(uint8_t d1,uint8_t d2);
    void (*operations4)(uint8_t d1,uint8_t d2);
};


//器件类型定义，器件一般包含数据操作
typedef struct _xxx_device xxx_device_t;

struct _xxx_device
{
    void (*devece1_init)(void);
    void (*devece2_init)(void);
    const xxx_operations_t *ops;
};




   
/* 外部变量与全局函数声明 */
extern uint8_t codestyle_a;
extern uint8_t codestyle_b;
extern const xxx_operations_t xxx_ops; //外部操作接口
extern const xxx_device_t xxxxxxx; //外部操作接口

void ops_example(void);




#endif /* __CODESTYLE_H */



