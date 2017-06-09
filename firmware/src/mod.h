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
#define MOD_FRAME_HEADER_SIZE_BYTES     2
#define MOD_FRAME_DATA_SIZE_BYTES       256
#define MOD_HANDSHAKE_NULL_BYTE         0b10100100
#define MOD_HANDSHAKE_FRAME_ALIGN_BYTE  0b01011011
#define MOD_HANDSHAKE_DATA_BYTE         0b10100111

// define default values for initial tx configuration
#define MOD_DEFAULT_TX_FREQ_HZ          243900//238000//476190//476000//238000

// define default values for initial rx configuration
#define MOD_DEFAULT_RX_ACTIVE_QUAD      1
#define MOD_DEFAULT_RX_ALIGN_ENB        1
#define MOD_DEFAULT_RX_SWAP_MULT        200    // multiplier when comparing ch
#define MOD_DEFAULT_RX_ALIGN_MULT_ONE   255
#define MOD_DEFAULT_RX_ALIGN_MULT_TWO   0
#define MOD_DEFAULT_RX_ALIGN_MULT_THREE 0
#define MOD_RX_ALIGN_RHO_MULT_L         64
#define MOD_RX_ALIGN_RHO_MULT_M         96
#define MOD_RX_ALIGN_RHO_MULT_H         255
#define MOD_RX_ALIGN_FRAME_STREAK_L     0
#define MOD_RX_ALIGN_FRAME_STREAK_M     4
#define MOD_RX_ALIGN_FRAME_STREAK_H     32
#define MOD_DEFAULT_RX_HICCUP_NS        20000   // differential in sampling (ns)
#define MOD_DEFAULT_RX_HICCUP_THRES     22

// define constants/limits relating to tx acquisition strategies 
#define MOD_TX_FREQ_HZ_MIN              3
#define MOD_TX_FREQ_HZ_MAX              10000000

// define constants/limits relating to rx acquisition strategies 
#define MOD_RX_ADC_SAMC_TAD             28  // auto-sample time (T_AD's)
#define MOD_RX_ADC_ADCS                 0   // conversion clock bits (#*2*T_PB)
#define MOD_RX_HICCUP_TAD               1   // differential in sampling (T_AD's)
#define MOD_RX_HICCUP_DIRECTION         1   // 0=neg (slower), 1=pos (faster)
#define MOD_RX_ALIGN_STEP               3
#define MOD_RX_SIG_LOCK_CNT             4
    

// variables available to external modules
extern uint8_t modState;
extern volatile uint8_t modSigLockState;
extern volatile uint8_t modRxHiccupState;
extern volatile uint8_t modRxAlignEnb;
extern uint32_t modTxFreqHz;
extern uint32_t modRxBitErrors;
extern uint32_t modRxHiccupNs;
extern volatile uint16_t modRxHiccupThresH;
extern volatile uint16_t modRxHiccupThresL;
extern volatile uint16_t modRxADCBuffer[MOD_FRAME_DATA_SIZE_BYTES];
extern volatile uint8_t modRxADCBufferIdx;

extern volatile uint32_t tickTock;//!!
extern volatile uint8_t modRxNibbleFlag;//!!
extern volatile uint16_t modRxSigLockIdx;
extern volatile uint8_t modTxSigLockFlag;//!!
extern uint8_t modRxAlignMultOne;//!!
extern uint8_t modRxAlignMultTwo;//!!
extern uint8_t modRxAlignMultThree;//!!
extern uint32_t modRxFrameStreak;
extern uint8_t modRxAlignRhoMult;
extern uint8_t modRxAlignSwapMult;
extern uint8_t modRxActiveQuadrant;
extern volatile uint8_t modRxBuffer[MOD_FRAME_SIZE_BYTES];
extern volatile uint8_t * modRxBufferCur;
extern volatile uint8_t * modTxBufferHeader;
extern volatile uint8_t * modTxBufferData;

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