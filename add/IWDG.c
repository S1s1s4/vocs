#include "IWDG.h"





/*************************************************
函数名: bsp_InitIwdg
功能说明: 独立看门狗时间配置函数
形参：IWDGTime: 0 - 0x0FFF，设置的是128分频，LSI的时钟频率按40KHz计算。
      128分频的情况下，最小3.2ms( 1/(40000/128)=3.2ms)，最大13107.2(3.2*4096=13107.2)ms。
返 回 值: 无
**************************************************/
void bsp_InitIwdg(unsigned short int _ulIWDGTime)
{

    /* 检测系统是否从独立看门狗复位中恢复 */
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        /* 清除复位标志 */
        RCC_ClearFlag();
    }

    /* 使能LSI */
    RCC_LSICmd(ENABLE);

    /* 等待直到LSI就绪 */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /* 写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* 128 分频*/
    IWDG_SetPrescaler(IWDG_Prescaler_128);

    IWDG_SetReload(_ulIWDGTime);

    /* 重载IWDG计数 */
    IWDG_ReloadCounter();

    /* 使能 IWDG (LSI oscillator 由硬件使能) */
    IWDG_Enable();
}

/*************************************************
*	函 数 名: IWDG_Feed
*	功能说明: 喂狗函数
*	形    参：无
*	返 回 值: 无
**************************************************/
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}
