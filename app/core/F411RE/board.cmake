set(MCU_DEFINE STM32F411xE)

set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/startup_stm32f411xe.s)
set(SYSTEM_FILE  ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Core/Src/system_stm32f4xx.c)
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32F411XX_FLASH.ld)

# Include HAL + CMSIS + board-specific HAL config
set(HAL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Core/Inc
    ${CMAKE_SOURCE_DIR}/hal/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/hal/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/hal/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)

file(GLOB HAL_SOURCES
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Middlewares/Third_Party/FreeRTOS/Source/*.c
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}/Core/Src/*.c
    ${CMAKE_SOURCE_DIR}/hal/Drivers/STM32F4xx_HAL_Driver/Src/*.c
)

list(FILTER HAL_SOURCES EXCLUDE REGEX ".*_template.c$")

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

