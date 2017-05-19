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

/*
// pin definitions
sbit LD1 at LATE3_bit;
sbit LD2 at LATA10_bit;
sbit LD1_Direction at  TRISE3_bit;
sbit LD2_Direction at TRISA10_bit;

sbit T1 at RE4_bit;
sbit T2 at RG9_bit;
sbit T1_Direction at TRISE4_bit;
sbit T2_Direction at TRISG9_bit;

// globals
char oldstate1 = 0, oldstate2 = 0;
char Example_State = 0;
char count;

//Timer2/3
//Prescaler 1:1; PR3 Preload = 610; PR2 Preload = 23650; Actual Interrupt Time = 500 ms
void InitTimer2_3() {
  T2CON         = 0x0;
  T3CON         = 0x0;
  TMR2          = 0;
  TMR3          = 0;
  T3IE_bit      = 1;
  T3IF_bit      = 0;
  T3IP0_bit     = 1;
  T3IP1_bit     = 1;
  T3IP2_bit     = 1;
  PR3           = 610;
  PR2           = 23650;
  T2CON         = 0x8008;
}

void Timer2_3Interrupt() iv IVT_TIMER_3 ilevel 7 ics ICS_SRS {
  char temp;

  // clear Timer Interrupt flag
  T3IF_bit = 0;
  T2CON = 0x0;
  T3CON = 0x0;

  // check T1 button state
  switch (Example_State & 0x0F){
    case 0 : LD1 = 0;                   // Both LEDs are OF
             LD2 = 0;
             break;
    
    case 1 : LD1 ^= 1;                  // Only LD1 blinks
             LD2 = 0;
             break;
    
    case 2 : LD1 = 0;                   // Only LD2 blinks
             LD2 ^= 1;
             break;
    
    case 3 : LD1 ^= 1;                  // Both LEDs blinks alternately
             LD2  = !LD1;
             break;
    
    case 4 : LD1 ^= 1;                  // Both LEDs blink simultaneously
             LD2  = LD1;             
             break;
    
    default : Example_State &= 0xF0;    // reset T1 state to zero
              break;
  }
  // check T2 button state
  switch (Example_State & 0xF0){
    case 0x00 : PR3 = 610;              // Set Timer Interrupt time to 500ms
                PR2 = 23650;
                break;
    
    case 0x10 : PR3 = 488;              // Set Timer Interrupt time to 400ms
                PR2 = 18920;
                break;
    
    case 0x20 : PR3 = 366;              // Set Timer Interrupt time to 300ms
                PR2 = 14190;
                break;
    
    case 0x30 : PR3 = 244;              // Set Timer Interrupt time to 200ms
                PR2 = 9460;
                break;
    
    case 0x40 : PR3 = 122;              // Set Timer Interrupt time to 100ms
                PR2 = 4730;
                break;
    
    default :   PR3 = 610;              // Set Timer Interrupt time to 500ms
                PR2 = 23650;
                Example_State &= 0x0F;  // reset T1 state to zero
                break;
  }
  T2CON = 0x8008;
}
*/

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
  
  //gimbalOn();
  //gimbalSetTilt( 90 );
  
  //lsrLoadSwitchOn();
  //lsrSetHigh();
  
  while (1) {     // loop forever
    
    
    // run operations for MPLAB Harmony modules and application(s)
    SYS_Tasks();
    
    // trigger action when button is pressed (action occurs on release)
    //if ( debugBtn1State() ) {             // wait for button to be depressed
    //  while( debugBtn1State() );          // hold here until button is released
    //  delayUs( BTN_DEBOUNCE_US );    // pause to avoid de-bounce issues
    //  debugLed1Tog();
    //  lsrLoadSwitchTog();
    //  lsrTog();
    //}
    if ( debugBtn1State() ) {             // wait for button to be depressed
      while( debugBtn1State() ) {         // hold here until button is released
        SERVO_PAN_OCRS = SERVO_PAN_OCR - 1; 
        SERVO_TILT_OCRS = SERVO_TILT_OCR - 1;
      }
      delayUs( BTN_DEBOUNCE_US );    // pause to avoid de-bounce issues
      debugLed1Tog();
      //delayS(2);
      //AD1CHS = 0x1111<<24;
    }
    if ( debugBtn2State() ) {             // wait for button to be depressed
      while( debugBtn2State() );          // hold here until button is released
      delayUs( BTN_DEBOUNCE_US );    // pause to avoid de-bounce issues
      debugLed2Tog();
      //delayS(2);
      //AD1CHS = 0x1111<<24;
    }
    
    //modOff();
    //delayMs(1000);
    //debugLed2On();
    //gimbalSetTilt(90);
    
    //qpReadCh1VSense();
    //qpReadCh2VSense();
    //qpReadCh3VSense();
    //qpReadCh4VSense();
    //qpAlign();
    
    //lsrReadVrefVSenseReg();
    //lsrReadISenseReg();
    //lsrCheckAlarms();
    
    
    // check T1 button
    /*
    if (Button(&PORTE, 4, 2, 0)) {                // Detect logical zero
      oldstate1 = 1;                              // Update flag
    }
    if (oldstate1 && Button(&PORTE, 4, 2, 1)) {   // Detect zero-to-one transition
      oldstate1 = 0;                              // Update flag
      Example_State += 0x01;                      // set new Example state
      if ((Example_State & 0x0F) > 4)
        Example_State &= 0xF0;
    }
    // check T2 button
    if (Button(&PORTG, 9, 2, 0)) {                // Detect logical zero
      oldstate2 = 1;                              // Update flag
    }
    if (oldstate2 && Button(&PORTG, 9, 2, 1)) {   // Detect zero-to-one transition
      oldstate2 = 0;                              // Update flag
      Example_State += 0x10;                      // set new Example state
      if ((Example_State & 0xF0) > 0x40)
        Example_State &= 0x0F;
    }
    */
  }
}



