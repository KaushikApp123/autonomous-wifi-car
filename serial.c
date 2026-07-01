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

// variable comes from another file
extern volatile unsigned char switch1;
// variable comes from another file
extern volatile unsigned char switch2;

// variable or helper used by the car
volatile char speed = BAUD_115200;
// variable or helper used by the car
volatile char IOT_Ring_Rx[RING_BUF_SIZE];
// variable or helper used by the car
volatile char USB_Ring_Rx[RING_BUF_SIZE];
// variable or helper used by the car
volatile char iot_TX_buf[RING_BUF_SIZE];
// variable or helper used by the car
volatile unsigned int iot_rx_wr  = BEGINNING;
// variable or helper used by the car
volatile unsigned int usb_rx_wr  = BEGINNING;
// variable or helper used by the car
volatile unsigned int iot_tx     = BEGINNING;
// variable or helper used by the car
volatile unsigned int direct_iot = BEGINNING;
// variable or helper used by the car
volatile char slow_input_down;
// variable comes from another file
extern char display_line[4][11];
// variable comes from another file
extern char *display[4];
// variable or helper used by the car
unsigned char display_mode;
// variable comes from another file
extern volatile unsigned char display_changed;
// variable comes from another file
extern volatile unsigned char update_display;
// variable comes from another file
extern volatile unsigned int update_display_count;
// variable or helper used by the car
volatile unsigned int display_index = BEGINNING;

// variable comes from another file
extern volatile char iot_receive_complete;
// variable comes from another file
extern volatile char current_command[MAX_COMMAND_LENGTH];

// variable or helper used by the car
volatile unsigned char pc_tx_enabled   = FALSE;
// variable or helper used by the car
volatile unsigned char fram_cmd_active = FALSE;


// update a value used by the program
#pragma vector = EUSCI_A0_VECTOR
// code line for the car logic
__interrupt void eUSCI_A0_ISR(void) {
    // variable or helper used by the car
    char iot_receive;
    // choose what to do based on the state
    switch (__even_in_range(UCA0IV, 0x08)) {
    // state or command case
    case 0: break;

    // state or command case
    case 2:
        // update a value used by the program
        iot_receive = UCA0RXBUF;

        // check if this condition is true
        if (iot_receive == CARRIAGE_RETURN)
        // block marker
        {
            // update a value used by the program
            current_command[iot_rx_wr] = NULL_CHAR;
            // update a value used by the program
            iot_receive_complete        = TRUE;
            // update a value used by the program
            iot_rx_wr                   = BEGINNING;
        // block marker
        }
        // otherwise handle the other case
        else if (iot_receive == LINE_FEED)
        // block marker
        {
            // discard
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // check if this condition is true
            if (iot_rx_wr < MAX_COMMAND_LENGTH)
            // block marker
            {
                // update a value used by the program
                current_command[iot_rx_wr++] = iot_receive;
            // block marker
            }
            // check if this condition is true
            if (iot_rx_wr >= MAX_COMMAND_LENGTH)
            // block marker
            {
                // update a value used by the program
                current_command[MAX_COMMAND_LENGTH - ONE_BYTE] = NULL_CHAR;
                // update a value used by the program
                iot_receive_complete                            = TRUE;
                // update a value used by the program
                iot_rx_wr                                       = BEGINNING;
            // block marker
            }
        // block marker
        }

        // Always forward to PC - no gate
        // loop until the condition changes
        while (!(UCA1IFG & UCTXIFG));
        // update a value used by the program
        UCA1TXBUF = iot_receive;
        // code line for the car logic
        break;

    // state or command case
    case 4:
        // check if this condition is true
        if (iot_TX_buf[iot_tx] == NULL_CHAR || iot_tx >= (RING_BUF_SIZE - ONE_BYTE))
        // block marker
        {
            // update a value used by the program
            UCA0IE &= ~UCTXIE;
            // update a value used by the program
            iot_tx  = BEGINNING;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // update a value used by the program
            UCA0TXBUF = iot_TX_buf[iot_tx++];
        // block marker
        }
        // code line for the car logic
        break;

    // code line for the car logic
    default: break;
    // block marker
    }
// block marker
}


// update a value used by the program
#pragma vector = EUSCI_A1_VECTOR
// code line for the car logic
__interrupt void eUSCI_A1_ISR(void) {
    // variable or helper used by the car
    char usb_value;
    // choose what to do based on the state
    switch (__even_in_range(UCA1IV, 0x08)) {
    // state or command case
    case 0: break;

    // state or command case
    case 2:
        // update a value used by the program
        usb_value = UCA1RXBUF;

        // update a value used by the program
        USB_Ring_Rx[usb_rx_wr++] = usb_value;
        // check if this condition is true
        if (usb_rx_wr >= RING_BUF_SIZE)
        // block marker
        {
            // update a value used by the program
            usb_rx_wr = BEGINNING;
        // block marker
        }

        // check if this condition is true
        if (usb_value == CMD_START_CHAR)
        // block marker
        {
            // update a value used by the program
            fram_cmd_active = TRUE;
        // block marker
        }

        // check if this condition is true
        if (fram_cmd_active == FALSE)
        // block marker
        {
            // loop until the condition changes
            while (!(UCA0IFG & UCTXIFG));
            // update a value used by the program
            UCA0TXBUF = usb_value;
        // block marker
        }

        // check if this condition is true
        if (usb_value == CARRIAGE_RETURN)
        // block marker
        {
            // update a value used by the program
            fram_cmd_active = FALSE;
        // block marker
        }
        // code line for the car logic
        break;

    // state or command case
    case 4:
        // update a value used by the program
        UCA1TXBUF = IOT_Ring_Rx[direct_iot++];
        // check if this condition is true
        if (direct_iot >= RING_BUF_SIZE)
        // block marker
        {
            // update a value used by the program
            direct_iot = BEGINNING;
        // block marker
        }
        // check if this condition is true
        if (iot_rx_wr == direct_iot)
        // block marker
        {
            // update a value used by the program
            UCA1IE &= ~UCTXIE;
        // block marker
        }
        // code line for the car logic
        break;

    // code line for the car logic
    default: break;
    // block marker
    }
// block marker
}


