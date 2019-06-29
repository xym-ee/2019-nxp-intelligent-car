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
   
   
/* 全局变量定义 */
uint8_t codestyle_a;
uint8_t codestyle_b;

/* ---------------------------- 方法声明 ------------------------------------ */
/* 模块中的函数声明 */
//数据操作
static void codestyle_fun1(uint8_t d1,uint8_t d2);
static void codestyle_fun2(uint8_t d1,uint8_t d2);
static void codestyle_fun3(uint8_t d1,uint8_t d2);
static void codestyle_fun4(uint8_t d1,uint8_t d2);

//设备操作
static void codestyle_device1_init(void);
static void codestyle_device2_init(void);

/* ---------------------------- 外部接口 ------------------------------------ */
/* 数据操作接口 */
const xxx_operations_t xxx_ops = {
        .operations1 = codestyle_fun1,
        .operations2 = codestyle_fun2,
        .operations3 = codestyle_fun3,
        .operations4 = codestyle_fun4,
};

/* 外部器件操作接口 */
const xxx_device_t xxxxxxx = {
        .devece1_init = codestyle_device1_init,
        .devece2_init = codestyle_device2_init,
        .ops = &xxx_ops
};



/* ---------------------------- 方法实现 ------------------------------------ */
/* 操作实现方法 */
static void codestyle_fun1(uint8_t d1,uint8_t d2)
{
  /*
      code...
  */
}

static void codestyle_fun2(uint8_t d1,uint8_t d2)
{
  /*
      code...
  */
}

static void codestyle_fun3(uint8_t d1,uint8_t d2)
{
  /*
      code...
  */
}

static void codestyle_fun4(uint8_t d1,uint8_t d2)
{
  /*
      code...
  */
}


static void codestyle_device1_init(void)
{
  /*
      code...
  */
}
static void codestyle_device2_init(void)
{
  /*
      code...
  */
}






/* 全局函数 */
void ops_example(void)
{
  xxx_ops.operations1(codestyle_a,codestyle_b);
  xxx_ops.operations2(codestyle_a,codestyle_b);
  xxx_ops.operations3(codestyle_a,codestyle_b);
  xxx_ops.operations4(codestyle_a,codestyle_b);
}



