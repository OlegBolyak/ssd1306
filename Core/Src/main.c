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
#include <string.h>
#include <math.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DeactivateScroll 0x2E
#define ActivateScroll 0x2F
#define NOP 0xE3
#define SetSegmentReMap 0xA0
#define EntireDisplayON 0xA4
#define SetNormalInverseDisplay 0xA6
#define SetDisplayONOFF 0xAE
#define SetCOMOutputScanDirection 0xC0
#define ChargePumpSetting 0x8D
#define SetVCOMHDeselectLevel 0xDB
#define SetMemoryAddressingMode 0x20
#define SetCOMPinsHardwareConfiguration 0xDA
#define SetPageStartAddressForPageAddressingMode 0xB0
#define SetHigherColumnStartAddressForPageAddressingMode 0x10
#define SetLowerColumnStartAddressForPageAddressingMode
#define SetPageAddress 0x22
#define SetDisplayStartLine 0x40
#define SetMultiplexRatio 0xA8
#define SetDisplayOffset 0xD3
#define SetColumnAddress 0x21
#define SetContrastControl 0x81
#define SetDisplayClockDivideRatioOscillatorFrequency 0xD5
#define SetPrechargePeriod 0xD9
#define ContinuousHorizontalScrollSetup 0x26
#define SetVerticalScrollArea 0xA3
#define ContinuousVerticalAndHorizontalScrollSetup 0x29
#define DisplayOn 0xAF
#define COMSCANDEC 0xC8

#define SlaveAddress 0x3C

const uint8_t char1[]={0x1C, 0x00, 0x22, 0xC0, 0x22, 0x40, 0x22, 0x40, 0x1F, 0x80, 0x00, 0x00};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void send_cmd(uint8_t slave_address,uint8_t D, uint8_t cmd, uint8_t *payload, uint16_t length){
	  uint8_t data[1+length+1];
	  data[0]=D;
	  data[1]=cmd;
	  memcpy(&data[2], payload, length);
	  HAL_I2C_Master_Transmit(&hi2c1, slave_address<<1, data, sizeof(data), 10);
}
void send_data(uint8_t slave_address,uint8_t D, uint8_t payload, uint16_t length){
	  uint8_t data[1+length];
	  data[0]=D;
	  memcpy(&data[1], &payload, length);
	  HAL_I2C_Master_Transmit(&hi2c1, slave_address<<1, data, sizeof(data), 10);
}
void ssd1306_init(){
	uint8_t init[][2]={
			{SetDisplayONOFF, 0x00},
			{SetDisplayClockDivideRatioOscillatorFrequency, 0x80},
			{SetMultiplexRatio, 0x3F},
			{SetDisplayOffset, 0x00},
			{SetDisplayStartLine, 0x00},
			{ChargePumpSetting, 0x14},
			{SetMemoryAddressingMode, 0x00},
			{SetSegmentReMap, 0x00},
			{COMSCANDEC, 0x00},
			{SetCOMPinsHardwareConfiguration, 0x12},
			{SetContrastControl, 0xCF},
			{SetPrechargePeriod, 0xF1},
			{SetVCOMHDeselectLevel, 0x40},
			{EntireDisplayON, 0x00},
			{SetNormalInverseDisplay, 0x00},
			{DisplayOn, 0x00}
	};
	for(uint8_t i=0; i<(sizeof(init)/sizeof(init[0])); i++){
		send_cmd(SlaveAddress,0x00, init[i][0], &init[i][1], 1);
	}
}
void ssd1306_fill(uint8_t fill){

    for(uint8_t page=0; page<8; page++){
    		for(uint8_t seg=0; seg<=127; seg++){
    			send_data(SlaveAddress, 0x40, fill, 1);
    		}
    	}
}
void ssd1306_clear(){
	for(uint8_t page=0; page<8; page++){
		for(uint8_t seg=0; seg<= 127; seg++){
			send_data(SlaveAddress, 0x40, 0x00, 1);
		}
	}

}
void ssd1306_set_cursor(uint8_t row, uint8_t col){
	uint8_t page=row/8;
	//uint8_t bit=row%8;
	send_cmd(SlaveAddress, 0x00, SetPageStartAddressForPageAddressingMode+page, 0x00,1);
	send_cmd(SlaveAddress, 0x00, 0x00|(col & 0x0F), 0x00, 1);
	send_cmd(SlaveAddress, 0x00, 0x10|(col>>4), 0x00, 1);

}
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
  MX_CRC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_init();
//  for(uint8_t i=0x01; i<=0xFF; i++){
//  			  ssd1306_fill(i);
//  			  HAL_Delay(200);
//  		  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  ssd1306_clear();
  ssd1306_fill(0x01);
  		  HAL_Delay(100);
  		  ssd1306_fill(0x0F);
  		  HAL_Delay(100);
  		  ssd1306_fill(0xFF);
  		 HAL_Delay(400);
  while (1)
  {
    /* USER CODE END WHILE */

	  for(uint8_t n=0; n<12; n++){
			send_data(SlaveAddress, 0x40, char1[n], 1);
	  }
	  for(uint8_t i=0; i<8; i++){
		  for(uint8_t i=0; i<128; i++){
			  send_data(SlaveAddress, 0x40, 0x00, 1);
		  }
	  }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  hi2c1.Init.ClockSpeed = 400000;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
