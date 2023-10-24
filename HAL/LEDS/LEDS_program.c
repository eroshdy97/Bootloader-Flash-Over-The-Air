/**
 * @file leds.c
 * @brief LED driver program file.
 *
 * This file contains the implementation of the LED driver module for controlling LEDs on a Tiva-C microcontroller.
 *
 * @authors Mohamed Gamal
 * @version V01
 * @date 25/09/2023
 */

#include "stdint.h"
#include "stdbool.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "./HAL/LEDS/LEDS_interface.h"
#include "./HAL/LEDS/LEDS_private.h"
#include "./HAL/LEDS/LEDS_config.h"

/**
 * @brief Initialize the LED driver.
 *
 * This function initializes the GPIO pins connected to LEDs and configures them as outputs.
 */
void LEDS_Init(void)
{
    /* Clock gating for PortF */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /* Set all LEDs as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

/**
 * @brief Turn on an LED.
 *
 * This function turns on the specified LED by setting its GPIO pin high.
 *
 * @param[in] ui8LEDPin GPIO pin corresponding to the LED.
 */
void LEDS_ON(uint8_t ui8LEDPin)
{
    if (ui8LEDPin <= GPIO_PIN_4)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, ui8LEDPin);
    }
}

/**
 * @brief Turn off an LED.
 *
 * This function turns off the specified LED by setting its GPIO pin low.
 *
 * @param[in] ui8LEDPin GPIO pin corresponding to the LED.
 */
void LEDS_OFF(uint8_t ui8LEDPin)
{
    if (ui8LEDPin <= GPIO_PIN_4)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, 0);
    }
}

/**
 * @brief Blink an LED.
 *
 * This function blinks the specified LED by toggling its GPIO pin state with a delay.
 *
 * @param[in] ui8LEDPin GPIO pin corresponding to the LED.
 * @param[in] ui32DelayMs Delay time in milliseconds for each on/off cycle.
 */
void LEDS_Blink(uint8_t ui8LEDPin, uint32_t ui32DelayMs)
{
    if (ui8LEDPin <= GPIO_PIN_4)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, ui8LEDPin);
        SysCtlDelay(ui32DelayMs * 16000 / 3);
        GPIOPinWrite(GPIO_PORTF_BASE, ui8LEDPin, 0);
        SysCtlDelay(ui32DelayMs * 16000 / 3);
    }
}
