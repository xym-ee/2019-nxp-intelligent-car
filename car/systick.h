
// Header:
// File Name: 
// Author: Z
// Date:2018/08/03

#ifndef __M_SYSTIME_H
#define __M_SYSTIME_H

#include "system.h"

typedef struct
{
		
	void (* init) (void);  
	uint64_t (* get_time_us) (void);
	uint32_t (* get_time_ms) (void);
	void (* delay_us)(uint64_t);//延时1us 实测为1.23us   延时10us 实测为10.23us   延时100us 实测为100.23us
	void (* delay_ms)(uint32_t);

}systime_t;

extern systime_t  _systime;


#endif //__M_SYSTIME_H




