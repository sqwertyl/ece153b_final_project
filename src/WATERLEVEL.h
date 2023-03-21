#ifndef __STM32L476R_NUCLEO_WATERLEVEL_H
#define __STM32L476R_NUCLEO_WATERLEVEL_H

#include "stm32l476xx.h"


void Trigger_Setup(void);
void Input_Capture_Setup(void);
int get_waterlevel(void);

void TIM4_IRQHandler(void);

#endif
