/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOA
#define RTC_SDA_Pin GPIO_PIN_4
#define RTC_SDA_GPIO_Port GPIOA
#define RTC_SCL_Pin GPIO_PIN_5
#define RTC_SCL_GPIO_Port GPIOA
#define RTC_INT_Pin GPIO_PIN_6
#define RTC_INT_GPIO_Port GPIOA
#define RTC_INT_EXTI_IRQn EXTI9_5_IRQn
#define LED_B_Pin GPIO_PIN_14
#define LED_B_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_15
#define LED_G_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_6
#define LED_R_GPIO_Port GPIOC
#define EP_SW_Pin GPIO_PIN_7
#define EP_SW_GPIO_Port GPIOC
#define EP2_CS_Pin GPIO_PIN_8
#define EP2_CS_GPIO_Port GPIOA
#define EPS_DC_Pin GPIO_PIN_9
#define EPS_DC_GPIO_Port GPIOA
#define EP2_RST_Pin GPIO_PIN_10
#define EP2_RST_GPIO_Port GPIOA
#define EP1_CS_Pin GPIO_PIN_3
#define EP1_CS_GPIO_Port GPIOB
#define EP1_SCK_Pin GPIO_PIN_4
#define EP1_SCK_GPIO_Port GPIOB
#define NB_RST_Pin GPIO_PIN_8
#define NB_RST_GPIO_Port GPIOB
#define NB_SW_Pin GPIO_PIN_9
#define NB_SW_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
