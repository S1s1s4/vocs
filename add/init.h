#ifndef __init_H
#define __init_H

#ifdef __cplusplus
extern "C" {
#endif
//系统状态指示灯
#define LED1_RUN_PORT      GPIOE
#define LED1_RUN_PIN       GPIO_Pin_2
#define LED1_RUN_GPIO_RCC  RCC_AHB1Periph_GPIOE
#define LED1_RUN_HIGH()    GPIO_ResetBits(LED1_RUN_PORT,LED1_RUN_PIN)
#define LED1_RUN_LOW()     GPIO_SetBits(LED1_RUN_PORT,LED1_RUN_PIN)

//CAN1指示灯
#define LED4_CAN1_PORT      GPIOE
#define LED4_CAN1_PIN       GPIO_Pin_4
#define LED4_CAN1_GPIO_RCC  RCC_AHB1Periph_GPIOE
#define LED4_CAN1_HIGH()    GPIO_ResetBits(LED4_CAN1_PORT,LED4_CAN1_PIN)
#define LED4_CAN1_LOW()     GPIO_SetBits(LED4_CAN1_PORT,LED4_CAN1_PIN)

//CAN2指示灯
#define LED5_CAN2_PORT      GPIOE
#define LED5_CAN2_PIN       GPIO_Pin_5
#define LED5_CAN2_GPIO_RCC  RCC_AHB1Periph_GPIOE
#define LED5_CAN2_HIGH()    GPIO_ResetBits(LED5_CAN2_PORT,LED5_CAN2_PIN)
#define LED5_CAN2_LOW()     GPIO_SetBits(LED5_CAN2_PORT,LED5_CAN2_PIN)

//系统网络指示灯
#define LED6_NET_PORT       GPIOF
#define LED6_NET_PIN        GPIO_Pin_4
#define LED6_NET_RCC        (RCC_AHB1Periph_GPIOF)
#define LED4_NET_HIGH()     GPIO_ResetBits(LED6_NET_PORT,LED6_NET_PIN)
#define LED4_NET_LOW()      GPIO_SetBits(LED6_NET_PORT,LED6_NET_PIN)
void JEC_IO_Init(void);
void JEC_USART_Init(void);
void led_start(void);
void Deinit(void);
/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* __APP_H */
