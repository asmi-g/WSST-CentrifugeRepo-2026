#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"

extern uint8_t rx_buffer[100];
extern volatile uint8_t rx_indx;
extern volatile uint8_t transfer_cplt;

void uart_init(UART_HandleTypeDef *huart);

void uart_start_rx_it(void);

int __io_putchar(int ch);

#endif /* UART_H */