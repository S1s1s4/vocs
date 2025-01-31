
#include "stm32f4xx.h"
#include "gprs_dev.h"
#include "mytime.h"







#define GPRS_DEV_PWREN_PORTCLK         RCC_AHB1Periph_GPIOG
#define GPRS_DEV_PWREN_PORT            GPIOG
#define GPRS_DEV_PWREN_PIN             GPIO_Pin_3

#define GPRS_DEV_PWRKEY_PORTCLK         RCC_AHB1Periph_GPIOG
#define GPRS_DEV_PWRKEY_PORT            GPIOG
#define GPRS_DEV_PWRKEY_PIN             GPIO_Pin_14

#define GPRS_DEV_RES_PORTCLK         RCC_AHB1Periph_GPIOG
#define GPRS_DEV_RES_PORT            GPIOG
#define GPRS_DEV_RES_PIN             GPIO_Pin_12


#define GPRS_DEV_DTR_PORTCLK         RCC_AHB1Periph_GPIOD
#define GPRS_DEV_DTR_PORT            GPIOD
#define GPRS_DEV_DTR_PIN             GPIO_Pin_11





#define GPRS_DEV_PWR_ON()                GPIO_SetBits(GPRS_DEV_PWREN_PORT,GPRS_DEV_PWREN_PIN)
#define GPRS_DEV_PWR_OFF()               GPIO_ResetBits(GPRS_DEV_PWREN_PORT,GPRS_DEV_PWREN_PIN)

#define GPRS_DEV_PWRKEY_HIGH()           GPIO_ResetBits(GPRS_DEV_PWRKEY_PORT,GPRS_DEV_PWRKEY_PIN)
#define GPRS_DEV_PWRKEY_LOW()            GPIO_SetBits(GPRS_DEV_PWRKEY_PORT,GPRS_DEV_PWRKEY_PIN)


#define GPRS_DEV_RES_LOW()               GPIO_SetBits(GPRS_DEV_RES_PORT,GPRS_DEV_RES_PIN)
#define GPRS_DEV_RES_HIGH()              GPIO_ResetBits(GPRS_DEV_RES_PORT,GPRS_DEV_RES_PIN)


#define GPRS_DEV_DTR_LOW()               GPIO_ResetBits(GPRS_DEV_DTR_PORT,GPRS_DEV_DTR_PIN)
#define GPRS_DEV_DTR_HIGH()              GPIO_SetBits(GPRS_DEV_DTR_PORT,GPRS_DEV_DTR_PIN)

void gprs_dev_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(GPRS_DEV_PWREN_PORTCLK|GPRS_DEV_PWRKEY_PORTCLK, ENABLE);

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPRS_DEV_PWREN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPRS_DEV_PWREN_PORT, &GPIO_InitStructure);
    GPRS_DEV_PWR_OFF();

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPRS_DEV_PWRKEY_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPRS_DEV_PWRKEY_PORT, &GPIO_InitStructure);
    GPRS_DEV_PWRKEY_HIGH();

#ifdef GPRS_DEV_RES_PORTCLK
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPRS_DEV_RES_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPRS_DEV_RES_PORT, &GPIO_InitStructure);
    GPRS_DEV_RES_HIGH();

#endif

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPRS_DEV_DTR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPRS_DEV_DTR_PORT, &GPIO_InitStructure);
    GPRS_DEV_DTR_LOW();


}

void gprs_dev_pwrup(void)
{

#if (GPRS_MODULE_TYPE == GPRS_MODULE_SIM7600CE)
    GPRS_DEV_RES_HIGH();
    delay_ms(100);
    GPRS_DEV_PWR_ON();
//vTaskDelay( pdMS_TO_TICKS(100) );
    delay_ms(100);
#else
    GPRS_DEV_PWR_OFF();
    GPRS_DEV_PWRKEY_HIGH();
    vTaskDelay( pdMS_TO_TICKS(6000) );
    GPRS_DEV_PWR_ON();
    vTaskDelay( pdMS_TO_TICKS(200) );
    GPRS_DEV_PWRKEY_LOW();
    vTaskDelay( pdMS_TO_TICKS(1500) );
    GPRS_DEV_PWRKEY_HIGH();
#endif
}


void gprs_dev_restart(void)
{

#if 1
//#if (GPRS_MODULE_TYPE == GPRS_MODULE_SIM7600CE)
    GPRS_DEV_PWR_OFF();
    GPRS_DEV_PWRKEY_HIGH();
    //vTaskDelay( pdMS_TO_TICKS(1000) );
    delay_ms(1000);
    GPRS_DEV_RES_LOW();
// vTaskDelay( pdMS_TO_TICKS(20000) );
    delay_ms(2000);
    GPRS_DEV_RES_HIGH();

    delay_ms(500);
    //vTaskDelay( pdMS_TO_TICKS(500) );
    GPRS_DEV_PWR_ON();
    delay_ms(500);
// vTaskDelay( pdMS_TO_TICKS(500) );

    GPRS_DEV_PWRKEY_LOW();
    delay_ms(1000);
    // vTaskDelay( pdMS_TO_TICKS(1000) );
    GPRS_DEV_PWRKEY_HIGH();
    delay_ms(3000);
    delay_ms(3000);
//  vTaskDelay( pdMS_TO_TICKS(3000) );


//#endif

#endif

}
void gprs_dev_pwrdown(void)
{

    GPRS_DEV_PWR_OFF();
}



void gprs_dev_gotosleep(void) {

    GPRS_DEV_DTR_HIGH();
}


void gprs_dev_wakeup(void) {

    GPRS_DEV_DTR_LOW();
}


