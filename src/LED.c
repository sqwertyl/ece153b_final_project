/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	
	// Initialize Green LED
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER5_1;
	GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_5;
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
}

void Green_LED_Off(void) {
	GPIOA->ODR &= ~GPIO_OTYPER_ODR_5;
}

void Green_LED_On(void) {
	GPIOA->ODR |= GPIO_OTYPER_ODR_5;
}

void Green_LED_Toggle(void) {
	GPIOA->ODR ^= GPIO_OTYPER_ODR_5;
}
