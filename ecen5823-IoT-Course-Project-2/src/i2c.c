/*
 * i2c.c
 *  Description : This file contains all the I2C related function definitions.
 *  Created on  : 02-Feb-2022
 *      Author  : Ganesh KM
 *
 *  Modified on : 11-FEB-2022 for Assignment 4
 *                19-FEB-2022 for Assignment 5
 */

#include "i2c.h"
#include "lcd.h"

#define INCLUDE_LOG_DEBUG 1
#include "log.h"
#include "oscillators.h"
#include "ble.h"



typedef struct{
  uint8_t data[2];
  uint16_t BUFFERLEN;
}I2CDATA;

I2CDATA BUFFER;

I2CSPM_Init_TypeDef I2C_Config;

I2C_TransferSeq_TypeDef seq;

uint8_t CMD_READ = 0XF3;
float temp;
uint16_t temperature_in_C = 0;
extern uint16_t temperature_in_C;


static uint8_t gesture_data_array[DATA_ARRAY_SIZE];

static gesture_list current_gesture = no_gesture;



 /*
  * @description: Function to initialize I2C
  *
  * @params: None
  *
  * @return: None
  */

void Init_I2C()
{
  I2C_Config.port = I2C0,
       I2C_Config.sclPort = gpioPortC;
       I2C_Config.sclPin = 10;
       I2C_Config.sdaPort = gpioPortC;
       I2C_Config.sdaPin = 11;
       I2C_Config.portLocationScl = 14;
       I2C_Config.portLocationSda = 16;
       I2C_Config.i2cRefFreq = 0;
       I2C_Config.i2cMaxFreq = I2C_FREQ_STANDARD_MAX;
       I2C_Config.i2cClhr = i2cClockHLRStandard;

  I2CSPM_Init(&I2C_Config);

}

/*
 * @description: Function for I2C READ functionality
 *
 * @params: pointer to buffer, Length
 *
 * @return: Transferstatus : Tells whether I2C READ is done or not.
 */


I2C_TransferReturn_TypeDef I2C_READ(uint8_t *BUFFER, uint16_t BUFFERLEN)
{

   I2C_TransferReturn_TypeDef transferstatus;

  seq.addr = SI7021_ADDRESS << 1;
  seq.flags = I2C_FLAG_READ;
  seq.buf[0].data = BUFFER;
  seq.buf[0].len = BUFFERLEN;
  NVIC_EnableIRQ( I2C0_IRQn );

   transferstatus = I2C_TransferInit(I2C0, &seq);

 /* if(transferstatus != i2cTransferDone)
    {
       LOG_ERROR("I2CSPM_TRANSFER : I2C BUS READ IS FAILED");
    }*/

   return transferstatus;
}

/*
 * @description: Function for I2C Write functionality
 *
 * @params: pointer to CMD_READ, Length
 *
 * @return: Transferstatus : Tells whether I2C Write is done or not.
 */

I2C_TransferReturn_TypeDef I2C_WRITE(uint16_t CMD_LEN)
{
  I2C_TransferReturn_TypeDef transferstatus;


  seq.addr = SI7021_ADDRESS << 1;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = &CMD_READ;
  seq.buf[0].len = CMD_LEN;
  NVIC_EnableIRQ( I2C0_IRQn );


  transferstatus = I2C_TransferInit(I2C0, &seq);

 /*if(transferstatus == i2cTransferInProgress)
  {
      //LOG_ERROR("I2CSPM_TRANSFER in progress : I2C BUS WRITE OF CMD = 0XF3 IS FAILED");
  }
 */
  //NVIC_DisableIRQ(I2C0_IRQn);
return transferstatus;
}

/*
 * @description: Utility function to Get temeperature from SI7021 sensor
 *
 * @params: void
 *
 * @return: void.
 */

void I2C_TemperatureMeasurement()
{

  I2C_TransferReturn_TypeDef status;
  BUFFER.BUFFERLEN = 2;

      status = I2C_READ(BUFFER.data,BUFFER.BUFFERLEN);

      //if(i2cTransferDone == status)
        {
          temp = (( BUFFER.data[0] << 8 ) | (BUFFER.data[1]));  //Temperature calculation as per manual
          temperature_in_C = ( ( 175.72 * (temp) ) / 65536 ) - 46.85;
         //LOG_INFO("transfer is done in I2C READ \n\r");
        }
      if(i2cTransferInProgress == status)
        {
          //LOG_INFO(" transfer in progress in I2C READ \n\r");
        }


      LOG_INFO(" Temperature = %d C ",temperature_in_C);
      //displayPrintf( DISPLAY_ROW_TEMPVALUE, "Temp = %d C",temperature_in_C);



      Sendindication(); //function to send indication to BT stack

    //  LOG_INFO( "\n \r buffer DATA written by I2C transfer in Hex values (Only for Debug) is %X %X \n", BUFFER.data[0],BUFFER.data[1] );

     // LOG_INFO( "\n \r buffer DATA is %d %d \n", BUFFER.data[0],BUFFER.data[1] );


}



/*
 * -----------------------------------------------------------------
 * */

void send_register_address()
{
  Init_I2C();
  I2C_TransferSeq_TypeDef i2cTransfer;

  i2cTransfer.addr = ZX_GESTURE_SENSOR_ADDRESS;
  i2cTransfer.flags = I2C_FLAG_WRITE_READ;

  uint8_t sensor_model_reg_addr = 0x04;
  i2cTransfer.buf[0].data = &(sensor_model_reg_addr);
  i2cTransfer.buf[0].len = sizeof(sensor_model_reg_addr);

  i2cTransfer.buf[1].data = gesture_data_array;
  i2cTransfer.buf[1].len = sizeof(gesture_data_array);

//  NVIC_EnableIRQ(I2C0_IRQn);
//  sl_power_manager_add_em_requirement(1);

  I2C_TransferReturn_TypeDef transferStatus = I2C_TransferInit(I2C0, &i2cTransfer);

  while(transferStatus != i2cTransferDone){
      transferStatus = I2C_Transfer(I2C0);
  }

  schedulerSetEvent_I2C_TransferComplete();
}


void read_register_data()
{
  //printf("register data = 0x%x\r\n", data_array[0]);

//  if(gesture_data_array[0] != no_gesture){
//
//  }

  switch(gesture_data_array[0])
  {
    case LEFT_SWIPE:
    case HOVER_LEFT:
      current_gesture = left_swipe;
      displayPrintf(DISPLAY_ROW_9, "Left Swipe");
      break;

    case RIGHT_SWIPE:
    case HOVER_RIGHT:
      current_gesture = right_swipe;
      displayPrintf(DISPLAY_ROW_9, "Right Swipe");
      break;

    case UP_SWIPE:
    case HOVER_UP:
      current_gesture = up_swipe;
      displayPrintf(DISPLAY_ROW_9, "Up Swipe");
      break;

    default:
      current_gesture = no_gesture;
      displayPrintf(DISPLAY_ROW_9, "");
      break;
  }
  Sendindication_GestureValue(gesture_data_array[0]);
  LOG_INFO("Gesture detected = 0x%x\r\n", current_gesture);
}
