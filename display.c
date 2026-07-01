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
void Display_Process(void){
  // code line for the car logic
  if(update_display){
    // update a value used by the program
    update_display = 0;
    // code line for the car logic
    if(display_changed){
      // update a value used by the program
      display_changed = 0;
      // call a helper function
      Display_Update(0,0,0,0);
    // block marker
    }
  // block marker
  }
// block marker
}
