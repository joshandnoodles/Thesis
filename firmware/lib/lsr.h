/* 
 * File:        lsr.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  All core operations of laser diode and laser diode driver can be found here.
 *  For this project, the hardware specifications are the following:
 *    - PL 450B laser diode
 *    - IC-HKB laser driver
 *  Note: This file only contains functionality for initialization, power-up, 
 *  current limit setting, and basic switching operations. Any complex 
 *  modulation operations should be externalized in another file.
 */

#ifndef LSR_H
#define	LSR_H

#ifdef	__cplusplus
extern "C" {
#endif

// acceptable current limit Vref range
#define LSR_VREF_VSENSE_MIN     0.0f        // Vref minimum (in volts)
#define LSR_VREF_VSENSE_MAX     1.95f       // Vref maximum (in volts)

// specify resistance of current sense resistor in series with laser
#define LSR_ISENSE_RES          0.82f       // sense resistor (in ohms)

// acceptable operating current of diode (will trigger shutdown if outside range)
// for reference, different diode operating characteristics are listed below:
//  - PL 450B laser diode
//      - operating current (typical) = 100mA
//      - operating current (maximum) = 145mA
//      - absolute maximum current = 165mA
//  - C503B-BCS/BCN/GCS/GCN 5mm LED
//      - absolute maximum forward current = 30mA
//      - absolute maximum peak current = 100mA
    
#define LSR_ISENSE_MIN            0.0f      // minimum diode current (in mA)
#define LSR_ISENSE_MAX            45.0f    // maximum diode current (in mA)

// variables available to external modules
extern unsigned char lsrLoadSwitchState;
extern unsigned char lsrEnCh1State;  //!! currently unused
extern unsigned char lsrEnCh2State;  //!! currently unused
extern unsigned char lsrVrefVSenseAlarm;
extern unsigned char lsrISenseAlarm;
extern unsigned int lsrLastVrefVSenseReg;
extern unsigned int lsrLastISenseReg;

// function prototypes
void initLsr( void );
unsigned char lsrLoadSwitchOn( void );
unsigned char lsrLoadSwitchOff( void );
unsigned char lsrLoadSwitchTog( void );
float lsrReadVrefVSense( void );
unsigned int lsrReadVrefVSenseReg( void );
float lsrReadISense( void );
unsigned int lsrReadISenseReg( void );
inline void lsrSetHigh( void );
inline void lsrSetLow( void );
inline void lsrTog( void );
void lsrCheckAlarms( void );
    
// static function prototypes

#ifdef	__cplusplus
}
#endif

#endif	/* LSR_H */