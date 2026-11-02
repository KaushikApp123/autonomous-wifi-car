/*
 * Kaushik A
 * ECE 306 Project 10
 */
#ifndef MOVEMENTS_H
#define MOVEMENTS_H

#define KP      5
#define KI      10
#define KD      1
#define SCALE   100

// Movement globals
extern volatile unsigned int  move_time_remaining;
extern volatile unsigned char move_active;
extern char pad8_state;
extern char exit_state;
extern char count;
extern char current_direction;
extern char found_white;
extern volatile unsigned int bl_seconds;
extern volatile unsigned int exit_timer;

// Basic movement
void stop_movement(void);
void forward_movement(void);
void forward_slow_movement(void);
void Reverse1(void);
void Reverse_movement_slow(void);
void Reverse_fast(void);
void turn_right(void);
void turn_right2(void);
void turn_left(void);
void circle(void);
void arc_right_movement(void);
void arc_sequence(void);         // U: forward + arc + auto BL
void arc_sequence_v(void);       // V: same arc, alt forward distances
void pad_forward_sequence(void); // P: straight to pad 8, stop + display

// Logic
void detect_black(void);
void hold_black_line(void);
void pad_8(void);
void exit_sequence(void);

// WiFi drive command functions
extern volatile char         pending_direction;
extern volatile unsigned int pending_time;

unsigned int  ASCII_Digits_To_Uint(const char *str, unsigned int offset);
unsigned char Validate_PIN(const char *payload);
void          Parse_Drive_Command(const char *payload);
void          Execute_Forward(unsigned int time_units);
void          Execute_Reverse(unsigned int time_units);
void          Execute_Right(unsigned int time_units);
void          Execute_Left(unsigned int time_units);
void          Execute_Detect(unsigned int time_units);
void          Execute_Circle(unsigned int time_units);
void          Execute_Arc(unsigned int time_units);
void          Execute_Arc_V(unsigned int time_units);
void          Execute_Pad(unsigned int time_units);
void          Execute_Arrive(unsigned int pad_num);
void          Execute_Exit(void);
void          Execute_Stop(void);

#endif
