/*
 * motor.h
 *
 *  Created on: Oct 7, 2024
 *      Author: Mohamed Bahaa
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>
#include "gpio.h"
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

typedef enum {
    STOP,
    CW,  // Clockwise
    ACW  // Anti-Clockwise
} DcMotor_State;

// Static configuration for motor control pins and PWM pin

// Motor Control Pin 1 Configuration
#define MOTOR_CTRL_PORT_1    PORTD_ID
#define MOTOR_CTRL_PIN_1     PIN6_ID

// Motor Control Pin 2 Configuration
#define MOTOR_CTRL_PORT_2    PORTD_ID
#define MOTOR_CTRL_PIN_2     PIN7_ID

// Motor PWM Pin Configuration (Assume Timer0 OC0 pin)
#define MOTOR_PWM_PORT       PORTB_ID
#define MOTOR_PWM_PIN        PIN3_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* MOTOR_H_ */
