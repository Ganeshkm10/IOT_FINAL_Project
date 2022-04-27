/*
 * scheduler.h
 * This file contains function declarations for scheduler functionality
 *  Created on: 02-Feb-2022
 *      Author: Ganesh KM
 *
 * Modified on : 11-FEB-2022 for Assignment 4
 *               19-FEB-2022 for Assignment 5
 *
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "sl_bt_api.h"

typedef enum{
  state_Idle,
  state_wait_for_PowerOn,
  state_wait_for_I2C_WriteComplete,
  state_wait_for_setupvalue,
  state_wait_for_I2C_Read,
  state_I2C0_register_read,
}state_t;

typedef enum{
  state_PowerOn,
  state_ADC_Conversion,
  state_Measure,
}Ambient_state_t;

typedef enum
{
  event_send_register_address,
  event_read_register_data
}gesture_state_enum;


typedef enum{
discovery_service_HTM =0,
discovery_service_Button,
discovery_service_Ambient,
discovery_service_Gesture,
discovery_characteristics_HTM,
discovery_characteristics_Button,
discovery_characteristics_Ambient,
discovery_characteristics_Gesture,
notification_HTM,
notification_Button,
notification_Ambient,
notification_Gesture,
complete,
wait,
}Discoverystate_t;



void init_Discoverystatemachine();

void schedulerSetEventUF(void);

void schedulerSetEventCOMP1();

void schedulerSetEventButtonPressed_PB0();

void schedulerSetEventButtonReleased_PB0();

void schedulerSetEventButtonPressed_PB1();

void schedulerSetEventButtonReleased_PB1();

void schedulerSetEvent_ADC();

//event getNextEvent();

void schedulerClearEvent();

//void statemachine_temperaturemeasurement(sl_bt_msg_t *evt);

void statemachine_AmbientSensor(sl_bt_msg_t *evt);

void schedulerSetEvent_I2C_TransferComplete();

void init_statemachine();

void discovery_state_machine(sl_bt_msg_t *evt);

void gesture_state_machine(sl_bt_msg_t *evt);

#endif /* SRC_SCHEDULER_H_ */
