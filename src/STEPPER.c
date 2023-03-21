#include "STEPPER.h"

void STEPPER_MOTOR_Init(void) {
	GPIO_Init_R();
	GPIO_Init_L();
}

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

void GPIO_Init_L(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	// set pins to output
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);
	// set output type to push pull
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
	// set pins to no pull up or pull down
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	// set pins to high speed
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 | GPIO_OSPEEDR_OSPEED1 | GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
	GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED0_1 | GPIO_OSPEEDR_OSPEED1_1 | GPIO_OSPEEDR_OSPEED2_1 | GPIO_OSPEEDR_OSPEED3_1);
}

#define DELAY 1200

static uint32_t pinA1_R = GPIO_ODR_OD5, pinA2_R = GPIO_ODR_OD6,
				pinA1_L = GPIO_ODR_OD0, pinA2_L = GPIO_ODR_OD1;

static uint32_t pinB1_R = GPIO_ODR_OD8, pinB2_R = GPIO_ODR_OD9,
				pinB1_L = GPIO_ODR_OD2, pinB2_L = GPIO_ODR_OD3;

void move_robot(enum DIR dir) {
	switch (dir) {
		case FORWARD:
			// step 1
			GPIOC->ODR |= pinA1_R | pinA2_L;
			GPIOC->ODR &= ~(pinA2_R | pinA1_L);
			break;
		case BACKWARD:
			// step 1
			GPIOC->ODR |= pinA2_R | pinA1_L;
			GPIOC->ODR &= ~(pinA1_R | pinA2_L);
			break;
		case LEFT:
			// step 1
			GPIOC->ODR |= pinA1_R | pinA1_L;
			GPIOC->ODR &= ~(pinA2_R | pinA2_L);
			break;
		case RIGHT:
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