/*
 * main.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed Bahaa
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include "uart.h"
#include "external_eeprom.h"
#include "motor.h"
#include "buzzer.h"
#include "pir.h"
#include "std_types.h"
#include "twi.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_LENGTH 5
#define EEPROM_ADDRESS 0x0311
#define START_COMMUNICATION 0x15
#define COMMAND_OPEN_DOOR '+'
#define COMMAND_CHANGE_PASSWORD '-'
#define WAIT_MESSAGE_COMMAND 0x20
#define ATTEMPTS_LIMIT 3
#define TRY_AGAIN 0x11

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 savedPassword[PASSWORD_LENGTH];
uint8 enteredPassword[PASSWORD_LENGTH];
uint8 attempts = 0;



/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/
void initializeSystem();
void unlockDoor();
void lockDoor();
void receiveAndVerifyPasswords();
void receiveAndVerifyPasswordForOperation(uint8 command);
void savePasswordToEEPROM(uint8 *password);
void readPasswordFromEEPROM(uint8 *password);
void handleFailedAttempts();

#endif /* CONTROL_MAIN_H_ */



