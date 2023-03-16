#include "stm32l476xx.h"
#include "STEPPER.h"
#include "EXTI.h"
#include "SysClock.h"
#include "LED.h"

int main(void) {
	STEPPER_MOTOR_Init();
	LED_Init();
	EXTI_Init();
	
	while (1);
	
	return 0;
	
}
