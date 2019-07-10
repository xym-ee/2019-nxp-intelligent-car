#ifndef _KEY_H
#define _KEY_H

#include "system.h"

/*******************************************************************************
* Definitions
******************************************************************************/
//extern gpio_pin_config_t GPIO_Input_Config;

//操作类型定义
typedef struct _key_operations key_operations_t;  

struct _key_operations
{
    uint8_t (*get)(uint8_t mode);
};


//器件类型定义，器件一般包含数据操作
typedef struct _key_device key_device_t;

struct _key_device
{
    void (*init)(void);
    void (*interrupt_init)(void);
    const key_operations_t *ops;
};




extern const key_device_t key;




//定义模块号
typedef enum
{
    KEY0 = 0,
    KEY1 = 1,
    KEY2 = 2,   
} KEYn_e;
typedef enum
{
    LOW  = 0,  //DOWN
    HIGH = 1, //UP  
    FAIL = 0xff,
}KEYs_e;

void key_init(void);
void ExInt_Init(void);
uint8_t key_read(uint8_t mode);
extern void Test_GPIO_ExInt(void);
#endif 
