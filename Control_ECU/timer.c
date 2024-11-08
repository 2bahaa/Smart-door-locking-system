/*
 * timer.c
 *
 *  Created on: Oct 25, 2024
 *      Author: Mohamed Bahaa
 */


#include "timer.h"


/* Global pointers to callback functions for each timer */
static void (*g_timer0CallBackPtr)(void) = NULL_PTR;
static void (*g_timer1CallBackPtr)(void) = NULL_PTR;
static void (*g_timer2CallBackPtr)(void) = NULL_PTR;

/* Timer initialization function */
void Timer_init(const Timer_ConfigType *Config_Ptr) {
	switch (Config_Ptr->timer_ID) {
	case TIMER_0:
		/* Set initial value */
		TCNT0 = Config_Ptr->timer_InitialValue;

		/* Configure mode (Normal or Compare) */
		if (Config_Ptr->timer_mode == 0) {
			TCCR0 |= (1 << FOC0);  // Normal mode
		} else {
			TCCR0 |= (1 << WGM01);  // Compare mode
			OCR0 = Config_Ptr->timer_compare_MatchValue;
		}

		/* Set clock source */
		TCCR0 |= Config_Ptr->timer_clock;

		/* Enable interrupts based on mode */
		if (Config_Ptr->timer_mode == 0) {
			TIMSK |= (1 << TOIE0);  // Overflow interrupt enable for TIMER0
		} else {
			TIMSK |= (1 << OCIE0);  // Output compare match interrupt enable for TIMER0
		}
		break;

	case TIMER_1:
		/* Set initial value */
		TCNT1 = Config_Ptr->timer_InitialValue;

		/* Configure mode (Normal or Compare) */
		if (Config_Ptr->timer_mode == 0) {
			TCCR1B |= (1 << FOC1A);  // Normal mode
		} else {
			TCCR1B |= (1 << WGM12);  // Compare mode
			OCR1A = Config_Ptr->timer_compare_MatchValue;
		}

		/* Set clock source */
		TCCR1B |= Config_Ptr->timer_clock;

		/* Enable interrupts based on mode */
		if (Config_Ptr->timer_mode == 0) {
			TIMSK |= (1 << TOIE1);  // Overflow interrupt enable for TIMER1
		} else {
			TIMSK |= (1 << OCIE1A);  // Output compare match interrupt enable for TIMER1
		}
		break;

	case TIMER_2:
		/* Set initial value */
		TCNT2 = Config_Ptr->timer_InitialValue;

		/* Configure mode (Normal or Compare) */
		if (Config_Ptr->timer_mode == 0) {
			TCCR2 |= (1 << FOC2);  // Normal mode
		} else {
			TCCR2 |= (1 << WGM21);  // Compare mode
			OCR2 = Config_Ptr->timer_compare_MatchValue;
		}

		/* Set clock source */
		TCCR2 |= Config_Ptr->timer_clock;

		/* Enable interrupts based on mode */
		if (Config_Ptr->timer_mode == 0) {
			TIMSK |= (1 << TOIE2);  // Overflow interrupt enable for TIMER2
		} else {
			TIMSK |= (1 << OCIE2);  // Output compare match interrupt enable for TIMER2
		}
		break;
	}
}

/* Timer de-initialization function */
void Timer_deInit(Timer_ID_Type timer_type) {
	switch (timer_type) {
	case TIMER_0:
		TCCR0 = 0;
		TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));  // Disable TIMER0 interrupts
		break;
	case TIMER_1:
		TCCR1B = 0;
		TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A));  // Disable TIMER1 interrupts
		break;
	case TIMER_2:
		TCCR2 = 0;
		TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));  // Disable TIMER2 interrupts
		break;
	}
}

/* Set callback function for timer interrupts */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID) {
	switch (a_timer_ID) {
	case TIMER_0:
		g_timer0CallBackPtr = a_ptr;
		break;
	case TIMER_1:
		g_timer1CallBackPtr = a_ptr;
		break;
	case TIMER_2:
		g_timer2CallBackPtr = a_ptr;
		break;
	}
}

/* ISR for TIMER0 overflow */
ISR(TIMER0_OVF_vect) {
	if (g_timer0CallBackPtr != NULL_PTR) {
		(*g_timer0CallBackPtr)();
	}
}

/* ISR for TIMER0 compare match */
ISR(TIMER0_COMP_vect) {
	if (g_timer0CallBackPtr != NULL_PTR) {
		(*g_timer0CallBackPtr)();
	}
}

/* ISR for TIMER1 overflow */
ISR(TIMER1_OVF_vect) {
	if (g_timer1CallBackPtr != NULL_PTR) {
		(*g_timer1CallBackPtr)();
	}
}

/* ISR for TIMER1 compare match */
ISR(TIMER1_COMPA_vect) {
	if (g_timer1CallBackPtr != NULL_PTR) {
		(*g_timer1CallBackPtr)();
	}
}

/* ISR for TIMER2 overflow */
ISR(TIMER2_OVF_vect) {
	if (g_timer2CallBackPtr != NULL_PTR) {
		(*g_timer2CallBackPtr)();
	}
}

/* ISR for TIMER2 compare match */
ISR(TIMER2_COMP_vect) {
	if (g_timer2CallBackPtr != NULL_PTR) {
		(*g_timer2CallBackPtr)();
	}
}
