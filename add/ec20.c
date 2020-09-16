#include "ec20.h"
#include "mytime.h"
#include "uart.h"
#include "gprs_dev.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stmflash.h"
extern  unsigned short int NRB_flag;

int FindString(char * pSrc, int srcSize, char * pDest, int dstSize)
{
    int iFind = -1;
    int j=0,i=0;
    int iCnt = 0;
    for( i=0; i<srcSize; i++)
    {
        iCnt=0;
        for ( j=0; j<dstSize; j++)
        {
            if(pDest[j] == pSrc[i+j])
                iCnt++;
        }
        if (iCnt==dstSize) {
            iFind = i;
            break;
        }
    }
    return iFind;
}

void AT_START()
{
    int i;
    i=sprintf((char*)Msg2TxBuf,"AT\r\n");
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);
    i=sprintf((char*)Msg2TxBuf,"ATE0\r\n");
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);

//    i=sprintf((char*)Msg2TxBuf,"AT+IPR?\r\n");
//    USART2SendString(Msg2TxBuf,i);
//    delay_ms(100);

    i=sprintf((char*)Msg2TxBuf,"ATV1\r\n");
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);

    i=sprintf((char*)Msg2TxBuf, "AT+CTZU=1\r\n");
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);
}

/***********************************************************************/
unsigned char  AT_CGSN(char* sn)
{
    int i, j;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+CGSN\r\n");         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(3000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[29]=='O'&&Msg2RxBuf[30]=='K')//10  24
    {
        // if(Msg2RxBuf[8]==',')
        //   flag = Msg2RxBuf[7]-0x30;
        // else
        //	 flag = (Msg2RxBuf[7]-0x30)*10+(Msg2RxBuf[8]-0x30);
        for(j=0; j<15; j++)
            sn[j] = Msg2RxBuf[10+j];
        return 1;
        //flag = flag*10+(Msg2RxBuf[8+j]-0x30);
    }
      return 0;
}




int AT_CSQ(void)
{
    int i, j,flag=99;
    i=sprintf((char*)Msg2TxBuf,"AT+CSQ\r\n");         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(2000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    i=FindString((char*)Msg2RxBuf,Msg2RxCount,"CSQ: ",5);
//	  if(Msg2RxBuf[Msg2RxLen-5]=='O'&&Msg2RxBuf[Msg2RxLen-4]=='K')
//		{
    if(Msg2RxBuf[i+7]==',')
        flag = (Msg2RxBuf[i+5]-0x30)*10+(Msg2RxBuf[i+6]-0x30);
    else
        flag = Msg2RxBuf[i+5]-0x30;

//		}
    return flag;
}



/***********************************************************************/
void  AT_CIMI(void)
{
    int i, j;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+CIMI\r\n");         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(2000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[29]=='O'&&Msg2RxBuf[30]=='K')
    {
        // if(Msg2RxBuf[8]==',')
        //   flag = Msg2RxBuf[7]-0x30;
        // else
        //	 flag = (Msg2RxBuf[7]-0x30)*10+(Msg2RxBuf[8]-0x30);
        //	 for(j=0;j<15;j++)
        //  flag[j] = Msg2RxBuf[8+j];
        //flag = flag*10+(Msg2RxBuf[8+j]-0x30);
    }
    //  return flag;
}

/***********************************************************************/
void  AT_QICSGP(void)
{
    int i, j;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+QICSGP=1\r\n");
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(2000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[44]=='O'&&Msg2RxBuf[45]=='K')
    {

    }
}

/***********************************************************************/
void  AT_QIACT(void)
{
    int i, j;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+QIACT=1\r\n");         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(2000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[13]=='O'&&Msg2RxBuf[14]=='K')
    {

    }

}

/***********************************************************************/
void  AT_QIOPEN(int port,char* ip)
{
    int i, j;

    static char cnt=0;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n",ip,port);         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(2000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[48]=='O'&&Msg2RxBuf[49]=='K')
    {

        cnt=0;
        return ;

    }
    cnt++;
    if(cnt>5)
    {
        NRB_flag=1;
        cnt=0;
    }

    //  return flag;

}

void AT_QICLOSE(void)
{
    int i, j;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+QICLOSE=0,1\r\n");         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[27]=='O'&&Msg2RxBuf[28]=='K')
    {

    }
}
/***********************************************************************/
void  AT_CLOSE(void)
{
    int i, j;
// long long flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+QICLOSE=0,1\r\n");         //AT+CSQ   强度
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[27]=='O'&&Msg2RxBuf[28]=='K')
    {

    }


    i=sprintf((char*)Msg2TxBuf,"AT+QIDEACT=1\r\n");         //AT+CSQ
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(100);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    if(Msg2RxBuf[27]=='O'&&Msg2RxBuf[28]=='K')
    {

    }
}


//需要调整
int AT_CEREG(void)
{
    int i,j,flag=0;
    i=sprintf((char*)Msg2TxBuf,"AT+CEREG?\r\n");         //AT+CEREG?  驻网状态确认
    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString(Msg2TxBuf,i);
    delay_ms(300);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if(Msg2RxBuf[22]=='1')
        flag=1;
    return flag;
}

//
unsigned char model_gettime(unsigned char isGeted)
{
    int i=0;
    unsigned char r=0;
    while(1!=AT_CEREG())
    {
        if(r>5)
        {
            USART1SendString((unsigned char*)"#restart gprs\r\n",15);
            gprs_dev_restart();
            if(isGeted==0)
                r=0;
            else
                return 1;  //failed
        }

        r++;
        delay_ms(800);
    }
    i = sprintf((char *)Msg2TxBuf, "AT+CCLK?\r\n"); //获取网络时间
    Msg2RxCount = 0;
    for (i = 0; i < 30; i++)
        Msg2RxBuf[i] = 0;
    USART2SendString(Msg2TxBuf, i);
    delay_ms(1200);
    Msg2RxLen = Msg2RxCount + 1;
    if (Msg2RxBuf[44] == 'O' && Msg2RxBuf[45] == 'K' && Msg2RxBuf[2] == '+')
    {
        year=((Msg2RxBuf[19] - '0') * 10 + (Msg2RxBuf[20] - '0') ) ;
        mon=((Msg2RxBuf[22] - '0') * 10 + (Msg2RxBuf[23] - '0') ) % 12;
        day=((Msg2RxBuf[25] - '0') * 10 + (Msg2RxBuf[26] - '0') ) ;
        hour = ((Msg2RxBuf[28] - '0') * 10 + (Msg2RxBuf[29] - '0') ) % 24;
        min = (Msg2RxBuf[31] - '0') * 10 + (Msg2RxBuf[32] - '0');
        second = (Msg2RxBuf[34] - '0') * 10 + (Msg2RxBuf[35] - '0');
        i = sprintf((char *)Msg1TxBuf, "time = %2d:%2d:%2d\r\n", hour, min, second);
        TIMEP=standard_to_stamp( year, mon, day, hour, min, second)/*+ 28800*/ ;
        USART1SendString(Msg1TxBuf, i);
        return 0; //success
    }
      return 1;  //failed
}
//
void connect(char* m_sn,char* ip,int port,char *rssi)
{
    unsigned char t=0;
    while(!AT_CGSN(m_sn))
    {
        t++;
        delay_ms(1000);
        USART1SendString("\r\nSN ERR:",8);
        USART1SendChar(t-0x30);
        if(t>5)
        {
            NRB_flag=1;
        }
        
    }
    AT_CIMI();
    *rssi=AT_CSQ();
    AT_QICSGP();
    AT_QIACT();
    AT_QIOPEN(port,ip);
    AT_QICLOSE();
}



