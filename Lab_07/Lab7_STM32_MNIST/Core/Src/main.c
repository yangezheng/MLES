/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/unistd.h>

#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
TS_StateTypeDef ui_state;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SET    1
#define RESET  0
#define LCD_LAYER_1 SET
#define LCD_LAYER_0 RESET
#define TRUE SET

/* Resolution of MNINST Input Image */
#define NN_INPUT_IMAGE_WIDTH				28
#define NN_INPUT_IMAGE_HEIGHT				28

/* Resolution of character on screen */
#define LCD_INPUT_IMAGE_WIDTH				240
#define LCD_INPUT_IMAGE_HEIGHT				240

/* Using 2 layers from datasheet */
#define LCD_FRAME_BUFFER_LAYER0             (LCD_FRAME_BUFFER +0x130000)
#define LCD_FRAME_BUFFER_LAYER1 			LCD_FRAME_BUFFER
#define CONVERTED_FRAME_BUFFER              (LCD_FRAME_BUFFER +0x260000)

/* Defining positions of screen elemnets */
#define DRAW_IMG_X1							10
#define	DRAW_IMG_X2							230

#define DRAW_IMG_Y1							10
#define	DRAW_IMG_Y2							230

#define ERASE_BUTTON_X1						5
#define ERASE_BUTTON_Y1						285
#define ERASE_BUTTON_X2						ERASE_BUTTON_X1 +230
#define ERASE_BUTTON_Y2						ERASE_BUTTON_Y1 + 30

#define IMG_SCALED_X_SHIFT					200
#define IMG_SCALED_Y_SHIFT					250

#define  PXL_SET							0.99

// TODO: LAB7: Add USART prototype for printf (described in lecture 9 materials)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void user_interface_init(void);
void user_interface_reset();
void touch_sensor_init();
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
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */
  printf("[MAIN] Application running \r\n");

  	 // TODO: LAB7: Initialize BUTTON_KEY. Use BSP_PB_Init() function.


	/* Initialize all peripherals related to LCD and Touch sensor */

	// TODO: LAB7: Initialize touch sensor


	// TODO: LAB7: Initialize LCD and reset it
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
    // TODO: LAB7: Get touchscreen detection state. Use function from stm32f429i_discovery_ts.c. use ui_state type definition to store the result. (Defined on line 46).

    		if (ui_state.TouchDetected) {

    			if ((ui_state.X > DRAW_IMG_X1 && ui_state.X < DRAW_IMG_X2) && (ui_state.Y > DRAW_IMG_Y1 && ui_state.Y < DRAW_IMG_Y2)) {
    				// TODO: LAB7: Draw pixels to the points on the scren where screen is touched


    				// Draw same object to the small screen
    				int x = ui_state.Y * ((float) 28 / 240);
    				int y = ui_state.X * ((float) 28 / 240);

    				BSP_LCD_DrawPixel(y + 1 + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x + 1, 255);
    				BSP_LCD_DrawPixel(y - 1 + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x - 1, 255);
    				BSP_LCD_DrawPixel(y + 1 + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x, 255);
    				BSP_LCD_DrawPixel(y - 1 + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x, 255);

    				BSP_LCD_DrawPixel(y + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x, 255);
    				BSP_LCD_DrawPixel(y + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x + 1, 255);
    				BSP_LCD_DrawPixel(y + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x - 1, 255);
    				BSP_LCD_DrawPixel(y + 1 + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x - 1, 255);
    				BSP_LCD_DrawPixel(y - 1 + IMG_SCALED_X_SHIFT, IMG_SCALED_Y_SHIFT + x + 1, 255);

    				printf("X =%d Y=%d\r\n", ui_state.X, ui_state.Y);

    			} else if ((ui_state.X > ERASE_BUTTON_X1 && ui_state.X < ERASE_BUTTON_X2) && (ui_state.Y > ERASE_BUTTON_Y1 && ui_state.Y < ERASE_BUTTON_Y2)) {
    				// TODO: LAB7: When erase button is pressed reset user interface.
    			}

    		}
    		HAL_Delay(1);

    		// TODO: LAB7: Print "NN processing..." with printf if button BUTTON_KEY pressed

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 *
 * @brief: TLCD INIT Implementation
 * @Param: Board Support Package
 *
 */
