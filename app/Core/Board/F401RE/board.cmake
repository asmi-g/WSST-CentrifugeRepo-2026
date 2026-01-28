set(MCU_DEFINE STM32F401xE)

set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/startup_stm32f401xe.s)
set(SYSTEM_FILE  ${CMAKE_CURRENT_LIST_DIR}/system_stm32f4xx.c)
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32F401XX_FLASH.ld)
# Include HAL + CMSIS + board-specific HAL config
set(HAL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/Boards/F401RE/Core/Inc
    ${CMAKE_SOURCE_DIR}/hal/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/hal/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/hal/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)

file(GLOB HAL_SOURCES
    ${CMAKE_SOURCE_DIR}/hal/Drivers/STM32F4xx_HAL_Driver/Src/*.c
)

list(FILTER HAL_SOURCES EXCLUDE REGEX ".*_template.c$")

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

