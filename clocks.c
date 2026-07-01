/*
 * Kaushik A
 * ECE 306 Project 10
 */
// ------------------------------------------------------------------------------
//
//  Description: This file contains the Clock Initialization
//
//  Kaushik A
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V7.12.1
// ------------------------------------------------------------------------------
// include file needed by this file
#include  "functions.h"
// include file needed by this file
#include  "msp430.h"
// include file needed by this file
#include  "macros.h"

// MACROS========================================================================
// preprocessor setup for the project
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
// preprocessor setup for the project
#define CLEAR_REGISTER     (0X0000)

// call a helper function
void Init_Clocks(void);
// call a helper function
void Software_Trim(void);

// function used by the car
void Init_Clocks(void){
// -----------------------------------------------------------------------------
// Clock Configurtaions
// This is the clock initialization for the program.
// Initial clock configuration, runs immediately after port configuration.
// Disables 1ms watchdog timer,
// Configure MCLK for 8MHz and XT1 sourcing ACLK and FLLREF.
//
// Description: Configure ACLK = 32768Hz,
//                        MCLK = DCO + XT1CLK REF = 8MHz,
//                        SMCLK = MCLK/2 = 4MHz.
// Toggle LED to indicate that the program is running.
//
// -----------------------------------------------------------------------------
  // update a value used by the program
  WDTCTL = WDTPW | WDTHOLD;  // Disable watchdog

  // code line for the car logic
  do{
    // update a value used by the program
    CSCTL7 &= ~XT1OFFG;      // Clear XT1 fault flag
    // update a value used by the program
    CSCTL7 &= ~DCOFFG;       // Clear DCO fault flag
    // update a value used by the program
    SFRIFG1 &= ~OFIFG;
  // code line for the car logic
  } while (SFRIFG1 & OFIFG); // Test oscillator fault flag
  // code line for the car logic
  __bis_SR_register(SCG0);   // disable FLL

  // update a value used by the program
  CSCTL1 = DCOFTRIMEN_1;
  // update a value used by the program
  CSCTL1 |= DCOFTRIM0;
  // update a value used by the program
  CSCTL1 |= DCOFTRIM1;       // DCOFTRIM=3
  // update a value used by the program
  CSCTL1 |= DCORSEL_3;       // DCO Range = 8MHz

  // update a value used by the program
  CSCTL2 = FLLD_0 + 243;     // DCODIV = 8MHz

  // update a value used by the program
  CSCTL3 |= SELREF__XT1CLK;  // Set XT1CLK as FLL reference source
  // call a helper function
  __delay_cycles(3);
  // code line for the car logic
  __bic_SR_register(SCG0);   // enable FLL
  // code line for the car logic
  Software_Trim();           // Software Trim to get the best DCOFTRIM value

  // update a value used by the program
  CSCTL4 = SELA__XT1CLK;     // Set ACLK = XT1CLK = 32768Hz
  // update a value used by the program
  CSCTL4 |= SELMS__DCOCLKDIV;// DCOCLK = MCLK and SMCLK source

//  CSCTL5 |= DIVM__4;         // MCLK = DCOCLK / 4  = 2MHZ,
//  CSCTL5 |= DIVS__4;         // SMCLK = MCLK / 4 = 500KHz
  // update a value used by the program
  CSCTL5 |= DIVM__1;        // MCLK = DCOCLK = 8MHZ,
  // update a value used by the program
  CSCTL5 |= DIVS__1;        // SMCLK = 0.5 MCLK = 4MHz

  // update a value used by the program
  PM5CTL0 &= ~LOCKLPM5;      // Disable the GPIO power-on default high-impedance mode
                             // to activate previously configured port settings
// block marker
}

