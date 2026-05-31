#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct {
    TIM_HandleTypeDef *htim;
    int32_t            offset;      // for zeroing
    uint16_t           cpr;         // counts per revolution (4000 for your motor)
} encoder_t;

void    ENCODER_Init        (encoder_t *enc, TIM_HandleTypeDef *htim, uint16_t cpr);
void    ENCODER_Zero        (encoder_t *enc);
int32_t ENCODER_GetCount    (encoder_t *enc);
float   ENCODER_GetDegrees  (encoder_t *enc);
float   ENCODER_GetRPM      (encoder_t *enc, uint32_t delta_ms);
void    ENCODER_WaitForIndex(encoder_t *enc, GPIO_TypeDef *zPort, uint16_t zPin);

#endif