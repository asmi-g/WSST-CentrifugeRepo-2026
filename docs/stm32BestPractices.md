# STM32 Development
User-implemented driver code is in: ```app/drivers/*```

## UART Driver
## Prerequisities
#include "uart.h"

## Transmission
To transmit via UART, you have 2 options:
1. Redirect via __io_putchar, by calling: 
    ```printf();```
2. Call HAL UART API functions, by using: 
    ```HAL_UART_Transmit(&huart2, Payload, strlen((char*)Payload), HAL_MAX_DELAY);```

## Reception
To recieve via UART, you can call: