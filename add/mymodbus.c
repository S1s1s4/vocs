#include "mymodbus.h"
#include "uart.h"

#include "mytime.h"

extern float g_user_voc;
extern float g_user_temp;
extern float g_user_hum;
extern unsigned short int EC;
static unsigned char query_times=0;
static float wd[21];
static uint8_t cnt=0;

//static uint8_t flags[6];  //取数据 成功与次数标志
/**
 * @description:
 * @param {type}
 * @return:
 */
void date_query(void)
{
    //char sendbuf[8] = {0x11, 0x03, 0x00, 0x02, 0x00, 0x04, 0x5A, 0xA5};
    char sendbuf[9] = {0xff, 0x01, /*0x78*/0x87, /*0x40*/0x00, 0x00,0x00,0x00,0x00,0x78};
    // sendbuf[0] = 0x11 + num;
    USART3SendString(sendbuf, 9);
    query_times++;
}


uint8_t Cal_Checksum_u8(uint8_t *buf, uint16_t size)
{
    uint16_t cksum = 0;
    uint8_t *p = buf+1;
    uint16_t i = size;
    for (; i > 0; i--)
    {
        cksum += *p++;
    }
    cksum = cksum&0x00FF;
    return (~cksum)+1;
}
/**
 * @description:
 * @param {type} buf ???????? result?????? type ????
 * @return:
 */
void date_preocess(unsigned char *buf, unsigned char offset)
{

    wd[cnt] = (float)((buf[offset + 6] << 8) | buf[offset + 7]);
    g_user_temp=(float)((buf[offset + 8] << 8) | buf[offset + 9])/100.0f;
    g_user_hum=(float)((buf[offset + 10] << 8) | buf[offset + 11])/100.0f;
    wd[cnt]*=0.05f;
    g_user_voc=g_user_voc+wd[cnt]-wd[20]	; //减少计算量
    _time_1s=0;
    __time_20ms=0;
    if(cnt+1>=20)
    {
        wd[20]=wd[0];  //即将删去的点
    }
    else
        wd[20]=wd[cnt+1];
    if(cnt<19)
        cnt++;
    else
    {
        cnt=0;
    }

}

void DataHand(void)
{

    unsigned char moveIndex=0;
    unsigned char len=13;
    uint8_t cksum = 0;
    if(bsp_Usart3_Rec_New())
    {
        while((Msg3RxCount>=9)&&(moveIndex<=(Msg3RxCount-9)))
        {
            if( Msg3RxBuf[moveIndex]!=0xff)  //
            {
                moveIndex++;
                continue ;
            }
            cksum = Cal_Checksum_u8(Msg3RxBuf + moveIndex, 11);
            if(cksum == Msg3RxBuf[moveIndex + 12])//判断校验码
            {
                date_preocess(Msg3RxBuf,moveIndex);
                query_times=0;
                EC=0;
                moveIndex=moveIndex+len;
            }
            else
            {

                moveIndex++; //因为是判断数据头，游标向着数组最尾端的方向移动
            }
        }
        Msg3RxCount=0;  //清空缓冲的下标，方便下次重新从0下标开始接
    }
}



void GetData(void)
{
    date_query();
    delay_ms(300);
    DataHand();  //如有成功解析 直接置为1
    if(query_times>=15)
    {
        USART1SendString("\r\n###senor no ack",17);
        g_user_hum=0.0f;
        g_user_temp=0.0f;
        g_user_voc=0.0f;
        EC|=0x0001;
        
    }
}
