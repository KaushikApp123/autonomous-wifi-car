/*
 * Kaushik A
 * ECE 306 Project 10
 */
#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"
#include "adc.h"
#include "dac.h"
#include "timers.h"
#include "movement.h"
#include "serial.h"

void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Process_FRAM_Command(void);
void Process_IOT_WiFi_Command(void);
void Display_IP_Address(void);
void Update_Status_Display(void);

volatile char slow_input_down;
char display_line[4][11];
char *display[4];
unsigned char display_mode;
volatile unsigned char display_changed;
volatile unsigned char update_display;
volatile unsigned int update_display_count;
volatile unsigned int Time_Sequence;
volatile unsigned int Timer_counter;
unsigned int status_display_second = 0xFFFF;
volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
unsigned char event;
extern unsigned char flash_backlight;
extern volatile unsigned char keep_display_off;
extern volatile unsigned char sw1_pressed;
extern volatile unsigned char sw2_pressed;
char adc_char[4];
int ADC_Thumb;
int ADC_Left_Detect;
int ADC_Right_Detect;
unsigned int wheel_move;
char forward;

volatile unsigned int Last_Time_Sequence;
volatile unsigned int cycle_time;
volatile unsigned int time_change;
volatile unsigned int myTime;

extern volatile unsigned int seconds;
extern volatile unsigned int bl_seconds;
extern volatile unsigned int exit_timer;

volatile char iot_receive_complete                = FALSE;
volatile char current_command[MAX_COMMAND_LENGTH] = {NULL_CHAR};
unsigned int  return_to_waiting_timer             = BEGINNING;

extern volatile unsigned char pc_tx_enabled;
extern volatile unsigned char fram_cmd_active;
extern volatile char USB_Ring_Rx[RING_BUF_SIZE];
extern volatile unsigned int  usb_rx_wr;
extern volatile unsigned int  iot_rx_wr;
extern volatile char          pending_direction;
extern volatile unsigned int  pending_time;

// store IP split across two display lines
char ip_line1[11] = "          ";
char ip_line2[11] = "          ";

// last command display
char last_cmd_display[11] = "----- 000s";
unsigned char final_display_lock = FALSE;

