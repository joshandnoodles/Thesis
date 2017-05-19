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
#include <string.h>

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
volatile uint8_t modTxBuffer[MOD_FRAME_SIZE_BYTES];
volatile uint8_t * modTxBufferHeader;
volatile uint8_t * modTxBufferData;
volatile uint8_t * modTxBufferEnd;
volatile uint8_t * modTxBufferCur;
volatile uint8_t modTxBufferBitMask;
volatile uint8_t modTxBufferEighthOfNibbleFlag;

// variables to keep track of ongoing rx acquisition strategies
uint8_t modRxActiveQuadrant;
uint32_t modRxHiccupNs;
volatile uint16_t modRxHiccupThresH;
volatile uint16_t modRxHiccupThresL;
volatile uint8_t modRxBuffer[MOD_FRAME_SIZE_BYTES];
volatile uint8_t * modRxBufferHeader;
volatile uint8_t * modRxBufferData;
volatile uint8_t * modRxBufferEnd;
volatile uint8_t * modRxBufferCur;
volatile uint16_t modRxSigLockIdx;
volatile uint16_t modRxADCBuffer[MOD_FRAME_DATA_SIZE_BYTES];
volatile uint8_t modRxADCBufferIdx;
volatile uint8_t modRxNibbleFlag;
uint32_t modRxHiccupFlipMask;   // used to invert between ADC timing states
uint32_t modRxBitErrors;
volatile uint8_t lastMsb;
volatile uint8_t lastLsb;

// variable to keep track of dummy test data set
volatile uint8_t * modBufferTestData;
volatile uint8_t * modBufferTestDataEnd;
volatile uint8_t * modBufferTestDataCur;

// variables to keep track of state changes
uint8_t modState;
volatile uint8_t modRxHiccupState;
volatile uint8_t modSigLockState; // 0 = no known signal recognized
                                  // 1 = null handshake seen and locked onto
                                  // 2 = data handshake seen, start sending data

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
  
  // set default hiccup timing
  modRxHiccupNs = MOD_DEFAULT_RX_HICCUP_NS;
          
  // set default threshold values for both hiccup ADC nodes
  modRxHiccupThresH = MOD_DEFAULT_RX_HICCUP_THRES;
  modRxHiccupThresL = MOD_DEFAULT_RX_HICCUP_THRES;
  
  // set any pointers for framing reference on the tx side
  modTxBufferHeader = modTxBuffer;
  modTxBufferData = modTxBufferHeader + MOD_FRAME_HEADER_SIZE_BYTES;
  modTxBufferEnd = modTxBufferData + MOD_FRAME_DATA_SIZE_BYTES;
  
  // set any pointers for framing reference on the rx side
  modRxBufferHeader = modRxBuffer;
  modRxBufferData = modRxBufferHeader + MOD_FRAME_HEADER_SIZE_BYTES;
  modRxBufferEnd = modRxBufferData + MOD_FRAME_DATA_SIZE_BYTES;
  modBufferTestData = modTxBufferData;//!!testDataSeq256BytesNoReplacement;
  modBufferTestDataEnd = modBufferTestData + MOD_FRAME_DATA_SIZE_BYTES;
  
  // reset any flags/variables that only need to be set once here at the start
  modSigLockState = 0x00;
  modRxHiccupState = 0x00;
  modRxBitErrors = 0x00000000;
  
  return;
}

uint8_t modOn( void ) {
  
  uint16_t qpCh1Reg,
           qpCh2Reg,
           qpCh3Reg,
           qpCh4Reg;
  uint16_t idx;
  
  // check modulation power supply source before trying to begin modulation
  if ( !lsrLoadSwitchState )
    return 0x0;
  
  // reset pointers for tx buffer(s) and frame tracking
  modTxBufferCur = modTxBuffer;
  modTxBufferBitMask = 0x80;
  modTxBufferEighthOfNibbleFlag = 0x00;
  
  // reset pointers for rx buffer(s) and frame tracking
  modRxBufferCur = modRxBuffer;
  modRxNibbleFlag = 0x00;
  modRxSigLockIdx = 0;
  
  // ready tx buffer by initializing to default values
  modTxBufferHeader[0] = MOD_FRAME_DATA_HANDSHAKE_BYTE;
  for ( idx=0; idx<MOD_FRAME_DATA_SIZE_BYTES; idx++ )
    modTxBufferData[idx] = modBufferTestData[idx];
  
  // ready rx buffer by zeroing it out
  for ( idx=0; idx<MOD_FRAME_SIZE_BYTES; idx++ )
    modRxBuffer[idx] = 0;
  
  // set period of interrupt source to correct rx hiccup timing value
  timerBSetPeriodNs( &MOD_TIMER_RX_CON, &MOD_TIMER_RX_PERIOD, modRxHiccupNs );
  
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
  
  // start calling interrupts for tx operation
  //timerBOn( &MOD_TIMER_TX_CON );
  //interruptOn( &MOD_TIMER_TX_INT_ENB, MOD_TIMER_TX_INT_MASK );
  
  // change flag(s) to keep track of state changes
  modState = 0x1;
  
  return 0x1;
}

