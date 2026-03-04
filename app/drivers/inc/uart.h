#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <string.h>

extern osMessageQueueId_t uartRxMessageQueueHandle;
extern osMutexId_t uartMutexHandle;

typedef struct {
    char command[16];
} uart_message_t;

extern uint8_t rx_buffer[100];
extern volatile uint8_t rx_indx;
extern volatile uint8_t transfer_cplt;

extern uint8_t rx_buffer[100];
extern volatile uint8_t rx_indx;
extern volatile uint8_t transfer_cplt;

void uart_init(UART_HandleTypeDef *huart);

void uart_tx(const char *str);

int __io_putchar(int ch);

#endif /* UART_H */