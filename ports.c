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

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
void Init_Ports(void) {
    Init_Port_1();
    Init_Port_2();
    Init_Port_3();
    Init_Port_4();
    Init_Port_5();
    Init_Port_6();
}

void Init_Port_1(void){
    P1OUT=0x00;
    P1DIR=0x00;

    P1SEL0 &= ~RED_LED;
    P1SEL1 &= ~RED_LED;
    P1OUT &= ~RED_LED;
    P1DIR |= RED_LED;

    P1SEL0 &= ~A1_SEEED;
    P1SEL1 &= ~A1_SEEED;
    P1OUT &= ~A1_SEEED;
    P1DIR &= ~A1_SEEED;

    P1SEL0 &= ~V_DETECT_L;
    P1SEL1 &= ~V_DETECT_L;
    P1OUT &= ~V_DETECT_L;
    P1DIR &= ~V_DETECT_L;

    P1SEL0 &= ~V_DETECT_R;
    P1SEL1 &= ~V_DETECT_R;
    P1OUT &= ~V_DETECT_R;
    P1DIR &= ~V_DETECT_R;

    P1SEL0 &= ~A4_SEEED;
    P1SEL1 &= ~A4_SEEED;
    P1OUT &= ~A4_SEEED;
    P1DIR &= ~A4_SEEED;

    P1SEL0 &= ~V_THUMB;
    P1SEL1 &= ~V_THUMB;
    P1OUT &= ~V_THUMB;
    P1DIR &= ~V_THUMB;

    P1SEL0 &= ~UCA0RXD;
    P1SEL1 &= ~UCA0RXD;
    P1OUT &= ~UCA0RXD;
    P1DIR &= ~UCA0RXD;

    P1SEL0 &= ~UCA0TXD;
    P1SEL1 &= ~UCA0TXD;
    P1OUT &= ~UCA0TXD;
    P1DIR &= ~UCA0TXD;

    P1SEL0 &= ~RED_LED; // Set RED_LED as GP I/O
     P1SEL1 &= ~RED_LED; // Set RED_LED as GP I/O
     P1OUT |= RED_LED; // Set Red LED On
     P1DIR |= RED_LED; // Set Red LED direction to output
     P1SELC |= A1_SEEED; // ADC input for A1_SEEED
     P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
     P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
     P1SELC |= A4_SEEED; // ADC input for V_A4_SEEED
     P1SELC |= V_THUMB; // ADC input for V_THUMB
     P1SEL0 |= UCA0TXD; // UCA0TXD pin
     P1SEL1 &= ~UCA0TXD; // UCA0TXD pin
     P1SEL0 |= UCA0RXD; // UCA0RXD pin
     P1SEL1 &= ~UCA0RXD; // UCA0RXD pin

}

void Init_Port_2(void){ // Configure Port 2
//------------------------------------------------------------------------------
 P2OUT = 0x00; // P2 set Low
 P2DIR = 0x00; // Set P2 direction to output

 P2SEL0 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
 P2SEL1 &= ~SLOW_CLK; // SLOW_CLK GPIO operation
 P2OUT &= ~SLOW_CLK; // Initial Value = Low / Off
 P2DIR |= SLOW_CLK; // Direction = output

 P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
 P2DIR |= CHECK_BAT; // Direction = output

 P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
 P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
 P2OUT |= IR_LED; // Initial Value = Low / Off
 P2DIR |= IR_LED; // Direction = output

 P2SEL0 &= ~SW2; // SW2 Operation
 P2SEL1 &= ~SW2; // SW2 Operation
 P2OUT |= SW2; // Configure pullup resistor
 P2DIR &= ~SW2; // Direction = input
 P2REN |= SW2; // Enable pullup resistor
 P2IES |= SW2;
 P2IFG &= ~SW2;
 P2IE  |= SW2;

 P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
 P2SEL1 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
 P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
 P2DIR |= IOT_RUN_CPU; // Direction = output

 P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
 P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
 P2OUT |= DAC_ENB; // Initial Value = High
 P2DIR |= DAC_ENB; // Direction = output

 P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
 P2SEL1 |= LFXOUT; // LFXOUT Clock operation

 P2SEL0 &= ~LFXIN; // LFXIN Clock operation
 P2SEL1 |= LFXIN; // LFXIN Clock operation
 //------------------------------------------------------------------------------
}

