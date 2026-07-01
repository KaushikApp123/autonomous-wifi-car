/*
 * Kaushik A
 * ECE 306 Project 10
 */
// include file needed by this file
#include "msp430.h"
// include file needed by this file
#include <string.h>
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

// call a helper function
void main(void);
// call a helper function
void Init_Conditions(void);
// call a helper function
void Display_Process(void);
// call a helper function
void Init_LEDs(void);
// call a helper function
void Process_FRAM_Command(void);
// call a helper function
void Process_IOT_WiFi_Command(void);
// call a helper function
void Display_IP_Address(void);
// call a helper function
void Update_Status_Display(void);

// variable or helper used by the car
volatile char slow_input_down;
// variable or helper used by the car
char display_line[4][11];
// variable or helper used by the car
char *display[4];
// variable or helper used by the car
unsigned char display_mode;
// variable or helper used by the car
volatile unsigned char display_changed;
// variable or helper used by the car
volatile unsigned char update_display;
// variable or helper used by the car
volatile unsigned int update_display_count;
// variable or helper used by the car
volatile unsigned int Time_Sequence;
// variable or helper used by the car
volatile unsigned int Timer_counter;
// variable or helper used by the car
unsigned int status_display_second = 0xFFFF;
// variable or helper used by the car
volatile char one_time;
// variable or helper used by the car
unsigned int test_value;
// variable or helper used by the car
char chosen_direction;
// variable or helper used by the car
char change;
// variable or helper used by the car
unsigned char event;
// variable comes from another file
extern unsigned char flash_backlight;
// variable comes from another file
extern volatile unsigned char keep_display_off;
// variable comes from another file
extern volatile unsigned char sw1_pressed;
// variable comes from another file
extern volatile unsigned char sw2_pressed;
// variable or helper used by the car
char adc_char[4];
// variable or helper used by the car
int ADC_Thumb;
// variable or helper used by the car
int ADC_Left_Detect;
// variable or helper used by the car
int ADC_Right_Detect;
// variable or helper used by the car
unsigned int wheel_move;
// variable or helper used by the car
char forward;

// variable or helper used by the car
volatile unsigned int Last_Time_Sequence;
// variable or helper used by the car
volatile unsigned int cycle_time;
// variable or helper used by the car
volatile unsigned int time_change;
// variable or helper used by the car
volatile unsigned int myTime;

// variable comes from another file
extern volatile unsigned int seconds;
// variable comes from another file
extern volatile unsigned int bl_seconds;
// variable comes from another file
extern volatile unsigned int exit_timer;

// variable or helper used by the car
volatile char iot_receive_complete                = FALSE;
// variable or helper used by the car
volatile char current_command[MAX_COMMAND_LENGTH] = {NULL_CHAR};
// variable or helper used by the car
unsigned int  return_to_waiting_timer             = BEGINNING;

// variable comes from another file
extern volatile unsigned char pc_tx_enabled;
// variable comes from another file
extern volatile unsigned char fram_cmd_active;
// variable comes from another file
extern volatile char USB_Ring_Rx[RING_BUF_SIZE];
// variable comes from another file
extern volatile unsigned int  usb_rx_wr;
// variable comes from another file
extern volatile unsigned int  iot_rx_wr;
// variable comes from another file
extern volatile char          pending_direction;
// variable comes from another file
extern volatile unsigned int  pending_time;

// store IP split across two display lines
// variable or helper used by the car
char ip_line1[11] = "          ";
// variable or helper used by the car
char ip_line2[11] = "          ";

// last command display
// variable or helper used by the car
char last_cmd_display[11] = "----- 000s";
// variable or helper used by the car
unsigned char final_display_lock = FALSE;


