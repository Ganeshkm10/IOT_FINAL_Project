/*
 * irq.c
 *
 *  Created on: 25-Jan-2022
 *      Author: Ganesh KM
 *      Description: This file contains interrupt handler functions.
 *
 *  Modified on 11-FEB-2022 for Assignment 4
 *  Modified on 12-MAR-2022 for Assignment 8
 */

#include<em_letimer.h>
#include"gpio.h"
#include"irq.h"
#include "scheduler.h"
#include"em_core.h"
#include "i2c.h"
#include "em_adc.h"

#define INCLUDE_LOG_DEBUG 1
#include "log.h"


//extern I2C_TransferSeq_TypeDef seq;
//extern uint8_t CMD_READ;

uint32_t Log_time=0;
extern uint16_t Ambient_Sensor_Value;
/*
 * @description: LETIMER0 Interrupt handler function
 * @params: None
 * @return: None
 */

void LETIMER0_IRQHandler(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL(); //enter critical section
  uint32_t flags;
  flags = LETIMER_IntGetEnabled(LETIMER0); //getting the flag being set from Interrupt register

  if(flags & LETIMER_IF_UF)
    {
      schedulerSetEventUF();
      Handle_count();

      // gpioLed1SetOff();

      LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);
    }
  if(flags & LETIMER_IF_COMP1)
      {
      schedulerSetEventCOMP1();
     // gpioLed1SetOn();

        LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1);
      }
  CORE_EXIT_CRITICAL();//exit critical section
}

/*
 * @description: I2C0 Interrupt handler function
 * @params: None
 * @return: None
 */

void I2C0_IRQHandler(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  I2C_TransferReturn_TypeDef transferstatus;

 // sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
  transferstatus = I2C_Transfer(I2C0);

  if(transferstatus == i2cTransferDone)
    {
      schedulerSetEvent_I2C_TransferComplete();
     // LOG_INFO(" I2C transfer is done ");
    }

  if(transferstatus < 0)
    {
     // LOG_ERROR("%d", transferstatus);
    }
  CORE_EXIT_CRITICAL();
 }

void ADC0_IRQHandler()
{
  CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();


  uint32_t flags = ADC_IntGetEnabled(ADC0);


  ADC_IntClear(ADC0, flags);

  Ambient_Sensor_Value = ADC_DataSingleGet(ADC0);
 // LOG_INFO(" ADC IRQ Val : %d \n",Ambient_Sensor_Value);

  if(flags == ADC_IEN_SINGLE)
    {
      schedulerSetEvent_ADC();
    }
  CORE_EXIT_CRITICAL();
}

/*
 * @description: GPIO PB0 Interrupt handler function
 * @params: None
 * @return: None
 */

void GPIO_EVEN_IRQHandler(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  uint32_t flags;
  flags = GPIO_IntGetEnabled();
  uint8_t PB_Status = GPIO_PinInGet(PB0_port, PB0_pin);

  if(flags==(1<<PB0_pin))
    {
      if(!PB_Status)
        {
          schedulerSetEventButtonPressed_PB0();
          GPIO_IntClear(flags);
        }
      else
        {
          schedulerSetEventButtonReleased_PB0();
          GPIO_IntClear(flags);
        }
    }

  CORE_EXIT_CRITICAL();
}

#if  !DEVICE_IS_BLE_SERVER
void GPIO_ODD_IRQHandler(void)
{

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
    uint32_t flags;
    flags = GPIO_IntGetEnabled();
    uint8_t PB_Status = GPIO_PinInGet(PB1_port, PB1_pin);

    if(flags==(1<<PB1_pin))
      {
        if(!PB_Status)
          {
            schedulerSetEventButtonPressed_PB1();
            GPIO_IntClear(flags);
          }
        else
          {
            schedulerSetEventButtonReleased_PB1();
            GPIO_IntClear(flags);
          }
      }

    CORE_EXIT_CRITICAL();
}

#endif













void Handle_count(void)
{
  Log_time+=3000;
}

/*
 * @description: Ms calculation function used while logging
 * @params: None
 * @return: None
 */

uint32_t letimerMilliseconds()
{
 return Log_time;
}


