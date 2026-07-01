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
extern char display_line[4][11];
// variable comes from another file
extern char *display[4];
// variable comes from another file
extern volatile unsigned char display_changed;
// variable comes from another file
extern volatile unsigned char update_display;
// variable comes from another file
extern volatile unsigned int update_display_count;
// variable comes from another file
extern volatile unsigned int Time_Sequence;
// variable comes from another file
extern volatile char one_time;
// function used by the car
void Init_Ports(void) {
    // call a helper function
    Init_Port_1();
    // call a helper function
    Init_Port_2();
    // call a helper function
    Init_Port_3();
    // call a helper function
    Init_Port_4();
    // call a helper function
    Init_Port_5();
    // call a helper function
    Init_Port_6();
// block marker
}

// function used by the car
void Init_Port_1(void){
    // update a value used by the program
    P1OUT=0x00;
    // update a value used by the program
    P1DIR=0x00;

    // update a value used by the program
    P1SEL0 &= ~RED_LED;
    // update a value used by the program
    P1SEL1 &= ~RED_LED;
    // update a value used by the program
    P1OUT &= ~RED_LED;
    // update a value used by the program
    P1DIR |= RED_LED;

    // update a value used by the program
    P1SEL0 &= ~A1_SEEED;
    // update a value used by the program
    P1SEL1 &= ~A1_SEEED;
    // update a value used by the program
    P1OUT &= ~A1_SEEED;
    // update a value used by the program
    P1DIR &= ~A1_SEEED;

    // update a value used by the program
    P1SEL0 &= ~V_DETECT_L;
    // update a value used by the program
    P1SEL1 &= ~V_DETECT_L;
    // update a value used by the program
    P1OUT &= ~V_DETECT_L;
    // update a value used by the program
    P1DIR &= ~V_DETECT_L;

    // update a value used by the program
    P1SEL0 &= ~V_DETECT_R;
    // update a value used by the program
    P1SEL1 &= ~V_DETECT_R;
    // update a value used by the program
    P1OUT &= ~V_DETECT_R;
    // update a value used by the program
    P1DIR &= ~V_DETECT_R;

    // update a value used by the program
    P1SEL0 &= ~A4_SEEED;
    // update a value used by the program
    P1SEL1 &= ~A4_SEEED;
    // update a value used by the program
    P1OUT &= ~A4_SEEED;
    // update a value used by the program
    P1DIR &= ~A4_SEEED;

    // update a value used by the program
    P1SEL0 &= ~V_THUMB;
    // update a value used by the program
    P1SEL1 &= ~V_THUMB;
    // update a value used by the program
    P1OUT &= ~V_THUMB;
    // update a value used by the program
    P1DIR &= ~V_THUMB;

    // update a value used by the program
    P1SEL0 &= ~UCA0RXD;
    // update a value used by the program
    P1SEL1 &= ~UCA0RXD;
    // update a value used by the program
    P1OUT &= ~UCA0RXD;
    // update a value used by the program
    P1DIR &= ~UCA0RXD;

    // update a value used by the program
    P1SEL0 &= ~UCA0TXD;
    // update a value used by the program
    P1SEL1 &= ~UCA0TXD;
    // update a value used by the program
    P1OUT &= ~UCA0TXD;
    // update a value used by the program
    P1DIR &= ~UCA0TXD;

    // update a value used by the program
    P1SEL0 &= ~RED_LED; // Set RED_LED as GP I/O
     // update a value used by the program
     P1SEL1 &= ~RED_LED; // Set RED_LED as GP I/O
     // update a value used by the program
     P1OUT |= RED_LED; // Set Red LED On
     // update a value used by the program
     P1DIR |= RED_LED; // Set Red LED direction to output
     // update a value used by the program
     P1SELC |= A1_SEEED; // ADC input for A1_SEEED
     // update a value used by the program
     P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
     // update a value used by the program
     P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
     // update a value used by the program
     P1SELC |= A4_SEEED; // ADC input for V_A4_SEEED
     // update a value used by the program
     P1SELC |= V_THUMB; // ADC input for V_THUMB
     // update a value used by the program
     P1SEL0 |= UCA0TXD; // UCA0TXD pin
     // update a value used by the program
     P1SEL1 &= ~UCA0TXD; // UCA0TXD pin
     // update a value used by the program
     P1SEL0 |= UCA0RXD; // UCA0RXD pin
     // update a value used by the program
     P1SEL1 &= ~UCA0RXD; // UCA0RXD pin






// block marker
}





