#include "stm32f4xx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "init.h"
//#include "MQTTPacket.h"
//#include "transport.h"

#include "uart.h"
#include "IWDG.h"
#include "mytime.h"
#include "mymodbus.h"
#include "gprs_dev.h"
#include "ec20.h"
#include <signal.h>
#include "mqtt_ec.h"
#include "stmflash.h"


/* Exported functions ------------------------------------------------------- */
char id[24];
//===========================================================================
/*************************ota adjust parameter*********************************/

unsigned char VER=0; //only change in app
unsigned int FLASH_FLGS=0xffffffff;
unsigned char ver=0xff;     //      remember to write  //only change in app
unsigned char isup=0xff;    // y:55 n:ff  
unsigned char toaddr=0xff;  // 1: ff 2:55   //only change in app  VER%2==0?toaddr=0x55:toaddr=0xff
unsigned char isok=0xff;   //ok:55 err:EE
//===========================================================================
float g_user_voc=0.0;
float g_user_temp=0.0;
float g_user_hum=0.0;
time_t TIMEP;
uint16_t EC = 0;   //1  sensor err 

unsigned char gg=0;
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
void iap_load_app(unsigned int appxaddr);//跳转到APP程序执行
iapfun jump2app;

char LO[11], LA[10];
char DT[5];
char ip1[]="39.104.68.199";
int port1 = 61613;
float BV = 0;
char RSSI;

unsigned short int flag_5_min=300;

uint16_t NRB_flag = 0;

uint16_t  NBsend_flag = 0,  Startsend_flag = 0;
uint16_t year,mon,day,second, min = 1, hour, consttimeflag;
int SenddateLen;
unsigned char Senddate[250];
char subTopic[23];
char ftpIp[] = "61.164.218.155";//jecinfo ftp
uint16_t ftpPort = 1023;
char ftpUser[] = "jecinfo";
char ftpPass[] = "1qaz2wsx,.";
char ftpDirectory[] = "/AQM3.0/vocs";
char ftpFilename[] = "update.jec";
union  MD5 {
    unsigned int u32md5[4];
    unsigned char u8md5[16];
} check;

unsigned int flash_info[5];





/*******************************************
  ### will clear old flag
    FLASH_FLGS update
    flush to flash_info
    save in flash
*******************************************/
void Flush_flags(void)
{
    unsigned char i=0;
    FLASH_FLGS=(ver<<24)|(isup<<16)|(toaddr<<8)|(isok);
    flash_info[0]=FLASH_FLGS;
    memcpy(flash_info+1,check.u32md5,16);
    USART1SendString((unsigned char*)"\r\n write flash:",14);
    i = sprintf((char *)Msg1TxBuf, "%08X,%08X,%08X,%08X\r\n", flash_info[0], flash_info[1], flash_info[2],flash_info[3]);
    USART1SendString(Msg1TxBuf, i);
    STMFLASH_Write(FLAG_ADDR,flash_info,5);
}

void analysis_flash(void)
{
    unsigned char i=0;
    STMFLASH_Read(FLAG_ADDR,&FLASH_FLGS,1);

    ver=VER;
    
    if(VER%2==0)
        toaddr=0x55;
    if(FLASH_FLGS!=0xffffffff)
    {
       // ver=(unsigned char)(FLASH_FLGS>>24);
      //  isup=(unsigned char)(FLASH_FLGS>>16);
        //toaddr=(unsigned char)(FLASH_FLGS>>8);
        isok=(unsigned char)(FLASH_FLGS);
        i= sprintf((char*)Msg1TxBuf, "\r\nflash_flgs:0x%08X",FLASH_FLGS);
        USART1SendString(Msg1TxBuf,i);

        if(isok==0xff)
        {
            USART1SendString("\r\nno update info",14);
        }
        else if(isok==0xee) //更新出错 重新去获取 md5
        {
            USART1SendString("\r\nupdate err",12);
            isok=0xff;
            isup=0xff;
             EC|=0x0010;
        }
        else if(isok==0x55)
        {
            isok=0xff;
            isup=0xff;
            USART1SendString("\r\nupdate success",16);
        }
    }
    Flush_flags();
}

