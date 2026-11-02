/*
 * Kaushik A
 * ECE 306 Project 10
 */
#ifndef SERIAL_H_
#define SERIAL_H_

#include <msp430.h>
#include "macros.h"

// Extern declarations - defined in serial.c
extern volatile char IOT_Ring_Rx[RING_BUF_SIZE];
extern volatile char USB_Ring_Rx[RING_BUF_SIZE];
extern volatile char iot_TX_buf[RING_BUF_SIZE];
extern volatile unsigned int iot_rx_wr;
extern volatile unsigned int usb_rx_wr;
extern volatile unsigned int iot_tx;
extern volatile unsigned int direct_iot;
extern volatile char speed;

extern volatile char iot_receive_complete;
extern volatile char current_command[MAX_COMMAND_LENGTH];
extern unsigned int  return_to_waiting_timer;

extern volatile unsigned char pc_tx_enabled;
extern volatile unsigned char fram_cmd_active;

// Function prototypes
void Init_Serial_UCA0(char speed);
void Init_Serial_UCA1(char speed);
void IOT_Transmit_String(const char *str);
void PC_Transmit_String(const char *str);

#endif