// code line for the car logic
void Init_Port_2(void){ // Configure Port 2
//------------------------------------------------------------------------------
 // update a value used by the program
 P2OUT = 0x00; // P2 set Low
 // update a value used by the program
 P2DIR = 0x00; // Set P2 direction to output

 // update a value used by the program
 P2SEL0 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
 // update a value used by the program
 P2SEL1 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
 // update a value used by the program
 P2OUT &= ~SLOW_CLK; // Initial Value = Low / Off
 // update a value used by the program
 P2DIR |= SLOW_CLK; // Direction = output

 // update a value used by the program
 P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 // update a value used by the program
 P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 // update a value used by the program
 P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
 // update a value used by the program
 P2DIR |= CHECK_BAT; // Direction = output

 // update a value used by the program
 P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
 // update a value used by the program
 P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
 // update a value used by the program
 P2OUT |= IR_LED; // Initial Value = Low / Off
 // update a value used by the program
 P2DIR |= IR_LED; // Direction = output

 // update a value used by the program
 P2SEL0 &= ~SW2; // SW2 Operation
 // update a value used by the program
 P2SEL1 &= ~SW2; // SW2 Operation
 // update a value used by the program
 P2OUT |= SW2; // Configure pullup resistor
 // update a value used by the program
 P2DIR &= ~SW2; // Direction = input
 // update a value used by the program
 P2REN |= SW2; // Enable pullup resistor
 // update a value used by the program
 P2IES |= SW2;
 // update a value used by the program
 P2IFG &= ~SW2;
 // update a value used by the program
 P2IE  |= SW2;

 // update a value used by the program
 P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
 // update a value used by the program
 P2SEL1 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
 // update a value used by the program
 P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
 // update a value used by the program
 P2DIR |= IOT_RUN_CPU; // Direction = output

 // update a value used by the program
 P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
 // update a value used by the program
 P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
 // update a value used by the program
 P2OUT |= DAC_ENB; // Initial Value = High
 // update a value used by the program
 P2DIR |= DAC_ENB; // Direction = output

 // update a value used by the program
 P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
 // update a value used by the program
 P2SEL1 |= LFXOUT; // LFXOUT Clock operation

 // update a value used by the program
 P2SEL0 &= ~LFXIN; // LFXIN Clock operation
 // update a value used by the program
 P2SEL1 |= LFXIN; // LFXIN Clock operation
 //------------------------------------------------------------------------------
// block marker
}

// function used by the car
void Init_Port_3(void){
// update a value used by the program
P3OUT= 0x00;
// update a value used by the program
P3DIR =0x00;

// update a value used by the program
P3SEL0 &= ~TEST_PROBE;
// update a value used by the program
P3SEL1 &= ~TEST_PROBE;
// update a value used by the program
P3OUT &= ~TEST_PROBE;
// update a value used by the program
P3DIR &= ~TEST_PROBE;

// update a value used by the program
P3SEL0 &= ~OA2O;
// update a value used by the program
P3SEL1 &= ~OA2O;
// update a value used by the program
P3OUT &= ~OA2O;
// update a value used by the program
P3DIR &= ~OA2O;

// update a value used by the program
P3SEL0 &= ~OA2N;
// update a value used by the program
P3SEL1 &= ~OA2N;
// update a value used by the program
P3OUT &= ~OA2N;
// update a value used by the program
P3DIR &= ~OA2N;

// update a value used by the program
P3SEL0 &= ~OA2P;
// update a value used by the program
P3SEL1 &= ~OA2P;
// update a value used by the program
P3OUT &= ~OA2P;
// update a value used by the program
P3DIR &= ~OA2P;

// update a value used by the program
P3SEL0 &= ~SMCLK_OUT;
// update a value used by the program
P3SEL1 &= ~SMCLK_OUT;
// update a value used by the program
P3OUT &= ~SMCLK_OUT;
// update a value used by the program
P3DIR &= ~SMCLK_OUT;

//P3SEL0 &= ~DAC_CNTL;
//P3SEL1 &= ~DAC_CNTL;
//P3OUT &= ~DAC_CNTL;
//P3DIR &= ~DAC_CNTL;
// update a value used by the program
P3SELC |= DAC_CNTL;

// update a value used by the program
P3SEL0 &= ~IOT_LINK_CPU;
// update a value used by the program
P3SEL1 &= ~IOT_LINK_CPU;
// update a value used by the program
P3OUT &= ~IOT_LINK_CPU;
// update a value used by the program
P3DIR &= ~IOT_LINK_CPU;

// update a value used by the program
P3SEL0 &= ~IOT_EN_CPU;
// update a value used by the program
P3SEL1 &= ~IOT_EN_CPU;
// update a value used by the program
P3OUT |= IOT_EN_CPU;
// update a value used by the program
P3DIR |= IOT_EN_CPU;
// block marker
}


