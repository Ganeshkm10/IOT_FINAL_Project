/*
 * scheduler.c
 *
 *  Description: This file contains functions to handle Scheduler functionality.
 *  Created on : 02-Feb-2022
 *      Author : Ganesh KM
 *
 * Modified on : 11-FEB-2022 for Assignment 4
 *               19-FEB-2022 for Assignment 5
 *               05-MAR-2022 for Assignment 7
 *
 */

#include <stdbool.h>
#include "em_core.h"
#include "ble_device_type.h"
#include "lcd.h"
#include"scheduler.h"
#include "gpio.h"
#include "timers.h"
#include "oscillators.h"
#include "i2c.h"
#include "irq.h"
#include "sl_bt_api.h"
#include "ble.h"
#include "em_adc.h"

typedef enum{
  eventclear,
  event_LETIMER0_UF,
  event_LETIMER0_COMP1,
  event_I2C_TransferComplete,
  event_timerSetupvalue_complete,
  event_Button_Pressed_PB0,
  event_Button_Released_PB0,
  event_Button_Pressed_PB1,
  event_Button_Released_PB1,
  event_ADC_Conv,
}event;

#define INCLUDE_LOG_DEBUG 1
#include "log.h"

extern bool BT_Connected;
extern uint16_t Ambient_Sensor_Value;
uint16_t Ambient_Sensor_Value;

#if  DEVICE_IS_BLE_SERVER
volatile event _EVENT;
volatile Ambient_state_t CURRENT_STATE;
volatile Ambient_state_t NEXT_STATE;
extern bool Temp_Indicated;
extern bool Ambient_Indicated;





#else

// Temperature Measurement characteristic UUID defined by Bluetooth SIG
static const uint8_t thermo_char[2] = { 0x1c, 0x2a };

// Bluetooth SIG >> button state service UUID
static const uint8_t button_service[16] = { 0x89, 0x62, 0x13, 0x2d, 0x2a, 0x65, 0xec, 0x87, 0x3e, 0x43, 0xc8, 0x38, 0x01, 0x00, 0x00, 0x00 };
// Bluetooth SIG >> button state characteristic UUID
static const uint8_t button_char[16] = { 0x89, 0x62, 0x13, 0x2d, 0x2a, 0x65, 0xec, 0x87, 0x3e, 0x43, 0xc8, 0x38, 0x02, 0x00, 0x00, 0x00 };


static const uint8_t ambient_service[16]={0x3a,0x49,0xd2,0x01,0x5d,0x3c,0x43,0x9a,0xf5,0x4c,0x3a,0xc5,0xa3,0xa6,0xfd,0x19};
static const uint8_t ambient_char[16]={0x26,0x08,0xb1,0xf4,0x71,0x4d,0xe1,0xaf,0xbf,0x48,0x47,0xc9,0x86,0xaf,0x73,0x93};


static const uint8_t gesture_service[16]={0xac,0xd5,0xe6,0x96,0x63,0x4b,0x4a,0xbd,0xb0,0x4c,0x29,0xef,0x94,0xe8,0xdb,0xc2};
static const uint8_t gesture_char[16]={0xd4,0x6f,0x38,0x85,0xc7,0x70,0x09,0x9e,0x10,0x45,0xae,0xcd,0x53,0xe9,0x4e,0xd2};

volatile Discoverystate_t CURR_DISC_STATE;
volatile Discoverystate_t NEXT_DISC_STATE;
//static state_t event_store = state_Idle;
static gesture_state_enum zx_state = event_send_register_address;

#endif



/*
 * @description: Function to handle an event when Underflow flag is set
 * @params: None
 * @return: None
 */



void schedulerSetEventUF(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  //UF_flag |= LETIMER_IF_UF;
 // _EVENT = event_LETIMER0_UF;

  sl_bt_external_signal(event_LETIMER0_UF);

  // gpioLed1SetOn();
 CORE_EXIT_CRITICAL() ;
}

/*
 * @description: Function to handle an event when I2C transfer flag is set
 * @params: None
 * @return: None
 */

