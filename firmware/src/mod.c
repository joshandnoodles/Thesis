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
#include "adc.h"
#include "test_data.h"
#include "debug.h"

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
volatile uint8_t modTxSigLockFlag;

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
uint8_t lastLsb;

// constants defining framing chunk to help simplify/speed up ADC interrupt
typedef struct { 
  volatile unsigned int * msbPartOneReg;
  volatile unsigned int * msbPartTwoReg;
  volatile unsigned int * lsbPartOneReg;
  volatile unsigned int * lsbPartTwoReg;
  
  volatile unsigned int * qpChVSenseRegTemps[4];
  
  uint8_t msbBitShiftArr[2];   // based on current nibble (index w/ nibble flag)
  uint8_t lsbBitShiftArr[2];   // based on current nibble (index w/ nibble flag)
} BufsNibbleStructure;
static const BufsNibbleStructure MOD_RX_FIRST_BUFS_STRUCT = {
    (&ADC1BUF1), (&ADC1BUF3), (&ADC1BUF5), (&ADC1BUF7),
    {(&ADC1BUF0), (&ADC1BUF2), (&ADC1BUF4), (&ADC1BUF6)},
    {7, 3}, {6, 2},
  };
static const BufsNibbleStructure MOD_RX_SECOND_BUFS_STRUCT = {
    (&ADC1BUF9), (&ADC1BUFB), (&ADC1BUFD), (&ADC1BUFF),
    {(&ADC1BUF8), (&ADC1BUFA), (&ADC1BUFC), (&ADC1BUFE)},
    {5, 1}, {4, 0},
  };
volatile uint32_t tickTock;//!!

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
volatile uint8_t modRxAlignEnb;

void initMod( void ) {
  
  uint16_t idx;
  
  //!! debugging
  TRISE &= ~(1<<6);
  TRISA &= ~(1<<0);
  
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
          6,
          MOD_TIMER_RX_SUBPRIO_OFFSET,
          1,
          MOD_TIMER_RX_INT_MASK );
  initInterrupt(
          &MOD_ADC_INT_PRIO,
          &MOD_ADC_INT_FLAG,
          MOD_ADC_PRIO_OFFSET,
          5,
          MOD_ADC_SUBPRIO_OFFSET,
          1,
          MOD_ADC_INT_MASK );
  
  // set defaults for tx operations
  modSetFreqHz( MOD_DEFAULT_TX_FREQ_HZ );
  
  // set defaults for rx operations
  modRxActiveQuadrant = MOD_DEFAULT_RX_ACTIVE_QUAD;
  modRxAlignEnb = MOD_DEFAULT_RX_ALIGN_ENB;
  modRxHiccupNs = MOD_DEFAULT_RX_HICCUP_NS;
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
  modBufferTestData = testDataSeq256BytesNoReplacement;
  modBufferTestDataEnd = modBufferTestData + MOD_FRAME_DATA_SIZE_BYTES;
  
  // reset any flags/variables that only need to be set once here at the start
  modSigLockState = 0x00;
  modTxSigLockFlag = 0x00;
  modRxHiccupState = 0x00;
  modRxBitErrors = 0x00000000;
  
  return;
}


