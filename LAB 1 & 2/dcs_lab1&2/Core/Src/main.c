/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint16_t counter = 0;
uint16_t counter_time = 0;
uint16_t counter_db = 0;
uint16_t v_db = 0;
uint16_t ccr_db = 0;
uint16_t speed=0;
uint16_t ccr_f = 0;
float kp=0;
int pre=0;
int ar=0;
int ta=0;
typedef enum{
	INIT = 0,
	IDLE,
	DB,
	DEL,
	K,
	END,
	T0,
	T
}
lock_state_t;
lock_state_t state = INIT;

typedef enum{
	OK = 0,
	ERR
} result_t;

result_t init(void)
{
	return OK;
}

result_t idle(void)
{
	return OK;
}

result_t db(void)
{
	if(counter_time == 0)
	{
		TIM11->CCR1 = TIM11->CCR1 + 100;
		return ERR;
	}
	else
	{
		ccr_db = TIM11->CCR1;
		v_db = counter_time;
		return OK;
	}
}

result_t del(void)
{
	return OK;
}

result_t k(void)
{
	return OK;
}
result_t t0(void)
{
	return OK;
}
result_t t(void)
{
	return OK;
}

int dir = 0;
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
  MX_TIM6_Init();
  MX_TIM11_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
  TIM11->CCR1 = 10;

  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  switch(state)
	 	  {
	 	  	  case INIT:
	 	  		  if(init() == OK)
	 	  		  {
	 	  			  state = IDLE;
	 	  		  }
	 	  		  break;
	 	  	  case IDLE:
	 	  		  if(idle() == OK)
	 	  		  {
	 	  			  state = DB;
	 	  		  }
	 	  		  break;
	 	  	  case DB:
	 	  		  break;
	 	  	  case DEL:
	 	  		TIM11->CCR1 = TIM11->CCR1 + 50;
	 	  		  	HAL_Delay(3000);
	 	  			TIM11->CCR1 = TIM11->CCR1 + 250;
	 	  			state = K;
	 	  		  break;
	 	  	  case K:
	 	  		  break;
	 	  	  case T0:
	 	  		  TIM11->CCR1 = ccr_db;
	 	  		  HAL_Delay(3000);
	 	  		  TIM11->CCR1 = ccr_f;
	 	  		  state = T;
	 	  		  break;
	 	  	  case T:
	 	  		  break;
	 	  	  case END:

	 	  		  break;


	 	  }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == A_Pin){
		counter++;
		dir = HAL_GPIO_ReadPin(B_GPIO_Port,B_Pin);
	}
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM6)
	{
	counter_time = (counter*60*100)/1024;
		// counter_time=counter;
		counter=0;

		if (state == T0)
			{
				ta = 0;
			}
			if (state == T)
			{
				if ((counter_time-v_db)<0.63*(speed-v_db))
				{
					ta++;
				}

			}
	}

	if(htim->Instance == TIM7)
	{

		if(state == DB)
		{
			if(db() == OK)
			{
				state = DEL;

			}
		}
		if (state == K)
		{
			if (counter_time-pre<20){
				ccr_f = TIM11->CCR1;
				speed = counter_time;
	 	  		kp = speed - v_db;
	 	  		ar = TIM11->ARR;
	 	  		kp = kp / (((ccr_f - ccr_db )*12)/ar);
				state = T0;
			} else {
			pre = counter_time;
			}
		}

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
