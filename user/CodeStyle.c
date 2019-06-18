/*******************************************************************************
 *  
 *
 *  文件名称：CodeStyle.c
 *  简要描述：函数实现
 *
 *  最后修改日期：2019/6/17
 *
 ******************************************************************************/

#include "system.h"
   
   
/* 全局变量定义 */
uint8_t codestyle_a;
uint8_t codestyle_b;


/* 模块中的函数声明 */
//数据操作
static void codestyle_fun1(uint8_t d1,uint8_t d2);
static void codestyle_fun2(uint8_t d1,uint8_t d2);
static void codestyle_fun3(uint8_t d1,uint8_t d2);
static void codestyle_fun4(uint8_t d1,uint8_t d2);

//设备操作
static void codestyle_device1_init(void);
static void codestyle_device2_init(void);

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



