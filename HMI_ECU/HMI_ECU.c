/*
 * main.c
 *
 *  Created on: Oct 25, 2024
 *      Author: Mohamed Bahaa
 */

// HMI_ECU.c
#include "main.h"

// Global variable to track time ticks for timer management
volatile uint16 g_tick = 0;

// Timer1 callback function to increment tick count
void Timer1_callback(void) {
	g_tick++;  // Increment the tick counter on each timer compare match
}

// Timer setup function to initialize Timer1 with a 1-second interval
void setupTimer1(void) {
	Timer_ConfigType timerConfig = {0, 7812, TIMER_1, TIMER_CLOCK_1024, TIMER_COMPARE_MODE};  // Configuring timer for 1-second interval with 8MHz clock
	Timer_init(&timerConfig);  // Initialize timer with specified configuration
	Timer_setCallBack(Timer1_callback, TIMER_1);  // Set the callback function for Timer1
}

// Stop Timer1 function to deinitialize the timer
void Timer1Stop(void) {
	Timer_deInit(TIMER_1);  // Deinitialize Timer1 to stop it
}

int main(void) {
	// Initialize system peripherals
	initializeSystem();
	sei();  // Enable global interrupts

	// Display initial screen with system information
	LCD_displayString("Smart Door");
	LCD_displayStringRowColumn(1, 0, "locking system");
	_delay_ms(1000);  // Short delay for screen visibility
	LCD_clearScreen();
	LCD_displayString("DONE BY:");
	LCD_displayStringRowColumn(1, 0, "Mohamed Bahaa");
	_delay_ms(1000);

	// Start password creation process
	createPassword();

	while (1) {
		if (isPasswordSet) {
			// Display main menu if password is set
			mainMenu();
			uint8 option = KEYPAD_getPressedKey();  // Get user input for menu option
			_delay_ms(500);  // Debounce delay to avoid multiple key press detection

			// Handle menu options for opening door or changing password
			if (option == COMMAND_OPEN_DOOR || option == COMMAND_CHANGE_PASSWORD) {
				handleOperation(option);
			} else {
				LCD_clearScreen();
				LCD_displayStringRowColumn(1, 0, "Invalid Option");  // Invalid option feedback
				_delay_ms(1000);
			}
		} else {
			// If password is not set, prompt to create password again
			createPassword();
		}
	}
}

// System initialization function for peripherals and UART setup
void initializeSystem() {
	LCD_init();  // Initialize LCD for display
	UART_ConfigType uartConfig = {8, 0, 1, 9600};  // UART configuration for 9600 baud rate
	UART_init(&uartConfig);  // Initialize UART with specified configuration
}

// Function to create and set the password
void createPassword() {
	while (1) {
		// Get the two passwords from the user
		enterPasswords(password1, password2);

		// Send the entered passwords to Control_ECU for verification
		UART_sendByte(START_COMMUNICATION);
		for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
			UART_sendByte(password1[i]);  // Send first password
		}
		UART_sendByte(START_COMMUNICATION);  // Send delimiter for second password
		for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
			UART_sendByte(password2[i]);  // Send second password
		}

		// Receive the match result from Control_ECU
		uint8 match = UART_recieveByte();
		if (match == 1) {
			// If passwords match, password set successfully
			LCD_clearScreen();
			LCD_displayString("Password Set!");
			_delay_ms(1000);
			isPasswordSet = 1;  // Mark password as set
			break;
		} else if (match == 0) {
			// If passwords do not match, ask to try again
			LCD_clearScreen();
			LCD_displayString("Mismatch!");
			LCD_displayStringRowColumn(1, 0, "Try Again");
			_delay_ms(1000);
			isPasswordSet = 0;  // Password setting failed, reset flag
			UART_sendByte(TRY_AGAIN);  // Notify Control_ECU to retry
		}
	}
}

// Function to display the main menu for door operation options
void mainMenu() {
	LCD_clearScreen();
	LCD_displayString("(+) Open Door");
	LCD_displayStringRowColumn(1, 0, "(-) Change Pass");
}

