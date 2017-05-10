/* 
 * File:        adc.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */


#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// number of ADC channels on board
#define ADC_CHANNELS    16

// bit resolution of ADC
#define ADC_BITS        10
    
// ADC reference voltage
#define ADC_REF_V       3.3f

// function prototypes
void initAdc( void );
void adcInitCh(
        volatile unsigned int *, 
        unsigned int,
        volatile unsigned int *,
        unsigned int );
volatile unsigned int * adcRead( unsigned char );
void adcAutoOn( uint16_t, uint16_t, uint8_t, uint8_t );
void adcAutoOff( void );
float adcRegToFloat( unsigned int );

// static function prototypes

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */