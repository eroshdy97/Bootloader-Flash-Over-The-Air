/*
 * SPIMANAGER_program.c
 *
 *  Created on: Oct 24, 2023
 *      Author: ERoshdy
 */


#include "stdbool.h"
#include "stdint.h"

#include "inc/hw_ssi.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

/* Include CAN Manager specific files */
#include "./HAL/SPIMANAGER/SPIMANAGER_config.h"
#include "./HAL/SPIMANAGER/SPIMANAGER_private.h"
#include "./HAL/SPIMANAGER/SPIMANAGER_interface.h"

//! - SSI0CLK - PA2     CLK
//! - SSI0Fss - PA3     SS
//! - SSI0Rx  - PA4     MOSI
//! - SSI0Tx  - PA5     MISO

void SPIMANAGER_IntHandler(void)
{
    ulStatus = SSIIntStatus(SSI0_BASE, 0);

    if(ulStatus & SSI_RXTO)
    {
        uint16_t dataElement = 0;
        SSIDataGet(SSI0_BASE, & dataElement);
        uint8_t msgID = (uint8_t)((dataElement >> 8) & 0xFF);

        if(gsSPIObjects[msgID].pnf != 0)
        {
            gsSPIObjects[msgID].content = (uint8_t)(dataElement & 0xFF);
            gsSPIObjects[msgID].pnf();
            gbErrFlag = false;
        }
    }
    else
    {
        gbErrFlag = true;
    }

    SSIIntClear(SSI0_BASE, ulStatus);
}

void SPIMANAGER_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_SLAVE, 1300000, 16);

    SSIEnable(SSI0_BASE);

    SSIIntEnable(SSI0_BASE, SSI_RXTO);

    IntEnable(INT_SSI0);

}

void SPIMANAGER_MsgReceiveSet(uint8_t u8MsgID, void (*pFn)(void))
{
    gsSPIObjects[u8MsgID].msgID = u8MsgID;
    gsSPIObjects[u8MsgID].pnf = pFn;
}

bool SPIMANAGER_MsgReceiveGet(uint8_t u8MsgID, uint8_t *pu8Data)
{
    if (gbErrFlag)
    {
        gbErrFlag = false;
        return false;
    }
    else
    {
        *pu8Data = gsSPIObjects[u8MsgID].content;
        return true;
    }
}
