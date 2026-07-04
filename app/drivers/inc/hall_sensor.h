
#ifndef HALL_SENSOR_H
#define HALL_SENSOR_H

#include "stm32f4xx_hal.h"

void     HallSensor_Init(TIM_HandleTypeDef *htim);
float    HallSensor_GetRPM(void);
uint32_t HallSensor_GetCounts(void);

#endif