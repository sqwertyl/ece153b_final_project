#include "DISTANCE.h"

static uint32_t volatile currentValue_s1 = 0;
static uint32_t volatile lastValue_s1 = 0;
static uint32_t volatile overflowCount_s1 = 0;
static uint32_t volatile timeInterval_s1 = 0;

static uint32_t volatile currentValue_s2 = 0;
static uint32_t volatile lastValue_s2 = 0;
static uint32_t volatile overflowCount_s2 = 0;
static uint32_t volatile timeInterval_s2 = 0;


void ULTRASONIC_Init(void) {
	Sensor1_Init();
	Sensor2_Init();
}

void Sensor1_Init(void) {
	Sensor1_GPIO_Init();
	Sensor1_TIM_Init();
}

void Sensor2_Init(void) {
	Sensor2_GPIO_Init();
	Sensor2_TIM_Init();
}

/* sets up pins PA9 and PB6; PA9 is trigger, PB6 is echo (input capture)*/
void Sensor1_GPIO_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;			// enable GPIO A

	// setup trigger pin (PA9)		
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;		// enable GPIOA clock
	GPIOA->MODER &= ~GPIO_MODER_MODE9_0;		// configure PA9 to be used as alternative function
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;			// Alternate Function AF9
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;			// set PA9 to no pull-up, no pull-down
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;		// set output type of PA9 to push-pull
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;	// set PA9 to very high output speed

	// setup echo (input capture) pin (PB6)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;		// enable GPIO B clock
	GPIOB->MODER &= ~GPIO_MODER_MODE6;		// PB6 used as alternative function
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;		// PB6  no pull-up, no pull-down
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;
	GPIOB->AFR[0]  |= GPIO_AFRL_AFSEL6_1;	// AF2 for PB6
}

/* sets up pins PA10 and PB4; PA10 is trigger, PB4 is echo (input capture)*/
void Sensor2_GPIO_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;			// enable GPIO A

	// setup trigger pin (PA10)		
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;		// enable GPIOA clock
	GPIOA->MODER &= ~GPIO_MODER_MODE10_0;		// configure PA10 to be used as alternative function
	GPIOA->MODER |= GPIO_MODER_MODE10_1;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10;			// Alternate Function AF9
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL10_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10;			// set PA10 to no pull-up, no pull-down
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT10;		// set output type of PA10 to push-pull
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED10;	// set PA10 to very high output speed

	// setup echo (input capture) pin (PB4)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;		// enable GPIO B clock
	GPIOB->MODER &= ~GPIO_MODER_MODE4;		// PB4 used as alternative function
	GPIOB->MODER |= GPIO_MODER_MODE4_1;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;		// PB4  no pull-up, no pull-down
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4;
	GPIOB->AFR[0]  |= GPIO_AFRL_AFSEL4_1;	// AF2 for PB4
}

