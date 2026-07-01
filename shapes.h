/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#ifndef SHAPES_H
// preprocessor setup for the project
#define SHAPES_H

// include file needed by this file
#include "msp430.h"

// Globals used by shapes.c (defined in another .c file, likely timers or time.c)
// variable comes from another file
extern volatile unsigned int Last_Time_Sequence;
// variable comes from another file
extern volatile unsigned int cycle_time;
// variable comes from another file
extern volatile unsigned int time_change;
// variable comes from another file
extern volatile unsigned int myTime;

// Function prototypes
// call a helper function
void Shapes(void);
// call a helper function
void Run_Circle(void);
// call a helper function
void Run_Eight(void);
// call a helper function
void Run_Triangle(void);
// call a helper function
void Run_Circle_CCW(void);

// preprocessor setup for the project
#endif  // SHAPES_H
