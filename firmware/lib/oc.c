/* 
 * File:        oc.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include "oc.h"

void initOutputCompare( volatile unsigned * con,
        unsigned char bits, unsigned char timer ) {
  // steps detailed in Section 16.3 p.219 of PIC32 Family user guide
  
  // configure OCxCON output compare 'x' control register
  
  // zero out register to begin configuration
  *con = 0x0000;
  
  // ON: Output Compare Peripheral On bit(1)
  // keep output compare off during configuration
  *con &= ~(0b1<<15);
  
  // bit 13 SIDL: Stop in Idle Mode bit
  // continue operation in idle mode
  *con |= 0b0<<13;
  
  // OC32: 32-bit Compare Mode bit
  if ( bits == 16 ) {
    // OCxR<15:0>/OCxRS<15:0> are used for comparisons to the 16-bit timer source
    *con |= 0b0<<5;
  } else if ( bits == 32 ) {
    // OCxR<31:0>/OCxRS<31:0> is used for comparisons to the 32-bit timer source
    *con |= 0b1<<5;
  }
  
  // OCFLT: PWM Fault Condition Status bit(2)
  // only relevant when OCM<2:0>=111, leave as no PWM fault has occurred to begin
  *con |= 0b0<<4;
  
  // OCTSEL: Output Compare Timer Select bit
  if ( timer == 2 ) {
    // Timer2 is the clock source for this output compare module
    *con |= 0b0<<3;
  } else if ( timer == 3 ) {
     // Timer3 is the clock source for this output compare module
    *con |= 0b1<<3;
  }
  
  // leave this to be done by other calling functions
  // OCM<2:0>: Output Compare Mode Select bits
  //  111 =  PWM mode on OCx; Fault pin enabled
  //  110 =  PWM mode on OCx; Fault pin disabled
  //  101 =  Initialize OCx pin low; generate continuous output pulses on OCx pin
  //  100 =  Initialize OCx pin low; generate single output pulse on OCx pin
  //  011 =  Compare event toggles OCx pin
  //  010 =  Initialize OCx pin high; compare event forces OCx pin low
  //  001 =  Initialize OCx pin low; compare event forces OCx pin high
  //  000 =  Output compare peripheral is disabled but continues to draw current
  
  return;
}

void initPwm(
        volatile unsigned * ocCon,
        volatile unsigned * ocR,
        volatile unsigned * ocRs,
        unsigned long pulsePeriodNs,
        unsigned long pulseWidthNs ,
        unsigned char bits,
        volatile unsigned * timerCon,
        volatile unsigned * timerPeriod,
        volatile unsigned * timerReg,
        unsigned char timerIdx ) {
  
  // initialize the timing mechanism to be used for the PWM
  initTimerB(
          timerCon,
          timerPeriod,
          timerReg,
          0,
          1,
          bits,
          'i' );
  
  // run initialize function for main output compare functionality
  initOutputCompare( ocCon, bits, timerIdx );
  
  // OCM<2:0>: Output Compare Mode Select bits
  // PWM mode on OCx; Fault pin disabled
  *ocCon |= 0b110<<0;
  
  // set initial pulse period for PWM
  timerBSetPeriodNs( timerCon, timerPeriod, pulsePeriodNs );
  *timerPeriod = *timerPeriod - 1;
  
  // set initial pulse width for PWM
  pwmUpdatePulseWidthNs( ocRs, timerBGetPrescaler(timerCon), pulseWidthNs );
  *ocR = *ocRs;     // need to also store in output compare 'x' register
  
  return;
}

void pwmUpdatePulseWidthNs( volatile unsigned int * ocRs, unsigned int prescaler, unsigned long nSec ) {
  
  // update register with the duty cycle calculated using knowledge of timer 
  *ocRs = (unsigned int)( ((float)(nSec)) / 1000000000.0 * ((float)(FPB)) / ((float)(prescaler)) );
  
  return;
}

void pwmOn( volatile unsigned int * ocCon, volatile unsigned int * timerCon, volatile unsigned int * timerReg ) {
  
  // reset and start up timer
  timerBReset( timerReg );
  timerBOn( timerCon );
  
  // turn on output compare module
  *ocCon |= 0b1<<15;
  
  return;
}

void pwmOff( volatile unsigned int * ocCon, volatile unsigned int * timerCon ) {
  
  // turn off output compare module
  *ocCon &= ~(0b1<<15);
  
  // shut off timer
  // just kidding don't do this as other things may be using it
  //timerBOff( timerCon );
  
  return;
}