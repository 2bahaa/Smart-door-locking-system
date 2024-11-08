/*
 * main.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed Bahaa
 */
// Control_ECU.c

#include "main.h"
volatile uint16 g_tick = 0;  // Global tick counter for timer

// Timer1 callback function to increment the global tick on each compare match
void Timer1_callback(void) {
	g_tick++;  // Increment the tick counter on each timer compare match
}

// Setup Timer1 with a 1-second interval using an 8MHz clock
void setupTimer1(void) {
	Timer_ConfigType timerConfig = {0, 7812, TIMER_1, TIMER_CLOCK_1024, TIMER_COMPARE_MODE};  // Configure timer for 1-second interval
	Timer_init(&timerConfig);  // Initialize the timer with the specified configuration
	Timer_setCallBack(Timer1_callback, TIMER_1);  // Set the callback function for Timer1
}

// Stop Timer1 by deinitializing it
void Timer1Stop(void) {
	Timer_deInit(TIMER_1);  // Deinitialize Timer1 to stop it
}

// Main function for Control_ECU operation
int main(void) {
	initializeSystem();  // Initialize the system peripherals
	sei();  // Enable global interrupts
	receiveAndVerifyPasswords();  // Start the process of receiving and verifying the passwords

	// Main loop to listen for commands and handle operations
	while (1) {
		uint8 command = UART_recieveByte();  // Receive the command from HMI_ECU
		if (command == COMMAND_OPEN_DOOR || command == COMMAND_CHANGE_PASSWORD) {
			// If the command is to open the door or change password, verify password for operation
			receiveAndVerifyPasswordForOperation(command);
		}
		else if (command == TRY_AGAIN) {
			// If the command is to try again, re-verify passwords
			receiveAndVerifyPasswords();
		}
	}
}

// Initialize system peripherals: UART, TWI, Motor, Buzzer, and PIR sensor
void initializeSystem() {
	UART_ConfigType uartConfig = {8, 0, 1, 9600};  // UART configuration for 9600 baud rate
	TWI_ConfigType twiConfig = {0x01, 12};  // I2C configuration (for any future peripheral, e.g., PIR sensor)
	UART_init(&uartConfig);  // Initialize UART
	TWI_init(&twiConfig);  // Initialize TWI (I2C)
	DcMotor_Init();  // Initialize DC motor for door operation
	Buzzer_init();  // Initialize Buzzer for alerts
	PIR_init();  // Initialize PIR sensor for motion detection
}

// Unlock the door by rotating the DC motor for 15 seconds
void unlockDoor() {
	setupTimer1();  // Start Timer1
	g_tick = 0;
	DcMotor_Rotate(CW, 100);  // Rotate motor in the clockwise direction (open the door)
	while (g_tick < 15);  // Wait for 15 seconds (keep motor running for 15 seconds)
	DcMotor_Rotate(STOP, 100);  // Stop the motor after 15 seconds
	g_tick = 0;
	Timer1Stop();  // Stop Timer1
	while (PIR_getState()) {
		// While PIR sensor detects motion, send wait message to HMI_ECU
		UART_sendByte(WAIT_MESSAGE_COMMAND);
	}
	UART_sendByte(0);  // Indicate that the motion detection has stopped (PIR sensor did not detect motion)
	lockDoor();  // Lock the door after the motion detection has stopped
}

// Lock the door by rotating the DC motor in the opposite direction for 15 seconds
void lockDoor() {
	setupTimer1();  // Start Timer1
	g_tick = 0;
	DcMotor_Rotate(ACW, 100);  // Rotate motor in the anticlockwise direction (lock the door)
	while (g_tick < 15);  // Wait for 15 seconds (keep motor running for 15 seconds to lock the door)
	DcMotor_Rotate(STOP, 100);  // Stop the motor after 15 seconds
	Timer1Stop();  // Stop Timer1
}

// Receive and verify the password from HMI_ECU for password creation
void receiveAndVerifyPasswords() {
	uint8 receivedPassword1[PASSWORD_LENGTH], receivedPassword2[PASSWORD_LENGTH];

	// Wait for the start communication signal
	while (UART_recieveByte() != START_COMMUNICATION);

	// Receive the first password
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		receivedPassword1[i] = UART_recieveByte();
	}

	// Wait for the second start communication signal
	while (UART_recieveByte() != START_COMMUNICATION);

	// Receive the second password
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		receivedPassword2[i] = UART_recieveByte();
	}

	// Compare the two received passwords to check if they match
	if (memcmp(receivedPassword1, receivedPassword2, PASSWORD_LENGTH) == 0) {
		savePasswordToEEPROM(receivedPassword1);  // If passwords match, save the password to EEPROM
		UART_sendByte(1);  // Send success signal to HMI_ECU
	} else {
		UART_sendByte(0);  // Send failure signal to HMI_ECU
	}
}

// Receive and verify the entered password for the operation (open door or change password)
void receiveAndVerifyPasswordForOperation(uint8 command) {
	readPasswordFromEEPROM(savedPassword);  // Read the saved password from EEPROM

	// Wait for the start communication signal
	while (UART_recieveByte() != START_COMMUNICATION);

	// Receive the entered password
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		enteredPassword[i] = UART_recieveByte();
	}

	// Compare the entered password with the saved password
	if (memcmp(savedPassword, enteredPassword, PASSWORD_LENGTH) == 0) {
		UART_sendByte(1);  // If password is correct, send success signal to HMI_ECU
		attempts = 0;  // Reset attempts counter
		if (command == COMMAND_OPEN_DOOR) unlockDoor();  // If command is to open door, unlock the door
		else if (command == COMMAND_CHANGE_PASSWORD) receiveAndVerifyPasswords();  // If command is to change password, prompt for new password
	} else {
		UART_sendByte(0);  // If password is incorrect, send failure signal to HMI_ECU
		attempts++;  // Increment attempts counter
		handleFailedAttempts();  // Handle the failed attempts
	}
}

// Save the password to EEPROM for future use
void savePasswordToEEPROM(uint8 *password) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_writeByte(EEPROM_ADDRESS + i, password[i]);  // Write each byte of the password to EEPROM
		_delay_ms(10);  // Short delay for EEPROM write stability
	}
}

// Read the saved password from EEPROM
void readPasswordFromEEPROM(uint8 *password) {
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_readByte(EEPROM_ADDRESS + i, &password[i]);  // Read each byte of the saved password from EEPROM
		_delay_ms(10);  // Short delay for EEPROM read stability
	}
}

// Handle failed password attempts (e.g., trigger a buzzer if the limit is exceeded)
void handleFailedAttempts() {
	if (attempts >= ATTEMPTS_LIMIT) {
		setupTimer1();  // Start Timer1 for delay
		g_tick = 0;
		Buzzer_on();  // Turn on buzzer to alert user about failed attempts
		while (g_tick < 60);  // Keep buzzer on for 1 minute (60 seconds)
		Buzzer_off();  // Turn off the buzzer after 1 minute
		attempts = 0;  // Reset failed attempts counter
		Timer1Stop();  // Stop Timer1
	}
}
