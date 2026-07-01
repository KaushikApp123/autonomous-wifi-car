/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#ifndef MACROS_H
// preprocessor setup for the project
#define MACROS_H

// General
// preprocessor setup for the project
#define ALWAYS                  (1)
// preprocessor setup for the project
#define RESET_STATE             (0)
// preprocessor setup for the project
#define BEGINNING               (0)
// preprocessor setup for the project
#define TRUE                    (0x01)
// preprocessor setup for the project
#define FALSE                   (0x00)
// preprocessor setup for the project
#define OKAY                    (1)
// preprocessor setup for the project
#define NOT_OKAY                (0)
// preprocessor setup for the project
#define ONE_BYTE                (1)
// preprocessor setup for the project
#define TWO_BYTES               (2)

// LEDs and probes
// preprocessor setup for the project
#define RED_LED                 (0x01)
// preprocessor setup for the project
#define GRN_LED                 (0x40)
// preprocessor setup for the project
#define TEST_PROBE              (0x01)

// Button states
// preprocessor setup for the project
#define RELEASED                (1)
// preprocessor setup for the project
#define PRESSED                 (0)
// preprocessor setup for the project
#define DEBOUNCE_RESTART        (0)
// preprocessor setup for the project
#define DEBOUNCE_TIME           (10000)

// Timer intervals
// preprocessor setup for the project
#define TB0CCR0_INTERVAL        (2500)
// preprocessor setup for the project
#define TB0CCR1_INTERVAL        (6250)
// preprocessor setup for the project
#define TB0CCR2_INTERVAL        (6250)
// preprocessor setup for the project
#define TICKS_PER_SECOND        (50)

// Timer vectors
// preprocessor setup for the project
#define TIMER_B0_CCR0_VECTOR            TIMER0_B0_VECTOR
// preprocessor setup for the project
#define TIMER_B0_CCR1_2_OV_VECTOR       TIMER0_B1_VECTOR
// preprocessor setup for the project
#define TIMER_B1_CCR0_VECTOR            TIMER1_B0_VECTOR
// preprocessor setup for the project
#define TIMER_B1_CCR1_2_OV_VECTOR       TIMER1_B1_VECTOR
// preprocessor setup for the project
#define TIMER_B2_CCR0_VECTOR            TIMER2_B0_VECTOR
// preprocessor setup for the project
#define TIMER_B2_CCR1_2_OV_VECTOR       TIMER2_B1_VECTOR
// preprocessor setup for the project
#define TIMER_B3_CCR0_VECTOR            TIMER3_B0_VECTOR
// preprocessor setup for the project
#define TIMER_B3_CCR1_2_OV_VECTOR       TIMER3_B1_VECTOR

// Line detect states
// preprocessor setup for the project
#define BLACK_LINE              ('K')
// preprocessor setup for the project
#define LEFT                    ('1')
// preprocessor setup for the project
#define RIGHT                   ('2')
// preprocessor setup for the project
#define WHITE_BOARD             ('W')

// ADC
// preprocessor setup for the project
#define SWITCH_COUNT_BLACK      (1)
// preprocessor setup for the project
#define SWITCH_COUNT_WHITE      (2)
// preprocessor setup for the project
#define ADC_BLACK_OFFSET        (100)

// Default sensor thresholds
// preprocessor setup for the project
#define ADC_DEFAULT_BLACK_THRESHOLD (770)
// preprocessor setup for the project
#define ADC_DEFAULT_WHITE_THRESHOLD (620)

// Serial characters
// preprocessor setup for the project
#define CARRIAGE_RETURN         (0x0D)
// preprocessor setup for the project
#define LINE_FEED               (0x0A)
// preprocessor setup for the project
#define NULL_CHAR               (0x00)

// Ring buffer
// preprocessor setup for the project
#define RING_BUF_SIZE           (32)

// Baud rate identifiers
// preprocessor setup for the project
#define BAUD_115200             (0x01)
// preprocessor setup for the project
#define BAUD_460800             (0x02)

// Baud register values (SMCLK = 8MHz, UCOS16 = 1)
// preprocessor setup for the project
#define UCA_115200_BRW          (4)
// preprocessor setup for the project
#define UCA_115200_MCTLW        (0x5551)
// preprocessor setup for the project
#define UCA_460800_BRW          (1)
// preprocessor setup for the project
#define UCA_460800_MCTLW        (0x4A11)

