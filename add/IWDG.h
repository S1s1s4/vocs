#ifndef __IWDG_H
#define __IWDG_H
#include "stm32f4xx_iwdg.h"

void bsp_InitIwdg(unsigned short int _ulIWDGTime);
void IWDG_Feed(void);

#endif
