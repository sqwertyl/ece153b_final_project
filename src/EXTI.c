#include "EXTI.h"
#include "LED.h"
#include "STEPPER.h"


void EXTI_Init(void) {
	// Initialize User Button
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
	// timer init
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	// Configure SYSCFG EXTI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	// Configure EXTI Trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	EXTI->RTSR1 |= EXTI_RTSR1_RT13;
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	//timer setup
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->PSC = 0;
	TIM4->ARR = 9999;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 1);
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
void EXTI15_10_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF13)) {
		EXTI->PR1 |= EXTI_PR1_PIF13;
		
		if (!(GPIOC->IDR & GPIO_IDR_ID13))
			TIM4->CR1 |= TIM_CR1_CEN;
		else
			TIM4->CR1 &= ~TIM_CR1_CEN;
	}
}

void TIM4_IRQHandler(void) {
	TIM4->SR &= ~TIM_SR_UIF;
	//move_robot(FORWARD);
}
