/* 
 * File:        timer.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// define core timer configuration
#define CORE_TIMER_PERIOD       (FCY/1)

// function prototypes
void initCoreTimer( void );
void resetCoreTimer( void );
uint32_t getCoreTimer( void );
void initTimerB(
        volatile unsigned int *,
        volatile unsigned int *,
        volatile unsigned int *,
        unsigned char,
        unsigned int,
        unsigned char,
        char );
unsigned char timerBSetPrescaler( volatile unsigned int *, unsigned int );
unsigned int timerBGetPrescaler( volatile unsigned int * );
void timerBOn( volatile unsigned int * );
void timerBOff( volatile unsigned int * );
void timerBReset( volatile unsigned int * );
unsigned char timerBSetPeriodNs(
        volatile unsigned int *,
        volatile unsigned int *,
        unsigned long );

// static function prototypes

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */