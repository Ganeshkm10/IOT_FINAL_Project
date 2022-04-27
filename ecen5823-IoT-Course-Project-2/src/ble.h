/*
 * ble.h
 * @Description:This file contains function declarations for BT stack handling.
 *  Created on : 18-Feb-2022
 *      Author : Ganesh KM
 */

#ifndef SRC_BLE_H_
#define SRC_BLE_H_

#include "stdio.h"
#include <inttypes.h>
#include "stdint.h"
#include "sl_bt_api.h"

//Helper macros

#define UINT8_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); }

#define UINT32_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
 *(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24); }


#define UINT32_TO_FLOAT(m, e) (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))


static const uint16_t MAX_ADVERTISING_INTERVAL = 400;
static const uint16_t MIN_ADVERTISING_INTERVAL = 400;
static const uint16_t MAX_CONNECTION_INTERVAL = 60;
static const uint16_t MIN_CONNECTION_INTERVAL = 60;
static const uint16_t SLAVE_LATENCY = 3;
static const uint16_t SUPERVISION_TIMEOUT = 150;

//CLIENT
static const uint8_t SCAN_TYPE = 0; //Passive scanning : Change to 1 to trigger active scanning
static const uint16_t SCAN_INTERVAL = 0X50;
static const uint16_t SCAN_WINDOW = 0X28;
static const uint16_t MIN_INTERVAL = 60 ;
static const uint16_t MAX_INTERVAL = 60 ;
static const uint16_t TIMEOUT = 75;

//Circular buffer
#define QUEUE_DEPTH      (16)


typedef struct {
 bd_addr Address;
 uint32_t event;
 uint8_t advertisingSetHandle;
 uint8_t IndicationsEnabled;
 uint8_t ConnectionHandle;
 uint8_t HTM_Service[2];
 uint32_t ServiceHandle;
 uint32_t Servicehandle_Button;
 uint32_t Servicehandle_Ambient;
 uint32_t Servicehandle_Gesture;
 uint16_t CharacteristicHandle;
 uint16_t CharacteristicHandle_Button;
 uint16_t CharacteristicHandle_Ambient;
 uint16_t CharacteristicHandle_Gesture;
 uint8_t *Server_Value;
 uint8_t *Ambient_Value_LSB;
 uint8_t *Ambient_Value_MSB;
 bd_addr Server_Address;
 bool button_pressed;
 bool bonded;
 uint32_t passkey;
 bool button_indication;
 bool Indication_in_Flight;

} ble_data_struct_t;

typedef struct{
  uint8_t data[ 2 ];
     uint8_t len;
}GATT_UUID;


//Circular buffer data struct
typedef struct {

    uint16_t  charHandle;   // Char handle from gatt_db.h
    size_t    bufferLength;  // Length of buffer in bytes to send
    uint8_t   buffer[5];        // The actual data buffer for the indication.
                                             // Need space for HTM (5 bytes) and button_state (2 bytes)
                                             // indications, array [0] holds the flags byte.

} queue_struct_t;

ble_data_struct_t *getBLEdata(void);

void handle_ble_event(sl_bt_msg_t *evt);

void Send_ButtonState(uint8_t value);

void Sendindication();

void Sendindication_AmbientLight();

void Sendindication_GestureValue(uint8_t Value);

void Gesture_Main_Logic();

//Circular buffer functions
bool write_queue (queue_struct_t SendMsg);
bool     read_queue ();
void     get_queue_status (uint32_t *_wptr, uint32_t *_rptr, bool *_full, bool *_empty);
uint32_t get_queue_depth (void);

#endif /* SRC_BLE_H_ */


