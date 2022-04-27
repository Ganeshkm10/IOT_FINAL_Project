/*
 * adc.c
 *
 *  Created on: 15-Apr-2022
 *      Author: Windows
 */

#include "em_adc.h"
#include "em_cmu.h"
#include "timers.h"
#include "gpio.h"

ADC_Init_TypeDef ADCinit = ADC_INIT_DEFAULT;
ADC_InitSingle_TypeDef ADCSingle;

void ADCInit()
{
  //CMU_ClockEnable( cmuClock_HFPER, true );
  CMU_ClockEnable(cmuClock_ADC0, true);
  ADCinit.prescale = ADC_PrescaleCalc(7000000,0);
  ADCinit.timebase = ADC_TimebaseCalc(0);
  ADCinit.ovsRateSel = adcOvsRateSel32;             //Oversampling
  ADC_Init(ADC0, &ADCinit);


  ADCSingle.acqTime     = adcAcqTime4;
  ADCSingle.reference   = adcRef5V;
  ADCSingle.resolution  = adcRes12Bit;
  ADCSingle.posSel      = adcPosSelAPORT2XCH9;
  ADCSingle.negSel      = adcNegSelVSS;
  ADCSingle.diff        = false;
  ADCSingle.prsEnable   = false;
  ADCSingle.leftAdjust  = false;
  ADCSingle.rep         = false;
  ADCSingle.fifoOverwrite = false;
  ADCSingle.singleDmaEm2Wu = false;

  ADC_InitSingle(ADC0, &ADCSingle);
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  NVIC_EnableIRQ(ADC0_IRQn);
  ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

}
