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

#include <stdint.h>
#include <math.h>
#include <string.h>   // memset

#include "timer.h"
#include "qp.h"
#include "lsr.h"
#include "mod.h"
#include "gimbal.h"
#include "debug.h"

#include "usb_handler.h"

volatile uint16_t usbTxIdx;
uint16_t usbModBufferCopy[MOD_FRAME_DATA_SIZE_BYTES];
volatile uint8_t usbModBufferIdx;
volatile uint8_t usbModBufferInitialIdx;

// packet information for different tx/rx items
// "address" part of packet stores information about what is being sent,
// the commands both from and to the host will contain an address that
// identifies what the following data represents, txBytes and rxBytes defines 
// how many data bytes are sent or received for each corresponding address type
typedef struct { 
  const uint8_t address;
  const uint8_t txBytes;
  const uint8_t rxBytes;
} PktInfo;

// 0x0*
// 0x1* and 0x2* debugging commands
static const PktInfo CMD_LED1_TOG             = { 0x10,   1,        0       };
static const PktInfo CMD_LED1_SET             = { 0x11,   0,        1       };
static const PktInfo CMD_LED1_GET             = { 0x12,   1,        0       };
static const PktInfo CMD_LED2_TOG             = { 0x13,   1,        0       };
static const PktInfo CMD_LED2_SET             = { 0x14,   0,        1       };
static const PktInfo CMD_LED2_GET             = { 0x15,   1,        0       };
static const PktInfo CMD_BTN1_GET             = { 0x16,   1,        0       };
static const PktInfo CMD_BTN2_GET             = { 0x17,   1,        0       };
static const PktInfo CMD_DBG_VAL1_GET         = { 0x18,   4,        0       };
static const PktInfo CMD_DBG_VAL2_GET         = { 0x19,   4,        0       };
static const PktInfo CMD_DBG_VAL3_GET         = { 0x1A,   4,        0       };
static const PktInfo CMD_DBG_VAL4_GET         = { 0x1B,   4,        0       };
static const PktInfo CMD_DBG_INC1             = { 0x1C,   4,        0       };
static const PktInfo CMD_DBG_DEC1             = { 0x1D,   4,        0       };
static const PktInfo CMD_DBG_INC2             = { 0x1E,   4,        0       };
static const PktInfo CMD_DBG_DEC2             = { 0x1F,   4,        0       };
static const PktInfo CMD_DBG_INC3             = { 0x20,   4,        0       };
static const PktInfo CMD_DBG_DEC3             = { 0x21,   4,        0       };
static const PktInfo CMD_DBG_INC4             = { 0x22,   4,        0       };
static const PktInfo CMD_DBG_DEC4             = { 0x23,   4,        0       };
// 0x3* Ethernet commands 
// 0x4* delay commands
static const PktInfo CMD_DELAY_US             = { 0x40,   0,        2       };
static const PktInfo CMD_DELAY_MS             = { 0x41,   0,        2       };
static const PktInfo CMD_DELAY_S              = { 0x42,   0,        2       };
// 0x5* gimbal commands
static const PktInfo CMD_GIMBAL_PAN_TOG       = { 0x50,   1,        0       };
static const PktInfo CMD_GIMBAL_PAN_SET       = { 0x51,   0,        1       };
static const PktInfo CMD_GIMBAL_PAN_GET       = { 0x52,   1,        0       };
static const PktInfo CMD_GIMBAL_TILT_TOG      = { 0x53,   1,        0       };
static const PktInfo CMD_GIMBAL_TILT_SET      = { 0x54,   0,        1       };
static const PktInfo CMD_GIMBAL_TILT_GET      = { 0x55,   1,        0       };
static const PktInfo CMD_GIMBAL_PAN_ANG_SET   = { 0x56,   0,        2       };  // (2 bytes) / 360 = resolution of 0.00549deg
static const PktInfo CMD_GIMBAL_PAN_ANG_GET   = { 0x57,   2,        0       };  // (2 bytes) / 360 = resolution of 0.00549deg
static const PktInfo CMD_GIMBAL_TILT_ANG_SET  = { 0x58,   0,        2       };  // (2 bytes) / 360 = resolution of 0.00549deg
static const PktInfo CMD_GIMBAL_TILT_ANG_GET  = { 0x59,   2,        0       };  // (2 bytes) / 360 = resolution of 0.00549deg
// 0x6 quadrant photodiode activities
static const PktInfo CMD_QP_CH1_VSENSE_GET    = { 0x60,   2,        0       };
static const PktInfo CMD_QP_CH2_VSENSE_GET    = { 0x61,   2,        0       };
static const PktInfo CMD_QP_CH3_VSENSE_GET    = { 0x62,   2,        0       };
static const PktInfo CMD_QP_CH4_VSENSE_GET    = { 0x63,   2,        0       };
static const PktInfo CMD_QP_ALL_BULK_RUN      = { 0x64,   0,        4       };
static const PktInfo CMD_QP_ALL_BULK_GET      = { 0x65,   62,       0       };
// 0x7* laser activities
static const PktInfo CMD_LSR_LOAD_SWTICH_TOG  = { 0x70,   1,        0       };
static const PktInfo CMD_LSR_LOAD_SWTICH_SET  = { 0x71,   0,        1       };
static const PktInfo CMD_LSR_LOAD_SWTICH_GET  = { 0x72,   1,        0       };
static const PktInfo CMD_LSR_VREF_VSENSE_GET  = { 0x73,   2,        0       };
static const PktInfo CMD_LSR_ISENSE_GET       = { 0x74,   2,        0       };
static const PktInfo CMD_LSR_TOG              = { 0x75,   1,        0       };
static const PktInfo CMD_LSR_SET              = { 0x76,   0,        1       };
static const PktInfo CMD_LSR_GET              = { 0x77,   1,        0       };
// 0x8* and 0x9* modulation activities
static const PktInfo CMD_MOD_TOG              = { 0x80,   1,        0       };
static const PktInfo CMD_MOD_SET              = { 0x81,   0,        1       };
static const PktInfo CMD_MOD_GET              = { 0x82,   1,        0       };
static const PktInfo CMD_MOD_FREQ_HZ_SET      = { 0x83,   0,        4       };
static const PktInfo CMD_MOD_FREQ_HZ_GET      = { 0x84,   4,        0       };
static const PktInfo CMD_MOD_DATA_BULK_RUN    = { 0x86,   0,        0,      };
static const PktInfo CMD_MOD_DATA_BULK_GET    = { 0x87,   62,       0,      };
static const PktInfo CMD_MOD_SIG_LOCK_GET     = { 0x88,   1,        0,      };
static const PktInfo CMD_MOD_HICCUP_NS_GET    = { 0x89,   4,        0,      };
static const PktInfo CMD_MOD_HICCUP_NS_SET    = { 0x8A,   0,        4,      };
static const PktInfo CMD_MOD_TOG_ALIGN_ENB    = { 0x8B,   1,        0       };
static const PktInfo CMD_MOD_SET_ALIGN_ENB    = { 0x8C,   0,        1       };
static const PktInfo CMD_MOD_GET_ALIGN_ENB    = { 0x8D,   1,        0       };
// 0xA*
// 0xB*
// 0xC*
// 0xD*
// 0xE*
// 0xF* identification commands
static const PktInfo CMD_ID_MASTER_GET        = { 0xF0,   1,        0       };

