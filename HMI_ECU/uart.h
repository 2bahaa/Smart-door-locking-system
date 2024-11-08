 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Define types for configuration settings */
typedef uint8 UART_BitDataType;
typedef uint8 UART_ParityType;
typedef uint8 UART_StopBitType;
typedef uint32 UART_BaudRateType;
/* Define UART_ConfigType structure */
typedef struct
{
    UART_BitDataType bit_data;     // Data bits (e.g., 8-bit, 9-bit)
    UART_ParityType parity;        // Parity (e.g., even, odd, none)
    UART_StopBitType stop_bit;     // Stop bits (e.g., 1 or 2)
    UART_BaudRateType baud_rate;   // Baud rate (e.g., 9600, 115200)
} UART_ConfigType;





/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
