/*
   gpio.h
  
    Created on: Dec 12, 2018
        Author: Dan Walkes

    Updated by Dave Sluiter Sept 7, 2020. moved #defines from .c to .h file.
    Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#define LED0_port  gpioPortF // change to correct ports and pins
#define LED0_pin   4
#define LED1_port  gpioPortF
#define LED1_pin   5

#define SI7021_PORT gpioPortD
#define SI7021_PIN  15

#define I2C0_SCL_PORT  gpioPortC
#define I2C0_SCL_PIN   10
#define I2C0_SDA_PORT  gpioPortC
#define I2C0_SDA_PIN   11

#define LCD_DISPLAY_PORT gpioPortD
#define LCD_DISPLAY_PIN  15

#define EXTCOMIN 13

#define PB0_port gpioPortF
#define PB0_pin  6

#define PB1_port gpioPortF
#define PB1_pin  7

#define AMBIENT_port gpioPortD
#define AMBIENT_pin 10


// Function prototypes
void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();

void gpioSi7021Enable();
void gpioSi7021Disable();

void gpioSensorEnSetOn();

void gpioSetDisplayExtcomin(bool value);
/*
void gpioI2cSclDisable();
void gpioI2cSdaDisable();
*/

void AmbientSensorOn();
void AmbientSensorOff();

#endif /* SRC_GPIO_H_ */
