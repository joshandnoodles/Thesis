/* 
 * File:        delay.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include "timer.h"

#include "delay.h"

unsigned int FCYuHz;
unsigned int FCYmHz;
unsigned int tick;

void initDelay( void ) {
  
  // not much initialization needed here since we are banking on using the
  // PIC32's core timer
  
  // pre-calculate variables delay component variables
  FCYuHz = (FCY/1000000);
  FCYmHz = (FCY/1000);
  
  return;
}

void delayUs( unsigned int uSec ) {
    
  // get last timer tick
  tick = getCoreTimer();
    

  // calculate cycle time we need (with as small as possible overhead)
  unsigned int tock = tick + FCYuHz*uSec;
  
  // wait here
  while ( getCoreTimer() < tock );
  
  return;
}

void delayMs( unsigned int mSec) {
  
  // get last timer tick
  tick = getCoreTimer();
    
  // calculate cycle time we need (with as small as possible overhead)
  unsigned int counts = FCYmHz*mSec;
  
  // wait here
  while ( ( getCoreTimer() - tick ) < counts );
  
  return;
}

void delayS( unsigned int sec ) {
  
  while ( sec-- )
    delayMs( 1000 );
  
  return;
}