// code line for the car logic
void Init_Port_4(void){ // Configure PORT 4
//------------------------------------------------------------------------------
 // update a value used by the program
 P4OUT = 0x00; // P4 set Low
 // update a value used by the program
 P4DIR = 0x00; // Set P4 direction to output
 // update a value used by the program
 P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
 // update a value used by the program
 P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
 // update a value used by the program
 P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
 // update a value used by the program
 P4DIR |= RESET_LCD; // Direction = output

 // update a value used by the program
 P4SEL0 &= ~SW1; // SW1 GPIO operation
 // update a value used by the program
 P4SEL1 &= ~SW1; // SW1 GPIO operation
 // update a value used by the program
 P4OUT |= SW1; // Configure pullup resistor
 // update a value used by the program
 P4DIR &= ~SW1; // Direction = input
 // update a value used by the program
 P4REN |= SW1; // Enable pullup resistor
 // update a value used by the program
 P4IES |= SW1;
 // update a value used by the program
 P4IFG &= ~SW1;
 // update a value used by the program
 P4IE  |= SW1;


 // update a value used by the program
 P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
 // update a value used by the program
 P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

 // update a value used by the program
 P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
 // update a value used by the program
 P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

 // update a value used by the program
 P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 // update a value used by the program
 P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 // update a value used by the program
 P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
 // update a value used by the program
 P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

 // update a value used by the program
 P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
 // update a value used by the program
 P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

 // update a value used by the program
 P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
 // update a value used by the program
 P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

 // update a value used by the program
 P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
 // update a value used by the program
 P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
 //------------------------------------------------------------------------------
// block marker
}

// function used by the car
void Init_Port_5(void){
// update a value used by the program
P5OUT=0x00;
// update a value used by the program
P5DIR=0x00;

// update a value used by the program
P5SEL0 &= ~V_BAT;
// update a value used by the program
P5SEL1 &= ~V_BAT;
// update a value used by the program
P5OUT &= ~V_BAT;
// update a value used by the program
P5DIR &= ~V_BAT;

// update a value used by the program
P5SEL0 &=  ~V_5_0;
// update a value used by the program
P5SEL1 &=  ~V_5_0;
// update a value used by the program
P5OUT &=  ~V_5_0;
// update a value used by the program
P5DIR &=  ~V_5_0;

// update a value used by the program
P5SEL0 &=  ~V_DAC;
// update a value used by the program
P5SEL1 &=  ~V_DAC;
// update a value used by the program
P5OUT &=  ~V_DAC;
// update a value used by the program
P5DIR &=  ~V_DAC;

// update a value used by the program
P5SEL0 &=  ~V_3_3;
// update a value used by the program
P5SEL1 &=  ~V_3_3;
// update a value used by the program
P5OUT &=  ~V_3_3;
// update a value used by the program
P5DIR &=  ~V_3_3;

// update a value used by the program
P5SEL0 &=  ~IOT_BOOT;
// update a value used by the program
P5SEL1 &=  ~IOT_BOOT;
// update a value used by the program
P5OUT &=  ~IOT_BOOT;
// update a value used by the program
P5DIR &=  ~IOT_BOOT;

// update a value used by the program
P5SELC |= V_BAT; // ADC input for V_BAT
 // update a value used by the program
 P5SELC |= V_5_0; // ADC input for V_BAT
 // update a value used by the program
 P5SELC |= V_DAC; // ADC input for V_DAC
 // update a value used by the program
 P5SELC |= V_3_3; // ADC input for V_3_3
 // update a value used by the program
 P5SEL0 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
 // update a value used by the program
 P5SEL1 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
 // update a value used by the program
 P5OUT |= IOT_BOOT; // Set Output value inactive
 // update a value used by the program
 P5DIR |= IOT_BOOT; // Set direction to outpu





// block marker
}


// function used by the car
void Init_Port_6(void){
    // update a value used by the program
    P6SEL0 |= LCD_BACKLITE;
    // update a value used by the program
    P6SEL1 &= ~LCD_BACKLITE;
    // update a value used by the program
    P6DIR |= LCD_BACKLITE;

    // update a value used by the program
    P6SEL0 |= R_FORWARD;
    // update a value used by the program
    P6SEL1 &= ~R_FORWARD;
    // update a value used by the program
    P6DIR |= R_FORWARD;

    // update a value used by the program
    P6SEL0 |= L_FORWARD;
    // update a value used by the program
    P6SEL1 &= ~L_FORWARD;
    // update a value used by the program
    P6DIR |= L_FORWARD;

    // update a value used by the program
    P6SEL0 |= R_REVERSE;
    // update a value used by the program
    P6SEL1 &= ~R_REVERSE;
    // update a value used by the program
    P6DIR |= R_REVERSE;

    // update a value used by the program
    P6SEL0 |= L_REVERSE;
    // update a value used by the program
    P6SEL1 &= ~L_REVERSE;
    // update a value used by the program
    P6DIR |= L_REVERSE;

    // update a value used by the program
    P6SEL0 &= ~P6_5;
    // update a value used by the program
    P6SEL1 &= ~P6_5;
    // update a value used by the program
    P6OUT &= ~P6_5;
    // update a value used by the program
    P6DIR &= ~P6_5;

    // update a value used by the program
    P6SEL0 &= ~GRN_LED;
    // update a value used by the program
    P6SEL1 &= ~GRN_LED;
    // update a value used by the program
    P6OUT &= ~GRN_LED;
    // update a value used by the program
    P6DIR |= GRN_LED;

// block marker
}
