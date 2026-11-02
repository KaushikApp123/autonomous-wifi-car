/*
 * Kaushik A
 * ECE 306 Project 10
 */
#ifndef TIMERS_H
#define TIMERS_H

#define SLOW        (35000)
#define FAST        (50000)
#define PERCENT_100 (50000)
#define PERCENT_80  (45000)
#define PERCENT_70  (35000)
#define PERCENT_65  (32500)
#define PERCENT_60  (30000)
#define PERCENT_55  (27500)
#define PERCENT_50  (25000)
#define PERCENT_45  (22500)
#define PERCENT_40  (20000)
#define PERCENT_35  (17500)
#define PERCENT_30  (15000)
#define PERCENT_25  (12500)
#define PERCENT_22  (11000)
#define PERCENT_20  (10000)
#define PERCENT_15  (7500)
#define PERCENT_12  (6000)
#define PERCENT_10  (5000)
#define PERCENT_5   (2500)
#define PERCENT_0   (00000)

#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LEFT_REVERSE_SPEED (TB3CCR5)

//============================================================
// Globals
//============================================================

//============================================================
// Function Prototypes
//============================================================
void Init_Timer_B0(void);

#endif  // TIMERS_H
