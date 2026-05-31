#include "encoder.h"

void ENCODER_Init(encoder_t *enc, TIM_HandleTypeDef *htim, uint16_t cpr)
{
    enc->htim   = htim;
    enc->cpr    = cpr;
    enc->offset = 0;

    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(htim, 0);
}

void ENCODER_Zero(encoder_t *enc)
{
    enc->offset = (int32_t)__HAL_TIM_GET_COUNTER(enc->htim);
}

int32_t ENCODER_GetCount(encoder_t *enc)
{
    return (int32_t)__HAL_TIM_GET_COUNTER(enc->htim) - enc->offset;
}

float ENCODER_GetDegrees(encoder_t *enc)
{
    int32_t count = ENCODER_GetCount(enc);
    return (float)(count % enc->cpr) * (360.0f / (float)enc->cpr);
}

// Call this from your task on a known interval, pass in your osDelay period
float ENCODER_GetRPM(encoder_t *enc, uint32_t delta_ms)
{
    static int32_t last_count = 0;

    int32_t curr  = ENCODER_GetCount(enc);
    int32_t delta = curr - last_count;
    last_count    = curr;

    // counts/ms → revolutions/min
    return ((float)delta / (float)enc->cpr) * (60000.0f / (float)delta_ms);
}

void ENCODER_WaitForIndex(encoder_t *enc, GPIO_TypeDef *zPort, uint16_t zPin)
{
    // Wait for Z pulse to go high then low (one full pulse)
    while(HAL_GPIO_ReadPin(zPort, zPin) == GPIO_PIN_RESET);  // wait for rising
    while(HAL_GPIO_ReadPin(zPort, zPin) == GPIO_PIN_SET);    // wait for falling
    ENCODER_Zero(enc);  // zero position at index
}
