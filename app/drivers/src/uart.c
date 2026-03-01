#include "uart.h"
#include "stm32f4xx_hal.h"

static UART_HandleTypeDef *uart_handle;
static uint8_t rx_byte;

void uart_init(UART_HandleTypeDef *huart)
{
    uart_handle = huart;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
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