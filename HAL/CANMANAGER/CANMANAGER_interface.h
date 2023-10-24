///**
// * @file canmanager_interface.h
// * @brief CAN Manager driver interface file.
// *
// * This file contains the interface (function prototypes) for the CAN Manager module
// * for Tiva-C microcontroller.
// *
// * @author Mohamed Gamal
// * @version V01
// * @date 25/09/2023
// */
//
//#ifndef _CANMANGER_INTERFACE_H
//#define _CANMANGER_INTERFACE_H
//
///* Global variables - No global variables are defined in this header file. */
//
///* Function Prototypes */
//
///**
// * @brief Delay in microseconds.
// *
// * This function introduces a delay in microseconds.
// *
// * @param[in] us The delay time in microseconds.
// */
//void CANMANAGER_Delay_us(uint32_t us);
//
///**
// * @brief Initialize CAN Manager.
// *
// * This function initializes the CAN Manager by configuring GPIO, CAN, interrupts, and enabling the CAN peripheral.
// */
//void CANMANAGER_Init(void);
//
///**
// * @brief Transmit a CAN message.
// *
// * This function transmits a CAN message with the provided parameters.
// *
// * @param[in] ui32MsgID Message ID.
// * @param[in] ui8MsgLength Message length.
// * @param[in] pui32Data Pointer to message data.
// * @param[in] ui8CANControllerId CAN controller ID.
// * @param[in] pFn Callback function to be associated with this message.
// * @return true if the transmission is successful, false otherwise.
// */
//bool CANMANAGER_Transmit(uint32_t ui32MsgID, uint8_t ui8MsgLength, uint32_t *pui32Data, uint8_t ui8CANControllerId, void (*pFn)(void));
//
///**
// * @brief Set up a CAN object to receive messages.
// *
// * This function sets up a CAN object to receive messages with the provided parameters and associates a callback function.
// *
// * @param[in] u32MsgID Message ID.
// * @param[in] u8MsgLength Message length.
// * @param[out] pu32Data Pointer to data storage.
// * @param[in] u8CANControllerId CAN controller ID.
// * @param[in] pFn Callback function to be associated with this object.
// */
//void CANMANAGER_ObjReceiveSet(uint32_t u32MsgID, uint8_t u8MsgLength, uint32_t *pu32Data, uint8_t u8CANControllerId, void (*pFn)(void));
//
///**
// * @brief Receive a CAN message.
// *
// * This function receives a CAN message and stores it in the provided data pointer.
// *
// * @param[out] pu32Data Pointer to store the received message data.
// * @param[in] u8CANControllerId CAN controller ID.
// * @return true if the reception is successful, false otherwise.
// */
//bool CANMANAGER_ObjReceiveGet(uint32_t *pu32Data, uint8_t u8CANControllerId);
//
//#endif /* _CANMANGER_INTERFACE_H */
