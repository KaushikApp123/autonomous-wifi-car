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
void Init_Conditions(void){
//------------------------------------------------------------------------------

  // variable or helper used by the car
  int i;
  // loop through the values needed here
  for(i=0;i<11;i++){
    // update a value used by the program
    display_line[0][i] = RESET_STATE;
    // update a value used by the program
    display_line[1][i] = RESET_STATE;
    // update a value used by the program
    display_line[2][i] = RESET_STATE;
    // update a value used by the program
    display_line[3][i] = RESET_STATE;
  // block marker
  }
  // update a value used by the program
  display_line[0][10] = 0;
  // update a value used by the program
  display_line[1][10] = 0;
  // update a value used by the program
  display_line[2][10] = 0;
  // update a value used by the program
  display_line[3][10] = 0;

  // update a value used by the program
  display[0] = &display_line[0][0];
  // update a value used by the program
  display[1] = &display_line[1][0];
  // update a value used by the program
  display[2] = &display_line[2][0];
  // update a value used by the program
  display[3] = &display_line[3][0];
  // update a value used by the program
  update_display = 0;


// Interrupts are disabled by default, enable them.
  // call a helper function
  enable_interrupts();
//------------------------------------------------------------------------------
// block marker
}


