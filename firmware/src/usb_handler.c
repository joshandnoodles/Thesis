/* 
 * File:        usbHandler.c
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  The following file contains the main receiver and transmitter handler 
 *  functions for processing USB HID communication buffers. These contain the 
 *  functionality to break down received buffers as well as build new ones to be 
 *  sent to the USB host. Any core USB operations should be externalized in 
 *  other files. 
 */

#include <p32mx460f512l.h>
#include "hardware_config.h"

#include <string.h>   // memset

#include "lsr.h"
#include "debug.h"

#include "usb_handler.h"

unsigned int txIdx;

// packet information for different tx/rx items
// "address" part of packet stores information about what is being sent,
// the commands both from and to the host will contain an address that
// identifies what the following data represents, txBytes and rxBytes defines 
// how many data bytes are sent or received for each corresponding address type
typedef struct { 
  const unsigned char address;
  const unsigned char txBytes;
  const unsigned char rxBytes;
} PktInfo;

// 0x1* debugging commands
static const PktInfo CMD_LED1_TOG             = { 0x10,   1,        0       };
static const PktInfo CMD_LED1_SET             = { 0x11,   0,        1       };
static const PktInfo CMD_LED1_GET             = { 0x12,   1,        0       };
static const PktInfo CMD_LED2_TOG             = { 0x13,   1,        0       };
static const PktInfo CMD_LED2_SET             = { 0x14,   0,        1       };
static const PktInfo CMD_LED2_GET             = { 0x15,   1,        0       };
static const PktInfo CMD_BTN1_GET             = { 0x16,   1,        0       };
static const PktInfo CMD_BTN2_GET             = { 0x17,   1,        0       };
// 0x2* 
// 0x3* Ethernet commands 
// 0x4* delay commands
static const PktInfo CMD_DELAY_US             = { 0x40,   0,        2       };
static const PktInfo CMD_DELAY_MS             = { 0x41,   0,        2       };
static const PktInfo CMD_DELAY_S              = { 0x42,   0,        2       };

// 0x5* gimbal commands
static const PktInfo CMD_PAN_SET              = { 0x60,   0,        2       };  // (2 bytes) / 360 = resolution of 0.00549deg
static const PktInfo CMD_PAN_GET              = { 0x61,   2,        0       };  // (2 bytes) / 360 = resolution of 0.00549deg
static const PktInfo CMD_TILT_SET             = { 0x62,   0,        2       };  // (2 bytes) / 360 = resolution of 0.00549deg
static const PktInfo CMD_TILT_GET             = { 0x63,   2,        0       };  // (2 bytes) / 360 = resolution of 0.00549deg
// 0x6* 
// 0x7* laser activities
static const PktInfo CMD_LSR_LOAD_SWTICH_TOG  = { 0x70,   1,        0       };
static const PktInfo CMD_LSR_LOAD_SWTICH_SET  = { 0x71,   0,        1       };
static const PktInfo CMD_LSR_LOAD_SWTICH_GET  = { 0x72,   1,        0       };
static const PktInfo CMD_LSR_VREF_VSENSE_GET  = { 0x73,   2,        0       };
static const PktInfo CMD_LSR_ISENSE_GET       = { 0x74,   2,        0       };
static const PktInfo CMD_LSR_TOG              = { 0x75,   1,        0       };
static const PktInfo CMD_LSR_SET              = { 0x76,   0,        1       };
static const PktInfo CMD_LSR_GET              = { 0x77,   1,        0       };
// 0x8* 
// 0x9* 
// 0xA*
// 0xB*
// 0xC*
// 0xD*
// 0xE*

void _insertTxBufUnsigned( unsigned char * txDataBuffer, unsigned char data ) {
  
  // place data in byte chunks
  txDataBuffer[txIdx++] = data;
  
  return;
}

void insertTxBufUnsignedChar( unsigned char * txDataBuffer, unsigned char data ) {
  
  // place data in byte chunks
  _insertTxBufUnsigned( txDataBuffer, data );
  
  return;
}