/* sets up TIM1 & TIM4; TIM4 captures input*/
void Sensor1_TIM_Init(void) {
	// setup trigger timer (TIM5)
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


	// setup echo (input capture) timer (TIM2)
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

void Sensor2_TIM_Init(void) {
	// setup trigger timer (TIM5)
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;			// enable Timer 1 
	TIM1->PSC = 15;													// set prescaler to 15
	TIM1->CR1 |= TIM_CR1_ARPE;							// enable ARR preload
	TIM1->ARR = 0xFFFFU;										// set auto reload value to maximum
	TIM1->CCR3 = 10;												// sets 10us pulse width
	TIM1->CCMR2 &= ~TIM_CCMR2_OC3M;					// set output compare mode to 0110 (PWM mode 1)
	TIM1->CCMR2 |= TIM_CCMR2_OC3M_1;
	TIM1->CCMR2 |= TIM_CCMR2_OC3M_2;
	TIM1->CCMR2 |= TIM_CCMR2_OC3PE;					// enable output c/c preload
	TIM1->CCER |= TIM_CCER_CC3E;						// enable c/c output
	TIM1->BDTR |= (TIM_BDTR_MOE + TIM_BDTR_OSSR + TIM_BDTR_OSSI);		// break and dead time
	TIM1->EGR |= TIM_EGR_UG;								// enable event generation
	TIM1->DIER |= TIM_DIER_UIE;							// enable update interrupt 
	TIM1->SR |= TIM_SR_UIF;									// clear update interrupt flag
	TIM1->CR1 &= ~TIM_CR1_DIR;							// set direction 
	TIM1->CR1 |= TIM_CR1_CEN;								// enable counter

	//TIM3
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;	// enable timer 4
	TIM3->PSC = 15;												// set prescaler to 15
	TIM3->CR1 |= TIM_CR1_ARPE;					// enable auto reload preload
	TIM3->ARR = 0xFFFFFFFFU;						// set reload value to maximum
	TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;			// set input capture mode bits, input capture is mapped to Timer Input 1
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM3->CCER |= TIM_CCER_CC1NP;					// set CC2 channel as 11, sensitive to both rising and falling edges
	TIM3->CCER |= TIM_CCER_CC1P;
	TIM3->CCER |= TIM_CCER_CC1E;					// Enable capturing for channel 1
	TIM3->DIER |= (TIM_DIER_CC1DE + TIM_DIER_CC1IE + TIM_DIER_UIE); // Enable interrupt, DMA, and Update interrupt
	TIM3->EGR |= TIM_EGR_UG;						// enable update generation 
	TIM3->SR &= ~TIM_SR_UIF;					// clear update interrupt flag
	TIM3->CR1 &= ~TIM_CR1_DIR;				// set direction of the counter
	TIM3->CR1 |= TIM_CR1_CEN;				// enable counter
	
	// NVIC Enable Interrupt
	NVIC_EnableIRQ(TIM3_IRQn);	// enable interrupt and set priority
	NVIC_SetPriority(TIM3_IRQn, 2); 
}

/* input capture*/
void TIM4_IRQHandler(void) {
	// [TODO]
	// This Interrupt Handler compute the difference between two consecutive CCR values
	if(TIM4->SR & TIM_SR_UIF) {
		TIM4->SR &= ~TIM_SR_UIF;

		overflowCount_s1++;
	}
	
	if(TIM4->SR & TIM_SR_CC1IF) {
		TIM4->SR &= ~TIM_SR_CC1IF;

		if(GPIOB->IDR & GPIO_IDR_ID6) {
			lastValue_s1 = TIM4->CCR1;
		}

		else {
			currentValue_s1 = TIM4->CCR1;
			timeInterval_s1 = currentValue_s1 - lastValue_s1;
			if(timeInterval_s1 > 38000 || timeInterval_s1 < 150) {
				timeInterval_s1 = 0;
			}
			
			overflowCount_s1 = 0;
		}
	}
}

void TIM3_IRQHandler(void) {
	if(TIM3->SR & TIM_SR_UIF) {
		TIM3->SR &= ~TIM_SR_UIF;

		overflowCount_s2++;
	}
	
	if(TIM3->SR & TIM_SR_CC1IF) {
		TIM3->SR &= ~TIM_SR_CC1IF;

		if(GPIOB->IDR & GPIO_IDR_ID4) {
			lastValue_s2 = TIM3->CCR1;
		}

		else {
			currentValue_s2 = TIM3->CCR1;
			timeInterval_s2 = currentValue_s2 - lastValue_s2;
			if(timeInterval_s2 > 38000 || timeInterval_s2 < 150) {
				timeInterval_s2 = 0;
			}
			
			overflowCount_s2 = 0;
		}
	}
}


int get_Distance_S1(void) {
	return timeInterval_s1 / 58;
}

int get_Distance_S2(void) {
	return timeInterval_s2 / 58;
}