void schedulerSetEvent_I2C_TransferComplete()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(state_I2C0_register_read);

  //_EVENT = event_I2C_TransferComplete;
 // LOG_INFO("Entering I2C transfer scheduler");
  CORE_EXIT_CRITICAL() ;
}

/*
 * @description: Function to handle an event when COMP1 is set
 * @params: None
 * @return: None
 */

void schedulerSetEventCOMP1()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(event_LETIMER0_COMP1);

 // _EVENT = event_LETIMER0_COMP1;
  //gpioLed1SetOff();
 CORE_EXIT_CRITICAL() ;
}
void schedulerSetEventButtonPressed_PB0()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  sl_bt_external_signal(event_Button_Pressed_PB0);
//  LOG_INFO("scheduler event button pressed");
  CORE_EXIT_CRITICAL() ;

}
void schedulerSetEventButtonReleased_PB0()
{
  CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
  sl_bt_external_signal(event_Button_Released_PB0);
  CORE_EXIT_CRITICAL() ;
}

void schedulerSetEventButtonPressed_PB1()
{
  CORE_DECLARE_IRQ_STATE;
      CORE_ENTER_CRITICAL();
    sl_bt_external_signal(event_Button_Pressed_PB1);
    CORE_EXIT_CRITICAL() ;

}

void schedulerSetEventButtonReleased_PB1()
{
  CORE_DECLARE_IRQ_STATE;
      CORE_ENTER_CRITICAL();
    sl_bt_external_signal(event_Button_Released_PB1);
    CORE_EXIT_CRITICAL() ;
}

void schedulerSetEvent_ADC()
{
  sl_bt_external_signal(event_ADC_Conv);
}
/*
 * @description: Function to get the event which is happening currently
 * @params: None
 * @return: event
 */

/*event getNextEvent()
{
  event CurrentEvent;

  CurrentEvent = _EVENT;
  schedulerClearEvent();

return CurrentEvent ;
}*/

/*
 * @description: Function to CLEAR THE scheduler event
 * @params: None
 * @return: event
 */

void schedulerClearEvent()
{
  //CORE_ENTER_CRITICAL();

  sl_bt_external_signal(eventclear);

  //  _EVENT = eventclear;
    //LOG_INFO("Entering clear event scheduler");
  //  CORE_EXIT_CRITICAL() ;
}

/*
 * @description: Function to set the statemachine to IDLE state
 * @params: None
 * @return: event
 */
#if  DEVICE_IS_BLE_SERVER
void init_statemachine()
{
  CURRENT_STATE = state_PowerOn;
}


/*
 * @description: Statemachine to handle the series of temperature measuement according to the events
 *
 * @params: evt ( enum : sl_bt_msg_t)
 *
 * @return: none
 *
 */

void statemachine_AmbientSensor(sl_bt_msg_t *evt)
{
  //ble_data_struct_t *ble_data = getBLEdata();
  if(BT_Connected == true)

    {
      if(Ambient_Indicated ==true)
      //if(ble_data->IndicationsEnabled ==1)
        {

            volatile uint32_t Event_to_consider = evt->data.evt_system_external_signal.extsignals;

            switch(CURRENT_STATE)
            {

              case state_PowerOn:
                {
                   if(Event_to_consider == event_LETIMER0_UF)
                     //  LOG_INFO(" entering IDLE state ");
                     {

                       AmbientSensorOn();
                       //timerWaitUs_irq(1000);
                       //schedulerClearEvent();

                       NEXT_STATE = state_ADC_Conversion;
                     }
                  break;
                }

              case state_ADC_Conversion:
                {

                 // if(Event_to_consider == event_LETIMER0_COMP1)
                    {
                      //LOG_INFO(" entering state wait for power on ");
                      ADC_Start(ADC0, adcStartSingle);
                          //NEXT_STATE = state_Measure;


                         // schedulerClearEvent();
                          NEXT_STATE = state_Measure;
                    }
                 break;
                }

              case state_Measure:
                {
                  //printf(" outside state measure \n");
                  if(Event_to_consider == event_ADC_Conv)
                    {


                     // Ambient_Sensor_Value = ADC_DataSingleGet(ADC0);
                       LOG_INFO(" Ambient_Sensor_Value = %d\r\n",  Ambient_Sensor_Value);
                       displayPrintf( DISPLAY_ROW_11, "Amb Val = %d lux",Ambient_Sensor_Value);
                       AmbientSensorOff();
                       Sendindication_AmbientLight();
                       NEXT_STATE = state_PowerOn;
                     }

                         break;
                }
              default:
                {
                  LOG_WARN("Unknown state in State machine");
                }

            }

          {
           CURRENT_STATE = NEXT_STATE;
          }

        }

    }
}

