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

#include <stdint.h>

// define default values for initial configuration
#define DEFAULT_MOD_FREQ_HZ            3

// define any limits imposed on modulation
#define MOD_FREQ_HZ_MIN                3
#define MOD_FREQ_HZ_MAX                100000000

// variables available to external modules
extern uint8_t modState;
extern uint32_t modFreqHz;

// function prototypes
void initMod( void );
uint8_t modOn( void );
uint8_t modOff( void );
uint8_t modTog( void );
uint8_t modSetFreqHz( uint32_t );

#ifdef	__cplusplus
}
#endif

#endif	/* MOD_H */