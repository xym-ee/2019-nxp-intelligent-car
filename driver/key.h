#ifndef _KEY_H
#define _KEY_H

#include "system.h"


typedef enum _key_name_t
{
  no_key      = 0U,
  key_minus   = 1U,
  key_plus    = 2U,
  key_ok      = 3U,
}key_name_t;



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


#endif 
