#ifndef _TSL1401_H
#define _TSL1401_H


//TSL1401引脚定义，方便控制
#define TSL_CLK				GPIO1_BASE_PTR->DRs.DR23
#define TSL_SI				GPIO1_BASE_PTR->DRs.DR16


void tsl1401_init(void);
void RD_TSL(void);
void tsl1401_test(void);

#endif
