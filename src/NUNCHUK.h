#ifndef __STM32L476G_NUCLEO_NUNCHUK_H
#define __STM32L476G_NUCLEO_NUNCHUK_H

#include "stm32l476xx.h"
#include "STEPPER.h"

void parse_Nunchuk(uint8_t *data);

enum DIR get_Direction(void);
bool get_Z_pressed(void);


#endif