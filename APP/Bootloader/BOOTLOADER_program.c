/**
 * @file bootloader.c
 * @brief BOOTLOADER driver program file.
 *
 * This file contains the implementation of the BOOTLOADER driver module for a Tiva-C microcontroller.
 * It is responsible for receiving firmware updates over the CAN bus, validating the received data, and
 * programming it into flash memory.
 *
 * @authors Ehab Roshdy
 * @version V01
 * @date 24/10/2023
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"

#include "HAL/SPIMANAGER/SPIMANAGER_interface.h"
#include "HAL/LEDS/LEDS_interface.h"

#include "./APP/BOOTLOADER/BOOTLOADER_config.h"
#include "./APP/BOOTLOADER/BOOTLOADER_private.h"
#include "./APP/BOOTLOADER/BOOTLOADER_interface.h"

/**
 * @brief Write application data to flash memory.
 *
 * This function erases flash memory and writes the provided data to the specified address.
 *
 * @param[in] pu32Src Pointer to the source data.
 * @param[in] u32DstAddr Destination address in flash memory.
 * @param[in] u32Count Size of the data to be written.
 */
static void WriteAppToFlash(uint32_t *pu32Src, uint32_t u32DstAddr, uint32_t u32Count)
{
    int i = 0;
    for (i = 0; i < u32Count; i += 1024)
    {
        FlashErase(u32DstAddr + i);
    }
    FlashProgram(pu32Src, u32DstAddr, u32Count);
}

/**
 * @brief Reset callback function.
 *
 * This function is called when a reset command is received over the CAN bus.
 * It resets the microcontroller.
 */
static void ResetCallBack(void)
{
    SysCtlReset();
}

/**
 * @brief Data callback function.
 *
 * This function is called when a data frame is received over the CAN bus.
 * It sets a flag to indicate that a data frame has been received and increments the data received length.
 */
static void DataCallBack(void)
{
    gbDataFrameReceived = true;
    ++gu32DataReceivedLength;
}

/**
 * @brief Start callback function.
 *
 * This function is called when a start command is received over the CAN bus.
 * It sets the current state to receiving data.
 */
static void StartCallBack(void)
{
    geCurruntState = BLStateReceivingData;
}

/**
 * @brief End callback function.
 *
 * This function is called when an end command is received over the CAN bus.
 * It sets the current state to done.
 */
static void EndCallBack(void)
{
    geCurruntState = BLStateDone;
}

/**
 * @brief Jump to the application code.
 *
 * This function jumps to the application code at the specified address.
 *
 * @param[in] u32Address2jmp Address to jump to.
 */
static void JumpToApp()
{
    LEDS_OFF(RED_LED | GREEN_LED | BLUE_LED);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOF);
    VTABLE_OFFSET_R |= BOOTLOADER_ADDRESS_BANK_1;

    __asm(" mov r0, #0x00005000\n");
    __asm(" ldr r1, [r0, #4]");
    __asm(" bx r1\n");
}

/**
 * @brief Set the application flag to run.
 *
 * This function sets the application flag in flash memory to indicate which application to run.
 *
 * @param[in] u32Flag Flag value to set.
 */
static void SetAppFlagToRun(uint32_t u32Flag)
{
    FlashErase(BOOTLOADER_ADDRESS_FLAG);
    FlashProgram(&u32Flag, BOOTLOADER_ADDRESS_FLAG, sizeof(u32Flag));
}

/**
 * @brief Bank switching handler.
 *
 * This function handles the bank switching process for flashing data.
 *
 * @param[in] pu32FlashToBank Pointer to the value indicating the bank to flash data to.
 */
static void BankSwitchingHandler()
{
    /* Write the received application data to flash memory for Bank 1 */
    WriteAppToFlash(gu8DataReceived, BOOTLOADER_ADDRESS_BANK_1, gu32DataReceivedLength);

    /* Reset the data received length counter */
    gu32DataReceivedLength = 0;

    /* Set the flag to indicate that Bank 1 should be run */
    SetAppFlagToRun(1);

    /* Turn off the red LED and turn on the green LED to indicate success */
    LEDS_OFF(RED_LED);
    LEDS_ON(GREEN_LED);

    /* Jump to the application code in Bank 1 */
    JumpToApp();
}

/**
 * @brief Initialize the BOOTLOADER module.
 *
 * This function initializes the CAN Manager and LED modules used by the BOOTLOADER module.
 */
void BOOTLOADER_Init(void)
{
    SPIMANAGER_Init();
    LEDS_Init();
}

/**
 * @brief Start the BOOTLOADER module.
 *
 * This function is the main entry point for the BOOTLOADER module. It sets up communication via the CAN bus,
 * receives firmware updates, validates them, and programs them into flash memory. After successful programming,
 * it sets a flag to indicate the application to run and performs a reset to start the new application.
 */
void BOOTLOADER_Start(void)
{
    /* Initialize LEDs to show bootloader operation */
    LEDS_ON(RED_LED | GREEN_LED);

    uint8_t u8SpiFrameData;
    uint32_t u32ProgramToRun = PROGRAM_TO_RUN_R;

    /* Set up CAN message receive callbacks */
    SPIMANAGER_MsgReceiveSet(BOOTLOADER_SPI_MSG_ID_RESET, ResetCallBack);
    SPIMANAGER_MsgReceiveSet(BOOTLOADER_SPI_MSG_ID_START, StartCallBack);
    SPIMANAGER_MsgReceiveSet(BOOTLOADER_SPI_MSG_ID_DATA , DataCallBack );
    SPIMANAGER_MsgReceiveSet(BOOTLOADER_SPI_MSG_ID_END  , EndCallBack  );

    /* Wait for a timeout to start receiving data */
    uint32_t u32TimeOutCounter = BOOTLOADER_TIMEOUT_START_MS * 3180;
    while (geCurruntState == BLStateIdle && (u32TimeOutCounter > 0))
        u32TimeOutCounter--;

    /* If the timeout occurred and a program is marked to run, jump to it */
    if (u32TimeOutCounter == 0 && u32ProgramToRun == 1)
    {
        JumpToApp(BOOTLOADER_ADDRESS_BANK_1);
    }
    else
    {
        /* Wait for data reception to start */
        while (geCurruntState == BLStateIdle);
        LEDS_OFF(RED_LED | GREEN_LED);
        LEDS_ON(BLUE_LED);

        /* Receive and process data frames */
        while (geCurruntState == BLStateReceivingData)
        {
            if (gbDataFrameReceived)
            {
                SPIMANAGER_MsgReceiveGet(BOOTLOADER_SPI_MSG_ID_DATA, &u8SpiFrameData);
                gu8DataReceived[gu32DataReceivedLength - 1] = u8SpiFrameData;
                gbDataFrameReceived = false;
            }
        }
        LEDS_OFF(BLUE_LED);
        LEDS_ON(RED_LED);

        /* Write the received data to the appropriate flash bank and set the flag to run it */
        BankSwitchingHandler();
    }
    while (1);
}
