// Servo source code (including: servo.h, servo_cfg.h, servo_cfg.c and servo.c) is adapted from the following tutorial:
// - Source Code: https://github.com/Khaled-Magdy-DeepBlue/STM32_Course_DeepBlue/tree/master/ECUAL/SERVO
// - Tutorial: https://deepbluembedded.com/stm32-servo-motor-control-with-pwm-servo-library-examples-code/
/*
 * File: SERVO.h
 * Driver Name: [[ SERVO Motor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.0
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

//Connections needed: 5V, GND, PB6

#ifndef SERVO_H_
#define SERVO_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f4xx_hal.h"

// The Number OF Servo Motors To Be Used In The Project
#define SERVO_NUM  1

typedef struct
{
	GPIO_TypeDef * SERVO_GPIO;
	uint16_t       SERVO_PIN;
	TIM_TypeDef*   TIM_Instance;
	volatile uint32_t*      TIM_CCRx;
	uint32_t       PWM_TIM_CH;
	uint32_t       TIM_CLK;
	float          MinPulse;
	float          MaxPulse;
}SERVO_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void SERVO_Init(uint16_t au16_SERVO_Instance);
void SERVO_MoveTo(uint16_t au16_SERVO_Instance, float af_Angle);
void SERVO_RawMove(uint16_t au16_SERVO_Instance, uint16_t au16_Pulse);
uint16_t SERVO_Get_MaxPulse(uint16_t au16_SERVO_Instance);
uint16_t SERVO_Get_MinPulse(uint16_t au16_SERVO_Instance);
void SERVO_Sweep(uint16_t au16_SERVO_Instance);



// Custom delay function using SysTick, adapted from:
// - Source: https://github.com/Khaled-Magdy-DeepBlue/STM32_Course_DeepBlue/tree/master/util 
// - Tutorial: https://deepbluembedded.com/stm32-systick-timer-microseconds-delay-us-delay-function/
#define SYSTICK_LOAD (SystemCoreClock/1000000U)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

#define delay_us(us) \
    do { \
         uint32_t start = SysTick->VAL; \
         uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;  \
         while((start - SysTick->VAL) < ticks); \
    } while (0)


#endif /* SERVO_H_ */