#include "stm32l476xx.h"
#include "MOTOR.h"
#include "EXTI.h"
#include "SysClock.h"
#include "LED.h"
#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include "DISTANCE.h"
#include "NUNCHUK.h"




int main(void) {
	//STEPPER_MOTOR_Init();
	System_Clock_Init();
	//LED_Init();
	//EXTI_Init();
	I2C_GPIO_Init();
	I2C_Initialization();
	// ULTRASONIC_Init();
		
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

	static uint8_t close_enough = 0;
	static enum DIR direction;
	static uint8_t z_button;

	while (1) {
		uint8_t data[7] = {0};
		I2C_ReceiveData(I2C1, n_addr_receive, &data, 6);
		I2C_SendData(I2C1, n_addr_receive, &n_done, 2);
		
		// debug nunchuk data
		printf("joyX: %i\t\t", data[0]);
		printf("joyY: %i\t\t", data[1]);
		printf("Z: %i\t", (data[5] >> 0) & 1);
		printf("C: %i\t", (data[5] >> 1) & 1);

		parse_Nunchuk(data);
		direction = get_Direction();
		z_button = get_Z_pressed();
		close_enough = get_Distance() < 3 ? 1 : 0;
		
		if (direction == FORWARD)
			printf("FOWARD\t");
		else if (direction == BACKWARD)
			printf("BACKWARD\t");
		else if (direction == LEFT)
			printf("LEFT\t");
		else if (direction == RIGHT)
			printf("RIGHT\t");
		else if (direction == NONE)
			printf("NONE\t");
		
		printf( z_button ? "Z pressed poggers\n" : "\n");
		printf( close_enough ? "close\n" : "not close\n");
		
		move_robot(direction);
		
		
	}
	
	return 0;
	
}

