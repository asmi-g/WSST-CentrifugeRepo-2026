# Motor Interface Documentation

Purpose: The PWM interface provides a modular way to control PWM capable STM32 timer channels for motor and actuator prototyping. It is intended to be reusable across different timer instances and channels without changing the driver implementation in pwm.c or pwm.h.

## Public API

HAL_StatusTypeDef pwm_init(pwm_t *p, TIM_HandleTypeDef *htim, uint32_t channel);  
HAL_StatusTypeDef pwm_start(pwm_t *p);  
HAL_StatusTypeDef pwm_set(pwm_t *p, uint8_t duty_u8);

## CubeMX Setup

Configure the desired PWM output pin in the `.ioc` file by assigning it to the required timer channel using PWM Generation CHx. Set timer parameters as needed for the application.

Prototype timer settings:
* Prescaler = 15  
* Period = 999  

After changing the `.ioc`:
* save the file  
* regenerate code  
* rebuild the project  

If the timer instance or channel changes, update the `pwm_init()` call in application code. No changes are required in pwm.c or pwm.h.

## Main.c

Include `pwm.h`, create a PWM object, initialize it with the required timer and channel, start PWM output, then set duty cycle.

Basic flow:

`pwm_t pwm1;`  
`pwm_init(&pwm1, &htim4, TIM_CHANNEL_1);`  
`pwm_start(&pwm1);`  
`pwm_set(&pwm1, 128);`

The same interface applies whether using:
* multiple channels on one timer  
* channels across different timers  

## Duty Cycle

The PWM interface uses an 8 bit duty input from 0 to 255.

* 0 = off  
* 128 ≈ 50 percent duty  
* 255 = maximum duty  

The driver automatically maps the 0 to 255 input to the timer compare register using the configured timer period, so duty cycle changes only require changing the value passed into `pwm_set()`.

## Prototyping Note

For motor or actuator testing, connect the STM32 PWM output to a motor driver input and share ground between the STM32 and driver.

Typical signal path:

STM32 PWM pin -> motor driver input  
STM32 GND -> shared ground  

An LED may also be used temporarily for PWM validation before connecting a motor driver.

## Pinouts
Motor Driver:
VCC to power supply (lab supply acceptable, 12V, 1A)
GND to power supply (lab supply acceptable, 12V, 1A)
PUL+ to STM32 PB8
PUL- to STM32 GND
DIR+ to STM32 VCC
DIR- to STM32 GND
ENA+ left floating
ENA- left floating
A+ to Motor
A- to Motor
B+ to Motor
B- to Motor

Motor: Directly to motor driver
A+ to Motor Driver
A- to Motor Driver
B+ to Motor Driver
B- to Motor Driver

Motor Encoder:
VCC to STM32 3.3V
GND to STM32 GND
EA+ to STM32 PC6 
EA- to STM32 GND
EB+ to STM32 PC7
EB- to STM32 GND
EZ+ left floating
EZ- left floating