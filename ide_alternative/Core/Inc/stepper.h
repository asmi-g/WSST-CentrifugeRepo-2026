#ifndef STEPPER_H
#define STEPPER_H

#include "stm32f4xx_hal.h" 
#include <stdint.h>


typedef struct {
    GPIO_TypeDef* stepPort;
    uint16_t      stepPin;
    GPIO_TypeDef* dirPort;   
    uint16_t      dirPin;   
} stepper_t;

// Function prototypes
void STEPPER_Init      (stepper_t* motor, GPIO_TypeDef* stepPort, uint16_t stepPin, GPIO_TypeDef* dirPort,  uint16_t dirPin);
void STEPPER_SetDir    (stepper_t* motor, uint8_t forward);


void STEPPER_Step(stepper_t* motor, uint32_t steps, uint32_t pulse_us, uint32_t delay_us);

void STEPPER_StepThree(stepper_t* m1, stepper_t* m2, stepper_t* m3, uint32_t steps, uint32_t delay_ms);
void STEPPER_StepTwo(stepper_t* m1, stepper_t* m2, uint32_t steps, uint32_t pulse_us, uint32_t delay_us);

// Temporary fix to bypass SysTick conflicts
#define delay_us(us) HAL_Delay(us / 1000 > 0 ? us / 1000 : 1) 

#endif

