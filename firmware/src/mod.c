/* 
 * File:        mod.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/attribs.h>
#include "delay.h"
#include "timer.h"
#include "interrupt.h"
#include "lsr.h"
#include "qp.h"
#include "test_data.h"

#include "mod.h"

// variables to keep track of modulation rate
uint32_t modTxFreqHz;
uint32_t modTxPeriodNs;


// variables to keep track of ongoing tx communication strategies
volatile uint8_t modTxHeaderBuffer[MOD_FRAME_HEADER_SIZE_BYTES];
volatile uint8_t modTxDataBuffer[MOD_FRAME_DATA_SIZE_BYTES];
uint8_t modTxBufferBitMask;
volatile uint16_t modTxHeaderBufferIdx;
volatile uint16_t modTxDataBufferIdx;
uint8_t modTxBufferEighthOfNibbleFlag;

// variables to keep track of ongoing rx acquisition strategies
uint8_t modRxActiveQuadrant;
uint16_t modRxHiccupThres;
volatile uint8_t modRxHeaderBuffer[MOD_FRAME_HEADER_SIZE_BYTES];
volatile uint8_t modRxDataBuffer[MOD_FRAME_DATA_SIZE_BYTES];
volatile uint16_t modRxADCBuffer[MOD_FRAME_DATA_SIZE_BYTES];
volatile uint16_t modRxHeaderBufferIdx;
volatile uint16_t modRxDataBufferIdx;
volatile uint8_t modRxADCBufferIdx;
uint8_t modRxNibbleFlag;
uint32_t modRxHiccupFlipMask;   // used to invert between ADC timing states
//uint32_t modRxHiccupTicks;       // differential in sampling states (in FOSC/2's)
uint32_t modRxBitErrors;
uint8_t lastMsb;
uint8_t lastLsb;

// variables to keep track of state changes
uint8_t modState;
uint8_t modSigLockState;
uint8_t modRxHiccupState;

void initMod( void ) {
  
  uint16_t idx;
  
  //!! debugging
  TRISE &= ~(1<<6);
  
  // initialize the timing mechanism to be used for the tx interrupts
  initTimerB(
          &MOD_TIMER_TX_CON,
          &MOD_TIMER_TX_PERIOD,
          &MOD_TIMER_TX_REG,
          0,
          1,
          16,
          'i' );
  
  // initialize the interrupts for tx operation
  initInterrupt(
          &MOD_TIMER_TX_INT_PRIO,
          &MOD_TIMER_TX_INT_FLAG,
          MOD_TIMER_TX_PRIO_OFFSET,
          7,
          MOD_TIMER_TX_SUBPRIO_OFFSET,
          1,
          MOD_TIMER_TX_INT_MASK );
  
  // initialize the timing mechanism to be used for the rx interrupts
  initTimerB(
          &MOD_TIMER_RX_CON,
          &MOD_TIMER_RX_PERIOD,
          &MOD_TIMER_RX_REG,
          0,
          1,
          16,
          'i' );
  
  // initialize the interrupts for rx operation
  initInterrupt(
          &MOD_TIMER_RX_INT_PRIO,
          &MOD_TIMER_RX_INT_FLAG,
          MOD_TIMER_RX_PRIO_OFFSET,
          7,
          MOD_TIMER_RX_SUBPRIO_OFFSET,
          0,
          MOD_TIMER_RX_INT_MASK );
  initInterrupt(
          &MOD_ADC_INT_PRIO,
          &MOD_ADC_INT_FLAG,
          MOD_ADC_PRIO_OFFSET,
          6,
          MOD_ADC_SUBPRIO_OFFSET,
          0,
          MOD_ADC_INT_MASK );
  
  // set default modulation rate
  modSetFreqHz( MOD_DEFAULT_TX_FREQ_HZ );
  
  // set default active quadrant
  modRxActiveQuadrant = MOD_DEFAULT_RX_ACTIVE_QUAD;
  
  // set default threshold values for both hiccup ADC nodes
  modRxHiccupThres = MOD_DEFAULT_RX_HICCUP_THRES;
  
  // reset pointers for bulk array buffers (and any obvious masks/flags)
  modTxHeaderBufferIdx = 0x00;
  modTxDataBufferIdx = 0x00;
  modTxBufferBitMask = 0x80;
  modTxBufferEighthOfNibbleFlag = 0x00;
  modRxNibbleFlag = 0x00;
  modSigLockState = 0x00;
  modRxHiccupState = 0x00;
  modRxBitErrors = 0x00000000;
  
  // pre-calculate the number of core timer ticks to wait when we hiccup
  // core timer runs at the main system clock / 2 (not the peripheral clock)
  //modRxHiccupTicks = (MOD_RX_HICCUP_NS / 1000000000.0) * (FOSC / 2);
  
  // set period of interrupt source to correct rx hiccup timing value
  timerBSetPeriodNs( &MOD_TIMER_RX_CON, &MOD_TIMER_RX_PERIOD, 
          MOD_RX_HICCUP_NS );
  
  // fill tx buffer with random values for testing purposes
  for ( idx=0; idx<(sizeof(modTxDataBuffer)/sizeof(modTxDataBuffer[0])); idx++ )
    modTxDataBuffer[idx] = testDataRand256BytesWithReplacement[idx];
  
  return;
}

uint8_t modOn( void ) {
  
  uint16_t qpCh1Reg,
           qpCh2Reg,
           qpCh3Reg,
           qpCh4Reg;
  
  // check modulation power supply source before trying to begin modulation
  if ( !lsrLoadSwitchState )
    return 0x0;
  
  // reset pointers for tx/rx buffer(s)
  modTxDataBufferIdx = 0x00;
  modTxHeaderBufferIdx = 0x00;
  modRxDataBufferIdx = 0x00;
  modRxHeaderBufferIdx = MOD_FRAME_HEADER_SIZE_BYTES;
  modRxADCBufferIdx = 0x00;

  
  // dynamically set active quadrant by checking each quadrant's value manually
  /* !! qpCh1Reg = qpReadCh1VSenseReg();
  qpCh2Reg = qpReadCh2VSenseReg();
  qpCh3Reg = qpReadCh3VSenseReg();
  qpCh4Reg = qpReadCh4VSenseReg();
  if ( ( qpCh1Reg > qpCh2Reg ) && 
          ( qpCh1Reg > qpCh3Reg ) && 
          ( qpCh1Reg > qpCh4Reg )  ) {
    modRxActiveQuadrant = 1;
  } else if ( ( qpCh2Reg > qpCh3Reg ) && 
          ( qpCh2Reg > qpCh4Reg ) ) {
    modRxActiveQuadrant = 2;
  } else if ( qpCh3Reg > qpCh4Reg ) {
    modRxActiveQuadrant = 3;
  } else {
    modRxActiveQuadrant = 4;
  }*/
  
  // clear the timer register TMRx
  timerBReset( &MOD_TIMER_TX_REG );
  
  // start calling interrupts for tx operation
  //timerBOn( &MOD_TIMER_TX_CON );
  //interruptOn( &MOD_TIMER_TX_INT_ENB, MOD_TIMER_TX_INT_MASK );
  
  // clear the timer register TMRx
  timerBReset( &MOD_TIMER_RX_REG );
  
  // start calling interrupts for rx operation (part 1)
  //adcAutoOn(
  //        QP_CH1_VSENSE_AN_MASK | QP_CH2_VSENSE_AN_MASK | 
  //        QP_CH3_VSENSE_AN_MASK | QP_CH4_VSENSE_AN_MASK, 
  //        QP_CH2_VSENSE_AN_MASK, 8, 0x01 );
 adcAutoOn( QP_CH2_VSENSE_AN_MASK, 0x0000, 8, 0x01 );
  
  // set hiccup mask so it can be used to quickly toggle between two different
  // sampling timing values
  modRxHiccupFlipMask = AD1CON3 & (0b11111<<8);
  if ( MOD_RX_HICCUP_DIRECTION )
    modRxHiccupFlipMask = (modRxHiccupFlipMask + (MOD_RX_HICCUP_TADS<<8)) ^ 
            modRxHiccupFlipMask;
  else
    modRxHiccupFlipMask = (modRxHiccupFlipMask - (MOD_RX_HICCUP_TADS<<8)) ^ 
            modRxHiccupFlipMask;
  
  // start calling interrupts for rx operation (part 2)
  interruptOn( &MOD_TIMER_RX_INT_ENB, MOD_TIMER_RX_INT_MASK );
  interruptOn( &MOD_ADC_INT_ENB, MOD_ADC_INT_MASK );  
  
  // change flag(s) to keep track of state changes
  modState = 0x1;
  
  return 0x1;
}

