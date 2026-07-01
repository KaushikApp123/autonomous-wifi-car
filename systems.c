/*
 * Kaushik A
 * ECE 306 Project 10
 */
//------------------------------------------------------------------------------
//
//  Description: This file contains the System Configurations
//
//  Kaushik A
//  Jan 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// include file needed by this file
#include  "functions.h"
// include file needed by this file
#include  "msp430.h"
// include file needed by this file
#include  "macros.h"

// call a helper function
void enable_interrupts(void);

//------------------------------------------------------------------------------
// System Configurations

// Tells the compiler to provide the value in reg as an input to an
// inline assembly block.  Even though the block contains no instructions,
// the compiler may not optimize it away, and is told that the value
// may change and should not be relied upon.

//inline void READ_AND_DISCARD(unsigned int reg) __attribute__((always_inline));

//  asm volatile ("" : "=m" (reg) : "r" (reg));
//}

//inline void enable_interrupts(void) __attribute__((always_inline));
// function used by the car
void enable_interrupts(void){
  // code line for the car logic
  __bis_SR_register(GIE);     // enable interrupts
//  asm volatile ("eint \n");
// block marker
}

//inline void disable_interrupts(void) __attribute__((always_inline));
//void disable_interrupts(void){
//  asm volatile ("dint \n");
//}
