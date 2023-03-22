#include "EXTI.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>


static uint8_t volatile motor_state = 0;
static enum DIR direction = NONE;

void EXTI_Init(void) {
	// Initialize User Button
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
	// timer init
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
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
	TIM2->CR1 |= TIM_CR1_URS;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->PSC = 40;
	TIM2->ARR = 999;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
}

void set_Direction(enum DIR dir) {
	direction = dir;
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
void EXTI15_10_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF13)) {
		EXTI->PR1 |= EXTI_PR1_PIF13;
		
		if (!(GPIOC->IDR & GPIO_IDR_ID13))
			TIM2->CR1 |= TIM_CR1_CEN;
		else
			TIM2->CR1 &= ~TIM_CR1_CEN;
	}
}

static uint32_t pinA1_R = GPIO_ODR_OD5, pinA2_R = GPIO_ODR_OD6,
				pinA1_L = GPIO_ODR_OD0, pinA2_L = GPIO_ODR_OD1;

static uint32_t pinB1_R = GPIO_ODR_OD8, pinB2_R = GPIO_ODR_OD9,
				pinB1_L = GPIO_ODR_OD10, pinB2_L = GPIO_ODR_OD12;

void TIM2_IRQHandler(void) {
	
	
	if (direction == NONE) {
		motor_state = 0;
		TIM2->SR &= ~TIM_SR_UIF;
		return;
	}
	if (motor_state == 0) {
		switch (direction) {
			case BACKWARD:
				// step 1
				GPIOC->ODR |= pinA1_R | pinA2_L;
				GPIOC->ODR &= ~(pinA2_R | pinA1_L);
				break;
			case FORWARD:
				// step 1
				GPIOC->ODR |= pinA2_R | pinA1_L;
				GPIOC->ODR &= ~(pinA1_R | pinA2_L);
				break;
			case RIGHT:
				// step 1
				GPIOC->ODR |= pinA1_R | pinA1_L;
				GPIOC->ODR &= ~(pinA2_R | pinA2_L);
				break;
			case LEFT:
				// step 1
				GPIOC->ODR |= pinA2_R | pinA2_L;
				GPIOC->ODR &= ~(pinA1_R | pinA1_L);
				break;
			case NONE:
				return;
		}
		GPIOC->ODR &= ~(pinB1_R | pinB1_L);
		GPIOC->ODR |= pinB2_R | pinB2_L;
		printf("motor state: %i\n", motor_state);
		motor_state++;
	} else if (motor_state == 1) {
		GPIOC->ODR ^= pinB1_R | pinB1_L;
		GPIOC->ODR ^= pinB2_R | pinB2_L;
		printf("motor state: %i\n", motor_state);
		motor_state++;
	} else if (motor_state == 2) {
		GPIOC->ODR ^= pinA1_R | pinA1_L;
		GPIOC->ODR ^= pinA2_R | pinA2_L;
		printf("motor state: %i\n", motor_state);
		motor_state++;
	} else if (motor_state == 3) {
		GPIOC->ODR ^= pinB1_R | pinB1_L;
		GPIOC->ODR ^= pinB2_R | pinB2_L;
		printf("motor state: %i\n", motor_state);
		motor_state = 0;
	}
	TIM2->SR &= ~TIM_SR_UIF;
}
