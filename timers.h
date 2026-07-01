/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#ifndef TIMERS_H
// preprocessor setup for the project
#define TIMERS_H

// preprocessor setup for the project
#define SLOW        (35000)
// preprocessor setup for the project
#define FAST        (50000)
// preprocessor setup for the project
#define PERCENT_100 (50000)
// preprocessor setup for the project
#define PERCENT_80  (45000)
// preprocessor setup for the project
#define PERCENT_70  (35000)
// preprocessor setup for the project
#define PERCENT_65  (32500)
// preprocessor setup for the project
#define PERCENT_60  (30000)
// preprocessor setup for the project
#define PERCENT_55  (27500)
// preprocessor setup for the project
#define PERCENT_50  (25000)
// preprocessor setup for the project
#define PERCENT_45  (22500)
// preprocessor setup for the project
#define PERCENT_40  (20000)
// preprocessor setup for the project
#define PERCENT_35  (17500)
// preprocessor setup for the project
#define PERCENT_30  (15000)
// preprocessor setup for the project
#define PERCENT_25  (12500)
// preprocessor setup for the project
#define PERCENT_22  (11000)
// preprocessor setup for the project
#define PERCENT_20  (10000)
// preprocessor setup for the project
#define PERCENT_15  (7500)
// preprocessor setup for the project
#define PERCENT_12  (6000)
// preprocessor setup for the project
#define PERCENT_10  (5000)
// preprocessor setup for the project
#define PERCENT_5   (2500)
// preprocessor setup for the project
#define PERCENT_0   (00000)




// preprocessor setup for the project
#define PWM_PERIOD (TB3CCR0)
// preprocessor setup for the project
#define LCD_BACKLITE_DIMING (TB3CCR1)
// preprocessor setup for the project
#define RIGHT_FORWARD_SPEED (TB3CCR2)
// preprocessor setup for the project
#define LEFT_FORWARD_SPEED (TB3CCR3)
// preprocessor setup for the project
#define RIGHT_REVERSE_SPEED (TB3CCR4)
// preprocessor setup for the project
#define LEFT_REVERSE_SPEED (TB3CCR5)



//============================================================
// Globals
//============================================================


//============================================================
// Function Prototypes
//============================================================
// call a helper function
void Init_Timer_B0(void);

// preprocessor setup for the project
#endif  // TIMERS_H
