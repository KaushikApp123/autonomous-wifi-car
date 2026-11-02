/*
 * Kaushik A
 * ECE 306 Project 10
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "serial.h"

extern volatile unsigned int Last_Time_Sequence;
extern volatile unsigned int cycle_time;
extern volatile unsigned int time_change;
extern volatile unsigned int myTime;
extern volatile unsigned int Timer_counter;
extern char line_detect;
unsigned int sw2_position = OKAY;
unsigned int okay_to_look_at_switch2 = OKAY;
unsigned int count_debounce_SW2;
unsigned int sw1_position;
unsigned int okay_to_look_at_switch1;
unsigned int count_debounce_SW1;
int switch_count;
extern unsigned char event;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
volatile unsigned char sw1_pressed;
volatile unsigned char sw2_pressed;
unsigned char flash_backlight;

// Calibration values used by the line follower.
// SW1 sets black thresholds. SW2 sets white/reference values.
int black       = ADC_DEFAULT_BLACK_THRESHOLD;
int black_left  = ADC_DEFAULT_BLACK_THRESHOLD;
int black_right = ADC_DEFAULT_BLACK_THRESHOLD;
int white       = ADC_DEFAULT_WHITE_THRESHOLD;
int white_left  = ADC_DEFAULT_WHITE_THRESHOLD;
int white_right = ADC_DEFAULT_WHITE_THRESHOLD;

// Raw calibration captures. The actual black_left/black_right used by
// Left_Sees_Black()/Right_Sees_Black() become midpoint thresholds after SW2.
static int raw_black_left  = ADC_DEFAULT_BLACK_THRESHOLD;
static int raw_black_right = ADC_DEFAULT_BLACK_THRESHOLD;

extern int ADC_Left_Detect;
extern int ADC_Right_Detect;

extern volatile char current_command[MAX_COMMAND_LENGTH];
extern unsigned int return_to_waiting_timer;

extern char count;
extern char pad8_state;

//------------------------------------------------------------------------------
// Write_4Digit
// Places a 0-padded 4 digit number into display_line[] starting at pos.
//------------------------------------------------------------------------------
static void Write_4Digit(char *line, unsigned int pos, int value)
{
    if (value < BEGINNING)
    {
        value = BEGINNING;
    }
    if (value > 9999)
    {
        value = 9999;
    }

    line[pos]     = ASCII_ZERO + ((value / 1000) % 10);
    line[pos + 1] = ASCII_ZERO + ((value / 100)  % 10);
    line[pos + 2] = ASCII_ZERO + ((value / 10)   % 10);
    line[pos + 3] = ASCII_ZERO + (value % 10);
}

void Switches_Process(void)
{
    Switch1_Process();
    Switch2_Process();
}

//------------------------------------------------------------------------------
// Switch1_Process
// SW1 = calibrate BLACK. Put both sensors over black tape and press SW1.
// This stores separate left/right black thresholds so each sensor is judged
// against its own calibrated value.
//------------------------------------------------------------------------------
void Switch1_Process(void)
{
    if (sw1_pressed == TRUE)
    {
        sw1_pressed = FALSE;

        // SW1 captures the RAW black-tape readings.
        // Do not subtract a fixed offset here. We will build the real threshold
        // after SW2 by taking the midpoint between black and white.
        raw_black_left  = ADC_Left_Detect;
        raw_black_right = ADC_Right_Detect;

        black_left  = raw_black_left;
        black_right = raw_black_right;
        black       = (black_left + black_right) / 2;

        strcpy(display_line[0], "Blk Raw   ");
        strcpy(display_line[1], "L:        ");
        strcpy(display_line[2], "R:        ");
        strcpy(display_line[3], "Press SW2 ");
        Write_4Digit(display_line[1], 2, raw_black_left);
        Write_4Digit(display_line[2], 2, raw_black_right);

        display_changed = TRUE;
        update_display  = TRUE;
    }
}

//------------------------------------------------------------------------------
// Switch2_Process
// SW2 = calibrate WHITE. Put both sensors over the white board/floor and press SW2.
// The current black-line follower does not need white to decide line/not-line,
// but it is kept for display/debugging and future white-board logic.
//------------------------------------------------------------------------------
void Switch2_Process(void)
{
    if (sw2_pressed == TRUE)
    {
        sw2_pressed = FALSE;

        // SW2 captures RAW white/background readings.
        white_left  = ADC_Left_Detect;
        white_right = ADC_Right_Detect;
        white       = (white_left + white_right) / 2;

        // Build real black-detection thresholds from the midpoint between
        // black and white for each sensor. This is much safer than black-100
        // when your black/white readings are close together.
        black_left  = (raw_black_left  + white_left)  / 2;
        black_right = (raw_black_right + white_right) / 2;
        black       = (black_left + black_right) / 2;

        strcpy(display_line[0], "Threshold ");
        strcpy(display_line[1], "L:        ");
        strcpy(display_line[2], "R:        ");
        strcpy(display_line[3], "SW2 Done  ");
        Write_4Digit(display_line[1], 2, black_left);
        Write_4Digit(display_line[2], 2, black_right);

        display_changed = TRUE;
        update_display  = TRUE;
    }
}
