/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#ifndef SWITCH_H
// preprocessor setup for the project
#define SWITCH_H

// Function prototypes
// call a helper function
void Switches_Process(void);
// code line for the car logic
void Switch1_Process(void);   // SW1 calibrates black tape
// code line for the car logic
void Switch2_Process(void);   // SW2 calibrates white board/background

// Calibration globals (defined in switch.c)
// variable comes from another file
extern int black;
// variable comes from another file
extern int black_left;
// variable comes from another file
extern int black_right;
// variable comes from another file
extern int white;
// variable comes from another file
extern int white_left;
// variable comes from another file
extern int white_right;

// preprocessor setup for the project
#endif // SWITCH_H
