/*
 * Kaushik A
 * ECE 306 Project 10
 */
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
// include file needed by this file
#include  "macros.h"
// include file needed by this file
#include  "serial.h"

// variable comes from another file
extern volatile unsigned int Last_Time_Sequence;
// variable comes from another file
extern volatile unsigned int cycle_time;
// variable comes from another file
extern volatile unsigned int time_change;
// variable comes from another file
extern volatile unsigned int myTime;
// variable comes from another file
extern volatile unsigned int Timer_counter;
// variable comes from another file
extern char line_detect;
// variable or helper used by the car
unsigned int sw2_position = OKAY;
// variable or helper used by the car
unsigned int okay_to_look_at_switch2 = OKAY;
// variable or helper used by the car
unsigned int count_debounce_SW2;
// variable or helper used by the car
unsigned int sw1_position;
// variable or helper used by the car
unsigned int okay_to_look_at_switch1;
// variable or helper used by the car
unsigned int count_debounce_SW1;
// variable or helper used by the car
int switch_count;
// variable comes from another file
extern unsigned char event;
// variable comes from another file
extern char display_line[4][11];
// variable comes from another file
extern volatile unsigned char display_changed;
// variable comes from another file
extern volatile unsigned char update_display;
// variable or helper used by the car
volatile unsigned char sw1_pressed;
// variable or helper used by the car
volatile unsigned char sw2_pressed;
// variable or helper used by the car
unsigned char flash_backlight;

// Calibration values used by the line follower.
// SW1 sets black thresholds. SW2 sets white/reference values.
// variable or helper used by the car
int black       = ADC_DEFAULT_BLACK_THRESHOLD;
// variable or helper used by the car
int black_left  = ADC_DEFAULT_BLACK_THRESHOLD;
// variable or helper used by the car
int black_right = ADC_DEFAULT_BLACK_THRESHOLD;
// variable or helper used by the car
int white       = ADC_DEFAULT_WHITE_THRESHOLD;
// variable or helper used by the car
int white_left  = ADC_DEFAULT_WHITE_THRESHOLD;
// variable or helper used by the car
int white_right = ADC_DEFAULT_WHITE_THRESHOLD;

// Raw calibration captures. The actual black_left/black_right used by
// Left_Sees_Black()/Right_Sees_Black() become midpoint thresholds after SW2.
// variable or helper used by the car
static int raw_black_left  = ADC_DEFAULT_BLACK_THRESHOLD;
// variable or helper used by the car
static int raw_black_right = ADC_DEFAULT_BLACK_THRESHOLD;

// variable comes from another file
extern int ADC_Left_Detect;
// variable comes from another file
extern int ADC_Right_Detect;

// variable comes from another file
extern volatile char current_command[MAX_COMMAND_LENGTH];
// variable comes from another file
extern unsigned int return_to_waiting_timer;

// variable comes from another file
extern char count;
// variable comes from another file
extern char pad8_state;

//------------------------------------------------------------------------------
// Write_4Digit
// Places a 0-padded 4 digit number into display_line[] starting at pos.
//------------------------------------------------------------------------------
// function used by the car
static void Write_4Digit(char *line, unsigned int pos, int value)
// block marker
{
    // check if this condition is true
    if (value < BEGINNING)
    // block marker
    {
        // update a value used by the program
        value = BEGINNING;
    // block marker
    }
    // check if this condition is true
    if (value > 9999)
    // block marker
    {
        // update a value used by the program
        value = 9999;
    // block marker
    }

    // update a value used by the program
    line[pos]     = ASCII_ZERO + ((value / 1000) % 10);
    // update a value used by the program
    line[pos + 1] = ASCII_ZERO + ((value / 100)  % 10);
    // update a value used by the program
    line[pos + 2] = ASCII_ZERO + ((value / 10)   % 10);
    // update a value used by the program
    line[pos + 3] = ASCII_ZERO + (value % 10);
// block marker
}

// function used by the car
void Switches_Process(void)
// block marker
{
    // call a helper function
    Switch1_Process();
    // call a helper function
    Switch2_Process();
// block marker
}

//------------------------------------------------------------------------------
// Switch1_Process
// SW1 = calibrate BLACK. Put both sensors over black tape and press SW1.
// This stores separate left/right black thresholds so each sensor is judged
// against its own calibrated value.
//------------------------------------------------------------------------------
// function used by the car
void Switch1_Process(void)
// block marker
{
    // check if this condition is true
    if (sw1_pressed == TRUE)
    // block marker
    {
        // update a value used by the program
        sw1_pressed = FALSE;

        // SW1 captures the RAW black-tape readings.
        // Do not subtract a fixed offset here. We will build the real threshold
        // after SW2 by taking the midpoint between black and white.
        // update a value used by the program
        raw_black_left  = ADC_Left_Detect;
        // update a value used by the program
        raw_black_right = ADC_Right_Detect;

        // update a value used by the program
        black_left  = raw_black_left;
        // update a value used by the program
        black_right = raw_black_right;
        // update a value used by the program
        black       = (black_left + black_right) / 2;

        // call a helper function
        strcpy(display_line[0], "Blk Raw   ");
        // call a helper function
        strcpy(display_line[1], "L:        ");
        // call a helper function
        strcpy(display_line[2], "R:        ");
        // call a helper function
        strcpy(display_line[3], "Press SW2 ");
        // call a helper function
        Write_4Digit(display_line[1], 2, raw_black_left);
        // call a helper function
        Write_4Digit(display_line[2], 2, raw_black_right);

        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
    // block marker
    }
// block marker
}

//------------------------------------------------------------------------------
// Switch2_Process
// SW2 = calibrate WHITE. Put both sensors over the white board/floor and press SW2.
// The current black-line follower does not need white to decide line/not-line,
// but it is kept for display/debugging and future white-board logic.
//------------------------------------------------------------------------------
// function used by the car
void Switch2_Process(void)
// block marker
{
    // check if this condition is true
    if (sw2_pressed == TRUE)
    // block marker
    {
        // update a value used by the program
        sw2_pressed = FALSE;

        // SW2 captures RAW white/background readings.
        // update a value used by the program
        white_left  = ADC_Left_Detect;
        // update a value used by the program
        white_right = ADC_Right_Detect;
        // update a value used by the program
        white       = (white_left + white_right) / 2;

        // Build real black-detection thresholds from the midpoint between
        // black and white for each sensor. This is much safer than black-100
        // when your black/white readings are close together.
        // update a value used by the program
        black_left  = (raw_black_left  + white_left)  / 2;
        // update a value used by the program
        black_right = (raw_black_right + white_right) / 2;
        // update a value used by the program
        black       = (black_left + black_right) / 2;

        // call a helper function
        strcpy(display_line[0], "Threshold ");
        // call a helper function
        strcpy(display_line[1], "L:        ");
        // call a helper function
        strcpy(display_line[2], "R:        ");
        // call a helper function
        strcpy(display_line[3], "SW2 Done  ");
        // call a helper function
        Write_4Digit(display_line[1], 2, black_left);
        // call a helper function
        Write_4Digit(display_line[2], 2, black_right);

        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
    // block marker
    }
// block marker
}
