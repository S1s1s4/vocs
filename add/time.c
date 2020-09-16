#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "mytime.h"
#include <time.h>
#include "stdlib.h"
#include "string.h"

unsigned char __time_20ms=0;
unsigned char _time_1s=0;
unsigned  short int _time_20s=0;
const char Days[12]= {31,28,31,30,31,30,31,31,30,31,30,31};



void deinit_timer3(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE );
    TIM_Cmd(TIM3, DISABLE);
}
void init_timer3(unsigned short int arr,unsigned short int psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //

    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    TIM_TimeBaseStructure.TIM_Period = arr;        //
    TIM_TimeBaseStructure.TIM_Prescaler =psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //?????

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
    TIM_Cmd(TIM3, ENABLE);

}



//标准时间转换时间戳
int standard_to_stamp(uint16_t y,uint16_t m,uint16_t d,uint16_t h,uint16_t mi,uint16_t s)
{
    struct tm stm;

    memset(&stm,0,sizeof(stm));
    stm.tm_year=y+2000-1900;
    stm.tm_mon=m-1;
    stm.tm_mday=d;
    stm.tm_hour=h;
    stm.tm_min=mi;
    stm.tm_sec=s;

    return (int)mktime(&stm);
}


void delay_ms(uint16_t time) //毫秒延时
{
    uint16_t i = 0;
    while (time--)
    {
        i = 34000;
        while (i--)
            ;
    }
}
