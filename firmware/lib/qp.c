/* 
 * File:        qp.c
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
#include <string.h>
#include "adc.h"
#include "delay.h"

#include "qp.h"

// variables to keep track of last values retrieved
uint16_t qpLastCh1VSenseReg;
uint16_t qpLastCh2VSenseReg;
uint16_t qpLastCh3VSenseReg;
uint16_t qpLastCh4VSenseReg;
uint16_t qpBulkBuffer[QP_BULK_BUFFER_SIZE];
uint32_t qpBulkBufferIdx;

// variables to keep track of positional alignment
int16_t qpAlignmentXAxis;
int16_t qpAlignmentYAxis;

void initQp( void ) {
  
  // configure ADC channel for quadrant photodiode channel 1 measurement
  adcInitCh( &QP_CH1_VSENSE_AN_BUF,
          QP_CH1_VSENSE_AN_MASK,
          &QP_CH1_VSENSE_TRIS,
          QP_CH1_VSENSE_MASK );
  
  // configure ADC channel for quadrant photodiode channel 2 measurement
  adcInitCh( &QP_CH2_VSENSE_AN_BUF,
          QP_CH2_VSENSE_AN_MASK,
          &QP_CH2_VSENSE_TRIS,
          QP_CH2_VSENSE_MASK );
  
  // configure ADC channel for quadrant photodiode channel 3 measurement
  adcInitCh( &QP_CH3_VSENSE_AN_BUF,
          QP_CH3_VSENSE_AN_MASK,
          &QP_CH3_VSENSE_TRIS,
          QP_CH3_VSENSE_MASK );
  
  // configure ADC channel for quadrant photodiode channel 4 measurement
  adcInitCh( &QP_CH4_VSENSE_AN_BUF,
          QP_CH4_VSENSE_AN_MASK,
          &QP_CH4_VSENSE_TRIS,
          QP_CH4_VSENSE_MASK );
  
  return;
}

float qpReadCh1VSense( void ) {
  
  // call on appropriate function that samples, converts, and returns the ADC 
  // buffer result register and send that register reference to decimal
  // conversion function
  return adcRegToFloat( qpReadCh1VSenseReg() );
}

uint16_t qpReadCh1VSenseReg( void ) {
  
  // use modular ADC read function to sample, convert, and return ADC buffer 
  // result, this assumes the particular channel being read has already been
  // initialized through the ADC channel initialization function
  qpLastCh1VSenseReg = (uint16_t)(*(adcRead( QP_CH1_VSENSE_AN_CH )));
  
  
  return qpLastCh1VSenseReg;
}

float qpReadCh2VSense( void ) {
  
  // call on appropriate function that samples, converts, and returns the ADC 
  // buffer result register and send that register reference to decimal
  // conversion function
  return adcRegToFloat( qpReadCh2VSenseReg() );
}

uint16_t qpReadCh2VSenseReg( void ) {

  // use modular ADC read function to sample, convert, and return ADC buffer 
  // result, this assumes the particular channel being read has already been
  // initialized through the ADC channel initialization function
  qpLastCh2VSenseReg = (uint16_t)(*(adcRead( QP_CH2_VSENSE_AN_CH )));
  
  return qpLastCh2VSenseReg;
}

float qpReadCh3VSense( void ) {
  
  // call on appropriate function that samples, converts, and returns the ADC 
  // buffer result register and send that register reference to decimal
  // conversion function
  return adcRegToFloat( qpReadCh3VSenseReg() );
}

uint16_t qpReadCh3VSenseReg( void ) {

  // use modular ADC read function to sample, convert, and return ADC buffer 
  // result, this assumes the particular channel being read has already been
  // initialized through the ADC channel initialization function
  qpLastCh3VSenseReg = (uint16_t)(*(adcRead( QP_CH3_VSENSE_AN_CH )));
  
  return qpLastCh3VSenseReg;
}

float qpReadCh4VSense( void ) {
  
  // call on appropriate function that samples, converts, and returns the ADC 
  // buffer result register and send that register reference to decimal
  // conversion function
  return adcRegToFloat( qpReadCh4VSenseReg() );
}

uint16_t qpReadCh4VSenseReg( void ) {

  // use modular ADC read function to sample, convert, and return ADC buffer 
  // result, this assumes the particular channel being read has already been
  // initialized through the ADC channel initialization function
  qpLastCh4VSenseReg = (uint16_t)(*(adcRead( QP_CH4_VSENSE_AN_CH )));
  
  return qpLastCh4VSenseReg;
}

char qpAlign( void ) {
  
  // this function assumes the channels on the quadrant photodiode are laid out
  // in the following configuration:
  //  Ch1 Ch4
  //  Ch2 Ch3
  
  // calculate alignment offsets for both axes
  qpAlignmentXAxis = -qpLastCh1VSenseReg - qpLastCh2VSenseReg + 
          qpLastCh3VSenseReg + qpLastCh4VSenseReg;
  qpAlignmentYAxis = -qpLastCh2VSenseReg - qpLastCh3VSenseReg +
          qpLastCh1VSenseReg + qpLastCh4VSenseReg;
  
  // fix any positional alignment in the x-direction
  if ( qpAlignmentXAxis < 0 ) {
    //gimbalRight();
  } else if ( qpAlignmentXAxis > 0 ) {
    //gimbalLeft();
  }
  
  // fix any positional alignment in the y-direction
  if ( qpAlignmentYAxis < 0 ) {
    //gimbalUp();
  } else if ( qpAlignmentYAxis > 0 ) {
    //gimbalDown();
  }
  
  return;
}

uint16_t * qpReadAllBulk( uint32_t pauseUs ) {
  
  uint32_t idx = 0;
  
  // ready buffer that will contain ADC values
  //qpBulkBuffer = (uint16_t *)malloc( 2 * (4*reads) );
  memset( &qpBulkBuffer[0], 0, 2 * QP_BULK_BUFFER_SIZE );
  
  // reset counter index of buffer
  qpBulkBufferIdx = 0;
  
  while ( idx<(QP_BULK_BUFFER_SIZE) ) {
    
    // get ADC reading for each channel and add to buffer
    // each takes ~2.5usec at speed since we are not auto sampling
    qpBulkBuffer[idx++] = qpReadCh1VSenseReg();
    qpBulkBuffer[idx++] = qpReadCh2VSenseReg();
    qpBulkBuffer[idx++] = qpReadCh3VSenseReg();
    qpBulkBuffer[idx++] = qpReadCh4VSenseReg();
    
    // pause a number of microseconds before reading again
    if ( pauseUs )
      delayUs( pauseUs );
  }
  
  return qpBulkBuffer;
}