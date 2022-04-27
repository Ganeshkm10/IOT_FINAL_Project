/*
 * ble.c
 *
 *  @Description: This file contains functionalities in order to handle Bluetooth stack
 *
 *  Created on: 14-Feb-2022
 *
 *  Author: Ganesh KM
 *
 *  Reference: Example project - Bluetooth SoC thermometer.
 */


#include "ble.h"
#include "gatt_db.h"
#include "lcd.h"
#include "ble_device_type.h"
#include <math.h>
#include "gpio.h"


#define INCLUDE_LOG_DEBUG 1
#include "log.h"

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
}event;

// Globals for Circular buffer
int response =0 ;
int CBFIFO_Length;
int  Queued_Indications =0;

// for SM Configure API
#define FLAGS 0x0F
#if  !DEVICE_IS_BLE_SERVER
static const uint8_t thermo_service[2] = { 0x09, 0x18 };
static const uint8_t thermo_char[2] = { 0x1c, 0x2a };
static const uint8_t button_service[16] = { 0x89, 0x62, 0x13, 0x2d, 0x2a, 0x65, 0xec, 0x87, 0x3e, 0x43, 0xc8, 0x38, 0x01, 0x00, 0x00, 0x00 };
static const uint8_t button_char[16] = { 0x89, 0x62, 0x13, 0x2d, 0x2a, 0x65, 0xec, 0x87, 0x3e, 0x43, 0xc8, 0x38, 0x02, 0x00, 0x00, 0x00 };

static const uint8_t ambient_service[16]={0x3a,0x49,0xd2,0x01,0x5d,0x3c,0x43,0x9a,0xf5,0x4c,0x3a,0xc5,0xa3,0xa6,0xfd,0x19};
static const uint8_t ambient_char[16]={0x26,0x08,0xb1,0xf4,0x71,0x4d,0xe1,0xaf,0xbf,0x48,0x47,0xc9,0x86,0xaf,0x73,0x93};

static const uint8_t gesture_service[16]={0xac,0xd5,0xe6,0x96,0x63,0x4b,0x4a,0xbd,0xb0,0x4c,0x29,0xef,0x94,0xe8,0xdb,0xc2};
static const uint8_t gesture_char[16]={0xd4,0x6f,0x38,0x85,0xc7,0x70,0x09,0x9e,0x10,0x45,0xae,0xcd,0x53,0xe9,0x4e,0xd2};

//Ambient Service

#endif

//Server implementation
bool Button_Indicated = true;
int adjust_indicate = 0;

bool Ambient_Indicated = false;
bool Temp_Indicated = false;
extern bool Temp_Indicated;
extern bool Ambient_Indicated;
extern uint8_t temperature_in_C ; //Temperature value from the I2C BUFFER.
bool BT_Connected = false;        //GLOBALvariable in order to know that BT is Connected.
extern bool BT_Connected;

extern uint16_t Ambient_Sensor_Value;

//extern uint32_t Ambient_Sensor_Value;

//Button Sequence variables to Enable/Disable Button state Indications
bool Press_Sequence_1 = false;
bool Press_Sequence_2 = false;
bool Press_Sequence_3 = false;

//client implementation
static ble_data_struct_t ble_data = {.HTM_Service = {0x09,0x18}};
uint8_t Received_temperature_in_C;
uint16_t Receieved_Ambient = 0;

//static uint8_t advertising_set_handle = 0xff;
//static uint8_t Connection_Handle = 0;

#if  !DEVICE_IS_BLE_SERVER
static const bd_addr server_addr[6]= {SERVER_BT_ADDRESS};

// private data structure
#endif


// Declare memory for the queue/buffer/fifo, and the write and read pointers
queue_struct_t   CB_Indication_Data[QUEUE_DEPTH]; // the queue
uint32_t         wptr = 0;              // write pointer
uint32_t         rptr = 0;              // read pointer

// Decide how will you handle the full condition.
bool _fullflag= false;
bool _emptyflag = true; //initially this flag will be true.

/*
 * @Description: Private Function which checks whether buffer is full
 *
 * @params : none
 *
 * @return : true if its full else false
 */
static bool Is_CircularBuffer_Full()
{
  if(((wptr+1)%QUEUE_DEPTH)==rptr)
  {
    //printf("buffer is full\n");
    _fullflag= true;
    _emptyflag = false;
    return true;
  }

  else
  {
      return false;
  }
}

/*
 * @Description: Private Function which checks whether buffer is empty
 *
 * @params : none
 *
 * @return : true if its empty else false
 */
static bool Is_CircularBuffer_Empty()
{
    if(rptr==wptr)
  {
    //printf("\n cannot read \n");
    _emptyflag = true;
    _fullflag = false;
    return true;
  }
  else{
      return false;
  }

}

// ---------------------------------------------------------------------
// Private function used only by this .c file.
// compute next ptr value
// Isolation of functionality: This defines "how" a pointer advances.
// ---------------------------------------------------------------------

/*
 * @Description:Function which computes the next ptr value
 *
 * @params : ptr : pointer variable
 *
 * @return : none
 */
static uint32_t nextPtr(uint32_t ptr) {

return (ptr+1)%QUEUE_DEPTH;  //QUEUE_DEPTH

} // nextPtr()


// ---------------------------------------------------------------------
// Public function
// This function writes an entry to the queue.
// Returns false if successful or true if writing to a full fifo.
// ---------------------------------------------------------------------


/*
 * @Description:Function which writes entry to circular buffer
 *
 * @params : a, b :
 *
 * @return : false if successful or true if writing to a full fifo
 */

bool write_queue (queue_struct_t SendMsg) {

  // Isolation of functionality:
  //     Create the logic for "when" a pointer advances
  if(Is_CircularBuffer_Full()==true)
  {
    return false;
  }
  else
  {
    _fullflag = false;
    _emptyflag = false;
    CB_Indication_Data[wptr]= SendMsg;
 //   CB_Indication_Data[wptr].b= b;
    wptr =nextPtr(wptr);
    Is_CircularBuffer_Full();
    return true;
  }
}

// ---------------------------------------------------------------------
// Public function
// This function reads an entry from the queue.
// Returns false if successful or true if reading from an empty fifo.
// ---------------------------------------------------------------------


/*
 * @Description: Function which reads entry from circular buffer
 *
 * @params : *a, *b :
 *
 * @return : false if successful or true if reading from an empty fifo
 */

bool read_queue () {
  ble_data_struct_t *bledata = getBLEdata();
  // Isolation of functionality:
  //     Create the logic for "when" a pointer advances
  if(Is_CircularBuffer_Empty()==true)
  {
    return false;
  }
  else
  {
    _emptyflag = false;
    _fullflag = false;
  //  *a = CB_Indication_Data[rptr].a ;
    //*b = CB_Indication_Data[rptr].b;
    queue_struct_t Read_Data;
    Read_Data = CB_Indication_Data[rptr];
    //bledata->ConnectionHandle = evt->data.evt_connection_opened.connection;

   // LOG_INFO(" Connection handle is %d \n",ble_data.ConnectionHandle);

    sl_status_t sc = 0;
    sc = sl_bt_gatt_server_send_indication(ble_data.ConnectionHandle,
                                           Read_Data.charHandle,
                                           Read_Data.bufferLength,
                                             &(Read_Data.buffer[0]));

      //check indication return status
      if(sc != SL_STATUS_OK) {
          LOG_ERROR("sl_bt_gatt_server_send_indication() failed while dequeueing in read_dequeue function =0x%04x\n\r", (unsigned int)sc);
          return -1;
      }

      bledata->Indication_in_Flight = true;

    rptr=nextPtr(rptr);
    Is_CircularBuffer_Empty();

    CBFIFO_Length = get_queue_depth();
    LOG_INFO(" CBFIFO LENGTH after dequeue : %d \n\r",CBFIFO_Length);

 return true;
  }
} // read_queue()



// ---------------------------------------------------------------------
// Public function
// This function returns the wptr, rptr, full and empty values.
// ---------------------------------------------------------------------

/*
 * @Description:Function which returns the status of circular buffer (flags)
 *
 * @params : wptr, rptr, full, empty.
 *
 * @return : returns all the flags and ptr values
 */

void get_queue_status (uint32_t *_wptr, uint32_t *_rptr, bool *_full, bool *_empty) {
   *_wptr=wptr;
  *_rptr=rptr;
   *_full=_fullflag;
   *_empty=_emptyflag;
} // get_queue_status()

// ---------------------------------------------------------------------
// Public function
// Function that computes the number of entries in the queue
// ---------------------------------------------------------------------

/*
 * @Description:Function which returns the number of entries in circular buffer (flags)
 *
 * @params : none.
 *
 * @return : returns queue depth
 */

uint32_t get_queue_depth() {

  if(wptr>rptr)
  {
    return wptr - rptr;
  }
  else if(rptr>wptr)
  {
    return QUEUE_DEPTH - rptr + wptr;
  }
  else
    return 0;
} // get_queue_depth()

//ble_data_struct_t ble_data;
ble_data_struct_t *getBLEdata()
{
  return &ble_data;
}


#if  !DEVICE_IS_BLE_SERVER

/*
 * @description: Got from the Assignment 7 write up.
 *               indications back to an integer. Convert IEEE-11073 32-bit float to signed integer.
 *
 * @params: uint8_t *value_start_little_endian
 *
 * @return: FLOAT to INT32 value
 *
 */

static  int32_t  FLOAT_TO_INT32(const  uint8_t  *value_start_little_endian)
{
  uint8_t signByte = 0;
  int32_t mantissa;
  // input data format is:
  // [0]       = flags byte
  // [3][2][1] = mantissa (2's complement)
  // [4]       = exponent (2's complement)
  // BT value_start_little_endian[0] has the flags byte
  int8_t  exponent = (int8_t)value_start_little_endian[4];
  // sign extend the mantissa value if the mantissa is negative
  if  (value_start_little_endian[3] & 0x80) {
      //  msb  of [3] is the sign of the mantissa
      signByte = 0xFF;

  }
  mantissa = (int32_t) (value_start_little_endian[1]  << 0)  |
      (value_start_little_endian[2]  << 8)  |
      (value_start_little_endian[3]  << 16) |
      (signByte                      << 24) ;
  // value = 10^exponent * mantissa,  pow() returns a double type
  //LOG_INFO(" %ld ",(int32_t) (pow(10, exponent) * mantissa));
  return  (int32_t) (pow(10, exponent) * mantissa); // FLOAT_TO_INT32

}
#endif

/*
 * @description: Function to handle Bluetooth stack events which has both client and server .
 *
 * @params: evt
 *
 * @return: None
 *
 */

