################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/encoder.c \
../Core/Src/freertos.c \
../Core/Src/hall_sensor.c \
../Core/Src/main.c \
../Core/Src/mcp9808.c \
../Core/Src/pwm.c \
../Core/Src/servo.c \
../Core/Src/servo_cfg.c \
../Core/Src/stepper.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/thermocouple.c \
../Core/Src/tip_cleaner.c \
../Core/Src/uart.c \
../Core/Src/wire_feeder.c 

OBJS += \
./Core/Src/encoder.o \
./Core/Src/freertos.o \
./Core/Src/hall_sensor.o \
./Core/Src/main.o \
./Core/Src/mcp9808.o \
./Core/Src/pwm.o \
./Core/Src/servo.o \
./Core/Src/servo_cfg.o \
./Core/Src/stepper.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/thermocouple.o \
./Core/Src/tip_cleaner.o \
./Core/Src/uart.o \
./Core/Src/wire_feeder.o 

C_DEPS += \
./Core/Src/encoder.d \
./Core/Src/freertos.d \
./Core/Src/hall_sensor.d \
./Core/Src/main.d \
./Core/Src/mcp9808.d \
./Core/Src/pwm.d \
./Core/Src/servo.d \
./Core/Src/servo_cfg.d \
./Core/Src/stepper.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/thermocouple.d \
./Core/Src/tip_cleaner.d \
./Core/Src/uart.d \
./Core/Src/wire_feeder.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/encoder.cyclo ./Core/Src/encoder.d ./Core/Src/encoder.o ./Core/Src/encoder.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/hall_sensor.cyclo ./Core/Src/hall_sensor.d ./Core/Src/hall_sensor.o ./Core/Src/hall_sensor.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mcp9808.cyclo ./Core/Src/mcp9808.d ./Core/Src/mcp9808.o ./Core/Src/mcp9808.su ./Core/Src/pwm.cyclo ./Core/Src/pwm.d ./Core/Src/pwm.o ./Core/Src/pwm.su ./Core/Src/servo.cyclo ./Core/Src/servo.d ./Core/Src/servo.o ./Core/Src/servo.su ./Core/Src/servo_cfg.cyclo ./Core/Src/servo_cfg.d ./Core/Src/servo_cfg.o ./Core/Src/servo_cfg.su ./Core/Src/stepper.cyclo ./Core/Src/stepper.d ./Core/Src/stepper.o ./Core/Src/stepper.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/thermocouple.cyclo ./Core/Src/thermocouple.d ./Core/Src/thermocouple.o ./Core/Src/thermocouple.su ./Core/Src/tip_cleaner.cyclo ./Core/Src/tip_cleaner.d ./Core/Src/tip_cleaner.o ./Core/Src/tip_cleaner.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su ./Core/Src/wire_feeder.cyclo ./Core/Src/wire_feeder.d ./Core/Src/wire_feeder.o ./Core/Src/wire_feeder.su

.PHONY: clean-Core-2f-Src

