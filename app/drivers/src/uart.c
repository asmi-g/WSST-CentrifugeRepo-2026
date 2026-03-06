#include "uart.h"
#include "stm32f4xx_hal.h"

static UART_HandleTypeDef *uart_handle;
static uint8_t rx_byte;
uint8_t rx_buffer[100];           // actual memory allocated
volatile uint8_t rx_indx = 0;
volatile uint8_t transfer_cplt = 0;

void uart_init(UART_HandleTypeDef *huart)
{
    uart_handle = huart;
    HAL_UART_Receive_IT(uart_handle, &rx_byte, 1);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == uart_handle)
  {
    if (rx_byte != '\r')
    {
      if (rx_indx < sizeof(rx_buffer) - 1)
      {
          rx_buffer[rx_indx++] = rx_byte;
      }
    }
    else
    {
      rx_buffer[rx_indx] = '\0';  // terminate string
      rx_indx = 0;
      transfer_cplt = 1;          // signal task
    }
    // Echo back the received byte
    HAL_UART_Transmit_IT(uart_handle, &rx_byte, 1);

    HAL_UART_Receive_IT(uart_handle, &rx_byte, 1);
  }

}

/**
  * @brief  Retargets the C library printf function to the USART.
  *   None
  * @retval None
  */
int __io_putchar(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  if (uart_handle == NULL)
    return ch;

  HAL_UART_Transmit(uart_handle, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;

}