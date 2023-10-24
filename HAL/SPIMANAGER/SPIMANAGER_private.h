/*
 * SPIMANAGER_private.h
 *
 *  Created on: Oct 24, 2023
 *      Author: ERoshdy
 */

#ifndef SPIMANAGER_PRIVATE_H_
#define SPIMANAGER_PRIVATE_H_

typedef struct
{
    void (*pnf)(void);
    uint8_t msgID;
    uint8_t content;
} SPIObjStruct;

SPIObjStruct gsSPIObjects[32];

uint32_t ulStatus = 0;

bool gbErrFlag = false;


#endif /* SPIMANAGER_PRIVATE_H_ */
