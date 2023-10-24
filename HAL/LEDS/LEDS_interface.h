/**
 * @file leds_interface.h
 * @brief LED driver interface file.
 *
 * This file contains the interface (function prototypes) for the LED driver module
 * for controlling LEDs on a Tiva-C microcontroller.
 *
 * @authors Mohamed Gamal
 * @version V01
 * @date 25/09/2023
 */

#ifndef _LEDS_INTERFACE_H
#define _LEDS_INTERFACE_H

/* Defines */

/**
 * @brief Red LED identifier.
 */
#define RED_LED             0x00000002

/**
 * @brief Blue LED identifier.
 */
#define BLUE_LED            0x00000004

/**
 * @brief Green LED identifier.
 */
#define GREEN_LED           0x00000008

/* Function Prototypes */

/**
 * @brief Initialize the LED driver.
 *
 * This function initializes the GPIO pins connected to LEDs and configures them as outputs.
 */
void LEDS_Init(void);

/**
 * @brief Turn on an LED.
 *
 * This function turns on the specified LED by setting its GPIO pin high.
 *
 * @param[in] ui8LEDPin GPIO pin corresponding to the LED.
 */
void LEDS_ON(uint8_t ui8LEDPin);

/**
 * @brief Turn off an LED.
 *
 * This function turns off the specified LED by setting its GPIO pin low.
 *
 * @param[in] ui8LEDPin GPIO pin corresponding to the LED.
 */
void LEDS_OFF(uint8_t ui8LEDPin);

/**
 * @brief Blink a LED.
 *
 * This function turns off and on the specified LED.
 *
 * @param[in] ui8LEDPin GPIO pin corresponding to the LED.
 * @param[in] ui32DelayMs Delay between blinks.
 */
void LEDS_Blink(uint8_t ui8LEDPin, uint32_t ui32DelayMs);

#endif /* _LEDS_INTERFACE_H */
