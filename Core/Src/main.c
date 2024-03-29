/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include "Rtosinclude.h"
#include "Queue.h"
#include "Mutex.h"
#include "Events.h"
#include <math.h>
#include <stdio.h>
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

T_TaskTCB TaskUSB;
T_TaskTCB TaskCharge;
T_TaskTCB TaskBat1;
T_TaskTCB TaskBat2;
T_TaskTCB TaskFloat;
T_TaskTCB TaskDisplay;
T_TaskTCB TaskUART;

u32 StackTaskUSB[256];
u32 StackTaskCharge[256];
u32 StackTaskBat1[256];
u32 StackTaskBat2[256];
u32 StackTaskFloat[256];
u32 StackTaskDisplay[256];
u32 StackTaskUART[256];



ADC_ChannelConfTypeDef sConfig = {0};


T_MutexHandler_Ptr Mutex_BAT1;
T_MutexHandler_Ptr Mutex_BAT2;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MED_BAT						0x0001
#define MED_USB						0x0010


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

u32 	Adc_Bat1 	= 	0;
u32 	Adc_Bat2	= 	0;
u8 		USB_Charge 	=	0;

uint8_t MessageOled[35] = {'\0'};

f Float_Bat1 = 0;
f Float_Bat2 = 0;
T_EventHanlder Evento;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void FunctionTaskUSB (void);
void FunctionTaskCharge (void);
void FunctionTaskBat1 (void);
void FunctionTaskBat2 (void);
void FunctionTaskFloat (void);
void FunctionTaskDisplay (void);
void FunctionTaskUART (void);
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

	RTOS_Init();

	//Task_Create(&Tarea3, 3, "Tarea3",1, (pu32)StackTarea3, 256, FunctionTarea3);




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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	//ssd1306_Init();

	//ssd1306_SetColor(White);

	Mutex_Init(Mutex_BAT1);
	Mutex_Init(Mutex_BAT2);

	HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);




  //RTOS_Start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		sConfig.Channel = ADC_CHANNEL_7;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		HAL_ADC_Start(&hadc2);
		for(int i = 0; i<10000; i++);
		Adc_Bat2 = HAL_ADC_GetValue(&hadc2);
		Float_Bat1 = Adc_Bat2/4096.0 * 3.3 * 2;
		u8 message[35] = {'\0'};
		sprintf(message, "1 %.2f\r\n", Float_Bat1);
		HAL_UART_Transmit(&huart2, message, sizeof(message), 100);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOB, S2_Pin|S3_Pin|CE1_Pin|CE2_Pin
                          |S1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : S2_Pin S3_Pin CE1_Pin CE2_Pin
                           S1_Pin */
  GPIO_InitStruct.Pin = S2_Pin|S3_Pin|CE1_Pin|CE2_Pin
                          |S1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void FunctionTaskUSB(void){
	for(;;)
	{
		// Set up channel 4 (USB measure)
		  sConfig.Channel = ADC_CHANNEL_4;
		  sConfig.Rank = ADC_REGULAR_RANK_1;
		  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
		  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		  {
		    Error_Handler();
		  }
		  HAL_ADC_Start(&hadc1);
		  Task_Sleep(1);
		  u32 Adc_usb = HAL_ADC_GetValue(&hadc1);
		  if(Adc_usb > 2482){
			  USB_Charge = 1;}
		  else {USB_Charge = 0;}
		  Events_Set(&Evento, MED_USB);


	}
}
void FunctionTaskCharge(void){
	for(;;)
	{
		Events_WaitAll(&Evento, MED_USB);
		Events_Clear(&Evento, MED_USB);
		if(USB_Charge == 1)
		{
			//Aislar baterias
			HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);

			//esperar por medicion de baterias
			Events_WaitAll(&Evento, MED_BAT);
			Events_Clear(&Evento, MED_BAT);
			while((USB_Charge == 1) && (fabs(Float_Bat1 - Float_Bat2) > 0.01))
			{
				//aislar bateria 1 de 2
				HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
				sprintf(MessageOled, "Baterias aisladas cargando\r\n");
				//Iniciar carga de ambas
				HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
				Task_Sleep(3);
				Events_WaitAll(&Evento, MED_USB);
				Events_Clear(&Evento, MED_USB);
				Events_WaitAll(&Evento, MED_BAT);
				Events_Clear(&Evento, MED_BAT);
			}
			//Deshabilitar carga
			while( (USB_Charge == 1) && (Float_Bat1 >= 4.1 ) && (fabs(Float_Bat1 - Float_Bat2) < 0.01) )// Bat 1 y Bat 2 mayor o igual a 4 V
			{
				// deshabilitar carga
				HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
				// Poner baterias en paralelo
				HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
				sprintf(MessageOled, "Baterias en paralelo\r\n");
				Task_Sleep(3);
				Events_WaitAll(&Evento, MED_USB);
				Events_Clear(&Evento, MED_USB);
				Events_WaitAll(&Evento, MED_BAT);
				Events_Clear(&Evento, MED_BAT);
			}
			while( (USB_Charge == 1) && (Float_Bat1 < 4.1 ) && (fabs(Float_Bat1 - Float_Bat2) < 0.01)  ) // BAT1 y BAT2 menor a 4 v
			{
					//Poner baterias en paralelo
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
				sprintf(MessageOled, "Baterias en paralelo cargando\r\n");
					//quitar bateria 2 de cargador 2
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
					//Cargar ambas baterias con cargador 1
				HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
				Task_Sleep(3);
				Events_WaitAll(&Evento, MED_USB);
				Events_Clear(&Evento, MED_USB);
				Events_WaitAll(&Evento, MED_BAT);
				Events_Clear(&Evento, MED_BAT);
			}
		}
		else
		{
			Events_WaitAll(&Evento, MED_BAT);
			Task_Sleep(10);
			Events_Clear(&Evento, MED_BAT);
			while ((fabs(Float_Bat1 - Float_Bat2) > 0.1) && (USB_Charge = 0))
			{
				//Mensaje para conectar cargador
				sprintf(MessageOled, "Conectar cargador\r\n");

				//Aislar baterias
				HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);

				Task_Sleep(3);
				Events_WaitAll(&Evento, MED_USB);
				Events_Clear(&Evento, MED_USB);
				Events_WaitAll(&Evento, MED_BAT);
				Events_Clear(&Evento, MED_BAT);
			}
			while ( (fabs(Float_Bat1 - Float_Bat2) <= 0.1) && (USB_Charge = 0))
			{
				//Baterias en paralelo
				HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
				sprintf(MessageOled, "Baterias en paralelo\r\n");
				//Mensaje de baterias balanceadas
				Task_Sleep(3);
				Events_WaitAll(&Evento, MED_USB);
				Events_Clear(&Evento, MED_USB);
				Events_WaitAll(&Evento, MED_BAT);
				Events_Clear(&Evento, MED_BAT);
			}
		}
		HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
	}
}

