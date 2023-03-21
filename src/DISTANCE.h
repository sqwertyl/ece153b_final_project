#ifndef __STM32L476R_NUCLEO_DISTANCE_H
#define __STM32L476R_NUCLEO_DISTANCE_H

#include "stm32l476xx.h"

void ULTRASONIC_Init(void);

void Sensor1_Init(void);
void Sensor1_GPIO_Init(void);
void Sensor1_TIM_Init(void);

void Sensor2_Init(void);
void Sensor2_GPIO_Init(void);
void Sensor2_TIM_Init(void);


int get_Distance_S1(void);
int get_Distance_S2(void);

void TIM4_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif
