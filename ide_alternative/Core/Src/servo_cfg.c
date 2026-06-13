#include "servo.h"

const SERVO_CfgType SERVO_CfgParam[SERVO_NUM] =
{
	// Servo Motor 1 Configurations
    {
        GPIOB,
        GPIO_PIN_6,
        TIM4,
        &TIM4->CCR1,
        TIM_CHANNEL_1,
        16000000,
		0.5,
		2.5
	}
};