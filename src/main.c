#include "stm32l476xx.h"
#include "STEPPER.h"
#include "EXTI.h"
#include "SysClock.h"
#include "LED.h"

int main(void) {
	STEPPER_MOTOR_Init();
	LED_Init();
	EXTI_Init();
	
	// Initialize User Button
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

	//fClockwise(L);
	int count = 0;
	while (1) {
		
	}
	
	return 0;
	
}