/*
void statemachine_temperaturemeasurement(sl_bt_msg_t *evt)
{
  //ble_data_struct_t *ble_data = getBLEdata();
  if(BT_Connected == true)

    {
      if(Temp_Indicated ==true)
      //if(ble_data->IndicationsEnabled ==1)
        {

            volatile uint32_t Event_to_consider = evt->data.evt_system_external_signal.extsignals;

            switch(CURRENT_STATE)
            {

              case state_Idle:
                {
                   if(Event_to_consider == event_LETIMER0_UF)
                     //  LOG_INFO(" entering IDLE state ");
                     {
                       Init_I2C();
                       gpioSi7021Enable();

                       timerWaitUs_irq(80);
                       NEXT_STATE = state_wait_for_PowerOn;
                       //schedulerClearEvent();
                     }
                  break;
                }

              case state_wait_for_PowerOn:
                {

                  if(Event_to_consider == event_LETIMER0_COMP1)
                    {
                      //LOG_INFO(" entering state wait for power on ");
                      sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
                       I2C_WRITE(1);
                          NEXT_STATE = state_wait_for_I2C_WriteComplete;

                         // schedulerClearEvent();
                    }
                 break;
                }

              case state_wait_for_I2C_WriteComplete:
                {
                  if(Event_to_consider == event_I2C_TransferComplete)
                    {
                    sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
                      timerWaitUs_irq(11);
                      NEXT_STATE = state_wait_for_setupvalue;
                    }


                         break;
                }

              case state_wait_for_setupvalue:
                {
                  if(Event_to_consider == event_LETIMER0_COMP1)
                    {

                      //_EVENT = event_timerSetupvalue_complete;
                      sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
                      I2C_TemperatureMeasurement();
                      NEXT_STATE = state_wait_for_I2C_Read;
                    }

                   break;
                }

              case state_wait_for_I2C_Read:
                {
                   // gpioSi7021Disable();
                    sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
                    NEXT_STATE = state_Idle;

                  break;
                }
              default:
                {
                  LOG_WARN("Unknown state in State machine");
                }

            }

          {
           CURRENT_STATE = NEXT_STATE;
          }

        }

    }
}
*/

#else
void init_Discoverystatemachine()
{
  CURR_DISC_STATE = discovery_service_HTM;
}