void user_interface_init(void)
{
	// Initialize LCD. Use functions from stm32f429i_discovery_lcd.h
	BSP_LCD_Init();

	// Initialize LCD Layer 1 (background layer) with FB address that points to layer 1
	BSP_LCD_LayerDefaultInit(LCD_LAYER_1, LCD_FRAME_BUFFER_LAYER1);

	// Select layer 1
	BSP_LCD_SelectLayer(LCD_LAYER_1);

	// Clear LCD (color red)
	BSP_LCD_Clear(LCD_COLOR_RED);

	// Set color keyring for layer 1 (color dark cyan)
	BSP_LCD_SetColorKeying(LCD_LAYER_1, LCD_COLOR_DARKCYAN);

	// Set layer 1 visibility to disabled
	BSP_LCD_SetLayerVisible(LCD_LAYER_1, DISABLE);

	// Initialize Layer 0 (foreground or visible layer) with FB address that points to layer 0
	BSP_LCD_LayerDefaultInit(LCD_LAYER_0, LCD_FRAME_BUFFER_LAYER0);

	// Select Layer 0
	BSP_LCD_SelectLayer(LCD_LAYER_0);

	// Switch LCD display on
	BSP_LCD_DisplayOn();

	HAL_Delay(200);

	// Clear the whole LCD with color lightcyan
	BSP_LCD_Clear(LCD_COLOR_LIGHTCYAN);

	HAL_Delay(100);

	printf("LCD initialization completed \r\n");
}



/**
 *
 * @brief: User Interface Control Function Implementation
 * @Param: Board Support Package
 *
 */
void user_interface_reset(void) {
	// Clear LCD with light cyan color. Possible color definitions in stm32f429i_discovery_lcd.h
	BSP_LCD_Clear(LCD_COLOR_LIGHTCYAN);

	// Set text color to black
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	// set font size 16
	BSP_LCD_SetFont(&Font16);

	// Set background color (lightcyan)
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTCYAN);

	// Write prediction output string "NN OUTPUT" to position x,y = (5, 255)
	BSP_LCD_DisplayStringAt(5,  255,  (uint8_t*) "NN OUTPUT: ", LEFT_MODE);

	// Draw rectangle to mark prediction output area. Marked with "B" on image.
	// Position x,y = (200, 250) and dimensions = (28, 28)
	BSP_LCD_DrawRect(200, 250, 28, 28);

	// Set text color to blue
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

	// Write clear button area, marked with "C". Draw filled rectangle to position x,y = (5, 285) with dimensions = (230, 30)
	BSP_LCD_FillRect(5, 285, 230, 30);

	// Set text color to white
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	// Set font size to 16
	BSP_LCD_SetFont(&Font16);

	// Set background color as blue
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

	// Display string "CLEAR" to position x,y = (330, 291)
	BSP_LCD_DisplayStringAt(330, 291, (uint8_t*) "CLEAR", LEFT_MODE);

	// Draw minimized user drawing area, marked with "D" on figure. Draw rectangle to position x,y = (2,2) and dimensions (237,237)
	BSP_LCD_DrawRect(2, 2, 237, 237);

	// Set text color as black
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	printf("LCD Interface INIT successful \r\n");
}

/**
 *
 * @brief: Touch Sensor INIT Implementation
 * @Param: Board Support Package
 *
 */
void touch_sensor_init(void) {
	// Initialize touch sensor to use a whole LCD area (240x320) and print to UART whether the initialization was successful or not
	if(BSP_TS_Init(240, 320) == TS_ERROR) {
		printf("Touch sensor initialization failed \r\n");
	} else{
		printf("Touch sensor initialization successful \r\n");
	}
	HAL_Delay(50);
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
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
