/* 
 * File:        timer.c
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

void initCoreTimer( void ) {
  
  unsigned int period = (unsigned int)(FCY);
  
  // clear the count from the register
  resetCoreTimer();
  
  // set up the period in the compare register
  asm volatile( "mtc0   %0,$11" : "+r"( period ) );
  
  return;
}

void resetCoreTimer( void ) {
  
  // clear the count from the register
  asm volatile( "mtc0   $0,$9" );
  
  return;
}

unsigned int getCoreTimer( void ) {
    
    unsigned int timer;

    // get the current count from the register
    asm volatile( "mfc0   %0, $9" : "=r"(timer) );

    return timer;
}
