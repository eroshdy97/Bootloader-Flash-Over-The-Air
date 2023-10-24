/*
 * SPIMANAGER_interface.h
 *
 *  Created on: Oct 24, 2023
 *      Author: ERoshdy
 */

#ifndef SPIMANAGER_INTERFACE_H_
#define SPIMANAGER_INTERFACE_H_

void SPIMANAGER_Init(void);
void SPIMANAGER_MsgReceiveSet(uint8_t u8MsgID, void (*pFn)(void));
bool SPIMANAGER_MsgReceiveGet(uint8_t u8MsgID, uint8_t *pu8Data);

#endif /* SPIMANAGER_INTERFACE_H_ */
