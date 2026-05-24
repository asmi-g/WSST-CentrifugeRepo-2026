#ifndef STEPPER_H
#define STEPPER_H

#include "stm32f4xx_hal.h" // Replace with your specific family header (e.g., stm32f1xx_hal.h)


typedef struct {
    GPIO_TypeDef* stepPort;
    uint16_t stepPin;
} stepper_t;

// Function prototypes
void STEPPER_Init(stepper_t* motor, GPIO_TypeDef* stepPort, uint16_t stepPin);
void STEPPER_Step(stepper_t* motor, uint32_t steps, uint32_t pulse_us, uint32_t delay_us);
void STEPPER_StepThree(stepper_t* m1, stepper_t* m2, stepper_t* m3, uint32_t steps, uint32_t delay_ms);
void STEPPER_StepTwo(stepper_t* m1, stepper_t* m2, uint32_t steps, uint32_t pulse_us, uint32_t delay_us);

// Temporary fix to bypass SysTick conflicts
#define delay_us(us) HAL_Delay(us / 1000 > 0 ? us / 1000 : 1) 

#endif