void discovery_state_machine(sl_bt_msg_t *evt)
{
  ble_data_struct_t *ble_data = getBLEdata();
  sl_status_t sc;
  //NEXT_DISC_STATE = discovery_service_HTM;

     if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_connection_closed_id)
       {
         NEXT_DISC_STATE = discovery_service_HTM;
       }



  switch(CURR_DISC_STATE)
  {
    case discovery_service_HTM:
      {
        if((SL_BT_MSG_ID(evt->header) == sl_bt_evt_connection_opened_id)&&
            (BT_Connected == true))
          {


          ble_data->ConnectionHandle = evt->data.evt_connection_opened.connection;
              //For Health thermometer
           sc =  sl_bt_gatt_discover_primary_services_by_uuid(ble_data->ConnectionHandle,
                                                              sizeof(ble_data->HTM_Service),
                                                              ble_data->HTM_Service);
           if(sc != SL_STATUS_OK) {
                       LOG_ERROR("sl_bt_gatt_discover_primary_services_by_uuid() of HTM returned WITH ERROR CODE =0x%04x\n\r", (unsigned int)sc);
                    }


         LOG_INFO(" State: Discovery Service HTM ");
           NEXT_DISC_STATE = discovery_service_Button;
          }

      }

 break;

    case discovery_service_Button:
          {
            if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
              {
               //For Button
               sc =  sl_bt_gatt_discover_primary_services_by_uuid(ble_data->ConnectionHandle,
                                                                  sizeof(button_service),
                                                                  (const uint8_t*)button_service);
               if(sc != SL_STATUS_OK) {
                           LOG_ERROR("sl_bt_gatt_discover_primary_services_by_uuid() OF Button returned with Error code =0x%04x\n\r", (unsigned int)sc);
                        }


             LOG_INFO(" State: discovery_service_Button ");
               NEXT_DISC_STATE = discovery_service_Ambient;
              }

          }

     break;
    case discovery_service_Ambient:
          {
            if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
              {
               //For Button
               sc =  sl_bt_gatt_discover_primary_services_by_uuid(ble_data->ConnectionHandle,
                                                                  sizeof(ambient_service),
                                                                  (const uint8_t*)ambient_service);
               if(sc != SL_STATUS_OK) {
                           LOG_ERROR("sl_bt_gatt_discover_primary_services_by_uuid() OF Button returned with Error code =0x%04x\n\r", (unsigned int)sc);
                        }


             LOG_INFO(" State: discovery_service_Ambient ");
               NEXT_DISC_STATE = discovery_service_Gesture;
              }

          }

     break;
    case discovery_service_Gesture:
          {
            if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
              {
               //For Button
               sc =  sl_bt_gatt_discover_primary_services_by_uuid(ble_data->ConnectionHandle,
                                                                  sizeof(gesture_service),
                                                                  (const uint8_t*)gesture_service);
               if(sc != SL_STATUS_OK) {
                           LOG_ERROR("sl_bt_gatt_discover_primary_services_by_uuid() OF Button returned with Error code =0x%04x\n\r", (unsigned int)sc);
                        }


             LOG_INFO(" State: discovery_service_gesture ");
               NEXT_DISC_STATE = discovery_characteristics_HTM;
              }

          }

     break;

    case  discovery_characteristics_HTM:
      {

        if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
          {


            sc = sl_bt_gatt_discover_characteristics_by_uuid(ble_data->ConnectionHandle,
                                                             ble_data->ServiceHandle,
                                                                       sizeof(thermo_char),
                                                                       (const uint8_t*)thermo_char);
          if(sc != SL_STATUS_OK) {
                          LOG_ERROR("sl_bt_gatt_discover_characteristics_by_uuid() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                     }
            NEXT_DISC_STATE =  discovery_characteristics_Button;

            LOG_INFO(" State: Discovery Characteristics HTM");
          }


      }
      break;

    case  discovery_characteristics_Button:
         {

           if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
             {

               sc = sl_bt_gatt_discover_characteristics_by_uuid(ble_data->ConnectionHandle,
                                                                ble_data->Servicehandle_Button,
                                                                          sizeof(button_char),
                                                                          (const uint8_t*)button_char);
             if(sc != SL_STATUS_OK) {
                             LOG_ERROR("sl_bt_gatt_discover_characteristics_by_uuid() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                        }


               NEXT_DISC_STATE = discovery_characteristics_Ambient;

               LOG_INFO(" State: Discovery Characteristics Button");
             }


         }
         break;

    case  discovery_characteristics_Ambient:
         {

           if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
             {

               sc = sl_bt_gatt_discover_characteristics_by_uuid(ble_data->ConnectionHandle,
                                                                ble_data->Servicehandle_Ambient,
                                                                          sizeof(ambient_char),
                                                                          (const uint8_t*)ambient_char);
             if(sc != SL_STATUS_OK) {
                             LOG_ERROR("sl_bt_gatt_discover_characteristics_by_uuid() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                        }


               NEXT_DISC_STATE = discovery_characteristics_Gesture;

               LOG_INFO(" State: Discovery Characteristics ambient");
             }


         }
         break;

    case  discovery_characteristics_Gesture:
         {

           if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
             {

               sc = sl_bt_gatt_discover_characteristics_by_uuid(ble_data->ConnectionHandle,
                                                                ble_data->Servicehandle_Gesture,
                                                                          sizeof(gesture_char),
                                                                          (const uint8_t*)gesture_char);
             if(sc != SL_STATUS_OK) {
                             LOG_ERROR("sl_bt_gatt_discover_characteristics_by_uuid() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                        }


               NEXT_DISC_STATE = notification_HTM;

               LOG_INFO(" State: Discovery Characteristics gesture");
             }


         }
         break;


    case notification_HTM:
      {
        if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
          {
            //For Health thermometer
            sc = sl_bt_gatt_set_characteristic_notification(ble_data->ConnectionHandle,
                                                            ble_data->CharacteristicHandle,
                                                            sl_bt_gatt_indication);
            if(sc != SL_STATUS_OK) {
                         LOG_ERROR("sl_bt_gatt_set_characteristic_notification_HTM() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                     }

            NEXT_DISC_STATE = notification_Button;
            displayPrintf(DISPLAY_ROW_CONNECTION, "Handling indications");
            LOG_INFO(" State: notification_HTM ");
          }


      }
      break;

    case notification_Button:
      {
        if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
          {
            //For Button
            sc = sl_bt_gatt_set_characteristic_notification(ble_data->ConnectionHandle,
                                                            ble_data->CharacteristicHandle_Button,
                                                            sl_bt_gatt_indication);
            if(sc != SL_STATUS_OK) {
                         LOG_ERROR("sl_bt_gatt_set_characteristic_notification_HTM() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                     }

            NEXT_DISC_STATE = notification_Ambient;
            displayPrintf(DISPLAY_ROW_CONNECTION, "Handling indications");
            LOG_INFO(" State: notification_Button ");
          }


      }
      break;

    case notification_Ambient:
          {
            if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
              {
                //For Button
                sc = sl_bt_gatt_set_characteristic_notification(ble_data->ConnectionHandle,
                                                                ble_data->CharacteristicHandle_Ambient,
                                                                sl_bt_gatt_indication);
                if(sc != SL_STATUS_OK) {
                             LOG_ERROR("sl_bt_gatt_set_characteristic_notification_HTM() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                         }

                NEXT_DISC_STATE = notification_Gesture;
                displayPrintf(DISPLAY_ROW_CONNECTION, "Handling indications");
                LOG_INFO(" State: notification_Ambient ");
              }


          }
          break;

    case notification_Gesture:
              {
                if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
                  {
                    //For Button
                    sc = sl_bt_gatt_set_characteristic_notification(ble_data->ConnectionHandle,
                                                                    ble_data->CharacteristicHandle_Gesture,
                                                                    sl_bt_gatt_indication);
                    if(sc != SL_STATUS_OK) {
                                 LOG_ERROR("sl_bt_gatt_set_characteristic_notification_HTM() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                             }

                    NEXT_DISC_STATE = complete;
                    displayPrintf(DISPLAY_ROW_CONNECTION, "Handling indications");
                    LOG_INFO(" State: notification_Ambient ");
                  }


              }
              break;


    case complete:
      {
        if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id)
          {
            NEXT_DISC_STATE = wait;
            LOG_INFO("State: Complete ");
            Gesture_Main_Logic();
          }
      }
      break;
    case wait:
      {
        if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_connection_closed_id)
          {
            NEXT_DISC_STATE = discovery_service_HTM;
            LOG_INFO(" State: Wait");
          }

      }
      break;


  }

  CURR_DISC_STATE = NEXT_DISC_STATE;
}


/*
 * Description : state machine for reading gestures.
 * Parameters : event_in -> from bluetooth event handler
 * Returns : none
 * */

void gesture_state_machine(sl_bt_msg_t *evt)
{
  volatile uint32_t Event_to_consider = evt->data.evt_system_external_signal.extsignals;
  switch(zx_state)
  {
    case event_send_register_address:
      send_register_address();
      zx_state = event_read_register_data;
      break;

    case event_read_register_data:
        if(Event_to_consider == state_I2C0_register_read){
          read_register_data();
         // timerWaitUs_irq(800);
          zx_state = event_send_register_address;
        }
      break;

  }
}



#endif



