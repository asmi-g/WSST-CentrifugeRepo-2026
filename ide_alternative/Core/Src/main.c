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
#include "thermocouple.h"
#include "servo.h"
#include "stepper.h"
#include "encoder.h"
#include "mcp9808.h"
#include "hall_sensor.h"
#include "tip_cleaner.h"
#include "wire_feeder.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define linearActuator  0
#define SERVO_ACTION_COMPLETE_FLAG  (1U << 0)
#define CARRIER_SEQUENCE_DIRECTION  0U
#define CARRIER_POSITION_STEPS       445U
#define CARRIER_STEP_PULSE_US          5U
#define CARRIER_STEP_DELAY_US        2000U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;

/* Definitions for HandleUartRxCmdTask */
osThreadId_t HandleUartRxCmdTaskHandle;
const osThreadAttr_t HandleUartRxCmdTask_attributes = {
  .name = "HandleUartRxCmdTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for StepperMotorControlTask */
osThreadId_t StepperMotorControlTaskHandle;
const osThreadAttr_t StepperMotorControlTask_attributes = {
  .name = "StepperMotorControlTask",
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
/* Definitions for ServoMotorControlTask */
osThreadId_t ServoMotorControlTaskHandle;
const osThreadAttr_t ServoMotorControlTask_attributes = {
  .name = "ServoMotorControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for EncoderTask */
osThreadId_t EncoderTaskHandle;
const osThreadAttr_t EncoderTask_attributes = {
  .name = "EncoderTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uartRxMessageQueue */
osMessageQueueId_t uartRxMessageQueueHandle;
const osMessageQueueAttr_t uartRxMessageQueue_attributes = {
  .name = "uartRxMessageQueue"
};
/* Definitions for stepperMotorQueue */
osMessageQueueId_t stepperMotorQueueHandle;
const osMessageQueueAttr_t stepperMotorQueue_attributes = {
  .name = "stepperMotorQueue"
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
/* Definitions for servoMotorQueue */
osMessageQueueId_t servoMotorQueueHandle;
const osMessageQueueAttr_t servoMotorQueue_attributes = {
  .name = "servoMotorQueue"
};
/* Definitions for encoderQueue */
osMessageQueueId_t encoderQueueHandle;
const osMessageQueueAttr_t encoderQueue_attributes = {
  .name = "encoderQueue"
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
  .name = "uartMutex"
};
/* Definitions for spiMutex */
osMutexId_t spiMutexHandle;
const osMutexAttr_t spiMutex_attributes = {
  .name = "spiMutex"
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
static void MX_SPI1_Init(void);
static void MX_TIM10_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM11_Init(void);
void StartUartRxCmdTask(void *argument);
void StartStepperMotorTask(void *argument);
void StartDynamicMotorTask(void *argument);
void StartHeaterTask(void *argument);
void StartDataAcquisitionTask(void *argument);
void StartServoMotorTask(void *argument);
void StartEncoderTask(void *argument);

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
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_I2C3_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of uartMutex */
  uartMutexHandle = osMutexNew(&uartMutex_attributes);

  /* creation of spiMutex */
  spiMutexHandle = osMutexNew(&spiMutex_attributes);

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

  /* creation of stepperMotorQueue */
  stepperMotorQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &stepperMotorQueue_attributes);

  /* creation of dynamicMotorQueue */
  dynamicMotorQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &dynamicMotorQueue_attributes);

  /* creation of heaterQueue */
  heaterQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &heaterQueue_attributes);

  /* creation of dataReadQueue */
  dataReadQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &dataReadQueue_attributes);

  /* creation of servoMotorQueue */
  servoMotorQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &servoMotorQueue_attributes);

  /* creation of encoderQueue */
  encoderQueueHandle = osMessageQueueNew (16, sizeof(cmd_msg_t), &encoderQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of HandleUartRxCmdTask */
  HandleUartRxCmdTaskHandle = osThreadNew(StartUartRxCmdTask, NULL, &HandleUartRxCmdTask_attributes);

  /* creation of StepperMotorControlTask */
  StepperMotorControlTaskHandle = osThreadNew(StartStepperMotorTask, NULL, &StepperMotorControlTask_attributes);

  /* creation of DynamicMotorControlTask */
  DynamicMotorControlTaskHandle = osThreadNew(StartDynamicMotorTask, NULL, &DynamicMotorControlTask_attributes);

  /* creation of HeaterControlTask */
  HeaterControlTaskHandle = osThreadNew(StartHeaterTask, NULL, &HeaterControlTask_attributes);

  /* creation of DataAcquisitionTask */
  DataAcquisitionTaskHandle = osThreadNew(StartDataAcquisitionTask, NULL, &DataAcquisitionTask_attributes);

  /* creation of ServoMotorControlTask */
  ServoMotorControlTaskHandle = osThreadNew(StartServoMotorTask, NULL, &ServoMotorControlTask_attributes);

  /* creation of EncoderTask */
  EncoderTaskHandle = osThreadNew(StartEncoderTask, NULL, &EncoderTask_attributes);

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
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 159;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0x03;
  if (HAL_TIM_IC_ConfigChannel(&htim11, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, STEPPER1_GPIO_Pin|STEPPER2_GPIO_Pin|HEATER2_GPIO_Pin|HEATER1_GPIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TC1_CS_Pin|TC2_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PCB_CARRIER_DIR_GPIO_Port, PCB_CARRIER_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PCB_CARRIER_GPIO_GPIO_Port, PCB_CARRIER_GPIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : STEPPER1_GPIO_Pin STEPPER2_GPIO_Pin HEATER2_GPIO_Pin HEATER1_GPIO_Pin */
  GPIO_InitStruct.Pin = STEPPER1_GPIO_Pin|STEPPER2_GPIO_Pin|HEATER2_GPIO_Pin|HEATER1_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TC1_CS_Pin TC2_CS_Pin PCB_CARRIER_GPIO_Pin */
  GPIO_InitStruct.Pin = TC1_CS_Pin|TC2_CS_Pin|PCB_CARRIER_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PCB_CARRIER_Z__Pin */
  GPIO_InitStruct.Pin = PCB_CARRIER_Z__Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PCB_CARRIER_Z__GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PCB_CARRIER_DIR_Pin */
  GPIO_InitStruct.Pin = PCB_CARRIER_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PCB_CARRIER_DIR_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

static osStatus_t queue_physical_action(cmd_t command)
{
  cmd_msg_t action_msg = {
    .cmd = command,
    .value = 0U
  };

  return osMessageQueuePut(
    stepperMotorQueueHandle,
    &action_msg,
    0U,
    0U
  );
}

static osStatus_t request_servo_action(cmd_t command)
{
  cmd_msg_t servo_msg = {
    .cmd = command,
    .value = 0U
  };

  (void)osThreadFlagsClear(SERVO_ACTION_COMPLETE_FLAG);

  if(osMessageQueuePut(servoMotorQueueHandle, &servo_msg, 0U, osWaitForever) != osOK)
  {
    return osError;
  }

  uint32_t flags = osThreadFlagsWait(
    SERVO_ACTION_COMPLETE_FLAG,
    osFlagsWaitAny,
    osWaitForever
  );

  if((flags & osFlagsError) != 0U)
  {
    return osError;
  }

  return ((flags & SERVO_ACTION_COMPLETE_FLAG) != 0U) ? osOK : osError;
}

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

  // Start interrupt-driven reception only after the RTOS queue exists.
  uart_init(&huart2);

  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(uartRxMessageQueueHandle, &rx_msg, NULL, osWaitForever) == osOK)
    {
      cmd_msg.cmd = CMD_NONE;
      cmd_msg.value = 0;

      /* SYSTEM ON/OFF are intentionally inactive. Operator commands are independent. */

      /* HEATER COMMANDS */

      if(strcmp(rx_msg.command, "HEATER ON") == 0)
      {
        cmd_msg.cmd = CMD_HEATER_ON;

        osMessageQueuePut(heaterQueueHandle, &cmd_msg, 0, 0);
      }

      else if(strcmp(rx_msg.command, "HEATER OFF") == 0)
      {
        cmd_msg.cmd = CMD_HEATER_OFF;

        osMessageQueuePut(heaterQueueHandle, &cmd_msg, 0, 0);
      }

      /* PHYSICAL ACTIONS */
      else if(strcmp(rx_msg.command, "SOLDER") == 0)
      {
        if(queue_physical_action(CMD_SOLDER) != osOK)
        {
          printf("Physical action queue full; SOLDER not accepted.\n");
        }
      }

      else if(strcmp(rx_msg.command, "NEXT POSITION") == 0)
      {
        if(queue_physical_action(CMD_NEXT_POSITION) != osOK)
        {
          printf("Physical action queue full; NEXT POSITION not accepted.\n");
        }
      }

      else if(strcmp(rx_msg.command, "CLEAN") == 0)
      {
        if(queue_physical_action(CMD_TIP_CLEAN) != osOK)
        {
          printf("Physical action queue full; CLEAN not accepted.\n");
        }
      }

      /* Hidden safety command; its GUI button remains commented out. */
      else if(strcmp(rx_msg.command, "RETRACT TIPS") == 0)
      {
        if(queue_physical_action(CMD_SERVO_RETRACT) != osOK)
        {
          printf("Physical action queue full; RETRACT TIPS not accepted.\n");
        }
      }

      /* MOTOR SPEED */

      else
      {
        char *end_ptr = NULL;
        long duty = strtol(rx_msg.command, &end_ptr, 10);

        if((end_ptr != rx_msg.command) && (*end_ptr == '\0')
            && (duty >= 0) && (duty <= 255))
        {
          cmd_msg.cmd = CMD_SET_PWM;
          cmd_msg.value = (uint16_t)duty;

          osMessageQueuePut(dynamicMotorQueueHandle, &cmd_msg, 0, 0);
        }
        else
        {
          printf("Unknown command: %s\n", rx_msg.command);
        }
      }
    }
    osDelay(50);

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartStepperMotorTask */
/**
* @brief Function implementing the StepperMotorControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStepperMotorTask */
void StartStepperMotorTask(void *argument)
{
  /* USER CODE BEGIN StartStepperMotorTask */
  cmd_msg_t msg;

  stepper_t stepper1;
  stepper_t stepper2;
  stepper_t stepper3;

  // PA6 and PA7 drive the two wire feeders; PB8 drives the PCB carrier.
  STEPPER_Init(&stepper1, GPIOA, GPIO_PIN_6, NULL, 0);
  STEPPER_Init(&stepper2, GPIOA, GPIO_PIN_7, NULL, 0);
  STEPPER_Init(&stepper3, GPIOB, GPIO_PIN_8, GPIOC, GPIO_PIN_11);

  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(stepperMotorQueueHandle, &msg, NULL, osWaitForever) == osOK)
    {
      if(msg.cmd == CMD_SOLDER)
      {
        if(request_servo_action(CMD_SERVO_SOLDER) == osOK)
        {
          WIRE_FEEDER_Run(&stepper1, &stepper2);
          osDelay(TIP_SOLDER_DWELL_MS);
        }

        // Always finish a solder request at the safe, fully retracted angle.
        (void)request_servo_action(CMD_SERVO_RETRACT);
      }

      else if(msg.cmd == CMD_NEXT_POSITION)
      {
        // Retraction must complete before the carrier is allowed to move.
        if(request_servo_action(CMD_SERVO_RETRACT) == osOK)
        {
          STEPPER_SetDir(&stepper3, CARRIER_SEQUENCE_DIRECTION);
          STEPPER_Step(
            &stepper3,
            CARRIER_POSITION_STEPS,
            CARRIER_STEP_PULSE_US,
            CARRIER_STEP_DELAY_US
          );
        }
      }

      else if(msg.cmd == CMD_TIP_CLEAN)
      {
        if(request_servo_action(CMD_TIP_CLEAN) == osOK)
        {
          // Enforce the full 70-degree safety retraction after all three strokes.
          (void)request_servo_action(CMD_SERVO_RETRACT);
        }
      }

      else if(msg.cmd == CMD_SERVO_RETRACT)
      {
        (void)request_servo_action(CMD_SERVO_RETRACT);
      }

    }
  }
  /* USER CODE END StartStepperMotorTask */
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

  pwm_init(&dcMotor, DYNAMIC_MOTOR_PWM_TIMER, DYNAMIC_MOTOR_PWM_CHANNEL);
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
      if(msg.cmd == CMD_HEATER_ON){
        HAL_GPIO_WritePin(HEATER1_GPIO_PORT, HEATER1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HEATER2_GPIO_PORT, HEATER2_PIN, GPIO_PIN_SET);
      }

      else if(msg.cmd == CMD_HEATER_OFF){
        HAL_GPIO_WritePin(HEATER1_GPIO_PORT, HEATER1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(HEATER2_GPIO_PORT, HEATER2_PIN, GPIO_PIN_RESET);
      }
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
  char msg[128];

  max31856_t therm1 = {&hspi1, {TC1_CS_GPIO_Port, TC1_CS_Pin}};
  max31856_init(&therm1);
  max31856_set_noise_filter(&therm1, CR0_FILTER_OUT_50Hz);
  max31856_set_cold_junction_enable(&therm1, CR0_CJ_ENABLED);
  max31856_set_thermocouple_type(&therm1, CR1_TC_TYPE_K);
  max31856_set_average_samples(&therm1, CR1_AVG_TC_SAMPLES_2);
  max31856_set_open_circuit_fault_detection(&therm1, CR0_OC_DETECT_ENABLED_TC_LESS_2ms);
  max31856_set_conversion_mode(&therm1, CR0_CONV_CONTINUOUS);

  max31856_t therm2 = {&hspi1, {TC2_CS_GPIO_Port, TC2_CS_Pin}};
  max31856_init(&therm2);
  max31856_set_noise_filter(&therm2, CR0_FILTER_OUT_50Hz);
  max31856_set_cold_junction_enable(&therm2, CR0_CJ_ENABLED);
  max31856_set_thermocouple_type(&therm2, CR1_TC_TYPE_K);
  max31856_set_average_samples(&therm2, CR1_AVG_TC_SAMPLES_2);
  max31856_set_open_circuit_fault_detection(&therm2, CR0_OC_DETECT_ENABLED_TC_LESS_2ms);
  max31856_set_conversion_mode(&therm2, CR0_CONV_CONTINUOUS);

  // MCP9808 setup
  MCP9808_DEVICE mcp = mcp9808_load_device(0x18);
  mcp.Configuration = 0x0000; // default: continuous conversion, no shutdown
  mcp.Resolution = 0x03;      // 0.0625C resolution
  mcp9808_apply_configuration(&mcp);

  HallSensor_Init(&htim11);

  osDelay(250);

  /* Infinite loop */
  for(;;)
  {

    //Read thermocouple temperature and send over UART
    max31856_read_fault(&therm1);
    max31856_read_fault(&therm2);
    if (therm1.sr.val) {
      /* Handle thermocouple error */
    }
    if (therm2.sr.val) {
      /* Handle thermocouple error */
    }

    // Thermocouples
    osMutexAcquire(spiMutexHandle, osWaitForever);
    float temp1 = max31856_read_TC_temp(&therm1);
    float temp2 = max31856_read_TC_temp(&therm2);
    osMutexRelease(spiMutexHandle);
    snprintf(msg, sizeof(msg), "TC1: %.2f C, TC2: %.2f C\r\n", temp1, temp2);
    uart_tx(msg);

    // External temp sensor, MCP9808
    float boardTemp = mcp9808_get_temp_float(mcp9808_read_temperature(&mcp));
    snprintf(msg, sizeof(msg), "TEMP:%.2f\r\n", boardTemp);
    uart_tx(msg);

    // BLDC Hall Sensor / speed output
    uint32_t counts = HallSensor_GetCounts();
    float rpm = HallSensor_GetRPM();
    float deg = HallSensor_CountsToDegrees(counts);
    snprintf(msg, sizeof(msg), "ENC: %ld counts | %.1f deg | %.1f RPM\r\n", counts, deg, rpm);
    uart_tx(msg);



    osDelay(500);
  }
  /* USER CODE END StartDataAcquisitionTask */
}

/* USER CODE BEGIN Header_StartServoMotorTask */
/**
* @brief Function implementing the ServoMotorControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartServoMotorTask */
void StartServoMotorTask(void *argument)
{
  /* USER CODE BEGIN StartServoMotorTask */
  cmd_msg_t msg;

  SERVO_Init(linearActuator);
  SERVO_MoveTo(linearActuator, TIP_RETRACTED_ANGLE);

  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(servoMotorQueueHandle, &msg, NULL, osWaitForever) == osOK)
    {
      uint8_t action_complete = 1U;

      if(msg.cmd == CMD_SERVO_SOLDER)
      {
        SERVO_MoveTo(linearActuator, TIP_SOLDER_ANGLE);
        osDelay(TIP_SERVO_MOVE_MS);
      }

      else if(msg.cmd == CMD_TIP_CLEAN)
      {
        TIP_CLEANER_Run(linearActuator);
      }

      else if(msg.cmd == CMD_SERVO_RETRACT)
      {
        SERVO_MoveTo(linearActuator, TIP_RETRACTED_ANGLE);
        osDelay(TIP_SERVO_MOVE_MS);
      }

      else
      {
        action_complete = 0U;
      }

      if(action_complete != 0U)
      {
        (void)osThreadFlagsSet(
          StepperMotorControlTaskHandle,
          SERVO_ACTION_COMPLETE_FLAG
        );
      }
    }
  }
  /* USER CODE END StartServoMotorTask */
}

/* USER CODE BEGIN Header_StartEncoderTask */
/**
* @brief Function implementing the EncoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartEncoderTask */
void StartEncoderTask(void *argument)
{
  /* USER CODE BEGIN StartEncoderTask */
  encoder_t enc1;
  ENCODER_Init(&enc1, &htim3, 2000);  // 4000 CPR from datasheet
  ENCODER_Zero(&enc1);                // start from 0

  char msg[64];

  /* Infinite loop */
  for(;;)
  {
    int32_t counts  = ENCODER_GetCount(&enc1);
    float   degrees = ENCODER_GetDegrees(&enc1);
    float   rpm     = ENCODER_GetRPM(&enc1, 50); // matches osDelay below

    // snprintf(msg, sizeof(msg), "ENC: %ld counts | %.1f deg | %.1f RPM\r\n", counts, degrees, rpm);
    // uart_tx(msg);

    ENCODER_WaitForIndex(&enc1, GPIOC, GPIO_PIN_8);

    osDelay(50);
  }
  /* USER CODE END StartEncoderTask */
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
