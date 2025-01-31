#include "stm32f4xx.h"
#include <stdio.h>

#include "uart.h"





unsigned char Msg1TxBuf[Msg1BufLen];       //USART1???????????
unsigned char Msg1RxBuf[Msg1BufLen];       //USART1???????????
unsigned char Msg1RxUse[Msg1BufLen];
unsigned int Msg1RxCount = 0;   		      //USART1?????????
unsigned char Msg1RxLen = 0;
unsigned char Msg1RxFrameOK = 0;			  //?OK???


unsigned char Msg2TxBuf[60];       //USART2???????????
unsigned char Msg2RxBuf[Msg2BufLen];       //USART2???????????
//unsigned char Msg2RxUse[Msg2BufLen];
int Msg2RxCount = 0;   		      //USART2?????????
unsigned int Msg2RxLen = 0;
unsigned char Msg2RxFrameOK = 0;			  //?OK???


unsigned char Msg3TxBuf[Msg3BufLen];       //???????????
unsigned char Msg3RxBuf[Msg3BufLen];       //???????????
unsigned char Msg3RxUse[Msg3BufLen];
int Msg3RxCount = 0;   		      //?????????
unsigned int Msg3RxLen = 0;
unsigned char Msg3RxFrameOK = 0;			  //?OK???


unsigned char Msg4TxBuf[Msg4BufLen];       //???????????
unsigned char Msg4RxBuf[Msg4BufLen];       //???????????
unsigned char Msg4RxUse[Msg4BufLen];
int Msg4RxCount = 0;   		      //?????????
unsigned int Msg4RxLen = 0;
unsigned char Msg4RxFrameOK = 0;			  //?OK???


//串口1发送一个字符
void USART1SendChar(char ch)
{
    while((USART1->SR&0x40)==0) {};
    USART1->DR = (unsigned char) ch;
}
//串口1发送字符串
void USART1SendString(unsigned char *str, unsigned int strlen)
{
    unsigned int k= 0 ;
    do {
        USART1SendChar(*(str + k));
        k++;
    } while (k < strlen);
    //确保发完
    while((USART1->SR&0x40)==0) {};
}

//串口2发送一个字符
void USART2SendChar(unsigned char ch)
{
    while((USART2->SR&0x40)==0) {};
    USART2->DR = (unsigned char) ch;
}
//串口2发送字符串
void USART2SendString(unsigned char *str, unsigned int strlen)
{
    unsigned int k= 0 ;
    do {
        USART2SendChar(*(str + k));
        k++;
    } while (k < strlen);
    //确保发完
    while((USART2->SR&0x40)==0) {};
}

//串口3发送一个字符
void USART3SendChar(char ch)
{
    while((USART3->SR&0x40)==0) {};
    USART3->DR = (unsigned char) ch;
}
//串口3发送字符串 RS485
void USART3SendString(char *str, unsigned int strlen)
{
    unsigned int k= 0 ;

    //GPIO_SetBits(GPIOB, GPIO_Pin_12); //MAX485T;

    do {
        USART3SendChar(*(str + k));
        k++;
    } while (k < strlen);
    //确保发完
    while((USART3->SR&0x40)==0) {};

    //GPIO_ResetBits(GPIOB, GPIO_Pin_12);  //MAX485R;
}

//串口4发送一个字符
void UART4SendChar(char ch)
{
    while((UART4->SR&0x40)==0) {};
    UART4->DR = (unsigned char) ch;
}
//串口4发送字符串
void UART4SendString(char *str, unsigned int strlen)
{
    unsigned int k= 0 ;

    do {
        UART4SendChar(*(str + k));
        k++;
    } while (k < strlen);

    //确保发完
    while((UART4->SR&0x40)==0) {};
}

//串口5发送一个字符
void uart5SendChar(char ch)
{
    while((UART5->SR&0x40)==0) {};
    UART5->DR = (unsigned char) ch;
}
//串口5发送字符串
void uart5SendChars(char *str, unsigned int strlen)
{
    unsigned int k= 0 ;
    do {
        uart5SendChar(*(str + k));    //循环发送,直到发送完毕
        k++;
    }
    while (k < strlen);
}

//串口6发送一个字符
void uart6SendChar(char ch)
{
    while((USART6->SR&0x40)==0) {};
    USART6->DR = (unsigned char) ch;
}
//串口6发送字符串
void uart6SendChars(char *str, unsigned int strlen)
{
    unsigned int k= 0 ;
    do {
        uart6SendChar(*(str + k));    //循环发送,直到发送完毕
        k++;
    }
    while (k < strlen);
}




/*************************************************
  函数名称:   void bsp_Usart2_RecTime_Add(void)
  函数功能:  串口接收计时累加
  形参:      无
  返回：无
  其它：要被定时器调用，目前工程中定时器中断时间20ms
*************************************************/
unsigned char u3iSendCnt = 0;  //???????????????????
unsigned char u3cSendLock = 1; //???????????,??????????????
#define u3_const_receive_time 2

void bsp_Usart3_RecTime_Add(void)
{
    if (u3iSendCnt < u3_const_receive_time)
    {
        u3iSendCnt++;
        u3cSendLock = 1;
    }
}

unsigned char bsp_Usart3_Rec_New(void)
{
    if (u3iSendCnt < u3_const_receive_time)
    {
        return 0;
    }
    else if (u3cSendLock == 1)
    {
        u3cSendLock = 0; //
        return 1;
    }
    else
    {
        return 0;
    }
}
