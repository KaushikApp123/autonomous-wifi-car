/*
 * Kaushik A
 * ECE 306 Project 10
 */
// include file needed by this file
#include "macros.h"
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
void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  // update a value used by the program
  P1OUT &= ~RED_LED;
  // update a value used by the program
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
// block marker
}

// function used by the car
void Kaushik A_StateMachine(void){
    // choose what to do based on the state
    switch(Time_Sequence){
      // state or command case
      case 250:                        //
        // code line for the car logic
        if(one_time){
          // call a helper function
          Init_LEDs();
          // call a helper function
          lcd_BIG_mid();
          // update a value used by the program
          display_changed = 1;
          // update a value used by the program
          one_time = 0;
        // block marker
        }
        // update a value used by the program
        Time_Sequence = 0;             //
        // code line for the car logic
        break;
      // state or command case
      case 200:                        //
        // code line for the car logic
        if(one_time){
//          P1OUT &= ~RED_LED;            // Change State of LED 4
          // update a value used by the program
          P6OUT |= GRN_LED;            // Change State of LED 5
          // update a value used by the program
          one_time = 0;
        // block marker
        }
        // code line for the car logic
        break;
      // state or command case
      case 150:                         //
        // code line for the car logic
        if(one_time){
          // update a value used by the program
          P1OUT |= RED_LED;            // Change State of LED 4
          // update a value used by the program
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          // update a value used by the program
          one_time = 0;
        // block marker
        }
        // code line for the car logic
        break;
      // state or command case
      case 100:                         //
        // code line for the car logic
        if(one_time){
//          lcd_4line();
          // call a helper function
          lcd_BIG_bot();
          // update a value used by the program
          P6OUT |= GRN_LED;            // Change State of LED 5
          // update a value used by the program
          display_changed = 1;
          // update a value used by the program
          one_time = 0;
        // block marker
        }
        // code line for the car logic
        break;
      // state or command case
      case  50:                        //
        // code line for the car logic
        if(one_time){
          // update a value used by the program
          one_time = 0;
        // block marker
        }
        // code line for the car logic
        break;                         //
      // code line for the car logic
      default: break;
    // block marker
    }
// block marker
}

