/*
 * Kaushik A
 * ECE 306 Project 10
 */
// include file needed by this file
#include "msp430.h"
// include file needed by this file
#include <string.h>
// include file needed by this file
#include <stdlib.h>
// include file needed by this file
#include "functions.h"
// include file needed by this file
#include "LCD.h"
// include file needed by this file
#include "ports.h"
// include file needed by this file
#include "macros.h"
// include file needed by this file
#include "adc.h"
// include file needed by this file
#include "dac.h"
// include file needed by this file
#include "timers.h"
// include file needed by this file
#include "movement.h"
// include file needed by this file
#include "serial.h"

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
// variable comes from another file
extern volatile unsigned int Timer_counter;
// variable comes from another file
extern volatile unsigned int seconds;
// variable comes from another file
extern char display_line[4][11];
// variable comes from another file
extern char last_cmd_display[11];
// variable comes from another file
extern unsigned char final_display_lock;
// variable comes from another file
extern char *display[4];
// variable comes from another file
extern int ADC_Thumb;
// variable comes from another file
extern int ADC_Left_Detect;
// variable comes from another file
extern int ADC_Right_Detect;
// variable comes from another file
extern int black;
// variable comes from another file
extern int black_left;
// variable comes from another file
extern int black_right;
// variable comes from another file
extern int black2;
// variable comes from another file
extern int white;

// variable or helper used by the car
char ADC_Channel;
// variable or helper used by the car
char line_detect           = 0;
// variable or helper used by the car
char count                 = 0;
// variable or helper used by the car
char pad8_state            = BEGINNING;
// variable or helper used by the car
char exit_state            = BEGINNING;
// variable or helper used by the car
char arc_state             = BEGINNING;
// variable or helper used by the car
char arc_state_v           = BEGINNING;
// variable or helper used by the car
char pad_forward_state     = BEGINNING;
// variable or helper used by the car
char current_direction     = NULL_CHAR;
// variable or helper used by the car
char last_bl_turn_direction = RIGHT;
// variable or helper used by the car
char found_white           = FALSE;
// variable or helper used by the car
unsigned int lost_line_count = BEGINNING;
// variable or helper used by the car
unsigned int intercept_lock_count = BEGINNING;
// variable or helper used by the car
volatile int ir_count;
// variable or helper used by the car
volatile char state;

// variable or helper used by the car
volatile unsigned int  move_time_remaining = BEGINNING;
// variable or helper used by the car
volatile unsigned char move_active         = FALSE;

// variable or helper used by the car
volatile char         pending_direction    = NULL_CHAR;
// variable or helper used by the car
volatile unsigned int pending_time         = BEGINNING;

// BL timer - only used during autonomous black line sequence
// variable or helper used by the car
volatile unsigned int bl_seconds  = BEGINNING;

// exit timer - dedicated timer for exit sequence
// variable or helper used by the car
volatile unsigned int exit_timer  = BEGINNING;


// forward declarations
// call a helper function
void Execute_Exit(void);
// call a helper function
void Execute_Stop(void);


// function used by the car
void arc_wide_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_63;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_17;
// block marker
}


