/*
 * main.h
 *
 *  Created on: Oct 25, 2024
 *      Author: Mohamed Bahaa
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include "lcd.h"
#include "keypad.h"
#include "common_macros.h"
#include "uart.h"
#include "std_types.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_LENGTH 5
#define ATTEMPTS_LIMIT 3
#define START_COMMUNICATION 0x15
#define ACKNOWLEDGE 0x16
#define COMMAND_OPEN_DOOR '+'
#define COMMAND_CHANGE_PASSWORD '-'
#define WAIT_MESSAGE_COMMAND 0x20
#define TRY_AGAIN 0x11
#define ENTER_BUTTON 13

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

// Global variables
uint8 password1[PASSWORD_LENGTH];
uint8 password2[PASSWORD_LENGTH];
uint8 enteredPassword[PASSWORD_LENGTH];
uint8 attempts = 0;
uint8 isPasswordSet = 0;


/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

// Function Prototypes
void initializeSystem();
void createPassword();
void mainMenu();
void handleOperation(uint8 command);
void enterPassword(uint8 *passwordBuffer, const char* prompt);
void sendPasswordToControlECU(uint8 password[]);
void handleFailedAttempts();
void displayWaitMessage();
void enterPasswords(uint8 *passwordBuffer1,uint8 *passwordBuffer2);

#endif /* HMI_MAIN_H_ */
