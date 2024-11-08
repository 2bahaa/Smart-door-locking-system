/*
 * pwm.c
 *
 *  Created on: Oct 6, 2024
 *      Author: Mohamed Bahaa
 */

#include"pwm.h"


void PWM_Timer0_Start(uint8 duty_cycle)
{
	if (duty_cycle > 100)
	{
		duty_cycle = 100;  // Clamp to 100 if value exceeds 100
	}
	else if (duty_cycle < 0)
	{
		duty_cycle = 0;    // Clamp to 0 if value is less than 0
	}
	// Set OC0 (Pin B3) as output
	 GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);

	// Configure Timer0 for Fast PWM mode with non-inverting output
	// Set WGM00 (Waveform Generation Mode) and WGM01 for Fast PWM, and COM01 for non-inverting
	TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01);

	// Set the pre-scaler to F_CPU/64
	TCCR0 |= (1 << CS01) | (1 << CS00);

	// Set the duty cycle (0-100%) by writing to OCR0
	// OCR0 = (duty_cycle / 100) * 255
	OCR0 = (duty_cycle * 255) / 100;
}
