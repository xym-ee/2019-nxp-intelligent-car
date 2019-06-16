#ifndef _DIRCON_H
#define _DIRCON_H



typedef struct 
{
  uint8_t _dkp;
  uint8_t _dkd;
}dir_pd_t;

extern dir_pd_t pd;




#define SPEED1  50
#define SPEED2  90
#define SPEED3  120
#define SPEED4  150
#define SPEED5  180
#define SPEED6  210
#define SPEED7  240
#define SPEED8  260
#define SPEED9  280



void direction_ctrl(void);

#endif


