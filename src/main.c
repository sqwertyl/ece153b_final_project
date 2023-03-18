#include "stm32l476xx.h"
#include "STEPPER.h"
#include "EXTI.h"
#include "SysClock.h"
#include "LED.h"
#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

int main(void) {
	//STEPPER_MOTOR_Init();
	//LED_Init();
	//EXTI_Init();
	I2C_GPIO_Init();
	I2C_Initialization();
	
	Init_USARTx(2);
	printf("initialized UART\n");
	
	// nunchuk data
	uint8_t n_init1[2] = {0xF0,0x55};
	uint8_t n_init2[2] = {0xFB,0x00};
	uint8_t n_done[2] = {0x00,0x00};
	uint8_t n_addr_send = 0x52 << 1;
	uint8_t n_addr_receive = 0xA4;
	
	// initialize nunchuk
	I2C_SendData(I2C1, n_addr_send, &n_init1, 2);
	I2C_SendData(I2C1, n_addr_send, &n_init2, 2);

	while (1) {
		uint8_t data[6] = {0};
		I2C_ReceiveData(I2C1, n_addr_receive, &data, 6);
		I2C_SendData(I2C1, n_addr_receive, &n_done, 2);
		
		
		printf("joyX: %i\t\t", data[0]);
		printf("joyY: %i\t\t", data[1]);
		printf("Z: %i\t", (data[5] >> 0) & 1);
		printf("C: %i\t", (data[5] >> 1) & 1);
		
		if (data[0] > 0) {
			if (data[1] > 200) {
				move_robot(FORWARD);
				printf("moved forward\n");
			} else if (data[1] < 40) {
				move_robot(BACKWARD);
				printf("moved backward\n");
			} else if (data[0] > 200) {
				move_robot(RIGHT);
				printf("moved right\n");
			} else if (data[0] < 40) {
				move_robot(LEFT);
				printf("moved left\n");
			} else {
				printf("\n");
			}
		}
	}
	
	return 0;
	
}
