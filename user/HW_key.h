#ifndef _HW_KEY_H_
#define _HW_KEY_H_
#include "stm32f1xx_hal.h"                  // Device header

#ifdef __cplusplus
extern "C" {
#endif

#define KEY1_Input HAL_GPIO_ReadPin(C1_GPIO_Port,C1_Pin)
#define KEY2_Input HAL_GPIO_ReadPin(C1_GPIO_Port,C2_Pin)
#define KEY3_Input HAL_GPIO_ReadPin(C1_GPIO_Port,C3_Pin)
#define KEY4_Input HAL_GPIO_ReadPin(C1_GPIO_Port,C4_Pin)
void key_scanf(void);
void Detection_key(void);
#ifdef __cplusplus
}
#endif

#endif   
