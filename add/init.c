#include "stm32f4xx.h"
#include <stdio.h>
#include "init.h"
#include "mytime.h"
#include "bsp_uart.h"
void display_ledinit(void) {

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED1_RUN_GPIO_RCC|LED4_CAN1_GPIO_RCC|LED5_CAN2_GPIO_RCC|LED6_NET_RCC, ENABLE);//Ê¹ÄÜGPIOFÊ±ÖÓ

    GPIO_InitStructure.GPIO_Pin = LED1_RUN_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED1_RUN_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED4_CAN1_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED4_CAN1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED5_CAN2_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED5_CAN2_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = LED6_NET_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED6_NET_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(LED1_RUN_PORT,LED1_RUN_PIN );
    GPIO_SetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_SetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_SetBits(LED6_NET_PORT,LED6_NET_PIN );
}

void led_start(void)
{
    GPIO_ResetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_ResetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_ResetBits(LED6_NET_PORT,LED6_NET_PIN );
    delay_ms(1000);
    GPIO_SetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_SetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_SetBits(LED6_NET_PORT,LED6_NET_PIN );
    delay_ms(1000);
    GPIO_ResetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_ResetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_ResetBits(LED6_NET_PORT,LED6_NET_PIN );
    delay_ms(1000);
    GPIO_SetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_SetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_SetBits(LED6_NET_PORT,LED6_NET_PIN );
    delay_ms(1000);
    GPIO_ResetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_ResetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_ResetBits(LED6_NET_PORT,LED6_NET_PIN );
    delay_ms(1000);
    GPIO_SetBits(LED4_CAN1_PORT,LED4_CAN1_PIN );
    GPIO_SetBits(LED5_CAN2_PORT,LED5_CAN2_PIN );
    GPIO_SetBits(LED6_NET_PORT,LED6_NET_PIN );

}


void Deinit(void)
{
    USART_DeInit(USART2);
    USART_DeInit(USART3);
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    USART_DeInit(USART1);
}
void JEC_IO_Init(void)
{
    display_ledinit();
}

void JEC_USART_Init(void)
{
    bsp_uart_open_port( 1,
                        115200,
                        USART_WordLength_8b,
                        USART_StopBits_1,
                        USART_Parity_No,
                        0,
                        0,
                        3);
    bsp_uart_open_port( 0,
                        115200,
                        USART_WordLength_8b,
                        USART_StopBits_1,
                        USART_Parity_No,
                        0,
                        0,
                        4);

    bsp_uart_open_port( 2,
                        9600,
                        USART_WordLength_8b,
                        USART_StopBits_1,
                        USART_Parity_No,
                        0,
                        0,
                        2);


}
