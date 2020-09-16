#include <stdio.h>
#include <string.h>
//#include "FreeRTOS.h"
//#include "Semphr.h"
//#include "queue.h"
//#include "task.h"
#include "stm32f4xx.h"
//#include "sys_types.h"
//#include "debug.h"
#include "bsp_uart.h"




typedef struct
{
    uint32_t          uart_clk;
    USART_TypeDef *   uart;
    uint8_t           IRQChannel;

    uint32_t          tx_port_clk;
    GPIO_TypeDef *    tx_port;
    uint16_t          tx_pin;
    uint16_t          tx_pin_AFSource;
    uint8_t           tx_GPIO_AF;

    uint32_t          rx_port_clk;
    GPIO_TypeDef *    rx_port;
    uint16_t          rx_pin;
    uint16_t          rx_pin_AFSource;
    uint8_t           rx_GPIO_AF;
} bsp_uart_phy_item_t;

static const bsp_uart_phy_item_t bsp_uart_phy_item_table[BSP_UART_NUMBERS]=
{
    {
        RCC_APB2Periph_USART1,
        USART1,
        USART1_IRQn,
        RCC_AHB1Periph_GPIOB,
        GPIOB,
        GPIO_Pin_6,
        GPIO_PinSource6,
        GPIO_AF_USART1,
        RCC_AHB1Periph_GPIOB,
        GPIOB,
        GPIO_Pin_7,
        GPIO_PinSource7,
        GPIO_AF_USART1
    },

    {
        RCC_APB1Periph_USART2,
        USART2,
        USART2_IRQn,

        RCC_AHB1Periph_GPIOA,
        GPIOA,
        GPIO_Pin_2,
        GPIO_PinSource2,
        GPIO_AF_USART2,
        RCC_AHB1Periph_GPIOA,
        GPIOA,
        GPIO_Pin_3,
        GPIO_PinSource3,
        GPIO_AF_USART2
    },

    {
        RCC_APB1Periph_USART3,
        USART3,
        USART3_IRQn,

        RCC_AHB1Periph_GPIOD,
        GPIOD,
        GPIO_Pin_8,
        GPIO_PinSource8,
        GPIO_AF_USART3,
        RCC_AHB1Periph_GPIOD,
        GPIOD,
#if 0
        GPIO_Pin_11,
        GPIO_PinSource11,
#else
        GPIO_Pin_9,
        GPIO_PinSource9,

#endif
        GPIO_AF_USART3
    },

    {
        RCC_APB1Periph_UART4,
        UART4,
        UART4_IRQn,

        RCC_AHB1Periph_GPIOC,
        GPIOC,
        GPIO_Pin_10,
        GPIO_PinSource10,
        GPIO_AF_UART4,
        RCC_AHB1Periph_GPIOC,
        GPIOC,
        GPIO_Pin_11,
        GPIO_PinSource11,
        GPIO_AF_UART4
    },

    {
        RCC_APB1Periph_UART5,
        UART5,
        UART5_IRQn,
        RCC_AHB1Periph_GPIOC,
        GPIOC,
        GPIO_Pin_12,
        GPIO_PinSource12,
        GPIO_AF_UART5,
        RCC_AHB1Periph_GPIOD,
        GPIOD,
        GPIO_Pin_2,
        GPIO_PinSource2,
        GPIO_AF_UART5
    },

    {
        RCC_APB2Periph_USART6,
        USART6,
        USART6_IRQn,

        RCC_AHB1Periph_GPIOC,
        GPIOC,
        GPIO_Pin_6,
        GPIO_PinSource6,
        GPIO_AF_USART6,
        RCC_AHB1Periph_GPIOC,
        GPIOC,
        GPIO_Pin_7,
        GPIO_PinSource7,
        GPIO_AF_USART6
    },
#if 0
    {
        RCC_APB1Periph_UART7,
        UART7,
        UART7_IRQn,

        RCC_AHB1Periph_GPIOE,
        GPIOE,
        GPIO_Pin_8,
        GPIO_PinSource8,
        GPIO_AF_UART7,
        RCC_AHB1Periph_GPIOE,
        GPIOE,
#if 0
        GPIO_Pin_11,
        GPIO_PinSource11,
#else
        GPIO_Pin_7,
        GPIO_PinSource7,

#endif
        GPIO_AF_UART7
    },

    {
        RCC_APB1Periph_UART8,
        UART8,
        UART8_IRQn,

        RCC_AHB1Periph_GPIOE,
        GPIOE,
        GPIO_Pin_1,
        GPIO_PinSource1,
        GPIO_AF_UART8,
        RCC_AHB1Periph_GPIOE,
        GPIOE,
        GPIO_Pin_0,
        GPIO_PinSource0,
        GPIO_AF_UART8
    },
#endif

};







