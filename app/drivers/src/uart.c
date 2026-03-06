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
    static uint8_t rx_indx = 0;
    static uint8_t rx_buffer_local[16];  // short buffer for one command
    uart_message_t msg;

    // Filter end of line
    if (rx_byte != '\r' && rx_byte != '\n')
    {
      if (rx_indx < sizeof(rx_buffer_local) - 1)
        rx_buffer_local[rx_indx++] = rx_byte;
    }
    else
    {
      if (rx_indx > 0)
      {
        rx_buffer_local[rx_indx] = '\0'; // Null terminate
        // Copy safely
        strncpy(msg.command, (char*)rx_buffer_local, sizeof(msg.command));
        msg.command[sizeof(msg.command)-1] = '\0';
        osMessageQueuePut(uartRxMessageQueueHandle, &msg, 0, 0);

        // Echo back complete line
        //HAL_UART_Transmit_IT(uart_handle, rx_buffer_local, rx_indx);

        rx_indx = 0; // Reset buffer
      }
      // Else: just ignore stray '\r' or '\n'
    }
    // Restart reception
    HAL_UART_Receive_IT(uart_handle, &rx_byte, 1);
  
  }

}

void uart_tx(const char *str){
  osMutexAcquire(uartMutexHandle, osWaitForever);
  HAL_UART_Transmit(uart_handle, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
  osMutexRelease(uartMutexHandle);
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