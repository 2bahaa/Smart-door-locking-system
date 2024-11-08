/*
 * motor.c
 *
 *  Created on: Oct 7, 2024
 *      Author: Mohamed Bahaa
 */

#include "motor.h"

// Initialize the DC Motor (set direction pins and stop the motor initially)
void DcMotor_Init(void)
{
	// Set Motor Control Pins as output
	GPIO_setupPinDirection(MOTOR_CTRL_PORT_1,MOTOR_CTRL_PIN_1,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_CTRL_PORT_2,MOTOR_CTRL_PIN_2,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PWM_PORT,MOTOR_PWM_PIN,PIN_OUTPUT);

	// Stop the motor initially
	GPIO_writePin(MOTOR_CTRL_PORT_1,MOTOR_CTRL_PIN_1,LOGIC_LOW);
	GPIO_writePin(MOTOR_CTRL_PORT_2,MOTOR_CTRL_PIN_2,LOGIC_LOW);

}



void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	// Ensure speed is between 0 and 100
	if (speed > 100)
	{
		speed = 100;
	}

	// Set the duty cycle for the PWM
	PWM_Timer0_Start(speed);

	switch (state)
	{
	case CW:
		// Set control pins for clockwise rotation
		GPIO_writePin(MOTOR_CTRL_PORT_1,MOTOR_CTRL_PIN_1,LOGIC_LOW);
		GPIO_writePin(MOTOR_CTRL_PORT_2,MOTOR_CTRL_PIN_2,LOGIC_HIGH);
		break;

	case ACW:
		// Set control pins for anti-clockwise rotation
		GPIO_writePin(MOTOR_CTRL_PORT_1,MOTOR_CTRL_PIN_1,LOGIC_LOW);
		GPIO_writePin(MOTOR_CTRL_PORT_2,MOTOR_CTRL_PIN_2,LOGIC_HIGH);
		break;

	case STOP:
	default:
		// Stop the motor
		GPIO_writePin(MOTOR_CTRL_PORT_1,MOTOR_CTRL_PIN_1,LOGIC_LOW);
		GPIO_writePin(MOTOR_CTRL_PORT_2,MOTOR_CTRL_PIN_2,LOGIC_LOW);
		break;
	}

}



