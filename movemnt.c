/*
 * Kaushik A
 * ECE 306 Project 10
 */
#include "msp430.h"
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"
#include "adc.h"
#include "dac.h"
#include "timers.h"
#include "movement.h"
#include "serial.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern volatile unsigned int Timer_counter;
extern volatile unsigned int seconds;
extern char display_line[4][11];
extern char last_cmd_display[11];
extern unsigned char final_display_lock;
extern char *display[4];
extern int ADC_Thumb;
extern int ADC_Left_Detect;
extern int ADC_Right_Detect;
extern int black;
extern int black_left;
extern int black_right;
extern int black2;
extern int white;

char ADC_Channel;
char line_detect           = 0;
char count                 = 0;
char pad8_state            = BEGINNING;
char exit_state            = BEGINNING;
char arc_state             = BEGINNING;
char arc_state_v           = BEGINNING;
char pad_forward_state     = BEGINNING;
char current_direction     = NULL_CHAR;
char last_bl_turn_direction = RIGHT;
char found_white           = FALSE;
unsigned int lost_line_count = BEGINNING;
unsigned int intercept_lock_count = BEGINNING;
volatile int ir_count;
volatile char state;

volatile unsigned int  move_time_remaining = BEGINNING;
volatile unsigned char move_active         = FALSE;

volatile char         pending_direction    = NULL_CHAR;
volatile unsigned int pending_time         = BEGINNING;

// BL timer - only used during autonomous black line sequence
volatile unsigned int bl_seconds  = BEGINNING;

// exit timer - dedicated timer for exit sequence
volatile unsigned int exit_timer  = BEGINNING;

// forward declarations
void Execute_Exit(void);
void Execute_Stop(void);

void arc_wide_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_63;
    LEFT_FORWARD_SPEED  = PERCENT_17;
}

void arc_sequence(void)
{
    switch (arc_state)
    {
    case 0:                          // U step 1: forward 1 second
        strcpy(display_line[0], " U Forward");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();

        if (Timer_counter >= U_FORWARD_TICKS)
        {
            stop_movement();
            strcpy(display_line[0], " Reached  ");
            strcpy(display_line[3], "  Pad 8   ");
            display_changed = TRUE;
            update_display  = TRUE;
            Timer_counter = BEGINNING;
            arc_state = 1;
        }
        break;

    case 1:                          // U step 2: 90 degree left turn
        strcpy(display_line[0], " U Left1  ");
        display_changed = TRUE;
        update_display  = TRUE;
        turn_left();

        if (Timer_counter >= U_90_RIGHT_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            arc_state = 2;
        }
        break;

    case 2:                          // U step 3: forward 3 seconds
        strcpy(display_line[0], " U Fwd 2  ");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();

        if (Timer_counter >= U_MID_FORWARD_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            arc_state = 3;
        }
        break;

    case 3:                          // U step 4: 90 degree left turn
        strcpy(display_line[0], " U Left2  ");
        display_changed = TRUE;
        update_display  = TRUE;
        turn_left();

        if (Timer_counter >= U_90_RIGHT_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            arc_state = 4;
        }
        break;

    case 4:                          // U step 5: forward 2 seconds then auto BL
        strcpy(display_line[0], " U Finish ");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();

        if (Timer_counter >= U_FINAL_FORWARD_TICKS)
        {
            stop_movement();
            strcpy(display_line[0], " BL Start ");
            display_changed = TRUE;
            update_display  = TRUE;

            arc_state            = BEGINNING;
            current_direction    = CMD_DETECT;
            count                = BEGINNING;
            pad8_state           = BEGINNING;
            found_white          = FALSE;
            Timer_counter        = BEGINNING;
            lost_line_count      = BEGINNING;
            intercept_lock_count = BEGINNING;
            bl_seconds           = BEGINNING;
            move_time_remaining  = 0xFFFF;
            move_active          = TRUE;
        }
        break;

    default:
        stop_movement();
        arc_state = BEGINNING;
        current_direction = NULL_CHAR;
        move_active = FALSE;
        move_time_remaining = BEGINNING;
        break;
    }
}

// V command: same arc as U, alt forward distances
void arc_sequence_v(void)
{
    switch (arc_state_v)
    {
    case 0:
        strcpy(display_line[0], " V Forward");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();
        if (Timer_counter >= V_FORWARD_TICKS)
        {
            stop_movement();
            strcpy(display_line[0], " Reached  ");
            strcpy(display_line[3], "  Pad 8   ");
            display_changed = TRUE;
            update_display  = TRUE;
            Timer_counter = BEGINNING;
            arc_state_v = 1;
        }
        break;

    case 1:
        strcpy(display_line[0], " V WideTrn");
        display_changed = TRUE;
        update_display  = TRUE;
        arc_wide_movement();
        if (Timer_counter >= V_FORWARD_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            arc_state_v = 2;
        }
        break;

    case 2:
        strcpy(display_line[0], " V Finish ");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();
        if (Timer_counter >= V_FINAL_FORWARD_TICKS)
        {
            stop_movement();
            strcpy(display_line[0], " BL Start ");
            display_changed = TRUE;
            update_display  = TRUE;
            arc_state_v          = BEGINNING;
            current_direction    = CMD_DETECT;
            count                = BEGINNING;
            pad8_state           = BEGINNING;
            found_white          = FALSE;
            Timer_counter        = BEGINNING;
            lost_line_count      = BEGINNING;
            intercept_lock_count = BEGINNING;
            bl_seconds           = BEGINNING;
            move_time_remaining  = 0xFFFF;
            move_active          = TRUE;
        }
        break;

    default:
        stop_movement();
        arc_state_v = BEGINNING;
        current_direction = NULL_CHAR;
        move_active = FALSE;
        move_time_remaining = BEGINNING;
        break;
    }
}

// P command: drive straight 1.5 sec to pad 8, stop, display Arrived at Pad 8
void pad_forward_sequence(void)
{
    switch (pad_forward_state)
    {
    case 0:
        strcpy(display_line[0], " To Pad 8 ");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();
        if (Timer_counter >= P_FORWARD_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            pad_forward_state = 1;
        }
        break;

    case 1:
        stop_movement();
        strcpy(display_line[0], "Arrived 08");
        strcpy(display_line[1], "Kaushik A ");
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
        update_display  = TRUE;
        move_active         = FALSE;
        current_direction   = NULL_CHAR;
        move_time_remaining = BEGINNING;
        pad_forward_state   = BEGINNING;
        break;

    default:
        stop_movement();
        pad_forward_state = BEGINNING;
        current_direction = NULL_CHAR;
        move_active = FALSE;
        move_time_remaining = BEGINNING;
        break;
    }
}

void exit_sequence(void);
static unsigned char Left_On_Black(void);
static unsigned char Right_On_Black(void);
static unsigned char Circle_Left_On_Black(void);
static unsigned char Circle_Right_On_Black(void);
static void Display_Final_Stop(void);
static void circle_follow_black(void);
static void circle_turn_right_slow(void);
static void circle_turn_left_slow(void);
static void circle_forward_slow(void);

static void line_turn_right_slow(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = PERCENT_17;
}

static void line_turn_left_slow(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_17;
}

static void line_forward_slow(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // BL Travel: very tiny slowdown so it stays calmer on the line.
    // Keeps the same general left/right bias, just slightly less speed.
    RIGHT_FORWARD_SPEED = PERCENT_12_5;
    LEFT_FORWARD_SPEED  = PERCENT_34;
}

static void intercept_approach(void);

void Parse_Drive_Command(const char *payload)
{
    unsigned int  offset;
    char          direction;
    unsigned int  time_units;
    unsigned char first_done = FALSE;

    if (payload[BEGINNING] != CMD_START_CHAR)
    {
        PC_Transmit_String("BAD START\r\n");
        strcpy(display_line[0], " BADSTART ");
        display_changed = TRUE;
        update_display  = TRUE;
        return;
    }

    if (Validate_PIN(payload) == FALSE)
    {
        PC_Transmit_String("AUTH FAIL\r\n");
        strcpy(display_line[0], " PIN FAIL ");
        display_changed = TRUE;
        update_display  = TRUE;
        return;
    }

    final_display_lock = FALSE;

    // Highest-priority commands:
    // S must stop immediately, and X must exit immediately, no matter what
    // sequence is currently running.
    direction = payload[CMD_DIRECTION_OFFSET];

    if (direction == CMD_STOP)
    {
        last_cmd_display[0] = CMD_STOP;
        last_cmd_display[1] = '0';
        last_cmd_display[2] = '0';
        last_cmd_display[3] = '0';
        last_cmd_display[4] = '0';
        last_cmd_display[5] = NULL_CHAR;
        strcpy(display_line[0], "   Stop   ");
        Execute_Stop();
        display_changed = TRUE;
        update_display  = TRUE;
        return;
    }

    if (direction == CMD_EXIT)
    {
        last_cmd_display[0] = CMD_EXIT;
        last_cmd_display[1] = '0';
        last_cmd_display[2] = '0';
        last_cmd_display[3] = '0';
        last_cmd_display[4] = '0';
        last_cmd_display[5] = NULL_CHAR;
        strcpy(display_line[0], " BL Exit  ");
        Execute_Exit();
        display_changed = TRUE;
        update_display  = TRUE;
        return;
    }

    pending_direction = NULL_CHAR;
    pending_time      = BEGINNING;

    offset = CMD_DIRECTION_OFFSET;

    while (payload[offset] != NULL_CHAR &&
           payload[offset] != CARRIAGE_RETURN &&
           payload[offset] != LINE_FEED)
    {
        direction  = payload[offset];
        time_units = ASCII_Digits_To_Uint(payload, offset + ONE_BYTE);

        last_cmd_display[0] = direction;
        last_cmd_display[1] = payload[offset + 1];
        last_cmd_display[2] = payload[offset + 2];
        last_cmd_display[3] = payload[offset + 3];
        last_cmd_display[4] = payload[offset + 4];
        last_cmd_display[5] = NULL_CHAR;

        if (direction != CMD_FORWARD  &&
            direction != CMD_BACKWARD &&
            direction != CMD_RIGHT    &&
            direction != CMD_LEFT     &&
            direction != CMD_DETECT   &&
            direction != CMD_CIRCLE   &&
            direction != CMD_ARC      &&
            direction != CMD_ARC_V    &&
            direction != CMD_PAD      &&
            direction != CMD_ARRIVE   &&
            direction != CMD_ARRIVE_LEGACY &&
            direction != CMD_EXIT     &&
            direction != CMD_STOP)
        {
            break;
        }

        if (first_done == FALSE)
        {
            switch (direction)
            {
                case CMD_FORWARD:   strcpy(display_line[0], " Forward  "); Execute_Forward(time_units);  break;
                case CMD_BACKWARD:  strcpy(display_line[0], " Backward "); Execute_Reverse(time_units);  break;
                case CMD_RIGHT:     strcpy(display_line[0], "  Right   "); Execute_Right(time_units);    break;
                case CMD_LEFT:      strcpy(display_line[0], "  Left    "); Execute_Left(time_units);     break;
                case CMD_DETECT:    strcpy(display_line[0], " BL Start "); Execute_Detect(time_units);   break;
                case CMD_CIRCLE:    strcpy(display_line[0], "  Circle  "); Execute_Circle(time_units);   break;
                case CMD_ARC:       strcpy(display_line[0], "   Arc U  "); Execute_Arc(time_units);      break;
                case CMD_ARC_V:     strcpy(display_line[0], "   Arc V  "); Execute_Arc_V(time_units);    break;
                case CMD_PAD:       strcpy(display_line[0], " To Pad 8 "); Execute_Pad(time_units);      break;
                case CMD_ARRIVE:
                case CMD_ARRIVE_LEGACY: Execute_Arrive(time_units);                                      break;
                case CMD_EXIT:      strcpy(display_line[0], " BL Exit  "); Execute_Exit();               break;
                case CMD_STOP:      strcpy(display_line[0], "   Stop   "); Execute_Stop();               break;
                default:            strcpy(display_line[0], "  Unknwn  "); break;
            }
            display_changed = TRUE;
            update_display  = TRUE;
            first_done = TRUE;
        }
        else
        {
            pending_direction = direction;
            pending_time      = time_units;
            break;
        }

        offset += (ONE_BYTE + NUM_TIME_DIGITS);
    }
}

void stop_movement(void)
{
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    P1OUT ^= RED_LED;
}

void forward_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    // Veer-right fix: nudged further
    RIGHT_FORWARD_SPEED = PERCENT_45;
    LEFT_FORWARD_SPEED  = PERCENT_48;
}

void Reverse1(void)
{
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    // Veer-right fix: match forward trim
    RIGHT_REVERSE_SPEED = PERCENT_45;
    LEFT_REVERSE_SPEED  = PERCENT_48;
}

void turn_right(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = PERCENT_30;
}

void turn_left(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_30;
}

static void fast_forward_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // Fast manual F command. Left is kept slightly higher to preserve the old trim direction.
    RIGHT_FORWARD_SPEED = PERCENT_65;
    LEFT_FORWARD_SPEED  = PERCENT_70;
}

static void fast_reverse_movement(void)
{
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;

    // Fast manual B command. Same trim direction as forward.
    RIGHT_REVERSE_SPEED = PERCENT_65;
    LEFT_REVERSE_SPEED  = PERCENT_70;
}

static void fast_turn_right_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = PERCENT_45;
}

static void fast_turn_left_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_45;
}

void turn_right2(void)
{
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = FAST;
    RIGHT_REVERSE_SPEED = FAST;
}

void forward_slow_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // BL Travel slowed down more because it was still flying off
    // at the squiggly-line-to-circle transition.
    // Same general bias direction, lower total PWM.
    RIGHT_FORWARD_SPEED = PERCENT_15;
    LEFT_FORWARD_SPEED  = PERCENT_45;
}

void Reverse_movement_slow(void)
{
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = SLOW;
    LEFT_REVERSE_SPEED  = SLOW;
}

void Reverse_fast(void)
{
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = SLOW;
    LEFT_REVERSE_SPEED  = FAST;
}

void circle(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_80;
    LEFT_FORWARD_SPEED  = PERCENT_60;
}
void arc_right_movement(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    RIGHT_FORWARD_SPEED = PERCENT_70;
    LEFT_FORWARD_SPEED  = PERCENT_10;
}

void detect_black(void)
{
    int ir_cur;
    unsigned char left_black;
    unsigned char right_black;

    ir_cur = ADC_Left_Detect - ADC_Right_Detect;
    left_black  = Left_On_Black();
    right_black = Right_On_Black();

    if (left_black == FALSE && right_black == FALSE)
    {
        line_turn_right_slow();
        strcpy(display_line[2], "   RIGHT  ");
        display_changed = TRUE;
        state = WHITE_BOARD;
    }
    else if (abs(ir_cur) > BL_SENSOR_DIFF_TICKS)
    {
        if (ir_cur > 0)
        {
            line_turn_left_slow();
            strcpy(display_line[2], "   LEFT   ");
            display_changed = TRUE;
            state = LEFT;
        }
        else
        {
            line_turn_right_slow();
            strcpy(display_line[2], "   RIGHT  ");
            display_changed = TRUE;
            state = RIGHT;
        }
    }
    else
    {
        line_forward_slow();
        strcpy(display_line[2], "   BLACK  ");
        display_changed = TRUE;
        state = BLACK_LINE;
    }
}

void hold_black_line(void)
{
    unsigned char left_black;
    unsigned char right_black;

    left_black  = Circle_Left_On_Black();
    right_black = Circle_Right_On_Black();

    if (left_black == FALSE && right_black == FALSE)
    {
        lost_line_count++;
        if (lost_line_count >= BL_LOST_STOP_TICKS)
        {
            circle_turn_right_slow();
            strcpy(display_line[2], "   RIGHT  ");
            display_changed = TRUE;
            state = WHITE_BOARD;
        }
        return;
    }

    lost_line_count = BEGINNING;
    circle_follow_black();
}

static unsigned char Left_On_Black(void)
{
    return (ADC_Left_Detect >= black_left) ? TRUE : FALSE;
}

static unsigned char Right_On_Black(void)
{
    return (ADC_Right_Detect >= black_right) ? TRUE : FALSE;
}

static unsigned char Circle_Left_On_Black(void)
{
    return (ADC_Left_Detect >= (black_left - BL_BLACK_MARGIN)) ? TRUE : FALSE;
}

static unsigned char Circle_Right_On_Black(void)
{
    return (ADC_Right_Detect >= (black_right - BL_BLACK_MARGIN)) ? TRUE : FALSE;
}

static void circle_follow_black(void)
{
    int ir_cur;

    ir_cur = ADC_Left_Detect - ADC_Right_Detect;

    if (abs(ir_cur) > BL_CIRCLE_DIFF_TICKS)
    {
        if (ir_cur > 0)
        {
            circle_turn_left_slow();
            strcpy(display_line[2], "   LEFT   ");
            state = LEFT;
        }
        else
        {
            circle_turn_right_slow();
            strcpy(display_line[2], "   RIGHT  ");
            state = RIGHT;
        }
    }
    else
    {
        circle_forward_slow();
        strcpy(display_line[2], "   BLACK  ");
        state = BLACK_LINE;
    }

    display_changed = TRUE;
}

static void circle_turn_right_slow(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = PERCENT_20;
}

static void circle_turn_left_slow(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    LEFT_FORWARD_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_20;
}

static void circle_forward_slow(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;

    // BL Circle slightly faster. BL Travel is unchanged.
    RIGHT_FORWARD_SPEED = PERCENT_18;
    LEFT_FORWARD_SPEED  = PERCENT_22;
}

static void intercept_approach(void)
{
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED  = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = PERCENT_35;
    LEFT_FORWARD_SPEED  = PERCENT_27;
}

static void Display_Final_Stop(void)
{
    unsigned int course_seconds;

    course_seconds = seconds / TICKS_PER_SECOND;

    final_display_lock = TRUE;

    strcpy(display_line[0], " BL Stop  ");
    strcpy(display_line[1], "Kaushik A ");
    strcpy(display_line[2], "Complete ");

    display_line[3][0]  = 'T';
    display_line[3][1]  = 'i';
    display_line[3][2]  = 'm';
    display_line[3][3]  = 'e';
    display_line[3][4]  = ':';
    display_line[3][5]  = ' ';
    display_line[3][6]  = ASCII_ZERO + (char)((course_seconds / 100) % DECIMAL_SHIFT);
    display_line[3][7]  = ASCII_ZERO + (char)((course_seconds / 10)  % DECIMAL_SHIFT);
    display_line[3][8]  = ASCII_ZERO + (char)(course_seconds % DECIMAL_SHIFT);
    display_line[3][9]  = 's';
    display_line[3][10] = NULL_CHAR;
}

void exit_sequence(void)
{
    switch (exit_state)
    {
    case 0:                          // BL Exit - turn right EXIT_TURN_TICKS
        strcpy(display_line[0], " BL Exit  ");
        display_changed = TRUE;
        update_display  = TRUE;
        turn_right();
        if (exit_timer >= EXIT_TURN_TICKS)
        {
            exit_timer = BEGINNING;
            exit_state = 1;
        }
        break;

    case 1:                          // forward EXIT_FORWARD_TICKS
        strcpy(display_line[0], " BL Exit  ");
        display_changed = TRUE;
        update_display  = TRUE;
        forward_movement();
        if (exit_timer >= EXIT_FORWARD_TICKS)
        {
            stop_movement();
            exit_timer = BEGINNING;
            exit_state = 2;
        }
        break;

    case 2:                          // BL Stop
        stop_movement();

        // Stay in the final exit state so the LCD keeps showing STOP
        // instead of being overwritten by the normal status display.
        move_active       = TRUE;
        current_direction = CMD_EXIT;

        Display_Final_Stop();
        display_changed = TRUE;
        update_display  = TRUE;
        break;

    default:
        stop_movement();
        break;
    }
}

void pad_8(void)
{
    switch (pad8_state)
    {
    case 0:                          // show BL Start briefly before moving
        stop_movement();
        strcpy(display_line[0], " BL Start ");
        display_changed = TRUE;
        update_display  = TRUE;
        if (Timer_counter >= BL_START_SETTLE_TICKS)
        {
            Timer_counter = BEGINNING;
            pad8_state = 1;
        }
        break;

    case 1:                          // start toward line, then enable sensors
        strcpy(display_line[0], " BL Start ");
        display_changed = TRUE;
        update_display  = TRUE;
        intercept_approach();

        if (Timer_counter >= BL_SENSOR_IGNORE_TICKS)
        {
            Timer_counter = BEGINNING;
            pad8_state = 2;
        }
        break;

    case 2:                          // now find black
        strcpy(display_line[0], " BL Start ");
        display_changed = TRUE;
        update_display  = TRUE;

        // Check sensors BEFORE commanding more forward motion.
        // This makes the car stop as soon as black is detected.
        if (Left_On_Black() == TRUE || Right_On_Black() == TRUE)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            pad8_state    = 3;
            break;
        }

        intercept_approach();
        break;

    case 3:                          // Intercept - stop BL_PAUSE_TICKS
        stop_movement();
        strcpy(display_line[0], " Intercept");
        display_changed = TRUE;
        update_display  = TRUE;
        if (Timer_counter >= BL_PAUSE_TICKS)
        {
            Timer_counter = BEGINNING;
            intercept_lock_count = BEGINNING;
            pad8_state = 4;
        }
        break;

    case 4:                          // move directly into line-following turn
        stop_movement();
        Timer_counter = BEGINNING;
        pad8_state = 6;
        break;

    case 5:                          // tiny final BL Turn alignment nudge
        strcpy(display_line[0], " BL Turn  ");
        display_changed = TRUE;
        update_display  = TRUE;

        if (last_bl_turn_direction == LEFT)
        {
            line_turn_left_slow();
        }
        else
        {
            line_turn_right_slow();
        }

        if (Timer_counter >= BL_TURN_EXTRA_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            pad8_state = 7;
        }
        break;

    case 6:                          // BL Turn - use line follower to settle on line
        strcpy(display_line[0], " BL Turn  ");
        display_changed = TRUE;
        update_display  = TRUE;
        detect_black();

        // Remember the last correction direction so the tiny final nudge
        // continues the same way the car was already turning.
        if (state == LEFT || state == RIGHT)
        {
            last_bl_turn_direction = state;
        }

        if (Left_On_Black() == TRUE && Right_On_Black() == TRUE)
        {
            intercept_lock_count++;
        }
        else
        {
            intercept_lock_count = BEGINNING;
        }

        if (intercept_lock_count >= BL_INTERCEPT_LOCK_TICKS ||
            (Timer_counter >= BL_TURN_MAX_TICKS &&
             state == BLACK_LINE &&
             (Left_On_Black() == TRUE || Right_On_Black() == TRUE)))
        {
            stop_movement();
            intercept_lock_count = BEGINNING;
            Timer_counter = BEGINNING;
            pad8_state = 5;
        }
        break;

    case 7:                          // BL Turn stop - BL_PAUSE_TICKS
        stop_movement();
        strcpy(display_line[0], " BL Turn  ");
        display_changed = TRUE;
        update_display  = TRUE;
        if (Timer_counter >= BL_PAUSE_TICKS)
        {
            Timer_counter = BEGINNING;
            pad8_state = 8;
        }
        break;

    case 8:                          // BL Travel - follow line before circle
        strcpy(display_line[0], " BL Travel");
        display_changed = TRUE;
        update_display  = TRUE;
        detect_black();
        if (Timer_counter >= BL_TRAVEL_TICKS)
        {
            stop_movement();
            Timer_counter = BEGINNING;
            pad8_state = 9;
        }
        break;

    case 9:                          // BL Circle stop - BL_PAUSE_TICKS
        stop_movement();
        strcpy(display_line[0], " BL Circle");
        display_changed = TRUE;
        update_display  = TRUE;
        if (Timer_counter >= BL_PAUSE_TICKS)
        {
            Timer_counter = BEGINNING;
            pad8_state = 10;
        }
        break;

    case 10:                         // BL Circle - follow circle indefinitely
        strcpy(display_line[0], " BL Circle");
        display_changed = TRUE;
        update_display  = TRUE;
        hold_black_line();
        break;

    default:
        stop_movement();
        break;
    }
}

unsigned int ASCII_Digits_To_Uint(const char *str, unsigned int offset)
{
    unsigned int result = BEGINNING;
    unsigned int i;
    char         c;

    for (i = BEGINNING; i < NUM_TIME_DIGITS; i++)
    {
        c = str[offset + i];
        if (c >= ASCII_ZERO && c <= ASCII_NINE)
        {
            result = (result * DECIMAL_SHIFT) + (c - ASCII_ZERO);
        }
        else
        {
            break;
        }
    }
    return result;
}

unsigned char Validate_PIN(const char *payload)
{
    if (strncmp(&payload[PIN_OFFSET], IOT_PIN, IOT_PIN_LENGTH) == BEGINNING)
    {
        return TRUE;
    }
    return FALSE;
}

void Execute_Forward(unsigned int time_units)
{
    stop_movement();
    current_direction   = CMD_FORWARD;
    fast_forward_movement();
    move_time_remaining = time_units * TICKS_PER_SECOND;
    move_active         = TRUE;
}

void Execute_Reverse(unsigned int time_units)
{
    stop_movement();
    current_direction   = CMD_BACKWARD;
    fast_reverse_movement();
    move_time_remaining = time_units * TICKS_PER_SECOND;
    move_active         = TRUE;
}

void Execute_Right(unsigned int time_units)
{
    unsigned int turn_ticks;

    stop_movement();
    current_direction = CMD_RIGHT;
    fast_turn_right_movement();

    turn_ticks = time_units * TICKS_PER_SECOND;
    if (turn_ticks > TURN_DURATION_TRIM_TICKS)
    {
        move_time_remaining = turn_ticks - TURN_DURATION_TRIM_TICKS;
    }
    else
    {
        move_time_remaining = turn_ticks;
    }

    move_active = TRUE;
}

void Execute_Left(unsigned int time_units)
{
    unsigned int turn_ticks;

    stop_movement();
    current_direction = CMD_LEFT;
    fast_turn_left_movement();

    turn_ticks = time_units * TICKS_PER_SECOND;
    if (turn_ticks > TURN_DURATION_TRIM_TICKS)
    {
        move_time_remaining = turn_ticks - TURN_DURATION_TRIM_TICKS;
    }
    else
    {
        move_time_remaining = turn_ticks;
    }

    move_active = TRUE;
}

void Execute_Detect(unsigned int time_units)
{
    stop_movement();
    current_direction   = CMD_DETECT;
    count               = BEGINNING;
    pad8_state          = BEGINNING;
    found_white         = FALSE;
    Timer_counter       = BEGINNING;
    lost_line_count     = BEGINNING;
    intercept_lock_count = BEGINNING;
    bl_seconds          = BEGINNING;
    move_time_remaining = 0xFFFF;
    move_active         = TRUE;
}

void Execute_Circle(unsigned int time_units)
{
    stop_movement();
    current_direction   = CMD_CIRCLE;
    circle();
    move_time_remaining = time_units * TICKS_PER_SECOND;
    move_active         = TRUE;
}

void Execute_Arc(unsigned int time_units)
{
    (void)time_units;
    stop_movement();
    current_direction   = CMD_ARC;
    arc_state           = BEGINNING;
    Timer_counter       = BEGINNING;
    move_time_remaining = 0xFFFF;
    move_active         = TRUE;
}

void Execute_Arc_V(unsigned int time_units)
{
    (void)time_units;
    stop_movement();
    current_direction   = CMD_ARC_V;
    arc_state_v         = BEGINNING;
    Timer_counter       = BEGINNING;
    move_time_remaining = 0xFFFF;
    move_active         = TRUE;
}

void Execute_Pad(unsigned int time_units)
{
    (void)time_units;
    stop_movement();
    current_direction   = CMD_PAD;
    pad_forward_state   = BEGINNING;
    Timer_counter       = BEGINNING;
    move_time_remaining = 0xFFFF;
    move_active         = TRUE;
}

void Execute_Arrive(unsigned int pad_num)
{
    stop_movement();

    current_direction   = NULL_CHAR;
    move_active         = FALSE;
    move_time_remaining = BEGINNING;
    pending_direction   = NULL_CHAR;
    pending_time        = BEGINNING;

    strcpy(display_line[0], "Arrived 00");
    strcpy(display_line[1], "Kaushik A ");
    strcpy(display_line[2], "          ");

    if (pad_num <= PAD_MAX)
    {
        display_line[0][9] = ASCII_ZERO + (char)(pad_num % DECIMAL_SHIFT);
        display_line[2][0] = ' ';
        display_line[2][1] = ' ';
        display_line[2][2] = 'P';
        display_line[2][3] = 'a';
        display_line[2][4] = 'd';
        display_line[2][5] = ' ';
        display_line[2][6] = ASCII_ZERO + (char)(pad_num % DECIMAL_SHIFT);
        display_line[2][7] = ' ';
        display_line[2][8] = ' ';
        display_line[2][9] = ' ';
        display_line[2][10] = NULL_CHAR;
    }
    else
    {
        display_line[0][9] = '?';
        strcpy(display_line[2], "  Pad ?   ");
    }

    display_changed = TRUE;
    update_display  = TRUE;
}

void Execute_Exit(void)
{
    // Highest-priority exit:
    // cancel any running/pending command and immediately start the exit sequence.
    stop_movement();

    pending_direction     = NULL_CHAR;
    pending_time          = BEGINNING;

    current_direction     = CMD_EXIT;
    exit_state            = BEGINNING;
    exit_timer            = BEGINNING;
    Timer_counter         = BEGINNING;

    pad8_state            = BEGINNING;
    count                 = BEGINNING;
    found_white           = FALSE;
    lost_line_count       = BEGINNING;
    intercept_lock_count  = BEGINNING;

    move_time_remaining   = 0xFFFF;
    move_active           = TRUE;

    strcpy(display_line[0], " BL Exit  ");
    display_changed = TRUE;
    update_display  = TRUE;
}

void Execute_Stop(void)
{
    // Highest-priority stop:
    // kill PWM and cancel every active/pending movement immediately.
    stop_movement();

    current_direction     = NULL_CHAR;
    move_active           = FALSE;
    move_time_remaining   = BEGINNING;

    pending_direction     = NULL_CHAR;
    pending_time          = BEGINNING;

    pad8_state            = BEGINNING;
    exit_state            = BEGINNING;
    exit_timer            = BEGINNING;
    Timer_counter         = BEGINNING;

    count                 = BEGINNING;
    found_white           = FALSE;
    lost_line_count       = BEGINNING;
    intercept_lock_count  = BEGINNING;
    bl_seconds            = BEGINNING;

    strcpy(display_line[0], "   Stop   ");
    display_changed = TRUE;
    update_display  = TRUE;
}