//typedef struct
//{
//  xQueueHandle xQueueRx;
//  xQueueHandle xQueueTx;
//  xQueueHandle tx_mutex;

//}bsp_uart_port_handler_t;

//typedef struct
//{
//  bsp_uart_port_handler_t *port[BSP_UART_NUMBERS];
//}bsp_uart_handler_t;

//static bsp_uart_handler_t* bsp_uart_handler;

/******************************************************************************************************


*******************************************************************************************************/
//unsigned char bsp_uart_init(void)
//{
//  bsp_uart_handler_t* handler;
//  if(bsp_uart_handler != NULL)
//    return 0;
//
//  handler = ( bsp_uart_handler_t * ) pvPortMalloc( sizeof( bsp_uart_handler_t ));
//  if(handler == NULL)
//    return 0;
//  memset(handler,0,sizeof(bsp_uart_handler_t));
//  bsp_uart_handler = handler;
//
//  return 1;
//}

static unsigned char  bsp_uart_pin_enable(const uint8_t  uart_id)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if(uart_id >= BSP_UART_NUMBERS)
        return 0;


    RCC_AHB1PeriphClockCmd(bsp_uart_phy_item_table[uart_id].tx_port_clk|bsp_uart_phy_item_table[uart_id].rx_port_clk, ENABLE);


    GPIO_PinAFConfig(bsp_uart_phy_item_table[uart_id].tx_port, bsp_uart_phy_item_table[uart_id].tx_pin_AFSource, bsp_uart_phy_item_table[uart_id].tx_GPIO_AF);
    GPIO_PinAFConfig(bsp_uart_phy_item_table[uart_id].rx_port, bsp_uart_phy_item_table[uart_id].rx_pin_AFSource, bsp_uart_phy_item_table[uart_id].rx_GPIO_AF);

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = bsp_uart_phy_item_table[uart_id].tx_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(bsp_uart_phy_item_table[uart_id].tx_port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = bsp_uart_phy_item_table[uart_id].rx_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(bsp_uart_phy_item_table[uart_id].rx_port, &GPIO_InitStructure);

    return 1;


}
unsigned char  bsp_uart_open_port(  const uint8_t  uart_id,
                                    const uint32_t baudRate,
                                    const uint16_t wordLen,
                                    const uint16_t stopBits,
                                    const uint16_t parity,
                                    const uint32_t rxBufSize,
                                    const uint32_t txBufSize,
                                    const uint16_t irq_pri)
{


    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    if(uart_id >= BSP_UART_NUMBERS)
        return 0;

//  if(bsp_uart_handler->port[uart_id] != NULL)
//    return 0;

//  bsp_uart_handler->port[uart_id] = ( bsp_uart_port_handler_t * ) pvPortMalloc( sizeof( bsp_uart_port_handler_t ));
//  memset(bsp_uart_handler->port[uart_id],0,sizeof(bsp_uart_port_handler_t));

//  bsp_uart_handler->port[uart_id]->xQueueRx = xQueueCreate(rxBufSize, sizeof(char));
//  if(bsp_uart_handler->port[uart_id]->xQueueRx  == NULL)
//    return sys_FAILED;

//  bsp_uart_handler->port[uart_id]->xQueueTx = xQueueCreate(txBufSize, sizeof(char));
//  if(bsp_uart_handler->port[uart_id]->xQueueTx  == NULL)
//    return sys_FAILED;

//  bsp_uart_handler->port[uart_id]->tx_mutex = xSemaphoreCreateMutex();

    bsp_uart_pin_enable(uart_id);

    if((uart_id == 0)||(uart_id == 5))
        RCC_APB2PeriphClockCmd(bsp_uart_phy_item_table[uart_id].uart_clk, ENABLE);
    else
        RCC_APB1PeriphClockCmd(bsp_uart_phy_item_table[uart_id].uart_clk, ENABLE);

    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = wordLen;
    USART_InitStructure.USART_StopBits = stopBits;
    USART_InitStructure.USART_Parity = parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(bsp_uart_phy_item_table[uart_id].uart, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = bsp_uart_phy_item_table[uart_id].IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (irq_pri&0xf0)>>4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = irq_pri&0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(bsp_uart_phy_item_table[uart_id].uart, USART_IT_RXNE, ENABLE);




    USART_Cmd(bsp_uart_phy_item_table[uart_id].uart, ENABLE);

    return 1;

}


unsigned char bsp_uart_reconfig(const uint8_t  uart_id,
                                const uint32_t baudRate,
                                const uint16_t wordLen,
                                const uint16_t stopBits,
                                const uint16_t parity)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = wordLen;
    USART_InitStructure.USART_StopBits = stopBits;
    USART_InitStructure.USART_Parity = parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(bsp_uart_phy_item_table[uart_id].uart, &USART_InitStructure);

    return 1;

}

