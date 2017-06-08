/* 
 * File:        debug.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  Functions not relevant to core operation, only to debugging code base.
 */

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// specify time to wait to after button switch off to avoid debounce issues
#define BTN_DEBOUNCE_US     10000

// variables available to external modules
volatile uint32_t debugVal1;
volatile uint32_t debugVal2;
volatile uint32_t debugVal3;
volatile uint32_t debugVal4;

// function prototypes
void initDebug( void );
inline void debugLed1Off( void );
inline void debugLed1On( void );
inline void debugLed1Tog( void );
inline void debugLed2On( void );
inline void debugLed2Off( void );
inline void debugLed2Tog( void );
inline unsigned int debugBtn1State( void );
inline unsigned int debugBtn2State( void );

// static function prototypes
inline static void initLed( volatile unsigned int *, volatile unsigned int *, unsigned int );
inline static void ledOff( volatile unsigned int *, unsigned int );
inline static void ledOn( volatile unsigned int *, unsigned int );
inline static void ledTog( volatile unsigned int *, unsigned int );
inline static void initBtn( volatile unsigned int *, unsigned int );
inline static unsigned int btnState( volatile unsigned int *, unsigned int );

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */