#include "lcd.h"
#include "gui.h"
#include "ShowLCD.h"
#include "icon.h"
#include "stm32f1xx_hal.h"                  
#include "pic.h"
#include <stdio.h>
#include "main.h"

extern uint16_t in_cnt;
extern uint16_t out_cnt;
extern int mq4_1,mq4_2 ,mq4_3 ,Fire1 ,Fire2 ,Fire3;
int Show_LCD_FLAG=0;
int refresh_lcd_1s_time_handler = 0;
void DrawTestPage(uint8_t *str)
{
LCD_Clear(WHITE);
LCD_Fill(0,0,lcddev.width,20,BLUE);
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=BLACK;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"https://www.wiznet.io/",16,1);
}

void show_lcd(void)
{
	LCD_direction(3);
	DrawTestPage("Intelligent Building System");	
	Gui_Drawbmp16(480-32,21,gImage_icon);
	LCD_Fill(0,136,480,136,BLACK);
	LCD_Fill(0,216,480,216,BLACK);
	LCD_Fill(0,56,480,56,BLACK);
	LCD_Fill(480/2-10,20,480/2-10,56,BLACK);
	LCD_ShowString(10,30,16,"Entry count:0000",0);
	LCD_ShowString(480/2,30,16,"Outgoing count:0000",0);
	
	Gui_StrCenter(0,66,BLACK,BLUE,"Zone 1",16,1);
	LCD_ShowString(10,86,16,"Combustible Gas:xxxppm",0);
	LCD_ShowString(10,106+5,16,"Flame Detector :Normal",0);
	LCD_ShowString(480/2,86,16,"Air Exchange System:OFF",0);
	LCD_ShowString(480/2,106+5,16,"Fire Extinguishing System:OFF",0);
	
	Gui_StrCenter(0,146,BLACK,BLUE,"Zone 2",16,1);
	LCD_ShowString(10,166,16,"Combustible Gas:xxxppm",0);
	LCD_ShowString(10,186+5,16,"Flame Detector :Normal",0);
	LCD_ShowString(480/2,166,16,"Air Exchange System:OFF",0);
	LCD_ShowString(480/2,186+5,16,"Fire Extinguishing System:OFF",0);
	
	Gui_StrCenter(0,226,BLACK,BLUE,"Zone 3",16,1);
	LCD_ShowString(10,246,16,"Combustible Gas:xxxppm",0);
	LCD_ShowString(10,266+5,16,"Flame Detector :Normal",0);
	LCD_ShowString(480/2,246,16,"Air Exchange System:OFF",0);
	LCD_ShowString(480/2,266+5,16,"Fire Extinguishing System:OFF",0);
	LCD_direction(USE_HORIZONTAL);
	HAL_Delay(1500);		
	HAL_Delay(1500);
}

void clear_lcd(void)
{
	LCD_direction(3);
	LCD_Fill(135,86,230,130,WHITE);
	LCD_Fill(135,166,230,210,WHITE);
	LCD_Fill(135,246,230,290,WHITE);
	LCD_direction(USE_HORIZONTAL);
}

void update_lcd(void)
{
	if(Show_LCD_FLAG==1)
	{
		//clear_lcd();
		LCD_direction(3);
		Show_LCD_FLAG=0;
		uint8_t temp[128] = {0};
		sprintf((char*)temp,"Entry count:%04d",in_cnt);
		LCD_ShowString(10,30,16,temp,0);
		sprintf((char*)temp,"Outgoing count:%04d",out_cnt);
		LCD_ShowString(480/2,30,16,temp,0);

		sprintf((char*)temp,"Combustible Gas:%05dppm",mq4_1);
		LCD_ShowString(10,86,16,temp,0);
		sprintf((char*)temp,"Flame Detector :%s",Fire1 == 1?"Normal":"Warning");
		LCD_ShowString(10,106+5,16,temp,0);
		sprintf((char*)temp,"Air Exchange System:%s",HAL_GPIO_ReadPin(RELAY1_GPIO_Port,RELAY1_Pin) == 0?" ON":"OFF");
		LCD_ShowString(480/2,86,16,temp,0);
		sprintf((char*)temp,"Fire Extinguishing System:%s",HAL_GPIO_ReadPin(RELAY4_GPIO_Port,RELAY4_Pin) == 0?" ON":"OFF");
		LCD_ShowString(480/2,106+5,16,temp,0);

		sprintf((char*)temp,"Combustible Gas:%05dppm",mq4_2);
		LCD_ShowString(10,166,16,temp,0);
		sprintf((char*)temp,"Flame Detector :%s",Fire2 == 1?"Normal":"Warning");
		LCD_ShowString(10,186+5,16,temp,0);
		sprintf((char*)temp,"Air Exchange System:%s",HAL_GPIO_ReadPin(RELAY2_GPIO_Port,RELAY2_Pin) == 0?" ON":"OFF");
		LCD_ShowString(480/2,166,16,temp,0);
		sprintf((char*)temp,"Fire Extinguishing System:%s",HAL_GPIO_ReadPin(RELAY5_GPIO_Port,RELAY5_Pin) == 0?" ON":"OFF");
		LCD_ShowString(480/2,186+5,16,temp,0);

		sprintf((char*)temp,"Combustible Gas:%05dppm",mq4_3);
		LCD_ShowString(10,246,16,temp,0);
		sprintf((char*)temp,"Flame Detector :%s",Fire3 == 1?"Normal":"Warning");
		LCD_ShowString(10,266+5,16,temp,0);
		sprintf((char*)temp,"Air Exchange System:%s",HAL_GPIO_ReadPin(RELAY3_GPIO_Port,RELAY3_Pin) == 0?" ON":"OFF");
		LCD_ShowString(480/2,246,16,temp,0);
		sprintf((char*)temp,"Fire Extinguishing System:%s",HAL_GPIO_ReadPin(RELAY6_GPIO_Port,RELAY6_Pin) == 0?" ON":"OFF");		
		LCD_ShowString(480/2,266+5,16,temp,0);
		LCD_direction(USE_HORIZONTAL);
	}
}

void refresh_lcd(void)
{
	if(refresh_lcd_1s_time_handler >= 60)
	{
		refresh_lcd_1s_time_handler = 0;
		LCD_Init();
		show_lcd();
		update_lcd();
	}
}
