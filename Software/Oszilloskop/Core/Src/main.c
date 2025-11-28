/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "channelData.h"
#include <wizchip_conf.h>
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == &hadc1) {
		// der DMA buffer ist complett voll und der ADC wird vom anfang an beginnen, zweite hälfte kann geschickt werden
		if (running == 1)  {
			//uint16_t len = DMA_BUFFER_LEN / 2;
			//HAL_UART_Transmit(&huart2, (void*)&len, sizeof(uint16_t), 100);
			//HAL_UART_Transmit(&huart2, (void*)&dmaBuffer[DMA_BUFFER_LEN / 2], DMA_BUFFER_LEN / 2, 100);
			HAL_UART_Transmit_IT(&huart2, (void*)&dmaBuffer[DMA_BUFFER_LEN / 2], DMA_BUFFER_LEN / 2);
		}
		/*
		else if (running == 2) {
			// for a single buffer the dma is simply stoped when a buffer is written
			HAL_ADC_Stop_DMA(&hadc1);
			running = 0;
		}*/
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == &hadc1) {
		// der DMA buffer ist halb voll und die erste hälfte kann geschickt werden
		if (running == 1) {
			//uint16_t len = DMA_BUFFER_LEN / 2;
			//HAL_UART_Transmit(&huart2, (void*)&len, sizeof(uint16_t), 100);
			//HAL_UART_Transmit(&huart2, (void*)dmaBuffer, DMA_BUFFER_LEN / 2, 100);
			HAL_UART_Transmit_IT(&huart2, (void*)dmaBuffer, DMA_BUFFER_LEN / 2);
		}
	}
}

void configADCStreaming() {
	HAL_ADC_Stop_DMA(&hadc1); // stops previous dma conversion
	MX_ADC1_Init(); // Streaming is the default ADC ioc config
}

