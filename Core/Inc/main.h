/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DOG_Pin GPIO_PIN_13
#define DOG_GPIO_Port GPIOC
#define OLED_RST_Pin GPIO_PIN_15
#define OLED_RST_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_0
#define OLED_DC_GPIO_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_1
#define OLED_CS_GPIO_Port GPIOA
#define DHT11_DI_Pin GPIO_PIN_2
#define DHT11_DI_GPIO_Port GPIOA
#define EPD_PWR_Pin GPIO_PIN_3
#define EPD_PWR_GPIO_Port GPIOA
#define EPD_CS_Pin GPIO_PIN_4
#define EPD_CS_GPIO_Port GPIOA
#define OLED_SCK_Pin GPIO_PIN_5
#define OLED_SCK_GPIO_Port GPIOA
#define EPD_BUSY_Pin GPIO_PIN_6
#define EPD_BUSY_GPIO_Port GPIOA
#define OLED_MOSI_Pin GPIO_PIN_7
#define OLED_MOSI_GPIO_Port GPIOA
#define EPD_DC_Pin GPIO_PIN_0
#define EPD_DC_GPIO_Port GPIOB
#define EPD_RST_Pin GPIO_PIN_1
#define EPD_RST_GPIO_Port GPIOB
#define SGP_SCL_Pin GPIO_PIN_10
#define SGP_SCL_GPIO_Port GPIOB
#define SGP_SDA_Pin GPIO_PIN_11
#define SGP_SDA_GPIO_Port GPIOB
#define EPD_SCK_Pin GPIO_PIN_13
#define EPD_SCK_GPIO_Port GPIOB
#define EPD_MOSI_Pin GPIO_PIN_15
#define EPD_MOSI_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
// e-papaer
//#define RST_Pin EPD_RST_Pin
//#define RST_GPIO_Port EPD_RST_GPIO_Port
//#define DC_Pin EPD_DC_Pin
//#define DC_GPIO_Port EPD_DC_GPIO_Port
//#define BUSY_Pin EPD_BUSY_Pin
//#define BUSY_GPIO_Port EPD_BUSY_GPIO_Port
//#define SPI_CS_Pin EPD_CS_Pin
//#define SPI_CS_GPIO_Port EPD_CS_GPIO_Port
//#define PWR_Pin EPD_PWR_Pin
//#define PWR_GPIO_Port EPD_PWR_GPIO_Port

//#define OLED_ENABLE
//#define EPD_ENABLE

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
