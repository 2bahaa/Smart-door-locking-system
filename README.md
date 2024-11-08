# Dual Microcontroller-Based Door Locker Security System Using Password Authentication
Objective:
The objective of this project is to design and implement a secure and efficient smart door control system using two microcontrollers (HMI_ECU and Control_ECU) that communicate via UART. The system integrates password authentication, EEPROM storage, motion detection via a PIR sensor, motor control through an H-bridge, and a buzzer for alerts, ensuring robust security features.

**Project Overview:**
This smart door control system utilizes two separate microcontrollers. The HMI_ECU functions as the interface for user interaction, allowing password entry and system control. The Control_ECU handles motor operations, stores data in external EEPROM, and responds to commands from the HMI_ECU.

**The system features:**

**Password Protection:** Secure access to door unlocking and system options.
**LCD and Keypad Interface:** A user-friendly interface for entering and managing passwords.
**UART Communication: **Communication between the HMI_ECU and Control_ECU.
**External EEPROM Storage:** Secure storage for passwords and system data.
**Motorized Door Control:** A motor controls the door via an H-bridge.
**PIR Sensor:** Detects motion and manages door operation based on user presence.
**Buzzer Alerts:** Activates in case of failed password attempts or system alerts.

**Features:**
**Password Protection:** Users can set and verify a password, which is securely stored in EEPROM.
**LCD and Keypad Interface:** Allows users to input and manage passwords via an LCD and keypad.
**UART Communication:** Data is transmitted between the two ECUs via UART.
**EEPROM Storage:** Passwords and other system data are stored in an external EEPROM.
**Motorized Door Control:** Door opening and closing are controlled by a motor and H-bridge driver.
**Buzzer Alert:** A buzzer sounds for failed password attempts or security events.
**PIR Motion Sensor: **The system detects motion and holds the door open for entering users.
**Password Change Option:** Users can change their password after verification.
**Security Lock:** The system locks for 1 minute after three consecutive incorrect password attempts.


**Hardware Components:**
**HMI_ECU Connections:**
LCD (8-bit mode)
RS pin connected to PC0
E pin connected to PC1
Data Pins (D0-D7) connected to Port A (PA0 to PA7)
Keypad (4x4)
Rows connected to PB0, PB1, PB2, PB3
Columns connected to PB4, PB5, PB6, PB7
UART Communication
TXD connected to RXD of Control_ECU
RXD connected to TXD of Control_ECU
Control_ECU Connections:
External EEPROM (I2C Communication)
SCL connected to PC0
SDA connected to PC1
Buzzer
Connected to PC7
H-bridge Motor Driver
Input 1 connected to PD6
Input 2 connected to PD7
Enable1 connected to PB3/OC0
Motor (for Door Control)
Connected to the H-bridge motor driver
PIR Motion Sensor
Connected to PC2
Operation Steps:
Step 1 – Create a System Password:
The LCD prompts the user to enter a 5-digit password, which is shown as * on the screen.
After confirmation, the system saves the password in EEPROM.
If the passwords match, proceed to Step 2. If they don’t, prompt for the password again.
Step 2 – Main Options:
The LCD displays the main system options.
Step 3 – Open Door:
The user enters a password to unlock the door.
If the password matches the stored one, the motor rotates to unlock the door for 15 seconds.
The door remains open as long as motion is detected by the PIR sensor. When motion stops, the door locks after 15 seconds.
Step 4 – Change Password:
The user can change the password by re-entering the correct old password.
If the new password matches, the system updates the EEPROM.
Step 5 – Security Lock:
If the password is entered incorrectly three times, the system locks for 1 minute, and a buzzer sounds for alerts.
The LCD displays an error message during the lockout period, and no further input is accepted.



**System Requirements:**
System Frequency: 8 MHz
Microcontroller: ATmega32
Architecture: Two-layered architecture with HMI_ECU and Control_ECU


**Driver Requirements:**
GPIO Driver:
Use the GPIO driver from the course for both ECUs.
UART Driver:
Modify the UART_init function to accept a pointer to the UART_ConfigType structure.
LCD Driver:
Use a 2x16 LCD connected to the HMI_ECU.
Keypad Driver:
Use a 4x4 keypad for user interaction with the HMI_ECU.
I2C Driver:
Implement the I2C driver for communication between Control_ECU and external EEPROM.
PWM Driver:
Implement PWM for motor control using Timer0.
Timer Driver:
Implement a timer driver for the management of door and display operations.
Buzzer Driver:
Implement the buzzer driver to handle system alerts.
PIR Driver:
Implement a PIR sensor driver to detect motion and manage door operations.
DC Motor Driver:
Implement motor control using Timer0 PWM.
EEPROM Driver:
Implement the EEPROM driver to manage data storage via I2C.
