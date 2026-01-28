cmake_minimum_required(VERSION 3.20)
project(stm32-multi-board C ASM)

# Toolchain
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain-arm-none-eabi.cmake)

# Board selection
set(BOARD "F411RE" CACHE STRING "Target board")

# Include board-specific files
include(core/${BOARD}/board.cmake)

# Application sources
file(GLOB APP_SOURCES src/*.c core/App/*.c)

add_executable(${PROJECT_NAME}.elf
    ${APP_SOURCES}
    ${SYSTEM_FILE}
    ${STARTUP_FILE}
    ${HAL_SOURCES}
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE ${HAL_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE ${MCU_DEFINE})
target_compile_options(${PROJECT_NAME}.elf PRIVATE -mcpu=cortex-m4 -mthumb -O0 -g -Wall)
target_link_options(${PROJECT_NAME}.elf PRIVATE -T${LINKER_SCRIPT} -Wl,--gc-sections)

target_compile_options(stm32-multi-board.elf PRIVATE
    ${MCU_FLAGS}
)

target_link_options(stm32-multi-board.elf PRIVATE
    ${MCU_FLAGS}
)


add_custom_target(flash
    COMMAND openocd
        -f interface/stlink.cfg
        -f target/stm32f4x.cfg
        -c "program $<TARGET_FILE:stm32-multi-board.elf> verify reset exit"
    DEPENDS stm32-multi-board.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
