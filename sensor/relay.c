#include "stm32f1xx_hal.h"                  // Device header
#include "gpio.h"
#include "relay.h"
#include "w5300_config.h"
#include "tim.h"
void relay_on(uint16_t pin)
{
	switch (pin)
	{
	case RELAY1_Pin:
		HAL_GPIO_WritePin(RELAY1_GPIO_Port, pin, GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Relay1:\"on\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY2_Pin:
		HAL_GPIO_WritePin(RELAY2_GPIO_Port, pin, GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Relay2:\"on\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY3_Pin:
		HAL_GPIO_WritePin(RELAY3_GPIO_Port, pin, GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Relay3:\"on\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY4_Pin:
		HAL_GPIO_WritePin(RELAY4_GPIO_Port, pin, GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Relay4:\"on\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY5_Pin:
		HAL_GPIO_WritePin(RELAY5_GPIO_Port, pin, GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Relay5:\"on\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY6_Pin:
		HAL_GPIO_WritePin(RELAY6_GPIO_Port, pin, GPIO_PIN_RESET);
		send_aliyun("{id:1,params:{Relay6:\"on\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	
	default:
		break;
	}
}

void relay_off(uint16_t pin)
{
	switch (pin)
	{
	case RELAY1_Pin:
		HAL_GPIO_WritePin(RELAY1_GPIO_Port, pin, GPIO_PIN_SET);
		send_aliyun("{id:1,params:{Relay1:\"off\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY2_Pin:
		HAL_GPIO_WritePin(RELAY2_GPIO_Port, pin, GPIO_PIN_SET);
		send_aliyun("{id:1,params:{Relay2:\"off\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY3_Pin:
		HAL_GPIO_WritePin(RELAY3_GPIO_Port, pin, GPIO_PIN_SET);
		send_aliyun("{id:1,params:{Relay3:\"off\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY4_Pin:
		HAL_GPIO_WritePin(RELAY4_GPIO_Port, pin, GPIO_PIN_SET);
		send_aliyun("{id:1,params:{Relay4:\"off\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY5_Pin:
		HAL_GPIO_WritePin(RELAY5_GPIO_Port, pin, GPIO_PIN_SET);
		send_aliyun("{id:1,params:{Relay5:\"off\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	case RELAY6_Pin:
		HAL_GPIO_WritePin(RELAY6_GPIO_Port, pin, GPIO_PIN_SET);
		send_aliyun("{id:1,params:{Relay6:\"off\"},version:\"1.0\",method:\"thing.event.property.post\"}");
		break;
	
	default:
		break;
	}
}



