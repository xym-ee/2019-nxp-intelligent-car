#ifndef _KEY_H
#define _KEY_H

#include "system.h"


#define DISTANCE_SWITCH_MUX            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)  


#define BARRIER_CHECK  (GPIO_PinRead(GPIO2,24) == 0)


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
    void (*barrier_check)(void);
    const key_operations_t *ops;
};


extern const key_device_t key;


#endif 
