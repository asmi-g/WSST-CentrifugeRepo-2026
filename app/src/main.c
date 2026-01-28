#include "stm32f4xx_hal.h"

int main(void)
{
    HAL_Init();

    while (1)
    {
        __NOP();
    }
}
