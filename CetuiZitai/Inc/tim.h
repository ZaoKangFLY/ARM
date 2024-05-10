/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN Private defines */
#define  Basic_htim                   htim6
#define  Ce_PWM_htim    		      htim3
#define  Ce1_CHANNEL                  TIM_CHANNEL_1
#define  Ce2_CHANNEL                  TIM_CHANNEL_2
	
#define  Ce1_PWM_ENABLE()             do{HAL_TIM_PWM_Start(&Ce_PWM_htim,Ce1_CHANNEL);}while(0)
#define  Ce2_PWM_ENABLE()             do{HAL_TIM_PWM_Start(&Ce_PWM_htim,Ce2_CHANNEL);}while(0)
#define  Ce1_SETCOMPAER(ChannelPulse)          do{__HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce1_CHANNEL,ChannelPulse);}while(0)    // 设置比较寄存器的值
#define  Ce2_SETCOMPAER(ChannelPulse)          do{ __HAL_TIM_SET_COMPARE(&Ce_PWM_htim , Ce2_CHANNEL,ChannelPulse);}while(0) 

#define  Basic_TIM_ENABLE()            do{HAL_TIM_Base_Start_IT(&Basic_htim);}while(0)
/* USER CODE END Private defines */

void MX_TIM3_Init(void);
void MX_TIM6_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
float Count(float x);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

