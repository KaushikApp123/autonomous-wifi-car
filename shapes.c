/*
 * Kaushik A
 * ECE 306 Project 10
 */
#include "macros.h"
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
// STATES ======================================================================
//#define NONE ('N')
//#define STRAIGHT ('L')
//#define CIRCLE ('C')
//#define WAIT ('W')
//#define START ('S')
//#define RUN ('R')
//#define END ('E')
//#define WHEEL_COUNT_TIME (10)
//#define RIGHT_COUNT_TIME (7)
//#define LEFT_COUNT_TIME (8)
//#define TRAVEL_DISTANCE (2)
//#define WAITING2START (50)

extern volatile unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
extern volatile unsigned int cycle_time; // is a new time base used to control making shapes
extern volatile unsigned int time_change; // is an identifier that a change has occurred
extern volatile unsigned int myTime;

void Shapes(void)
{
//    void Run_Straight(void){
//     switch(state){
//     case WAIT: // Begin
//     wait_case(
//       void wait_case(void){
//       if(time_change){
//       time_change = 0;
//       if(delay_start++ >= WAITING2START){
//       delay_start = 0;
//       state = START;
//        }
//            }
//                }
//                    );
//     break; //
//     case START: // Begin
//     start_case();
//     break; //
//     case RUN: // Run
//     run_case()
//     break; //
//     case END: // End
//     end_case();
//     break; //
//     default: break;
//     }
//    }

// try 100 and 200 when making triangle

    //can use this as a starting point for figure 8 1400
}
void Run_Circle(void)
{
    if (myTime > 3000)
    {
        P6OUT &= ~L_FORWARD;
        P6OUT &= ~R_FORWARD;
        return;
    }

    if (cycle_time < 20) {
        P6OUT |= L_FORWARD;
        P6OUT |= R_FORWARD;
    }
    else if (cycle_time < 55)
    {
        P6OUT |= L_FORWARD;
        P6OUT &= ~R_FORWARD;
    }
    else
    {
        cycle_time = 0;
    }
}

void Run_Eight(void)
{
// Stop condition - check first and return immediately
    if (myTime >= 5800)
    {
        P6OUT &= ~L_FORWARD;
        P6OUT &= ~R_FORWARD;
        return;  // <-- EXIT so nothing below runs
    }

// First half of eight - turn left
    if (myTime < 2800)
    {
        if (cycle_time < 20)
        {
            P6OUT |= L_FORWARD;
            P6OUT |= R_FORWARD;
        }
        else if (cycle_time < 55)
        {
            P6OUT |= L_FORWARD;
            P6OUT &= ~R_FORWARD;
        }
        else
        {
            cycle_time = 0;
        }
    }
// Second half of eight - turn right
    else
    {
        if (cycle_time < 20)
        {
            P6OUT |= R_FORWARD;
            P6OUT |= L_FORWARD;
        }
        else if (cycle_time < 55)
        {
            P6OUT |= R_FORWARD;
            P6OUT &= ~L_FORWARD;
        }
        else
        {
            cycle_time = 0;
        }
    }
}

void Run_Triangle(void)
{

    if (cycle_time < 120)
    {
        // Straight
        P6OUT |= R_FORWARD;
        P6OUT |= L_FORWARD;
    }
    else if (cycle_time < 210)   // 90 units turn right
    {
        P6OUT |= R_FORWARD;
        P6OUT &= ~L_FORWARD;
    }
    else if (cycle_time < 330)
    {
        // Straight
        P6OUT |= R_FORWARD;
        P6OUT |= L_FORWARD;
    }
    else if (cycle_time < 420)   // 90 units turn right
    {
        P6OUT |= R_FORWARD;
        P6OUT &= ~L_FORWARD;
    }
    else if (cycle_time < 540)
    {
        // Straight
        P6OUT |= R_FORWARD;
        P6OUT |= L_FORWARD;
    }
    else if (cycle_time < 630)   // 90 units turn right
    {
        P6OUT |= R_FORWARD;
        P6OUT &= ~L_FORWARD;
    }
    else
    {
        cycle_time = 0;
    }
}

void Run_Circle_CCW(void)
{
    if (myTime > 3000)
    {
        P6OUT &= ~L_FORWARD;
        P6OUT &= ~R_FORWARD;
        return;
    }

    if (cycle_time < 20) {
        P6OUT |= L_FORWARD;
        P6OUT |= R_FORWARD;
    }
    else if (cycle_time < 55)
    {
        P6OUT &= ~L_FORWARD;
        P6OUT |= R_FORWARD;
    }
    else
    {
        cycle_time = 0;
    }
}

