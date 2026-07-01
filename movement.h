/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#ifndef MOVEMENTS_H
// preprocessor setup for the project
#define MOVEMENTS_H

// preprocessor setup for the project
#define KP      5
// preprocessor setup for the project
#define KI      10
// preprocessor setup for the project
#define KD      1
// preprocessor setup for the project
#define SCALE   100

// Movement globals
// variable comes from another file
extern volatile unsigned int  move_time_remaining;
// variable comes from another file
extern volatile unsigned char move_active;
// variable comes from another file
extern char pad8_state;
// variable comes from another file
extern char exit_state;
// variable comes from another file
extern char count;
// variable comes from another file
extern char current_direction;
// variable comes from another file
extern char found_white;
// variable comes from another file
extern volatile unsigned int bl_seconds;
// variable comes from another file
extern volatile unsigned int exit_timer;

// Basic movement
// call a helper function
void stop_movement(void);
// call a helper function
void forward_movement(void);
// call a helper function
void forward_slow_movement(void);
// call a helper function
void Reverse1(void);
// call a helper function
void Reverse_movement_slow(void);
// call a helper function
void Reverse_fast(void);
// call a helper function
void turn_right(void);
// call a helper function
void turn_right2(void);
// call a helper function
void turn_left(void);
// call a helper function
void circle(void);
// call a helper function
void arc_right_movement(void);
// code line for the car logic
void arc_sequence(void);         // U: forward + arc + auto BL
// code line for the car logic
void arc_sequence_v(void);       // V: same arc, alt forward distances
// code line for the car logic
void pad_forward_sequence(void); // P: straight to pad 8, stop + display

// Logic
// call a helper function
void detect_black(void);
// call a helper function
void hold_black_line(void);
// call a helper function
void pad_8(void);
// call a helper function
void exit_sequence(void);

// WiFi drive command functions
// variable comes from another file
extern volatile char         pending_direction;
// variable comes from another file
extern volatile unsigned int pending_time;

// variable or helper used by the car
unsigned int  ASCII_Digits_To_Uint(const char *str, unsigned int offset);
// variable or helper used by the car
unsigned char Validate_PIN(const char *payload);
// call a helper function
void          Parse_Drive_Command(const char *payload);
// call a helper function
void          Execute_Forward(unsigned int time_units);
// call a helper function
void          Execute_Reverse(unsigned int time_units);
// call a helper function
void          Execute_Right(unsigned int time_units);
// call a helper function
void          Execute_Left(unsigned int time_units);
// call a helper function
void          Execute_Detect(unsigned int time_units);
// call a helper function
void          Execute_Circle(unsigned int time_units);
// call a helper function
void          Execute_Arc(unsigned int time_units);
// call a helper function
void          Execute_Arc_V(unsigned int time_units);
// call a helper function
void          Execute_Pad(unsigned int time_units);
// call a helper function
void          Execute_Arrive(unsigned int pad_num);
// call a helper function
void          Execute_Exit(void);
// call a helper function
void          Execute_Stop(void);

// preprocessor setup for the project
#endif
