/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "pwm.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* Definitions for HandleUartRxCmdTask */
osThreadId_t HandleUartRxCmdTaskHandle;
const osThreadAttr_t HandleUartRxCmdTask_attributes = {
  .name = "HandleUartRxCmdTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for StaticMotorControlTask */
osThreadId_t StaticMotorControlTaskHandle;
const osThreadAttr_t StaticMotorControlTask_attributes = {
  .name = "StaticMotorControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DynamicMotorControlTask */
osThreadId_t DynamicMotorControlTaskHandle;
const osThreadAttr_t DynamicMotorControlTask_attributes = {
  .name = "DynamicMotorControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for HeaterControlTask */
osThreadId_t HeaterControlTaskHandle;
const osThreadAttr_t HeaterControlTask_attributes = {
  .name = "HeaterControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DataAcquisitionTask */
osThreadId_t DataAcquisitionTaskHandle;
const osThreadAttr_t DataAcquisitionTask_attributes = {
  .name = "DataAcquisitionTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uartRxMessageQueue */
osMessageQueueId_t uartRxMessageQueueHandle;
const osMessageQueueAttr_t uartRxMessageQueue_attributes = {
  .name = "uartRxMessageQueue"
};
/* Definitions for staticMotorQueue */
osMessageQueueId_t staticMotorQueueHandle;
const osMessageQueueAttr_t staticMotorQueue_attributes = {
  .name = "staticMotorQueue"
};
/* Definitions for dynamicMotorQueue */
osMessageQueueId_t dynamicMotorQueueHandle;
const osMessageQueueAttr_t dynamicMotorQueue_attributes = {
  .name = "dynamicMotorQueue"
};
/* Definitions for heaterQueue */
osMessageQueueId_t heaterQueueHandle;
const osMessageQueueAttr_t heaterQueue_attributes = {
  .name = "heaterQueue"
};
/* Definitions for dataReadQueue */
osMessageQueueId_t dataReadQueueHandle;
const osMessageQueueAttr_t dataReadQueue_attributes = {
  .name = "dataReadQueue"
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
  .name = "uartMutex"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
void StartUartRxCmdTask(void *argument);
void StartStaticMotorTask(void *argument);
void StartDynamicMotorTask(void *argument);
void StartHeaterTask(void *argument);
void StartDataAcquisitionTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  uart_init(&huart2);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of uartMutex */
  uartMutexHandle = osMutexNew(&uartMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of uartRxMessageQueue */
  uartRxMessageQueueHandle = osMessageQueueNew (16, sizeof(uart_message_t), &uartRxMessageQueue_attributes);

  /* creation of staticMotorQueue */
  staticMotorQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &staticMotorQueue_attributes);

  /* creation of dynamicMotorQueue */
  dynamicMotorQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &dynamicMotorQueue_attributes);

  /* creation of heaterQueue */
  heaterQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &heaterQueue_attributes);

  /* creation of dataReadQueue */
  dataReadQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &dataReadQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of HandleUartRxCmdTask */
  HandleUartRxCmdTaskHandle = osThreadNew(StartUartRxCmdTask, NULL, &HandleUartRxCmdTask_attributes);

  /* creation of StaticMotorControlTask */
  StaticMotorControlTaskHandle = osThreadNew(StartStaticMotorTask, NULL, &StaticMotorControlTask_attributes);

  /* creation of DynamicMotorControlTask */
  DynamicMotorControlTaskHandle = osThreadNew(StartDynamicMotorTask, NULL, &DynamicMotorControlTask_attributes);

  /* creation of HeaterControlTask */
  HeaterControlTaskHandle = osThreadNew(StartHeaterTask, NULL, &HeaterControlTask_attributes);

  /* creation of DataAcquisitionTask */
  DataAcquisitionTaskHandle = osThreadNew(StartDataAcquisitionTask, NULL, &DataAcquisitionTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartUartRxCmdTask */
/**
  * @brief  Function implementing the HandleUartRxCmdTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartUartRxCmdTask */
void StartUartRxCmdTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  uart_message_t rx_msg;
  cmd_msg_t cmd_msg;

  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(uartRxMessageQueueHandle, &rx_msg, NULL, osWaitForever) == osOK)
    {
      cmd_msg.cmd = CMD_NONE;
      cmd_msg.value = 0;

      /* SYSTEM COMMANDS */

      if(strcmp(rx_msg.command, "SYSTEM ON") == 0)
      {
        cmd_msg.cmd = CMD_SYSTEM_ON;

        osMessageQueuePut(staticMotorQueueHandle, &cmd_msg, 0, 0);
        osMessageQueuePut(dataReadQueueHandle, &cmd_msg, 0, 0);
      }

      else if(strcmp(rx_msg.command, "SYSTEM OFF") == 0)
      {
        cmd_msg.cmd = CMD_SYSTEM_OFF;

        osMessageQueuePut(staticMotorQueueHandle, &cmd_msg, 0, 0);
        osMessageQueuePut(dataReadQueueHandle, &cmd_msg, 0, 0);
      }

        /* HEATER COMMANDS */

      else if(strcmp(rx_msg.command, "HEATER ON") == 0)
      {
        cmd_msg.cmd = CMD_HEATER_ON;

        osMessageQueuePut(heaterQueueHandle, &cmd_msg, 0, 0);
      }

      else if(strcmp(rx_msg.command, "HEATER OFF") == 0)
      {
        cmd_msg.cmd = CMD_HEATER_OFF;

        osMessageQueuePut(heaterQueueHandle, &cmd_msg, 0, 0);
      }

      /* MOTOR SPEED */

      else
      {
        int duty = atoi(rx_msg.command);

        if(duty >= 0 && duty <= 255)
        {
          cmd_msg.cmd = CMD_SET_PWM;
          cmd_msg.value = duty;

          osMessageQueuePut(dynamicMotorQueueHandle, &cmd_msg, 0, 0);
        }
        else
        {
          printf("Unknown command: %s\n", rx_msg.command);
        }
      }
    }
    osDelay(1000);

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartStaticMotorTask */
/**
* @brief Function implementing the StaticMotorControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStaticMotorTask */
void StartStaticMotorTask(void *argument)
{
  /* USER CODE BEGIN StartStaticMotorTask */
  // Integrates PWM with UART input
  cmd_msg_t msg;
  pwm_t stepperMotor1;
  pwm_t stepperMotor2;
  pwm_t linearActuator;

  int fixedStepperMotor1Duty = 255;
  int fixedStepperMotor2Duty = 255;
  int fixedLinearActuatorDuty = 255;

  pwm_init(&stepperMotor1, &htim3, TIM_CHANNEL_1);
  pwm_start(&stepperMotor1);  

  pwm_init(&stepperMotor2, &htim3, TIM_CHANNEL_2);
  pwm_start(&stepperMotor2);

  pwm_init(&linearActuator, &htim4, TIM_CHANNEL_1);
  pwm_start(&linearActuator);

  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(staticMotorQueueHandle, &msg, NULL, osWaitForever) == osOK)
    {
      if(msg.cmd == CMD_SYSTEM_ON)
      {
        pwm_set(&stepperMotor1, fixedStepperMotor1Duty);
        pwm_set(&stepperMotor2, fixedStepperMotor2Duty);
        pwm_set(&linearActuator, fixedLinearActuatorDuty);
      }
      else if(msg.cmd == CMD_SYSTEM_OFF)
      {
        pwm_set(&stepperMotor1, 0);
        pwm_set(&stepperMotor2, 0);
        pwm_set(&linearActuator, 0);
      }
    }
    
    osDelay(100);
  }
  /* USER CODE END StartStaticMotorTask */
}

/* USER CODE BEGIN Header_StartDynamicMotorTask */
/**
* @brief Function implementing the DynamicMotorControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDynamicMotorTask */
void StartDynamicMotorTask(void *argument)
{
  /* USER CODE BEGIN StartDynamicMotorTask */
  // Integrates PWM with UART input
  cmd_msg_t msg;
  pwm_t dcMotor;

  pwm_init(&dcMotor, &htim2, TIM_CHANNEL_1);
  pwm_start(&dcMotor);

  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(dynamicMotorQueueHandle, &msg, NULL, osWaitForever) == osOK)
    {
      if(msg.cmd == CMD_SET_PWM)
      {
        pwm_set(&dcMotor, msg.value);
      }
    }

    osDelay(100);
  }
  /* USER CODE END StartDynamicMotorTask */
}

/* USER CODE BEGIN Header_StartHeaterTask */
/**
* @brief Function implementing the HeaterControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHeaterTask */
void StartHeaterTask(void *argument)
{
  /* USER CODE BEGIN StartHeaterTask */
  cmd_msg_t msg;
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(heaterQueueHandle, &msg, NULL, osWaitForever) == osOK)
    {
      if(msg.cmd == CMD_HEATER_ON)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

      else if(msg.cmd == CMD_HEATER_OFF)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
    }
    osDelay(100);
  }
  /* USER CODE END StartHeaterTask */
}

/* USER CODE BEGIN Header_StartDataAcquisitionTask */
/**
* @brief Function implementing the DataAcquisitionTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDataAcquisitionTask */
void StartDataAcquisitionTask(void *argument)
{
  /* USER CODE BEGIN StartDataAcquisitionTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDataAcquisitionTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
