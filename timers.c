/*
 * Kaushik A
 * ECE 306 Project 10
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "adc.h"
#include "dac.h"
#include "timers.h"

void Display_Process(void);

volatile unsigned int Timer_counter;
volatile unsigned int Time_Sequence;
extern volatile unsigned char update_display;
unsigned int debounce_count;
extern unsigned char flash_backlight;
extern volatile unsigned char keep_display_off;
unsigned int display_counting;
extern volatile int DAC_data;
extern volatile int ir_count;
extern volatile char state;
volatile unsigned int seconds;

void Init_Timers(void)
{
    Init_Timer_B0();
    Init_Timer_B3();
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
    Timer_counter++;
    seconds++;
    //flash_backlight = TRUE;
    display_counting++;
    if (display_counting >= 10)
    {
        update_display = TRUE;
        display_counting = 0;
    }

    ADCCTL0 |= ADCSC;
//----------------------------------------------------------------------------
}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    switch (__even_in_range(TB0IV, 14))
    {
    case 0:
        break; // No interrupt
    case 2: // CCR1 not used
        TB0CCTL1 &= ~CCIE;
        TB0CCR1 += TB0CCR1_INTERVAL;
        P4IFG &= ~SW1;   //CLEARS INTPERUT FLAG/
        P4IE |= SW1;     // enables
        keep_display_off = FALSE;

        break;
    case 4: // CCR2 not used
        TB0CCTL2 &= ~CCIE;
        TB0CCR2 += TB0CCR2_INTERVAL;
        P2IFG &= ~SW2;   //CLEARS INTPERUT FLAG
        P2IE |= SW2;     //enables

        keep_display_off = FALSE;
        break;
    case 14:   // overflow

        DAC_data = DAC_data - 100;
        SAC3DAT = DAC_data;           // Initial DAC data

        if (DAC_data <= DAC_Limit)
        {

            DAC_data = DAC_Adjust;
            SAC3DAT = DAC_data;        // Reset DAC data

            TB0CTL &= ~TBIE;            // Disable TimerB0 overflow interrupt
            P1DIR &= ~RED_LED;         // Turn RED LED off

        }

        break;

    default:
        break;
    }
}
//----------------------------------------------------------------------------
    void Init_Timer_B3(void) {
    //------------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    //TB3.1 P6.0 LCD_BACKLITE
    //TB3.2 P6.1 R_FORWARD
    //TB3.3 P6.2 R_REVERSE
    //TB3.4 P6.3 L_FORWARD
    //TB3.5 P6.4 L_REVERSE
    //------------------------------------------------------------------------------
     TB3CTL = TBSSEL__SMCLK; // SMCLK
     TB3CTL |= MC__UP; // Up Mode
     TB3CTL |= TBCLR; // Clear TAR
     PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]
     TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
     LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle
     TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
     RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
     TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
     LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
     TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
     RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
     TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
     LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
    //------------------------------------------------------------------------------
    }

void Init_Timer_B0(void)
{
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__8; // Divide clock by 8
    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8

    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt

    TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    TB0CCTL1 |= CCIE; // CCR1 enable interrupt

    TB0CCR2 = TB0CCR2_INTERVAL; // CCR2

    TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

