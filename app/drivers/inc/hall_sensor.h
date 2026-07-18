
#ifndef HALL_SENSOR_H
#define HALL_SENSOR_H

#include "stm32f4xx_hal.h"

void    HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void     HallSensor_Init(TIM_HandleTypeDef *htim);
float    HallSensor_GetRPM(void);
uint32_t HallSensor_GetCounts(void);

#endif

// Map of Input Values to Expected RPM
// 0G: 0 RPM
// 1G: 97 RPM
// 3G: 167 RPM
// 5G: 216 RPM
// 7.5G: 265 RPM