// IOT command framing
// preprocessor setup for the project
#define CMD_START_CHAR          ('^')
// preprocessor setup for the project
#define IPD_PREFIX              "+IPD"
// preprocessor setup for the project
#define IPD_PREFIX_LEN          (4)

// Security PIN
// preprocessor setup for the project
#define IOT_PIN                 "6769"
// preprocessor setup for the project
#define IOT_PIN_LENGTH          (4)
// preprocessor setup for the project
#define PIN_OFFSET              (1)

// Drive command payload offsets
// preprocessor setup for the project
#define CMD_DIRECTION_OFFSET    (5)
// preprocessor setup for the project
#define CMD_TIME_OFFSET         (6)
// preprocessor setup for the project
#define NUM_TIME_DIGITS         (4)

// ASCII digit conversion
// preprocessor setup for the project
#define ASCII_ZERO              ('0')
// preprocessor setup for the project
#define ASCII_NINE              ('9')
// preprocessor setup for the project
#define DECIMAL_SHIFT           (10)

// Direction characters
// preprocessor setup for the project
#define CMD_FORWARD             ('F')
// preprocessor setup for the project
#define CMD_BACKWARD            ('B')
// preprocessor setup for the project
#define CMD_RIGHT               ('R')
// preprocessor setup for the project
#define CMD_LEFT                ('L')
// preprocessor setup for the project
#define CMD_DETECT              ('D')
// preprocessor setup for the project
#define CMD_ARRIVE              ('N')
// preprocessor setup for the project
#define CMD_ARRIVE_LEGACY       ('A')
// preprocessor setup for the project
#define CMD_EXIT                ('X')
// preprocessor setup for the project
#define CMD_CIRCLE              ('C')
// preprocessor setup for the project
#define CMD_ARC                 ('U')   // U: 1s fwd + 2s wide curve + 3s straight + auto BL
// preprocessor setup for the project
#define CMD_ARC_V               ('V')   // V: same arc, alt forward distances
// preprocessor setup for the project
#define CMD_PAD                 ('P')   // P: straight to pad 8, stop + display
// preprocessor setup for the project
#define CMD_STOP                ('S')

// U command sequence:
// Step 1: 1s  forward
// U command sequence:
// Step 1: 1s  forward
// Step 2: 90 degree right turn (tune U_90_RIGHT_TICKS)
// Step 3: 2s  forward
// Step 4: 90 degree right turn
// Step 5: 2s  forward -> auto BL
// preprocessor setup for the project
#define U_FORWARD_TICKS         (1 * TICKS_PER_SECOND)
// U turns were about 2-3 degrees short.
// Add 2 timer ticks = about 40 ms because TICKS_PER_SECOND is 50.
// preprocessor setup for the project
#define U_TURN_FINE_TICKS       (5)   // +1 tick: about 20 ms more, roughly 1-2 degrees sharper
// preprocessor setup for the project
#define U_90_RIGHT_TICKS        ((1 * TICKS_PER_SECOND) + U_TURN_FINE_TICKS)
// Middle forward was 3.1 sec. Add 0.1 sec -> 3.2 sec.
// preprocessor setup for the project
#define U_MID_FORWARD_TICKS     ((32 * TICKS_PER_SECOND) / 10) // 3.2 sec
// Final forward was 1.0 sec. Add about 0.25 sec.
// Timer ticks are 20 ms each, so 13 ticks = about 0.26 sec.
// preprocessor setup for the project
#define U_FINAL_FORWARD_EXTRA_TICKS (13)
// preprocessor setup for the project
#define U_FINAL_FORWARD_TICKS   ((1 * TICKS_PER_SECOND) + U_FINAL_FORWARD_EXTRA_TICKS)

// V command: same arc as U, alt forward distances
// preprocessor setup for the project
#define V_FORWARD_TICKS         (2 * TICKS_PER_SECOND)
// preprocessor setup for the project
#define V_FINAL_FORWARD_TICKS   (1 * TICKS_PER_SECOND)

// P command: straight to pad 8 for 1.5 sec then stop
// preprocessor setup for the project
#define P_FORWARD_TICKS         ((3 * TICKS_PER_SECOND) / 2)

