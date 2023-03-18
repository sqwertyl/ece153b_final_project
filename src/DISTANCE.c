#include "DISTANCE.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void ULTRASONIC_Init(void) {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;			// enable GPIO A

	// setup trigger pin (PA0)		
	GPIOA->MODER &= ~GPIO_MODER_MODE0;				// PA0 used as alternative function
	GPIOA->MODER |= GPIO_MODER_MODE0_1;		
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;				// PA0  no pull-up, no pull-down
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0;		
	GPIOA->AFR[0]  |= GPIO_AFRL_AFSEL0_0;			// AF2 for PA0

	// setup echo (input capture) pin (PA1)		
	GPIOA->MODER &= ~GPIO_MODER_MODE1_0;			// configure PA1 to be used as alternative function
	GPIOA->MODER |= GPIO_MODER_MODE1_1;		
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL1;				// Alternate Function AF9
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL1_1;		
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;				// set PA1 to no pull-up, no pull-down
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT1;				// set output type of PA1 to push-pull
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED1;			// set PA1 to very high output speed

	// setup trigger timer (TIM2)
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;			// enable Timer 5
	TIM5->PSC = 3;									// set prescaler to 15
	TIM5->CR1 |= TIM_CR1_ARPE;						// enable ARR preload
	TIM5->ARR = 0xFFFFU;							// set auto reload value to maximum
	TIM5->CCR2 = 10;								// sets 10us pulse width
	TIM5->CCMR1 &= ~TIM_CCMR1_OC2M;					// set output compare mode to 0110 (PWM mode 1)
	TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
	TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;
	TIM5->CCMR1 |= TIM_CCMR1_OC2PE;					// enable output c/c preload
	TIM5->CCER |= TIM_CCER_CC2E;					// enable c/c output
	TIM5->BDTR |= (TIM_BDTR_MOE + TIM_BDTR_OSSR		// break and dead time
	+ TIM_BDTR_OSSI);	
	TIM5->EGR |= TIM_EGR_UG;						// enable event generation
	TIM5->DIER |= TIM_DIER_UIE;						// enable update interrupt 
	TIM5->SR |= TIM_SR_UIF;							// clear update interrupt flag
	TIM5->CR1 &= ~TIM_CR1_DIR;						// set direction 
	TIM5->CR1 |= TIM_CR1_CEN;						// enable counter


	// setup echo (input capture) timer (TIM5)
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;			// enable timer 2
	TIM2->PSC = 3;									// set prescaler to 15
	TIM2->CR1 |= TIM_CR1_ARPE;						// enable auto reload preload
	TIM2->ARR = 0xFFFFFFFFU;						// set reload value to maximum
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;					// set input capture mode bits, input capture is mapped to Timer Input 1
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM2->CCER |= TIM_CCER_CC1NP;					// set CC2 channel as 11, sensitive to both rising and falling edges
	TIM2->CCER |= TIM_CCER_CC1P;
	TIM2->CCER |= TIM_CCER_CC1E;					// Enable capturing for channel 2
	TIM2->DIER |= (TIM_DIER_CC1DE + TIM_DIER_CC1IE	// Enable interrupt, DMA, and Update interrupt
		+ TIM_DIER_UIE); 
	TIM2->EGR |= TIM_EGR_UG;						// enable update generation 
	TIM2->SR &= ~TIM_SR_UIF;						// clear update interrupt flag
	TIM2->CR1 &= ~TIM_CR1_DIR;						// set direction of the counter
	TIM2->CR1 |= TIM_CR1_CEN;						// enable counter
	
	// NVIC Enable Interrupt
	NVIC_EnableIRQ(TIM2_IRQn);	// enable interrupt and set priority
	NVIC_SetPriority(TIM2_IRQn, 2); 
}

int get_Distance(void) {
	return timeInterval / 58;
}

void TIM2_IRQHandler(void) {
	// This Interrupt Handler compute the difference between two consecutive CCR values
	uint32_t tmp;
	
	if(TIM2->SR & TIM_SR_UIF) {
		TIM2->SR &= ~TIM_SR_UIF;

		overflowCount++;
	}
	
	if(TIM2->SR & TIM_SR_CC1IF) {
		TIM2->SR &= ~TIM_SR_CC1IF;

		if(GPIOA->IDR & GPIO_IDR_ID0) {
			lastValue = TIM2->CCR1;
		}

		else {
			currentValue = TIM2->CCR1;
			timeInterval = currentValue - lastValue;
			if(timeInterval > 38000 || timeInterval < 150) {
				timeInterval = 0;
			}
			
			overflowCount = 0;
		}
	}
}