void _insertTxBufUnsigned( uint8_t * txDataBuffer, uint8_t data ) {
  
  // place data in byte chunks
  txDataBuffer[usbTxIdx++] = data;
  
  return;
}

void insertTxBufUnsignedChar( uint8_t * txDataBuffer, uint8_t data ) {
  
  // place data in byte chunks
  _insertTxBufUnsigned( txDataBuffer, data );
  
  return;
}

void insertTxBufUnsignedInt( uint8_t * txDataBuffer, uint16_t data ) {
  
  char idx;
  
  // place data in byte chunks
  for ( idx=8; idx>=0; idx-=8 )
    _insertTxBufUnsigned( txDataBuffer, (uint8_t)(data>>idx) );
  
  return;
}

void insertTxBufUnsignedLong( uint8_t * txDataBuffer, uint32_t data ) {
  
  char idx;
  
  // place data in byte chunks
  for ( idx=24; idx>=0; idx-=8 )
    _insertTxBufUnsigned( txDataBuffer, (uint8_t)(data>>idx) );
  
  return;
}

void insertTxBufFloat( uint8_t * txDataBuffer, float data ) {
  // this function uses the following approach to represent a floating-point
  // number in 4 consecutive byes of memory in c using a scientific notation
  // approach (i.e. n * 10^[(+/-)x]:
  //  <----byte one----><----byte two----><---byte three---><---byte four---->
  //  <                 base number int24_t               ><+/-   exponent  >
  //    (24 bits to represent base, n)--^                     ^         ^
  //    (first bit is sign of exponent)-----------------------/         |
  //    (next seven bits represent power of ten exponent i.e 10^x)------/
  
  uint32_t base;
  uint8_t baseSign;
  uint8_t exp;
  uint8_t expSign;
  uint32_t uintEntire;
  
  float temp;
  float lastTemp;
  
  if ( data >= 0 )
    baseSign = 0;
  else
    baseSign = 0;
  data = fabs( data );
  
  if ( (data > -1) && (data < 1) ) {
    expSign = 1;
    
  } else {
    expSign = 0;
    
    lastTemp = 1;
    for ( exp=0; exp<0x80; exp++ ) {
      temp = lastTemp * 10;
      if ( temp > data )
        break;
      lastTemp = temp;
    }
    
    base = (uint32_t)(data / (temp));
  }
  
  // place data in byte chunks
  insertTxBufUnsignedLong( txDataBuffer, (base<<8) | (exp<<0) );
  
  return;
}