void System_SoftReset(void)
{
    __set_FAULTMASK(1); //关闭所有中断
    NVIC_SystemReset(); //复位
}
#if 0
void iap_load_app(unsigned int appxaddr)
{
    unsigned char i=0;
    if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
    {
        i= sprintf((char*)Msg1TxBuf, "\r\nADDR:0x%08X",appxaddr);
        USART1SendString(Msg1TxBuf,i);
        // USART_DeInit(USART1);
        jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        jump2app();									//跳转到APP.
    }
}
#endif
int main(void)
{

    uint8_t i,j;

    char IMEIdate[15];

    SystemInit();// VECT_TAB_OFFSET
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
    NVIC_SetPriority(SysTick_IRQn, 0x0);
    SysTick_Config(SystemCoreClock / 100);
    //init Systick inrupt，100Hz,10ms

    Deinit();
    init_timer3(249,7199);  //20ms
    JEC_USART_Init();
    JEC_IO_Init();	  //init IO 
    gprs_dev_init();  //ec20 
    gprs_dev_restart(); 
    analysis_flash(); //get some update info.
    led_start(); //led flew
    delay_ms(1000); //enough time wait ec200s on
    connect(IMEIdate,ip1,port1,&RSSI);   //gprs  set context and get rssi imei
    delay_ms(10);
    consttimeflag = model_gettime(0); //get current time
    while (1)
    {
 #if 0   //jump debug flag 
        if(gg==1)
        {
            USART_DeInit(USART2);
            USART_DeInit(USART3);

            TIM_DeInit(TIM3);
            iap_load_app(APP1_ADDR);
        }
        if(gg==2)
        {
            USART_DeInit(USART2);
            USART_DeInit(USART3);
            TIM_DeInit(TIM3);
            USART_DeInit(USART1);
            System_SoftReset();
        }
        #endif
        if ((consttimeflag == 0) &&( flag_5_min>=300)&& (Startsend_flag == 0)) 
        {

            Startsend_flag = 1;
            _time_20s=0;
            flag_5_min=0;
            RSSI=AT_CSQ();
            i = sprintf((char *)Msg1TxBuf, "time = %2d:%2d:%2d\r\n", hour, min, second);
            USART1SendString(Msg1TxBuf, i);
        }
        else if (consttimeflag == 1)
        {
            consttimeflag = 0;
            if(0!=model_gettime(1))
                consttimeflag = 1;
        }
        

        if(Startsend_flag==1)
        {
            LED4_NET_HIGH();
            if(g_user_voc<0.0f)g_user_voc=0.0f;
            if(g_user_temp<-20.0f)g_user_temp=-20.0f;
            if(g_user_temp>99.9f)g_user_temp=99.9f;
            if(g_user_hum<=0.0f)g_user_hum=0.0f;
            if(g_user_hum>99.9f)g_user_hum=99.9f;
            SenddateLen = sprintf((char *)Senddate, 
            "{\"SN\":\"%s\",\"FW\":\"%d\",\"EC\":%d,\"LO\":\"%s\",\"LA\":\"%s\",\"DT\":\"%s\",\"BV\":%.1f,\"RSSI\":\"%d\",\"TIME\":%d,\"TVOC\":%.2f,\"AT\":%.1f,\"AH\":%.1f}", 
            IMEIdate,VER,EC, LO, LA, DT, BV, RSSI, TIMEP, g_user_voc, g_user_temp, g_user_hum);
            
            AT_QIOPEN(port1,ip1);
            mqtt_connect(id,IMEIdate,0,0,0);

            publish("jecinfo/vocs",0,0,1,0,(char*)Senddate,SenddateLen);//AQM2.0/JiaXing
#if 1
            i=sprintf(subTopic, "command/%s", IMEIdate);
            if(subscribe(subTopic,i,0, (char*)Senddate) > 0)
            {
                unsubscribe(subTopic);
                disconnect();
                i=FindString((char*)Senddate,250,"update",6);
                if(i!=0xff)
                {
                    USART_DeInit(USART2);
                    USART_DeInit(USART3);
                    TIM_DeInit(TIM3);
                    
                    memset(check.u8md5,0,16);
                    for(j=0; j<32; j+=2)
                    {
                        if(j%2==0)
                        {
                            check.u8md5[j/2]=asctou8(Senddate+i+j+5+1);
                        }
                    }

                    //write flash flags
                    isup=0x55;
                    Flush_flags();
                    USART_DeInit(USART1);
                    System_SoftReset();

                }
            }
#endif
            disconnect();
            AT_QICLOSE();
            Startsend_flag=0;
            LED4_NET_LOW();
        }



        if(_time_1s>4)  //在解析函数内部清零
        {
            GPIO_ToggleBits(LED1_RUN_PORT,LED1_RUN_PIN);
            USART1SendString("getdata\r\n", 10);
            GetData();
            _time_1s=0;
        }

        if (NRB_flag == 1) //重启模块
        {
            LED4_CAN1_HIGH();
            AT_CLOSE();
            NRB_flag = 0;
            gprs_dev_restart();
            delay_ms(4000);
            delay_ms(4000);
            delay_ms(4000);
            connect(IMEIdate,ip1,port1,&RSSI);   //gprs 第一次配置场景
            LED4_CAN1_LOW();
        }
       // IWDG_Feed();//喂狗
    }
}