void main(void)
{

    PM5CTL0 &= ~LOCKLPM5;

    Init_Ports();
    Init_Clocks();
    Init_Conditions();
    Init_Timers();
    __enable_interrupt();

    Timer_counter = BEGINNING;
    while (Timer_counter < 5);

    Init_LCD();

    while (Timer_counter < 10);
    Init_ADC();

    while (Timer_counter < 15);
    Init_Serial_UCA1(BAUD_115200);
    Init_Serial_UCA0(BAUD_115200);

    while (Timer_counter < 20);
    Init_DAC();

    while (Timer_counter < 25);
    P3OUT |= IOT_EN_CPU;

    // wait for IOT to boot
    while (Timer_counter < 175);
    IOT_Transmit_String("AT+SYSSTORE=0");

    while (Timer_counter < 185);
    IOT_Transmit_String("AT+CIPMUX=1");

    while (Timer_counter < 195);
    IOT_Transmit_String("AT+CIPSERVER=1,1055");

    while (Timer_counter < 205);

    // request IP address
    IOT_Transmit_String("AT+CIFSR");

    // wait until we get a response containing STAIP
    Timer_counter = BEGINNING;
    while (Timer_counter < 500)
    {
        if (iot_receive_complete == TRUE)
        {
            iot_receive_complete = FALSE;
            if (strstr((char*)current_command, "STAIP") != NULL)
            {
                Display_IP_Address();
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                iot_rx_wr = BEGINNING;
                break;
            }
            memset((char*)current_command, NULL_CHAR, sizeof(current_command));
            iot_rx_wr = BEGINNING;
        }
    }

    // if we timed out show no IP
    if (Timer_counter >= 500)
    {
        strcpy(display_line[0], " Waiting  ");
        strcpy(display_line[0], "for input ");
        strcpy(display_line[2], "  No  IP  ");
        strcpy(display_line[3], "          ");
        strcpy(ip_line1, "  No  IP  ");
        strcpy(ip_line2, "          ");
        display_changed = TRUE;
        update_display  = TRUE;
        Display_Update(RESET_STATE, RESET_STATE, RESET_STATE, RESET_STATE);
    }

    seconds    = BEGINNING;
    bl_seconds = BEGINNING;
    exit_timer = BEGINNING;

    wheel_move = BEGINNING;
    forward    = TRUE;

    while (ALWAYS)
    {
        Switches_Process();

        // Check incoming commands before updating the LCD.
        // This keeps S/X and all WiFi/USB commands responsive even when the display is active.
        if (iot_receive_complete == TRUE)
        {
            iot_receive_complete = FALSE;

            if (current_command[BEGINNING] == NULL_CHAR ||
                current_command[BEGINNING] == CARRIAGE_RETURN ||
                current_command[BEGINNING] == LINE_FEED)
            {
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                iot_rx_wr = BEGINNING;
            }
            else if (strncmp((char*)current_command, IPD_PREFIX, IPD_PREFIX_LEN) == BEGINNING)
            {
                Process_IOT_WiFi_Command();
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                iot_rx_wr = BEGINNING;
            }
            else
            {
                memset((char*)current_command, NULL_CHAR, sizeof(current_command));
                iot_rx_wr = BEGINNING;
            }
        }

        if ((usb_rx_wr > BEGINNING) &&
            (USB_Ring_Rx[BEGINNING] == CMD_START_CHAR) &&
            (USB_Ring_Rx[usb_rx_wr - ONE_BYTE] == CARRIAGE_RETURN))
        {
            Process_FRAM_Command();
            memset((char*)USB_Ring_Rx, NULL_CHAR, RING_BUF_SIZE);
            usb_rx_wr = BEGINNING;
        }

        Display_Process();
        P3OUT ^= TEST_PROBE;
        // Do not refresh IP while the car is driving; let movement states control LCD.
// CMD_DETECT runs pad_8 state machine
        if (move_active == TRUE && current_direction == CMD_DETECT)
        {
            pad_8();
        }

        // CMD_EXIT runs exit sequence
        if (move_active == TRUE && current_direction == CMD_EXIT)
        {
            exit_sequence();
        }

        // CMD_ARC / U command
        if (move_active == TRUE && current_direction == CMD_ARC)
        {
            arc_sequence();
        }

        // CMD_ARC_V / V command: same arc, alt forward distances
        if (move_active == TRUE && current_direction == CMD_ARC_V)
        {
            arc_sequence_v();
        }

        // CMD_PAD / P command: straight to pad 8, stop, display arrived
        if (move_active == TRUE && current_direction == CMD_PAD)
        {
            pad_forward_sequence();
        }

        if ((RIGHT_REVERSE_SPEED > BEGINNING) && (RIGHT_FORWARD_SPEED > BEGINNING))
        {
            P1OUT |= RED_LED;
            stop_movement();
        }
        if ((LEFT_REVERSE_SPEED > BEGINNING) && (LEFT_FORWARD_SPEED > BEGINNING))
        {
            P1OUT |= RED_LED;
            stop_movement();
        }

        if (Last_Time_Sequence != Timer_counter)
        {
            Last_Time_Sequence = Timer_counter;
            cycle_time++;
            myTime++;
            time_change = TRUE;

            // Update LCD status only once per second.
            // Updating every 20ms made WiFi/USB commands feel laggy.
            if ((seconds / TICKS_PER_SECOND) != status_display_second)
            {
                status_display_second = seconds / TICKS_PER_SECOND;
                Update_Status_Display();
            }

            // increment exit_timer on every 20ms tick during exit sequence
            if (current_direction == CMD_EXIT)
            {
                exit_timer++;
            }

            if (move_active == TRUE && current_direction != CMD_DETECT && current_direction != CMD_EXIT && current_direction != CMD_ARC && current_direction != CMD_ARC_V && current_direction != CMD_PAD)
            {
                if (move_time_remaining > BEGINNING)
                {
                    move_time_remaining--;
                }
                else
                {
                    stop_movement();
                    move_active       = FALSE;
                    current_direction = NULL_CHAR;

                    if (pending_direction != NULL_CHAR)
                    {
                        switch (pending_direction)
                        {
                            case CMD_FORWARD:   strcpy(display_line[0], " Forward  "); Execute_Forward(pending_time);  break;
                            case CMD_BACKWARD:  strcpy(display_line[0], " Backward "); Execute_Reverse(pending_time);  break;
                            case CMD_RIGHT:     strcpy(display_line[0], "  Right   "); Execute_Right(pending_time);    break;
                            case CMD_LEFT:      strcpy(display_line[0], "  Left    "); Execute_Left(pending_time);     break;
                            case CMD_ARC:       strcpy(display_line[1], "   Arc U  "); Execute_Arc(pending_time);      break;
                            case CMD_ARC_V:     strcpy(display_line[1], "   Arc V  "); Execute_Arc_V(pending_time);    break;
                            case CMD_PAD:       strcpy(display_line[0], " To Pad 8 "); Execute_Pad(pending_time);      break;
                            default: break;
                        }
                        display_changed   = TRUE;
                        update_display    = TRUE;
                        pending_direction = NULL_CHAR;
                        pending_time      = BEGINNING;
                    }
                }
            }

            // return to waiting only if no movement active
            if (return_to_waiting_timer > BEGINNING)
            {
                return_to_waiting_timer--;
                if (return_to_waiting_timer == BEGINNING && move_active == FALSE)
                {
                    strcpy(display_line[0], " Waiting  ");
                    strcpy(display_line[2], "for input ");
                    display_changed = TRUE;
                    update_display  = TRUE;
                }
            }
        }

        if (keep_display_off == TRUE)
        {
            P6OUT &= ~LCD_BACKLITE;
        }
        else if (flash_backlight == TRUE)
        {
            flash_backlight = FALSE;
            P6OUT ^= LCD_BACKLITE;
        }
switch (event)
        {
        case OKAY:
            if (current_direction != CMD_EXIT && current_direction != CMD_DETECT)
            {
                pad_8();
            }
            break;
        default:
            break;
        }
    }
}

