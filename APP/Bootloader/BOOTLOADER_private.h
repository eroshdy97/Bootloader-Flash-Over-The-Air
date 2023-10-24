/**
 * @file bootloader_private.h
 * @brief BOOTLOADER driver private file.
 *
 * This file contains private elements, including enum definitions, static global variables,
 * and static function prototypes used internally by the BOOTLOADER driver module.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 25/09/2023
 */

#ifndef _BOOTLOADER_PRIVATE_H
#define _BOOTLOADER_PRIVATE_H

/* Enum for boot-loader states */
typedef enum
{
    BLStateIdle,           /**< The boot-loader is in the idle state.           */
    BLStateReceivingData,  /**< The boot-loader is receiving data.              */
    BLStateDone,           /**< The boot-loader has completed its operation.    */
} BLStates;

/* Register definitions related to the boot-loader */
#define VTABLE_OFFSET_R                 (*((volatile unsigned long *)0xE000ED08))               /**< VTABLE Offset Register.  */
#define PROGRAM_TO_RUN_R                (*((volatile unsigned long *)BOOTLOADER_ADDRESS_FLAG))  /**< Program to Run Register. */

/* Static global variables */

static BLStates geCurruntState = BLStateIdle;              /**< Current state of the boot-loader.                   */
static bool gbDataFrameReceived = false;                   /**< Flag to indicate if a data frame has been received. */
static uint32_t gu32FlashToBank = 0;                       /**< Value indicating which bank to flash data to.       */
static uint32_t gu32DataReceivedLength = 0;                /**< Length of data received.                            */
static uint8_t gu8DataReceived[6 * 4 * 1024];              /**< Buffer to store received data.                      */

/* Static function prototypes */

/**
 * @brief Write application data to flash memory.
 *
 * This function erases flash memory and programs application data into it.
 *
 * @param[in] pu32Src Pointer to the source data to be written.
 * @param[in] u32DstAddr Destination address in flash memory.
 * @param[in] u32Count Number of bytes to write.
 */
static void WriteAppToFlash(uint32_t *pu32Src, uint32_t u32DstAddr, uint32_t u32Count);

/**
 * @brief Reset callback function.
 *
 * This callback function is executed when a reset command is received over the CAN bus.
 */
static void ResetCallBack(void);

/**
 * @brief Data callback function.
 *
 * This callback function is executed when a data frame is received over the CAN bus.
 */
static void DataCallBack(void);

/**
 * @brief Start callback function.
 *
 * This callback function is executed when a start command is received over the CAN bus.
 */
static void StartCallBack(void);

/**
 * @brief End callback function.
 *
 * This callback function is executed when an end command is received over the CAN bus.
 */
static void EndCallBack(void);

/**
 * @brief Jump to the application code.
 *
 * This function performs a jump to the application code located at the specified address.
 *
 */
static void JumpToApp();

/**
 * @brief Bank switching handler.
 *
 * This function handles the bank switching process for flashing data.
 *
 */
static void BankSwitchingHandler();

#endif
