/*
 * Kaushik A
 * ECE 306 Project 10
 */
#ifndef SHAPES_H
#define SHAPES_H

#include "msp430.h"

// Globals used by shapes.c (defined in another .c file, likely timers or time.c)
extern volatile unsigned int Last_Time_Sequence;
extern volatile unsigned int cycle_time;
extern volatile unsigned int time_change;
extern volatile unsigned int myTime;

// Function prototypes
void Shapes(void);
void Run_Circle(void);
void Run_Eight(void);
void Run_Triangle(void);
void Run_Circle_CCW(void);

#endif  // SHAPES_H
