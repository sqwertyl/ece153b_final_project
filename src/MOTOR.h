#ifndef __STM32L476R_NUCLEO_MOTOR_H
#define __STM32L476R_NUCLEO_MOTOR_H

#include "stm32l476xx.h"

enum DIR{
	FORWARD, BACKWARD,
	RIGHT, LEFT, NONE,
};



void MOTOR_Init(void);

void GPIO_Init_R(void);
void GPIO_Init_L(void);
void GPIO_Air(void);

void move_robot(enum DIR dir);
void enable_pump(uint8_t enable);


#endif