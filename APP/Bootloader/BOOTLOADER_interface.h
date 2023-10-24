/**
 * @file bootloader_interface.h
 * @brief BOOTLOADER driver interface file.
 *
 * This file contains function prototypes and definitions for the BOOTLOADER driver module.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
 */

#ifndef _BOOTLOADER_INTERFACE_H
#define _BOOTLOADER_INTERFACE_H

/**
 * @brief Initialize the BOOTLOADER driver.
 *
 * This function initializes the BOOTLOADER driver, including the initialization of
 * CAN communication and LED indicators.
 */
void BOOTLOADER_Init(void);

/**
 * @brief Start the BOOTLOADER operation.
 *
 * This function starts the BOOTLOADER operation. It checks whether to jump to the
 * application code or enter the BOOTLOADER mode based on the received commands
 * over the CAN bus. If needed, it receives data, validates CRC, and writes the
 * application to flash memory.
 */
void BOOTLOADER_Start(void);

#endif
