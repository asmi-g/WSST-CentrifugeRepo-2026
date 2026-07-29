#include "hall_sensor.h"

static TIM_HandleTypeDef *motorTimer;
static volatile uint32_t prevCapture = 0;
static volatile uint32_t periodTicks = 0;
static volatile uint32_t pulseCount = 0;
static volatile uint8_t  firstCapture = 1;
static volatile uint32_t lastCaptureTick = 0; // HAL_GetTick() only used for stall detection, not RPM math

#define TIM11_TICK_HZ   100000UL

void HallSensor_Init(TIM_HandleTypeDef *htim)
{
    motorTimer = htim;
    HAL_TIM_IC_Start_IT(motorTimer, TIM_CHANNEL_1);
}

uint32_t HallSensor_GetCounts(void) { return pulseCount; }

float HallSensor_CountsToDegrees(uint32_t counts)
{
    return (float)(counts % HALL_SENSOR_PULSES_PER_REV)
         * (360.0f / (float)HALL_SENSOR_PULSES_PER_REV);
}

float HallSensor_GetRPM(void)
{
    if (HAL_GetTick() - lastCaptureTick > 300) return 0.0f;
    if (periodTicks == 0) return 0.0f;

    float period_s = (float)periodTicks / (float)TIM11_TICK_HZ;
    float raw_rpm = 60.0f / (period_s * (float)HALL_SENSOR_PULSES_PER_REV);
    return raw_rpm;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM11 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        uint32_t capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

        if (!firstCapture) {
            uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);
            periodTicks = (capture >= prevCapture)
                        ? (capture - prevCapture)
                        : (arr - prevCapture) + capture + 1;
        } else {
            firstCapture = 0;
        }

        prevCapture = capture;
        pulseCount++;
        lastCaptureTick = HAL_GetTick();
    }
}