uint8_t modOff( void ) {
  
  // stop calling interrupts for tx operation
  interruptOff( &MOD_TIMER_TX_INT_ENB, MOD_TIMER_TX_INT_MASK );
  timerBOff( &MOD_TIMER_TX_CON );
  
  // stop calling interrupts for rx operation
  interruptOff( &MOD_ADC_INT_ENB, MOD_ADC_INT_MASK );
  adcAutoOff();
  interruptOff( &MOD_TIMER_RX_INT_ENB, MOD_TIMER_RX_INT_MASK );
  timerBOff( &MOD_TIMER_RX_CON );
  
  // end modulation with laser in an off state
  lsrSetLow();
  
  // end hiccup state in the off position so our timing doesn't get all screwy
  // flip necessary ADC configuration bits to switch sampling timing state
  if (modRxHiccupState)
    AD1CON3 ^= modRxHiccupFlipMask;

  // change flag(s) to keep track of state changes
  modState = 0x00;
  modSigLockState = 0x00;
  modRxHiccupState = 0x00;
  
  return 0x1;
}

uint8_t modTog( void ) {
  
  // look at modulation state and toggle appropriately
  if ( modState )
    return modOff();
  else
    return modOn();
  
}

uint8_t modSetFreqHz( uint32_t freqHz ) {
  
  // check if new modulation rate is within all specified limitations
  if ( ( freqHz < MOD_TX_FREQ_HZ_MIN ) || ( freqHz > MOD_TX_FREQ_HZ_MAX ) )
    return 0x0;
    
  // if we made it here, every checks out, go ahead and set it as new rate
  modTxFreqHz = freqHz;
  modTxPeriodNs = 1000000000 / modTxFreqHz;
    
  // set period of interrupt source for the current modulation frequency rate
  return timerBSetPeriodNs( &MOD_TIMER_TX_CON, &MOD_TIMER_TX_PERIOD,
          modTxPeriodNs );
}

