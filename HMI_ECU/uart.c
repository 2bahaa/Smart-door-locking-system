 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/* UART_init to use UART_ConfigType */
void UART_init(const UART_ConfigType *Config_Ptr)
{
    uint16 ubrr_value = 0;

    /* U2X = 1 for double transmission speed */
    UCSRA = (1 << U2X);

    /* Configure UCSRB based on data bits */
    UCSRB = (1 << RXEN) | (1 << TXEN);
    if (Config_Ptr->bit_data == 9) {
        UCSRB |= (1<<URSEL) |(1 << UCSZ2);  // Set for 9-bit data mode if specified
    }

    /* Configure UCSRC for frame format, parity, and stop bits */


    /* Configure data bit size */
    if (Config_Ptr->bit_data == 8) {
        UCSRC |=(1<<URSEL) | (1 << UCSZ1) | (1 << UCSZ0);  // 8-bit data
    } else if (Config_Ptr->bit_data == 7) {
        UCSRC |= (1<<URSEL) |(1 << UCSZ1);                 // 7-bit data
    }
    else if (Config_Ptr->bit_data == 6) {
            UCSRC |= (1<<URSEL) |(1 << UCSZ0);                 // 6-bit data
        }
    else if (Config_Ptr->bit_data == 5) {
            UCSRC &=~ (1 << UCSZ0);                 // 5-bit data
            UCSRC &=~ (1 << UCSZ1);
            UCSRC &=~ (1 << UCSZ2);
        }


    /* Configure parity */
    if (Config_Ptr->parity == 1) {
        UCSRC |= (1 << UPM1);  // Even parity
    } else if (Config_Ptr->parity == 2) {
        UCSRC |= (1 << UPM1) | (1 << UPM0);  // Odd parity
    }

    /* Configure stop bits */
    if (Config_Ptr->stop_bit == 2) {
        UCSRC |= (1 << USBS);  // 2 stop bits
    }

    /* Calculate the UBRR register value for the specified baud rate */
    ubrr_value = (uint16_t)((F_CPU / (Config_Ptr->baud_rate * 8UL)) - 1);

    /* Set UBRR register */
    UBRRH = ubrr_value >> 8;
    UBRRL = ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
