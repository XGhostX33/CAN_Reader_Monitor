#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "u8g2.h"

void Error_Handler(void);

#define OLED_CS_Pin         GPIO_PIN_4
#define OLED_CS_GPIO_Port   GPIOA
#define OLED_DC_Pin         GPIO_PIN_8
#define OLED_DC_GPIO_Port   GPIOA
#define OLED_RES_Pin        GPIO_PIN_1
#define OLED_RES_GPIO_Port  GPIOB

#ifdef __cplusplus
}
#endif

#endif