uint8_t modOn( void ) {
  
  uint16_t intialActiveQuadrantVal;
  uint32_t initialActiveQuadrantMask;
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
  modTxBufferHeader[0] = MOD_HANDSHAKE_DATA_BYTE;
  for ( idx=0; idx<MOD_FRAME_DATA_SIZE_BYTES; idx++ )
    modTxBufferData[idx] = modBufferTestData[idx];
  
  // ready rx buffer by zeroing it out
  for ( idx=0; idx<MOD_FRAME_SIZE_BYTES; idx++ )
    modRxBuffer[idx] = 0;
  
  // set period of interrupt source to correct rx hiccup timing value
  timerBSetPeriodNs( &MOD_TIMER_RX_CON, &MOD_TIMER_RX_PERIOD, modRxHiccupNs );
  
  // dynamically set active quadrant by checking each quadrant's value manually
  qpReadCh1VSense();
  qpReadCh2VSense();
  qpReadCh3VSense();
  qpReadCh4VSense();
  intialActiveQuadrantVal = 0;
  for ( idx=1; idx<4; idx+=2 ) {
    if ( (intialActiveQuadrantVal) < (*qpLastChVSenseRegPtrs[idx]) ) {
      modRxActiveQuadrant = idx;
      intialActiveQuadrantVal = (*qpLastChVSenseRegPtrs[idx]);
    }
  }
  
  // clear the timer register TMRx
  timerBReset( &MOD_TIMER_TX_REG );
  
  // clear the timer register TMRx
  timerBReset( &MOD_TIMER_RX_REG );
  
  // start calling interrupts for rx operation (part 1)
  switch ( modRxActiveQuadrant ) {
    case 0:
      initialActiveQuadrantMask = QP_CH1_VSENSE_AN_MASK;
      break;
    case 1:
      initialActiveQuadrantMask = QP_CH2_VSENSE_AN_MASK;
      break;
    case 2:
      initialActiveQuadrantMask = QP_CH3_VSENSE_AN_MASK;
      break;
    case 3:
      initialActiveQuadrantMask = QP_CH4_VSENSE_AN_MASK;
      break;   
  }
  adcAutoOn(
          QP_CH1_VSENSE_AN_MASK | QP_CH2_VSENSE_AN_MASK | 
          QP_CH3_VSENSE_AN_MASK | QP_CH4_VSENSE_AN_MASK, 
          initialActiveQuadrantMask, 8, 0x01,
          MOD_RX_ADC_SAMC_TAD, MOD_RX_ADC_ADCS );
  //adcAutoOn( QP_CH2_VSENSE_AN_MASK, 0x0000, 8, 0x01 );
  
  // set hiccup mask so it can be used to quickly toggle between two different
  // sampling timing values
  modRxHiccupFlipMask = AD1CON3 & (0b11111<<8);
  if ( MOD_RX_HICCUP_DIRECTION )
    modRxHiccupFlipMask = (modRxHiccupFlipMask + (MOD_RX_HICCUP_TAD<<8)) ^ 
            modRxHiccupFlipMask;
  else
    modRxHiccupFlipMask = (modRxHiccupFlipMask - (MOD_RX_HICCUP_TAD<<8)) ^ 
            modRxHiccupFlipMask;
  
  // start calling interrupts for rx operation (part 2)
  interruptOn( &MOD_TIMER_RX_INT_ENB, MOD_TIMER_RX_INT_MASK );
  interruptOn( &MOD_ADC_INT_ENB, MOD_ADC_INT_MASK );
  
  // start calling interrupts for tx operation
  timerBOn( &MOD_TIMER_TX_CON );
  interruptOn( &MOD_TIMER_TX_INT_ENB, MOD_TIMER_TX_INT_MASK );
  
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
  modTxSigLockFlag = 0x00;
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

void modSetActiveQuadrant( uint8_t newActiveQuadrant ) {
  
  uint32_t flipMask;
  
  if ( newActiveQuadrant != modRxActiveQuadrant ) {
    // start with current CH0SB bit settings for flip mask, inverting this will
    // zero out settings completely
    flipMask = ((0b1111)<<24) & AD1CHS;

    // determine what CH0SB bit settings should be for new active quadrant and
    // combine this in the flip mask
    switch ( newActiveQuadrant ) {
      case 0:
        flipMask ^= (QP_CH1_VSENSE_AN_CH<<24);
        break;
      case 1:
        flipMask ^= (QP_CH2_VSENSE_AN_CH<<24);
        break;
      case 2:
        flipMask ^= (QP_CH3_VSENSE_AN_CH<<24);
        break;
      case 3:
        flipMask ^= (QP_CH4_VSENSE_AN_CH<<24);
        break;   
    }
  
    // we should now wait until any rx ADC interrupts to change the current active
    // quadrant value since the logic within these interrupts are dependent on
    // this value (and changing it in the middle is a bad idea)
    while ( MOD_ADC_INT_FLAG & MOD_ADC_INT_MASK );

    // now we should be safe to change the ADC sampler bits and the active
    // quadrant variable
    AD1CHSINV = flipMask;
    modRxActiveQuadrant = newActiveQuadrant;
  }
  
  return;
}

uint8_t modUpdateAlignmentFrame( void ) {
  // this assumes the QP is aligned in the following configuration:
  //   ch3 ch2
  //   ch4 ch1
  
  uint8_t tempAlignFrame;
  
  tempAlignFrame = 0x00;
  
  switch (modRxActiveQuadrant) {
    case 1:
      if ( (MOD_RX_ALIGN_THRES_MULT_THREE * (*qpLastChVSenseRegPtrs[2])) > 
              (*qpLastChVSenseRegPtrs[3]) ) {
        // laser is aligned way way too far up, need to move down
        tempAlignFrame |= 0x05;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_THREE * (*qpLastChVSenseRegPtrs[0])) > 
              (*qpLastChVSenseRegPtrs[3]) ) {
        // laser is aligned way way too far right, need to move left
        tempAlignFrame |= 0x50;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_TWO * (*qpLastChVSenseRegPtrs[2])) > 
              (*qpLastChVSenseRegPtrs[3]) ) {
        // laser is aligned way too far up, need to move down
        tempAlignFrame |= 0x03;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_TWO * (*qpLastChVSenseRegPtrs[0])) > 
              (*qpLastChVSenseRegPtrs[3]) ) {
        // laser is aligned way too far right, need to move left
        tempAlignFrame |= 0x30;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_ONE * (*qpLastChVSenseRegPtrs[2])) > 
              (*qpLastChVSenseRegPtrs[3]) ) {
        // laser is aligned too far up, need to move down
        tempAlignFrame |= 0x01;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_ONE * (*qpLastChVSenseRegPtrs[0])) > 
              (*qpLastChVSenseRegPtrs[3]) ) {
        // laser is aligned too far right, need to move left
        tempAlignFrame |= 0x10;
      }
      break;
    case 3:
      if ( (MOD_RX_ALIGN_THRES_MULT_THREE * (*qpLastChVSenseRegPtrs[0])) > 
              (*qpLastChVSenseRegPtrs[1]) ) {
        // laser is aligned way too far down, need to move up
        tempAlignFrame |= 0x06;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_THREE * (*qpLastChVSenseRegPtrs[2])) > 
              (*qpLastChVSenseRegPtrs[1]) ) {
        // laser is aligned way too far left, need to move right
        tempAlignFrame |= 0x60;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_TWO * (*qpLastChVSenseRegPtrs[0])) > 
              (*qpLastChVSenseRegPtrs[1]) ) {
        // laser is aligned way too far down, need to move up
        tempAlignFrame |= 0x04;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_TWO * (*qpLastChVSenseRegPtrs[2])) > 
              (*qpLastChVSenseRegPtrs[1]) ) {
        // laser is aligned way too far left, need to move right
        tempAlignFrame |= 0x40;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_ONE * (*qpLastChVSenseRegPtrs[0])) > 
              (*qpLastChVSenseRegPtrs[1]) ) {
        // laser is aligned too far down, need to move up
        tempAlignFrame |= 0x02;
      } else if ( (MOD_RX_ALIGN_THRES_MULT_ONE * (*qpLastChVSenseRegPtrs[2])) > 
              (*qpLastChVSenseRegPtrs[1]) ) {
        // laser is aligned too far left, need to move right
        tempAlignFrame |= 0x20;
      }
      break;
  }
  
  modTxBufferHeader[1] = tempAlignFrame;
  
  return tempAlignFrame;
}

