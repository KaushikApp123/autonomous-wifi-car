/*
 * Kaushik A
 * ECE 306 Project 10
 */
#ifndef SWITCH_H
#define SWITCH_H

// Function prototypes
void Switches_Process(void);
void Switch1_Process(void);   // SW1 calibrates black tape
void Switch2_Process(void);   // SW2 calibrates white board/background

// Calibration globals (defined in switch.c)
extern int black;
extern int black_left;
extern int black_right;
extern int white;
extern int white_left;
extern int white_right;

#endif // SWITCH_H
