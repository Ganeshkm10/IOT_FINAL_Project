/*
 * ble_device_type.h
 *
 *  Created on: Feb 16, 2019
 *      Author: dan walkes
 *
 *      Editor: Mar 14, 2021, Dave Sluiter
 *      Change: Added some vertical white space and bd_addr array indices
 */

#ifndef SRC_BLE_DEVICE_TYPE_H_
#define SRC_BLE_DEVICE_TYPE_H_
#include <stdbool.h>

/*
 * Students:
 * Set to 1 to configure this build as a BLE server.
 * Set to 0 to configure as a BLE client
 */
#define DEVICE_IS_BLE_SERVER 1



// For your Bluetooth Client implementations.
// Set this #define to the bd_addr of the Gecko that will be your Server.
//                   bd_addr  [0]   [1]   [2]   [3]   [4]   [5] <- array indices
//#define SERVER_BT_ADDRESS {{ 0x84, 0x71, 0x27, 0x64, 0x0E, 0x44 }}
//#define SERVER_BT_ADDRESS {{ 0x44, 0x0e, 0x64, 0x27, 0x71, 0x84 }}
#define SERVER_BT_ADDRESS {{ 0x48, 0x35, 0xEF, 0x57, 0x0B, 0x00 }}
//#define SERVER_BT_ADDRESS {{ 0x00, 0X0B, 0X57, 0XEF, 0X35, 0X48 }}

#if DEVICE_IS_BLE_SERVER

#define BUILD_INCLUDES_BLE_SERVER 1
#define BUILD_INCLUDES_BLE_CLIENT 0
#define BLE_DEVICE_TYPE_STRING "Server"
static inline bool IsServerDevice() { return true; }
static inline bool IsClientDevice() { return false; }

#else

#define BUILD_INCLUDES_BLE_SERVER 0
#define BUILD_INCLUDES_BLE_CLIENT 1
#define BLE_DEVICE_TYPE_STRING "Client"
static inline bool IsClientDevice() { return true;}
static inline bool IsServerDevice() { return false; }

#endif

#endif /* SRC_BLE_DEVICE_TYPE_H_ */
