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
// include file needed by this file
#include "timers.h"

// call a helper function
void Display_Process(void);

// variable or helper used by the car
volatile unsigned int Timer_counter;
// variable or helper used by the car
volatile unsigned int Time_Sequence;
// variable comes from another file
extern volatile unsigned char update_display;
// variable or helper used by the car
unsigned int debounce_count;
// variable comes from another file
extern unsigned char flash_backlight;
// variable comes from another file
extern volatile unsigned char keep_display_off;
// variable or helper used by the car
unsigned int display_counting;
// variable comes from another file
extern volatile int DAC_data;
// variable comes from another file
extern volatile int ir_count;
// variable comes from another file
extern volatile char state;
// variable or helper used by the car
volatile unsigned int seconds;


// function used by the car
void Init_Timers(void)
// block marker
{
    // call a helper function
    Init_Timer_B0();
    // call a helper function
    Init_Timer_B3();
// block marker
}

// update a value used by the program
#pragma vector = TIMER0_B0_VECTOR
// code line for the car logic
__interrupt void Timer0_B0_ISR(void)
// block marker
{
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------
    // update a value used by the program
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
    // code line for the car logic
    Timer_counter++;
    // code line for the car logic
    seconds++;
    //flash_backlight = TRUE;
    // code line for the car logic
    display_counting++;
    // check if this condition is true
    if (display_counting >= 10)
    // block marker
    {
        // update a value used by the program
        update_display = TRUE;
        // update a value used by the program
        display_counting = 0;
    // block marker
    }



    // update a value used by the program
    ADCCTL0 |= ADCSC;
//----------------------------------------------------------------------------
// block marker
}
// update a value used by the program
#pragma vector=TIMER0_B1_VECTOR
// code line for the car logic
__interrupt void TIMER0_B1_ISR(void)
// block marker
{
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    // choose what to do based on the state
    switch (__even_in_range(TB0IV, 14))
    // block marker
    {
    // state or command case
    case 0:
        // code line for the car logic
        break; // No interrupt
    // state or command case
    case 2: // CCR1 not used
        // update a value used by the program
        TB0CCTL1 &= ~CCIE;
        // update a value used by the program
        TB0CCR1 += TB0CCR1_INTERVAL;
        // update a value used by the program
        P4IFG &= ~SW1;   //CLEARS INTPERUT FLAG/
        // update a value used by the program
        P4IE |= SW1;     // enables
        // update a value used by the program
        keep_display_off = FALSE;


        // code line for the car logic
        break;
    // state or command case
    case 4: // CCR2 not used
        // update a value used by the program
        TB0CCTL2 &= ~CCIE;
        // update a value used by the program
        TB0CCR2 += TB0CCR2_INTERVAL;
        // update a value used by the program
        P2IFG &= ~SW2;   //CLEARS INTPERUT FLAG
        // update a value used by the program
        P2IE |= SW2;     //enables

        // update a value used by the program
        keep_display_off = FALSE;
        // code line for the car logic
        break;
    // state or command case
    case 14:   // overflow

        // update a value used by the program
        DAC_data = DAC_data - 100;
        // update a value used by the program
        SAC3DAT = DAC_data;           // Initial DAC data

        // check if this condition is true
        if (DAC_data <= DAC_Limit)
        // block marker
        {

            // update a value used by the program
            DAC_data = DAC_Adjust;
            // update a value used by the program
            SAC3DAT = DAC_data;        // Reset DAC data

            // update a value used by the program
            TB0CTL &= ~TBIE;            // Disable TimerB0 overflow interrupt
            // update a value used by the program
            P1DIR &= ~RED_LED;         // Turn RED LED off

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
//----------------------------------------------------------------------------
    // function used by the car
    void Init_Timer_B3(void) {
    //------------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    //TB3.1 P6.0 LCD_BACKLITE
    //TB3.2 P6.1 R_FORWARD
    //TB3.3 P6.2 R_REVERSE
    //TB3.4 P6.3 L_FORWARD
    //TB3.5 P6.4 L_REVERSE
    //------------------------------------------------------------------------------
     // update a value used by the program
     TB3CTL = TBSSEL__SMCLK; // SMCLK
     // update a value used by the program
     TB3CTL |= MC__UP; // Up Mode
     // update a value used by the program
     TB3CTL |= TBCLR; // Clear TAR
     // update a value used by the program
     PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]
     // update a value used by the program
     TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
     // update a value used by the program
     LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle
     // update a value used by the program
     TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
     // update a value used by the program
     RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
     // update a value used by the program
     TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
     // update a value used by the program
     LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
     // update a value used by the program
     TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
     // update a value used by the program
     RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
     // update a value used by the program
     TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
     // update a value used by the program
     LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
    //------------------------------------------------------------------------------
    // block marker
    }





// function used by the car
void Init_Timer_B0(void)
// block marker
{
    // update a value used by the program
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    // update a value used by the program
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    // update a value used by the program
    TB0CTL |= MC__CONTINOUS; // Continuous up
    // update a value used by the program
    TB0CTL |= ID__8; // Divide clock by 8
    // update a value used by the program
    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8

    // update a value used by the program
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    // update a value used by the program
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt

    // update a value used by the program
    TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    // update a value used by the program
    TB0CCTL1 |= CCIE; // CCR1 enable interrupt

    // update a value used by the program
    TB0CCR2 = TB0CCR2_INTERVAL; // CCR2

    // update a value used by the program
    TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    // update a value used by the program
    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    // update a value used by the program
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
// block marker
}