void Update_Status_Display(void)
{
    unsigned int s;

    if (final_display_lock == TRUE)
    {
        return;
    }

    strcpy(display_line[1], "Kaushik A ");

    s = seconds / TICKS_PER_SECOND;

    display_line[3][0]  = last_cmd_display[0];
    display_line[3][1]  = last_cmd_display[1];
    display_line[3][2]  = last_cmd_display[2];
    display_line[3][3]  = last_cmd_display[3];
    display_line[3][4]  = last_cmd_display[4];
    display_line[3][5]  = ' ';
    display_line[3][6]  = ASCII_ZERO + (char)((s / 100) % DECIMAL_SHIFT);
    display_line[3][7]  = ASCII_ZERO + (char)((s / 10)  % DECIMAL_SHIFT);
    display_line[3][8]  = ASCII_ZERO + (char)(s         % DECIMAL_SHIFT);
    display_line[3][9]  = 's';
    display_line[3][10] = NULL_CHAR;

    display_changed = TRUE;
    update_display  = TRUE;
}

void Display_IP_Address(void)
{
    char         *staip_ptr;
    char         *end_ptr;
    char         *dot_ptr;
    unsigned int  i;
    unsigned int  dot_count;

    staip_ptr = strstr((char*)current_command, "STAIP");
    if (staip_ptr == NULL)
    {
        strcpy(display_line[0], " Waiting  ");
        strcpy(display_line[1], "Kaushik A ");
        strcpy(display_line[2], "  No  IP  ");
        strcpy(display_line[3], "for input ");
        strcpy(ip_line1, "  No  IP  ");
        strcpy(ip_line2, "          ");
        display_changed = TRUE;
        update_display  = TRUE;
        return;
    }

    staip_ptr += 7;

    end_ptr = strchr(staip_ptr, '"');
    if (end_ptr != NULL)
    {
        *end_ptr = NULL_CHAR;
    }

    dot_count = BEGINNING;
    dot_ptr   = staip_ptr;
    while (*dot_ptr != NULL_CHAR)
    {
        if (*dot_ptr == '.')
        {
            dot_count++;
            if (dot_count == 2)
            {
                dot_ptr++;
                break;
            }
        }
        dot_ptr++;
    }

    i = BEGINNING;
    while ((staip_ptr + i) < dot_ptr && i < DISPLAY_LINE_LEN)
    {
        ip_line1[i] = staip_ptr[i];
        i++;
    }
    while (i < DISPLAY_LINE_LEN)
    {
        ip_line1[i] = ' ';
        i++;
    }
    ip_line1[DISPLAY_LINE_LEN] = NULL_CHAR;

    i = BEGINNING;
    while (*dot_ptr != NULL_CHAR && i < DISPLAY_LINE_LEN)
    {
        ip_line2[i] = *dot_ptr;
        i++;
        dot_ptr++;
    }
    while (i < DISPLAY_LINE_LEN)
    {
        ip_line2[i] = ' ';
        i++;
    }
    ip_line2[DISPLAY_LINE_LEN] = NULL_CHAR;

    strncpy(display_line[0], ip_line1, DISPLAY_LINE_LEN);
    display_line[0][DISPLAY_LINE_LEN] = NULL_CHAR;
    strcpy(display_line[1], "Kaushik A ");
    strncpy(display_line[2], ip_line2, DISPLAY_LINE_LEN);
    display_line[2][DISPLAY_LINE_LEN] = NULL_CHAR;
    strcpy(display_line[3], "for input ");

    display_changed = TRUE;
    update_display  = TRUE;
    Display_Update(RESET_STATE, RESET_STATE, RESET_STATE, RESET_STATE);
}

void Process_FRAM_Command(void)
{
    char cmd_char;

    if (USB_Ring_Rx[BEGINNING] != CMD_START_CHAR)
    {
        return;
    }

    cmd_char = USB_Ring_Rx[ONE_BYTE];

    switch (cmd_char)
    {
        case CMD_START_CHAR:
            PC_Transmit_String("I'm here\r\n");
            break;

        case 'F':
            Init_Serial_UCA0(BAUD_115200);
            PC_Transmit_String("115,200\r\n");
            strcpy(display_line[2], " 115,200  ");
            display_changed = TRUE;
            update_display  = TRUE;
            break;

        case 'S':
            Init_Serial_UCA0(BAUD_460800);
            PC_Transmit_String("460,800\r\n");
            strcpy(display_line[2], " 460,800  ");
            display_changed = TRUE;
            update_display  = TRUE;
            break;

        default:
            PC_Transmit_String("Unknown CMD\r\n");
            break;
    }
}

void Process_IOT_WiFi_Command(void)
{
    char *colon_ptr;

    colon_ptr = strchr((char*)current_command, ':');
    if (colon_ptr == NULL)
    {
        return;
    }

    char *payload = colon_ptr + ONE_BYTE;

    if (payload[BEGINNING] == CMD_START_CHAR)
    {
        Parse_Drive_Command(payload);
    }
}