// Function to handle door operation based on selected command (open door/change password)
void handleOperation(uint8 command) {
	attempts = 0;  // Reset failed attempts count
	while (attempts < ATTEMPTS_LIMIT) {
		// Prompt user to enter password
		enterPassword(enteredPassword, "Enter Password:");
		UART_sendByte(command);  // Send the selected command (open door or change password)
		_delay_ms(100);  // Brief delay for synchronization
		sendPasswordToControlECU(enteredPassword);  // Send the entered password to Control_ECU for verification

		uint8 response = UART_recieveByte();  // Receive password verification response from Control_ECU
		if (response == 1) {
			// If password is correct, perform operation (open door or change password)
			LCD_clearScreen();
			if (command == COMMAND_OPEN_DOOR) {
				// If opening door, run the unlocking procedure
				LCD_displayString("Door is");
				LCD_moveCursor(1, 0);
				LCD_displayString("Unlocking...");
				setupTimer1();  // Start timer for unlocking operation
				g_tick = 0;
				while (g_tick < 15);  // Wait for 15 seconds for motor operation
				g_tick = 0;
				Timer1Stop();  // Stop timer
				displayWaitMessage();  // Display message to wait while people enter
			} else if (command == COMMAND_CHANGE_PASSWORD) {
				// If changing password, re-initiate password creation
				createPassword();
			}
			return;
		} else {
			// If password is incorrect, increment attempt counter and show error message
			LCD_clearScreen();
			LCD_displayString("Incorrect Pass!");
			_delay_ms(500);
			attempts++;
		}
	}
	// If failed attempts exceed the limit, lock the system
	handleFailedAttempts();
}

// Function to handle system lockout after failed attempts
void handleFailedAttempts() {
	if (attempts >= ATTEMPTS_LIMIT) {
		setupTimer1();  // Start timer for lockout duration
		g_tick = 0;
		LCD_clearScreen();
		LCD_displayString("System Locked!");
		while (g_tick < 60);  // Lock the system for 60 seconds (1 minute)
		attempts = 0;  // Reset attempts count
		Timer1Stop();  // Stop timer
	}
}

// Function to prompt the user to enter two passwords (for creation or verification)
void enterPasswords(uint8 *passwordBuffer1, uint8 *passwordBuffer2) {
	uint8 key1, key2;
	LCD_clearScreen();
	LCD_displayString("Create pass :)");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1, 0);

	// User enters the first password
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		*(passwordBuffer1 + i) = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');  // Display '*' for each entered character
		_delay_ms(300);  // Delay for key debounce
	}

	// Wait for user to press the Enter key to finish entering the first password
	while (key1 != ENTER_BUTTON) {
		key1 = KEYPAD_getPressedKey();
		_delay_ms(500);
	}

	// Prompt to re-enter the password for verification
	LCD_clearScreen();
	LCD_displayString("Plz re-enter");
	LCD_moveCursor(1, 0);
	LCD_displayString("same pass:");
	LCD_moveCursor(1, 10);
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		*(passwordBuffer2 + i) = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(300);
	}

	// Wait for user to press Enter key again
	while (key2 != ENTER_BUTTON) {
		key2 = KEYPAD_getPressedKey();
		_delay_ms(500);
	}
}

// Function to prompt the user to enter a password for operation (unlock door or change password)
void enterPassword(uint8 *passwordBuffer, const char *prompt) {
	uint8 key;
	LCD_clearScreen();
	LCD_displayString(prompt);  // Display the prompt for user input
	LCD_moveCursor(1, 0);

	// User enters the password
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		*(passwordBuffer + i) = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');  // Display '*' for each entered character
		_delay_ms(300);  // Delay for key debounce
	}

	// Wait for user to press Enter key to finish entering password
	while (key != ENTER_BUTTON) {
		key = KEYPAD_getPressedKey();
		_delay_ms(500);
	}
}

// Function to send the entered password to the Control_ECU for verification
void sendPasswordToControlECU(uint8 password[]) {
	UART_sendByte(START_COMMUNICATION);  // Send start communication signal
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		UART_sendByte(password[i]);  // Send each password character
	}
}

// Function to display a message while waiting for door lock operation to complete
void displayWaitMessage() {
	LCD_clearScreen();
	LCD_displayString("Wait for people");
	LCD_displayStringRowColumn(1, 0, "to enter");
	_delay_ms(1000);
	while (1) {
		uint8 pirStatus = UART_recieveByte();  // Check PIR sensor status from Control_ECU
		if (pirStatus == 0) {
			LCD_clearScreen();
			LCD_displayString("Door is");
			LCD_displayStringRowColumn(1, 0, "locking...");
			setupTimer1();
			g_tick = 0;
			while (g_tick < 15);  // Wait for 15 seconds for motor operation
			g_tick = 0;
			Timer1Stop();
			break;
		}
	}
}
