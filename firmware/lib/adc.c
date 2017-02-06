/* 
 * File:        adc.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include "adc.h"

// variables to keep track ADC buffer result registers and currently enabled
// ADC channels
volatile unsigned int * adcBufRegArr[ADC_CHANNELS];
unsigned int adcMask;

void initAdc( void ) {
  
  // create a mask represents all AN ADC channels This may be useful for 
  // several occasions but mainly to use in the ADC configuration registers
  adcMask = 0x0000;
  
  // configure AD1CON1 ADC control register 1
  
  // zero out register to begin configuration
  AD1CON1 = 0x0000;
  
  // ON: ADC Operating Mode bit(1)
  // keep ADC off during configuration
  AD1CON1 |= 0b0<<15;
  
  // FORM<2:0>: Data Output Format bits
  // output data as 16-bit integer
  // (DOUT = 0000 0000 0000 0000 0000 00dd dddd dddd)
  AD1CON1 |= 0b000<<8;
  
  // SSRC<2:0>: Conversion Trigger Source Select bits
  // set to auto convert after sampling (duration determined by internal counter)
  AD1CON1 |= 0b111<<5;
  
  // ASAM: ADC Sample Auto-Start bit
  // turn off auto sample, we want control of when sampling starts
  AD1CON1 |= 0b0<<2;
  
  // configure AD1CON2 ADC control register 2
  
  // zero out register to begin configuration
  AD1CON2 = 0x0000;
  
  // CSCNA: Scan Input Selections for CH0+ SHA Input for MUX A Input
  // Multiplexer Setting bit
  // do not scan inputs
  AD1CON2 |= 0b0<<10;
  
  // SMPI<3:0>: Sample/Convert Sequences Per Interrupt Selection bits
  // sent an interrupt every 4th sample/convert sequence
  AD1CON2 |= 0b0011<<2;
  
  // ALTS: Alternate Input Sample Mode Select bit
  // do not alternate between MUX A and MUX B every sample
  AD1CON2 |= 0b0<<0;
  
  // configure AD1CON3 ADC control register 3
  
  // zero out register to begin configuration
  AD1CON3 = 0x0000;
  
  // the SAMC and ADCS registers are key to achieving 1Msps ADC operation, the 
  // specifications to achieve this are as follows:
  //  1/1Msps = 1us + (convert time)
  //  Minimum Sample time >= 1 T_AD
  //  Minimum convert time = 12 T_AD
  // the implementation and verification of timing is below
  //  T_PB = (1/80MHz)*2 = 25ns
  //  T_AD = 2 * T_PB = 50ns
  //  Sample time = 8 T_AD
  //  Convert time = 12 T_AD
  //  ADC clock divisor = 1
  //  Sample rate = 1/(2*(12+8)*25ns) = 1Msps 
  
  // SAMC<4:0>: Auto-sample Time bits
  AD1CON3 |= 8<<0;
  
  // ADCS<7:0>: ADC Conversion Clock Select bits(1)
  // T_AD = T_PB * 2 * (ADCS<7:0>+1) = 2 * T_PB
  AD1CON3 |= 0<<0;
  
  // configure AD1CHS ADC input select register
  
  // zero out register to begin configuration
  AD1CHS = 0x0000;
  
  // CH0SB<3:0>: Positive Input Select bits for MUX B
  // set AN0 as MUX B Channel 0 positive input 
  AD1CHS |= 0<<24;
  
  // CH0SA<3:0>: Positive Input Select bits for MUX A Multiplexer Setting 
  // set AN0 as MUX A Channel 0 positive input 
  AD1CHS |= 0<<16;
  
  // configure AD1PCFG ADC port configuration register
  
  // one out register to begin configuration
  AD1PCFG = 0xFFFF;
  
  // PCFG<15:0>: Analog Input Pin Configuration Control bits
  // this register will be left alone for now, however, any channels on the ADC
  // that need to be functional should be initialized using the 'adcInitCh'
  // function
  AD1PCFG &= ~(0x0000);
  
  // configure AD1CSSL ADC input scan select register
  
  // zero out register to begin configuration
  // set the following pins for input scan:
  // AN11, AN13, AN14, AN15
  // !! move to QP file (not generic enough to be here)
  AD1CSSL |= 0b1110100000000000;
  
  return;
}

void adcInitCh(
        volatile unsigned int * buf, 
        unsigned int mask,
        volatile unsigned int * tris,
        unsigned int trisMask ) {
  
  unsigned int idx;
  
  // ON: ADC Operating Mode bit(1)
  // turn ADC module off, it is not recommend to write to configuration
  // registers while ADC module is on
  AD1CON1 &= ~(0b1<<15);
  
  // update mask representing all enabled AN ADC channels
  adcMask |= mask;
  
  // add ADC buffer result register to array for tracking
  for ( idx=0; idx<ADC_CHANNELS; idx++ ) {
    if ( (mask>>idx) & 0x1 ) {
      adcBufRegArr[idx] = buf;
      break;
    }
  }
  
  // set tri-state register to make pin an input
  *tris |= trisMask;
  
  // PCFG<15:0>: Analog Input Pin Configuration Control bits
  // update analog pin configuration for newly added channel
  AD1PCFG &= ~(mask);
  
  // ON: ADC Operating Mode bit(1)
  // turn ADC module back on
  AD1CON1 |= 0b1<<15;
  
  return;
}

volatile unsigned int * adcRead( unsigned char ch ) {
  
  // CH0SA<3:0>: Positive Input Select bits for MUX A Multiplexer Setting 
  // set AN<ch> as MUX A Channel 0 positive input 
  AD1CHS |= ch<<16;
  
  // SAMP: ADC Sample Enable bit
  // start ADC SHA sampling
  AD1CON1 |= 0b1<<1;
  
  // DONE: Analog-to-Digital Conversion Status bit(2)
  // wait for conversion to complete
  while ( !(AD1CON1 & 0x0001) );
  
  // return buffer register
  return adcBufRegArr[ch];
}

float adcRegToFloat( unsigned int reg ) {
  // convert register to decimal value using knowledge of reference voltage and
  // ADC bit resolution
  return reg * ADC_REF_V / ((1<<ADC_BITS)-1);
}