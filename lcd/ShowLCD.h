#ifndef __SHOWLCD_H__
#define __SHOWLCD_H__
#include "stm32f1xx_hal.h"

void DrawTestPage(uint8_t *str);
void show_lcd(void);
void update_lcd(void);
void refresh_lcd(void);
#endif
