#ifndef __mytime_h
#define __mytime_h


#include "user.h"
typedef unsigned int time_t;

//struct tm{

//    int tm_sec;
//    int tm_min;
//    int tm_hour;
//    int tm_mday;
//    int tm_mon;
//    int tm_year;
//};



void init_timer3(unsigned short int arr,unsigned short int psc);
int standard_to_stamp(u16 y,u16 m,u16 d,u16 h,u16 mi,u16 s);
extern unsigned char __time_20ms;
extern unsigned char _time_1s;
extern u16 _time_20s;
void delay_ms(u16 time);

#endif
