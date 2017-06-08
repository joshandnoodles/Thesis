/* 
 * File:        gimbal.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include <stdint.h>
#include "oc.h"
#include "timer.h"

#include "gimbal.h"

// variables to keep track of state changes
uint8_t gimbalPanState;
uint8_t gimbalTiltState;

// variables to keep calculated servo range values
uint16_t gimbalPanPulseMinReg;
static uint16_t gimbalPanPulseMaxReg;
static uint16_t gimbalPanPulseRngReg;
static uint16_t gimbalTiltPulseMinReg;
static uint16_t gimbalTiltPulseMaxReg;
static uint16_t gimbalTiltPulseRngReg;

void initGimbal( void ) { 
  
  // initialize pulse-width-modulation module (PWM) for the pan servo
  initPwm( 
          &SERVO_PAN_OCCON,
          &SERVO_PAN_OCR,
          &SERVO_PAN_OCRS,
          SERVO_PAN_PERIOD_NS,
          (SERVO_PAN_PULSE_MIN_NS+SERVO_PAN_PULSE_MAX_NS)/2,
          16,
          &SERVO_PAN_TIMER_CON,
          &SERVO_PAN_TIMER_PERIOD,
          &SERVO_PAN_TIMER_REG,
          SERVO_PAN_TIMER_IDX );
  
  // initialize pulse-width-modulation module (PWM) for the tilt servo
  initPwm( 
          &SERVO_TILT_OCCON,
          &SERVO_TILT_OCR,
          &SERVO_TILT_OCRS,
          SERVO_TILT_PERIOD_NS,
          SERVO_TILT_PULSE_MAX_NS,
          16,
          &SERVO_TILT_TIMER_CON,
          &SERVO_TILT_TIMER_PERIOD,
          &SERVO_TILT_TIMER_REG,
          SERVO_TILT_TIMER_IDX );
  
  // pre-calculate servo range values, we take the nanosecond values and find 
  // what their equivalent value is in the output compare register (this will
  // also depend on the output compare timer source and prescaler), we should
  // save these values so they can be used in internal functions for fast pulse 
  // width manipulation
  gimbalPanPulseMinReg = (uint16_t)( ((float)(SERVO_PAN_PULSE_MIN_NS/1000000000.0)) * ((float)(FPB)) / timerBGetPrescaler(&SERVO_PAN_TIMER_CON) );
  gimbalPanPulseMaxReg = (uint16_t)( ((float)(SERVO_PAN_PULSE_MAX_NS/1000000000.0)) * ((float)(FPB)) / timerBGetPrescaler(&SERVO_PAN_TIMER_CON) );
  gimbalPanPulseRngReg = gimbalPanPulseMaxReg - gimbalPanPulseMinReg;
  gimbalTiltPulseMinReg = (uint16_t)( ((float)(SERVO_TILT_PULSE_MIN_NS/1000000000.0)) * ((float)(FPB)) / timerBGetPrescaler(&SERVO_TILT_TIMER_CON) );
  gimbalTiltPulseMaxReg = (uint16_t)( ((float)(SERVO_TILT_PULSE_MAX_NS/1000000000.0)) * ((float)(FPB)) / timerBGetPrescaler(&SERVO_TILT_TIMER_CON) );
  gimbalTiltPulseRngReg = gimbalTiltPulseMaxReg - gimbalTiltPulseMinReg;
  
  return;
}

void gimbalOn( void ) {
  
  // call sub-functions to turn on all servos
  gimbalPanOn();
  gimbalTiltOn();
  
  return;
}

void gimbalOff( void ) {
  
  // call sub-functions to turn off all servos
  gimbalPanOff();
  gimbalTiltOff();
  
  return;
}

void gimbalPanOn( void ) {
  
  // turn on PWM for the servo
  pwmOn( &SERVO_PAN_OCCON, &SERVO_PAN_TIMER_CON, &SERVO_PAN_TIMER_REG );
  
  // change flag(s) to keep track of state changes
  gimbalPanState = 0x1;
  
  return;
}

void gimbalTiltOn( void ) {
  
  // turn on PWM for the servo
  pwmOn( &SERVO_TILT_OCCON, &SERVO_TILT_TIMER_CON, &SERVO_TILT_TIMER_REG );
  
  // change flag(s) to keep track of state changes
  gimbalTiltState = 0x1;
  
  return;
}

void gimbalPanOff( void ) {
  
  // turn on PWM for the servo
  pwmOff( &SERVO_PAN_OCCON, &SERVO_PAN_TIMER_CON );
  
  // change flag(s) to keep track of state changes
  gimbalPanState = 0x0;
  
  return;
}

void gimbalTiltOff( void ) {
  
  // turn on PWM for the servo
  pwmOff( &SERVO_TILT_OCCON, &SERVO_TILT_TIMER_CON );
  
  // change flag(s) to keep track of state changes
  gimbalTiltState = 0x0;
  
  return;
}

void gimbalPanTog( void ) {
  
  // look at servo state and toggle appropriately
  if ( gimbalPanState )
    return gimbalPanOff();
  else
    return gimbalPanOn();
  
  return;
}

void gimbalTiltTog( void ) {
  
  // look at servo state and toggle appropriately
  if ( gimbalTiltState )
    return gimbalTiltOff();
  else
    return gimbalTiltOn();
  
  return;
}

void gimbalSetPan( float angle ) {
  
  // update servo to new location based on information we know about servo
  gimbalServoSet( &SERVO_PAN_OCRS,
        gimbalPanPulseMinReg, gimbalPanPulseRngReg,
        SERVO_PAN_ANGLE_RNG, angle );
          
  return;
}

void gimbalSetTilt( float angle ) {
    
  // update servo to new location based on information we know about servo
  gimbalServoSet( &SERVO_TILT_OCRS,
        gimbalTiltPulseMinReg, gimbalTiltPulseRngReg,
        SERVO_TILT_ANGLE_RNG, angle );
  
  return;
}

float gimbalGetPan( void ) {
    
  // using what we know about the servo's characteristics, figure out what
  // angle the servo is currently set at
  float angle = gimbalServoGet( &SERVO_PAN_OCR,
          gimbalPanPulseMinReg, gimbalPanPulseRngReg,
          SERVO_PAN_ANGLE_RNG );
  
  return angle;
}

float gimbalGetTilt( void ) {
    
  // using what we know about the servo's characteristics, figure out what
  // angle the servo is currently set at
  float angle = gimbalServoGet( &SERVO_TILT_OCR,
          gimbalTiltPulseMinReg, gimbalTiltPulseRngReg,
          SERVO_TILT_ANGLE_RNG );
  
  return angle;
}

static void gimbalServoSet( volatile unsigned int * ocRs,
        uint16_t servoPulseMin, uint16_t servoPulseRng,
        uint16_t servoAngleRange, float angle ) {
          
  // update servo position based on data we are being told about specific servo
  // adding on 0.5 before integer typecasting will properly round
  *ocRs = (uint16_t)( (float)(angle/servoAngleRange) * servoPulseRng + servoPulseMin + 0.5 );  
  
  return;
}

static float gimbalServoGet( volatile unsigned int * ocR,
        uint16_t servoPulseMin, uint16_t servoPulseRng,
        uint16_t servoAngleRange ) {
  
  // reverse relation for setting servo to return decimal angle of servo position
  float angle = (float)( *ocR-servoPulseMin ) * servoAngleRange / servoPulseRng;
  
  return angle;
}