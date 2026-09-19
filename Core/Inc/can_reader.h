#ifndef INC_CAN_READER_H_
#define INC_CAN_READER_H_

#include "stm32f1xx_hal.h"

void can_reader_Handler(uint32_t id, uint8_t *data);
void can_reader_Init(void);

#endif
