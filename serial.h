/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#ifndef SERIAL_H_
// preprocessor setup for the project
#define SERIAL_H_

// include file needed by this file
#include <msp430.h>
// include file needed by this file
#include "macros.h"

// Extern declarations - defined in serial.c
// variable comes from another file
extern volatile char IOT_Ring_Rx[RING_BUF_SIZE];
// variable comes from another file
extern volatile char USB_Ring_Rx[RING_BUF_SIZE];
// variable comes from another file
extern volatile char iot_TX_buf[RING_BUF_SIZE];
// variable comes from another file
extern volatile unsigned int iot_rx_wr;
// variable comes from another file
extern volatile unsigned int usb_rx_wr;
// variable comes from another file
extern volatile unsigned int iot_tx;
// variable comes from another file
extern volatile unsigned int direct_iot;
// variable comes from another file
extern volatile char speed;

// variable comes from another file
extern volatile char iot_receive_complete;
// variable comes from another file
extern volatile char current_command[MAX_COMMAND_LENGTH];
// variable comes from another file
extern unsigned int  return_to_waiting_timer;

// variable comes from another file
extern volatile unsigned char pc_tx_enabled;
// variable comes from another file
extern volatile unsigned char fram_cmd_active;

// Function prototypes
// call a helper function
void Init_Serial_UCA0(char speed);
// call a helper function
void Init_Serial_UCA1(char speed);
// call a helper function
void IOT_Transmit_String(const char *str);
// call a helper function
void PC_Transmit_String(const char *str);

// preprocessor setup for the project
#endif