unsigned char bsp_uart_close_port(const uint8_t  uart_id)
{
    if(uart_id >= BSP_UART_NUMBERS)
        return 0;

//  if(bsp_uart_handler->port[uart_id] == NULL)
//    return sys_FAILED;

    if(uart_id == 0)
        RCC_APB2PeriphClockCmd(bsp_uart_phy_item_table[uart_id].uart_clk, DISABLE);
    else
        RCC_APB1PeriphClockCmd(bsp_uart_phy_item_table[uart_id].uart_clk, DISABLE);

    USART_ITConfig(bsp_uart_phy_item_table[uart_id].uart, USART_IT_RXNE, DISABLE);
    USART_ITConfig(bsp_uart_phy_item_table[uart_id].uart, USART_IT_TXE, DISABLE);
    USART_Cmd(bsp_uart_phy_item_table[uart_id].uart, DISABLE);

// vPortFree(bsp_uart_handler->port[uart_id]);

    //bsp_uart_handler->port[uart_id] = NULL;

    return 1;

}

unsigned char bsp_uart_ITConfig(const uint8_t  uart_id,uint16_t usart_it,FunctionalState NewState)
{
    if(uart_id >= BSP_UART_NUMBERS)
        return 0;
    USART_ITConfig(bsp_uart_phy_item_table[uart_id].uart, usart_it, NewState);

    return 1;
}



//uint32_t  bsp_uart_write(const uint8_t  uart_id,uint8_t* pData,const uint32_t datalen,TickType_t xTicksToWait )
//{
//  portTickType  startTicks;
//  portTickType  endTicks;
//  portTickType  coutTicks = 0;


//  portTickType  timoutTicks = 0;
//  BaseType_t    sendResult;


//  uint8_t* pos = NULL;

//  pos = pData;
//  if(uart_id >= BSP_UART_NUMBERS)
//    return 0;

//  if(bsp_uart_handler->port[uart_id] == NULL)
//    return 0;

//  timoutTicks = xTicksToWait;
//  startTicks = xTaskGetTickCount();
//  endTicks = startTicks + timoutTicks;
//

//

//  xSemaphoreTake(bsp_uart_handler->port[uart_id]->tx_mutex,portMAX_DELAY);

//  startTicks = xTaskGetTickCount();
//  while(pos < pData+datalen)
//  {
//    sendResult = xQueueSend(bsp_uart_handler->port[uart_id]->xQueueTx,pos,timoutTicks);
//    if(sendResult == pdPASS)
//    {
//      pos++;
//      //判断是否超时
//      if(timoutTicks != portMAX_DELAY)
//      {
//         endTicks = xTaskGetTickCount();
//         if(endTicks >= startTicks)
//            coutTicks = endTicks - startTicks;
//         else
//            coutTicks = portMAX_DELAY -(startTicks - endTicks);
//
//         if(xTicksToWait > coutTicks)
//           timoutTicks = xTicksToWait - coutTicks;
//         else
//           break;
//      }
//    }
//    else
//    {
//      break;
//    }
//  }
//
//  if(pos != pData)
//  {
//    bsp_uart_ITConfig(uart_id,USART_IT_TXE,ENABLE);
//  }
//  xSemaphoreGive(bsp_uart_handler->port[uart_id]->tx_mutex);
//  return (uint32_t)(pos-pData);
//}