uint8_t modOff( void ) {
  
  // stop calling interrupts for tx operation
  interruptOff( &MOD_TIMER_TX_INT_ENB, &MOD_TIMER_TX_INT_FLAG, MOD_TIMER_TX_INT_MASK );
  timerBOff( &MOD_TIMER_TX_CON );
  
  // stop calling interrupts for rx operation
  interruptOff( &MOD_ADC_INT_ENB, &MOD_ADC_INT_FLAG, MOD_ADC_INT_MASK );
  adcAutoOff();
  interruptOff( &MOD_TIMER_RX_INT_ENB, &MOD_TIMER_RX_INT_FLAG, MOD_TIMER_RX_INT_MASK );
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
  
  switch (modSigLockState) {
    case 2:      
      // determine bit value to send (1 or 0)
      txBit = *modTxBufferCur & modTxBufferBitMask;
      
      break;
    case 1:
      // determine bit value to send (in this case should be the null handshake)
      // every byte of the buffer length except one which will be the data
      // handshake byte
      if ( modTxBufferCur == modTxBuffer )
        txBit = MOD_FRAME_DATA_HANDSHAKE_BYTE & modTxBufferBitMask;
      else
        txBit = MOD_FRAME_NULL_HANDSHAKE_BYTE & modTxBufferBitMask;
      
      break;
    case 0:
      // determine bit value to send (in this case should be the null handshake)
      // every byte of the entire buffer length
      txBit = MOD_FRAME_NULL_HANDSHAKE_BYTE & modTxBufferBitMask;
      
      break;
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
    
    if ( (modTxBufferBitMask & 0x80) ) {
      // do special activities if we are at the end of a byte
      if ( (++modTxBufferCur) == modTxBufferEnd )
        modTxBufferCur = modTxBuffer;
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

  // BUFS: Buffer Fill Status bit
  // Only valid when BUFM = 1 (ADRES split into 2 x 8-word buffers).
  if (AD1CON2 & (1<<7)) {
    // 1 = ADC is currently filling buffer 0x8-0xF, user should access 0x0-0x7
    
    //!! debugging
    LATA |= (1<<0);
  
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF0;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF1;;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF2;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF3;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF4;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF5;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF6;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF7;
    
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
  
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF8;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF9;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFA;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFB;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFC;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFD;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFE;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFF;    
    
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
  *modRxBufferCur |= (
              ((msb)<<msbBitShift) |
              ((lsb)<<lsbBitShift) );
  
  switch ( modRxActiveQuadrant ) {
    case 1:
      hiccupChannelValue = qpLastCh1VSenseReg;
      break;
    case 2:
      hiccupChannelValue = qpLastCh2VSenseReg;
      if ( modSigLockState ) {
        hiccupCompareValue = msb;
        // re-determine hiccup threshold value based on last data bits
        if ( msb )
          modRxHiccupThresH = (msbPartOneReg + msbPartTwoReg ) / 2;
        else
          modRxHiccupThresL = (msbPartOneReg + msbPartTwoReg ) / 2;
      } else {
        hiccupCompareValue = (1<<msbBitShift) & MOD_FRAME_NULL_HANDSHAKE_BYTE;
      }
      break;
    case 3:
      hiccupChannelValue = qpLastCh3VSenseReg;
      break;
    case 4:
      hiccupChannelValue = qpLastCh4VSenseReg;
      break;
  }
  
  // we need to hiccup to keep clock in sync...do it
  //if ( (modRxDataBuffer[modRxDataBufferIdx] & MOD_RX_HANDSHAKE_BYTE) && !modRxHiccupState ) {
  if ( (!modRxHiccupState) && (modSigLockState) &&
          ( (!hiccupCompareValue && (hiccupChannelValue > modRxHiccupThresL)) ||
            (hiccupCompareValue && (hiccupChannelValue < modRxHiccupThresH)) ) ) {
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
  
  // the following is less crucial for timing, here we take the received data 
  // and do important things with them that are dependent on where we are at in
  // the frame, other housekeeping issues such as keeping out old bit values
  // in memory and averaging threshold values to account for changes in signal
  // strength  
  
  if ( !(AD1CON2 & (1<<7)) && !(modRxNibbleFlag) ) {
    // do activities if we are at the very end of a byte
    
    switch (modSigLockState) {
      case 2:
        // byte is finished and we are locked and already sending data, proceed
        // nominally
        if ( modRxBufferCur < modRxBufferData ) {
          // do activities only if we are receiving the header frame portion

          if ( modRxBufferCur == (modRxBufferHeader+0) ) {
            if ( ((*modRxBufferCur) != MOD_FRAME_DATA_HANDSHAKE_BYTE) ) {
              // if first byte of header is not handshake, something went wrong,
              // assume signal lock has been lost
              modSigLockState = 0x00;
              modRxSigLockIdx = 0;
            }
          } else if ( modRxBufferCur == (modRxBufferHeader+1) ) {
          }

        } else {
          // do activities only if we are receiving the data frame portion
      
          // do error checking to see what our bit error rate is like
          bitErrorTemp = (*modRxBufferCur) ^ (*modBufferTestDataCur);
          if ( (++modBufferTestDataCur) == modBufferTestDataEnd )
            modBufferTestDataCur = modBufferTestData;
          while ( bitErrorTemp ) {
            bitErrorTemp &= (bitErrorTemp - 1);
            modRxBitErrors++;
          }
        }
        
        break;
      case 1:
        // byte is finished and we are locked on but not sending data yet,
        // check to see if other endpoint has sent data handshake flag
        if ( (*modRxBufferCur) == MOD_FRAME_DATA_HANDSHAKE_BYTE ) {
          modTxBufferCur = modTxBuffer;
          modTxBufferBitMask = 0x80;
          modTxBufferEighthOfNibbleFlag = 0x00;
          //*modRxBuffer = *modRxBufferCur;
          modRxBufferCur = modRxBuffer;
          modBufferTestDataCur = modBufferTestData;
          modSigLockState = 0x02;
        } else if ( (*modRxBufferCur) != MOD_FRAME_NULL_HANDSHAKE_BYTE ) {
          // if byte of is not either null or data handshake, something went
          // wrong, assume signal lock has been lost
          modSigLockState = 0x00;
          modRxSigLockIdx = 0;
        }
        
        // do error checking to see what our bit error rate is like
        bitErrorTemp = ((*modRxBufferCur) ^ MOD_FRAME_NULL_HANDSHAKE_BYTE);
                ((*modRxBufferCur) ^ MOD_FRAME_DATA_HANDSHAKE_BYTE);
        while ( bitErrorTemp ) {
          bitErrorTemp &= (bitErrorTemp - 1);
          modRxBitErrors++;
        }
        
        break;
      case 0:
        // byte is finished and we are not locked yet, check to see if we
        // have signal lock
        if ( (*modRxBufferCur) == MOD_FRAME_NULL_HANDSHAKE_BYTE ) {
          if ( (++modRxSigLockIdx) == MOD_RX_SIG_LOCK_CNT )
            modSigLockState = 0x01;
        } else {
          modRxSigLockIdx = 0;
        }
       
        // do error checking to see what our bit error rate is like
        bitErrorTemp = ((*modRxBufferCur) ^ MOD_FRAME_NULL_HANDSHAKE_BYTE);
                ((*modRxBufferCur) ^ MOD_FRAME_DATA_HANDSHAKE_BYTE);
        while ( bitErrorTemp ) {
          bitErrorTemp &= (bitErrorTemp - 1);
          modRxBitErrors++;
        }
        
        break;
    }
    
    // now we need increment our index clear the next byte in the buffer,
    // do not, I repeat do not remove the clear byte command as it is stupid 
    // important in retaining the integrity of our byte buffer filling process
    if ( (++modRxBufferCur) == modRxBufferEnd )
      modRxBufferCur = modRxBuffer;
    *modRxBufferCur = 0x00;
   
  }
  
  // store last bits in case we need to reference them for clock recovery
  // operations in the next cycle
  lastMsb = msb;
  lastLsb = lsb;
  
  // clear the TxIF interrupt flag bit
  MOD_ADC_INT_FLAG &= ~MOD_ADC_INT_MASK;
  
  return;
}