// BL state timing
// preprocessor setup for the project
#define BL_PAUSE_TICKS          (500)
// preprocessor setup for the project
#define BL_CIRCLE_TICKS         (1200)
// preprocessor setup for the project
#define BL_START_SETTLE_TICKS   (50)
// preprocessor setup for the project
#define BL_SENSOR_IGNORE_TICKS  (5)   // shorter ignore so black is detected almost immediately
// preprocessor setup for the project
#define BL_TURN_SETTLE_TICKS    (50)
// preprocessor setup for the project
#define BL_TRAVEL_TICKS         (450)
// preprocessor setup for the project
#define BL_TURN_MAX_TICKS       (250)
// preprocessor setup for the project
#define BL_INTERCEPT_LOCK_TICKS (3)
// preprocessor setup for the project
#define BL_TURN_EXTRA_TICKS    (4)   // +1 tick: tiny sharper BL Turn alignment nudge
// preprocessor setup for the project
#define BL_SENSOR_DIFF_TICKS    (50)
// preprocessor setup for the project
#define BL_CIRCLE_DIFF_TICKS    (30)
// preprocessor setup for the project
#define BL_BLACK_MARGIN         (35)
// preprocessor setup for the project
#define BL_LOST_STOP_TICKS      (1)

// PWM percentages - single source of truth
// preprocessor setup for the project
#define PERCENT_4               (2000)
// preprocessor setup for the project
#define PERCENT_5               (2500)
// preprocessor setup for the project
#define PERCENT_10              (5000)
// preprocessor setup for the project
#define PERCENT_12              (6000)
// preprocessor setup for the project
#define PERCENT_12_5            (6250)
// preprocessor setup for the project
#define PERCENT_13              (6500)
// preprocessor setup for the project
#define PERCENT_15              (7500)
// preprocessor setup for the project
#define PERCENT_17              (8500)
// preprocessor setup for the project
#define PERCENT_18              (9000)
// preprocessor setup for the project
#define PERCENT_20              (10000)
// preprocessor setup for the project
#define PERCENT_22              (11000)
// preprocessor setup for the project
#define PERCENT_24              (12000)
// preprocessor setup for the project
#define PERCENT_25              (12500)
// preprocessor setup for the project
#define PERCENT_27              (13500)
// preprocessor setup for the project
#define PERCENT_27_5            (13750)
// preprocessor setup for the project
#define PERCENT_30              (15000)
// preprocessor setup for the project
#define PERCENT_34              (17000)
// preprocessor setup for the project
#define PERCENT_35              (17500)
// preprocessor setup for the project
#define PERCENT_40              (20000)
// preprocessor setup for the project
#define PERCENT_42              (21000)
// preprocessor setup for the project
#define PERCENT_45              (22500)
// preprocessor setup for the project
#define PERCENT_48              (24000)
// preprocessor setup for the project
#define PERCENT_50              (25000)
// preprocessor setup for the project
#define PERCENT_55              (27500)
// preprocessor setup for the project
#define PERCENT_57              (28500)
// preprocessor setup for the project
#define PERCENT_60              (30000)
// preprocessor setup for the project
#define PERCENT_63              (31500)
// preprocessor setup for the project
#define PERCENT_65              (32500)
// preprocessor setup for the project
#define PERCENT_70              (35000)
// preprocessor setup for the project
#ifndef PERCENT_80
// preprocessor setup for the project
#define PERCENT_80              (40000)
// preprocessor setup for the project
#endif
// preprocessor setup for the project
#define PERCENT_100             (50000)

// Turn duration trim
// preprocessor setup for the project
#define TURN_DURATION_TRIM_TICKS (5)

// Exit sequence timing
// preprocessor setup for the project
#define EXIT_TURN_TICKS         (80)
// preprocessor setup for the project
#define EXIT_FORWARD_TICKS      (120)

// Pad number range
// preprocessor setup for the project
#define PAD_MIN                 (0)
// preprocessor setup for the project
#define PAD_MAX                 (8)

// Display
// preprocessor setup for the project
#define DISPLAY_LINE_LEN        (10)
// preprocessor setup for the project
#define RETURN_WAIT_TICKS       (300)
// preprocessor setup for the project
#define MAX_COMMAND_LENGTH      (64)

// preprocessor setup for the project
#endif
