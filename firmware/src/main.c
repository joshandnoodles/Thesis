/*
 * Project name:
     clicker2 for PIC32MX (Simple 'Hello World' project)
 * Copyright:
     (c) Mikroelektronika, 2015.
 * Revision History:
       - initial release (FJ);
 * Description:
     This is a simple 'Hello World' project. It turns on/off LEDs connected to
     RE3 and RA10 pins depending on pressed buttons. Left button changes mode
     of blinking and right button changes frequency of blinking.
 * Test configuration:
     MCU:             PIC32MX460F512L
                      http://ww1.microchip.com/downloads/en/DeviceDoc/61143G.pdf
     Dev.Board:       clicker2 for PIC32MX
                      http://www.mikroe.com/pic32/clicker-2-pic32mx/
     Oscillator:      XT PLL, 80.000MHz
     Ext. Modules:    None.
     SW:              mikroC PRO for PIC32
                      http://www.mikroe.com/mikroc/pic32/
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include "timer.h"
#include "adc.h"
#include "qp.h"
#include "lsr.h"
#include "mod.h"
#include "gimbal.h"
#include "debug.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes

// main function
void main() {
  
  // before we begin anything, initialize the following:
  SYS_Initialize( NULL );   // all MPLAB Harmony modules, including app(s)
  initCoreTimer();          // core timer
  initAdc();                // ADC configuration
  initDelay();              // blocking delays
  initQp();                 // quadrant photodiode circuitry
  initLsr();                // laser driver and diode circuitry
  initMod();                // modulation processes/hardware
  initGimbal();             // gimbal controlling pan/tilt mechanisms
  initDebug();              // debug hardware on the Clicker 2
  
  lsrLoadSwitchOn();
  modOn();
  gimbalOn();
  #ifdef MASTER
    gimbalSetPan( 90 );
    gimbalSetTilt( 6 );
  #else
    gimbalSetPan( 90 );
    gimbalSetTilt( 7 );
  #endif
  
  while (1) {     // loop forever
    
    // run operations for MPLAB Harmony modules and application(s)
    SYS_Tasks();
    
    if ( debugBtn1State() ) {             // wait for button to be depressed
      while( debugBtn1State() ) {         // hold here until button is released
        SERVO_PAN_OCRS = SERVO_PAN_OCR - 1; 
        SERVO_TILT_OCRS = SERVO_TILT_OCR - 1;
      }
      delayUs( BTN_DEBOUNCE_US );    // pause to avoid de-bounce issues
      debugLed1Tog();
    }
    if ( debugBtn2State() ) {             // wait for button to be depressed
      while( debugBtn2State() );          // hold here until button is released
      delayUs( BTN_DEBOUNCE_US );    // pause to avoid de-bounce issues
      debugLed2Tog();
    }
    
    // check to see if we should change our active data channel based on the 
    // intensities of the the quadrants nominal values
    switch (modRxActiveQuadrant) {
      case 1:
        if ( (MOD_DEFAULT_RX_THRES_SWAP_MULT * (*qpLastChVSenseRegPtrs[3])) > 
                (*qpLastChVSenseRegPtrs[modRxActiveQuadrant]) ) {
          modSetActiveQuadrant( 3 );
          debugLed2On();
        }
        break;
      case 3:
        if ( (MOD_DEFAULT_RX_THRES_SWAP_MULT * (*qpLastChVSenseRegPtrs[1])) > 
                (*qpLastChVSenseRegPtrs[modRxActiveQuadrant]) ) {
          modSetActiveQuadrant( 1 );
          debugLed2Off();
        }
        break;
    }
    
    // use the debug LEDs as an indicator of signal lock status
    switch (modSigLockState) {
      case 0:
        debugLed1Off();
        debugLed2Off();
        break;
      case 1:
        debugLed1On();
        debugLed2Off();
        break;
      case 2:
        debugLed1On();
        debugLed2On();
        break;
    }
    
    //lsrReadVrefVSenseReg();
    //lsrReadISenseReg();
    //lsrCheckAlarms();
  }
  
  return;
}



