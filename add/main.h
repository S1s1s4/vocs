/*
 * @Author: your name
 * @Date: 2020-05-15 10:14:07
 * @LastEditTime: 2020-05-19 10:51:02
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \presse:\SJJ\jec\sw20190225\main.h
 */
#ifndef __main_H
#define __main_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

//IO�����궨��
#define LED1ON GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define LED2ON GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define LED3ON GPIO_ResetBits(GPIOC, GPIO_Pin_8)

#define LED1OFF GPIO_SetBits(GPIOC, GPIO_Pin_6)
#define LED2OFF GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define LED3OFF GPIO_SetBits(GPIOC, GPIO_Pin_8)

#define MAX485T GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define MAX485R GPIO_ResetBits(GPIOB, GPIO_Pin_12)



//
//unsigned char NBRun,NBConnected;
//unsigned char LTERun,LTEConnected;





#ifdef __cplusplus
}
#endif

#endif /* __APP_H */
