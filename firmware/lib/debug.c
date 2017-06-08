/* 
 * File:        debug.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  Functions not relevant to core operation, only to debugging code base.
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include <stdint.h>

#include "debug.h"

// variables to be used for misc debugging purposes
volatile uint32_t * debugValPtrs[4];
volatile uint32_t debugVal1;
volatile uint32_t debugVal2;
volatile uint32_t debugVal3;
volatile uint32_t debugVal4;

void initDebug( void ) {
  
  uint16_t idx;
  
  // initialize any LEDs we have
  initLed( &DEBUG_LED1_TRIS, &DEBUG_LED1_LAT, DEBUG_LED1_MASK );
  initLed( &DEBUG_LED2_TRIS, &DEBUG_LED2_LAT, DEBUG_LED2_MASK );
  
  // initialize any buttons we have
  initBtn( &DEBUG_BTN1_TRIS, DEBUG_BTN1_MASK );
  initBtn( &DEBUG_BTN2_TRIS, DEBUG_BTN2_MASK );
  
  // build up array of pointers containing debug values
  debugValPtrs[0] = &debugVal1;
  debugValPtrs[1] = &debugVal2;
  debugValPtrs[2] = &debugVal3;
  debugValPtrs[3] = &debugVal4;
  
  // zero out debug values for initialization
  for ( idx=0; idx<4; idx++ )
    *debugValPtrs[idx] = 0;
  
  return;
}

inline void debugLed1Off( void ) {
  ledOff( &DEBUG_LED1_LAT, DEBUG_LED1_MASK );
  return;
}

inline void debugLed1On( void ) {
  ledOn( &DEBUG_LED1_LAT, DEBUG_LED1_MASK );
  return;
}

inline void debugLed1Tog( void ) {
  ledTog( &DEBUG_LED1_LAT, DEBUG_LED1_MASK );
  return;
}

inline void debugLed2Off( void ) {
  ledOff( &DEBUG_LED2_LAT, DEBUG_LED2_MASK );
  return;
}

inline void debugLed2On( void ) {
  ledOn( &DEBUG_LED2_LAT, DEBUG_LED2_MASK );
  return;
}

inline void debugLed2Tog( void ) {
  ledTog( &DEBUG_LED2_LAT, DEBUG_LED2_MASK );
  return;
}

inline static void initLed( volatile unsigned int * tris, volatile unsigned int * lat, unsigned int mask ) {
  *tris &= ~mask;
  *lat &= ~mask;
  return;
}

inline static void ledOff( volatile unsigned int * lat, unsigned int mask ) {
  *lat &= ~mask;
  return;
}

inline static void ledOn( volatile unsigned int * lat, unsigned int mask ) {
  *lat |= mask;
  return;
}

inline static void ledTog( volatile unsigned int * lat, unsigned int mask ) {
  *lat ^= mask;
  return;
}

inline unsigned int debugBtn1State( void ) {
  return btnState( &DEBUG_BTN1_PORT, DEBUG_BTN1_MASK );
}

inline unsigned int debugBtn2State( void ) {
  return btnState( &DEBUG_BTN2_PORT, DEBUG_BTN2_MASK );
}

inline static void initBtn( volatile unsigned int * tris, unsigned int mask ) {
  *tris |= mask;
  return;
}

inline static unsigned int btnState( volatile unsigned int * port, unsigned int mask ) {
  return !( *port & mask );
}