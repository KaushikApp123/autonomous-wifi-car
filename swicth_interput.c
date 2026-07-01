/*
 * Kaushik A
 * ECE 306 Project 10
 */
// include file needed by this file
#include  "msp430.h"
// include file needed by this file
#include  <string.h>
// include file needed by this file
#include  "functions.h"
// include file needed by this file
#include  "LCD.h"
// include file needed by this file
#include  "ports.h"
// include file needed by this file
#include "macros.h"

// variable comes from another file
extern volatile unsigned char sw1_pressed;
// variable comes from another file
extern volatile unsigned char sw2_pressed;
// variable or helper used by the car
volatile unsigned char keep_display_off;

//------------------------------------------------------------------------------
// Port 4 interrupt - SW1 = calibrate BLACK
// update a value used by the program
#pragma vector=PORT4_VECTOR
// code line for the car logic
__interrupt void switch1_interrupt(void) {
    // check if this condition is true
    if (P4IFG & SW1) {
        // update a value used by the program
        P4IE    &= ~SW1;
        // update a value used by the program
        P4IFG   &= ~SW1;
        // update a value used by the program
        TB0CCTL1 &= ~CCIFG;
        // update a value used by the program
        TB0CCR1   = TB0R + TB0CCR1_INTERVAL;
        // update a value used by the program
        TB0CCTL1 |= CCIE;
        // update a value used by the program
        sw1_pressed = TRUE;
    // block marker
    }
// block marker
}

//------------------------------------------------------------------------------
// Port 2 interrupt - SW2 = calibrate WHITE
// update a value used by the program
#pragma vector=PORT2_VECTOR
// code line for the car logic
__interrupt void switch2_interrupt(void) {
    // check if this condition is true
    if (P2IFG & SW2) {
        // update a value used by the program
        P2IE    &= ~SW2;
        // update a value used by the program
        P2IFG   &= ~SW2;
        // update a value used by the program
        TB0CCTL2 &= ~CCIFG;
        // update a value used by the program
        TB0CCR2   = TB0R + TB0CCR2_INTERVAL;
        // update a value used by the program
        TB0CCTL2 |= CCIE;
        // update a value used by the program
        sw2_pressed = TRUE;
    // block marker
    }
// block marker
}