void FunctionTaskBat1(void){

	for(;;)
	{
		//borrar bandera de envento
		Mutex_Take(Mutex_BAT1);
		sConfig.Channel = ADC_CHANNEL_7;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		HAL_ADC_Start(&hadc1);
		Task_Sleep(1);
		Adc_Bat1 = HAL_ADC_GetValue(&hadc1);
		Mutex_Give(Mutex_BAT1);
		Task_Sleep(10);

	}

}

void FunctionTaskBat2(void)
{

	while(1)
	{
		Mutex_Take(Mutex_BAT1);
		HAL_ADC_Start(&hadc2);
		Task_Sleep(1);
		Adc_Bat2 = HAL_ADC_GetValue(&hadc2);
		Mutex_Give(Mutex_BAT1);
		Task_Sleep(10);

	}
}
void FunctionTaskFloat(void)
{
	while(1)
	{
		Mutex_Take(Mutex_BAT1);
		Mutex_Take(Mutex_BAT2);
		Float_Bat1 = Adc_Bat2/4096.0 * 3.3 * 2;
		Float_Bat2 = Adc_Bat2/4096.0 * 3.3 * 2;
		Events_Set(&Evento, MED_BAT);
		Mutex_Give(Mutex_BAT1);
		Mutex_Give(Mutex_BAT2);
		Task_Sleep(5);
	}
}

void FunctionTaskDisplay(void)
{
	while(1)
	{
		HAL_UART_Transmit(&huart2, MessageOled, sizeof(MessageOled), 100);
		Task_Sleep(10);
	}
	{

		ssd1306_Clear();

		ssd1306_SetCursor(50, 15);
		ssd1306_WriteString("Voltaje Bateria 1: ",Font_7x10);
		i len = snprintf(NULL, 0, "%f", Float_Bat1);
		c *result = malloc(len + 1);
		snprintf(result, len + 1, "%.2f\r\n", Float_Bat1);
		ssd1306_WriteString(result,Font_7x10);

		ssd1306_SetCursor(60, 15);
		ssd1306_WriteString("Voltaje Bateria 2: ",Font_7x10);
		len = snprintf(NULL, 0, "%f", Float_Bat2);
		result = malloc(len + 1);
		snprintf(result, len + 1, "%.2f\r\n", Float_Bat2);
		ssd1306_WriteString(result,Font_7x10);

		Task_Sleep(10);
		ssd1306_UpdateScreen();


	}
}

void FunctionTaskUART(void)
{
	while(1)
	{

		  u8 message[35] = {'\0'};
		  sprintf(message, "1 %.2f\r\n", Float_Bat1);
		  HAL_UART_Transmit(&huart2, message, sizeof(message), 100);
		  sprintf(message, "2 %.2f\r\n", Float_Bat2);
		  HAL_UART_Transmit(&huart2, message, sizeof(message), 100);
		  Task_Sleep(500);


	}
}


/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