void insertTxBufUnsignedInt( unsigned char * txDataBuffer, unsigned int data ) {
  
  char idx;
  
  // place data in byte chunks
  for ( idx=8; idx>=0; idx-=8 )
    _insertTxBufUnsigned( txDataBuffer, (unsigned char)(data>>idx) );
  
  return;
}

void insertTxBufUnsignedLong( unsigned char * txDataBuffer, unsigned long data ) {
  
  char idx;
  
  // place data in byte chunks
  for ( idx=24; idx>=0; idx-=8 )
    _insertTxBufUnsigned( txDataBuffer, (unsigned char)(data>>idx) );
  
  return;
}

void usbHandler( unsigned char * rxDataBuffer, unsigned char * txDataBuffer,
    unsigned int bufSize ) {
  
  unsigned char rxDataCmd;  
  unsigned int idx;
  
    
  // reset counter for building our tx buffer
  txIdx = 0;
  memset( &txDataBuffer[0], 0, sizeof(txDataBuffer) );
    
  // we got a packet, check what the hosts wants us to do and do it,
  // the packet contains a command id at the beginning followed by one or
  // more data byte that is used for certain commands, so let's look at
  // these to decide what to do
  idx = 0;
  while ( idx<(bufSize) ) {

    // get next address to figure out what we should do
    rxDataCmd = (unsigned char)(rxDataBuffer[idx++]);

    // if we ran out of addresses, exit
    if ( rxDataCmd == 0 )
      break;      

    if ( rxDataCmd == CMD_DELAY_US.address ) {
      // pause a number of microseconds
      
      // use delay function to pause
      //!!delayUs( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_DELAY_MS.address ) {
      // pause a number of milliseconds
      
      // use delay function to pause
      //!!delayMs( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_DELAY_S.address ) {
      // pause a number of seconds
      
      // use delay function to pause
      //!!delayMs( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    /*} else if ( rxDataCmd == CMD_PAN_SET.address ) {
      // set gimbal pan angle

      // look at the data received to determine and the gimbal angle
      // to set, the first data byte is the msb of the angle and the 
      // second data byte is the lsb
      // this will be sent as an whole number, this number has a theoretical
      // maximum value of (2^16-1), divide this number by 360 to the angle
      // angular resolution = 360 / 2^16 = 0.0054931640625deg
      //!!setGimbalPan( ((float)(unsigned int)( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) )) * 360 / ((float)(unsigned int)((1<<16)-1)) );

      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );

    } else if ( rxDataCmd == CMD_PAN_GET.address ) {
      // get gimbal pan angle

      // echo back command id to host and gimbal information
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      //!!insertTxBufUnsignedInt( txDataBuffer, getGimbalPan() / 360 * ((float)(unsigned int)((1<<16)-1)) );

    } else if ( rxDataCmd == CMD_TILT_SET.address ) {
      // set gimbal pan angle

      // look at the data received to determine and the gimbal angle
      // to set, the first data byte is the msb of the angle and the 
      // second data byte is the lsb
      // this will be sent as an whole number, this number has a theoretical
      // maximum value of (2^16-1), divide this number by 360 to the angle
      // angular resolution = 360 / 2^16 = 0.0054931640625deg
      //!!setGimbalTilt( ((float)(unsigned int)( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) )) * 360 / ((float)(unsigned int)((1<<16)-1)) );

      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_TILT_GET.address ) {
      // get gimbal tilt angle

      // echo back command id to host and gimbal information
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      //!!insertTxBufUnsignedInt( txDataBuffer, getGimbalTilt() / 360 * ((float)(unsigned int)((1<<16)-1)) );
    */
    } else if ( rxDataCmd == CMD_LSR_LOAD_SWTICH_TOG.address ) {
      // toggle load switch on and off
      
      // set load switch to opposite state
      lsrLoadSwitchTog();
      
      // echo back command id to host along with load switch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)(lsrLoadSwitchState) );
    
    } else if ( rxDataCmd == CMD_LSR_LOAD_SWTICH_SET.address ) {
      // set load switch on or off
      
      // set load switch to certain state
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        lsrLoadSwitchOff();
      } else {
        lsrLoadSwitchOn();
      }
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_LSR_LOAD_SWTICH_GET.address ) {
      // get state of load switch
      
      // echo back command id to host along with load switch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)(lsrLoadSwitchState) );
      
    } else if ( rxDataCmd == CMD_LSR_VREF_VSENSE_GET.address ) {
      // get current limit Vref ADC node
      
      // echo back command id to host along with vref data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (unsigned int)(lsrLastVrefVSenseReg) );
      
    } else if ( rxDataCmd == CMD_LSR_ISENSE_GET.address ) {
      // get current limit Vref ADC node
      
      // echo back command id to host along with vref data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (unsigned int)(lsrLastISenseReg) );
      
    
    /*} else if ( rxDataCmd == CMD_LSR_TOG.address ) {
      // set laser on/off

      // set laser to a certain power level
      //!!if ( getLaserPower() )
      //!!  setLaserPower( 0 );
      //!!else
      //!!  setLaserPower( 30 );

      // echo back command id to host along with laser power data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      //!!insertTxBufUnsignedChar( txDataBuffer, getLaserPower() );
      
    } else if ( rxDataCmd == CMD_LSR_SET.address ) {
      // set laser to specific value

      // set laser to a certain power level
      //!!setLaserPower( rxDataBuffer[idx++]<<0 );

      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    */
    
    } else if ( rxDataCmd == CMD_LSR_TOG.address ) {
      // toggle load switch on and off
      
      // set laser to opposite state
      lsrTog();
      
      // echo back command id to host along with laser state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)(lsrEnCh1State) );
    
    } else if ( rxDataCmd == CMD_LSR_SET.address ) {
      // set laser on or off
      
      // set laser to a certain state
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        lsrSetLow();
      } else {
        lsrSetHigh();
      }
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_LSR_GET.address ) {
      // get state of laser

      // echo back command id to host along with laser power data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)(lsrEnCh1State) );
    
    } else if ( rxDataCmd == CMD_LED1_TOG.address ) {
      // toggle debug led on and off

      // toggle output latch of debug led
      debugLed1Tog();

      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)((DEBUG_LED1_LAT&DEBUG_LED1_MASK)>0) );

    } else if ( rxDataCmd == CMD_LED1_SET.address ) {
      // set debug led to specific value

      // set the debug led on or off depending on input
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        debugLed1On();
      } else {
        debugLed1Off();
      }

      // echo back command id to host along with current led latch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );

    } else if ( rxDataCmd == CMD_LED1_GET.address ) {
      // get state of debug led

      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)((DEBUG_LED1_LAT&DEBUG_LED1_MASK)>0) );
      
    } else if ( rxDataCmd == CMD_LED2_TOG.address ) {
      // toggle debug led on and off

      // toggle output latch of debug led
      debugLed2Tog();

      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)((DEBUG_LED2_LAT&DEBUG_LED2_MASK)>0) );

    } else if ( rxDataCmd == CMD_LED2_SET.address ) {
      // set debug led to specific value

      // set the debug led on or off depending on input
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        debugLed2On();
      } else {
        debugLed2Off();
      }

      // echo back command id to host along with current led latch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );

    } else if ( rxDataCmd == CMD_LED2_GET.address ) {
      // get state of debug led

      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)((DEBUG_LED2_LAT&DEBUG_LED2_MASK)>0) );
      
    } else if ( rxDataCmd == CMD_BTN1_GET.address ) {
      // get state of debug button

      // echo back command id to host along with button state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)(debugBtn1State()>0) );

    } else if ( rxDataCmd == CMD_BTN2_GET.address ) {
      // get state of debug button

      // echo back command id to host along with button state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (unsigned char)(debugBtn2State()>0) );
      
    } else {
      // we can't match this to anything corresponding to our command addresses...

      // flash LED b/c we are confused
      debugLed1Tog();
      //delayMs(500)!!
      debugLed1Tog();

    }

  }

  return;
}
