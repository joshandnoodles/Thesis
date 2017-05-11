/* 
 * File:        mod.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef MOD_H
#define	MOD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// define framing constants for synchronization of tx/rx acquisition strategies
#define MOD_FRAME_HEADER_SIZE_BYTES     2
#define MOD_FRAME_DATA_SIZE_BYTES       256
#define MOD_FRAME_NULL_HANDSHAKE_BYTE   0b10100000
#define MOD_FRAME_DATA_DHANDSHAKE_BYTE  0b11111111

// define default values for initial tx configuration
#define MOD_DEFAULT_TX_FREQ_HZ          476190//476000//238000

// define default values for initial rx configuration
#define MOD_DEFAULT_RX_ACTIVE_QUAD      2
#define MOD_DEFAULT_RX_HICCUP_THRES     22

// define constants/limints relating to tx acquisition strategies 
#define MOD_TX_FREQ_HZ_MIN              3
#define MOD_TX_FREQ_HZ_MAX              10000000

// define constants relating to rx acquisition strategies 
#define MOD_RX_HICCUP_TADS              1   // differential in sampling (T_AD's)
#define MOD_RX_HICCUP_DIRECTION         1   // 0=neg (slower), 1=pos (faster)
#define MOD_RX_HICCUP_NS                10735// differential in sampling (nanosec)

// variables available to external modules
extern uint8_t modState;
extern uint8_t modSigLockState;
extern uint8_t modRxHiccupState;
extern uint32_t modTxFreqHz;
extern volatile uint16_t modTxDataBufferIdx;
extern uint32_t modRxBitErrors;
extern uint16_t modRxHiccupThres;
extern volatile uint8_t modRxHeaderBuffer[MOD_FRAME_HEADER_SIZE_BYTES];
extern volatile uint8_t modRxDataBuffer[MOD_FRAME_DATA_SIZE_BYTES];
extern volatile uint16_t modRxADCBuffer[MOD_FRAME_DATA_SIZE_BYTES];
extern volatile uint16_t modRxHeaderBufferIdx;
extern volatile uint16_t modRxDataBufferIdx;
extern volatile uint8_t modRxADCBufferIdx;

// function prototypes
void initMod( void );
uint8_t modOn( void );
uint8_t modOff( void );
uint8_t modTog( void );
uint8_t modSetFreqHz( uint32_t );

#ifdef	__cplusplus
}
#endif

#endif	/* MOD_H */