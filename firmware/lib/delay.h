/* 
 * File:        delay.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */


#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

// function prototypes
void initDelay( void );
void delayUs( unsigned int );
void delayMs( unsigned int );
void delayS( unsigned int );

// static function prototypes

#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */