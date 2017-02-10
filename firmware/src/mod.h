/* 
 * File:        mod.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef MOD_H
#define	MOD_H

#ifdef	__cplusplus
extern "C" {
#endif

// define default values for initial configuration
#define DEFAULT_MOD_FREQ_HZ            3

// define any limits imposed on modulation
#define MOD_FREQ_HZ_MIN                3
#define MOD_FREQ_HZ_MAX                100000000

// variables available to external modules
extern unsigned char modState;
extern unsigned long modFreqHz;

// function prototypes
void initMod( void );
unsigned char modOn( void );
unsigned char modOff( void );
unsigned char modTog( void );
unsigned char modSetFreqHz( unsigned long );

#ifdef	__cplusplus
}
#endif

#endif	/* MOD_H */