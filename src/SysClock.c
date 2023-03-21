#include "SysClock.h"

void System_Clock_Init(void) {
	
	// Select MSI as system clock source
	RCC->CFGR |= RCC_CFGR_SW_MSI; // [DONE] - Replace with value that will make MSI the system clock source
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_8; // [DONE] - Replace with value that will make range 8 MHz
	
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= RCC_CR_MSIRGSEL; // [DONE] - Replace with value that will select range in RCC->CR
	
	// Enable MSI oscillator
	RCC->CR |= RCC_CR_MSION; // [DONE] - Replace with value that will enable MSI
	
	// Wait until MSI is ready
	while((RCC->CR & (uint32_t)RCC_CR_MSIRDY /* [DONE] - Replace with value that checks whether MSI is ready */) == 0);
}