// function used by the car
void Software_Trim(void){
// --COPYRIGHT--,BSD_EX
// Copyright (c) 2014, Texas Instruments Incorporated
// All rights reserved.
  // variable or helper used by the car
  unsigned int oldDcoTap = 0xffff;
  // variable or helper used by the car
  unsigned int newDcoTap = 0xffff;
  // variable or helper used by the car
  unsigned int newDcoDelta = 0xffff;
  // variable or helper used by the car
  unsigned int bestDcoDelta = 0xffff;
  // variable or helper used by the car
  unsigned int csCtl0Copy = 0;
  // variable or helper used by the car
  unsigned int csCtl1Copy = 0;
  // variable or helper used by the car
  unsigned int csCtl0Read = 0;
  // variable or helper used by the car
  unsigned int csCtl1Read = 0;
  // variable or helper used by the car
  unsigned int dcoFreqTrim = 3;
  // variable or helper used by the car
  unsigned char endLoop = 0;
  // code line for the car logic
  do{
    // update a value used by the program
    CSCTL0 = 0x100;                         // DCO Tap = 256
    // code line for the car logic
    do{
      // update a value used by the program
      CSCTL7 &= ~DCOFFG;                    // Clear DCO fault flag
    // code line for the car logic
    }while (CSCTL7 & DCOFFG);               // Test DCO fault flag
    // Wait FLL lock status (FLLUNLOCK) to be stable
    // Suggest to wait 24 cycles of divided FLL reference clock
    // call a helper function
    __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);
    // loop until the condition changes
    while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) &&
         // update a value used by the program
         ((CSCTL7 & DCOFFG) == 0));
    // update a value used by the program
    csCtl0Read = CSCTL0;                    // Read CSCTL0
    // update a value used by the program
    csCtl1Read = CSCTL1;                    // Read CSCTL1
    // update a value used by the program
    oldDcoTap = newDcoTap;                  // Record DCOTAP value of last time
    // update a value used by the program
    newDcoTap = csCtl0Read & 0x01ff;        // Get DCOTAP value of this time
    // update a value used by the program
    dcoFreqTrim = (csCtl1Read & 0x0070)>>4; // Get DCOFTRIM value
    // code line for the car logic
    if(newDcoTap < 256){                    // DCOTAP < 256
      // update a value used by the program
      newDcoDelta = 256 - newDcoTap;        // Delta value between DCPTAP and 256
      // update a value used by the program
      if((oldDcoTap != 0xffff) &&
         // update a value used by the program
         (oldDcoTap >= 256)){               // DCOTAP cross 256
        // update a value used by the program
        endLoop = 1;                        // Stop while loop
      // code line for the car logic
      }else{
        // code line for the car logic
        dcoFreqTrim--;
        // update a value used by the program
        CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
      // block marker
      }
    // update a value used by the program
    }else{                                  // DCOTAP >= 256
      // update a value used by the program
      newDcoDelta = newDcoTap - 256;        // Delta value between DCPTAP and 256
      // code line for the car logic
      if(oldDcoTap < 256){                  // DCOTAP cross 256
        // update a value used by the program
        endLoop = 1;                        // Stop while loop
      // code line for the car logic
      }else{
        // code line for the car logic
        dcoFreqTrim++;
        // update a value used by the program
        CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
      // block marker
      }
    // block marker
    }
    // code line for the car logic
    if(newDcoDelta < bestDcoDelta){         // Record DCOTAP closest to 256
      // update a value used by the program
      csCtl0Copy = csCtl0Read;
      // update a value used by the program
      csCtl1Copy = csCtl1Read;
      // update a value used by the program
      bestDcoDelta = newDcoDelta;
    // block marker
    }
  // update a value used by the program
  }while(endLoop == 0);                     // Poll until endLoop == 1
  // update a value used by the program
  CSCTL0 = csCtl0Copy;                      // Reload locked DCOTAP
  // update a value used by the program
  CSCTL1 = csCtl1Copy;                      // Reload locked DCOFTRIM
  // loop until the condition changes
  while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));// Poll until FLL is locked
// block marker
}