void __ISR( _TIMER_4_VECTOR, IPL7SRS ) _TIMER4_HANDLER( void ) {
  
  uint8_t txBit;
  
  if ( (modTxSigLockFlag) && (modTxBufferBitMask == 0x80) && 
          (!modTxBufferEighthOfNibbleFlag) ) {
    modTxSigLockFlag = 0x00;
    modTxBufferCur = modTxBuffer;
    modSigLockState = 0x02;
    #ifdef MASTER
      modRxBufferCur = modRxBufferEnd - 2;
      modBufferTestDataCur = modBufferTestDataEnd - 2;
    #endif
  }
  
  switch (modSigLockState) {
    case 2:      
      // determine bit value to send (1 or 0)
      txBit = *modTxBufferCur & modTxBufferBitMask;
      
      break;
    case 1:
      // determine bit value to send (in this case should be the null handshake)
      // every byte of the buffer length except one which will be the data
      // handshake byte
      if ( (modTxBufferCur == modTxBuffer) )
        txBit = MOD_HANDSHAKE_FRAME_ALIGN_BYTE & modTxBufferBitMask;
      else
        txBit = MOD_HANDSHAKE_NULL_BYTE & modTxBufferBitMask;
      
      break;
    case 0:
      // determine bit value to send (in this case should be the null handshake)
      // every byte of the entire buffer length
      txBit = MOD_HANDSHAKE_NULL_BYTE & modTxBufferBitMask;
      
      break;
  }
  
  if ( modTxBufferEighthOfNibbleFlag ){
    
    // toggle the modulation source
    if ( txBit ) {
      //LSR_EN_CH1_LAT |= LSR_EN_CH1_MASK;
      LATDSET = LSR_EN_CH1_MASK;
    } else {
      //LSR_EN_CH1_LAT &= ~LSR_EN_CH1_MASK;
      LATDCLR = LSR_EN_CH1_MASK;
    }
    
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
    if ( txBit ) {
      //LSR_EN_CH1_LAT &= ~LSR_EN_CH1_MASK;
      LATDCLR = LSR_EN_CH1_MASK;
    } else {
      //LSR_EN_CH1_LAT |= LSR_EN_CH1_MASK;
      LATDSET = LSR_EN_CH1_MASK;
    }
    
    // invert half bit flag which helps us implement Manchester encoding scheme
    modTxBufferEighthOfNibbleFlag = 0x01;
    
  }
  
  // clear the TxIF interrupt flag bit
  //MOD_TIMER_TX_INT_FLAG &= ~MOD_TIMER_TX_INT_MASK;
  IFS0CLR = MOD_TIMER_TX_INT_MASK;
  
  return;
}

