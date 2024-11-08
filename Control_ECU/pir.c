/*
 * pir.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed Bahaa
 */


#include "pir.h"
#include "gpio.h"

/*
 * Description: Function to initialize the PIR sensor.
 */
void PIR_init(void)
{
    // Set the PIR sensor pin as an input
    GPIO_setupPinDirection(PIR_SENSOR_PORT, PIR_SENSOR_PIN, PIN_INPUT);
}

/*
 * Description: Function to get the current state of the PIR sensor.
 *              Returns 1 if motion is detected, 0 if no motion.
 */
uint8 PIR_getState(void)
{
    // Read and return the state of the PIR sensor pin (1 = motion detected, 0 = no motion)
    return GPIO_readPin(PIR_SENSOR_PORT, PIR_SENSOR_PIN);
}
