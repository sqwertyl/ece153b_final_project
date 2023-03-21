#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include "LED.h"
#include "MOTOR.h"
#include "DISTANCE.h"
#include "NUNCHUK.h"


// nunchuk data
static const uint8_t n_init1[2] = {0xF0,0x55};
static const uint8_t n_init2[2] = {0xFB,0x00};
static const uint8_t n_done[2] = {0x00,0x00};
static const uint8_t n_addr_send = 0x52 << 1;
static const uint8_t n_addr_receive = 0xA4;

// runtime variables
static uint8_t first_run = 1;
static enum DIR direction = NONE;
static uint8_t close_enough = 0;
static uint8_t z_button = 0;
static uint8_t c_button = 0;
static uint8_t pump_enable = 0;
static uint8_t data[6] = {0};


int main(void) {
	// initialize pins and functions
	MOTOR_Init();
	while(1) {
			for(int i=0; i < 1000000; i++);
			enable_pump(1);
			for(int i=0; i < 1000000; i++);
			enable_pump(0);
	}
	System_Clock_Init();
	I2C_GPIO_Init();
	I2C_Initialization();
	// ULTRASONIC_Init();
	LED_Init();
		
	// initialize UART -> 1 for BT, 2 for USB
	Init_USARTx(2);
	printf("initialized UART over: %s", (RCC->APB1ENR1 & RCC_APB1ENR1_USART2EN) ? 
			"USB\n" : "BT\n");

	// initialize nunchuk
	I2C_SendData(I2C1, n_addr_send, &n_init1, 2);
	I2C_SendData(I2C1, n_addr_send, &n_init2, 2);
	
	while (1) {
		// receive nunchuk data
		I2C_ReceiveData(I2C1, n_addr_receive, &data, 6);
		I2C_SendData(I2C1, n_addr_receive, &n_done, 2);
		if (first_run) {
			data[0] = 128; data[1] = 128; data[5] = 255;
			first_run ^= first_run;
		}
		
		// debug nunchuk data
		printf("joyX: %i\t", data[0]);
		printf("joyY: %i\t", data[1]);
		printf("Z: %i\t", (data[5] >> 0) & 1);
		printf("C: %i\t", (data[5] >> 1) & 1);

		// parse nunchuk and ultrasonic sensor
		parse_Nunchuk(data);
		direction = get_Direction();
		z_button = get_Z_pressed();
		c_button = get_C_pressed();
		close_enough = get_Distance() < 3 ? 1 : 0;
		pump_enable = (z_button && close_enough) || c_button;
		
		// debug motor controls
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
		
		// debug other controls
		printf( z_button ? "Z\t" : "\t");
		printf( c_button ? "C\t" : "\t");
		printf( close_enough ? "close\t" : "not close\t");
		printf( "distance: %i\t", get_Distance() );
		printf( pump_enable ? c_button ? "pump on: override\n" : "pump on\n" : "pump off%s", 
				((z_button && !close_enough) ? ": not close enough\n" : "\n"));
		
		// instruct robot
		move_robot(direction);
		pump_enable ? LED_On() : LED_Off(); // enable_pump(pump_enable);
		
	}	
}

