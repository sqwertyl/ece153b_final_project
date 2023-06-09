#include "MOTOR.h"

/* initialize all motors */
void MOTOR_Init(void) {
	GPIO_Init_R();
	GPIO_Init_L();
	GPIO_Air();
}

/* initialize pins for right side motors */
void GPIO_Init_R(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	// set pins to output
	GPIOC->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
	GPIOC->MODER |= GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0;
	// set pins to high speed
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1 | GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1;
	// set output type to push pull
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	// set pins to no pull up or pull down
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
}

/* initialize pins for left side motors */
void GPIO_Init_L(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	// set pins to output
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE10 | GPIO_MODER_MODE12);
	GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE10_0 | GPIO_MODER_MODE12_0);
	// set output type to push pull
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT10 | GPIO_OTYPER_OT12);
	// set pins to no pull up or pull down
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD10 | GPIO_PUPDR_PUPD12);
	// set pins to high speed
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 | GPIO_OSPEEDR_OSPEED1 | GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED12);
	GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED0_1 | GPIO_OSPEEDR_OSPEED1_1 | GPIO_OSPEEDR_OSPEED10_1 | GPIO_OSPEEDR_OSPEED12_1);
}

/* initialize pins for the air pump */
void GPIO_Air(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// set pins to output
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6);
	GPIOA->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0);
	// set output type to push pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6);
	// set pins to no pull up or pull down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6);
	// set pins to high speed
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED6);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1);
}

#define DELAY 6000

/* enable/disable air pump */
void enable_pump(uint8_t enable) {
	GPIOA->ODR &= ~GPIO_ODR_OD6;
	
	if (enable) GPIOA->ODR |= GPIO_ODR_OD5;
	else GPIOA->ODR &= ~GPIO_ODR_OD5;
}


/* stepper motor pin mapping */
static uint32_t pinA1_R = GPIO_ODR_OD5, pinA2_R = GPIO_ODR_OD6,
				pinA1_L = GPIO_ODR_OD0, pinA2_L = GPIO_ODR_OD1;

static uint32_t pinB1_R = GPIO_ODR_OD8, pinB2_R = GPIO_ODR_OD9,
				pinB1_L = GPIO_ODR_OD10, pinB2_L = GPIO_ODR_OD12;


/* stepper motor driver code */
void move_robot(enum DIR dir) {
	for (int j = 0; j < 16; j++) {
		switch (dir) {
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
		for (int i = 0; i < DELAY; i++);
		// step 2
		GPIOC->ODR ^= pinB1_R | pinB1_L;
		GPIOC->ODR ^= pinB2_R | pinB2_L;
		for (int i = 0; i < DELAY; i++);
		// step 3
		GPIOC->ODR ^= pinA1_R | pinA1_L;
		GPIOC->ODR ^= pinA2_R | pinA2_L;
		for (int i = 0; i < DELAY; i++);
		// step 4
		GPIOC->ODR ^= pinB1_R | pinB1_L;
		GPIOC->ODR ^= pinB2_R | pinB2_L;
		for (int i = 0; i < DELAY; i++);
	}
}
