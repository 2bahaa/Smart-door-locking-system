/*
 * pir.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed Bahaa
 */

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Define the PIR sensor pin connected to CONTROL_ECU */
#define PIR_SENSOR_PORT PORTC_ID  // Example port, adjust as needed
#define PIR_SENSOR_PIN  PIN2_ID   // Example pin, adjust as needed

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Function to initialize the PIR sensor.
 */
void PIR_init(void);

/*
 * Description: Function to get the current state of the PIR sensor.
 *              Returns 1 if motion is detected, 0 if no motion.
 */
uint8 PIR_getState(void);


#endif /* PIR_H_ */
