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
int standard_to_stamp(unsigned  short int y,unsigned  short int m,unsigned  short int d,unsigned  short int h,unsigned  short int mi,unsigned  short int s);
extern unsigned char __time_20ms;
extern unsigned char _time_1s;
extern unsigned  short int _time_20s;
void delay_ms(unsigned  short int time);
extern unsigned  short int year,mon,day,second, min, hour;
extern  unsigned int TIMEP;
void deinit_timer3(void);
#endif