// function used by the car
void main(void)
// block marker
{

    // update a value used by the program
    PM5CTL0 &= ~LOCKLPM5;

    // call a helper function
    Init_Ports();
    // call a helper function
    Init_Clocks();
    // call a helper function
    Init_Conditions();
    // call a helper function
    Init_Timers();
    // call a helper function
    __enable_interrupt();

    // update a value used by the program
    Timer_counter = BEGINNING;
    // loop until the condition changes
    while (Timer_counter < 5);

    // call a helper function
    Init_LCD();

    // loop until the condition changes
    while (Timer_counter < 10);
    // call a helper function
    Init_ADC();

    // loop until the condition changes
    while (Timer_counter < 15);
    // call a helper function
    Init_Serial_UCA1(BAUD_115200);
    // call a helper function
    Init_Serial_UCA0(BAUD_115200);

    // loop until the condition changes
    while (Timer_counter < 20);
    // call a helper function
    Init_DAC();

    // loop until the condition changes
    while (Timer_counter < 25);
    // update a value used by the program
    P3OUT |= IOT_EN_CPU;

    // wait for IOT to boot
    // loop until the condition changes
    while (Timer_counter < 175);
    // update a value used by the program
    IOT_Transmit_String("AT+SYSSTORE=0");

    // loop until the condition changes
    while (Timer_counter < 185);
    // update a value used by the program
    IOT_Transmit_String("AT+CIPMUX=1");

    // loop until the condition changes
    while (Timer_counter < 195);
    // update a value used by the program
    IOT_Transmit_String("AT+CIPSERVER=1,1055");

    // loop until the condition changes
    while (Timer_counter < 205);

    // request IP address
    // call a helper function
    IOT_Transmit_String("AT+CIFSR");

    // wait until we get a response containing STAIP
    // update a value used by the program
    Timer_counter = BEGINNING;
    // loop until the condition changes
    while (Timer_counter < 500)
    // block marker
    {
        // check if this condition is true
        if (iot_receive_complete == TRUE)
        // block marker
        {
            // update a value used by the program
            iot_receive_complete = FALSE;
            // check if this condition is true
            if (strstr((char*)current_command, "STAIP") != NULL)
            // block marker
            {
                // call a helper function
                Display_IP_Address();
                // call a helper function
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                // update a value used by the program
                iot_rx_wr = BEGINNING;
                // code line for the car logic
                break;
            // block marker
            }
            // call a helper function
            memset((char*)current_command, NULL_CHAR, sizeof(current_command));
            // update a value used by the program
            iot_rx_wr = BEGINNING;
        // block marker
        }
    // block marker
    }

    // if we timed out show no IP
    // check if this condition is true
    if (Timer_counter >= 500)
    // block marker
    {
        // call a helper function
        strcpy(display_line[0], " Waiting  ");
        // call a helper function
        strcpy(display_line[0], "for input ");
        // call a helper function
        strcpy(display_line[2], "  No  IP  ");
        // call a helper function
        strcpy(display_line[3], "          ");
        // call a helper function
        strcpy(ip_line1, "  No  IP  ");
        // call a helper function
        strcpy(ip_line2, "          ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // call a helper function
        Display_Update(RESET_STATE, RESET_STATE, RESET_STATE, RESET_STATE);
    // block marker
    }

    // update a value used by the program
    seconds    = BEGINNING;
    // update a value used by the program
    bl_seconds = BEGINNING;
    // update a value used by the program
    exit_timer = BEGINNING;

    // update a value used by the program
    wheel_move = BEGINNING;
    // loop through the values needed here
    forward    = TRUE;

    // loop until the condition changes
    while (ALWAYS)
    // block marker
    {
        // call a helper function
        Switches_Process();

        // Check incoming commands before updating the LCD.
        // This keeps S/X and all WiFi/USB commands responsive even when the display is active.
        // check if this condition is true
        if (iot_receive_complete == TRUE)
        // block marker
        {
            // update a value used by the program
            iot_receive_complete = FALSE;

            // check if this condition is true
            if (current_command[BEGINNING] == NULL_CHAR ||
                // update a value used by the program
                current_command[BEGINNING] == CARRIAGE_RETURN ||
                // update a value used by the program
                current_command[BEGINNING] == LINE_FEED)
            // block marker
            {
                // call a helper function
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                // update a value used by the program
                iot_rx_wr = BEGINNING;
            // block marker
            }
            // otherwise handle the other case
            else if (strncmp((char*)current_command, IPD_PREFIX, IPD_PREFIX_LEN) == BEGINNING)
            // block marker
            {
                // call a helper function
                Process_IOT_WiFi_Command();
                // call a helper function
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                // update a value used by the program
                iot_rx_wr = BEGINNING;
            // block marker
            }
            // otherwise handle the other case
            else
            // block marker
            {
                // call a helper function
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                // update a value used by the program
                iot_rx_wr = BEGINNING;
            // block marker
            }
        // block marker
        }

        // check if this condition is true
        if ((usb_rx_wr > BEGINNING) &&
            // update a value used by the program
            (USB_Ring_Rx[BEGINNING] == CMD_START_CHAR) &&
            // update a value used by the program
            (USB_Ring_Rx[usb_rx_wr - ONE_BYTE] == CARRIAGE_RETURN))
        // block marker
        {
            // call a helper function
            Process_FRAM_Command();
            // call a helper function
            memset((char*)USB_Ring_Rx, NULL_CHAR, RING_BUF_SIZE);
            // update a value used by the program
            usb_rx_wr = BEGINNING;
        // block marker
        }

        // call a helper function
        Display_Process();
        // update a value used by the program
        P3OUT ^= TEST_PROBE;
        // Do not refresh IP while the car is driving; let movement states control LCD.
// CMD_DETECT runs pad_8 state machine
        // check if this condition is true
        if (move_active == TRUE && current_direction == CMD_DETECT)
        // block marker
        {
            // call a helper function
            pad_8();
        // block marker
        }

        // CMD_EXIT runs exit sequence
        // check if this condition is true
        if (move_active == TRUE && current_direction == CMD_EXIT)
        // block marker
        {
            // call a helper function
            exit_sequence();
        // block marker
        }


        // CMD_ARC / U command
        // check if this condition is true
        if (move_active == TRUE && current_direction == CMD_ARC)
        // block marker
        {
            // call a helper function
            arc_sequence();
        // block marker
        }

        // CMD_ARC_V / V command: same arc, alt forward distances
        // check if this condition is true
        if (move_active == TRUE && current_direction == CMD_ARC_V)
        // block marker
        {
            // call a helper function
            arc_sequence_v();
        // block marker
        }

        // CMD_PAD / P command: straight to pad 8, stop, display arrived
        // check if this condition is true
        if (move_active == TRUE && current_direction == CMD_PAD)
        // block marker
        {
            // call a helper function
            pad_forward_sequence();
        // block marker
        }

        // check if this condition is true
        if ((RIGHT_REVERSE_SPEED > BEGINNING) && (RIGHT_FORWARD_SPEED > BEGINNING))
        // block marker
        {
            // update a value used by the program
            P1OUT |= RED_LED;
            // call a helper function
            stop_movement();
        // block marker
        }
        // check if this condition is true
        if ((LEFT_REVERSE_SPEED > BEGINNING) && (LEFT_FORWARD_SPEED > BEGINNING))
        // block marker
        {
            // update a value used by the program
            P1OUT |= RED_LED;
            // call a helper function
            stop_movement();
        // block marker
        }

        // check if this condition is true
        if (Last_Time_Sequence != Timer_counter)
        // block marker
        {
            // update a value used by the program
            Last_Time_Sequence = Timer_counter;
            // code line for the car logic
            cycle_time++;
            // code line for the car logic
            myTime++;
            // update a value used by the program
            time_change = TRUE;

            // Update LCD status only once per second.
            // Updating every 20ms made WiFi/USB commands feel laggy.
            // check if this condition is true
            if ((seconds / TICKS_PER_SECOND) != status_display_second)
            // block marker
            {
                // update a value used by the program
                status_display_second = seconds / TICKS_PER_SECOND;
                // call a helper function
                Update_Status_Display();
            // block marker
            }

            // increment exit_timer on every 20ms tick during exit sequence
            // check if this condition is true
            if (current_direction == CMD_EXIT)
            // block marker
            {
                // code line for the car logic
                exit_timer++;
            // block marker
            }

            // check if this condition is true
            if (move_active == TRUE && current_direction != CMD_DETECT && current_direction != CMD_EXIT && current_direction != CMD_ARC && current_direction != CMD_ARC_V && current_direction != CMD_PAD)
            // block marker
            {
                // check if this condition is true
                if (move_time_remaining > BEGINNING)
                // block marker
                {
                    // code line for the car logic
                    move_time_remaining--;
                // block marker
                }
                // otherwise handle the other case
                else
                // block marker
                {
                    // call a helper function
                    stop_movement();
                    // update a value used by the program
                    move_active       = FALSE;
                    // update a value used by the program
                    current_direction = NULL_CHAR;

                    // check if this condition is true
                    if (pending_direction != NULL_CHAR)
                    // block marker
                    {
                        // choose what to do based on the state
                        switch (pending_direction)
                        // block marker
                        {
                            // state or command case
                            case CMD_FORWARD:   strcpy(display_line[0], " Forward  "); Execute_Forward(pending_time);  break;
                            // state or command case
                            case CMD_BACKWARD:  strcpy(display_line[0], " Backward "); Execute_Reverse(pending_time);  break;
                            // state or command case
                            case CMD_RIGHT:     strcpy(display_line[0], "  Right   "); Execute_Right(pending_time);    break;
                            // state or command case
                            case CMD_LEFT:      strcpy(display_line[0], "  Left    "); Execute_Left(pending_time);     break;
                            // state or command case
                            case CMD_ARC:       strcpy(display_line[1], "   Arc U  "); Execute_Arc(pending_time);      break;
                            // state or command case
                            case CMD_ARC_V:     strcpy(display_line[1], "   Arc V  "); Execute_Arc_V(pending_time);    break;
                            // state or command case
                            case CMD_PAD:       strcpy(display_line[0], " To Pad 8 "); Execute_Pad(pending_time);      break;
                            // code line for the car logic
                            default: break;
                        // block marker
                        }
                        // update a value used by the program
                        display_changed   = TRUE;
                        // update a value used by the program
                        update_display    = TRUE;
                        // update a value used by the program
                        pending_direction = NULL_CHAR;
                        // update a value used by the program
                        pending_time      = BEGINNING;
                    // block marker
                    }
                // block marker
                }
            // block marker
            }

            // return to waiting only if no movement active
            // check if this condition is true
            if (return_to_waiting_timer > BEGINNING)
            // block marker
            {
                // send the result back
                return_to_waiting_timer--;
                // check if this condition is true
                if (return_to_waiting_timer == BEGINNING && move_active == FALSE)
                // block marker
                {
                    // call a helper function
                    strcpy(display_line[0], " Waiting  ");
                    // call a helper function
                    strcpy(display_line[2], "for input ");
                    // update a value used by the program
                    display_changed = TRUE;
                    // update a value used by the program
                    update_display  = TRUE;
                // block marker
                }
            // block marker
            }
        // block marker
        }

        // check if this condition is true
        if (keep_display_off == TRUE)
        // block marker
        {
            // update a value used by the program
            P6OUT &= ~LCD_BACKLITE;
        // block marker
        }
        // otherwise handle the other case
        else if (flash_backlight == TRUE)
        // block marker
        {
            // update a value used by the program
            flash_backlight = FALSE;
            // update a value used by the program
            P6OUT ^= LCD_BACKLITE;
        // block marker
        }
// choose what to do based on the state
switch (event)
        // block marker
        {
        // state or command case
        case OKAY:
            // check if this condition is true
            if (current_direction != CMD_EXIT && current_direction != CMD_DETECT)
            // block marker
            {
                // call a helper function
                pad_8();
            // block marker
            }
            // code line for the car logic
            break;
        // code line for the car logic
        default:
            // code line for the car logic
            break;
        // block marker
        }
    // block marker
    }
// block marker
}


// function used by the car
void Update_Status_Display(void)
// block marker
{
    // variable or helper used by the car
    unsigned int s;

    // check if this condition is true
    if (final_display_lock == TRUE)
    // block marker
    {
        // send the result back
        return;
    // block marker
    }

    // call a helper function
    strcpy(display_line[1], "Kaushik A ");

    // update a value used by the program
    s = seconds / TICKS_PER_SECOND;

    // update a value used by the program
    display_line[3][0]  = last_cmd_display[0];
    // update a value used by the program
    display_line[3][1]  = last_cmd_display[1];
    // update a value used by the program
    display_line[3][2]  = last_cmd_display[2];
    // update a value used by the program
    display_line[3][3]  = last_cmd_display[3];
    // update a value used by the program
    display_line[3][4]  = last_cmd_display[4];
    // update a value used by the program
    display_line[3][5]  = ' ';
    // update a value used by the program
    display_line[3][6]  = ASCII_ZERO + (char)((s / 100) % DECIMAL_SHIFT);
    // update a value used by the program
    display_line[3][7]  = ASCII_ZERO + (char)((s / 10)  % DECIMAL_SHIFT);
    // update a value used by the program
    display_line[3][8]  = ASCII_ZERO + (char)(s         % DECIMAL_SHIFT);
    // update a value used by the program
    display_line[3][9]  = 's';
    // update a value used by the program
    display_line[3][10] = NULL_CHAR;

    // update a value used by the program
    display_changed = TRUE;
    // update a value used by the program
    update_display  = TRUE;
// block marker
}




// function used by the car
void Display_IP_Address(void)
// block marker
{
    // variable or helper used by the car
    char         *staip_ptr;
    // variable or helper used by the car
    char         *end_ptr;
    // variable or helper used by the car
    char         *dot_ptr;
    // variable or helper used by the car
    unsigned int  i;
    // variable or helper used by the car
    unsigned int  dot_count;

    // update a value used by the program
    staip_ptr = strstr((char*)current_command, "STAIP");
    // check if this condition is true
    if (staip_ptr == NULL)
    // block marker
    {
        // call a helper function
        strcpy(display_line[0], " Waiting  ");
        // call a helper function
        strcpy(display_line[1], "Kaushik A ");
        // call a helper function
        strcpy(display_line[2], "  No  IP  ");
        // call a helper function
        strcpy(display_line[3], "for input ");
        // call a helper function
        strcpy(ip_line1, "  No  IP  ");
        // call a helper function
        strcpy(ip_line2, "          ");
        // update a value used by the program
        display_changed = TRUE;
        // update a value used by the program
        update_display  = TRUE;
        // send the result back
        return;
    // block marker
    }

    // update a value used by the program
    staip_ptr += 7;

    // update a value used by the program
    end_ptr = strchr(staip_ptr, '"');
    // check if this condition is true
    if (end_ptr != NULL)
    // block marker
    {
        *end_ptr = NULL_CHAR;
    // block marker
    }

    // update a value used by the program
    dot_count = BEGINNING;
    // update a value used by the program
    dot_ptr   = staip_ptr;
    // loop until the condition changes
    while (*dot_ptr != NULL_CHAR)
    // block marker
    {
        // check if this condition is true
        if (*dot_ptr == '.')
        // block marker
        {
            // code line for the car logic
            dot_count++;
            // check if this condition is true
            if (dot_count == 2)
            // block marker
            {
                // code line for the car logic
                dot_ptr++;
                // code line for the car logic
                break;
            // block marker
            }
        // block marker
        }
        // code line for the car logic
        dot_ptr++;
    // block marker
    }

    // update a value used by the program
    i = BEGINNING;
    // loop until the condition changes
    while ((staip_ptr + i) < dot_ptr && i < DISPLAY_LINE_LEN)
    // block marker
    {
        // update a value used by the program
        ip_line1[i] = staip_ptr[i];
        // code line for the car logic
        i++;
    // block marker
    }
    // loop until the condition changes
    while (i < DISPLAY_LINE_LEN)
    // block marker
    {
        // update a value used by the program
        ip_line1[i] = ' ';
        // code line for the car logic
        i++;
    // block marker
    }
    // update a value used by the program
    ip_line1[DISPLAY_LINE_LEN] = NULL_CHAR;

    // update a value used by the program
    i = BEGINNING;
    // loop until the condition changes
    while (*dot_ptr != NULL_CHAR && i < DISPLAY_LINE_LEN)
    // block marker
    {
        // update a value used by the program
        ip_line2[i] = *dot_ptr;
        // code line for the car logic
        i++;
        // code line for the car logic
        dot_ptr++;
    // block marker
    }
    // loop until the condition changes
    while (i < DISPLAY_LINE_LEN)
    // block marker
    {
        // update a value used by the program
        ip_line2[i] = ' ';
        // code line for the car logic
        i++;
    // block marker
    }
    // update a value used by the program
    ip_line2[DISPLAY_LINE_LEN] = NULL_CHAR;

    // call a helper function
    strncpy(display_line[0], ip_line1, DISPLAY_LINE_LEN);
    // update a value used by the program
    display_line[0][DISPLAY_LINE_LEN] = NULL_CHAR;
    // call a helper function
    strcpy(display_line[1], "Kaushik A ");
    // call a helper function
    strncpy(display_line[2], ip_line2, DISPLAY_LINE_LEN);
    // update a value used by the program
    display_line[2][DISPLAY_LINE_LEN] = NULL_CHAR;
    // call a helper function
    strcpy(display_line[3], "for input ");

    // update a value used by the program
    display_changed = TRUE;
    // update a value used by the program
    update_display  = TRUE;
    // call a helper function
    Display_Update(RESET_STATE, RESET_STATE, RESET_STATE, RESET_STATE);
// block marker
}

// function used by the car
void Process_FRAM_Command(void)
// block marker
{
    // variable or helper used by the car
    char cmd_char;

    // check if this condition is true
    if (USB_Ring_Rx[BEGINNING] != CMD_START_CHAR)
    // block marker
    {
        // send the result back
        return;
    // block marker
    }

    // update a value used by the program
    cmd_char = USB_Ring_Rx[ONE_BYTE];

    // choose what to do based on the state
    switch (cmd_char)
    // block marker
    {
        // state or command case
        case CMD_START_CHAR:
            // call a helper function
            PC_Transmit_String("I'm here\r\n");
            // code line for the car logic
            break;

        // state or command case
        case 'F':
            // call a helper function
            Init_Serial_UCA0(BAUD_115200);
            // call a helper function
            PC_Transmit_String("115,200\r\n");
            // call a helper function
            strcpy(display_line[2], " 115,200  ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;
            // code line for the car logic
            break;

        // state or command case
        case 'S':
            // call a helper function
            Init_Serial_UCA0(BAUD_460800);
            // call a helper function
            PC_Transmit_String("460,800\r\n");
            // call a helper function
            strcpy(display_line[2], " 460,800  ");
            // update a value used by the program
            display_changed = TRUE;
            // update a value used by the program
            update_display  = TRUE;
            // code line for the car logic
            break;

        // code line for the car logic
        default:
            // call a helper function
            PC_Transmit_String("Unknown CMD\r\n");
            // code line for the car logic
            break;
    // block marker
    }
// block marker
}


// function used by the car
void Process_IOT_WiFi_Command(void)
// block marker
{
    // variable or helper used by the car
    char *colon_ptr;

    // update a value used by the program
    colon_ptr = strchr((char*)current_command, ':');
    // check if this condition is true
    if (colon_ptr == NULL)
    // block marker
    {
        // send the result back
        return;
    // block marker
    }

    // variable or helper used by the car
    char *payload = colon_ptr + ONE_BYTE;

    // check if this condition is true
    if (payload[BEGINNING] == CMD_START_CHAR)
    // block marker
    {
        // call a helper function
        Parse_Drive_Command(payload);
    // block marker
    }
// block marker
}
