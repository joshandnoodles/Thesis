/* 
 * File:        gimbal.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef GIMBAL_H
#define	GIMBAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// variables available to external modules
extern uint8_t gimbalPanState;
extern uint8_t gimbalTiltState;

// function prototypes
void initGimbal( void );
void gimbalPanOn( void );
void gimbalTiltOn( void );
void gimbalPanOff( void );
void gimbalTiltOff( void );
void gimbalPanTog( void );
void gimbalTiltTog( void );
void gimbalSetPan( float );
void gimbalSetTilt( float );
float gimbalGetPan( void );
float gimbalGetTilt( void );

// static function prototypes
static void gimbalServoSet( volatile unsigned int *,
        uint16_t, uint16_t,
        uint16_t, float );
static float gimbalServoGet( volatile unsigned int *,
        uint16_t, uint16_t,
        uint16_t );

#ifdef	__cplusplus
}
#endif

#endif	/* GIMBAL_H */

    
    