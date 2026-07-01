/*
 * Kaushik A
 * ECE 306 Project 10
 */
// call a helper function
void Display_Process(void);

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

// variable comes from another file
extern char display_line[4][11];
// variable comes from another file
extern char adc_char[];
// variable comes from another file
extern int ADC_Thumb;
// variable comes from another file
extern int ADC_Left_Detect;
// variable comes from another file
extern int ADC_Right_Detect;
// variable comes from another file
extern char current_direction;

// variable or helper used by the car
char ADC_Channel;
//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor //
//-----------------------------------------------------------------
// function used by the car
void HEXtoBCD(int hex_value)
// block marker
{
    // variable or helper used by the car
    int i;
    // variable or helper used by the car
    int value = 0;
    // loop through the values needed here
    for (i = 0; i < 4; i++)
    // block marker
    {
        // update a value used by the program
        adc_char[i] = '0';
    // block marker
    }
    // loop until the condition changes
    while (hex_value > 999)
    // block marker
    {
        // update a value used by the program
        hex_value = hex_value - 1000;
        // update a value used by the program
        value = value + 1;
        // update a value used by the program
        adc_char[0] = 0x30 + value;
    // block marker
    }
    // update a value used by the program
    value = 0;
    // loop until the condition changes
    while (hex_value > 99)
    // block marker
    {
        // update a value used by the program
        hex_value = hex_value - 100;
        // update a value used by the program
        value = value + 1;
        // update a value used by the program
        adc_char[1] = 0x30 + value;
    // block marker
    }
    // update a value used by the program
    value = 0;
    // loop until the condition changes
    while (hex_value > 9)
    // block marker
    {
        // update a value used by the program
        hex_value = hex_value - 10;
        // update a value used by the program
        value = value + 1;
        // update a value used by the program
        adc_char[2] = 0x30 + value;
    // block marker
    }
    // update a value used by the program
    adc_char[3] = 0x30 + hex_value;
// block marker
}

//-----------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//-----------------------------------------------------------------
// function used by the car
void adc_line(char line, char location)
// block marker
{
    // variable or helper used by the car
    int i;
    // variable or helper used by the car
    unsigned int real_line;
    // update a value used by the program
    real_line = line - 1;
    // loop through the values needed here
    for (i = 0; i < 4; i++)
    // block marker
    {
        // update a value used by the program
        display_line[real_line][i + location] = adc_char[i];
    // block marker
    }
// block marker
}

// ADC INIT

// function used by the car
void Init_ADC(void)
// block marker
{
//------------------------------------------------------------------------------
// V_DETECT_L (0x04) // Pin 2 A2
// V_DETECT_R (0x08) // Pin 3 A3
// V_THUMB (0x20) // Pin 5 A5
//------------------------------------------------------------------------------
// ADCCTL0 Register
    // update a value used by the program
    ADCCTL0 = 0; // Reset
    // update a value used by the program
    ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
    // update a value used by the program
    ADCCTL0 |= ADCMSC; // MSC
    // update a value used by the program
    ADCCTL0 |= ADCON; // ADC ON
// ADCCTL1 Register
    // update a value used by the program
    ADCCTL1 = 0; // Reset
    // update a value used by the program
    ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
    // update a value used by the program
    ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
    // update a value used by the program
    ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
    // update a value used by the program
    ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
    // update a value used by the program
    ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
    // update a value used by the program
    ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
// ADCCTL1 & ADCBUSY identifies a conversion is in process
// ADCCTL2 Register
    // update a value used by the program
    ADCCTL2 = 0; // Reset
    // update a value used by the program
    ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
    // update a value used by the program
    ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
    // update a value used by the program
    ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
    // update a value used by the program
    ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
// ADCMCTL0 Register
    // update a value used by the program
    ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
    // update a value used by the program
    ADCMCTL0 |= ADCINCH_2; // V_THUMB (0x20) Pin 5 A5
    // update a value used by the program
    ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
    // update a value used by the program
    ADCCTL0 |= ADCENC; // ADC enable conversion.
    // update a value used by the program
    ADCCTL0 |= ADCSC; // ADC start conversion.
// block marker
}

// ADC INTERUPTS

// update a value used by the program
#pragma vector=ADC_VECTOR
// code line for the car logic
__interrupt void ADC_ISR(void)
// block marker
{
    // variable or helper used by the car
    unsigned char start_next_conversion = FALSE;

    // choose what to do based on the state
    switch (__even_in_range(ADCIV, ADCIV_ADCIFG))
    // block marker
    {
    // state or command case
    case ADCIV_NONE:
        // code line for the car logic
        break;
    // state or command case
    case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
        // code line for the car logic
        break;
    // state or command case
    case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
        // code line for the car logic
        break;
    // state or command case
    case ADCIV_ADCHIIFG: // Window comparator interrupt flags
        // code line for the car logic
        break;
    // state or command case
    case ADCIV_ADCLOIFG: // Window comparator interrupt flag
        // code line for the car logic
        break;
    // state or command case
    case ADCIV_ADCINIFG: // Window comparator interrupt flag
        // code line for the car logic
        break;
    // state or command case
    case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
        // update a value used by the program
        ADCCTL0 &= ~ADCENC; // Disable ENC bit.
        // choose what to do based on the state
        switch (ADC_Channel++)
        // block marker
        {
        // state or command case
        case 0: // Channel A2 Interrupt
            // update a value used by the program
            ADC_Left_Detect = ADCMEM0; // Move result into Global
            // update a value used by the program
            ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
            // update a value used by the program
            ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
            // update a value used by the program
            ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
            // update a value used by the program
            start_next_conversion = TRUE;
            // code line for the car logic
            break;
        // state or command case
        case 1:
            // update a value used by the program
            ADC_Right_Detect = ADCMEM0; // Move result into Global
            // update a value used by the program
            ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
            // update a value used by the program
            ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A3
            // update a value used by the program
            ADCMCTL0 |= ADCINCH_5; // Enable Next channel A5
            // update a value used by the program
            start_next_conversion = TRUE;

            // Replicate for each channel being read
            // code line for the car logic
            break;
        // state or command case
        case 2:
            // update a value used by the program
            ADC_Channel = 0;
            // update a value used by the program
            ADC_Thumb = ADCMEM0; // Move result into Global
            // update a value used by the program
            ADC_Thumb = ADC_Thumb >> 2; // Divide the result by 4
            // update a value used by the program
            ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A5
            // update a value used by the program
            ADCMCTL0 |= ADCINCH_2; // Enable Next channel A2
            // check if this condition is true
            if (current_direction == CMD_DETECT)
            // block marker
            {
                // update a value used by the program
                start_next_conversion = TRUE;
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
        // update a value used by the program
        ADCCTL0 |= ADCENC; // Enable Conversions
        // check if this condition is true
        if (start_next_conversion == TRUE)
        // block marker
        {
            // update a value used by the program
            ADCCTL0 |= ADCSC;
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





