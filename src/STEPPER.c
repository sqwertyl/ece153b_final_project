#include "STEPPER.h"

void STEPPER_MOTOR_Init(void) {
	//GPIO_Init_R();
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
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// set pins to output
	GPIOB->MODER &= ~(GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE11 | GPIO_MODER_MODE12);
	GPIOB->MODER |= (GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE11_0 | GPIO_MODER_MODE12_0);
	// set output type to push pull
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 | GPIO_OTYPER_OT11 | GPIO_OTYPER_OT12);
	// set pins to no pull up or pull down
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD11 | GPIO_PUPDR_PUPD12);
	// set pins to high speed
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED1 | GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED11 | GPIO_OSPEEDR_OSPEED12);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED1_1 | GPIO_OSPEEDR_OSPEED2_1 | GPIO_OSPEEDR_OSPEED11_1 | GPIO_OSPEEDR_OSPEED12_1);
}

#define DELAY 500


void fClockwise(enum MTR motor) {
	if (motor == R) {
		// step 1
		GPIOC->ODR |= GPIO_ODR_OD5;		// A	-> 1
		GPIOC->ODR &= ~GPIO_ODR_OD6;	// A'	-> 0
		GPIOC->ODR &= ~GPIO_ODR_OD8;	// B	-> 0
		GPIOC->ODR |= GPIO_ODR_OD9;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 2
		GPIOC->ODR ^= GPIO_ODR_OD8;		// B	-> 1
		GPIOC->ODR ^= GPIO_ODR_OD9;		// B'	-> 0
		for (int i = 0; i < DELAY; i++);
		// step 3
		GPIOC->ODR ^= GPIO_ODR_OD5;		// A	-> 0
		GPIOC->ODR ^= GPIO_ODR_OD6;		// A'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 4
		GPIOC->ODR ^= GPIO_ODR_OD8;		// B	-> 0
		GPIOC->ODR ^= GPIO_ODR_OD9;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
	} else if (motor == L) {
		// step 1
		GPIOB->ODR |= GPIO_ODR_OD1;		// A	-> 1
		GPIOB->ODR &= ~GPIO_ODR_OD2;	// A'	-> 0
		GPIOB->ODR &= ~GPIO_ODR_OD11;	// B	-> 0
		GPIOB->ODR |= GPIO_ODR_OD12;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 2
		GPIOB->ODR ^= GPIO_ODR_OD11;		// B	-> 1
		GPIOB->ODR ^= GPIO_ODR_OD12;		// B'	-> 0
		for (int i = 0; i < DELAY; i++);
		// step 3
		GPIOB->ODR ^= GPIO_ODR_OD1;		// A	-> 0
		GPIOB->ODR ^= GPIO_ODR_OD2;		// A'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 4
		GPIOB->ODR ^= GPIO_ODR_OD11;		// B	-> 0
		GPIOB->ODR ^= GPIO_ODR_OD12;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
	}
}



void fCounterClockwise(enum MTR motor) {
	if (motor == R) {
		// step 1
		GPIOC->ODR &= ~GPIO_ODR_OD5;	// A	-> 1
		GPIOC->ODR |= GPIO_ODR_OD6;		// A'	-> 0
		GPIOC->ODR &= ~GPIO_ODR_OD8;	// B	-> 0
		GPIOC->ODR |= GPIO_ODR_OD9;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 2
		GPIOC->ODR ^= GPIO_ODR_OD8;		// B	-> 1
		GPIOC->ODR ^= GPIO_ODR_OD9;		// B'	-> 0
		for (int i = 0; i < DELAY; i++);
		// step 3
		GPIOC->ODR ^= GPIO_ODR_OD5;		// A	-> 0
		GPIOC->ODR ^= GPIO_ODR_OD6;		// A'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 4
		GPIOC->ODR ^= GPIO_ODR_OD8;		// B	-> 0
		GPIOC->ODR ^= GPIO_ODR_OD9;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
	} else if (motor == L) {
		// step 1
		GPIOB->ODR &= ~GPIO_ODR_OD1;	// A	-> 1
		GPIOB->ODR |= GPIO_ODR_OD2;		// A'	-> 0
		GPIOB->ODR &= ~GPIO_ODR_OD11;	// B	-> 0
		GPIOB->ODR |= GPIO_ODR_OD12;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 2
		GPIOB->ODR ^= GPIO_ODR_OD1;		// B	-> 1
		GPIOB->ODR ^= GPIO_ODR_OD2;		// B'	-> 0
		for (int i = 0; i < DELAY; i++);
		// step 3
		GPIOB->ODR ^= GPIO_ODR_OD1;		// A	-> 0
		GPIOB->ODR ^= GPIO_ODR_OD2;		// A'	-> 1
		for (int i = 0; i < DELAY; i++);
		// step 4
		GPIOB->ODR ^= GPIO_ODR_OD11;		// B	-> 0
		GPIOB->ODR ^= GPIO_ODR_OD12;		// B'	-> 1
		for (int i = 0; i < DELAY; i++);
		
	}

}
// void fClockwise(enum MTR motor) {
//     GPIO_TypeDef* portA;
//     GPIO_TypeDef* portB;
//     uint32_t pinA1;
//     uint32_t pinA2;
//     uint32_t pinB1;
//     uint32_t pinB2;
//     if (motor == R) {
//         portA = GPIOC;
//         portB = GPIOC;
//         pinA1 = GPIO_ODR_OD5;
//         pinA2 = GPIO_ODR_OD6;
//         pinB1 = GPIO_ODR_OD8;
//         pinB2 = GPIO_ODR_OD9;
//     } else {
//         portA = GPIOB;
//         portB = GPIOB;
//         pinA1 = GPIO_ODR_OD1;
//         pinA2 = GPIO_ODR_OD2;
//         pinB1 = GPIO_ODR_OD11;
//         pinB2 = GPIO_ODR_OD12;
//     }
// 		for (int j = 0; j < 512; j++) {
//     // step 1
//     portA->ODR |= pinA1;
//     portA->ODR &= ~pinA2;
//     portB->ODR &= ~pinB1;
//     portB->ODR |= pinB2;
//     for (int i = 0; i < DELAY; i++);
//     // step 2
//     portB->ODR ^= pinB1;
//     portB->ODR ^= pinB2;
//     for (int i = 0; i < DELAY; i++);
//     // step 3
//     portA->ODR ^= pinA1;
//     portA->ODR ^= pinA2;
//     for (int i = 0; i < DELAY; i++);
//     // step 4
//     portB->ODR ^= pinB1;
//     portB->ODR ^= pinB2;
//     for (int i = 0; i < DELAY; i++);
// 		}

// }


