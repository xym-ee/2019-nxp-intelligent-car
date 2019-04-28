#ifndef _UART_H
#define _UART_H

#include "system.h"

void LPUART1_Init(uint32_t bound);
extern void LQ_UART_PutChar(LPUART_Type *base, uint8_t data);

#endif