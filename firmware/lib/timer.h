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
#define CORE_TIMER_PERIOD       FCY

// function prototypes
void initCoreTimer( void );
void resetCoreTimer( void );
unsigned int getCoreTimer( void );

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */