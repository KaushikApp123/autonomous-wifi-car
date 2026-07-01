/*
 * Kaushik A
 * ECE 306 Project 10
 */
// include file needed by this file
#include "macros.h"
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

// variable comes from another file
extern volatile unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
// variable comes from another file
extern volatile unsigned int cycle_time; // is a new time base used to control making shapes
// variable comes from another file
extern volatile unsigned int time_change; // is an identifier that a change has occurred
// variable comes from another file
extern volatile unsigned int myTime;

// function used by the car
void Shapes(void)
// block marker
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
// block marker
}
// function used by the car
void Run_Circle(void)
// block marker
{
    // check if this condition is true
    if (myTime > 3000)
    // block marker
    {
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
        // update a value used by the program
        P6OUT &= ~R_FORWARD;
        // send the result back
        return;
    // block marker
    }

    // check if this condition is true
    if (cycle_time < 20) {
        // update a value used by the program
        P6OUT |= L_FORWARD;
        // update a value used by the program
        P6OUT |= R_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 55)
    // block marker
    {
        // update a value used by the program
        P6OUT |= L_FORWARD;
        // update a value used by the program
        P6OUT &= ~R_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // update a value used by the program
        cycle_time = 0;
    // block marker
    }
// block marker
}


// function used by the car
void Run_Eight(void)
// block marker
{
// Stop condition - check first and return immediately
    // check if this condition is true
    if (myTime >= 5800)
    // block marker
    {
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
        // update a value used by the program
        P6OUT &= ~R_FORWARD;
        // send the result back
        return;  // <-- EXIT so nothing below runs
    // block marker
    }

// First half of eight - turn left
    // check if this condition is true
    if (myTime < 2800)
    // block marker
    {
        // check if this condition is true
        if (cycle_time < 20)
        // block marker
        {
            // update a value used by the program
            P6OUT |= L_FORWARD;
            // update a value used by the program
            P6OUT |= R_FORWARD;
        // block marker
        }
        // otherwise handle the other case
        else if (cycle_time < 55)
        // block marker
        {
            // update a value used by the program
            P6OUT |= L_FORWARD;
            // update a value used by the program
            P6OUT &= ~R_FORWARD;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // update a value used by the program
            cycle_time = 0;
        // block marker
        }
    // block marker
    }
// Second half of eight - turn right
    // otherwise handle the other case
    else
    // block marker
    {
        // check if this condition is true
        if (cycle_time < 20)
        // block marker
        {
            // update a value used by the program
            P6OUT |= R_FORWARD;
            // update a value used by the program
            P6OUT |= L_FORWARD;
        // block marker
        }
        // otherwise handle the other case
        else if (cycle_time < 55)
        // block marker
        {
            // update a value used by the program
            P6OUT |= R_FORWARD;
            // update a value used by the program
            P6OUT &= ~L_FORWARD;
        // block marker
        }
        // otherwise handle the other case
        else
        // block marker
        {
            // update a value used by the program
            cycle_time = 0;
        // block marker
        }
    // block marker
    }
// block marker
}

// function used by the car
void Run_Triangle(void)
// block marker
{

    // check if this condition is true
    if (cycle_time < 120)
    // block marker
    {
        // Straight
        // update a value used by the program
        P6OUT |= R_FORWARD;
        // update a value used by the program
        P6OUT |= L_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 210)   // 90 units turn right
    // block marker
    {
        // update a value used by the program
        P6OUT |= R_FORWARD;
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 330)
    // block marker
    {
        // Straight
        // update a value used by the program
        P6OUT |= R_FORWARD;
        // update a value used by the program
        P6OUT |= L_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 420)   // 90 units turn right
    // block marker
    {
        // update a value used by the program
        P6OUT |= R_FORWARD;
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 540)
    // block marker
    {
        // Straight
        // update a value used by the program
        P6OUT |= R_FORWARD;
        // update a value used by the program
        P6OUT |= L_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 630)   // 90 units turn right
    // block marker
    {
        // update a value used by the program
        P6OUT |= R_FORWARD;
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // update a value used by the program
        cycle_time = 0;
    // block marker
    }
// block marker
}

// function used by the car
void Run_Circle_CCW(void)
// block marker
{
    // check if this condition is true
    if (myTime > 3000)
    // block marker
    {
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
        // update a value used by the program
        P6OUT &= ~R_FORWARD;
        // send the result back
        return;
    // block marker
    }

    // check if this condition is true
    if (cycle_time < 20) {
        // update a value used by the program
        P6OUT |= L_FORWARD;
        // update a value used by the program
        P6OUT |= R_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else if (cycle_time < 55)
    // block marker
    {
        // update a value used by the program
        P6OUT &= ~L_FORWARD;
        // update a value used by the program
        P6OUT |= R_FORWARD;
    // block marker
    }
    // otherwise handle the other case
    else
    // block marker
    {
        // update a value used by the program
        cycle_time = 0;
    // block marker
    }
// block marker
}


