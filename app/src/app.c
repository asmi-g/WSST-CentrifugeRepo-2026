#include "stm32f4xx_hal.h"

void App_Main(void)
{
    HAL_Init();

    while (1)
    {
        __NOP();
    }
}
