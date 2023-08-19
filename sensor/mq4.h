#ifndef _MQ4_H_
#define _MQ4_H_
#include "stm32f1xx_hal.h"                  // Device header

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_ADC_CHANNEL 3
#define RELAY_ON 0
#define RELAY_OFF 1
uint16_t ADC_Read(uint32_t Channel);
void sensor_1s_time_handler(void);
void detection_mq(void);
void sensor_time_send(void);
void test_get_adc(void);
#ifdef __cplusplus
}
#endif

#endif   
