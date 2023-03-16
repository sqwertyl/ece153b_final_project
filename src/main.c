#include "stm32l476xx.h"
#include "STEPPER.h"
#include "EXTI.h"
#include "SysClock.h"
#include "LED.h"
#include "I2C.h"
#include "SysClock.h"

int main(void) {
	System_Clock_Init();	// 8MHz
	STEPPER_MOTOR_Init();
	LED_Init();
	EXTI_Init();
	I2C_GPIO_Init();
	I2C_Initialization();
	
	// nunchuk data
	uint8_t n_enable[] = {0x40,0x00};
	uint8_t n_addr = 0x52;
	
	// initialize nunchuk
	I2C_SendData(I2C1, n_addr, &n_enable, 2);
	I2C_SendData(I2C1, n_addr, 0, 1);
	
	while (1) {
		uint8_t n_data[6];
		I2C_ReceiveData(I2C1, n_addr, &n_data, 6);
		uint8_t byte_6 = n_data[5];
	}
	
	return 0;
	
}