void Init_Port_3(void){
P3OUT= 0x00;
P3DIR =0x00;

P3SEL0 &= ~TEST_PROBE;
P3SEL1 &= ~TEST_PROBE;
P3OUT &= ~TEST_PROBE;
P3DIR &= ~TEST_PROBE;

P3SEL0 &= ~OA2O;
P3SEL1 &= ~OA2O;
P3OUT &= ~OA2O;
P3DIR &= ~OA2O;

P3SEL0 &= ~OA2N;
P3SEL1 &= ~OA2N;
P3OUT &= ~OA2N;
P3DIR &= ~OA2N;

P3SEL0 &= ~OA2P;
P3SEL1 &= ~OA2P;
P3OUT &= ~OA2P;
P3DIR &= ~OA2P;

P3SEL0 &= ~SMCLK_OUT;
P3SEL1 &= ~SMCLK_OUT;
P3OUT &= ~SMCLK_OUT;
P3DIR &= ~SMCLK_OUT;

//P3SEL0 &= ~DAC_CNTL;
//P3SEL1 &= ~DAC_CNTL;
//P3OUT &= ~DAC_CNTL;
//P3DIR &= ~DAC_CNTL;
P3SELC |= DAC_CNTL;

P3SEL0 &= ~IOT_LINK_CPU;
P3SEL1 &= ~IOT_LINK_CPU;
P3OUT &= ~IOT_LINK_CPU;
P3DIR &= ~IOT_LINK_CPU;

P3SEL0 &= ~IOT_EN_CPU;
P3SEL1 &= ~IOT_EN_CPU;
P3OUT |= IOT_EN_CPU;
P3DIR |= IOT_EN_CPU;
}

void Init_Port_4(void){ // Configure PORT 4
//------------------------------------------------------------------------------
 P4OUT = 0x00; // P4 set Low
 P4DIR = 0x00; // Set P4 direction to output
 P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
 P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
 P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
 P4DIR |= RESET_LCD; // Direction = output

 P4SEL0 &= ~SW1; // SW1 GPIO operation
 P4SEL1 &= ~SW1; // SW1 GPIO operation
 P4OUT |= SW1; // Configure pullup resistor
 P4DIR &= ~SW1; // Direction = input
 P4REN |= SW1; // Enable pullup resistor
 P4IES |= SW1;
 P4IFG &= ~SW1;
 P4IE  |= SW1;

 P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
 P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

 P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
 P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

 P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
 P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

 P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
 P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

 P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
 P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

 P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
 P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
 //------------------------------------------------------------------------------
}

void Init_Port_5(void){
P5OUT=0x00;
P5DIR=0x00;

P5SEL0 &= ~V_BAT;
P5SEL1 &= ~V_BAT;
P5OUT &= ~V_BAT;
P5DIR &= ~V_BAT;

P5SEL0 &=  ~V_5_0;
P5SEL1 &=  ~V_5_0;
P5OUT &=  ~V_5_0;
P5DIR &=  ~V_5_0;

P5SEL0 &=  ~V_DAC;
P5SEL1 &=  ~V_DAC;
P5OUT &=  ~V_DAC;
P5DIR &=  ~V_DAC;

P5SEL0 &=  ~V_3_3;
P5SEL1 &=  ~V_3_3;
P5OUT &=  ~V_3_3;
P5DIR &=  ~V_3_3;

P5SEL0 &=  ~IOT_BOOT;
P5SEL1 &=  ~IOT_BOOT;
P5OUT &=  ~IOT_BOOT;
P5DIR &=  ~IOT_BOOT;

P5SELC |= V_BAT; // ADC input for V_BAT
 P5SELC |= V_5_0; // ADC input for V_BAT
 P5SELC |= V_DAC; // ADC input for V_DAC
 P5SELC |= V_3_3; // ADC input for V_3_3
 P5SEL0 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
 P5SEL1 &= ~IOT_BOOT; // IOT_BOOT GPIO operation
 P5OUT |= IOT_BOOT; // Set Output value inactive
 P5DIR |= IOT_BOOT; // Set direction to outpu

}

void Init_Port_6(void){
    P6SEL0 |= LCD_BACKLITE;
    P6SEL1 &= ~LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE;

    P6SEL0 |= R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6DIR |= R_FORWARD;

    P6SEL0 |= L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6DIR |= L_FORWARD;

    P6SEL0 |= R_REVERSE;
    P6SEL1 &= ~R_REVERSE;
    P6DIR |= R_REVERSE;

    P6SEL0 |= L_REVERSE;
    P6SEL1 &= ~L_REVERSE;
    P6DIR |= L_REVERSE;

    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6OUT &= ~P6_5;
    P6DIR &= ~P6_5;

    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6OUT &= ~GRN_LED;
    P6DIR |= GRN_LED;

}