//uint32_t bsp_uart_read(const uint8_t  uart_id,uint8_t* pBuf,const uint32_t readLen,TickType_t xTicksToWait)
//{

//  portTickType  startTicks;
//  portTickType  endTicks;
//  portTickType  coutTicks = 0;
//  portTickType  timoutTicks = 0;
//  uint8_t* pos = pBuf;

//  if(pos ==NULL){while(1);}
//  timoutTicks = xTicksToWait;

//  startTicks = xTaskGetTickCount();
//  while(pos < pBuf + readLen)
//  {
//    if(xQueueReceive(bsp_uart_handler->port[uart_id]->xQueueRx, pos, timoutTicks)==pdTRUE)
//    {
//      pos++;
//      if(timoutTicks != portMAX_DELAY)
//      {
//         endTicks = xTaskGetTickCount();
//         if(endTicks >= startTicks)
//            coutTicks = endTicks - startTicks;
//         else
//            coutTicks = portMAX_DELAY -(startTicks - endTicks);

//
//         if(xTicksToWait > coutTicks)
//           timoutTicks = xTicksToWait - coutTicks;
//         else
//           break;
//      }
//    }
//    else
//    {
//      break;
//    }


//  }

//  return (uint32_t)(pos-pBuf);
//}

//void USART1_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//  portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//  char rxData;
//  char txData;
//   /*接收中断,并且接收缓冲不满*/
//  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET )
//  {
//    rxData = USART_ReceiveData(USART1);
//  // SYS_DEBUG("cChar = %x",rxData);
//    xQueueSendToBackFromISR(bsp_uart_handler->port[0]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);
//


//  }
//
//  if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
//  {


//    //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//    if( xQueueReceiveFromISR( bsp_uart_handler->port[0]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//    {
//      /* A character was retrieved from the queue so can be sent to the
//        THR now. */
//          USART_SendData( USART1,txData);
//    }
//    else
//    {
//      USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
//    }
//  }

//  portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//

//}

//void USART2_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//   portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//   char rxData;
//   char txData;
//    /*接收中断,并且接收缓冲不满*/
//   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET )
//   {
//     rxData = USART_ReceiveData(USART2);
//     //GPRS_DEBUG_WITHOUTHEAD("##%x",rxData);
//     xQueueSendToBackFromISR(bsp_uart_handler->port[1]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);

//   }
//
//   if(USART_GetITStatus(USART2,USART_IT_TXE)!=RESET)
//   {
//
//     //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//     if( xQueueReceiveFromISR( bsp_uart_handler->port[1]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//     {
//       /* A character was retrieved from the queue so can be sent to the
//         THR now. */
//           USART_SendData( USART2,txData);
//     }
//     else
//     {
//       USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
//     }
//   }
//
//   portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//


//}

//void USART3_IRQHandler(void)
//{

//portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
// portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
// char rxData;
// char txData;
// //if(bsp_uart_handler->port[2] == NULL)
//  // return;
//  /*接收中断,并且接收缓冲不满*/
// if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET )
// {
//
//// GNSS_DEBUG("USART_IT_RXNE\r\n");

//   rxData = USART_ReceiveData(USART3);
//  // GNSS_DEBUG("RX=%d,xQueueRx=%x\r\n",rxData,bsp_uart_handler->port[2]->xQueueRx);
//

//   xQueueSendToBackFromISR(bsp_uart_handler->port[2]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);
//   //  GNSS_DEBUG("RX over\r\n");


// }
//
// if(USART_GetITStatus(USART3,USART_IT_TXE)!=RESET)
// {
//   //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//   if( xQueueReceiveFromISR( bsp_uart_handler->port[2]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//   {
//     /* A character was retrieved from the queue so can be sent to the
//       THR now. */
//         USART_SendData( USART3,txData);
//   }
//   else
//   {
//     USART_ITConfig( USART3, USART_IT_TXE, DISABLE );
//   }
// }

// portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//

//}

//void UART4_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//   portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//   char rxData;
//   char txData;
//    /*接收中断,并且接收缓冲不满*/
//   if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET )
//   {
//     rxData = USART_ReceiveData(UART4);
//     xQueueSendToBackFromISR(bsp_uart_handler->port[3]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);

