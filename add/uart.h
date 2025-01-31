#ifndef __uart_H
#define __uart_H
#include "user.h"
#ifdef __cplusplus
extern "C" {
#endif
#define Msg1BufLen 80
#define Msg2BufLen 300
#define Msg3BufLen 200
#define Msg4BufLen 200

extern unsigned char Msg1TxBuf[Msg1BufLen];       //USART1???????????
extern unsigned char Msg1RxBuf[Msg1BufLen];       //USART1???????????
extern unsigned char Msg1RxUse[Msg1BufLen];
extern unsigned int Msg1RxCount ;   		      //USART1?????????
extern unsigned char  Msg1RxLen ;
extern unsigned char Msg1RxFrameOK ;			  //?OK???


extern unsigned char Msg2TxBuf[60];       //USART2???????????
extern unsigned char Msg2RxBuf[Msg2BufLen];       //USART2???????????
//extern unsigned char Msg2RxUse[Msg2BufLen];
extern int Msg2RxCount ;   		      //USART2?????????
extern unsigned int Msg2RxLen ;
extern unsigned char Msg2RxFrameOK ;			  //?OK???


extern unsigned char Msg3TxBuf[Msg3BufLen];       //???????????
extern unsigned char Msg3RxBuf[Msg3BufLen];       //???????????
extern unsigned char Msg3RxUse[Msg3BufLen];
extern int Msg3RxCount ;   		      //?????????
extern unsigned int Msg3RxLen ;
extern unsigned char Msg3RxFrameOK ;			  //?OK???


extern unsigned char Msg4TxBuf[Msg4BufLen];       //???????????
extern unsigned char Msg4RxBuf[Msg4BufLen];       //???????????
extern unsigned char Msg4RxUse[Msg4BufLen];
extern int Msg4RxCount ;   		      //?????????
extern unsigned int Msg4RxLen ;
extern unsigned char Msg4RxFrameOK ;			  //?OK???

void USART1SendChar(char ch);
void USART1SendString(unsigned char *str, unsigned int strlen);
void USART2SendChar(unsigned char ch);
void USART2SendString(unsigned char *str, unsigned int strlen);
void USART3SendChar(char ch);
void USART3SendString(char *str, unsigned int strlen);
void UART4SendChar(char ch);
void UART4SendString(char *str, unsigned int strlen);
void uart5SendChar(char ch);
void uart5SendChars(char *str, unsigned int strlen);
void uart6SendChar(char ch);
void uart6SendChars(char *str, unsigned int strlen);
/* Exported functions ------------------------------------------------------- */
void bsp_Usart3_RecTime_Add(void);
unsigned char bsp_Usart3_Rec_New(void);
extern unsigned char u3cSendLock;
extern unsigned char u3iSendCnt;
#ifdef __cplusplus
}
#endif

#endif /* __APP_H */
