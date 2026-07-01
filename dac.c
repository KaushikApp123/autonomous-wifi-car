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
#include "macros.h"
// include file needed by this file
#include "adc.h"
// include file needed by this file
#include "dac.h"

// variable or helper used by the car
volatile int DAC_data;






// function used by the car
void Init_DAC(void)
// block marker
{
    // update a value used by the program
    SAC3DAC = DACSREF_0; // Select VCC as DAC reference
    // update a value used by the program
    SAC3DAC |= DACLSEL_0; // DAC latch loads when DACDAT written
    // update a value used by the program
    SAC3OA = NMUXEN; // SAC Negative input MUX control
    // update a value used by the program
    SAC3OA |= PMUXEN; // SAC Positive input MUX control
    // update a value used by the program
    SAC3OA |= PSEL_1; // 12-bit reference DAC source selected
    // update a value used by the program
    SAC3OA |= NSEL_1; // Select negative pin input
    // update a value used by the program
    SAC3OA |= OAPM; // Select low speed and low power mode
    // update a value used by the program
    SAC3PGA = MSEL_1; // Set OA as buffer mode
    // update a value used by the program
    SAC3OA |= SACEN; // Enable SAC
    // update a value used by the program
    SAC3OA |= OAEN; // Enable OA
    // update a value used by the program
    DAC_data = DAC_Begin; // Starting Low value for DAC output [2v]
    // update a value used by the program
    SAC3DAT = DAC_data; // Initial DAC data
    // update a value used by the program
    TB0CTL |= TBIE; // Timer B0 overflow interrupt enable
    //RED_LED_ON; // Set RED_LED On indicates the OVERFLOW timer start
    // update a value used by the program
    SAC3DAC |= DACEN; // Enable DAC
// block marker
}
