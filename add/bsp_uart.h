
#ifndef  _BSP_UART_H_
#define   _BSP_UART_H_

//#include "sys_types.h"
#include "stm32f4xx.h"
//#include "FreeRTOS.h"

#define BSP_UART_NUMBERS    (6)



extern unsigned char bsp_uart_init(void);

extern unsigned char  bsp_uart_open_port(  const uint8_t  uart_id,
        const uint32_t baudRate,
        const uint16_t wordLen,
        const uint16_t stopBits,
        const uint16_t parity,
        const uint32_t rxBufSize,
        const uint32_t txBufSize,
        const uint16_t irq_pri);
extern unsigned char bsp_uart_reconfig(const uint8_t  uart_id,
                                       const uint32_t baudRate,
                                       const uint16_t wordLen,
                                       const uint16_t stopBits,
                                       const uint16_t parity);
extern unsigned char bsp_uart_close_port(const uint8_t  uart_id);
//extern unsigned int  bsp_uart_write(const uint8_t  uart_id, uint8_t* pData,const uint32_t datalen,TickType_t xTicksToWait);
//extern uint32_t bsp_uart_read(const uint8_t  uart_id, uint8_t* pBuf,const uint32_t readLen,TickType_t xTicksToWait);



#endif