void __ISR( _TIMER_5_VECTOR, IPL6SOFT ) _TIMER5_HANDLER( void ) {
  
  // timing:
  //  c code = 20 core timer ticks = 500ns
  //  optimized c code = 10 core timer ticks = 250ns
  
  // turn off timer since this is a single call operation
  //timerBOff( &MOD_TIMER_RX_CON );
  T5CONCLR = 0x8000;
  
  // flip necessary ADC configuration bits to switch sampling timing state
  //AD1CON3 ^= modRxHiccupFlipMask;
  AD1CON3INV = modRxHiccupFlipMask;
  
  //!! debugging
  //LATE &= ~(1<<6);
  
  // clear hiccup state flag so we know we are done hiccuping (for now)
  modRxHiccupState = 0x00;
  
  // clear the TxIF interrupt flag bit
  //MOD_TIMER_RX_INT_FLAG &= ~MOD_TIMER_RX_INT_MASK;
  IFS0CLR = 0x100000;  
  
  return;
}

void __ISR( _ADC_VECTOR, IPL5SOFT ) __ADC_HANDLER( void ) {
  
  uint8_t msb;
  uint8_t lsb;
  BufsNibbleStructure bufsNibbleStruct;
  
  uint16_t hiccupChannelValue;
  uint8_t hiccupCompareValue;
  uint8_t hiccupCompareMask;
  
  uint8_t bitErrorTemp;
  uint16_t idx;
  
  // BUFS: Buffer Fill Status bit
  // Only valid when BUFM = 1 (ADRES split into 2 x 8-word buffers).
  if (AD1CON2 & (1<<7)) {
    // 1 = ADC is currently filling buffer 0x8-0xF, user should access 0x0-0x7
    
    //!! debugging
    //LATA |= (1<<0);
  
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF0;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF1;;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF2;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF3;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF4;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF5;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF6;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF7;
    
    // get pointers to ADC registers relevant to this chunk in the frame,
    // this struct also contains information about the location markers of where
    // to put bits in the byte (these bit shift arrays should be indexed with 
    // the nibble flag)
    bufsNibbleStruct = MOD_RX_FIRST_BUFS_STRUCT;
    
  } else {
    // 0 = ADC is currently filling buffer 0x0-0x7, user should access 0x8-0xF
    
    //!! debugging
    //LATA &= ~(1<<0);
  
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF8;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUF9;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFA;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFB;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFC;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFD;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFE;
    //modRxADCBuffer[modRxADCBufferIdx++] = ADC1BUFF;    
    
    // get pointers to ADC registers relevant to this chunk in the frame,
    // this struct also contains information about the location markers of where
    // to put bits in the byte (these bit shift arrays should be indexed with 
    // the nibble flag)
    bufsNibbleStruct = MOD_RX_SECOND_BUFS_STRUCT;
  }
  
  if ( (*bufsNibbleStruct.msbPartOneReg) < (*bufsNibbleStruct.msbPartTwoReg) )
    msb = 0x01;
  else
    msb = 0x00;
  if ( (*bufsNibbleStruct.lsbPartOneReg) < (*bufsNibbleStruct.lsbPartTwoReg) )
    lsb = 0x01;
  else
    lsb = 0x00;
  
  hiccupChannelValue = (*bufsNibbleStruct.qpChVSenseRegTemps[modRxActiveQuadrant]);
  switch ( modRxActiveQuadrant ) {
    case 1:
      if ( modSigLockState ) {
        // since we locked, assume received signal data is good enough to
        // compare against
        hiccupCompareValue = msb;
      } else {
        // since we are not locked, compare byte value to target handshake
        hiccupCompareValue = (1<<(bufsNibbleStruct.msbBitShiftArr[modRxNibbleFlag])) & MOD_HANDSHAKE_NULL_BYTE;
      }
      
      // re-determine hiccup threshold value based on data bits received
      // and save alignment ADC channel nodes for symmetric channels
      if ( hiccupCompareValue ) {
        modRxHiccupThresH = (((*bufsNibbleStruct.msbPartOneReg) + (*bufsNibbleStruct.msbPartTwoReg)) >> 1);
        //modRxHiccupThresH = (modRxHiccupThresH >> 1) + (((*bufsNibbleStruct.msbPartOneReg) + (*bufsNibbleStruct.msbPartTwoReg)) >> 2) + 0;
        //modRxHiccupThresH = (modRxHiccupThresH >> 1) + (modRxHiccupThresH >> 2) + (((*bufsNibbleStruct.msbPartOneReg) + (*bufsNibbleStruct.msbPartTwoReg)) >> 3) + 1;
      } else {
        modRxHiccupThresH = (((*bufsNibbleStruct.msbPartOneReg) + (*bufsNibbleStruct.msbPartTwoReg)) >> 1);
        //modRxHiccupThresL = (modRxHiccupThresL >> 1) + (((*bufsNibbleStruct.msbPartOneReg) + (*bufsNibbleStruct.msbPartTwoReg)) >> 2) + 0;
        //modRxHiccupThresL = (modRxHiccupThresL >> 1) + (modRxHiccupThresL >> 2) + (((*bufsNibbleStruct.msbPartOneReg) + (*bufsNibbleStruct.msbPartTwoReg)) >> 3) + 1;
     }
      break;
    case 3:
      if ( modSigLockState ) {
        // since we locked, assume received signal data is good enough to
        // compare against
        hiccupCompareValue = lsb;
      } else {
        // since we are not locked, compare byte value to target handshake
        hiccupCompareValue = (1<<(bufsNibbleStruct.lsbBitShiftArr[modRxNibbleFlag])) & MOD_HANDSHAKE_NULL_BYTE;
      }
      
      // re-determine hiccup threshold value based on data bits received
      // and save alignment ADC channel nodes for symmetric channels
      if ( hiccupCompareValue ) {
        modRxHiccupThresH = (((*bufsNibbleStruct.lsbPartOneReg) + (*bufsNibbleStruct.lsbPartTwoReg)) >> 1);
        //modRxHiccupThresH = (modRxHiccupThresH >> 1) + (((*bufsNibbleStruct.lsbPartOneReg) + (*bufsNibbleStruct.lsbPartTwoReg)) >> 2) + 0;
        //modRxHiccupThresH = (modRxHiccupThresH >> 1) + (modRxHiccupThresH >> 2) + (((*bufsNibbleStruct.lsbPartOneReg) + (*bufsNibbleStruct.lsbPartTwoReg)) >> 3) + 1;
      } else {
        modRxHiccupThresH = (((*bufsNibbleStruct.lsbPartOneReg) + (*bufsNibbleStruct.lsbPartTwoReg)) >> 1);
        //modRxHiccupThresL = (modRxHiccupThresL >> 1) + (((*bufsNibbleStruct.lsbPartOneReg) + (*bufsNibbleStruct.lsbPartTwoReg)) >> 2) + 0;
        //modRxHiccupThresL = (modRxHiccupThresL >> 1) + (modRxHiccupThresL >> 2) + (((*bufsNibbleStruct.lsbPartOneReg) + (*bufsNibbleStruct.lsbPartTwoReg)) >> 3) + 1;
      }
      break;
  }
  
  // we need to hiccup to keep clock in sync...do it
  //if ( (modRxDataBuffer[modRxDataBufferIdx] & MOD_RX_HANDSHAKE_BYTE) && !modRxHiccupState ) {
  if ( (!modRxHiccupState) && (modSigLockState) && (hiccupChannelValue) &&
          ( (!hiccupCompareValue && (hiccupChannelValue > modRxHiccupThresL)) ||
            (hiccupCompareValue && (hiccupChannelValue < modRxHiccupThresH)) ) ) {
    
    // change flag so we don't undo what we already did if in hiccup state
    modRxHiccupState = 0x01;

    // flip necessary ADC configuration bits to switch sampling timing state
    //AD1CON3 ^= modRxHiccupFlipMask;
    AD1CON3INV = modRxHiccupFlipMask;
    
    //!! debugging
    //LATE |= (1<<6);
    
    // clear the timer register TMRx
    //timerBReset( &MOD_TIMER_RX_REG );
    TMR5CLR = 0xFFFF;
  
    // start running timer to time when to re-flip sampling timing state (this 
    // assumes timer register is reset at the end of every interrupt and during 
    // initialization)
    //timerBOn( &MOD_TIMER_RX_CON );
    T5CONSET = 0x8000;
  }
  
  // the following is less crucial for timing, here we take the received data 
  // and do important things with them that are dependent on where we are at in
  // the frame, other housekeeping issues such as keeping out old bit values
  // in memory and averaging threshold values to account for changes in signal
  // strength  
  
  // placing newly received bit(s) in their correct places with regard to
  // framing
  *modRxBufferCur |= (
              ((msb)<<(bufsNibbleStruct.msbBitShiftArr[modRxNibbleFlag])) |
              ((lsb)<<(bufsNibbleStruct.lsbBitShiftArr[modRxNibbleFlag])) );
  
  if ( !(AD1CON2 & (1<<7)) ) {
    if ( modRxNibbleFlag ) {
      // do activities if we are at the very end of a byte

      switch (modSigLockState) {
        case 2:
          // byte is finished and we are locked and already sending data, proceed
          // nominally
          if ( modRxBufferCur < modRxBufferData ) {
            // do activities only if we are receiving the header frame portion

            if ( modRxBufferCur == (modRxBufferHeader+0) ) {
              if ( ((*modRxBufferCur) != MOD_HANDSHAKE_DATA_BYTE) &&
                      ((*modRxBufferCur) != MOD_HANDSHAKE_FRAME_ALIGN_BYTE) ) {
                // if first byte of header is not handshake, something went wrong,
                // assume signal lock has been lost
                modSigLockState = 0x00;
                modRxSigLockIdx = 0;
              }
            } else if ( modRxBufferCur == (modRxBufferHeader+1) ) {
              // second byte should contain alignment information
              
              if ( modRxAlignEnb ) {
                switch ( (*modRxBufferCur) & 0x0F ) {
                  // least-significant nibble contains vertical alignment cues
                  case 0x06:
                    // laser is aligned way way too far down, need to move up
                    SERVO_TILT_OCRS = SERVO_TILT_OCR + 3;
                    break;
                  case 0x05:
                    // laser is aligned way way too far up, need to move down
                    SERVO_TILT_OCRS = SERVO_TILT_OCR - 3;
                    break;
                  case 0x04:
                    // laser is aligned way too far down, need to move up
                    SERVO_TILT_OCRS = SERVO_TILT_OCR + 2;
                    break;
                  case 0x03:
                    // laser is aligned way too far up, need to move down
                    SERVO_TILT_OCRS = SERVO_TILT_OCR - 2;
                    break;
                  case 0x02:
                    // laser is aligned too far down, need to move up
                    SERVO_TILT_OCRS = SERVO_TILT_OCR + 1;
                    break;
                  case 0x01:
                    // laser is aligned too far up, need to move down
                    SERVO_TILT_OCRS = SERVO_TILT_OCR - 1;
                    break;
                  case 0x00:
                    // laser is properly aligned vertically, do nothing for now
                    break;
                }
                switch ( (*modRxBufferCur) & 0xF0 ) {
                  // most-significant nibble contains horizontal alignment cues
                  case 0x60:
                    // laser is aligned way way too far left, need to move right
                    SERVO_PAN_OCRS = SERVO_PAN_OCR + 3;
                    debugVal4++;
                    break;
                  case 0x50:
                    // laser is aligned way way too far right, need to move left
                    SERVO_PAN_OCRS = SERVO_PAN_OCR - 3;
                    debugVal4++;
                    break;
                  case 0x40:
                    // laser is aligned way too far left, need to move right
                    SERVO_PAN_OCRS = SERVO_PAN_OCR + 2;
                    debugVal3++;
                    break;
                  case 0x30:
                    // laser is aligned way too far right, need to move left
                    SERVO_PAN_OCRS = SERVO_PAN_OCR - 2;
                    debugVal3++;
                    break;
                  case 0x20:
                    // laser is aligned too far left, need to move right
                    SERVO_PAN_OCRS = SERVO_PAN_OCR + 1;
                    debugVal2++;
                    break;
                  case 0x10:
                    // laser is aligned too far right, need to move left
                    SERVO_PAN_OCRS = SERVO_PAN_OCR - 1;
                    debugVal2++;
                    break;
                  case 0x00:
                    // laser is properly aligned vertically, do nothing for now
                    break;
                }
                modUpdateAlignmentFrame();
              }
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
          if ( (*modRxBufferCur) == MOD_HANDSHAKE_FRAME_ALIGN_BYTE ) {
            #ifdef MASTER
              modTxSigLockFlag = 0x01;
            #else
              modRxBufferCur = modRxBuffer;
            #endif
          } else if ( (*modRxBufferCur) == MOD_HANDSHAKE_DATA_BYTE ) {
            #ifdef SLAVE
              modTxSigLockFlag = 0x01;
              modRxBufferCur = modRxBuffer;
              modBufferTestDataCur = modBufferTestData;
              modRxSigLockIdx = 0;
            #endif
          } else if ( (*modRxBufferCur) == MOD_HANDSHAKE_NULL_BYTE ) {
            // if byte is the null handshake, everything is probs okay, we just
            // know we aren't at the frame portion containing alignment markers
          } else {
            // if byte of is not either null or data handshake, something went
            // wrong, assume signal lock has been lost
            modSigLockState = 0x00;
            modRxSigLockIdx = 0;
            
            // do error checking to see what our bit error rate is like
            bitErrorTemp = ((*modRxBufferCur) ^ MOD_HANDSHAKE_NULL_BYTE);
            while ( bitErrorTemp ) {
              bitErrorTemp &= (bitErrorTemp - 1);
              modRxBitErrors++;
            }
          }

          break;
        case 0:
          // byte is finished and we are not locked yet, check to see if we
          // have signal lock
          if ( ((*modRxBufferCur) == MOD_HANDSHAKE_NULL_BYTE) ) {
            if ( (++modRxSigLockIdx) == MOD_RX_SIG_LOCK_CNT ) {
              modRxSigLockIdx = 0;
              modSigLockState = 0x01;
            }
          } else {
            // reset counter since null handshakes must all be continuous
            modRxSigLockIdx = 0;
            
            // do error checking to see what our bit error rate is like
            bitErrorTemp = (*modRxBufferCur) ^ (MOD_HANDSHAKE_NULL_BYTE & 
                    MOD_HANDSHAKE_FRAME_ALIGN_BYTE);
            while ( bitErrorTemp ) {
              bitErrorTemp &= (bitErrorTemp - 1);
              modRxBitErrors++;
            }
          }

          break;
      }

      // now we need increment our index clear the next byte in the buffer,
      // do not, I repeat do not remove the clear byte command as it is stupid 
      // important in retaining the integrity of our byte buffer filling process
      if ( (++modRxBufferCur) == modRxBufferEnd ) {
        modRxBufferCur = modRxBuffer;
      }
      *modRxBufferCur = 0x00;
    }
    
    // invert nibble flag to keep track of where we are at in this byte
    modRxNibbleFlag = !modRxNibbleFlag;
  }
  
  if ( modSigLockState ) {
    //*qpLastChVSenseRegPtrs[modRxActiveQuadrant] = (*bufsNibbleStruct.qpChVSenseRegTemps[modRxActiveQuadrant]);
    switch ( modRxActiveQuadrant ) {
      case 1:
        // decide whether we should save alignment ADC channel nodes for
        // even channels
        if ( hiccupChannelValue ) {
          if ( hiccupCompareValue )
            *qpLastChVSenseRegPtrs[modRxActiveQuadrant] = (*bufsNibbleStruct.msbPartTwoReg);
          else
            *qpLastChVSenseRegPtrs[modRxActiveQuadrant] = (*bufsNibbleStruct.msbPartOneReg);
        }
        *qpLastChVSenseRegPtrs[3] = (*bufsNibbleStruct.qpChVSenseRegTemps[3]);
        // decide whether we should save alignment ADC channel nodes for
        // odd channels
        if ( msb ) {
          if ( !lsb )
            *qpLastChVSenseRegPtrs[2] = (*bufsNibbleStruct.qpChVSenseRegTemps[2]);
        } else {
          if ( lastLsb )
           *qpLastChVSenseRegPtrs[0] = (*bufsNibbleStruct.qpChVSenseRegTemps[0]);
        }
        break;
      case 3:
        // decide whether we should save alignment ADC channel nodes for
        // even channels
        if ( hiccupChannelValue ) {
          if ( hiccupCompareValue )
            *qpLastChVSenseRegPtrs[modRxActiveQuadrant] = (*bufsNibbleStruct.lsbPartTwoReg);
          else
            *qpLastChVSenseRegPtrs[modRxActiveQuadrant] = (*bufsNibbleStruct.lsbPartOneReg);
        }
        *qpLastChVSenseRegPtrs[1] = (*bufsNibbleStruct.qpChVSenseRegTemps[1]);
        // decide whether we should save alignment ADC channel nodes for
        // odd channels
        if ( msb ) {
          if ( !lsb )
            *qpLastChVSenseRegPtrs[2] = (*bufsNibbleStruct.qpChVSenseRegTemps[2]);
        } else {
          if ( lastLsb )
           *qpLastChVSenseRegPtrs[0] = (*bufsNibbleStruct.qpChVSenseRegTemps[0]);
        }
        break;
    }
  } else {
    // since we are not locked on to the signal yet, might as well not care
    // about peculiarities of sampling alignment ADC node channels
    for ( idx=0; idx<4; idx++ )
      *qpLastChVSenseRegPtrs[idx] = (*bufsNibbleStruct.qpChVSenseRegTemps[idx]);
  }
  
  // store last bits in case we need to reference them for clock recovery
  // operations in the next cycle
  lastLsb = lsb;
  
  // clear the TxIF interrupt flag bit
  //MOD_ADC_INT_FLAG &= ~MOD_ADC_INT_MASK;
  IFS1CLR = MOD_ADC_INT_MASK;
  
  return;
}