// function used by the car
void Init_Serial_UCA0(char baud)
// block marker
{
    // update a value used by the program
    UCA0CTLW0  = RESET_STATE;
    // update a value used by the program
    UCA0CTLW0 |= UCSWRST;
    // update a value used by the program
    UCA0CTLW0 |= UCSSEL__SMCLK;
    // update a value used by the program
    UCA0CTLW0 &= ~UCMSB;
    // update a value used by the program
    UCA0CTLW0 &= ~UCSPB;
    // update a value used by the program
    UCA0CTLW0 &= ~UCPEN;
    // update a value used by the program
    UCA0CTLW0 &= ~UCSYNC;
    // update a value used by the program
    UCA0CTLW0 &= ~UC7BIT;
    // update a value used by the program
    UCA0CTLW0 |= UCMODE_0;

    // choose what to do based on the state
    switch (baud)
    // block marker
    {
        // state or command case
        case BAUD_115200:
            // update a value used by the program
            UCA0BRW   = UCA_115200_BRW;
            // update a value used by the program
            UCA0MCTLW = UCA_115200_MCTLW;
            // code line for the car logic
            break;
        // state or command case
        case BAUD_460800:
            // update a value used by the program
            UCA0BRW   = UCA_460800_BRW;
            // update a value used by the program
            UCA0MCTLW = UCA_460800_MCTLW;
            // code line for the car logic
            break;
        // code line for the car logic
        default:
            // update a value used by the program
            UCA0BRW   = UCA_115200_BRW;
            // update a value used by the program
            UCA0MCTLW = UCA_115200_MCTLW;
            // code line for the car logic
            break;
    // block marker
    }

    // update a value used by the program
    UCA0CTLW0 &= ~UCSWRST;
    // update a value used by the program
    UCA0TXBUF  = RESET_STATE;
    // update a value used by the program
    UCA0IE    |= UCRXIE;
// block marker
}


// function used by the car
void Init_Serial_UCA1(char baud)
// block marker
{
    // update a value used by the program
    UCA1CTLW0  = RESET_STATE;
    // update a value used by the program
    UCA1CTLW0 |= UCSWRST;
    // update a value used by the program
    UCA1CTLW0 |= UCSSEL__SMCLK;
    // update a value used by the program
    UCA1CTLW0 &= ~UCMSB;
    // update a value used by the program
    UCA1CTLW0 &= ~UCSPB;
    // update a value used by the program
    UCA1CTLW0 &= ~UCPEN;
    // update a value used by the program
    UCA1CTLW0 &= ~UCSYNC;
    // update a value used by the program
    UCA1CTLW0 &= ~UC7BIT;
    // update a value used by the program
    UCA1CTLW0 |= UCMODE_0;

    // choose what to do based on the state
    switch (baud)
    // block marker
    {
        // state or command case
        case BAUD_115200:
            // update a value used by the program
            UCA1BRW   = UCA_115200_BRW;
            // update a value used by the program
            UCA1MCTLW = UCA_115200_MCTLW;
            // code line for the car logic
            break;
        // state or command case
        case BAUD_460800:
            // update a value used by the program
            UCA1BRW   = UCA_460800_BRW;
            // update a value used by the program
            UCA1MCTLW = UCA_460800_MCTLW;
            // code line for the car logic
            break;
        // code line for the car logic
        default:
            // update a value used by the program
            UCA1BRW   = UCA_115200_BRW;
            // update a value used by the program
            UCA1MCTLW = UCA_115200_MCTLW;
            // code line for the car logic
            break;
    // block marker
    }

    // update a value used by the program
    UCA1CTLW0 &= ~UCSWRST;
    // update a value used by the program
    UCA1TXBUF  = RESET_STATE;

    // update a value used by the program
    pc_tx_enabled   = FALSE;
    // update a value used by the program
    fram_cmd_active = FALSE;

    // update a value used by the program
    UCA1IE |= UCRXIE;
// block marker
}


// function used by the car
void IOT_Transmit_String(const char *str)
// block marker
{
    // variable or helper used by the car
    unsigned int i = BEGINNING;

    // loop until the condition changes
    while (str[i] != NULL_CHAR)
    // block marker
    {
        // loop until the condition changes
        while (!(UCA0IFG & UCTXIFG));
        // update a value used by the program
        UCA0TXBUF = str[i++];
    // block marker
    }
    // Append CR+LF required by AT commands
    // loop until the condition changes
    while (!(UCA0IFG & UCTXIFG));
    // update a value used by the program
    UCA0TXBUF = CARRIAGE_RETURN;
    // loop until the condition changes
    while (!(UCA0IFG & UCTXIFG));
    // update a value used by the program
    UCA0TXBUF = LINE_FEED;
// block marker
}


// function used by the car
void PC_Transmit_String(const char *str)
// block marker
{
    // variable or helper used by the car
    unsigned int i = BEGINNING;

    // loop until the condition changes
    while (str[i] != NULL_CHAR)
    // block marker
    {
        // loop until the condition changes
        while (!(UCA1IFG & UCTXIFG));
        // update a value used by the program
        UCA1TXBUF = str[i++];
    // block marker
    }
// block marker
}
