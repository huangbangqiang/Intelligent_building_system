#ifndef _RELAY_H_
#define _RELAY_H_
#include "stm32f1xx_hal.h"                  // Device header

#ifdef __cplusplus
extern "C" {
#endif

void relay_on(uint16_t pin);
void relay_off(uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif   
