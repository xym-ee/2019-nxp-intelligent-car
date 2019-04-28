#ifndef _CONTROL_H
#define _CONTROL_H

#include "system.h"
#include "PID.h"

#define STRAIGHT_SPEED  80          //直道速度
#define BRAKE_SPEED     40          //入弯前减速
#define CURVE_SPEED     60          //弯道内速度

extern void TEST(void);
extern void LQ_Control_Motor(int16_t velocity);
extern void LQ_Control_Servo(void);    //PID舵机控制
extern void seek_road(void);
extern void LQ_Line_Hunting(uint8_t Pixle [Use_ROWS][Use_Line]);
#endif