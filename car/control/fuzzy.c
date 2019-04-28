#include "system.h"

#define PL   6
#define PM   4
#define PS   2 
#define NL   -6
#define NM   -4
#define NS   -2
#define ZE   0

/*原始PID参数*/
_pid_t direction;
/*PID增量*/
_pid_t dir_change;


/*Kp取值*/
static const float fuzzyRuleKp[7][7]={
  PL, PL, PM, PM, PS, PS, ZE,
  PL, PL, PM, PM, PS, ZE, ZE,
  PM, PM, PM, PS, ZE, NS, NM,
  PM, PS, PS, ZE, NS, NM, NM,
  PS, PS, ZE, NS, NS, NM, NM,
  ZE, ZE, NS, NM, NM, NM, NL,
  ZE, NS, NS, NM, NM, NL, NL
};
/*Ki取值*/
static const float fuzzyRuleKi[7][7]={
  NL, NL, NL, NM, NM, ZE, ZE,
  NL, NL, NM, NM, NS, ZE, ZE,
  NM, NM, NS, NS, ZE, PS, PS,
  NM, NS, NS, ZE, PS, PS, PM,
  NS, NS, ZE, PS, PS, PM, PM,
  ZE, ZE, PS, PM, PM, PL, PL,
  ZE, ZE, PS, PM, PL, PL, PL
};
/*Kd取值*/
static const float fuzzyRuleKd[7][7]={
  PS, PS, ZE, ZE, ZE, PL, PL,
  NS, NS, NS, NS, ZE, NS, PM,
  NL, NL, NM, NS, ZE, PS, PM,
  NL, NM, NM, NS, ZE, PS, PM,
  NL, NM, NS, NS, ZE, PS, PS,
  NM, NS, NS, NS, ZE, PS, PS,
  PS, ZE, ZE, ZE, ZE, PL, PL
};

/*动态PID参数类型定义*/
typedef struct{
  float Kp;
  float Ki;
  float Kd;
}PID;

/**
 *  PID在线调整(模糊控制)
 *  ----------------
 *  e   偏差
 *  ec  偏差变化
 *  ----------------
 *  返回PID参数的调整量
 */
void fuzzy(_pid_t *base,short e,short ec)
{
  float etemp,ectemp;
  float eLefttemp,eRighttemp;
  float ecLefttemp,ecRighttemp;
  
  int eLeftIndex,ecLeftIndex;
  int eRightIndex,ecRightIndex;
  //PID fuzzy_PID;
  
 /*模糊化*/ 
  etemp = e > 3.0 ? 0.0 : (e < - 3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5: (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0) ))));
//  if(e>3.0)
//    etemp = 0.0;
//  if((e<3.0)&&(e>=2.0))
//    etemp = 2.5;
//  if((e<2.0)&&(e>=1.0))
//    etemp = 1.5;
//  if((e<1.0)&&(e>=0.0))
//    etemp = 0.5;
//  if((e<0)&&(e>=-1.0))
//    etemp = -0.5;
//  if((e<-1.0)&&(e>=-2.0))
//    etemp = -1.5;
//  if((e<-2.0)&&(e>=-3.0))
//    etemp = -2.5;

  eLeftIndex = e;
  eRightIndex = eLeftIndex;
  eLeftIndex = (int)((etemp-0.5) + 3);        //[-3,3] -> [0,6]
  eRightIndex = (int)((etemp+0.5) + 3);
  
  eLefttemp = etemp == 0.0 ? 0.0:((etemp+0.5)-e);
  eRighttemp = etemp == 0.0 ? 0.0:( e-(etemp-0.5));
  
  ectemp = ec > 3.0 ? 0.0 : (ec < - 3.0 ? 0.0 : (ec >= 0.0 ? (ec >= 2.0 ? 2.5: (ec >= 1.0 ? 1.5 : 0.5)) : (ec >= -1.0 ? -0.5 : (ec >= -2.0 ? -1.5 : (ec >= -3.0 ? -2.5 : 0.0) ))));
  
  ecLeftIndex = (int)((ectemp-0.5) + 3);        //[-3,3] -> [0,6]
  ecRightIndex = (int)((ectemp+0.5) + 3);
  ecLefttemp = ectemp == 0.0 ? 0.0:((ectemp+0.5)-ec);
  ecRighttemp = ectemp == 0.0 ? 0.0:( ec-(ectemp-0.5));
  
 /*************************************反模糊*************************************/
  base->_kp = (short)((eLefttemp * ecLefttemp *  fuzzyRuleKp[ecLeftIndex][eLeftIndex]
                  + eLefttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eLeftIndex]
                    + eRighttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eRightIndex]
                      + eRighttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eRightIndex]));
  
  base->_ki = (short)((eLefttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eLeftIndex]
                  + eLefttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eLeftIndex]
                   + eRighttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eRightIndex]
                    + eRighttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eRightIndex]));
  
  base->_kp = (short)((eLefttemp * ecLefttemp *    fuzzyRuleKd[ecLeftIndex][eLeftIndex]
                  + eLefttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eLeftIndex]
                    + eRighttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eRightIndex]
                      + eRighttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eRightIndex]));

}


/**
 *  控制输出值
 *  ----------------
 *  e   车头偏差
 *  ec  车身中线与引导线夹角(积分即为变化率)
 *  ----------------
 *  普通的PID控制器
 */
short direction_pid(_pid_t* base,short e,short ec)
{
  short servo_control;   //舵机pwm调整量

  if((e<2)&&(e>-2))//积分分离
    base->_sume =+ e;

  fuzzy(&dir_change, e, ec); //PID参数调整量保存在dir_change内

  servo_control = (base->_kp + dir_change._kp)*e
                + (base->_ki + dir_change._ki)*(base->_sume)
                + (base->_kd + dir_change._kd)*ec;
  
  return servo_control;
}


void fuzzy_test(void)
{
  LCD_Init();
  LCD_CLS();
  adc1_init();

  
  pid_value_init(&direction);
  pid_change(&direction,30,0,4);

  pid_value_init(&dir_change);//增量初始化为0
   
  pid_control_init();
  pit_init(kPIT_Chnl_0, 10000);    //10ms 一次中断
  
  while(1)
  {
    LED_Color_Reverse(red);
    delayms(100);
  }
  
}

