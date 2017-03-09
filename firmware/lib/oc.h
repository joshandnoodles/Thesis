/* 
 * File:        oc.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef OC_H
#define	OC_H

#ifdef	__cplusplus
extern "C" {
#endif

// function prototypes
void initOutputCompare( volatile unsigned *, unsigned char, unsigned char );
void initPwm(
        volatile unsigned *,
        volatile unsigned *,
        volatile unsigned *,
        unsigned long,
        unsigned long,
        unsigned char,
        volatile unsigned *,
        volatile unsigned *,
        volatile unsigned *,
        unsigned char );
void pwmUpdatePulseWidthNs( volatile unsigned int *, unsigned int, unsigned long );
void pwmOn( volatile unsigned int *, volatile unsigned int *, volatile unsigned int * );
void pwmOff( volatile unsigned int *, volatile unsigned int * );

// static function prototypes

#ifdef	__cplusplus
}
#endif

#endif	/* OC_H */

