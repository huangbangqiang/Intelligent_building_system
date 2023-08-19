/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define RELAY2_Pin GPIO_PIN_2
#define RELAY2_GPIO_Port GPIOE
#define RELAY3_Pin GPIO_PIN_3
#define RELAY3_GPIO_Port GPIOE
#define FIRE1_Pin GPIO_PIN_0
#define FIRE1_GPIO_Port GPIOC
#define FIRE2_Pin GPIO_PIN_1
#define FIRE2_GPIO_Port GPIOC
#define FIRE3_Pin GPIO_PIN_2
#define FIRE3_GPIO_Port GPIOC
#define RELAY4_Pin GPIO_PIN_6
#define RELAY4_GPIO_Port GPIOA
#define RELAY5_Pin GPIO_PIN_7
#define RELAY5_GPIO_Port GPIOA
#define RELAY6_Pin GPIO_PIN_0
#define RELAY6_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_10
#define LCD_RS_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_13
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_MISO_Pin GPIO_PIN_14
#define LCD_MISO_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_15
#define LCD_MOSI_GPIO_Port GPIOB
#define BTN1_Pin GPIO_PIN_11
#define BTN1_GPIO_Port GPIOD
#define BTN2_Pin GPIO_PIN_12
#define BTN2_GPIO_Port GPIOD
#define BTN3_Pin GPIO_PIN_13
#define BTN3_GPIO_Port GPIOD
#define C3_Pin GPIO_PIN_2
#define C3_GPIO_Port GPIOG
#define C2_Pin GPIO_PIN_3
#define C2_GPIO_Port GPIOG
#define C1_Pin GPIO_PIN_4
#define C1_GPIO_Port GPIOG
#define R1_Pin GPIO_PIN_5
#define R1_GPIO_Port GPIOG
#define BUZZER1_Pin GPIO_PIN_6
#define BUZZER1_GPIO_Port GPIOG
#define BUZZER2_Pin GPIO_PIN_7
#define BUZZER2_GPIO_Port GPIOG
#define BUZZER3_Pin GPIO_PIN_8
#define BUZZER3_GPIO_Port GPIOG
#define IN_COUNT_Pin GPIO_PIN_6
#define IN_COUNT_GPIO_Port GPIOB
#define IN_COUNT_EXTI_IRQn EXTI9_5_IRQn
#define OUT_COUNT_Pin GPIO_PIN_7
#define OUT_COUNT_GPIO_Port GPIOB
#define OUT_COUNT_EXTI_IRQn EXTI9_5_IRQn
#define LCD_LED_Pin GPIO_PIN_8
#define LCD_LED_GPIO_Port GPIOB
#define LINK_STATUS_Pin GPIO_PIN_9
#define LINK_STATUS_GPIO_Port GPIOB
#define RST_Pin GPIO_PIN_0
#define RST_GPIO_Port GPIOE
#define RELAY1_Pin GPIO_PIN_1
#define RELAY1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
