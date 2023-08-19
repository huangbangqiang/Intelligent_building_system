#include "HW_key.h"
#include <stdio.h>
#include "main.h"
#include "relay.h"
#include "w5300_config.h"

uint8_t KEY_FLAG = 0;
uint8_t key = 0;
void key_scanf(void)
{
		if(HAL_GPIO_ReadPin(BTN1_GPIO_Port,BTN1_Pin)==0)
		{
				HAL_Delay(10);
				if(HAL_GPIO_ReadPin(BTN1_GPIO_Port,BTN1_Pin)==0)
				{
					KEY_FLAG = 1;
					key = 1;
					while(!HAL_GPIO_ReadPin(BTN1_GPIO_Port,BTN1_Pin));
				}
		}
		if(HAL_GPIO_ReadPin(BTN2_GPIO_Port,BTN2_Pin)==0)
		{
			HAL_Delay(10);
			if(HAL_GPIO_ReadPin(BTN2_GPIO_Port,BTN2_Pin)==0)
			{
					KEY_FLAG = 1;
					key = 2;
					while(!HAL_GPIO_ReadPin(BTN2_GPIO_Port,BTN2_Pin));
			}
		}
		if(HAL_GPIO_ReadPin(BTN3_GPIO_Port,BTN3_Pin)==0)
		{
				HAL_Delay(10);
				if(HAL_GPIO_ReadPin(BTN3_GPIO_Port,BTN3_Pin)==0)
				{
					KEY_FLAG = 1;
					key = 3;
					while(!HAL_GPIO_ReadPin(BTN3_GPIO_Port,BTN3_Pin));
				}
		}
    HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,(GPIO_PinState)0);
    if(KEY1_Input==0 || KEY2_Input==0 || KEY3_Input==0)
    {
        HAL_Delay(10);
        if(KEY1_Input == 0)
        {
            KEY_FLAG = 1;
            key = 4;
            while(!HAL_GPIO_ReadPin(C1_GPIO_Port,C1_Pin));
        }
        else if(KEY2_Input == 0)
        {
            KEY_FLAG = 1;
            key = 5;
            while(!HAL_GPIO_ReadPin(C2_GPIO_Port,C2_Pin));
        }
        else if(KEY3_Input == 0)
        {
            KEY_FLAG = 1;
            key = 6;
            while(!HAL_GPIO_ReadPin(C3_GPIO_Port,C3_Pin));
        }
        else
        {
            KEY_FLAG = 0;
            HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,(GPIO_PinState)1);
        }
    }
}

void Detection_key(void)
{
    key_scanf();
    if(KEY_FLAG == 1)
    {
        KEY_FLAG = 0;
        switch (key)
        {
        case 1:
            relay_on(RELAY4_Pin);
		    HAL_GPIO_WritePin(BUZZER1_GPIO_Port,BUZZER1_Pin,GPIO_PIN_RESET);
		    send_aliyun("{id:1,params:{Zone1_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
            printf("{id:1,params:{Zone1_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}\r\n");
            break;
        case 2:
            relay_on(RELAY5_Pin);
		    HAL_GPIO_WritePin(BUZZER2_GPIO_Port,BUZZER2_Pin,GPIO_PIN_RESET);
		    send_aliyun("{id:1,params:{Zone2_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
            printf("{id:1,params:{Zone2_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}\r\n");
            break;
        case 3:
            relay_on(RELAY6_Pin);
		    HAL_GPIO_WritePin(BUZZER3_GPIO_Port,BUZZER3_Pin,GPIO_PIN_RESET);
		    send_aliyun("{id:1,params:{Zone3_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}");
            printf("{id:1,params:{Zone3_Status:\"Warning\"},version:\"1.0\",method:\"thing.event.property.post\"}\r\n");
            break;
        case 4:
            relay_off(RELAY1_Pin);
            relay_off(RELAY4_Pin);
		    HAL_GPIO_WritePin(BUZZER1_GPIO_Port,BUZZER1_Pin,GPIO_PIN_SET);
		    send_aliyun("{id:1,params:{Zone1_Status:\"Normal\"},version:\"1.0\",method:\"thing.event.property.post\"}");
            printf("{id:1,params:{Zone1_Status:\"Normal\"},version:\"1.0\",method:\"thing.event.property.post\"}\r\n");
            break;
        case 5:
            relay_off(RELAY2_Pin);
            relay_off(RELAY5_Pin);
		    HAL_GPIO_WritePin(BUZZER2_GPIO_Port,BUZZER2_Pin,GPIO_PIN_SET);
		    send_aliyun("{id:1,params:{Zone2_Status:\"Normal\"},version:\"1.0\",method:\"thing.event.property.post\"}");
            printf("{id:1,params:{Zone2_Status:\"Normal\"},version:\"1.0\",method:\"thing.event.property.post\"}\r\n");
            break;
        case 6:
            relay_off(RELAY3_Pin);
            relay_off(RELAY6_Pin);
		    HAL_GPIO_WritePin(BUZZER3_GPIO_Port,BUZZER3_Pin,GPIO_PIN_SET);
		    send_aliyun("{id:1,params:{Zone3_Status:\"Normal\"},version:\"1.0\",method:\"thing.event.property.post\"}");
            printf("{id:1,params:{Zone3_Status:\"Normal\"},version:\"1.0\",method:\"thing.event.property.post\"}\r\n");
            break;
        
        default:
            break;
        }
    }
}
