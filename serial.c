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

extern volatile unsigned char switch1;
extern volatile unsigned char switch2;

volatile char speed = BAUD_115200;
volatile char IOT_Ring_Rx[RING_BUF_SIZE];
volatile char USB_Ring_Rx[RING_BUF_SIZE];
volatile char iot_TX_buf[RING_BUF_SIZE];
volatile unsigned int iot_rx_wr  = BEGINNING;
volatile unsigned int usb_rx_wr  = BEGINNING;
volatile unsigned int iot_tx     = BEGINNING;
volatile unsigned int direct_iot = BEGINNING;
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
volatile unsigned int display_index = BEGINNING;

extern volatile char iot_receive_complete;
extern volatile char current_command[MAX_COMMAND_LENGTH];

volatile unsigned char pc_tx_enabled   = FALSE;
volatile unsigned char fram_cmd_active = FALSE;

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void) {
    char iot_receive;
    switch (__even_in_range(UCA0IV, 0x08)) {
    case 0: break;

    case 2:
        iot_receive = UCA0RXBUF;

        if (iot_receive == CARRIAGE_RETURN)
        {
            current_command[iot_rx_wr] = NULL_CHAR;
            iot_receive_complete        = TRUE;
            iot_rx_wr                   = BEGINNING;
        }
        else if (iot_receive == LINE_FEED)
        {
            // discard
        }
        else
        {
            if (iot_rx_wr < MAX_COMMAND_LENGTH)
            {
                current_command[iot_rx_wr++] = iot_receive;
            }
            if (iot_rx_wr >= MAX_COMMAND_LENGTH)
            {
                current_command[MAX_COMMAND_LENGTH - ONE_BYTE] = NULL_CHAR;
                iot_receive_complete                            = TRUE;
                iot_rx_wr                                       = BEGINNING;
            }
        }

        // Always forward to PC - no gate
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = iot_receive;
        break;

    case 4:
        if (iot_TX_buf[iot_tx] == NULL_CHAR || iot_tx >= (RING_BUF_SIZE - ONE_BYTE))
        {
            UCA0IE &= ~UCTXIE;
            iot_tx  = BEGINNING;
        }
        else
        {
            UCA0TXBUF = iot_TX_buf[iot_tx++];
        }
        break;

    default: break;
    }
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
    char usb_value;
    switch (__even_in_range(UCA1IV, 0x08)) {
    case 0: break;

    case 2:
        usb_value = UCA1RXBUF;

        USB_Ring_Rx[usb_rx_wr++] = usb_value;
        if (usb_rx_wr >= RING_BUF_SIZE)
        {
            usb_rx_wr = BEGINNING;
        }

        if (usb_value == CMD_START_CHAR)
        {
            fram_cmd_active = TRUE;
        }

        if (fram_cmd_active == FALSE)
        {
            while (!(UCA0IFG & UCTXIFG));
            UCA0TXBUF = usb_value;
        }

        if (usb_value == CARRIAGE_RETURN)
        {
            fram_cmd_active = FALSE;
        }
        break;

    case 4:
        UCA1TXBUF = IOT_Ring_Rx[direct_iot++];
        if (direct_iot >= RING_BUF_SIZE)
        {
            direct_iot = BEGINNING;
        }
        if (iot_rx_wr == direct_iot)
        {
            UCA1IE &= ~UCTXIE;
        }
        break;

    default: break;
    }
}

void Init_Serial_UCA0(char baud)
{
    UCA0CTLW0  = RESET_STATE;
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0CTLW0 &= ~UCMSB;
    UCA0CTLW0 &= ~UCSPB;
    UCA0CTLW0 &= ~UCPEN;
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;

    switch (baud)
    {
        case BAUD_115200:
            UCA0BRW   = UCA_115200_BRW;
            UCA0MCTLW = UCA_115200_MCTLW;
            break;
        case BAUD_460800:
            UCA0BRW   = UCA_460800_BRW;
            UCA0MCTLW = UCA_460800_MCTLW;
            break;
        default:
            UCA0BRW   = UCA_115200_BRW;
            UCA0MCTLW = UCA_115200_MCTLW;
            break;
    }

    UCA0CTLW0 &= ~UCSWRST;
    UCA0TXBUF  = RESET_STATE;
    UCA0IE    |= UCRXIE;
}

void Init_Serial_UCA1(char baud)
{
    UCA1CTLW0  = RESET_STATE;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 &= ~UCMSB;
    UCA1CTLW0 &= ~UCSPB;
    UCA1CTLW0 &= ~UCPEN;
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    switch (baud)
    {
        case BAUD_115200:
            UCA1BRW   = UCA_115200_BRW;
            UCA1MCTLW = UCA_115200_MCTLW;
            break;
        case BAUD_460800:
            UCA1BRW   = UCA_460800_BRW;
            UCA1MCTLW = UCA_460800_MCTLW;
            break;
        default:
            UCA1BRW   = UCA_115200_BRW;
            UCA1MCTLW = UCA_115200_MCTLW;
            break;
    }

    UCA1CTLW0 &= ~UCSWRST;
    UCA1TXBUF  = RESET_STATE;

    pc_tx_enabled   = FALSE;
    fram_cmd_active = FALSE;

    UCA1IE |= UCRXIE;
}

void IOT_Transmit_String(const char *str)
{
    unsigned int i = BEGINNING;

    while (str[i] != NULL_CHAR)
    {
        while (!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = str[i++];
    }
    // Append CR+LF required by AT commands
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = CARRIAGE_RETURN;
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = LINE_FEED;
}

void PC_Transmit_String(const char *str)
{
    unsigned int i = BEGINNING;

    while (str[i] != NULL_CHAR)
    {
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = str[i++];
    }
}
