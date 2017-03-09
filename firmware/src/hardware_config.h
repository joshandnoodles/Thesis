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
#define FCY             (FOSC/2)                // execution freq in Hz
#define FPB             (FOSC/2)                // peripheral clock freq in Hz

// qp.h configuration

// hardware configuration for quadrant photodiode channel 1 VSense
#define QP_CH1_VSENSE_TRIS      TRISB           // port TRIS I/O buffer register
#define QP_CH1_VSENSE_MASK      (1<<11)         // RB11
#define QP_CH1_VSENSE_AN_BUF    ADC1BUF0        // ADC result buffer register
#define QP_CH1_VSENSE_AN_MASK   (1<<11)         // AN11
#define QP_CH1_VSENSE_AN_CH     11              // channe1001l index of ADC port

// hardware configuration for quadrant photodiode channel 2 VSense
#define QP_CH2_VSENSE_TRIS      TRISB           // port TRIS I/O buffer register
#define QP_CH2_VSENSE_MASK      (1<<13)         // RB13
#define QP_CH2_VSENSE_AN_BUF    ADC1BUF0        // ADC result buffer register
#define QP_CH2_VSENSE_AN_MASK   (1<<13)         // AN13
#define QP_CH2_VSENSE_AN_CH     13              // channel index of ADC port

// hardware configuration for quadrant photodiode channel 1 VSense
#define QP_CH3_VSENSE_TRIS      TRISB           // port TRIS I/O buffer register
#define QP_CH3_VSENSE_MASK      (1<<14)         // RB14
#define QP_CH3_VSENSE_AN_BUF    ADC1BUF0        // ADC result buffer register
#define QP_CH3_VSENSE_AN_MASK   (1<<14)         // AN14
#define QP_CH3_VSENSE_AN_CH     14              // channel index of ADC port

// hardware configuration for quadrant photodiode channel 1 VSense
#define QP_CH4_VSENSE_TRIS      TRISB           // port TRIS I/O buffer register
#define QP_CH4_VSENSE_MASK      (1<<15)         // RB15
#define QP_CH4_VSENSE_AN_BUF    ADC1BUF0        // ADC result buffer register
#define QP_CH4_VSENSE_AN_MASK   (1<<15)         // AN15
#define QP_CH4_VSENSE_AN_CH     15              // channel index of ADC port

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

// hardware configuration for laser current limit Vref VSense
#define LSR_VREF_VSENSE_TRIS    TRISB           // port TRIS I/O buffer register
#define LSR_VREF_VSENSE_MASK    (1<<8)          // RB8
#define LSR_VREF_VSENSE_AN_BUF  ADC1BUF0        // ADC result buffer register
#define LSR_VREF_VSENSE_AN_MASK (1<<8)          // AN8
#define LSR_VREF_VSENSE_AN_CH   8               // channel index of ADC port

// hardware configuration for laser current low-side current sense positive node
#define LSR_ISENSE_LS_P_TRIS    TRISB           // port TRIS I/O buffer register
#define LSR_ISENSE_LS_P_MASK    (1<<9)          // RB9
#define LSR_ISENSE_LS_P_AN_BUF  ADC1BUF0        // ADC result buffer register
#define LSR_ISENSE_LS_P_AN_MASK (1<<9)          // AN9
#define LSR_ISENSE_LS_P_AN_CH   9               // channel index of ADC port
    
// hardware configuration for laser low-side current sense negative node
#define LSR_ISENSE_LS_N_TRIS    TRISB           // port TRIS I/O buffer register
#define LSR_ISENSE_LS_N_MASK    (1<<10)         // RB10
#define LSR_ISENSE_LS_N_AN_BUF  ADC1BUF0        // ADC result buffer register
#define LSR_ISENSE_LS_N_AN_MASK (1<<10)         // AN10
#define LSR_ISENSE_LS_N_AN_CH   10              // channel index of ADC port

// mod.h configuration

// hardware configuration for modulation timer entity and interrupts
#define MOD_TIMER_CON            T4CON          // type B timer control register
#define MOD_TIMER_PERIOD         PR4            // type B timer period register
#define MOD_TIMER_REG            TMR4           // type B timer count register
#define MOD_TIMER_INT_ENB        IEC0           // type B timer interrupt enable
#define MOD_TIMER_INT_PRIO       IPC4           // type B timer intterupt priority
#define MOD_TIMER_INT_FLAG       IFS0           // type B timer interrupt flag
#define MOD_TIMER_PRIO_OFFSET    2              // interrupt priority level bit offset
#define MOD_TIMER_SUBPRIO_OFFSET 0              // interrupt subpriority level bit offset
#define MOD_TIMER_INT_MASK       (1<<16)        // type B timer interrupt mask

// gimbal.h configuration

// hardware configuration for pan servo
#define SERVO_PAN_ANGLE_RNG     180             // assuming a 180 degree servo
#define SERVO_PAN_OCCON         OC1CON          // output compare control register
#define SERVO_PAN_OCR           OC1R            // oputput compare 'x' control register
#define SERVO_PAN_OCRS          OC1RS           // output compare 'x' 2nd control register
#define SERVO_PAN_PERIOD_NS     (20000000ul)    // period of PWM pulse cycle (in nsec)
#define SERVO_PAN_PULSE_MIN_NS  (553000ul)      // minimum range pulse width (in nsec)
#define SERVO_PAN_PULSE_MAX_NS  (2520000ul)     // maximum range pulse width (in nsec)
#define SERVO_PAN_PULSE_RNG_NS  SERVO_PAN_PULSE_MAX_NS-SERVO_PAN_PULSE_MIN_NS
#define SERVO_PAN_TIMER_CON     T2CON           // type B timer control register
#define SERVO_PAN_TIMER_PERIOD  PR2             // type B timer period register
#define SERVO_PAN_TIMER_REG     TMR2            // type B timer count register
#define SERVO_PAN_TIMER_IDX     2               // timer to use for PWM (2 or 3)

// hardware configuration for tilt servo
#define SERVO_TILT_ANGLE_RNG    180             // assuming a 180 degree servo
#define SERVO_TILT_OCCON        OC4CON          // output compare control register
#define SERVO_TILT_OCR          OC4R            // oputput compare 'x' control register
#define SERVO_TILT_OCRS         OC4RS           // output compare 'x' 2nd control register
#define SERVO_TILT_PERIOD_NS    (20000000ul)    // period of PWM pulse cycle (in nsec)
#define SERVO_TILT_PULSE_MIN_NS (553000ul)      // minimum range pulse width (in nsec)
#define SERVO_TILT_PULSE_MAX_NS (2520000ul)     // maximum range pulse width (in nsec)
#define SERVO_TILT_PULSE_RNG_NS SERVO_TILT_PULSE_MAX_NS-SERVO_TILT_PULSE_MIN_NS
#define SERVO_TILT_TIMER_CON    T2CON           // type B timer control register
#define SERVO_TILT_TIMER_PERIOD PR2             // type B timer period register
#define SERVO_TILT_TIMER_REG    TMR2            // type B timer count register
#define SERVO_TILT_TIMER_IDX    2               // timer to use for PWM (2 or 3)

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