//   }
//
//   if(USART_GetITStatus(UART4,USART_IT_TXE)!=RESET)
//   {
//     //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//     if( xQueueReceiveFromISR( bsp_uart_handler->port[3]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//     {
//       /* A character was retrieved from the queue so can be sent to the
//         THR now. */
//           USART_SendData( UART4,txData);
//     }
//     else
//     {
//       USART_ITConfig( UART4, USART_IT_TXE, DISABLE );
//     }
//   }
//
//   portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//

//}

//void UART5_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//   portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//   char rxData;
//   char txData;
//    /*接收中断,并且接收缓冲不满*/
//   if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET )
//   {
//     rxData = USART_ReceiveData(UART5);
//     xQueueSendToBackFromISR(bsp_uart_handler->port[4]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);
//
//   }
//
//   if(USART_GetITStatus(UART5,USART_IT_TXE)!=RESET)
//   {
//     //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//     if( xQueueReceiveFromISR( bsp_uart_handler->port[4]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//     {
//       /* A character was retrieved from the queue so can be sent to the
//         THR now. */
//           USART_SendData( UART5,txData);
//     }
//     else
//     {
//       USART_ITConfig( UART5, USART_IT_TXE, DISABLE );
//     }
//   }
//
//   portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//

//}

//void USART6_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//   portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//   char rxData;
//   char txData;
//    /*接收中断,并且接收缓冲不满*/
//   if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET )
//   {
//     rxData = USART_ReceiveData(USART6);
//     xQueueSendToBackFromISR(bsp_uart_handler->port[5]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);
//
//   }
//
//   if(USART_GetITStatus(USART6,USART_IT_TXE)!=RESET)
//   {
//     //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//     if( xQueueReceiveFromISR( bsp_uart_handler->port[5]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//     {
//       /* A character was retrieved from the queue so can be sent to the
//         THR now. */
//           USART_SendData( USART6,txData);
//     }
//     else
//     {
//       USART_ITConfig( USART6, USART_IT_TXE, DISABLE );
//     }
//   }
//
//   portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//

//}
//#if 0
//void UART7_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//   portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//   char rxData;
//   char txData;
//    /*接收中断,并且接收缓冲不满*/
//   if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET )
//   {
//     rxData = USART_ReceiveData(UART7);
//     xQueueSendToBackFromISR(bsp_uart_handler->port[6]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);
//
//   }
//
//   if(USART_GetITStatus(UART7,USART_IT_TXE)!=RESET)
//   {
//     //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//     if( xQueueReceiveFromISR( bsp_uart_handler->port[6]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//     {
//       /* A character was retrieved from the queue so can be sent to the
//         THR now. */
//           USART_SendData( UART7,txData);
//     }
//     else
//     {
//       USART_ITConfig( UART7, USART_IT_TXE, DISABLE );
//     }
//   }
//
//   portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
//
//}

//void UART8_IRQHandler(void)
//{
//  portBASE_TYPE xRxHigherPriorityTaskWoken = pdFALSE;
//   portBASE_TYPE xTxHigherPriorityTaskWoken = pdFALSE;
//   char rxData;
//   char txData;
//    /*接收中断,并且接收缓冲不满*/
//   if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET )
//   {
//     rxData = USART_ReceiveData(UART8);
//     xQueueSendToBackFromISR(bsp_uart_handler->port[7]->xQueueRx, &rxData, &xRxHigherPriorityTaskWoken);
//
//   }
//
//   if(USART_GetITStatus(UART8,USART_IT_TXE)!=RESET)
//   {
//     //USART_ClearITPendingBit(USART1,USART_IT_TXE);
//     if( xQueueReceiveFromISR( bsp_uart_handler->port[7]->xQueueTx, &txData, &xTxHigherPriorityTaskWoken ) == pdTRUE )
//     {
//       /* A character was retrieved from the queue so can be sent to the
//         THR now. */
//           USART_SendData( UART8,txData);
//     }
//     else
//     {
//       USART_ITConfig( UART8, USART_IT_TXE, DISABLE );
//     }
//   }
//   portEND_SWITCHING_ISR( xRxHigherPriorityTaskWoken||xTxHigherPriorityTaskWoken );
// }
//#endif






