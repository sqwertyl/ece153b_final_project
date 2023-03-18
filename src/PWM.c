#include "PWM.h"

void PWM_Init(void) {
	// Enable GPIO Port A Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Enable TIM2 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	// Configure PA5 -> sets PA5 to alternative mode (10) & high speed output
	GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
	GPIOA->MODER |= GPIO_MODER_MODE5_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	
	// Configure PWM Output for TIM2 CH 1
	TIM2->CR1 &= ~TIM_CR1_DIR; 				// set direction to 0 (upcounting)
	TIM2->PSC = 7;									// prescaler 
	TIM2->ARR = 4096;									// auto reload value = MAX 4096 from ADC measurement
	
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;		// set output compare mode to 0110 (PWM mode 1)
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	
	TIM2->CCER &= ~TIM_CCER_CC1P;			// set compare 1 polarity 0 (active high)
	TIM2->CCER |= TIM_CCER_CC1E;			// enable channel 1 output
	
	TIM2->CR1 |= TIM_CR1_CEN;
}

