/* 
 * File:        hardware_config.h
 * Author:      Josh
 * Created:     January 29, 2017, 08:58 PM
 * Revisions:
 *  2017-01-29  initial release
 * Description: 
 *  Specifications of hardware dependent items. This includes pins, registers,
 *  and oscillators of MCU used in project.
 *  For this project, the hardware specifications are the following:
 *    - MikroElektronika Clicker 2 PIC32 Dev Brd (using the PIC32MX460F512L MCU)
 *    - optics_r001 (custom optics board for MS project)
 */

#ifndef HARDWARE_CONFIG_H
#define	HARDWARE_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

// clock speed characteristic
#define FOSC            (80000000ul)            // main osicallator freq in Hz
#define FCY             (FOSC/2)                // effective execution freq in Hz
#define FPB             (FOSC/4)                // peripheral clock freq in Hz

// lsr.h configuration

// hardware configuration for laser load switch
#define LSR_LOAD_SWITCH_LAT     LATD            // port latch register
#define LSR_LOAD_SWITCH_TRIS    TRISD           // port TRIS I/O buffer register
#define LSR_LOAD_SWITCH_MASK    (1<<5)          // RD5

// hardware configuration for laser CH1 EN
#define LSR_EN_CH1_LAT          LATD            // port latch register
#define LSR_EN_CH1_TRIS         TRISD           // port TRIS I/O buffer register
#define LSR_EN_CH1_MASK         (1<<12)         // RD12

// hardware configuration for laser CH2 EN
#define LSR_EN_CH2_LAT          LATD            // port latch register
#define LSR_EN_CH2_TRIS         TRISD           // port TRIS I/O buffer register
#define LSR_EN_CH2_MASK         (1<<13)         // RD13

// hardware configuration for laser current limit Vref VSENSE
#define LSR_VREF_VSENSE_TRIS    TRISB           // port TRIS I/O buffer register
#define LSR_VREF_VSENSE_MASK    (1<<8)          // RB8
#define LSR_VREF_VSENSE_AN_BUF  ADC1BUF0        // ADC result buffer register
#define LSR_VREF_VSENSE_AN_MASK (1<<8)          // AN8
#define LSR_VREF_VSENSE_AN_CH   8               // channel index of ADC port


// hardware configuration for laser current low-side sense //!! add analog config register
#define LSR_ISENSE_LS_TRIS      TRISB           // port TRIS I/O buffer register
#define LSR_ISENSE_LS_MASK      (1<<9)          // RB9
#define LSR_ISENSE_LS_AN_BUF    ADC1BUF0        // ADC result buffer register
#define LSR_ISENSE_LS_AN_MASK   (1<<9)          // AN9
#define LSR_ISENSE_LS_AN_CH     9               // channel index of ADC port
    
// hardware configuration for laser current high-side sense //!! add analog config register
#define LSR_ISENSE_HS_TRIS      TRISB           // port TRIS I/O buffer register
#define LSR_ISENSE_HS_MASK      (1<<10)         // RB10
#define LSR_ISENSE_HS_AN_BUF    ADC1BUF0        // ADC result buffer register
#define LSR_ISENSE_HS_AN_MASK   (1<<10)         // AN10
#define LSR_ISENSE_HS_AN_CH     10              // channel index of ADC port

// debug.h configuration

// hardware configuration for debug LED1
#define DEBUG_LED1_LAT      LATE                // port latch register
#define DEBUG_LED1_TRIS     TRISE               // port TRIS I/O buffer register
#define DEBUG_LED1_PORT     PORTE               // port digitital I/O pin (for reading)
#define DEBUG_LED1_MASK     (1<<3)              // RE3

// hardware configuration for debug LED2
#define DEBUG_LED2_LAT      LATA                // port latch register
#define DEBUG_LED2_TRIS     TRISA               // port TRIS I/O buffer register
#define DEBUG_LED2_PORT     PORTA               // port digitital I/O pin (for reading)
#define DEBUG_LED2_MASK     (1<<10)             // RA10

// hardware configuration for debug BTN1
#define DEBUG_BTN1_LAT      LATE                // port latch register
#define DEBUG_BTN1_TRIS     TRISE               // port TRIS I/O buffer register
#define DEBUG_BTN1_PORT     PORTE               // port digitital I/O pin (for reading)
#define DEBUG_BTN1_MASK     (1<<4)              // RE4

// hardware configuration for debug BTN1
#define DEBUG_BTN2_LAT      LATG                // port latch register
#define DEBUG_BTN2_TRIS     TRISG               // port TRIS I/O buffer register
#define DEBUG_BTN2_PORT     PORTG               // port digitital I/O pin (for reading)
#define DEBUG_BTN2_MASK     (1<<9)              // RG9

/*
// hardware configuration for pan servo
#define SERVOX_PPS          RPOR1               // PPS output register
#define SERVOX_PPS_MASK     (0b00111111<<0)     // RP2Rx, pin 6
#define SERVOX_PPS_OUT_FUNC (13<<0)             // use Output Compare/PWM 1 (OC1)
#define SERVOX_OCxCON1      OC1CON1             // output compare control reg 1
#define SERVOX_OCxCON2      OC1CON2             // output compare control reg 2
#define SERVOX_OCxRS        OC1RS               // PWM period control reg
#define SERVOX_OCxR         OC1R                // PWM duty cycle control reg
#define SERVOX_DUTY_MIN     1350
#define SERVOX_DUTY_MAX     4800
#define SERVOX_DUTY_RNG     SERVOX_DUTY_MAX-SERVOX_DUTY_MIN
#define SERVOX_ANGLE_RNG    180                 //  assuming a 180 degree servo

// hardware configuration for tilt servo
#define SERVOY_PPS          RPOR1               // PPS output register
#define SERVOY_PPS_MASK     (0b00111111<<8)     // RP3Rx, pin 7
#define SERVOY_PPS_OUT_FUNC (14<<8)             // use Output Compare/PWM 2 (OC2)
#define SERVOY_OCxCON1      OC2CON1             // output compare control reg 1
#define SERVOY_OCxCON2      OC2CON2             // output compare control reg 2
#define SERVOY_OCxRS        OC2RS               // PWM period control reg
#define SERVOY_OCxR         OC2R                // PWM duty cycle control reg
#define SERVOY_DUTY_MIN     1250
#define SERVOY_DUTY_MAX     4900
#define SERVOY_DUTY_RNG     SERVOY_DUTY_MAX-SERVOY_DUTY_MIN
#define SERVOY_ANGLE_RNG    180                 //  assuming a 180 degree servo
*/

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_CONFIG_H */