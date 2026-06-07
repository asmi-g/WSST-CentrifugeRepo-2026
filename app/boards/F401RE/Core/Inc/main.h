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
#define STEPPER1_GPIO_Pin GPIO_PIN_6
#define STEPPER1_GPIO_GPIO_Port GPIOA
#define STEPPER2_GPIO_Pin GPIO_PIN_7
#define STEPPER2_GPIO_GPIO_Port GPIOA
#define TC1_CS_Pin GPIO_PIN_0
#define TC1_CS_GPIO_Port GPIOB
#define TC2_CS_Pin GPIO_PIN_1
#define TC2_CS_GPIO_Port GPIOB
#define PCB_CARRIER_EA__Pin GPIO_PIN_6
#define PCB_CARRIER_EA__GPIO_Port GPIOC
#define PCB_CARRIER_EB__Pin GPIO_PIN_7
#define PCB_CARRIER_EB__GPIO_Port GPIOC
#define HEATER2_GPIO_Pin GPIO_PIN_8
#define HEATER2_GPIO_GPIO_Port GPIOA
#define HEATER1_GPIO_Pin GPIO_PIN_9
#define HEATER1_GPIO_GPIO_Port GPIOA
#define PCB_CARRIER_Z__Pin GPIO_PIN_10
#define PCB_CARRIER_Z__GPIO_Port GPIOC
#define PCB_CARRIER_DIR_Pin GPIO_PIN_11
#define PCB_CARRIER_DIR_GPIO_Port GPIOC
#define SERVO_PWM_Pin GPIO_PIN_6
#define SERVO_PWM_GPIO_Port GPIOB
#define PCB_CARRIER_GPIO_Pin GPIO_PIN_8
#define PCB_CARRIER_GPIO_GPIO_Port GPIOB

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

// At PB0
#define TC1_CS_GPIO_Port GPIOB
#define TC1_CS_Pin GPIO_PIN_0

// At PB1
#define TC2_CS_GPIO_Port GPIOB
#define TC2_CS_Pin GPIO_PIN_1

// At PB5
// MOSI = SDI
#define SPI_MOSI_GPIO_Port GPIOB
#define SPI_MOSI_Pin GPIO_PIN_5

// At PB4
// MISO = SDO
#define SPI_MISO_GPIO_Port GPIOB
#define SPI_MISO_Pin GPIO_PIN_4

// At PB3
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_SCK_Pin GPIO_PIN_3

// At PA9
#define HEATER1_GPIO_PORT GPIOA
#define HEATER1_PIN GPIO_PIN_9

// At PA8
#define HEATER2_GPIO_PORT GPIOA
#define HEATER2_PIN GPIO_PIN_8

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
#define STEPPER_MOTOR3_PWM_TIMER &htim10
#define STEPPER_MOTOR3_PWM_CHANNEL TIM_CHANNEL_1


// At PB6
// If this requires any modification, also update the corresponding object initialization in app/drivers/servo_cfg.c
//Connections needed: 5V, GND, PB6
#define LINEAR_ACTUATOR_PWM_TIMER &htim4
#define LINEAR_ACTUATOR_PWM_CHANNEL TIM_CHANNEL_1

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
