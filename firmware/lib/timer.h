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

// define core timer configuration
#define CORE_TIMER_PERIOD       (FCY/1)

// function prototypes
void initCoreTimer( void );
void resetCoreTimer( void );
unsigned int getCoreTimer( void );
void initTimerB(
        volatile unsigned int *,
        volatile unsigned int *,
        volatile unsigned int *,
        unsigned char,
        unsigned int,
        unsigned char,
        char );
unsigned char timerBSetPrescaler( volatile unsigned int *, unsigned int );
void timerBOn( volatile unsigned int * );
void timerBOff( volatile unsigned int * );
void timerBReset( volatile unsigned int * );
unsigned char timerBSetPeriodNs(
        volatile unsigned int *,
        volatile unsigned int *,
        unsigned long );

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */