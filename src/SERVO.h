#ifndef __STM32L476R_NUCLEO_SERVO_H
#define __STM32L476R_NUCLEO_SERVO_H

#include "stm32l476xx.h"

void SERVO_GPIO_Init(void);

void TIM5_CH1_Init(void);

void SERVO_L90(void);
void SERVO_R90(void);



#endif
