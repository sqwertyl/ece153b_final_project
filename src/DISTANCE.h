#ifndef __STM32L476R_NUCLEO_DISTANCE_H
#define __STM32L476R_NUCLEO_DISTANCE_H

#include "stm32l476xx.h"

void ULTRASONIC_Init(void);
int get_Distance(void);

void TIM2_IRQHandler(void);

#endif
