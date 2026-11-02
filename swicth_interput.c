/*
 * Kaushik A
 * ECE 306 Project 10
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern volatile unsigned char sw1_pressed;
extern volatile unsigned char sw2_pressed;
volatile unsigned char keep_display_off;

//------------------------------------------------------------------------------
// Port 4 interrupt - SW1 = calibrate BLACK
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void) {
    if (P4IFG & SW1) {
        P4IE    &= ~SW1;
        P4IFG   &= ~SW1;
        TB0CCTL1 &= ~CCIFG;
        TB0CCR1   = TB0R + TB0CCR1_INTERVAL;
        TB0CCTL1 |= CCIE;
        sw1_pressed = TRUE;
    }
}

//------------------------------------------------------------------------------
// Port 2 interrupt - SW2 = calibrate WHITE
#pragma vector=PORT2_VECTOR
__interrupt void switch2_interrupt(void) {
    if (P2IFG & SW2) {
        P2IE    &= ~SW2;
        P2IFG   &= ~SW2;
        TB0CCTL2 &= ~CCIFG;
        TB0CCR2   = TB0R + TB0CCR2_INTERVAL;
        TB0CCTL2 |= CCIE;
        sw2_pressed = TRUE;
    }
}