void handle_ble_event(sl_bt_msg_t *evt)
{
  sl_status_t sc = 0;
   uint8_t address_type;
   ble_data_struct_t *bledata = getBLEdata();
 // printf(" Indication in flight check : %d ",bledata->Indication_in_Flight);
  //printf(" Queued indications check in buttonstate : %d ",Queued_Indications);
   switch(SL_BT_MSG_ID(evt->header))
   {
     case sl_bt_evt_system_boot_id:
       //handle boot event
             displayInit();

         sc = sl_bt_system_get_identity_address(&ble_data.Address, &address_type);

         displayPrintf( DISPLAY_ROW_BTADDR, "%X:%X:%X:%X:%X:%X",
                         ble_data.Address.addr[ 0 ], ble_data.Address.addr[1], ble_data.Address.addr[ 2 ],
                         ble_data.Address.addr[ 3 ], ble_data.Address.addr[ 4 ], ble_data.Address.addr[ 5 ] );


#if  DEVICE_IS_BLE_SERVER

         sc = sl_bt_advertiser_create_set(&(ble_data.advertisingSetHandle));

                if(sc!=SL_STATUS_OK)
                        {
                         LOG_ERROR(" BT Advertiser Create set is FAILED With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                        }

            //set parameters for advertising
         sc = sl_bt_advertiser_set_timing(ble_data.advertisingSetHandle,
                                             MAX_ADVERTISING_INTERVAL,
                                             MIN_ADVERTISING_INTERVAL,
                                             0,
                                             0);
              if(sc!=SL_STATUS_OK)
                       {
                         LOG_ERROR(" BT Advertiser set timing is FAILED With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                       }



            //start advertising
         sc = sl_bt_advertiser_start(ble_data.advertisingSetHandle,
                                   sl_bt_advertiser_general_discoverable,
                                   sl_bt_advertiser_connectable_scannable);

                  if(sc!=SL_STATUS_OK)
                       {
                         LOG_ERROR(" BT Advertiser start event is failed with the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                       }

                  displayPrintf( DISPLAY_ROW_NAME, "Server" );

                  displayPrintf(  DISPLAY_ROW_CONNECTION, "Advertising" );

                  LOG_INFO(" BLE Started and Radio is ready! \n");

#else

         sc = sl_bt_scanner_set_mode(sl_bt_gap_phy_1m, SCAN_TYPE);

                    if (sc != SL_STATUS_OK)
                      {
                        LOG_ERROR("BT Scanner set mode failed with the ERROR CODE = 0x%04x \r\n", (unsigned int) sc);
                      }
                    LOG_INFO(" Client -> scan set mode is set");

         sc = sl_bt_scanner_set_timing(sl_bt_gap_phy_1m, SCAN_INTERVAL, SCAN_WINDOW);

                    if (sc != SL_STATUS_OK)
                      {
                        LOG_ERROR("sl_bt_scanner_set_timing() returned != 0 status=0x%04x", (unsigned int) sc);
                      }


         sc = sl_bt_connection_set_default_parameters(MIN_INTERVAL, MAX_INTERVAL, SLAVE_LATENCY,
                                                                 TIMEOUT, 0, 0xFFFF);

                    if (sc != SL_STATUS_OK)
                      {
                        LOG_ERROR("sl_bt_connection_set_default_parameters() returned != 0 status=0x%04x", (unsigned int) sc);
                      }


         sc = sl_bt_scanner_start(sl_bt_gap_phy_1m, sl_bt_scanner_discover_observation);

                    if (sc != SL_STATUS_OK)
                      {
                        LOG_ERROR("sl_bt_scanner_start() returned != 0 status=0x%04x", (unsigned int) sc);
                      }

         displayPrintf( DISPLAY_ROW_NAME, "Client" );
         displayPrintf(  DISPLAY_ROW_CONNECTION, "Discovering" );


#endif

         //Configure security requirements and I/O capabilities of the system
              sc = sl_bt_sm_configure(FLAGS, sl_bt_sm_io_capability_displayyesno);



                  if(sc != SL_STATUS_OK)
                        {
                            LOG_ERROR("sl_bt_sm_configure() is FAILED With the ERROR CODE = 0x%04x\n\r", (unsigned int)sc);
                        }

                        //delete bonding data from server
             sc = sl_bt_sm_delete_bondings();
                   if(sc != SL_STATUS_OK)
                        {
                            LOG_ERROR("sl_bt_sm_delete_bondings() is FAILED With the ERROR CODE = 0x%04x\n\r", (unsigned int)sc);
                        }

            BT_Connected = false;

            displayPrintf(DISPLAY_ROW_BTADDR, "%02X:%02X:%02X:%02X:%02X:%02X",
                             bledata->Address.addr[0],
                             bledata->Address.addr[1],
                             bledata->Address.addr[2],
                             bledata->Address.addr[3],
                             bledata->Address.addr[4],
                             bledata->Address.addr[5]);
              // displayPrintf(DISPLAY_ROW_ASSIGNMENT, "A9");

               BT_Connected = false;

               adjust_indicate=0;
               bledata->bonded = false;
               bledata->button_pressed = false;
               bledata->Indication_in_Flight = false;

       break;

     case sl_bt_evt_connection_opened_id:

#if  DEVICE_IS_BLE_SERVER
         sc = 0;
         displayPrintf( DISPLAY_ROW_CONNECTION, "Connected" );
         sc = sl_bt_advertiser_stop(ble_data.advertisingSetHandle);

             if(sc!=SL_STATUS_OK)
                {
                  LOG_ERROR(" BT Advertiser Stop is FAILED With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                 }

         ble_data.ConnectionHandle = evt->data.evt_connection_opened.connection;


         sc = sl_bt_connection_set_parameters(evt->data.evt_connection_opened.connection,
                                            MIN_CONNECTION_INTERVAL,
                                            MAX_CONNECTION_INTERVAL,
                                            SLAVE_LATENCY,
                                            SUPERVISION_TIMEOUT,
                                            0,
                                            0XFFFF);
             if(sc!=SL_STATUS_OK)
                  {
                    LOG_ERROR(" BT Connection set parameters is FAILED With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                  }

         LOG_INFO(" BLE connection opened with parameters: Connection = 1, Connection interval = %d, Latency = %d, Timeout = %d \n", MAX_CONNECTION_INTERVAL,SLAVE_LATENCY,SUPERVISION_TIMEOUT);
#else
         displayPrintf(DISPLAY_ROW_BTADDR2, "%02X:%02X:%02X:%02X:%02X:%02X",
                           server_addr->addr[0],
                           server_addr->addr[1],
                           server_addr->addr[2],
                           server_addr->addr[3],
                           server_addr->addr[4],
                           server_addr->addr[5]);


#endif
         BT_Connected = true;

         break;



     case sl_bt_evt_connection_closed_id:


           gpioLed0SetOff();
           gpioLed1SetOff();

           displayPrintf(DISPLAY_ROW_9, "");
           displayPrintf(DISPLAY_ROW_TEMPVALUE, "");
           displayPrintf(DISPLAY_ROW_PASSKEY, "");
           displayPrintf(DISPLAY_ROW_ACTION, "");
           bledata->Indication_in_Flight = false;
           sc = sl_bt_sm_delete_bondings();
                  if(sc != SL_STATUS_OK)
                           {
                              LOG_ERROR("sl_bt_sm_delete_bondings() Failed with ERROR CODE = 0x%04x\n\r", (unsigned int)sc);
                           }

#if  DEVICE_IS_BLE_SERVER

       displayPrintf( DISPLAY_ROW_CONNECTION, "Advertising" );

       ble_data.advertisingSetHandle = 0;

       sc = sl_bt_advertiser_start(ble_data.advertisingSetHandle,
                                   sl_bt_advertiser_general_discoverable,
                                   sl_bt_advertiser_connectable_scannable);
       if (sc != SL_STATUS_OK)
                {
                  LOG_ERROR("BLE re-advertising failed with error code : 0x%04x\r\n", (unsigned int) sc);
                }
       displayPrintf( DISPLAY_ROW_TEMPVALUE, " ",temperature_in_C);
       displayPrintf(DISPLAY_ROW_CONNECTION, "Advertising");
       LOG_INFO(" BLE connection closed \n");

#else

       sc = sl_bt_scanner_start(sl_bt_gap_phy_1m, sl_bt_scanner_discover_observation);

                          if (sc != SL_STATUS_OK)
                            {
                              LOG_ERROR("sl_bt_scanner_start() function failed with error code : 0x%04x", (unsigned int) sc);
                            }
       displayPrintf(  DISPLAY_ROW_CONNECTION, "Discovering" );
       displayPrintf(DISPLAY_ROW_TEMPVALUE, " " );

#endif
       displayPrintf(DISPLAY_ROW_BTADDR2, " ");

       BT_Connected = false;

       adjust_indicate=0;
       bledata->bonded = false;
       bledata->button_pressed = false;
       bledata->Indication_in_Flight = false;

        break;

     case sl_bt_evt_sm_confirm_bonding_id:

           //Accept or reject the bonding request.
           sc = sl_bt_sm_bonding_confirm(bledata->ConnectionHandle, 1);
           if(sc != SL_STATUS_OK)
             {
                 LOG_ERROR("sl_bt_sm_bonding_confirm() Failed with ERROR CODE = 0x%04x\n\r", (unsigned int)sc);
             }

           break;

     case sl_bt_evt_sm_confirm_passkey_id:

           //store the passkey
           bledata->passkey = evt->data.evt_sm_confirm_passkey.passkey;

           displayPrintf(DISPLAY_ROW_PASSKEY, "%d", bledata->passkey);
           displayPrintf(DISPLAY_ROW_ACTION, "Confirm with PB0");

           break;

           //Triggered after the pairing or bonding procedure is successfully completed.
         case sl_bt_evt_sm_bonded_id:

           displayPrintf(DISPLAY_ROW_CONNECTION, "Bonded");
           bledata->bonded           = true;
           displayPrintf(DISPLAY_ROW_PASSKEY, "");
           displayPrintf(DISPLAY_ROW_ACTION, "");

           break;

           // This event is triggered if the pairing or bonding procedure fails.
         case sl_bt_evt_sm_bonding_failed_id:

           LOG_ERROR("Bonding failed reason=0x%04x\n\r", evt->data.evt_sm_bonding_failed.reason);

           //close connection
           sc = sl_bt_connection_close(bledata->ConnectionHandle);
           if(sc != SL_STATUS_OK)
             {
               LOG_ERROR("sl_bt_connection_close() Failed with ERROR CODE = 0x%04x\n\r", (unsigned int)sc);
             }

           break;
         case sl_bt_evt_system_external_signal_id:

         //  LOG_INFO("Button pressed = %d \n",evt->data.evt_system_external_signal.extsignals);
         //Handle Button press event

           if(evt->data.evt_system_external_signal.extsignals == event_Button_Pressed_PB0)
             {
#if  DEVICE_IS_BLE_SERVER

               //LOG_INFO("Button pressed = %d \n",evt->data.evt_system_external_signal.extsignals);
               displayPrintf(DISPLAY_ROW_9, "Button Pressed");

               if( bledata->bonded)
                  {
                Send_ButtonState(0x01);
                   }

#endif
             }
           //Handling button release
           else if(evt->data.evt_system_external_signal.extsignals ==  event_Button_Released_PB0)
             {
#if  DEVICE_IS_BLE_SERVER
               displayPrintf(DISPLAY_ROW_9, "Button Released");

               if(bledata->bonded)
                 {
                Send_ButtonState(0x00);
                   }

#endif
             }
           if(evt->data.evt_system_external_signal.extsignals ==  event_Button_Pressed_PB1)
             {

#if  !DEVICE_IS_BLE_SERVER


               sc = sl_bt_gatt_read_characteristic_value(bledata->ConnectionHandle,
                                                         bledata->CharacteristicHandle_Button);
                             if(sc != SL_STATUS_OK) {
                                 LOG_ERROR("sl_bt_gatt_read_characteristic_value() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                             }


#endif
             }
           if((evt->data.evt_system_external_signal.extsignals ==  event_Button_Pressed_PB0) && (bledata->bonded == false))
             {
               //Accept or reject the reported passkey confirm value.
                             sc = sl_bt_sm_passkey_confirm(bledata->ConnectionHandle, 1);

                             if(sc != SL_STATUS_OK) {
                                 LOG_ERROR("sl_bt_sm_passkey_confirm() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                             }
             }

#if  !DEVICE_IS_BLE_SERVER

           /*
            * Button Sequencing is handled which Enables/Disables the Button
            * state Indications to the server. (PB0 Pressed -> PB1 Press -> PB1 release -> PB0 Release)
            */

       if(bledata->bonded)
         {

          if(evt->data.evt_system_external_signal.extsignals ==  event_Button_Pressed_PB0)
              {
                  Press_Sequence_1 = true;


              }
          if((Press_Sequence_1 = true)&&(evt->data.evt_system_external_signal.extsignals ==  event_Button_Pressed_PB1))
               {

                  Press_Sequence_2 = true;

               }

           if((Press_Sequence_2 == true)&&(evt->data.evt_system_external_signal.extsignals ==  event_Button_Released_PB1))
               {
                 Press_Sequence_3 = true;
               }

           if((Press_Sequence_3 == true)&&(evt->data.evt_system_external_signal.extsignals ==  event_Button_Released_PB0))
               {
                  Button_Indicated = !Button_Indicated;

                  Press_Sequence_1 = false;
                  Press_Sequence_2 = false;
                  Press_Sequence_3 = false;
                  if(Button_Indicated)
                    {
                      sc = sl_bt_gatt_set_characteristic_notification(bledata->ConnectionHandle,
                                                                                                 bledata->CharacteristicHandle_Button,
                                                                                                 sl_bt_gatt_indication);
                                                 if(sc != SL_STATUS_OK) {
                                                              LOG_ERROR("sl_bt_gatt_set_characteristic_notification_button sequencelogic returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                                                          }
                    }
                  else
                    {
                      sc = sl_bt_gatt_set_characteristic_notification(bledata->ConnectionHandle,
                                                                                                 bledata->CharacteristicHandle_Button,
                                                                                                 sl_bt_gatt_disable);
                                                 if(sc != SL_STATUS_OK) {
                                                              LOG_ERROR("sl_bt_gatt_set_characteristic_notification_button in sequencelogic returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                                                          }
                    }

               }
         }


#endif

           break;


     case sl_bt_evt_system_soft_timer_id:
       {
         if(evt->data.evt_system_soft_timer.handle == 0)
           {
             displayUpdate();

           }
         if((Queued_Indications!=0)&& (!bledata->Indication_in_Flight)&&((bledata->button_indication == true)||(Temp_Indicated ==true)||(Ambient_Indicated ==true)))
           {
             response = read_queue();
             if(response)
               {
                 LOG_INFO(" *** Indications sent from Queue *** ");
                 Queued_Indications--;
               }

           }

       }
       break;

#if  DEVICE_IS_BLE_SERVER


     case sl_bt_evt_gatt_server_characteristic_status_id:
        {

         if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement)
            {


                if( evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_indication)//sl_bt_gatt_server_indication


                         //adjust_indicate++;
                         // if(adjust_indicate>1)
                       {
                          ble_data.IndicationsEnabled = 1;
                          Temp_Indicated = true;
                         // LOG_INFO("HTM Indications Enabled");
                          //displayPrintf( DISPLAY_ROW_TEMPVALUE, "Temp = %d C",temperature_in_C);
                          gpioLed0SetOn();

                      }


              if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)
                                      evt->data.evt_gatt_server_characteristic_status.status_flags)
                           {
                                 bledata->Indication_in_Flight = false;

                           }
              else if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable)
                  {
                          ble_data.IndicationsEnabled = 0;
                          Temp_Indicated = false;
                          LOG_INFO("HTM Indications Disabled");
                          displayPrintf( DISPLAY_ROW_TEMPVALUE, " ",temperature_in_C);
                          gpioLed0SetOff();
                  }

             }


          if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_button_state)
               {

                                //check if indication flag is enabled
                                if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_indication)
                                  {
                                    bledata->button_indication = true;

                                    //(Button_Indicated)?gpioLed1SetOn():gpioLed1SetOff();
                                    gpioLed1SetOn();
                                  }
                                if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)
                                                                      evt->data.evt_gatt_server_characteristic_status.status_flags)
                                  {
                                    bledata->Indication_in_Flight = false;

                                  }


                                else if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable)
                                   {
                                     bledata->button_indication = false;
                                    LOG_INFO(" Button Indications Disabled \n");
                                      gpioLed1SetOff();
                                   }



                      }



        if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_Ambient_light_Sensor)
                       {

                                        //check if indication flag is enabled
                                        if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_indication)
                                          {
                                           // bledata->button_indication = true;
                                            Ambient_Indicated =true;
                                            //(Button_Indicated)?gpioLed1SetOn():gpioLed1SetOff();
                                            gpioLed1SetOn();
                                          }
                                        if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)
                                                                              evt->data.evt_gatt_server_characteristic_status.status_flags)
                                          {
                                            bledata->Indication_in_Flight = false;

                                          }


                                        else if(evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable)
                                           {
                                             //bledata->button_indication = false;
                                            LOG_INFO(" Button Indications Disabled \n");
                                            Ambient_Indicated =false;
                                              gpioLed1SetOff();
                                           }



                              }
                }



        break;

     case sl_bt_evt_gatt_server_indication_timeout_id:

           LOG_ERROR("server indication timeout\n\r");
           break;
#else

     case sl_bt_evt_scanner_scan_report_id:


          // Parse advertisement packets
          if (evt->data.evt_scanner_scan_report.packet_type == 0) {
             //LOG_INFO(" server address: %x %x %x %x %x %x ",evt->data.evt_scanner_scan_report.address.addr[0]
              //     ,evt->data.evt_scanner_scan_report.address.addr[1],evt->data.evt_scanner_scan_report.address.addr[2]
                //          ,evt->data.evt_scanner_scan_report.address.addr[3],evt->data.evt_scanner_scan_report.address.addr[4],
                 //        evt->data.evt_scanner_scan_report.address.addr[5]);
              //check whether it is desired server

             // LOG_INFO("%x %x %x %x %x %x ",server_addr[0],server_addr[1],server_addr[2],server_addr[3],server_addr[4],server_addr[5]);
              if((evt->data.evt_scanner_scan_report.address.addr[0] == server_addr->addr[0]) &&
                  (evt->data.evt_scanner_scan_report.address.addr[1] == server_addr->addr[1]) &&
                  (evt->data.evt_scanner_scan_report.address.addr[2] == server_addr->addr[2]) &&
                  (evt->data.evt_scanner_scan_report.address.addr[3] == server_addr->addr[3]) &&
                  (evt->data.evt_scanner_scan_report.address.addr[4] == server_addr->addr[4]) &&
                  (evt->data.evt_scanner_scan_report.address.addr[5] == server_addr->addr[5])
                 // &&
                 // (evt->data.evt_scanner_scan_report.address_type==0)
                  ) {

                  LOG_INFO(" Client -> Desired Server is connected ");
                  sc = sl_bt_scanner_stop();
                  if(sc != SL_STATUS_OK) {
                      LOG_ERROR("sl_bt_scanner_stop()  failed with error code : 0x%04x\n\r", (unsigned int)sc);
                  }

                  sc = sl_bt_connection_open(evt->data.evt_scanner_scan_report.address,
                                             evt->data.evt_scanner_scan_report.address_type, sl_bt_gap_1m_phy, NULL);
                  if(sc != SL_STATUS_OK) {
                      LOG_ERROR("sl_bt_connection_open()  failed with error code : 0x%04x\n\r", (unsigned int)sc);
                  }
              }

              //LOG_INFO(" scanner set report is entered");
          }
    // Check do you need a confirmation check received from the client here for In flight logic
          break;

     case sl_bt_evt_gatt_procedure_completed_id:

       if(evt->data.evt_gatt_procedure_completed.result == 0x110F) {
            LOG_INFO("Entered 0X110F error code state");
                //increase security(numeric comparison)
                sc = sl_bt_sm_increase_security(bledata->ConnectionHandle);
                if(sc != SL_STATUS_OK) {
                    LOG_ERROR("sl_bt_sm_increase_security() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                }
            }


       break;





     case sl_bt_evt_gatt_service_id:
       if(memcmp(evt->data.evt_gatt_service.uuid.data, thermo_service, sizeof(thermo_service)) == 0)
         {
           bledata->ServiceHandle = evt->data.evt_gatt_service.service;
         }

       else if(memcmp(evt->data.evt_gatt_service.uuid.data, button_service, sizeof(button_service)) == 0)
         {
           bledata->Servicehandle_Button = evt->data.evt_gatt_service.service;
         }

       else if(memcmp(evt->data.evt_gatt_service.uuid.data, ambient_service, sizeof(ambient_service)) == 0)
         {
           bledata->Servicehandle_Ambient = evt->data.evt_gatt_service.service;
         }

       //ambient service
       else if(memcmp(evt->data.evt_gatt_service.uuid.data, gesture_service, sizeof(gesture_service)) == 0)
                {
                  bledata->Servicehandle_Gesture = evt->data.evt_gatt_service.service;
                }

            break;


     case sl_bt_evt_gatt_characteristic_id:
       if(memcmp(evt->data.evt_gatt_characteristic.uuid.data, thermo_char, sizeof(thermo_char)) == 0)
         {
           bledata->CharacteristicHandle = evt->data.evt_gatt_characteristic.characteristic;
         }

       else if(memcmp(evt->data.evt_gatt_characteristic.uuid.data, button_char, sizeof(button_char)) == 0)
         {
           bledata->CharacteristicHandle_Button = evt->data.evt_gatt_characteristic.characteristic;
         }
       else if(memcmp(evt->data.evt_gatt_characteristic.uuid.data, ambient_char, sizeof(ambient_char)) == 0)
               {
                 bledata->CharacteristicHandle_Ambient = evt->data.evt_gatt_characteristic.characteristic;
               }

       //ambient characteristic

       else if(memcmp(evt->data.evt_gatt_characteristic.uuid.data, gesture_char, sizeof(gesture_char)) == 0)
                      {
                        bledata->CharacteristicHandle_Gesture = evt->data.evt_gatt_characteristic.characteristic;
                      }
             break;

     case sl_bt_evt_gatt_characteristic_value_id:

       //<change required>
       if(evt->data.evt_gatt_characteristic_value.att_opcode == sl_bt_gatt_handle_value_indication) {
       sc = sl_bt_gatt_send_characteristic_confirmation(bledata->ConnectionHandle);

            if(sc != SL_STATUS_OK) {
                LOG_ERROR("sl_bt_gatt_set_characteristic_notification()  failed with error code : 0x%04x\n\r", (unsigned int)sc);
            }
       }
            //save value got from server in a variable
           //  uint8_t *ServerP = &evt->data.evt_gatt_characteristic_value.value.data;
       if(evt->data.evt_gatt_characteristic_value.characteristic == bledata->CharacteristicHandle)
         {
           bledata->Server_Value = evt->data.evt_gatt_characteristic_value.value.data;

                     //LOG_INFO("server from server %d %d \n",evt->data.evt_gatt_characteristic_value.value.data[0],evt->data.evt_gatt_characteristic_value.value.data[1]);

                      Received_temperature_in_C = FLOAT_TO_INT32(bledata->Server_Value);
                      displayPrintf(DISPLAY_ROW_TEMPVALUE, "Temp = %d C", Received_temperature_in_C);
                      LOG_INFO("Client : Received Temperature from Server = %d C",Received_temperature_in_C);
                      bledata->Indication_in_Flight = false;
         }

       if(evt->data.evt_gatt_characteristic_value.characteristic == bledata->CharacteristicHandle_Ambient)
             {
               bledata->Ambient_Value_MSB = evt->data.evt_gatt_characteristic_value.value.data[0];
               bledata->Ambient_Value_LSB = evt->data.evt_gatt_characteristic_value.value.data[1];

               uint8_t Ambient_Val_LSB = evt->data.evt_gatt_characteristic_value.value.data[0];
               uint8_t Ambient_Val_MSB = evt->data.evt_gatt_characteristic_value.value.data[1];


                         //LOG_INFO("server from server %d %d \n",evt->data.evt_gatt_characteristic_value.value.data[0],evt->data.evt_gatt_characteristic_value.value.data[1]);

               //for debug commented           // Receieved_Ambient = FLOAT_TO_INT32(bledata->Ambient_Value);

                          uint16_t temp = (Ambient_Val_MSB<<8);
                        //  LOG_INFO("Client : Temp value % d \n ",temp);
                           Receieved_Ambient = temp|Ambient_Val_LSB;


                        //  displayPrintf(DISPLAY_ROW_TEMPVALUE, "Ambient val = %d lux", Ambient_Val_LSB);
                         // displayPrintf(DISPLAY_ROW_8, "Ambient val = %d lux", Ambient_Val_MSB);
                          displayPrintf(DISPLAY_ROW_8, "Amb val = %d lux", Receieved_Ambient);
                         // LOG_INFO("Client : Received ambient sensor from Server = %d lux",Receieved_Ambient);
                          bledata->Indication_in_Flight = false;
             }

           // LOG_INFO(" evt gatt characteristic value is entered & temperature can be seen");


            if(evt->data.evt_gatt_characteristic_value.att_opcode == sl_bt_gatt_read_response)
              {
               // LOG_INFO(" data value for read response data[0] = %x ",evt->data.evt_gatt_characteristic_value.value.data[0]);
               // LOG_INFO(" data value for read response data [1] = %x ",evt->data.evt_gatt_characteristic_value.value.data[1]);
                      if(evt->data.evt_gatt_characteristic_value.value.data[0] == 0x01) {
                          displayPrintf(DISPLAY_ROW_9, "Button Pressed");
                      }
                      else if(evt->data.evt_gatt_characteristic_value.value.data[0] == 0x00){
                          displayPrintf(DISPLAY_ROW_9, "Button Released");
                      }
                  }


                // LOG_INFO(" bonded = %d , button_indication = %d charactertistic button handle = %d \n",bledata->bonded,bledata->button_indication,evt->data.evt_gatt_characteristic_value.characteristic == bledata->CharacteristicHandle_Button);
                // LOG_INFO(" Button Indicated value check  = %d \n",Button_Indicated);
                  if(bledata->bonded && Button_Indicated && (evt->data.evt_gatt_characteristic_value.characteristic == bledata->CharacteristicHandle_Button))
                    {
                      //LOG_INFO(" data value for characteristic= %x ",evt->data.evt_gatt_characteristic_value.value.data[0]);
                      //LOG_INFO(" data value for characteristic = %x ",evt->data.evt_gatt_characteristic_value.value.data[1]);
                      if(evt->data.evt_gatt_characteristic_value.value.data[1] == 0x01)
                      {
                          displayPrintf(DISPLAY_ROW_9, "Button Pressed");
                      }
                      else if(evt->data.evt_gatt_characteristic_value.value.data[1] == 0x00){
                          displayPrintf(DISPLAY_ROW_9, "Button Released");
                      }
                  }

            break;

#endif

   }
}




/*
 * @description: Function to send Data values when indicated
 *
 * @params: None
 *
 * @return: None
 *
 */

void Sendindication()
{
  queue_struct_t Send_temp_data;
  ble_data_struct_t *bledata = getBLEdata();
  sl_status_t sc = 0;
  uint8_t htm_temperature_buffer[5]={0};
  uint8_t *p = htm_temperature_buffer;
   uint32_t htm_temperature_flt;

   sc =  sl_bt_gatt_server_write_attribute_value( gattdb_temperature_measurement, // handle from gatt_db.h
                                                            0, // offset
                                                            4, // length
                                                            &temperature_in_C);  // pointer to buffer where data is

                   if(sc!=SL_STATUS_OK)
                       {
                          LOG_ERROR(" GATT SERVER WRITE ATTRIBUTE FAILED With the ERROR COD;lkjhE = 0x%04x\r\n",(unsigned int)sc);
                       }

  UINT8_TO_BITSTREAM(p, 0x00);

  htm_temperature_flt = UINT32_TO_FLOAT(temperature_in_C*1000, -3);

             UINT32_TO_BITSTREAM(p, htm_temperature_flt);



if(Temp_Indicated ==true)
  {

  if(bledata->Indication_in_Flight == true)
    {
      Send_temp_data.charHandle = gattdb_temperature_measurement;
      Send_temp_data.bufferLength = 5;
      for(int i=0;i<5;i++)
        {
          Send_temp_data.buffer[i]=htm_temperature_buffer[i];
        }
      response = write_queue(Send_temp_data);
      if(response == 1)
        {
          Queued_Indications++;
          //LOG_INFO("Successfully queued ");
        }

      CBFIFO_Length = get_queue_depth();
      LOG_INFO(" CBFIFO LENGTH after temperature value inserted : %d \n\r",CBFIFO_Length);

    }
  else
    {
      sc = sl_bt_gatt_server_send_indication(ble_data.ConnectionHandle,

                                                    gattdb_temperature_measurement, // handle from gatt_db.h
                                                    5,
                                                    &htm_temperature_buffer[0]);


      bledata->Indication_in_Flight = true;
             if(sc!=SL_STATUS_OK)
                          {
                           LOG_ERROR(" Problem with the server send indication function With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                           bledata->Indication_in_Flight = false;
                          }

    }

  }
}
void Sendindication_AmbientLight()
{
  //ble_data_struct_t *bledata = getBLEdata();
    sl_status_t sc = 0;

   // Ambient_Sensor_Value = 1111;
     sc =  sl_bt_gatt_server_write_attribute_value( gattdb_Ambient_light_Sensor, // handle from gatt_db.h
                                                              0, // offset
                                                              2, // length
                                                              &Ambient_Sensor_Value);  // pointer to buffer where data is
   //  LOG_INFO(" sending val  = %d\r\n",  Ambient_Sensor_Value);

                     if(sc!=SL_STATUS_OK)
                         {
                            LOG_ERROR(" GATT SERVER WRITE ATTRIBUTE FAILED With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                         }


  if(Ambient_Indicated ==true)
    {

      {
        sc = sl_bt_gatt_server_send_indication(ble_data.ConnectionHandle,

                                               gattdb_Ambient_light_Sensor, // handle from gatt_db.h
                                                      2,
                                                      &Ambient_Sensor_Value);



               if(sc!=SL_STATUS_OK)
                            {
                             LOG_ERROR(" Problem with the server send indication function With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);

                            }

      }

    }
}

void Sendindication_GestureValue(uint8_t Value)
{
  //ble_data_struct_t *bledata = getBLEdata();
    sl_status_t sc = 0;

   // Ambient_Sensor_Value = 1111;
     sc =  sl_bt_gatt_server_write_attribute_value( gattdb_gesture_value, // handle from gatt_db.h
                                                              0, // offset
                                                              1, // length
                                                              &Value);  // pointer to buffer where data is
   //  LOG_INFO(" sending val  = %d\r\n",  Ambient_Sensor_Value);

                     if(sc!=SL_STATUS_OK)
                         {
                            LOG_ERROR(" GATT SERVER WRITE ATTRIBUTE FAILED With the ERROR CODE = 0x%04x\r\n",(unsigned int)sc);
                         }



}

void Gesture_Main_Logic()
{
  uint8_t received_gesture = 0;
  ble_data_struct_t *bledata = getBLEdata();
  sl_status_t sc = 0;
  sc = sl_bt_gatt_read_characteristic_value(bledata->ConnectionHandle,
                                                           bledata->CharacteristicHandle_Gesture);
                               if(sc != SL_STATUS_OK) {
                                   LOG_ERROR("sl_bt_gatt_read_characteristic_value() returned != 0 status=0x%04x\n\r", (unsigned int)sc);
                               }

              // received_gesture =

}


void Send_ButtonState(uint8_t value)
{
  queue_struct_t Send_button_data;
  ble_data_struct_t *bledata = getBLEdata();
 uint8_t Buffer[2];
 Buffer[0]=0;
 Buffer[1]=value;

 if(BT_Connected == true)
   {


             sl_status_t sc = sl_bt_gatt_server_write_attribute_value(gattdb_button_state,
                                                                    0,
                                                                    1,
                                                                    &Buffer[1]);

               if(sc != SL_STATUS_OK)
                 {
                   LOG_ERROR("sl_bt_gatt_server_write_attribute_value() Failed with ERROR CODE = 0x%04x\n\r", (unsigned int)sc);
                 }

   if(bledata->button_indication == true)
     {


          if(bledata->Indication_in_Flight)
            {

              Send_button_data.charHandle = gattdb_button_state;
              Send_button_data.bufferLength = 2;
                    for(int i=0;i<2;i++)
                      {
                        Send_button_data.buffer[i]=Buffer[i];
                      }
                    response = write_queue(Send_button_data);
                    if(response == 1)
                      {
                        Queued_Indications++;
                       // printf("Successfully queued ");
                      }

                    CBFIFO_Length = get_queue_depth();
                    LOG_INFO(" CBFIFO LENGTH after button state inserted: %d \n\r",CBFIFO_Length);


            }
          else
            {
              sc = sl_bt_gatt_server_send_indication(bledata->ConnectionHandle,
                                                                              gattdb_button_state,
                                                                              2,
                                                                              &Buffer[0]);
                                       if(sc != SL_STATUS_OK)
                                         {
                                           LOG_ERROR("sl_bt_gatt_server_send_indication() failed in sendbutton state with error code =0x%04x\n\r", (unsigned int)sc);
                                         }
                                       else
                                         {
                                           bledata->Indication_in_Flight = true;
                                       }
                                     //  printf(" else condition send indication ");

            }

   }

   }
}
