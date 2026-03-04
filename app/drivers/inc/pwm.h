#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct
{
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  uint32_t arr;
} pwm_t;

HAL_StatusTypeDef pwm_init(pwm_t *p, TIM_HandleTypeDef *htim, uint32_t channel);
HAL_StatusTypeDef pwm_start(pwm_t *p);
HAL_StatusTypeDef pwm_set_u8(pwm_t *p, uint8_t duty_u8);

#endif