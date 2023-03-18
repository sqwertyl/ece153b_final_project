#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;  // Enable I/O analog switch voltage booster
		ADC123_COMMON->CCR |= ADC_CCR_VREFEN;		// Enable internal voltage reference
    ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;   // ensure clock is not divided
		ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;	// clear clock mode
		ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;	// set clock to HCLK/1 synchronous mode
		ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;		// ensure ADC in independent mode
}

void ADC_Pin_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// Enable GPIOA Clock
    GPIOA->MODER |= GPIO_MODER_MODE1;			// Set PA1 to Analog Mode
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;		// Set PA1 to No Pull-up, No Pull-down
		GPIOA->ASCR |= GPIO_ASCR_ASC1;				// Connect pin 1 to ADC
}

void ADC_Init(void) {
    // [TODO] Enable & Reset ADC Clock
		RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;	// enable adc clock
	
		RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;		// reset ADC
		RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;	// clear reset bit
	
	// Other ADC Initialization
    ADC_Pin_Init();
    ADC_Common_Configuration();
    ADC_Wakeup();
	
		ADC1->CR &= ~ADC_CR_ADEN;				// ensure ADC disabled
		ADC1->CR &= ~ADC_CR_ADSTART;
		ADC1->CR &= ~ADC_CR_JADSTART;
	
		ADC1->CFGR &= ~ADC_CFGR_RES;		// 12-bit resolution
		ADC1->CFGR &= ~ADC_CFGR_ALIGN;	// right align
		ADC1->SQR1 &= ~ADC_SQR1_L;			// set only 1 conversion
		ADC1->SQR1 |= ADC_SQR1_SQ1_1 + ADC_SQR1_SQ1_2;	// set 1st conversion to channel 6
		ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;	// channel 6 set to single-ended
		ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;	// reset ch6 sample time
		ADC1->SMPR1 |= (ADC_SMPR1_SMP6_0 + ADC_SMPR1_SMP6_1);	// set ch6 to 24.5 adc clock cycles
		ADC1->CFGR &= ~ADC_CFGR_CONT;		// set to single conversion mode
		ADC1->CFGR &= ~ADC_CFGR_EXTEN;	// disable hardware trigger detection
		ADC1->CR |= ADC_CR_ADEN;				// enable ADC1
	
		while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
		
	
    

    // [TODO] Other Configuration
}