void __ISR( _TIMER_4_VECTOR, IPL7AUTO ) _TIMER4_HANDLER( void ) {
  
  uint8_t txBit;
  volatile uint8_t * buffer;
  volatile uint16_t * bufferIdx;
  volatile uint16_t * otherBufferIdx;
  uint16_t bufferSizeBytes;
  
  if ( modSigLockState ) {
    // determine whether we are sending the preamble or data chunk, when either
    // index is at its max that is a sign the associated data has already been 
    // sent
    if ( modTxHeaderBufferIdx == MOD_FRAME_HEADER_SIZE_BYTES ) {
      // preamble has been sent, we should be sending the data
      buffer = modTxDataBuffer;
      bufferIdx = &modTxDataBufferIdx;
      bufferSizeBytes = MOD_FRAME_DATA_SIZE_BYTES;
      otherBufferIdx = &modTxHeaderBufferIdx;
    } else {
      // data has been sent, we should be sending the preamble
      buffer = modTxHeaderBuffer;
      bufferIdx = &modTxHeaderBufferIdx;
      bufferSizeBytes = MOD_FRAME_HEADER_SIZE_BYTES;
      otherBufferIdx = &modTxDataBufferIdx;
    }
    // determine bit value to send (1 or 0)
    txBit = buffer[(*bufferIdx)] & modTxBufferBitMask;
  } else {
    // null buffer address b/c we shouldn't be changing anything
    buffer = 0x00;
    // determine bit value to send (in this case should be the handshake)
    txBit = MOD_FRAME_NULL_HANDSHAKE_BYTE & modTxBufferBitMask;
  }
  
  if ( modTxBufferEighthOfNibbleFlag ){
    
    // toggle the modulation source
    if ( txBit )
      LSR_EN_CH1_LAT |= LSR_EN_CH1_MASK;
    else
      LSR_EN_CH1_LAT &= ~LSR_EN_CH1_MASK;
    
    // circular bit shift byte mask
    //modTxBufferBitMask = (modTxBufferBitMask << 1) | (modTxBufferBitMask >> 7);
    modTxBufferBitMask = (modTxBufferBitMask >> 1) | (modTxBufferBitMask << 7);
    
    // invert half bit flag which helps us implement Manchester encoding scheme
    modTxBufferEighthOfNibbleFlag = 0x00;
    
    // increment byte and fill new byte from queue if we had carry bit on mask
    if ( (modSigLockState) && (modTxBufferBitMask & 0x80) && (buffer != 0x00) ) {
      //modTxDataBufferQueue[modTxDataBufferIdx-1] = 0x00;
      if ( ++(*bufferIdx) == bufferSizeBytes );
        *otherBufferIdx = 0;
    }
    
  } else {
    
    // toggle the modulation source
    if ( txBit )
      LSR_EN_CH1_LAT &= ~LSR_EN_CH1_MASK;
    else
      LSR_EN_CH1_LAT |= LSR_EN_CH1_MASK;
    
    // invert half bit flag which helps us implement Manchester encoding scheme
    modTxBufferEighthOfNibbleFlag = 0x01;
    
  }
  
  // clear the TxIF interrupt flag bit
  MOD_TIMER_TX_INT_FLAG &= ~MOD_TIMER_TX_INT_MASK;
  
  return;
}

