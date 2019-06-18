/*******************************************************************************
 *  
 *
 *  文件名称：CodeStyle.h
 *  简要描述：代码风格的头文件内容
 *
 *  最后修改日期：2019/6/17
 *
 ******************************************************************************/

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
} _xxx_data_t;


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



