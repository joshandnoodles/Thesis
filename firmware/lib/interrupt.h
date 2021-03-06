/* 
 * File:        interrupt.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif

// function prototypes
void initInterrupt(
        volatile unsigned int *,
        volatile unsigned int *,
        unsigned int,
        unsigned char,
        unsigned int,
        unsigned char,
        unsigned );
void interruptOn( volatile unsigned int *, unsigned int );
void interruptOff( volatile unsigned int *, volatile unsigned int *, 
        unsigned int );

#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPT_H */