/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "mytime.h"
#include "uart.h"
#include "main.h"
extern unsigned short int flag_5_min;
extern unsigned char gg;
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}



uint32_t SysTickCount=0;

//extern uint32_t TimeCount;
extern uint16_t second,min,hour,consttimeflag;
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{

    SysTickCount++;
    //TimeCount++;   //


    //????
    if(SysTickCount%100==0) {
        SysTickCount = 0;
        second++;
        TIMEP++;
        if(second>=60)
        {
            ++min;
//            if(min%5==0)
//                flag_5_min=1;
            second=0;
            if (min >= 60)
            {
                hour++;

                if (hour>=24)
                    hour = 0;
                consttimeflag=1;//?????????
            }
        }
    }
    //以15%占空比闪烁心跳灯
    if(SysTickCount%100<=15) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    }
    else {
        GPIO_SetBits(GPIOC, GPIO_Pin_6);
    }

}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

extern unsigned char Msg1TxBuf[];
extern unsigned char Msg1RxBuf[];
extern unsigned int Msg1RxCount;
extern unsigned char Msg1RxLen;
extern unsigned char Msg1RxFrameOK;			  //帧OK标志
void USART1_IRQHandler(void)
{
    //如果是接收中断
    if((USART1->SR & 0x020) != 0)	{
        //内部测试指令接收
        Msg1RxBuf[Msg1RxCount] = USART1->DR;   //接收数据读入缓冲区
        if(Msg1RxBuf[Msg1RxCount]=='u')  {
            flag_5_min = 301;				 //如果是结束符，置帧OK标志
            //Msg1RxLen = Msg1RxCount+1;
            Msg1RxBuf[Msg1RxCount]=0;
            Msg1RxCount = 0;					 //清零接收计数器
        }
        else if(Msg1RxBuf[Msg1RxCount]=='g')
        {
            gg=1;
        }
        else if(Msg1RxBuf[Msg1RxCount]=='G')
        {
            gg=2;
        } else {
            Msg1RxCount++;					 //否则继续计数并判断是否溢出
            if(Msg1RxCount>=79) {
                Msg1RxCount = 0;     //如果接收溢出，采用简单的循环方式
            }


        }
    }
}

extern unsigned char Msg2TxBuf[];
extern unsigned char Msg2RxBuf[];
extern int Msg2RxCount;
extern unsigned int Msg2RxLen;
extern unsigned char Msg2RxFrameOK;			  //帧OK标志
void USART2_IRQHandler(void)
{
    //如果是接收中断
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)	{
        Msg2RxBuf[Msg2RxCount++] = USART2->DR;   //接收数据读入缓冲区
        if(Msg2RxCount>=299)
        {
            Msg2RxCount = 0;
        }
    }
    else
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
}
/*void USART2_IRQHandler(void)
{
	//如果是接收中断
	if((USART2->SR & 0x020) != 0)	{
		//内部测试指令接收
		Msg2RxBuf[Msg2RxCount] = USART2->DR;   //接收数据读入缓冲区
		if(Msg2RxBuf[Msg2RxCount]=='\n')  {
			Msg2RxFrameOK = 1;				 //如果是结束符，置帧OK标志
			Msg2RxLen = Msg2RxCount+1;
			Msg2RxCount = 0;					 //清零接收计数器
		} else {
			Msg2RxCount++;					 //否则继续计数并判断是否溢出
			if(Msg2RxCount>=190) {  Msg2RxCount = 0;  }	 //如果接收溢出，采用简单的循环方式

		}
	}
}*/

extern unsigned char Msg3TxBuf[];
extern unsigned char Msg3RxBuf[];
extern int Msg3RxCount;
extern unsigned int Msg3RxLen;
extern unsigned char Msg3RxFrameOK;			  //帧OK标志
void USART3_IRQHandler(void)
{
    unsigned char Res;
    //如果是接收中断  USART_GetITStatus(USARTx, USART_IT_RXNE) == SET
    //if((USART3->SR & 0x020) != 0)	{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)	{
        ++Msg3RxCount;
        Res = USART3->DR;   //接收数据读入缓冲区
        if(Msg3RxCount>Msg3BufLen)
        {
            Msg3RxCount=0;
        }
        Msg3RxBuf[Msg3RxCount-1]=Res;
        u3iSendCnt=0;
    }
    else
        USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
}

extern unsigned char Msg4TxBuf[];
extern unsigned char Msg4RxBuf[];
extern int Msg4RxCount;
extern unsigned int Msg4RxLen;
extern unsigned char Msg4RxFrameOK;			  //帧OK标志
void UART4_IRQHandler(void)
{
    //如果是接收中断
    if((UART4->SR & 0x020) != 0)	{
        //内部测试指令接收
        Msg4RxBuf[Msg4RxCount] = UART4->DR;   //接收数据读入缓冲区
        if(Msg4RxBuf[Msg4RxCount]=='\n')  {
            Msg4RxFrameOK = 1;				 //如果是结束符，置帧OK标志
            Msg4RxLen = Msg4RxCount+1;
            Msg4RxCount = 0;					 //清零接收计数器
        } else {
            Msg4RxCount++;					 //否则继续计数并判断是否溢出
            if(Msg4RxCount>=190) {
                Msg4RxCount = 0;     //如果接收溢出，采用简单的循环方式
            }
        }
    }
}


void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //??????
    {
        //串口计时
        bsp_Usart3_RecTime_Add();

        if(__time_20ms<50)
            __time_20ms++;
        if(__time_20ms>=50)
        {
            if(_time_1s<5)
                _time_1s++;
            __time_20ms=0;
            if(flag_5_min<300)
                flag_5_min++;
        }
        if(_time_20s<1000)
            _time_20s++;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update );        //??????

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