void __ISR( _TIMER_5_VECTOR, IPL7AUTO ) _TIMER5_HANDLER( void ) {
  
  // turn off timer since this is a single call operation
  timerBOff( &MOD_TIMER_RX_CON );
  
  // flip necessary ADC configuration bits to switch sampling timing state
  AD1CON3 ^= modRxHiccupFlipMask;
  
  // clear the timer register TMRx
  timerBReset( &MOD_TIMER_RX_REG );
  
  //!! debugging
  LATE &= ~(1<<6);
  
  // clear hiccup state flag so we know we are done hiccuping (for now)
  modRxHiccupState = 0x00;
  
  // clear the TxIF interrupt flag bit
  MOD_TIMER_RX_INT_FLAG &= ~MOD_TIMER_RX_INT_MASK;
  
  return;
}
  
void __ISR( _ADC_VECTOR, IPL6AUTO ) __ADC_HANDLER( void ) {
  
  uint8_t msbPartOneReg;
  uint8_t msbPartTwoReg;
  uint8_t lsbPartOneReg;
  uint8_t lsbPartTwoReg;
  uint8_t msb;
  uint8_t lsb;
  uint8_t msbBitShift;
  uint8_t lsbBitShift;
  
  uint16_t hiccupChannelValue;
  uint8_t hiccupCompareValue;
  uint8_t hiccupCompareMask;
  
  uint8_t bitErrorTemp;
  
  volatile uint8_t * buffer;
  volatile uint16_t * bufferIdx;
  volatile uint16_t * otherBufferIdx;
  uint16_t bufferSizeBytes;
  
  // BUFS: Buffer Fill Status bit
  // Only valid when BUFM = 1 (ADRES split into 2 x 8-word buffers).
  if (AD1CON2 & (1<<7)) {
    // 1 = ADC is currently filling buffer 0x8-0xF, user should access 0x0-0x7
    
    //!! debugging
    LATA |= (1<<0);
  
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF0;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF1;;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF2;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF3;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF4;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF5;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF6;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF7;
    
    msbPartOneReg = (ADC1BUF1);
    msbPartTwoReg = (ADC1BUF3);
    lsbPartOneReg = (ADC1BUF5);
    lsbPartTwoReg = (ADC1BUF7);
    
    qpLastCh1VSenseReg = (ADC1BUF0);
    qpLastCh2VSenseReg = (ADC1BUF2);
    qpLastCh3VSenseReg = (ADC1BUF4);
    qpLastCh4VSenseReg = (ADC1BUF6);
    
    if ( !modRxNibbleFlag ) {
      // define location markers of where to put bits in byte
      msbBitShift = 7;
      lsbBitShift = 6;
    } else {
      // define location markers of where to put bits in byte
      msbBitShift = 3;
      lsbBitShift = 2;
    }
    
  } else {
    // 0 = ADC is currently filling buffer 0x0-0x7, user should access 0x8-0xF
    
    //!! debugging
    LATA &= ~(1<<0);
  
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF8;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF9;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFA;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFB;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFC;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFD;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFE;
    modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFF;
    
    msbPartOneReg = (ADC1BUF9);
    msbPartTwoReg = (ADC1BUFB);
    lsbPartOneReg = (ADC1BUFD);
    lsbPartTwoReg = (ADC1BUFF);
    
    qpLastCh1VSenseReg = (ADC1BUF8);
    qpLastCh2VSenseReg = (ADC1BUFA);
    qpLastCh3VSenseReg = (ADC1BUFC);
    qpLastCh4VSenseReg = (ADC1BUFE);
    
    if ( !modRxNibbleFlag ) {
      // define location markers of where to put bits in byte
      msbBitShift = 5;
      lsbBitShift = 4;
      
      // invert nibble flag to keep track of where we are at in this byte
      modRxNibbleFlag = 0x01;
    } else {
      // define location markers of where to put bits in byte
      msbBitShift = 1;
      lsbBitShift = 0;
      
      // invert nibble flag to keep track of where we are at in this byte
      modRxNibbleFlag = 0x00;
    }
  
  }
  
  // determine whether we are receiving the preamble or data chunk, when either
  // index is at its max that is a sign the associated data has already been 
  // received
  //if ( modRxHeaderBufferIdx == MOD_FRAME_HEADER_SIZE_BYTES ) {
    // preamble has been received, we should be receiving the data
    buffer = modRxDataBuffer;
    bufferIdx = &modRxDataBufferIdx;
    bufferSizeBytes = MOD_FRAME_DATA_SIZE_BYTES;
    otherBufferIdx = &modRxHeaderBufferIdx;
  //} else {
  //  // data has been received, we should be receiving the preamble
  //  buffer = modRxHeaderBuffer;
  //  bufferIdx = &modRxHeaderBufferIdx;
  //  bufferSizeBytes = MOD_FRAME_HEADER_SIZE_BYTES;
  //  otherBufferIdx = &modRxDataBufferIdx;
  //}
  
  if ( msbPartOneReg < msbPartTwoReg )
    msb = 0x01;
  else
    msb = 0x00;
  
  if ( lsbPartOneReg < lsbPartTwoReg )
    lsb = 0x01;
  else
    lsb = 0x00;
  
  // placing newly received bit(s) in their correct places with regard to
  // framing
  buffer[(*bufferIdx)] |= (
          ((msb)<<msbBitShift) |
          ((lsb)<<lsbBitShift) );
    
  if ( !(AD1CON2 & (1<<7)) && (modRxNibbleFlag) ) {
    // do activities if we are at the very end of a byte
    
    
    //if ( bufferSizeBytes == MOD_FRAME_HEADER_SIZE_BYTES ) {
      // do activities only if we are receiving the header portion of the frame
      
    //} else {
      // do activities only if we are receiving the data portion of the frame
      
      if ( (!modSigLockState) ) {
        // byte is finished and we are not locked yet, we should first check to
        // if we have signal lock
        if ( (buffer[(uint8_t)((*bufferIdx))] == MOD_FRAME_NULL_HANDSHAKE_BYTE) &&
                (buffer[(uint8_t)((*bufferIdx)-1)] == MOD_FRAME_NULL_HANDSHAKE_BYTE) &&
                (buffer[(uint8_t)((*bufferIdx)-2)] == MOD_FRAME_NULL_HANDSHAKE_BYTE) &&
                (buffer[(uint8_t)((*bufferIdx)-3)] == MOD_FRAME_NULL_HANDSHAKE_BYTE) )
          modSigLockState = 0x01;

        // do error checking to see what our bit error rate is like
        bitErrorTemp = (buffer[(*bufferIdx)] ^ MOD_FRAME_NULL_HANDSHAKE_BYTE);
        while ( bitErrorTemp ) {
          bitErrorTemp &= (bitErrorTemp - 1);
          modRxBitErrors++;
        }

      } else {

        // do error checking to see what our bit error rate is like
        bitErrorTemp = (buffer[(*bufferIdx)] ^ MOD_FRAME_NULL_HANDSHAKE_BYTE);
        while ( bitErrorTemp ) {
          bitErrorTemp &= (bitErrorTemp - 1);
          modRxBitErrors++;
        }
      }
      
    //}
    //if ( modSigLockState )
    //    *otherBufferIdx = 0;
    //  else
    // increment byte index
    if ( ++(*bufferIdx) == bufferSizeBytes )
      *bufferIdx = 0;
    
  }
  
  
  switch ( modRxActiveQuadrant ) {
    case 1:
      hiccupChannelValue = qpLastCh1VSenseReg;
      break;
    case 2:
      hiccupChannelValue = qpLastCh2VSenseReg;
      //if ( modSigLockState )
      //  hiccupCompareValue = msb;
      //else
        hiccupCompareValue = (1<<msbBitShift) & MOD_FRAME_NULL_HANDSHAKE_BYTE;      
      break;
    case 3:
      hiccupChannelValue = qpLastCh3VSenseReg;
      break;
    case 4:
      hiccupChannelValue = qpLastCh4VSenseReg;
      break;
  }
  
  // store last bits in case we need to reference them for clock recovery
  // operations in the next cycle
  lastMsb = msb;
  lastLsb = lsb;
  
  // re-determine hiccup threshold value based on last data bits
  //modRxHiccupThres = (
  //        msbPartOneReg + msbPartTwoReg + 
  //        lsbPartOneReg + lsbPartTwoReg ) / 4;
  
  // we need to hiccup to keep clock in sync...do it
  //if ( (modRxDataBuffer[modRxDataBufferIdx] & MOD_RX_HANDSHAKE_BYTE) && !modRxHiccupState ) {
  if ( (!modRxHiccupState) && (modSigLockState) &&
          ( (!hiccupCompareValue && (hiccupChannelValue > modRxHiccupThres)) ||
            (hiccupCompareValue && (hiccupChannelValue < modRxHiccupThres)) ) ) {
  //if ( (!modRxHiccupState) &&
  //        ( (!modSigLockState) ||
  //          (!hiccupCompareValue && (hiccupChannelValue > modRxHiccupThres)) ||
  //          (hiccupCompareValue && (hiccupChannelValue < modRxHiccupThres)) ) ) {
  //if ( (qpLastCh2VSenseReg > modRxHiccupThres) && (!modRxHiccupState) ) {
  //if ( (qpLastCh2VSenseReg > modRxHiccupThres) && (!modRxHiccupState) ) {
    
    
    // change flag so we don't undo what we already did if in hiccup state
    modRxHiccupState = 0x01;

    // flip necessary ADC configuration bits to switch sampling timing state
    AD1CON3 ^= modRxHiccupFlipMask;
    
    //!! debugging
    LATE |= (1<<6);
    
    // start running timer to time when to re-flip sampling timing state (this 
    // assumes timer register is reset at the end of every interrupt and during 
    // initialization)
    timerBOn( &MOD_TIMER_RX_CON );
  }
  
  // clear the TxIF interrupt flag bit
  MOD_ADC_INT_FLAG &= ~MOD_ADC_INT_MASK;
  
  return;
}