#include "stm32f1xx_hal.h"    
#include <stdio.h>
#include "mq4.h"
#include "adc.h"
#include "w5300_config.h"
#include "relay.h"
#include "dhcp.h"
#include "bsp_flash.h"



extern uint16_t in_cnt;
extern uint16_t out_cnt;
uint32_t adc_value[6];
uint32_t sensor_1s_tick=0;
extern uint8_t dns_get_ip_flag;
extern uint8_t retval;
int mq4_1,mq4_2 ,mq4_3 ,Fire1 ,Fire2 ,Fire3;
uint32_t send_time=0;
void ADC_Get_Value(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
	for(size_t i = 0; i < NUM_ADC_CHANNEL;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,10);
		adc_value[i] = HAL_ADC_GetValue(&hadc1);
//		if(adc_value[i]<621)
//			adc_value[i] = 621;
	}
	HAL_ADC_Stop(&hadc1);
	Fire1 = HAL_GPIO_ReadPin(FIRE1_GPIO_Port,FIRE1_Pin);
	Fire2 = HAL_GPIO_ReadPin(FIRE2_GPIO_Port,FIRE2_Pin);
	Fire3 = HAL_GPIO_ReadPin(FIRE3_GPIO_Port,FIRE3_Pin);
	
	return ;
}

// void test_get_adc(void)
// {
// 	ADC_Get_Value();
// 	mq4_1 = (int)((adc_value[0]*3.3f/4096-0.5)/0.1*200);
// 	mq4_2 = (int)((adc_value[1]*3.3f/4096-0.5)/0.1*200);
// 	mq4_3 = (int)((adc_value[2]*3.3f/4096-0.5)/0.1*200);
// 	if(mq4_1 < 0)
// 	{
// 		mq4_1 = 0;
// 	}
// 	if(mq4_2 < 0)
// 	{
// 		mq4_2 = 0;
// 	}
// 	if(mq4_3 < 0)
// 	{
// 		mq4_3 = 0;
// 	}
// 	printf("mq4_1:%.4f\r\nmq4_1_ppm:%d\r\n",adc_value[0]*3.3f/4096,mq4_1);
// 	printf("mq4_2:%.4f\r\nmq4_2_ppm:%d\r\n",adc_value[1]*3.3f/4096,mq4_2);
// 	printf("mq4_3:%.4f\r\nmq4_3_ppm:%d\r\n\r\n",adc_value[2]*3.3f/4096,mq4_3);
// }

void detection_mq(void)
{
	uint16_t mq4_1_max = 0;
	uint16_t mq4_2_max = 0;
	uint16_t mq4_3_max = 0;
	char temp[256] = {0};
	STMFLASH_Read(MQ4_1_ADDR,&mq4_1_max,2);
	STMFLASH_Read(MQ4_2_ADDR,&mq4_2_max,2);
	STMFLASH_Read(MQ4_3_ADDR,&mq4_3_max,2);
	
	ADC_Get_Value();
	mq4_1 = (int)((adc_value[0]*3.3f/4096-0.88)/0.1*200);
	mq4_2 = (int)((adc_value[1]*3.3f/4096-1.12)/0.1*200);
	mq4_3 = (int)((adc_value[2]*3.3f/4096-1.4)/0.1*200);
	if(mq4_1<0)
		mq4_1 = 0;
	if(mq4_2<0)
		mq4_2 = 0;
	if(mq4_3<0)
		mq4_3 = 0;
	
	if(mq4_1 > mq4_1_max && HAL_GPIO_ReadPin(RELAY1_GPIO_Port,RELAY1_Pin))
	{
		relay_on(RELAY1_Pin);
		sprintf(temp,"{id:1,params:{MQ4_1:%d,Zone1_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}",mq4_1);
		send_aliyun(temp);
		printf(temp);
		printf("\r\n");
	}
	if(mq4_2 > mq4_2_max && HAL_GPIO_ReadPin(RELAY2_GPIO_Port,RELAY2_Pin))
	{
		relay_on(RELAY2_Pin);
		sprintf(temp,"{id:1,params:{MQ4_2:%d,Zone2_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}",mq4_2);
		send_aliyun(temp);
		printf(temp);
		printf("\r\n");
	}
	if(mq4_3 > mq4_3_max && HAL_GPIO_ReadPin(RELAY3_GPIO_Port,RELAY3_Pin))
	{
		relay_on(RELAY3_Pin);
		sprintf(temp,"{id:1,params:{MQ4_3:%d,Zone3_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}",mq4_3);
		send_aliyun(temp);
		printf(temp);
		printf("\r\n");
	}
	if(Fire1 == 0 && HAL_GPIO_ReadPin(RELAY4_GPIO_Port,RELAY4_Pin))
	{
		relay_on(RELAY4_Pin);
		HAL_GPIO_WritePin(BUZZER1_GPIO_Port,BUZZER1_Pin,GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Fire_1:\"Warning\",Zone1_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		printf("{id:1,params:{Fire_1:\"Warning\",Zone1_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		printf("\r\n");
	}
	if(Fire2 == 0 && HAL_GPIO_ReadPin(RELAY5_GPIO_Port,RELAY5_Pin))
	{
		relay_on(RELAY5_Pin);
		HAL_GPIO_WritePin(BUZZER1_GPIO_Port,BUZZER2_Pin,GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Fire_2:\"Warning\",Zone2_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		printf("{id:1,params:{Fire_2:\"Warning\",Zone2_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		printf("\r\n");
	}
	if(Fire3 == 0 && HAL_GPIO_ReadPin(RELAY6_GPIO_Port,RELAY6_Pin))
	{

		relay_on(RELAY6_Pin);
		HAL_GPIO_WritePin(BUZZER1_GPIO_Port,BUZZER3_Pin,GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Fire_3:\"Warning\",Zone3_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		printf("{id:1,params:{Fire_3:\"Warning\",Zone3_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		printf("\r\n");
	}
}

void sensor_time_send(void)
{
	int ret;
	if((dns_get_ip_flag == 1) && (retval == DHCP_IP_LEASED) && (send_time >= 5))
	{
		send_time = 0;
		char temp[256] = {0};
		sprintf(temp,"{id:1,params:{MQ4_1:%d,MQ4_2:%d,MQ4_3:%d,Fire_1:\"%s\",Fire_2:\"%s\",Fire_3:\"%s\",IN_COUNT:%d,OUT_COUNT:%d},version:\"1.0\",method:\"thing.event.property.post\"}",mq4_1,mq4_2,mq4_3,Fire1 == 1?"Normal":"Warning",Fire2 == 1?"Normal":"Warning",Fire3 == 1?"Normal":"Warning",in_cnt,out_cnt);
		ret = send_aliyun(temp);
		if(ret == 0)
			printf("tx:%s\r\n",temp);
	}
}

void sensor_1s_time_handler(void)
{
	send_time++;
}
