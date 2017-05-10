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

#include <stdint.h>

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

uint32_t getCoreTimer( void ) {
    
  uint32_t timer;
  
  // get the current count from the register
  asm volatile( "mfc0   %0, $9" : "=r"(timer) );
  
  return timer;
}

void initTimerB(
        volatile unsigned int * con,
        volatile unsigned int * period,
        volatile unsigned int * reg,
        unsigned char tGate,
        unsigned int prescaler,
        unsigned char bits,
        char clkSrc ) {

  // zero out register to begin configuration
  *con = 0x0000;
  
  // ON: Timer On bit
  // keep timer off during configuration
  *con |= 0b0<<15;
  
  // TGATE: Timer Gated Time Accumulation Enable bit
  if ( tGate ) {
    // gated time accumulation is enabled
    *con |= 0b0<<7;
  } else {
    // gated time accumulation is disabled
    *con |= 0b0<<7;
  }
  
  // TCKPS<2:0>: Timer Input Clock Prescale Select bits
  timerBSetPrescaler( con, prescaler );
  
  // T32: 32-bit Timer Mode Select bit
  if ( bits == 16 ) {
    // TMRx and TMRy form separate 16-bit timer
    *con |= 0b0<<3;
  } else if ( bits == 32 ) {
    // TMRx and TMRy form a single 32-bit timer
    *con |= 0b1<<3;
  }
  
  // TCS: Timer Clock Source Select bit
  if ( clkSrc == 'i' ) {
    // use Internal peripheral clock
    *con |= 0b0<<1;
  } else if ( clkSrc == 'e' ) {
    // use external clock from TxCK pin
    *con |= 0b1<<1;
  }
  
  // clear the timer register TMRx
  *reg = 0x0000;

  // load the period register PRx with the desired match value
  *period = 0xFFFF;
  
  return;
}

unsigned char timerBSetPrescaler( volatile unsigned int * con, unsigned int prescaler ) {
  
  // first clear out prescale bits
  *con &= ~(0b111<<4);
  
  // TCKPS<2:0>: Timer Input Clock Prescale Select bits
  switch ( prescaler ) {
    case 256:
      *con |= 0b111<<4;                 // 1:256 prescale value
      break;
    case 64:
      *con |= 0b110<<4;                 // 1:64 prescale value
      break;
    case 32:
      *con |= 0b101<<4;                 // 1:32 prescale value
      break;
    case 16:
      *con |= 0b100<<4;                 // 1:16 prescale value
      break;
    case 8:
      *con |= 0b011<<4;                 // 1:8 prescale value
      break;
    case 4:
      *con |= 0b010<<4;                 // 1:4 prescale value
      break;
    case 2:
      *con |= 0b001<<4;                 // 1:2 prescale value
      break;
    case 1:
      *con |= 0b000<<4;                 // 1:1 prescale value
      break;
    default:
      return 0x0;
  }
  
  return 0x1;
}


unsigned int timerBGetPrescaler( volatile unsigned int * con ) {
  
  // TCKPS<2:0>: Timer Input Clock Prescale Select bits
  switch ( *con & (0b111<<4) ) {
    case 0b111<<4:                      // 1:256 prescale value
      return 256;
      break;
    case 0b110<<4:                      // 1:64 prescale value
      return 64;
      break;
    case 0b101<<4:                      // 1:32 prescale value
      return 32;
      break;
    case 0b100<<4:                      // 1:16 prescale value
      return 16;
      break;
    case 0b011<<4:                      // 1:8 prescale value
      return 8;
      break;
    case 0b010<<4:                      // 1:4 prescale value
      return 4;
      break;
    case 0b001<<4:                      // 1:2 prescale value
      return 2;
      break;
    case 0b000<<4:                      // 1:1 prescale value
      return 1;
      break;
    default:
      return 0x0;
  }
  
  return 0x0;
}

void timerBOn( volatile unsigned int * con ) {
  
  // start timer
  *con |= 0b1<<15;
  
  return;
}

void timerBOff( volatile unsigned int * con ) {
  
  // stop timer
  *con &= ~(0b1<<15);
  
  return;
}

void timerBReset( volatile unsigned int * reg ) {
  
  // clear the timer register TMRx
  *reg = 0x0000;
  
  return;
}

unsigned char timerBSetPeriodNs(
        volatile unsigned int * con,
        volatile unsigned int * period,
        unsigned long nSec ) {
  
  float nsFullRegNoPrescaler;
  float nsTickWithPrescaler;
  unsigned int prescaler;
  unsigned long bits;
  
  // first figure out if our timer is operating in 16- or 32-bit mode
  if ( *con & 0b1<<3 )
    bits = 0xFFFFFFFF;
  else
    bits = 0xFFFF;
  
  // using knowledge of the clock source and timer register size calculate the
  // time required to fill the timer's register (without considering any 
  // prescaler)
  nsFullRegNoPrescaler = 1000000000 / ((float)(FPB)) * ((float)(bits));
  
  // determine the best prescaler value to use for the given rate, for accurate
  // timing processes, this means to use the smallest prescaler possible that
  // still allows us to count up to our desired rate
  if ( ( nsFullRegNoPrescaler * 1 ) >= nSec ) {
    prescaler = 1;
  } else if ( ( nsFullRegNoPrescaler * 2 ) >= nSec ) {
    prescaler = 2;
  } else if ( ( nsFullRegNoPrescaler * 4 ) >= nSec ) {
    prescaler = 4;
  } else if ( ( nsFullRegNoPrescaler * 8 ) >= nSec ) {
    prescaler = 8;
  } else if ( ( nsFullRegNoPrescaler * 16 ) >= nSec ) {
    prescaler = 16;
  } else if ( ( nsFullRegNoPrescaler * 32 ) >= nSec ) {
    prescaler =32;
  } else if ( ( nsFullRegNoPrescaler * 64 ) >= nSec ) {
    prescaler = 64;
  } else {
    prescaler = 256;
  }
  
  // now that we have our prescaler chosen, set it and calculate our new tick
  // time (register count increments by one every this time interval)
  timerBSetPrescaler( con, prescaler );
  nsTickWithPrescaler = 1000000000 / ((float)(FPB)) * ((float)(prescaler));
  
  // if the desired timing interval is to long to achieve with the current bits
  // of this timer and the current prescaler, set our period to the maximum
  // allowed and exit with an error code
  if ( nSec > ( nsTickWithPrescaler * ((float)(bits)) ) ) {
    *period = bits;
    return 0x0;
  }
  
  // if we made it here, the desired timing interval is not too large, proceed
  // to set the PRx register to the closest value
  *period = (unsigned int)(((float)(nSec))/nsTickWithPrescaler);
  
  return 0x1;
}