#include "WATERLEVEL.h"

static uint32_t volatile currentValue_waterlevel = 0;
static uint32_t volatile lastValue_waterlevel = 0;
static uint32_t volatile overflowCount_waterlevel = 0;
static uint32_t volatile timeInterval_waterlevel = 0;

void Input_Capture_Setup() {
	//PB6
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;		// enable GPIO B clock
	
	GPIOB->MODER &= ~GPIO_MODER_MODE6;		// PB6 used as alternative function
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;		// PB6  no pull-up, no pull-down
	
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;
	GPIOB->AFR[0]  |= GPIO_AFRL_AFSEL6_1;	// AF2 for PB6
	
	//TIM4
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;	// enable timer 4
	
	TIM4->PSC = 15;												// set prescaler to 15
	
	TIM4->CR1 |= TIM_CR1_ARPE;					// enable auto reload preload
	TIM4->ARR = 0xFFFFFFFFU;						// set reload value to maximum
	
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;			// set input capture mode bits, input capture is mapped to Timer Input 1
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	TIM4->CCER |= TIM_CCER_CC1NP;					// set CC2 channel as 11, sensitive to both rising and falling edges
	TIM4->CCER |= TIM_CCER_CC1P;
	
	TIM4->CCER |= TIM_CCER_CC1E;					// Enable capturing for channel 2
	
	TIM4->DIER |= (TIM_DIER_CC1DE + TIM_DIER_CC1IE + TIM_DIER_UIE); // Enable interrupt, DMA, and Update interrupt
	
	TIM4->EGR |= TIM_EGR_UG;						// enable update generation 
	
	TIM4->SR &= ~TIM_SR_UIF;					// clear update interrupt flag
	
	TIM4->CR1 &= ~TIM_CR1_DIR;				// set direction of the counter
	
	TIM4->CR1 |= TIM_CR1_CEN;				// enable counter
	
	// NVIC Enable Interrupt
	NVIC_EnableIRQ(TIM4_IRQn);	// enable interrupt and set priority
	NVIC_SetPriority(TIM4_IRQn, 2); 
}

void TIM4_IRQHandler(void) {
	// [TODO]
	// This Interrupt Handler compute the difference between two consecutive CCR values
	uint32_t tmp;
	
	if(TIM4->SR & TIM_SR_UIF) {
		TIM4->SR &= ~TIM_SR_UIF;

		overflowCount_waterlevel++;
	}
	
	if(TIM4->SR & TIM_SR_CC1IF) {
		TIM4->SR &= ~TIM_SR_CC1IF;

		if(GPIOB->IDR & GPIO_IDR_ID6) {
			lastValue_waterlevel = TIM4->CCR1;
		}

		else {
			currentValue_waterlevel = TIM4->CCR1;
			timeInterval_waterlevel = currentValue_waterlevel - lastValue_waterlevel;
			if(timeInterval_waterlevel > 38000 || timeInterval_waterlevel < 150) {
				timeInterval_waterlevel = 0;
			}
			
			overflowCount_waterlevel = 0;
		}
	}
}

void Trigger_Setup() {
	// [TODO]
	// PA9 setup
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;		// enable GPIOA clock
	
	GPIOA->MODER &= ~GPIO_MODER_MODE9_0;		// configure PA9 to be used as alternative function
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;			// Alternate Function AF9
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0;
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;			// set PA9 to no pull-up, no pull-down
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;		// set output type of PA9 to push-pull
	
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;	// set PA9 to very high output speed
	
	//TIM1 setup
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;			// enable Timer 1 
	
	TIM1->PSC = 15;													// set prescaler to 15
	
	TIM1->CR1 |= TIM_CR1_ARPE;							// enable ARR preload
	
	TIM1->ARR = 0xFFFFU;										// set auto reload value to maximum
	
	TIM1->CCR2 = 10;												// sets 10us pulse width
	
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;					// set output compare mode to 0110 (PWM mode 1)
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1;
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2;
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;					// enable output c/c preload
	
	TIM1->CCER |= TIM_CCER_CC2E;						// enable c/c output
	
	TIM1->BDTR |= (TIM_BDTR_MOE + TIM_BDTR_OSSR + TIM_BDTR_OSSI);		// break and dead time
	
	TIM1->EGR |= TIM_EGR_UG;								// enable event generation
	
	TIM1->DIER |= TIM_DIER_UIE;							// enable update interrupt 
	TIM1->SR |= TIM_SR_UIF;									// clear update interrupt flag
	
	TIM1->CR1 &= ~TIM_CR1_DIR;							// set direction 
	
	TIM1->CR1 |= TIM_CR1_CEN;								// enable counter
}
int get_waterlevel(void) {
	return timeInterval_waterlevel / 58;
}
