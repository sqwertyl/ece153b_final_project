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
	uint8_t n_init1[] = {0x55,0xF0};
	uint8_t n_init2[2] = {0x00,0xFB};
	uint8_t n_addr = 0x52 << 1;
	
	// initialize nunchuk
	I2C_SendData(I2C1, n_addr, &n_init1, 2);
	for (int i = 0; i < 10; i++);
	I2C_SendData(I2C1, n_addr, &n_init2, 2);
	
	while (1) {
		uint8_t n_data[6];
		I2C_ReceiveData(I2C1, n_addr, 0, 6);
		uint8_t byte_6 = n_data[5];
	}
	
	return 0;
	
}
