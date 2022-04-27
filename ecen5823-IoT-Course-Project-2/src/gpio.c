/*
  gpio.c
 
   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.

   Edited by : Ganesh KM for Assignment 3

 */


// *****************************************************************************
// Students:
// We will be creating additional functions that configure and manipulate GPIOs.
// For any new GPIO function you create, place that function in this file.
// *****************************************************************************

#include <stdbool.h>
#include "em_gpio.h"
#include <string.h>

#include "gpio.h"

// Student Edit: Define these, 0's are placeholder values.
// See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
// and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
// to determine the correct values for these.




// Set GPIO drive strengths and modes of operation
void gpioInit()
{

  // Student Edit:

	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	 //SI7021 sensor initialization
	GPIO_PinModeSet( SI7021_PORT, SI7021_PIN, gpioModePushPull, true );

	//PB0 Initialization
	GPIO_PinModeSet(PB0_port, PB0_pin, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(PB0_port, PB0_pin, PB0_pin, true, true, true);

  GPIO_PinModeSet(PB1_port, PB1_pin, gpioModeInputPullFilter, true);
	GPIO_ExtIntConfig(PB1_port, PB1_pin, PB1_pin, true, true, true);

	//Amnbient sensor
	GPIO_DriveStrengthSet(AMBIENT_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(AMBIENT_port, AMBIENT_pin, gpioModePushPull, false);



  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);

  /* Enable Interrupts in the NVIC for GPIO */
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

} // gpioInit()


void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}


void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}


void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}


void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}

void gpioSi7021Enable()
{
  GPIO_PinOutSet(SI7021_PORT,SI7021_PIN);
}

void gpioSi7021Disable()
{
  GPIO_PinOutClear(SI7021_PORT,SI7021_PIN);
}

void gpioSensorEnSetOn()
{
  GPIO_PinOutSet(LCD_DISPLAY_PORT,LCD_DISPLAY_PIN);
}
void gpioSetDisplayExtcomin(bool value)
{
 if(value == true)
   {
     GPIO_PinOutSet(LCD_DISPLAY_PORT,EXTCOMIN);
   }
 else
   {
     GPIO_PinOutClear(LCD_DISPLAY_PORT,EXTCOMIN);
   }
}

void AmbientSensorOn()
{
  GPIO_PinOutSet(AMBIENT_port,AMBIENT_pin);
}

void AmbientSensorOff()
{
  GPIO_PinOutClear(AMBIENT_port,AMBIENT_pin);
}






/* need to check --> not sure about disabling I2C after reading measurement
void gpioI2cSdaDisable() //
{
    GPIO_PinOutClear( I2C0_SDA_PORT, I2C0_SDA_PIN );
}


void gpioI2cSclDisable()
{
    GPIO_PinOutClear( I2C0_SCL_PORT, I2C0_SCL_PIN );
}
*/