// function used by the car
void arc_sequence(void)
// block marker
{
    // choose what to do based on the state
    switch (arc_state)
    // block marker
    {
    // state or command case
    case 0:                          // U step 1: forward 1 second
        // call a helper function
        strcpy(display_line[0], " U Forward");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();

        // check if this condition is true
        if (Timer_counter >= U_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // call a helper function
            strcpy(display_line[0], " Reached  ");
            // call a helper function
            strcpy(display_line[3], "  Pad 8   ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            arc_state = 1;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 1:                          // U step 2: 90 degree left turn
        // call a helper function
        strcpy(display_line[0], " U Left1  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        turn_left();

        // check if this condition is true
        if (Timer_counter >= U_90_RIGHT_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            arc_state = 2;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 2:                          // U step 3: forward 3 seconds
        // call a helper function
        strcpy(display_line[0], " U Fwd 2  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();

        // check if this condition is true
        if (Timer_counter >= U_MID_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            arc_state = 3;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 3:                          // U step 4: 90 degree left turn
        // call a helper function
        strcpy(display_line[0], " U Left2  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        turn_left();

        // check if this condition is true
        if (Timer_counter >= U_90_RIGHT_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            arc_state = 4;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 4:                          // U step 5: forward 2 seconds then auto BL
        // call a helper function
        strcpy(display_line[0], " U Finish ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();

        // check if this condition is true
        if (Timer_counter >= U_FINAL_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // call a helper function
            strcpy(display_line[0], " BL Start ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;

            // update a value used by the program
            arc_state            = BEGINNING;
            // update a value used by the program
            current_direction    = CMD_DETECT;
            // update a value used by the program
            count                = BEGINNING;
            // update a value used by the program
            pad8_state           = BEGINNING;
            // update a value used by the program
            found_white          = FALSE;
            // update a value used by the program
            Timer_counter        = BEGINNING;
            // update a value used by the program
            lost_line_count      = BEGINNING;
            // update a value used by the program
            intercept_lock_count = BEGINNING;
            // update a value used by the program
            bl_seconds           = BEGINNING;
            // update a value used by the program
            move_time_remaining  = 0xFFFF;
            // update a value used by the program
            move_active          = TRUE;
        // block marker
        }
        // code line for the car logic
        break;

    // code line for the car logic
    default:
        // call a helper function
        stop_movement();
        // update a value used by the program
        arc_state = BEGINNING;
        // update a value used by the program
        current_direction = NULL_CHAR;
        // update a value used by the program
        move_active = FALSE;
        // update a value used by the program
        move_time_remaining = BEGINNING;
        // code line for the car logic
        break;
    // block marker
    }
// block marker
}


// V command: same arc as U, alt forward distances
// function used by the car
void arc_sequence_v(void)
// block marker
{
    // choose what to do based on the state
    switch (arc_state_v)
    // block marker
    {
    // state or command case
    case 0:
        // call a helper function
        strcpy(display_line[0], " V Forward");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();
        // check if this condition is true
        if (Timer_counter >= V_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // call a helper function
            strcpy(display_line[0], " Reached  ");
            // call a helper function
            strcpy(display_line[3], "  Pad 8   ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            arc_state_v = 1;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 1:
        // call a helper function
        strcpy(display_line[0], " V WideTrn");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        arc_wide_movement();
        // check if this condition is true
        if (Timer_counter >= V_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            arc_state_v = 2;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 2:
        // call a helper function
        strcpy(display_line[0], " V Finish ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();
        // check if this condition is true
        if (Timer_counter >= V_FINAL_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // call a helper function
            strcpy(display_line[0], " BL Start ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;
            // update a value used by the program
            arc_state_v          = BEGINNING;
            // update a value used by the program
            current_direction    = CMD_DETECT;
            // update a value used by the program
            count                = BEGINNING;
            // update a value used by the program
            pad8_state           = BEGINNING;
            // update a value used by the program
            found_white          = FALSE;
            // update a value used by the program
            Timer_counter        = BEGINNING;
            // update a value used by the program
            lost_line_count      = BEGINNING;
            // update a value used by the program
            intercept_lock_count = BEGINNING;
            // update a value used by the program
            bl_seconds           = BEGINNING;
            // update a value used by the program
            move_time_remaining  = 0xFFFF;
            // update a value used by the program
            move_active          = TRUE;
        // block marker
        }
        // code line for the car logic
        break;

    // code line for the car logic
    default:
        // call a helper function
        stop_movement();
        // update a value used by the program
        arc_state_v = BEGINNING;
        // update a value used by the program
        current_direction = NULL_CHAR;
        // update a value used by the program
        move_active = FALSE;
        // update a value used by the program
        move_time_remaining = BEGINNING;
        // code line for the car logic
        break;
    // block marker
    }
// block marker
}


// P command: drive straight 1.5 sec to pad 8, stop, display Arrived at Pad 8
// function used by the car
void pad_forward_sequence(void)
// block marker
{
    // choose what to do based on the state
    switch (pad_forward_state)
    // block marker
    {
    // state or command case
    case 0:
        // call a helper function
        strcpy(display_line[0], " To Pad 8 ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();
        // check if this condition is true
        if (Timer_counter >= P_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad_forward_state = 1;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 1:
        // call a helper function
        stop_movement();
        // call a helper function
        strcpy(display_line[0], "Arrived 08");
        // call a helper function
        strcpy(display_line[1], "Kaushik A ");
        // call a helper function
        strcpy(display_line[2], "          ");
        // call a helper function
        strcpy(display_line[3], "          ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // update a value used by the program
        move_active         = FALSE;
        // update a value used by the program
        current_direction   = NULL_CHAR;
        // update a value used by the program
        move_time_remaining = BEGINNING;
        // update a value used by the program
        pad_forward_state   = BEGINNING;
        // code line for the car logic
        break;

    // code line for the car logic
    default:
        // call a helper function
        stop_movement();
        // update a value used by the program
        pad_forward_state = BEGINNING;
        // update a value used by the program
        current_direction = NULL_CHAR;
        // update a value used by the program
        move_active = FALSE;
        // update a value used by the program
        move_time_remaining = BEGINNING;
        // code line for the car logic
        break;
    // block marker
    }
// block marker
}


// call a helper function
void exit_sequence(void);
// variable or helper used by the car
static unsigned char Left_On_Black(void);
// variable or helper used by the car
static unsigned char Right_On_Black(void);
// variable or helper used by the car
static unsigned char Circle_Left_On_Black(void);
// variable or helper used by the car
static unsigned char Circle_Right_On_Black(void);
// variable or helper used by the car
static void Display_Final_Stop(void);
// variable or helper used by the car
static void circle_follow_black(void);
// variable or helper used by the car
static void circle_turn_right_slow(void);
// variable or helper used by the car
static void circle_turn_left_slow(void);
// variable or helper used by the car
static void circle_forward_slow(void);

// function used by the car
static void line_turn_right_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_17;
// block marker
}


// function used by the car
static void line_turn_left_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_17;
// block marker
}


// function used by the car
static void line_forward_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // BL Travel: very tiny slowdown so it stays calmer on the line.
    // Keeps the same general left/right bias, just slightly less speed.
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_12_5;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_34;
// block marker
}


// variable or helper used by the car
static void intercept_approach(void);


// function used by the car
void Parse_Drive_Command(const char *payload)
// block marker
{
    // variable or helper used by the car
    unsigned int  offset;
    // variable or helper used by the car
    char          direction;
    // variable or helper used by the car
    unsigned int  time_units;
    // variable or helper used by the car
    unsigned char first_done = FALSE;

    // check if this condition is true
    if (payload[BEGINNING] != CMD_START_CHAR)
    // block marker
    {
        // call a helper function
        PC_Transmit_String("BAD START\r\n");
        // call a helper function
        strcpy(display_line[0], " BADSTART ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // send the result back
        return;
    // block marker
    }

    // check if this condition is true
    if (Validate_PIN(payload) == FALSE)
    // block marker
    {
        // call a helper function
        PC_Transmit_String("AUTH FAIL\r\n");
        // call a helper function
        strcpy(display_line[0], " PIN FAIL ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // send the result back
        return;
    // block marker
    }

    // update a value used by the program
    final_display_lock = FALSE;

    // Highest-priority commands:
    // S must stop immediately, and X must exit immediately, no matter what
    // sequence is currently running.
    // update a value used by the program
    direction = payload[CMD_DIRECTION_OFFSET];

    // check if this condition is true
    if (direction == CMD_STOP)
    // block marker
    {
        // update a value used by the program
        last_cmd_display[0] = CMD_STOP;
        // update a value used by the program
        last_cmd_display[1] = '0';
        // update a value used by the program
        last_cmd_display[2] = '0';
        // update a value used by the program
        last_cmd_display[3] = '0';
        // update a value used by the program
        last_cmd_display[4] = '0';
        // update a value used by the program
        last_cmd_display[5] = NULL_CHAR;
        // call a helper function
        strcpy(display_line[0], "   Stop   ");
        // call a helper function
        Execute_Stop();
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // send the result back
        return;
    // block marker
    }

    // check if this condition is true
    if (direction == CMD_EXIT)
    // block marker
    {
        // update a value used by the program
        last_cmd_display[0] = CMD_EXIT;
        // update a value used by the program
        last_cmd_display[1] = '0';
        // update a value used by the program
        last_cmd_display[2] = '0';
        // update a value used by the program
        last_cmd_display[3] = '0';
        // update a value used by the program
        last_cmd_display[4] = '0';
        // update a value used by the program
        last_cmd_display[5] = NULL_CHAR;
        // call a helper function
        strcpy(display_line[0], " BL Exit  ");
        // call a helper function
        Execute_Exit();
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // send the result back
        return;
    // block marker
    }

    // update a value used by the program
    pending_direction = NULL_CHAR;
    // update a value used by the program
    pending_time      = BEGINNING;

    // update a value used by the program
    offset = CMD_DIRECTION_OFFSET;

    // loop until the condition changes
    while (payload[offset] != NULL_CHAR &&
           // update a value used by the program
           payload[offset] != CARRIAGE_RETURN &&
           // update a value used by the program
           payload[offset] != LINE_FEED)
    // block marker
    {
        // update a value used by the program
        direction  = payload[offset];
        // update a value used by the program
        time_units = ASCII_Digits_To_Uint(payload, offset + ONE_BYTE);

        // update a value used by the program
        last_cmd_display[0] = direction;
        // update a value used by the program
        last_cmd_display[1] = payload[offset + 1];
        // update a value used by the program
        last_cmd_display[2] = payload[offset + 2];
        // update a value used by the program
        last_cmd_display[3] = payload[offset + 3];
        // update a value used by the program
        last_cmd_display[4] = payload[offset + 4];
        // update a value used by the program
        last_cmd_display[5] = NULL_CHAR;

        // check if this condition is true
        if (direction != CMD_FORWARD  &&
            // update a value used by the program
            direction != CMD_BACKWARD &&
            // update a value used by the program
            direction != CMD_RIGHT    &&
            // update a value used by the program
            direction != CMD_LEFT     &&
            // update a value used by the program
            direction != CMD_DETECT   &&
            // update a value used by the program
            direction != CMD_CIRCLE   &&
            // update a value used by the program
            direction != CMD_ARC      &&
            // update a value used by the program
            direction != CMD_ARC_V    &&
            // update a value used by the program
            direction != CMD_PAD      &&
            // update a value used by the program
            direction != CMD_ARRIVE   &&
            // update a value used by the program
            direction != CMD_ARRIVE_LEGACY &&
            // update a value used by the program
            direction != CMD_EXIT     &&
            // update a value used by the program
            direction != CMD_STOP)
        // block marker
        {
            // code line for the car logic
            break;
        // block marker
        }

        // check if this condition is true
        if (first_done == FALSE)
        // block marker
        {
            // choose what to do based on the state
            switch (direction)
            // block marker
            {
                // state or command case
                case CMD_FORWARD:   strcpy(display_line[0], " Forward  "); Execute_Forward(time_units);  break;
                // state or command case
                case CMD_BACKWARD:  strcpy(display_line[0], " Backward "); Execute_Reverse(time_units);  break;
                // state or command case
                case CMD_RIGHT:     strcpy(display_line[0], "  Right   "); Execute_Right(time_units);    break;
                // state or command case
                case CMD_LEFT:      strcpy(display_line[0], "  Left    "); Execute_Left(time_units);     break;
                // state or command case
                case CMD_DETECT:    strcpy(display_line[0], " BL Start "); Execute_Detect(time_units);   break;
                // state or command case
                case CMD_CIRCLE:    strcpy(display_line[0], "  Circle  "); Execute_Circle(time_units);   break;
                // state or command case
                case CMD_ARC:       strcpy(display_line[0], "   Arc U  "); Execute_Arc(time_units);      break;
                // state or command case
                case CMD_ARC_V:     strcpy(display_line[0], "   Arc V  "); Execute_Arc_V(time_units);    break;
                // state or command case
                case CMD_PAD:       strcpy(display_line[0], " To Pad 8 "); Execute_Pad(time_units);      break;
                // state or command case
                case CMD_ARRIVE:
                // state or command case
                case CMD_ARRIVE_LEGACY: Execute_Arrive(time_units);                                      break;
                // state or command case
                case CMD_EXIT:      strcpy(display_line[0], " BL Exit  "); Execute_Exit();               break;
                // state or command case
                case CMD_STOP:      strcpy(display_line[0], "   Stop   "); Execute_Stop();               break;
                // code line for the car logic
                default:            strcpy(display_line[0], "  Unknwn  "); break;
            // block marker
            }
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;
            // update a value used by the program
            first_done = TRUE;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // update a value used by the program
            pending_direction = direction;
            // update a value used by the program
            pending_time      = time_units;
            // code line for the car logic
            break;
        // block marker
        }

        // update a value used by the program
        offset += (ONE_BYTE + NUM_TIME_DIGITS);
    // block marker
    }
// block marker
}


// function used by the car
void stop_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    P1OUT ^= RED_LED;
// block marker
}

// function used by the car
void forward_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // Veer-right fix: nudged further
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_45;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_48;
// block marker
}

// function used by the car
void Reverse1(void)
// block marker
{
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // Veer-right fix: match forward trim
    // update a value used by the program
    RIGHT_REVERSE_SPEED = PERCENT_45;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = PERCENT_48;
// block marker
}

// function used by the car
void turn_right(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_30;
// block marker
}

// function used by the car
void turn_left(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_30;
// block marker
}

// function used by the car
static void fast_forward_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // Fast manual F command. Left is kept slightly higher to preserve the old trim direction.
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_65;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_70;
// block marker
}

// function used by the car
static void fast_reverse_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;

    // Fast manual B command. Same trim direction as forward.
    // update a value used by the program
    RIGHT_REVERSE_SPEED = PERCENT_65;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = PERCENT_70;
// block marker
}

// function used by the car
static void fast_turn_right_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_45;
// block marker
}

// function used by the car
static void fast_turn_left_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_45;
// block marker
}

// function used by the car
void turn_right2(void)
// block marker
{
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = FAST;
    // update a value used by the program
    RIGHT_REVERSE_SPEED = FAST;
// block marker
}

// function used by the car
void forward_slow_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // BL Travel slowed down more because it was still flying off
    // at the squiggly-line-to-circle transition.
    // Same general bias direction, lower total PWM.
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_15;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_45;
// block marker
}

// function used by the car
void Reverse_movement_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_REVERSE_SPEED = SLOW;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = SLOW;
// block marker
}

// function used by the car
void Reverse_fast(void)
// block marker
{
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_REVERSE_SPEED = SLOW;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = FAST;
// block marker
}

// function used by the car
void circle(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_80;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_60;
// block marker
}
// function used by the car
void arc_right_movement(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_70;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_10;
// block marker
}


// function used by the car
void detect_black(void)
// block marker
{
    // variable or helper used by the car
    int ir_cur;
    // variable or helper used by the car
    unsigned char left_black;
    // variable or helper used by the car
    unsigned char right_black;

    // update a value used by the program
    ir_cur = ADC_Left_Detect - ADC_Right_Detect;
    // update a value used by the program
    left_black  = Left_On_Black();
    // update a value used by the program
    right_black = Right_On_Black();

    // check if this condition is true
    if (left_black == FALSE && right_black == FALSE)
    // block marker
    {
        // call a helper function
        line_turn_right_slow();
        // call a helper function
        strcpy(display_line[2], "   RIGHT  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        state = WHITE_BOARD;
    // block marker
    }
    // otherwise handle the other case
    else if (abs(ir_cur) > BL_SENSOR_DIFF_TICKS)
    // block marker
    {
        // check if this condition is true
        if (ir_cur > 0)
        // block marker
        {
            // call a helper function
            line_turn_left_slow();
            // call a helper function
            strcpy(display_line[2], "   LEFT   ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            state = LEFT;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // call a helper function
            line_turn_right_slow();
            // call a helper function
            strcpy(display_line[2], "   RIGHT  ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            state = RIGHT;
        // block marker
        }
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // call a helper function
        line_forward_slow();
        // call a helper function
        strcpy(display_line[2], "   BLACK  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        state = BLACK_LINE;
    // block marker
    }
// block marker
}



// function used by the car
void hold_black_line(void)
// block marker
{
    // variable or helper used by the car
    unsigned char left_black;
    // variable or helper used by the car
    unsigned char right_black;

    // update a value used by the program
    left_black  = Circle_Left_On_Black();
    // update a value used by the program
    right_black = Circle_Right_On_Black();

    // check if this condition is true
    if (left_black == FALSE && right_black == FALSE)
    // block marker
    {
        // code line for the car logic
        lost_line_count++;
        // check if this condition is true
        if (lost_line_count >= BL_LOST_STOP_TICKS)
        // block marker
        {
            // call a helper function
            circle_turn_right_slow();
            // call a helper function
            strcpy(display_line[2], "   RIGHT  ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            state = WHITE_BOARD;
        // block marker
        }
        // send the result back
        return;
    // block marker
    }

    // update a value used by the program
    lost_line_count = BEGINNING;
    // call a helper function
    circle_follow_black();
// block marker
}


// function used by the car
static unsigned char Left_On_Black(void)
// block marker
{
    // send the result back
    return (ADC_Left_Detect >= black_left) ? TRUE : FALSE;
// block marker
}


// function used by the car
static unsigned char Right_On_Black(void)
// block marker
{
    // send the result back
    return (ADC_Right_Detect >= black_right) ? TRUE : FALSE;
// block marker
}


// function used by the car
static unsigned char Circle_Left_On_Black(void)
// block marker
{
    // send the result back
    return (ADC_Left_Detect >= (black_left - BL_BLACK_MARGIN)) ? TRUE : FALSE;
// block marker
}


// function used by the car
static unsigned char Circle_Right_On_Black(void)
// block marker
{
    // send the result back
    return (ADC_Right_Detect >= (black_right - BL_BLACK_MARGIN)) ? TRUE : FALSE;
// block marker
}


// function used by the car
static void circle_follow_black(void)
// block marker
{
    // variable or helper used by the car
    int ir_cur;

    // update a value used by the program
    ir_cur = ADC_Left_Detect - ADC_Right_Detect;

    // check if this condition is true
    if (abs(ir_cur) > BL_CIRCLE_DIFF_TICKS)
    // block marker
    {
        // check if this condition is true
        if (ir_cur > 0)
        // block marker
        {
            // call a helper function
            circle_turn_left_slow();
            // call a helper function
            strcpy(display_line[2], "   LEFT   ");
            // update a value used by the program
            state = LEFT;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // call a helper function
            circle_turn_right_slow();
            // call a helper function
            strcpy(display_line[2], "   RIGHT  ");
            // update a value used by the program
            state = RIGHT;
        // block marker
        }
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // call a helper function
        circle_forward_slow();
        // call a helper function
        strcpy(display_line[2], "   BLACK  ");
        // update a value used by the program
        state = BLACK_LINE;
    // block marker
    }

    // update a value used by the program
    display_changed = TRUE;
// block marker
}


// function used by the car
static void circle_turn_right_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_20;
// block marker
}


// function used by the car
static void circle_turn_left_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_20;
// block marker
}


// function used by the car
static void circle_forward_slow(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // BL Circle slightly faster. BL Travel is unchanged.
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_18;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_22;
// block marker
}


// function used by the car
static void intercept_approach(void)
// block marker
{
    // update a value used by the program
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    // update a value used by the program
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // update a value used by the program
    RIGHT_FORWARD_SPEED = PERCENT_35;
    // update a value used by the program
    LEFT_FORWARD_SPEED  = PERCENT_27;
// block marker
}


// function used by the car
static void Display_Final_Stop(void)
// block marker
{
    // variable or helper used by the car
    unsigned int course_seconds;

    // update a value used by the program
    course_seconds = seconds / TICKS_PER_SECOND;

    // update a value used by the program
    final_display_lock = TRUE;

    // call a helper function
    strcpy(display_line[0], " BL Stop  ");
    // call a helper function
    strcpy(display_line[1], "Kaushik A ");
    // call a helper function
    strcpy(display_line[2], "Complete ");

    // update a value used by the program
    display_line[3][0]  = 'T';
    // update a value used by the program
    display_line[3][1]  = 'i';
    // update a value used by the program
    display_line[3][2]  = 'm';
    // update a value used by the program
    display_line[3][3]  = 'e';
    // update a value used by the program
    display_line[3][4]  = ':';
    // update a value used by the program
    display_line[3][5]  = ' ';
    // update a value used by the program
    display_line[3][6]  = ASCII_ZERO + (char)((course_seconds / 100) % DECIMAL_SHIFT);
    // update a value used by the program
    display_line[3][7]  = ASCII_ZERO + (char)((course_seconds / 10)  % DECIMAL_SHIFT);
    // update a value used by the program
    display_line[3][8]  = ASCII_ZERO + (char)(course_seconds % DECIMAL_SHIFT);
    // update a value used by the program
    display_line[3][9]  = 's';
    // update a value used by the program
    display_line[3][10] = NULL_CHAR;
// block marker
}


// function used by the car
void exit_sequence(void)
// block marker
{
    // choose what to do based on the state
    switch (exit_state)
    // block marker
    {
    // state or command case
    case 0:                          // BL Exit - turn right EXIT_TURN_TICKS
        // call a helper function
        strcpy(display_line[0], " BL Exit  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        turn_right();
        // check if this condition is true
        if (exit_timer >= EXIT_TURN_TICKS)
        // block marker
        {
            // update a value used by the program
            exit_timer = BEGINNING;
            // update a value used by the program
            exit_state = 1;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 1:                          // forward EXIT_FORWARD_TICKS
        // call a helper function
        strcpy(display_line[0], " BL Exit  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // loop through the values needed here
        forward_movement();
        // check if this condition is true
        if (exit_timer >= EXIT_FORWARD_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            exit_timer = BEGINNING;
            // update a value used by the program
            exit_state = 2;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 2:                          // BL Stop
        // call a helper function
        stop_movement();

        // Stay in the final exit state so the LCD keeps showing STOP
        // instead of being overwritten by the normal status display.
        // update a value used by the program
        move_active       = TRUE;
        // update a value used by the program
        current_direction = CMD_EXIT;

        // call a helper function
        Display_Final_Stop();
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // code line for the car logic
        break;

    // code line for the car logic
    default:
        // call a helper function
        stop_movement();
        // code line for the car logic
        break;
    // block marker
    }
// block marker
}


// function used by the car
void pad_8(void)
// block marker
{
    // choose what to do based on the state
    switch (pad8_state)
    // block marker
    {
    // state or command case
    case 0:                          // show BL Start briefly before moving
        // call a helper function
        stop_movement();
        // call a helper function
        strcpy(display_line[0], " BL Start ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // check if this condition is true
        if (Timer_counter >= BL_START_SETTLE_TICKS)
        // block marker
        {
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 1;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 1:                          // start toward line, then enable sensors
        // call a helper function
        strcpy(display_line[0], " BL Start ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        intercept_approach();

        // check if this condition is true
        if (Timer_counter >= BL_SENSOR_IGNORE_TICKS)
        // block marker
        {
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 2;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 2:                          // now find black
        // call a helper function
        strcpy(display_line[0], " BL Start ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;

        // Check sensors BEFORE commanding more forward motion.
        // This makes the car stop as soon as black is detected.
        // check if this condition is true
        if (Left_On_Black() == TRUE || Right_On_Black() == TRUE)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state    = 3;
            // code line for the car logic
            break;
        // block marker
        }

        // call a helper function
        intercept_approach();
        // code line for the car logic
        break;

    // state or command case
    case 3:                          // Intercept - stop BL_PAUSE_TICKS
        // call a helper function
        stop_movement();
        // call a helper function
        strcpy(display_line[0], " Intercept");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // check if this condition is true
        if (Timer_counter >= BL_PAUSE_TICKS)
        // block marker
        {
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            intercept_lock_count = BEGINNING;
            // update a value used by the program
            pad8_state = 4;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 4:                          // move directly into line-following turn
        // call a helper function
        stop_movement();
        // update a value used by the program
        Timer_counter = BEGINNING;
        // update a value used by the program
        pad8_state = 6;
        // code line for the car logic
        break;

    // state or command case
    case 5:                          // tiny final BL Turn alignment nudge
        // call a helper function
        strcpy(display_line[0], " BL Turn  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;

        // check if this condition is true
        if (last_bl_turn_direction == LEFT)
        // block marker
        {
            // call a helper function
            line_turn_left_slow();
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // call a helper function
            line_turn_right_slow();
        // block marker
        }

        // check if this condition is true
        if (Timer_counter >= BL_TURN_EXTRA_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 7;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 6:                          // BL Turn - use line follower to settle on line
        // call a helper function
        strcpy(display_line[0], " BL Turn  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        detect_black();

        // Remember the last correction direction so the tiny final nudge
        // continues the same way the car was already turning.
        // check if this condition is true
        if (state == LEFT || state == RIGHT)
        // block marker
        {
            // update a value used by the program
            last_bl_turn_direction = state;
        // block marker
        }

        // check if this condition is true
        if (Left_On_Black() == TRUE && Right_On_Black() == TRUE)
        // block marker
        {
            // code line for the car logic
            intercept_lock_count++;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // update a value used by the program
            intercept_lock_count = BEGINNING;
        // block marker
        }

        // check if this condition is true
        if (intercept_lock_count >= BL_INTERCEPT_LOCK_TICKS ||
            // update a value used by the program
            (Timer_counter >= BL_TURN_MAX_TICKS &&
             // update a value used by the program
             state == BLACK_LINE &&
             // update a value used by the program
             (Left_On_Black() == TRUE || Right_On_Black() == TRUE)))
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            intercept_lock_count = BEGINNING;
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 5;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 7:                          // BL Turn stop - BL_PAUSE_TICKS
        // call a helper function
        stop_movement();
        // call a helper function
        strcpy(display_line[0], " BL Turn  ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // check if this condition is true
        if (Timer_counter >= BL_PAUSE_TICKS)
        // block marker
        {
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 8;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 8:                          // BL Travel - follow line before circle
        // call a helper function
        strcpy(display_line[0], " BL Travel");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        detect_black();
        // check if this condition is true
        if (Timer_counter >= BL_TRAVEL_TICKS)
        // block marker
        {
            // call a helper function
            stop_movement();
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 9;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 9:                          // BL Circle stop - BL_PAUSE_TICKS
        // call a helper function
        stop_movement();
        // call a helper function
        strcpy(display_line[0], " BL Circle");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // check if this condition is true
        if (Timer_counter >= BL_PAUSE_TICKS)
        // block marker
        {
            // update a value used by the program
            Timer_counter = BEGINNING;
            // update a value used by the program
            pad8_state = 10;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 10:                         // BL Circle - follow circle indefinitely
        // call a helper function
        strcpy(display_line[0], " BL Circle");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        hold_black_line();
        // code line for the car logic
        break;

    // code line for the car logic
    default:
        // call a helper function
        stop_movement();
        // code line for the car logic
        break;
    // block marker
    }
// block marker
}


// variable or helper used by the car
unsigned int ASCII_Digits_To_Uint(const char *str, unsigned int offset)
// block marker
{
    // variable or helper used by the car
    unsigned int result = BEGINNING;
    // variable or helper used by the car
    unsigned int i;
    // variable or helper used by the car
    char         c;

    // loop through the values needed here
    for (i = BEGINNING; i < NUM_TIME_DIGITS; i++)
    // block marker
    {
        // update a value used by the program
        c = str[offset + i];
        // check if this condition is true
        if (c >= ASCII_ZERO && c <= ASCII_NINE)
        // block marker
        {
            // update a value used by the program
            result = (result * DECIMAL_SHIFT) + (c - ASCII_ZERO);
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // code line for the car logic
            break;
        // block marker
        }
    // block marker
    }
    // send the result back
    return result;
// block marker
}


// variable or helper used by the car
unsigned char Validate_PIN(const char *payload)
// block marker
{
    // check if this condition is true
    if (strncmp(&payload[PIN_OFFSET], IOT_PIN, IOT_PIN_LENGTH) == BEGINNING)
    // block marker
    {
        // send the result back
        return TRUE;
    // block marker
    }
    // send the result back
    return FALSE;
// block marker
}


// function used by the car
void Execute_Forward(unsigned int time_units)
// block marker
{
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_FORWARD;
    // call a helper function
    fast_forward_movement();
    // update a value used by the program
    move_time_remaining = time_units * TICKS_PER_SECOND;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Reverse(unsigned int time_units)
// block marker
{
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_BACKWARD;
    // call a helper function
    fast_reverse_movement();
    // update a value used by the program
    move_time_remaining = time_units * TICKS_PER_SECOND;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Right(unsigned int time_units)
// block marker
{
    // variable or helper used by the car
    unsigned int turn_ticks;

    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction = CMD_RIGHT;
    // call a helper function
    fast_turn_right_movement();

    // update a value used by the program
    turn_ticks = time_units * TICKS_PER_SECOND;
    // check if this condition is true
    if (turn_ticks > TURN_DURATION_TRIM_TICKS)
    // block marker
    {
        // update a value used by the program
        move_time_remaining = turn_ticks - TURN_DURATION_TRIM_TICKS;
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // update a value used by the program
        move_time_remaining = turn_ticks;
    // block marker
    }

    // update a value used by the program
    move_active = TRUE;
// block marker
}

// function used by the car
void Execute_Left(unsigned int time_units)
// block marker
{
    // variable or helper used by the car
    unsigned int turn_ticks;

    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction = CMD_LEFT;
    // call a helper function
    fast_turn_left_movement();

    // update a value used by the program
    turn_ticks = time_units * TICKS_PER_SECOND;
    // check if this condition is true
    if (turn_ticks > TURN_DURATION_TRIM_TICKS)
    // block marker
    {
        // update a value used by the program
        move_time_remaining = turn_ticks - TURN_DURATION_TRIM_TICKS;
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // update a value used by the program
        move_time_remaining = turn_ticks;
    // block marker
    }

    // update a value used by the program
    move_active = TRUE;
// block marker
}

// function used by the car
void Execute_Detect(unsigned int time_units)
// block marker
{
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_DETECT;
    // update a value used by the program
    count               = BEGINNING;
    // update a value used by the program
    pad8_state          = BEGINNING;
    // update a value used by the program
    found_white         = FALSE;
    // update a value used by the program
    Timer_counter       = BEGINNING;
    // update a value used by the program
    lost_line_count     = BEGINNING;
    // update a value used by the program
    intercept_lock_count = BEGINNING;
    // update a value used by the program
    bl_seconds          = BEGINNING;
    // update a value used by the program
    move_time_remaining = 0xFFFF;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Circle(unsigned int time_units)
// block marker
{
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_CIRCLE;
    // call a helper function
    circle();
    // update a value used by the program
    move_time_remaining = time_units * TICKS_PER_SECOND;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Arc(unsigned int time_units)
// block marker
{
    // code line for the car logic
    (void)time_units;
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_ARC;
    // update a value used by the program
    arc_state           = BEGINNING;
    // update a value used by the program
    Timer_counter       = BEGINNING;
    // update a value used by the program
    move_time_remaining = 0xFFFF;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Arc_V(unsigned int time_units)
// block marker
{
    // code line for the car logic
    (void)time_units;
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_ARC_V;
    // update a value used by the program
    arc_state_v         = BEGINNING;
    // update a value used by the program
    Timer_counter       = BEGINNING;
    // update a value used by the program
    move_time_remaining = 0xFFFF;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Pad(unsigned int time_units)
// block marker
{
    // code line for the car logic
    (void)time_units;
    // call a helper function
    stop_movement();
    // update a value used by the program
    current_direction   = CMD_PAD;
    // update a value used by the program
    pad_forward_state   = BEGINNING;
    // update a value used by the program
    Timer_counter       = BEGINNING;
    // update a value used by the program
    move_time_remaining = 0xFFFF;
    // update a value used by the program
    move_active         = TRUE;
// block marker
}

// function used by the car
void Execute_Arrive(unsigned int pad_num)
// block marker
{
    // call a helper function
    stop_movement();

    // update a value used by the program
    current_direction   = NULL_CHAR;
    // update a value used by the program
    move_active         = FALSE;
    // update a value used by the program
    move_time_remaining = BEGINNING;
    // update a value used by the program
    pending_direction   = NULL_CHAR;
    // update a value used by the program
    pending_time        = BEGINNING;

    // call a helper function
    strcpy(display_line[0], "Arrived 00");
    // call a helper function
    strcpy(display_line[1], "Kaushik A ");
    // call a helper function
    strcpy(display_line[2], "          ");

    // check if this condition is true
    if (pad_num <= PAD_MAX)
    // block marker
    {
        // update a value used by the program
        display_line[0][9] = ASCII_ZERO + (char)(pad_num % DECIMAL_SHIFT);
        // update a value used by the program
        display_line[2][0] = ' ';
        // update a value used by the program
        display_line[2][1] = ' ';
        // update a value used by the program
        display_line[2][2] = 'P';
        // update a value used by the program
        display_line[2][3] = 'a';
        // update a value used by the program
        display_line[2][4] = 'd';
        // update a value used by the program
        display_line[2][5] = ' ';
        // update a value used by the program
        display_line[2][6] = ASCII_ZERO + (char)(pad_num % DECIMAL_SHIFT);
        // update a value used by the program
        display_line[2][7] = ' ';
        // update a value used by the program
        display_line[2][8] = ' ';
        // update a value used by the program
        display_line[2][9] = ' ';
        // update a value used by the program
        display_line[2][10] = NULL_CHAR;
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // update a value used by the program
        display_line[0][9] = '?';
        // call a helper function
        strcpy(display_line[2], "  Pad ?   ");
    // block marker
    }

    // update a value used by the program
    display_changed = TRUE;
    // update a value used by the program
    update_display  = TRUE;
// block marker
}

// function used by the car
void Execute_Exit(void)
// block marker
{
    // Highest-priority exit:
    // cancel any running/pending command and immediately start the exit sequence.
    // call a helper function
    stop_movement();

    // update a value used by the program
    pending_direction     = NULL_CHAR;
    // update a value used by the program
    pending_time          = BEGINNING;

    // update a value used by the program
    current_direction     = CMD_EXIT;
    // update a value used by the program
    exit_state            = BEGINNING;
    // update a value used by the program
    exit_timer            = BEGINNING;
    // update a value used by the program
    Timer_counter         = BEGINNING;

    // update a value used by the program
    pad8_state            = BEGINNING;
    // update a value used by the program
    count                 = BEGINNING;
    // update a value used by the program
    found_white           = FALSE;
    // update a value used by the program
    lost_line_count       = BEGINNING;
    // update a value used by the program
    intercept_lock_count  = BEGINNING;

    // update a value used by the program
    move_time_remaining   = 0xFFFF;
    // update a value used by the program
    move_active           = TRUE;

    // call a helper function
    strcpy(display_line[0], " BL Exit  ");
    // update a value used by the program
    display_changed = TRUE;
    // update a value used by the program
    update_display  = TRUE;
// block marker
}

// function used by the car
void Execute_Stop(void)
// block marker
{
    // Highest-priority stop:
    // kill PWM and cancel every active/pending movement immediately.
    // call a helper function
    stop_movement();

    // update a value used by the program
    current_direction     = NULL_CHAR;
    // update a value used by the program
    move_active           = FALSE;
    // update a value used by the program
    move_time_remaining   = BEGINNING;

    // update a value used by the program
    pending_direction     = NULL_CHAR;
    // update a value used by the program
    pending_time          = BEGINNING;

    // update a value used by the program
    pad8_state            = BEGINNING;
    // update a value used by the program
    exit_state            = BEGINNING;
    // update a value used by the program
    exit_timer            = BEGINNING;
    // update a value used by the program
    Timer_counter         = BEGINNING;

    // update a value used by the program
    count                 = BEGINNING;
    // update a value used by the program
    found_white           = FALSE;
    // update a value used by the program
    lost_line_count       = BEGINNING;
    // update a value used by the program
    intercept_lock_count  = BEGINNING;
    // update a value used by the program
    bl_seconds            = BEGINNING;

    // call a helper function
    strcpy(display_line[0], "   Stop   ");
    // update a value used by the program
    display_changed = TRUE;
    // update a value used by the program
    update_display  = TRUE;
// block marker
}