void configADCSingleBuffer() {
	// stop prevous conversion
	HAL_ADC_Stop_DMA(&hadc1);

	// reconfig the adc for fastest conversion
	ADC_ChannelConfTypeDef sConfig = {0};

	/** Common config
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1; // the clock prescaler has to be 1 for max samples
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = 4;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
	Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5; // sets the sample time to 2.5 cycles instead of 12.5 for faster conversion
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = ADC_REGULAR_RANK_4;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
}

void cs_sel() {
	HAL_GPIO_WritePin(NRST_GPIO_Port, NRST_Pin, GPIO_PIN_RESET); //CS LOW
}

void cs_desel() {
	HAL_GPIO_WritePin(NRST_GPIO_Port, NRST_Pin, GPIO_PIN_SET); //CS HIGH
}

uint8_t spi_rb(void) {
	uint8_t rbuf;
	HAL_SPI_Receive(&hspi1, &rbuf, 1, 0xFFFFFFFF);
	return rbuf;
}

void spi_wb(uint8_t b) {
	HAL_SPI_Transmit(&hspi1, &b, 1, 0xFFFFFFFF);
}

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  uint8_t retVal, sockStatus;
  int16_t rcvLen;
  uint8_t rcvBuf[20], bufSize[] = {2, 2, 2, 2};

  reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
  reg_wizchip_spi_cbfunc(spi_rb, spi_wb);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //HAL_TIM_Base_Start(&htim6);
  //htim6.Instance->CNT = 0;
  // der DMA (direct memory access) speichert direct den ADC wert in einen gegebenen buffer mit einer gegebenen länge
  // wenn der buffer halb voll ist und voll ist, dann schickt man die daten und es loopt wieder zum start zum schreiben
  // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)dmaBuffer, DMA_BUFFER_LEN);
  //HAL_DMA_Start(hdma, SrcAddress, DstAddress, DataLength);

  /*
   * The current config is for 4 channels so the following data has to be sent:
   * 	- num Channels: 4 (1 byte uint)
   * 	- for each channel: (4 times)
   * 		- scale (4 byte float)
   * 	- frequency: 10000 (4 byte float)
   * 	- data packet length DMA_BUFFER_LEN / 2 (2 byte uint)
   */

  while (1)
  {
	  uint8_t numChannels;
	  uint8_t newState;
	  float frequency;
	  float channelInputRanges[4] = {10, 36, 10, 1};
	  while (HAL_UART_Receive(&huart2, &newState, 1, 100) != HAL_OK) {}
	  running = 0;
	  switch (newState) {

	  case 1:
		  // configure the adc for a uart manageable conversion speed
		  //configADCStreaming();

		  // send all relevant data for streaming
		  numChannels = 4;
		  HAL_UART_Transmit(&huart2, (void*)&numChannels, sizeof(uint8_t), 100);
		  for (int i = 0; i < numChannels; i++) {
			  float scale = calculateScale(channelInputRanges[i]);
			  HAL_UART_Transmit(&huart2, (void*)&scale, sizeof(float), 100);
		  }
		  frequency = 10000;
		  HAL_UART_Transmit(&huart2, (void*)&frequency, sizeof(float), 100);
		  uint16_t len = DMA_BUFFER_LEN / 2;
		  HAL_UART_Transmit(&huart2, (void*)&len, sizeof(uint16_t), 100);

		  // start continuous dma conversion
		  running = 1;
		  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)dmaBuffer, DMA_BUFFER_LEN);
		  break;

	  /*case 2:
		  // configure the adc for maximum converison speed
		  configADCSingleBuffer();

		  // in case two we want to read a single data lenght at our max reading frequency into a variable lenght dma buffer
		  uint16_t numReadings;
		  while (HAL_UART_Receive(&huart2, &numReadings, 2, 100) != HAL_OK) {}
		  uint16_t dmaBufferLen = numReadings * 4;
		  uint16_t* dmaSingleBuffer = (uint16_t*)malloc(dmaBufferLen);
		  HAL_ADC_Stop_DMA(&hadc1);
		  running = 2;
		  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)dmaSingleBuffer, dmaBufferLen);

		  // wait until the dma is full
		  while (running != 0) {}

		  // send all the data needed for interpreting the buffer
		  numChannels = 4;
		  HAL_UART_Transmit(&huart2, (void*)&numChannels, sizeof(uint8_t), 100);
		  for (int i = 0; i < numChannels; i++) {
			  float scale = calculateScale(channelInputRanges[i]);
			  HAL_UART_Transmit(&huart2, (void*)&scale, sizeof(float), 100);
		  }
		  frequency = 533333.333;
		  HAL_UART_Transmit(&huart2, (void*)&frequency, sizeof(float), 100);

		  // send the dma buffer
		  HAL_UART_Transmit(&huart2, dmaSingleBuffer, dmaBufferLen * sizeof(uint16_t), 100);

		  // free all of the data to avoid memory leaks
		  free(dmaSingleBuffer);
		  break;*/

	  }

	  /*if (newState) {
		  // wenn es anfängt zu schicken muss man folgendes schicken zur syncronisierung
		  uint8_t numChannels = 4;
		  HAL_UART_Transmit(&huart2, (void*)&numChannels, sizeof(uint8_t), 100);
		  float channelInputRanges[4] = {10, 36, 10, 1};
		  for (int i = 0; i < numChannels; i++) {
			  float scale = calculateScale(channelInputRanges[i]);
			  HAL_UART_Transmit(&huart2, (void*)&scale, sizeof(float), 100);
		  }
		  float frequency = 10000;
		  HAL_UART_Transmit(&huart2, (void*)&frequency, sizeof(float), 100);
		  uint16_t len = DMA_BUFFER_LEN / 2;
		  HAL_UART_Transmit(&huart2, (void*)&len, sizeof(uint16_t), 100);
		  running = 1;
	  }*/
	  	// alles außer 1 wird als stop interprätiert
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

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
