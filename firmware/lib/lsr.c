/* 
 * File:        lsr.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  All core operations of laser diode and laser diode driver can be found here.
 *  For this project, the hardware specifications are the following:
 *    - PL 450B laser diode
 *    - IC-HKB laser driver
 *  Note: This file only contains functionality for initialization, power-up, 
 *  current limit setting, and basic switching operations. Any complex 
 *  modulation operations should be externalized in another file.
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include "adc.h"

#include "lsr.h"

// variables to keep track of state changes
unsigned char lsrLoadSwitchState;
unsigned char lsrEnCh1State;
unsigned char lsrEnCh2State;  //!! currently unused

// variables to keep track of alarms
unsigned char lsrVrefVSenseAlarm;
unsigned char lsrISenseAlarm;

// variables to keep track of last values retrieved
unsigned int lsrLastVrefVSenseReg;
unsigned int lsrLastISenseReg;

// variables to store register equivalents of current/Vref limits
unsigned int lsrVrefVSenseMinReg;
unsigned int lsrVrefVSenseMaxReg;
unsigned int lsrISenseMinReg;
unsigned int lsrISenseMaxReg;

void initLsr( void ) {
  
  // specify the correct tri-state register configuration for the following:
  LSR_LOAD_SWITCH_TRIS &= ~LSR_LOAD_SWITCH_MASK;     // load switch
  LSR_EN_CH1_TRIS &= ~LSR_EN_CH1_MASK;               // EN CH1
  LSR_EN_CH2_TRIS &= ~LSR_EN_CH2_MASK;               // EN CH2
  
  // it is smart to start off with no power to the driver/diode
  lsrLoadSwitchOff();
  
  // configure ADC channel for Vref measurements
  adcInitCh( &LSR_VREF_VSENSE_AN_BUF,
          LSR_VREF_VSENSE_AN_MASK,
          &LSR_VREF_VSENSE_TRIS,
          LSR_VREF_VSENSE_MASK );
  
  // configure ADC channel for laser current low-side measurements
  adcInitCh( &LSR_ISENSE_LS_AN_BUF,
          LSR_ISENSE_LS_AN_MASK,
          &LSR_ISENSE_LS_TRIS,
          LSR_ISENSE_LS_MASK );
  
  // configure ADC channel for laser current high-side measurements
  adcInitCh( &LSR_ISENSE_HS_AN_BUF,
          LSR_ISENSE_HS_AN_MASK,
          &LSR_ISENSE_HS_TRIS,
          LSR_ISENSE_HS_MASK );
  
  // pre-calculate register equivalent values for current and Vref 
  // minimum/maximum limits to test ADC results against after ADC readings
  lsrVrefVSenseMinReg = (unsigned int)(LSR_VREF_VSENSE_MIN / ADC_REF_V * ((1<<ADC_BITS)-1));
  lsrVrefVSenseMaxReg = (unsigned int)(LSR_VREF_VSENSE_MAX / ADC_REF_V * ((1<<ADC_BITS)-1));
  lsrISenseMinReg = (unsigned int)(LSR_ISENSE_MIN / ADC_REF_V * ((1<<ADC_BITS)-1) / LSR_ISENSE_RES) ;
  lsrISenseMaxReg = (unsigned int)(LSR_ISENSE_MAX / ADC_REF_V * ((1<<ADC_BITS)-1) / LSR_ISENSE_RES);

  return;
}

unsigned char lsrLoadSwitchOn( void ) {
  
  // only turn on the load switch if the current limit Vref node is within the 
  // acceptable range, force this check by calling Vref read function
  lsrLastVrefVSenseReg = lsrReadVrefVSenseReg();
  
  // do not continue with load switch turn on procedure if we have an alarm flag
  if ( lsrVrefVSenseAlarm )
    return 0x0;
  
  // if we made it here, we should be safe to turn on the load switch
  LSR_LOAD_SWITCH_LAT |= LSR_LOAD_SWITCH_MASK;
  
  // change flag(s) to keep track of state changes
  lsrLoadSwitchState = 0x1;
  
  return 0x1;
}

unsigned char lsrLoadSwitchOff( void ) {
  
  // no checks needed here, just turn the load switch off
  LSR_LOAD_SWITCH_LAT &= ~LSR_LOAD_SWITCH_MASK;
  
  // change flag(s) to keep track of state changes
  lsrLoadSwitchState = 0x0;
  
  return 0x1;
}

unsigned char lsrLoadSwitchTog( void ) {
  
  // look at load switch state and toggle appropriately
  if ( lsrLoadSwitchState )
    return lsrLoadSwitchOff();
  else
    return lsrLoadSwitchOn();
  
}

float lsrReadVrefVSense( void ) {
  
  // call on appropriate function that samples, converts, and returns the ADC 
  // buffer result register and send that register reference to decimal
  // conversion function
  return adcRegToFloat( lsrReadVrefVSenseReg() );
}

unsigned int lsrReadVrefVSenseReg( void ) {
  
  volatile unsigned int * vrefReg;
  
  // use modular ADC read function to sample, convert, and return ADC buffer 
  // result, this assumes the particular channel being read has already been
  // initialized through the ADC channel initialization function
  vrefReg = adcRead( LSR_VREF_VSENSE_AN_CH );
  
  // store value as normal unsigned int value (not pointer)
  lsrLastVrefVSenseReg = (unsigned int)(*vrefReg);

  // reset alarm flag
  lsrVrefVSenseAlarm = 0x0;
  
  // check ADC buffer register to see if we are outside our limits and set alarm 
  // flag if we are violating any limits imposed
  if ( ( lsrLastVrefVSenseReg < lsrVrefVSenseMinReg ) || 
       ( lsrLastVrefVSenseReg > lsrVrefVSenseMaxReg ) )
    lsrVrefVSenseAlarm = 0x1;
  
  return lsrLastVrefVSenseReg;
}

unsigned int lsrReadISenseReg( void ) {
  
  volatile unsigned int * iSenseLsReg;
  volatile unsigned int * iSenseHsReg;
  
  // use modular ADC read function to sample, convert, and return ADC buffer 
  // result, this assumes the particular channel being read has already been
  // initialized through the ADC channel initialization function
  iSenseLsReg = adcRead( LSR_ISENSE_LS_AN_CH );
  iSenseHsReg = adcRead( LSR_ISENSE_HS_AN_CH );
  
  // calculate current value using high-side and low-side ADC measurements along
  // with knowledge of sense circuitry
  lsrLastISenseReg = *iSenseHsReg - *iSenseLsReg;
  
  // reset alarm flag
  lsrISenseAlarm = 0x0;
  
  // check register to see if we are outside our limits and set alarm 
  // flag if we are violating any limits imposed
  if ( ( lsrLastISenseReg < lsrISenseMinReg ) || 
       ( lsrLastISenseReg > lsrISenseMaxReg ) )
    lsrISenseAlarm = 0x1;
  
  return lsrLastISenseReg;
}

inline void lsrSetHigh( void ) {
  
  // the toggling of laser driver enable channels is dependent on hardware
  // configuration of RK1 and RK2 resistors
  LSR_EN_CH1_LAT |= LSR_EN_CH1_MASK; 
  
  // change flag(s) to keep track of state changes
  lsrEnCh1State = 0x1;
  
  return;
}

inline void lsrSetLow( void ) {
  
  // the toggling of laser driver enable channels is dependent on hardware
  // configuration of RK1 and RK2 resistors
  LSR_EN_CH1_LAT &= ~LSR_EN_CH1_MASK;
  
  // change flag(s) to keep track of state changes
  lsrEnCh1State = 0x0;
  
  return;
}

inline void lsrTog( void ) {
  
  // look at laser switch state and toggle appropriately
  if ( lsrEnCh1State )
    return lsrSetLow();
  else
    return lsrSetHigh();
  
  return;
}

void lsrCheckAlarms( void ) {
  
  // immediately turn of diode/driver power it has an unacceptable Vref value or
  // is operating outside its acceptable current range
  if ( lsrVrefVSenseAlarm || lsrISenseAlarm ) {
    lsrSetLow();
    lsrLoadSwitchOff();
  }
  
  return;
}