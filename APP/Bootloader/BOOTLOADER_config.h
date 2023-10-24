/**
 * @file bootloader_config.h
 * @brief BOOTLOADER driver configuration file.
 *
 * This file contains configuration settings for the BOOTLOADER driver module,
 * including SPI message IDs, memory addresses, and timeouts.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 24/10/2023
 */

#ifndef _BOOTLOADER_CONFIG_H
#define _BOOTLOADER_CONFIG_H

/* SPI message IDs */
#define BOOTLOADER_SPI_MSG_ID_RESET             0x00 /**< SPI message ID for the reset command. */
#define BOOTLOADER_SPI_MSG_ID_START             0x01 /**< SPI message ID for the start command. */
#define BOOTLOADER_SPI_MSG_ID_END               0x02 /**< SPI message ID for the end command. */
#define BOOTLOADER_SPI_MSG_ID_DATA              0x03 /**< SPI message ID for data frames. */

/* SPI addresses to flash */
#define BOOTLOADER_ADDRESS_FLAG                 0x00004C00 /**< Flash memory address for flag storage (1 KB). */
#define BOOTLOADER_ADDRESS_BANK_1               0x00005000 /**< Flash memory address for the first application bank (86 KB). */
//#define BOOTLOADER_ADDRESS_BANK_2               0x0001A800 /**< Flash memory address for the second application bank (150 KB). */

/* Maximum time (in milliseconds) to wait for receiving the START command */
#define BOOTLOADER_TIMEOUT_START_MS             10 /**< Timeout duration for receiving the START message. */

#endif
