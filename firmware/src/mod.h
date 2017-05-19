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
#define MOD_FRAME_SIZE_BYTES            MOD_FRAME_HEADER_SIZE_BYTES + MOD_FRAME_DATA_SIZE_BYTES
#define MOD_FRAME_HEADER_SIZE_BYTES     1
#define MOD_FRAME_DATA_SIZE_BYTES       256
#define MOD_FRAME_NULL_HANDSHAKE_BYTE   0b10100000
#define MOD_FRAME_DATA_HANDSHAKE_BYTE   0b10100010

// define default values for initial tx configuration
#define MOD_DEFAULT_TX_FREQ_HZ          476190//476000//238000

// define default values for initial rx configuration
#define MOD_DEFAULT_RX_ACTIVE_QUAD      2
#define MOD_DEFAULT_RX_HICCUP_NS        10000ul // differential in sampling (ns)
#define MOD_DEFAULT_RX_HICCUP_THRES     22

// define constants/limits relating to tx acquisition strategies 
#define MOD_TX_FREQ_HZ_MIN              3
#define MOD_TX_FREQ_HZ_MAX              10000000

// define constants/limits relating to rx acquisition strategies 
#define MOD_RX_HICCUP_TADS              1   // differential in sampling (T_AD's)
#define MOD_RX_HICCUP_DIRECTION         1   // 0=neg (slower), 1=pos (faster)
#define MOD_RX_SIG_LOCK_CNT             4

// variables available to external modules
extern uint8_t modState;
extern volatile uint8_t modSigLockState;
extern volatile uint8_t modRxHiccupState;
extern uint32_t modTxFreqHz;
extern uint32_t modRxBitErrors;
extern uint32_t modRxHiccupNs;
extern volatile uint16_t modRxHiccupThresH;
extern volatile uint16_t modRxHiccupThresL;
extern volatile uint16_t modRxADCBuffer[MOD_FRAME_DATA_SIZE_BYTES];
extern volatile uint8_t modRxADCBufferIdx;

extern volatile uint8_t modRxBuffer[MOD_FRAME_SIZE_BYTES];
extern volatile uint8_t * modRxBufferCur;
extern volatile uint8_t * modTxBufferData;
extern volatile uint8_t modTxBufferBitMask;
extern volatile uint8_t modTxBufferEighthOfNibbleFlag;

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