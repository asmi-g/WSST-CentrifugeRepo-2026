/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DYNAMIC_PWM_Pin GPIO_PIN_5
#define DYNAMIC_PWM_GPIO_Port GPIOA
#define STEPPER1_PWM_Pin GPIO_PIN_6
#define STEPPER1_PWM_GPIO_Port GPIOA
#define STEPPER2_PWM_Pin GPIO_PIN_7
#define STEPPER2_PWM_GPIO_Port GPIOA
#define TC1_CS_Pin GPIO_PIN_0
#define TC1_CS_GPIO_Port GPIOB
#define TC2_CS_Pin GPIO_PIN_1
#define TC2_CS_GPIO_Port GPIOB
#define HEATER1_GPIO_Pin GPIO_PIN_9
#define HEATER1_GPIO_GPIO_Port GPIOA
#define LINEAR1_PWM_Pin GPIO_PIN_6
#define LINEAR1_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef enum
{
    CMD_NONE = 0,

    CMD_SYSTEM_ON,
    CMD_SYSTEM_OFF,

    CMD_HEATER_ON,
    CMD_HEATER_OFF,

    CMD_SET_PWM

} cmd_t;

typedef struct
{
    cmd_t cmd;
    uint16_t value;

} cmd_msg_t;

#define TC1_CS_GPIO_Port GPIOB
#define TC1_CS_Pin GPIO_PIN_0

#define TC2_CS_GPIO_Port GPIOB
#define TC2_CS_Pin GPIO_PIN_1

#define HEATER1_GPIO_PORT GPIOA
#define HEATER1_PIN GPIO_PIN_9

// At PA5
#define DYNAMIC_MOTOR_PWM_TIMER &htim2
#define DYNAMIC_MOTOR_PWM_CHANNEL TIM_CHANNEL_1

// At PA6
#define STEPPER_MOTOR1_PWM_TIMER &htim3
#define STEPPER_MOTOR1_PWM_CHANNEL TIM_CHANNEL_1

// At PA7
#define STEPPER_MOTOR2_PWM_TIMER &htim3
#define STEPPER_MOTOR2_PWM_CHANNEL TIM_CHANNEL_2

// At PB6
#define LINEAR_ACTUATOR_PWM_TIMER &htim4
#define LINEAR_ACTUATOR_PWM_CHANNEL TIM_CHANNEL_1

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
