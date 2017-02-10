/* 
 * File:        mod.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include <sys/attribs.h>

#include "timer.h"
#include "interrupt.h"
#include "lsr.h"

#include "mod.h"

// variables to keep track of modulation rate
unsigned long modFreqHz;
unsigned long modPeriodNs;

// variables to keep track of state changes
unsigned char modState;

void initMod( void ) {
  
  // initialize the timing mechanism to be used for the interrupts
  initTimerB(
          &MOD_TIMER_CON,
          &MOD_TIMER_PERIOD,
          &MOD_TIMER_REG,
          0,
          256,
          16,
          'i' );

  // initialize the actual interrupts
  initInterrupt( 
      &MOD_TIMER_INT_ENB,
      &MOD_TIMER_INT_PRIO,
      &MOD_TIMER_INT_FLAG,
      MOD_TIMER_PRIO_OFFSET,
      MOD_TIMER_SUBPRIO_OFFSET,
      MOD_TIMER_INT_MASK );
  
  // set default modulation rate
  modSetFreqHz( DEFAULT_MOD_FREQ_HZ );
  
  return;
}

unsigned char modOn( void ) {
  
  // check modulation power supply source before trying to begin modulation
  if ( !lsrLoadSwitchState )
    return 0x0;
  
  // clear the timer register TMRx
  timerBReset( &MOD_TIMER_REG );
  
  // start calling interrupts
  timerBOn( &MOD_TIMER_CON );
  
  // change flag(s) to keep track of state changes
  modState = 0x1;
  
  return 0x1;
}


unsigned char modOff( void ) {
  
  // turn off the timer to stop any additionally interrupts from calling
  timerBOff( &MOD_TIMER_CON );
  
  // end modulation with laser in an off state
  lsrSetLow();
  
  // change flag(s) to keep track of state changes
  modState = 0x0;
  
  return 0x1;
}

unsigned char modTog( void ) {
  
  // look at modulation state and toggle appropriately
  if ( modState )
    return modOff();
  else
    return modOn();
  
}

unsigned char modSetFreqHz( unsigned long freqHz ) {
  
  // check if new modulation rate is within all specified limitations
  if ( ( freqHz < MOD_FREQ_HZ_MIN ) || ( freqHz > MOD_FREQ_HZ_MAX ) )
    return 0x0;
    
  // if we made it here, every checks out, go ahead and set it as new rate
  modFreqHz = freqHz;
  modPeriodNs = 1000000000 / modFreqHz;
    
  // set period of interrupt source for the current modulation frequency rate
  return timerBSetPeriodNs( &MOD_TIMER_CON, &MOD_TIMER_PERIOD, modPeriodNs );
}



/*
This code example demonstrates a simple interrupt service routine for Timer
interrupts. The user?s code at this ISR handler should perform any application
specific operations and must clear the corresponding Timer interrupt status flag
before exiting.
*/
void __ISR( _TIMER_2_VECTOR, IPL5 ) _TIMER2_HANDLER( void ) {

  //... perform application specific operations in response to the interrupt
  
  // toggle the modulation source
  LSR_EN_CH1_LAT ^= LSR_EN_CH1_MASK;
  
  // clear the TxIF interrupt flag bit
  IFS0CLR = 0b1<<8;
  
  return;
}