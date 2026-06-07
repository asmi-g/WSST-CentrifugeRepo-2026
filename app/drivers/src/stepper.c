#include "stepper.h"

void STEPPER_Init(stepper_t* motor, GPIO_TypeDef* stepPort, uint16_t stepPin, GPIO_TypeDef* dirPort,  uint16_t dirPin)
{
    motor->stepPort = stepPort;
    motor->stepPin  = stepPin;
    motor->dirPort  = dirPort;
    motor->dirPin   = dirPin;

    HAL_GPIO_WritePin(stepPort, stepPin, GPIO_PIN_RESET);

    if(dirPort != NULL)
        HAL_GPIO_WritePin(dirPort, dirPin, GPIO_PIN_RESET);
}

void STEPPER_SetDir(stepper_t* motor, uint8_t forward)
{
    if(motor->dirPort == NULL) return;  // silently skip if no DIR pin

    HAL_GPIO_WritePin(motor->dirPort, motor->dirPin,
                      forward ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void STEPPER_Step(stepper_t* motor, uint32_t steps, uint32_t pulse_us, uint32_t delay_us) {
    if (steps == 0) return; 

    if (pulse_us < 5) pulse_us = 5; 
    if (delay_us < 5) delay_us = 5; 
    
    for (uint32_t i = 0; i < steps; i++) {
        HAL_GPIO_WritePin(motor->stepPort, motor->stepPin, GPIO_PIN_SET);
        delay_us(pulse_us);  // Controls speed (lower delay = faster)
        HAL_GPIO_WritePin(motor->stepPort, motor->stepPin, GPIO_PIN_RESET);
        delay_us(delay_us); 
    }
}

void STEPPER_StepTwo(stepper_t* m1, stepper_t* m2, uint32_t steps, uint32_t pulse_us, uint32_t delay_us) {
    if (steps == 0) return; 

    if (pulse_us < 5) pulse_us = 5; 
    if (delay_us < 5) delay_us = 5; 

    for (uint32_t i = 0; i < steps; i++) {
        // Set all Step pins HIGH
        HAL_GPIO_WritePin(m1->stepPort, m1->stepPin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m2->stepPort, m2->stepPin, GPIO_PIN_SET);
        
        delay_us(pulse_us); 

        // Set all Step pins LOW
        HAL_GPIO_WritePin(m1->stepPort, m1->stepPin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m2->stepPort, m2->stepPin, GPIO_PIN_RESET);
        
        delay_us(delay_us); 
    }
    
}


void STEPPER_StepThree(stepper_t* m1, stepper_t* m2, stepper_t* m3, uint32_t steps, uint32_t us) {
    if (steps == 0) return; 

    if (us < 5) us = 5; 

    for (uint32_t i = 0; i < steps; i++) {
        // Set all Step pins HIGH
        HAL_GPIO_WritePin(m1->stepPort, m1->stepPin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m2->stepPort, m2->stepPin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m3->stepPort, m3->stepPin, GPIO_PIN_SET);
        
        delay_us(us); 

        // Set all Step pins LOW
        HAL_GPIO_WritePin(m1->stepPort, m1->stepPin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m2->stepPort, m2->stepPin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m3->stepPort, m3->stepPin, GPIO_PIN_RESET);
        
        delay_us(us); 
    }
    
}
