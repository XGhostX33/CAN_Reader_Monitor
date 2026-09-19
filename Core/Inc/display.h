#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "u8g2.h"

void display_UpdateElement(uint8_t index, int16_t value);
void display_Init(u8g2_t *u8g2);

#endif
