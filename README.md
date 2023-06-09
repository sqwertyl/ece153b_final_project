# ECE153B Final Project

[**Project Site**](https://sites.google.com/view/ece153b-project-proposal/home)

This repository contains the final project for ECE153B. The project was implemented using the [STM32L476RG Nucleo Board](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html) from STMicroelectronics.

## Description

Our project is a robot controlled by a Wii Nunchuk, with an air pump connected to a container of fluid. It is controlled using the joystick and buttons on the Nunchuk.

The robot also features an ultrasonic sensor that measures the distance in front of it, ensuring that the fluid is only dispensed if the target container is close enough.

### Pins Used

| Pin | Function | Pin | Function |
| --- | -------- | --- | -------- |
| **PC0** | Stepper Motor | **PB8** | I2C Clock |
| **PC1** | Stepper Motor | **PB9** | I2C Data |
| **PC10** | Stepper Motor | **PA5** | Air Pump |
| **PC12** | Stepper Motor | **PA6** | Air Pump |
| **PC5** | Stepper Motor | **PA9** | Ultrasonic Sensor 1 Trigger |
| **PC6** | Stepper Motor | **PB6** | Ultrasonic Sensor 1 Echo |
| **PC8** | Stepper Motor | **PA10** | Ultrasonic Sensor 2 Trigger |
| **PC9** | Stepper Motor | **PB4** | Ultrasonic Sensor 2 Echo |


### Part Links

| Part | Link |
| ---- | ---- |
| Wii Nunchuk | drawer :D |
| Stepper Motors | https://www.amazon.com/gp/product/B00PNEQ79Q |
| Motor Drivers | https://www.amazon.com/gp/product/B07BK1QL5T |
| 5mm Hex Couplers | https://www.amazon.com/dp/B08M3S6Y6T | 
| Wheels | https://www.amazon.com/gp/product/B079MFH6CQ |
| Air Pumps | https://www.amazon.com/gp/product/B06Y2CXZ67 |
| Ultrasonic Sensor (from kit) | https://www.amazon.com/gp/product/B01MG49ZQ5 |
