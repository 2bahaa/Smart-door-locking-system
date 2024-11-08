/*
 * timer.h
 *
 *  Created on: Oct 25, 2024
 *      Author: Mohamed Bahaa
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include <avr/interrupt.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define NULL_PTR ((void*)0)  // Define NULL_PTR as a null pointer

/* Define types for timer configuration settings */

// Timer ID Type
typedef enum
{
    TIMER_0 = 0,
    TIMER_1,
    TIMER_2
} Timer_ID_Type;

// Timer Clock Type
typedef enum
{
    TIMER_CLOCK_1 = 1,
    TIMER_CLOCK_8,
    TIMER_CLOCK_64,
    TIMER_CLOCK_256,
    TIMER_CLOCK_1024
} Timer_ClockType;

// Timer Mode Type
typedef enum
{
    TIMER_NORMAL_MODE = 0,
    TIMER_COMPARE_MODE
} Timer_ModeType;
/* Enum for Timer IDs */


/* Timer configuration structure */
typedef struct
{
	uint16 timer_InitialValue;              // Initial timer value
	uint16 timer_compare_MatchValue;        // Compare match value (only used in compare mode)
	Timer_ID_Type timer_ID;                 // Timer ID (e.g., TIMER0, TIMER1, TIMER2)
	Timer_ClockType timer_clock;            // Clock source selection
	Timer_ModeType timer_mode;              // Mode selection (Normal or Compare)
} Timer_ConfigType;



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/* Function declarations */
void Timer_init(const Timer_ConfigType *Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_H_ */