void usbHandler( uint8_t * rxDataBuffer, uint8_t * txDataBuffer,
    uint16_t bufSize ) {
  
  uint8_t rxDataCmd;  
  volatile uint16_t idx;
  
  // reset counter for building our tx buffer
  usbTxIdx = 0;
  memset( txDataBuffer, 0, sizeof(uint8_t)*64 );
  
  // we got a packet, check what the hosts wants us to do and do it,
  // the packet contains a command id at the beginning followed by one or
  // more data byte that is used for certain commands, so let's look at
  // these to decide what to do
  idx = 0;
  while ( idx<(bufSize) ) {

    // get next address to figure out what we should do
    rxDataCmd = (uint8_t)(rxDataBuffer[idx++]);

    // if we ran out of addresses, exit
    if ( rxDataCmd == 0 )
      break;      

    if ( rxDataCmd == CMD_DELAY_US.address ) {
      // pause a number of microseconds
      
      // use delay function to pause
      delayUs( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_DELAY_MS.address ) {
      // pause a number of milliseconds
      
      // use delay function to pause
      delayMs( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_DELAY_S.address ) {
      // pause a number of seconds
      
      // use delay function to pause
      delayS( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[(idx++)+1]) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );

    } else if ( rxDataCmd == CMD_GIMBAL_PAN_TOG.address ) {
      // toggle pan servo on and off
      
      // set servo to opposite state
      gimbalPanTog();
      
      // echo back command id to host along with laser state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(gimbalPanState) );
    
    } else if ( rxDataCmd == CMD_GIMBAL_PAN_SET.address ) {
      // set pan servo on or off
      
      // set servo to a certain state
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        gimbalPanOff();
      } else {
        gimbalPanOn();
      }
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_GIMBAL_PAN_GET.address ) {
      // get state of pan servo

      // echo back command id to host along with state data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(gimbalPanState) );
      
    } else if ( rxDataCmd == CMD_GIMBAL_TILT_TOG.address ) {
      // toggle tilt servo on and off
      
      // set servo to opposite state
      gimbalTiltTog();
      
      // echo back command id to host along with laser state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(gimbalTiltState) );
    
    } else if ( rxDataCmd == CMD_GIMBAL_TILT_SET.address ) {
      // set tilt servo on or off
      
      // set servo to a certain state
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        gimbalTiltOff();
      } else {
        gimbalTiltOn();
      }
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_GIMBAL_TILT_GET.address ) {
      // get state of tilt servo

      // echo back command id to host along with state data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(gimbalTiltState) );
        
    } else if ( rxDataCmd == CMD_GIMBAL_PAN_ANG_SET.address ) {
      // set gimbal pan angle
      
      // look at the data received to determine and the gimbal angle
      // to set, the first data byte is the msb of the angle and the 
      // second data byte is the lsb
      // this will be sent as an whole number, this number has a theoretical
      // maximum value of (2^16-1), divide this number by 360 to the angle
      // angular resolution = 360 / 2^16 = 0.0054931640625deg
      gimbalSetPan( ((float)(uint16_t)( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[idx++]) ) * 360.0 / ((float)(uint16_t)((1<<16)-1)) ) );

      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_GIMBAL_PAN_ANG_GET.address ) {
      // get gimbal pan angle

      // echo back command id to host and gimbal information
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, gimbalGetPan() / 360 * ((float)(uint16_t)((1<<16)-1)) );

    } else if ( rxDataCmd == CMD_GIMBAL_TILT_ANG_SET.address ) {
      // set gimbal tilt angle
      
      // look at the data received to determine and the gimbal angle
      // to set, the first data byte is the msb of the angle and the 
      // second data byte is the lsb
      // this will be sent as an whole number, this number has a theoretical
      // maximum value of (2^16-1), divide this number by 360 to the angle
      // angular resolution = 360 / 2^16 = 0.0054931640625deg
      gimbalSetTilt( ((float)(uint16_t)( (rxDataBuffer[idx++]<<8) | (rxDataBuffer[idx++]) ) * 360.0 / ((float)(uint16_t)((1<<16)-1)) ) );

      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
     } else if ( rxDataCmd == CMD_GIMBAL_TILT_ANG_GET.address ) {
      // get gimbal tilt angle

      // echo back command id to host and gimbal information
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, gimbalGetTilt() / 360 * ((float)(uint16_t)((1<<16)-1)) );
      
    } else if ( rxDataCmd == CMD_QP_CH1_VSENSE_GET.address ) {
      // get quadrant photodiode ADC node
      
      // read ADC channel (only if ADC is not in auto-sample mode)
      if (!modState)
        qpReadCh1VSense();
      
      // echo back command id to host along with ADC data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(qpLastCh1VSenseReg) );
      
    } else if ( rxDataCmd == CMD_QP_CH2_VSENSE_GET.address ) {
      // get quadrant photodiode ADC node
      
      // read ADC channel (only if ADC is not in auto-sample mode)
      if (!modState)
        qpReadCh2VSense();
      
      // echo back command id to host along with ADC data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(qpLastCh2VSenseReg) );
      
    } else if ( rxDataCmd == CMD_QP_CH3_VSENSE_GET.address ) {
      // get quadrant photodiode ADC node
      
      // read ADC channel (only if ADC is not in auto-sample mode)
      if (!modState)
        qpReadCh3VSense();
      
      // echo back command id to host along with ADC data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(qpLastCh3VSenseReg) );
      
    } else if ( rxDataCmd == CMD_QP_CH4_VSENSE_GET.address ) {
      // get quadrant photodiode ADC node
      
      // read ADC channel (only if ADC is not in auto-sample mode)
      if (!modState)
        qpReadCh4VSense();
    
      // echo back command id to host along with ADC data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(qpLastCh4VSenseReg) );
      
    } else if ( rxDataCmd == CMD_QP_ALL_BULK_RUN.address ) {
      // collect bulk quadrant photodiode ADC nodes
      
      // run bulk ADC collection function with desired pause between every cycle
      qpReadAllBulk( (uint32_t)(
              (rxDataBuffer[(idx++)]<<24) |
              (rxDataBuffer[(idx++)]<<16) |
              (rxDataBuffer[(idx++)]<<8) |
              (rxDataBuffer[(idx++)]<<0) ) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_QP_ALL_BULK_GET.address ) {
      // send back bulk quadrant photodiode ADC nodes
      
      // echo back command id to host along with ADC node data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      do {
        insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(qpBulkBuffer[qpBulkBufferIdx++]) );
      } while ( (qpBulkBufferIdx) % (CMD_QP_ALL_BULK_GET.txBytes/2) );
    
    } else if ( rxDataCmd == CMD_LSR_LOAD_SWTICH_TOG.address ) {
      // toggle load switch on and off
      
      // set load switch to opposite state
      lsrLoadSwitchTog();
      
      // echo back command id to host along with load switch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(lsrLoadSwitchState) );
    
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
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(lsrLoadSwitchState) );
      
    } else if ( rxDataCmd == CMD_LSR_VREF_VSENSE_GET.address ) {
      // get current limit Vref ADC node
      
      // read ADC channel (only if ADC is not in auto-sample mode)
      if (!modState)
        lsrReadVrefVSenseReg();
      
      // echo back command id to host along with vref data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(lsrLastVrefVSenseReg) );
      
    } else if ( rxDataCmd == CMD_LSR_ISENSE_GET.address ) {
      // get current limit Vref ADC node
      
      // read ADC channel (only if ADC is not in auto-sample mode)
      if (!modState)
        lsrReadISenseReg();
      
      // echo back command id to host along with vref data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(lsrLastISenseReg) );
      
    } else if ( rxDataCmd == CMD_MOD_TOG.address ) {
      // toggle modulation on and off
      
      // set modulation to opposite state
      modTog();
      
      // echo back command id to host along with load switch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(modState) );
    
    } else if ( rxDataCmd == CMD_MOD_SET.address ) {
      // set modulation on or off
      
      // set modulation to certain state
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        modOff();
      } else {
        modOn();
      }
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_MOD_GET.address ) {
      // get state of modulation
      
      // echo back command id to host along with modulation state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(modState) );
    
    } else if ( rxDataCmd == CMD_MOD_FREQ_HZ_SET.address ) {
      // set rate of modulation
      
      // set received packets as new desired modulation rate (in Hz)
      modSetFreqHz( (uint32_t)(
              (rxDataBuffer[(idx++)]<<24) |
              (rxDataBuffer[(idx++)]<<16) |
              (rxDataBuffer[(idx++)]<<8) |
              (rxDataBuffer[(idx++)]<<0) ) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_MOD_FREQ_HZ_GET.address ) {
      // get rate of modulation
      
      // echo back command id to host along with modulation rate
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modTxFreqHz) );
      
    } else if ( rxDataCmd == CMD_MOD_DATA_BULK_RUN.address ) {
      // collect bulk modulation data from photodiode ADC nodes
      
      // save pointer to current index so we know where to start
      usbModBufferIdx = 0;
      usbModBufferInitialIdx = (uint8_t)(modRxADCBufferIdx);
      
      // make deep copy of buffer so we have static copy
      memcpy( usbModBufferCopy, (uint16_t *)(modRxADCBuffer), sizeof( modRxADCBuffer ) );
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_MOD_DATA_BULK_GET.address ) {
      // send back bulk modulation data from photodiode ADC node
      
      // echo back command id to host along with ADC node data
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      do {
        insertTxBufUnsignedInt( txDataBuffer, (uint16_t)(usbModBufferCopy[(uint8_t)(usbModBufferInitialIdx + (usbModBufferIdx++))]) );
      } while ( (usbModBufferIdx) % (CMD_MOD_DATA_BULK_GET.txBytes/2) );
    
    } else if ( rxDataCmd == CMD_MOD_SIG_LOCK_GET.address ) {
      // get state of signal lock
      
      // echo back command id to host along with signal lock state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(modSigLockState) );
    
    } else if ( rxDataCmd == CMD_MOD_HICCUP_NS_SET.address ) {
      // set timing of clock recovery hiccup
      
      // first turn off modulation (can't change settings while active)
      modOff();
      
      // set received packets as new desired hiccup timing (in nanoseconds)
      modRxHiccupNs = (uint32_t)(
              (rxDataBuffer[(idx++)]<<24) |
              (rxDataBuffer[(idx++)]<<16) |
              (rxDataBuffer[(idx++)]<<8) |
              (rxDataBuffer[(idx++)]<<0) );
      
      // turn modulation back on
      modOn();
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
    
    } else if ( rxDataCmd == CMD_MOD_HICCUP_NS_GET.address ) {
      // get timing of clock recovery hiccup
      
      // echo back command id to host along with hiccup timing
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxHiccupNs) );
      
    } else if ( rxDataCmd == CMD_MOD_TOG_ALIGN_ENB.address ) {
      // toggle modulation alignment on and off
      
      // set modulation  alignment enable to opposite state
      if ( modRxAlignEnb )
        modRxAlignEnb = 0x00;
      else
        modRxAlignEnb = 0x01;
      
      // echo back command id to host along with alignment enable state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(modRxAlignEnb) );
    
    } else if ( rxDataCmd == CMD_MOD_SET_ALIGN_ENB.address ) {
      // set modulation alignment on or off
      
      // set modulation alignment enable to certain state
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        modRxAlignEnb = 0x00;
      } else {
        modRxAlignEnb = 0x01;
      }
      
      // echo back command id to host
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      
    } else if ( rxDataCmd == CMD_MOD_GET_ALIGN_ENB.address ) {
      // get state of modulation alignment
      
      // echo back command id to host along with alignment enable state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(modRxAlignEnb) );
      
    } else if ( rxDataCmd == CMD_LSR_TOG.address ) {
      // toggle load switch on and off
      
      // set laser to opposite state
      lsrTog();
      
      // echo back command id to host along with laser state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(lsrEnCh1State) );
    
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
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(lsrEnCh1State) );
    
    } else if ( rxDataCmd == CMD_LED1_TOG.address ) {
      // toggle debug led on and off
      SERVO_PAN_OCRS = SERVO_PAN_OCR + 1;//!!
      // toggle output latch of debug led
      debugLed1Tog();

      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)((DEBUG_LED1_LAT&DEBUG_LED1_MASK)>0) );

    } else if ( rxDataCmd == CMD_LED1_SET.address ) {
      // set debug led to specific value
      
      // set the debug led on or off depending on input
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        debugLed1Off();
      } else {
        debugLed1On();
      }

      // echo back command id to host along with current led latch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );

    } else if ( rxDataCmd == CMD_LED1_GET.address ) {
      // get state of debug led
      
      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)((DEBUG_LED1_LAT&DEBUG_LED1_MASK)>0) );
      
    } else if ( rxDataCmd == CMD_LED2_TOG.address ) {
      // toggle debug led on and off
      SERVO_TILT_OCRS = SERVO_TILT_OCR + 1;//!!
      // toggle output latch of debug led
      debugLed2Tog();
      
      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)((DEBUG_LED2_LAT&DEBUG_LED2_MASK)>0) );

    } else if ( rxDataCmd == CMD_LED2_SET.address ) {
      // set debug led to specific value

      // set the debug led on or off depending on input
      if ( (rxDataBuffer[idx++]<<0) == 0 ) {
        debugLed2Off();
      } else {
        debugLed2On();
      }

      // echo back command id to host along with current led latch state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );

    } else if ( rxDataCmd == CMD_LED2_GET.address ) {
      // get state of debug led

      // echo back command id to host along with LED state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)((DEBUG_LED2_LAT&DEBUG_LED2_MASK)>0) );
      
    } else if ( rxDataCmd == CMD_BTN1_GET.address ) {
      // get state of debug button

      // echo back command id to host along with button state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(debugBtn1State()>0) );

    } else if ( rxDataCmd == CMD_BTN2_GET.address ) {
      // get state of debug button

      // echo back command id to host along with button state
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(debugBtn2State()>0) );
    
    } else if ( rxDataCmd == CMD_ID_MASTER_GET.address ) {
      // get id of this device
      
      // echo back command id to host along with device id
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      #ifdef MASTER
        insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(0x01) );
      #else
        insertTxBufUnsignedChar( txDataBuffer, (uint8_t)(0x00) );
      #endif
      
    } else if ( rxDataCmd == CMD_DBG_VAL1_GET.address ) {
      // send value back for troubleshooting

      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxBitErrors) );
    
    } else if ( rxDataCmd == CMD_DBG_VAL2_GET.address ) {
      // send value back for troubleshooting
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxFrameStreak) );
      
    } else if ( rxDataCmd == CMD_DBG_VAL3_GET.address ) {
      // send value back for troubleshooting
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignRhoMult) );
      
    } else if ( rxDataCmd == CMD_DBG_VAL4_GET.address ) {
      // send value back for troubleshooting
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(debugVal4) );
      
    } else if ( rxDataCmd == CMD_DBG_INC1.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignMultOne += 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignMultOne) );
      
    } else if ( rxDataCmd == CMD_DBG_DEC1.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignMultOne -= 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignMultOne) );
      
    } else if ( rxDataCmd == CMD_DBG_INC2.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignMultTwo += 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignMultTwo) );
      
    } else if ( rxDataCmd == CMD_DBG_DEC2.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignMultTwo -= 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignMultTwo) );
      
    } else if ( rxDataCmd == CMD_DBG_INC3.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignMultThree += 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignMultThree) );
      
    } else if ( rxDataCmd == CMD_DBG_DEC3.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignMultThree -= 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignMultThree) );
      
    } else if ( rxDataCmd == CMD_DBG_INC4.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignSwapMult += 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignSwapMult) );
      
    } else if ( rxDataCmd == CMD_DBG_DEC4.address ) {
      // increment/decrement value for troubleshooting
      
      modRxAlignSwapMult -= 13;
      
      // echo back command id to host along with value
      insertTxBufUnsignedChar( txDataBuffer, rxDataCmd );
      insertTxBufUnsignedLong( txDataBuffer, (uint32_t)(modRxAlignSwapMult) );
      
    } else {
      // we can't match this to anything corresponding to our command addresses...

      // flash LED b/c we are confused
      debugLed1Tog();
      delayMs( 250 );
      debugLed1Tog();
      delayMs( 250 );
      debugLed1Tog();
      delayMs( 250 );
      debugLed1Tog();
    }
  }

  return;
}
