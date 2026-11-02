/*
 * Kaushik A
 * ECE 306 Project 10
 */
#ifndef MACROS_H
#define MACROS_H

// General
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define BEGINNING               (0)
#define TRUE                    (0x01)
#define FALSE                   (0x00)
#define OKAY                    (1)
#define NOT_OKAY                (0)
#define ONE_BYTE                (1)
#define TWO_BYTES               (2)

// LEDs and probes
#define RED_LED                 (0x01)
#define GRN_LED                 (0x40)
#define TEST_PROBE              (0x01)

// Button states
#define RELEASED                (1)
#define PRESSED                 (0)
#define DEBOUNCE_RESTART        (0)
#define DEBOUNCE_TIME           (10000)

// Timer intervals
#define TB0CCR0_INTERVAL        (2500)
#define TB0CCR1_INTERVAL        (6250)
#define TB0CCR2_INTERVAL        (6250)
#define TICKS_PER_SECOND        (50)

// Timer vectors
#define TIMER_B0_CCR0_VECTOR            TIMER0_B0_VECTOR
#define TIMER_B0_CCR1_2_OV_VECTOR       TIMER0_B1_VECTOR
#define TIMER_B1_CCR0_VECTOR            TIMER1_B0_VECTOR
#define TIMER_B1_CCR1_2_OV_VECTOR       TIMER1_B1_VECTOR
#define TIMER_B2_CCR0_VECTOR            TIMER2_B0_VECTOR
#define TIMER_B2_CCR1_2_OV_VECTOR       TIMER2_B1_VECTOR
#define TIMER_B3_CCR0_VECTOR            TIMER3_B0_VECTOR
#define TIMER_B3_CCR1_2_OV_VECTOR       TIMER3_B1_VECTOR

// Line detect states
#define BLACK_LINE              ('K')
#define LEFT                    ('1')
#define RIGHT                   ('2')
#define WHITE_BOARD             ('W')

// ADC
#define SWITCH_COUNT_BLACK      (1)
#define SWITCH_COUNT_WHITE      (2)
#define ADC_BLACK_OFFSET        (100)

// Default sensor thresholds
#define ADC_DEFAULT_BLACK_THRESHOLD (770)
#define ADC_DEFAULT_WHITE_THRESHOLD (620)

// Serial characters
#define CARRIAGE_RETURN         (0x0D)
#define LINE_FEED               (0x0A)
#define NULL_CHAR               (0x00)

// Ring buffer
#define RING_BUF_SIZE           (32)

// Baud rate identifiers
#define BAUD_115200             (0x01)
#define BAUD_460800             (0x02)

// Baud register values (SMCLK = 8MHz, UCOS16 = 1)
#define UCA_115200_BRW          (4)
#define UCA_115200_MCTLW        (0x5551)
#define UCA_460800_BRW          (1)
#define UCA_460800_MCTLW        (0x4A11)

// IOT command framing
#define CMD_START_CHAR          ('^')
#define IPD_PREFIX              "+IPD"
#define IPD_PREFIX_LEN          (4)

// Security PIN
#define IOT_PIN                 "6769"
#define IOT_PIN_LENGTH          (4)
#define PIN_OFFSET              (1)

// Drive command payload offsets
#define CMD_DIRECTION_OFFSET    (5)
#define CMD_TIME_OFFSET         (6)
#define NUM_TIME_DIGITS         (4)

// ASCII digit conversion
#define ASCII_ZERO              ('0')
#define ASCII_NINE              ('9')
#define DECIMAL_SHIFT           (10)

// Direction characters
#define CMD_FORWARD             ('F')
#define CMD_BACKWARD            ('B')
#define CMD_RIGHT               ('R')
#define CMD_LEFT                ('L')
#define CMD_DETECT              ('D')
#define CMD_ARRIVE              ('N')
#define CMD_ARRIVE_LEGACY       ('A')
#define CMD_EXIT                ('X')
#define CMD_CIRCLE              ('C')
#define CMD_ARC                 ('U')   // U: 1s fwd + 2s wide curve + 3s straight + auto BL
#define CMD_ARC_V               ('V')   // V: same arc, alt forward distances
#define CMD_PAD                 ('P')   // P: straight to pad 8, stop + display
#define CMD_STOP                ('S')

// U command sequence:
// Step 1: 1s  forward
// U command sequence:
// Step 1: 1s  forward
// Step 2: 90 degree right turn (tune U_90_RIGHT_TICKS)
// Step 3: 2s  forward
// Step 4: 90 degree right turn
// Step 5: 2s  forward -> auto BL
#define U_FORWARD_TICKS         (1 * TICKS_PER_SECOND)
// U turns were about 2-3 degrees short.
// Add 2 timer ticks = about 40 ms because TICKS_PER_SECOND is 50.
#define U_TURN_FINE_TICKS       (5)   // +1 tick: about 20 ms more, roughly 1-2 degrees sharper
#define U_90_RIGHT_TICKS        ((1 * TICKS_PER_SECOND) + U_TURN_FINE_TICKS)
// Middle forward was 3.1 sec. Add 0.1 sec -> 3.2 sec.
#define U_MID_FORWARD_TICKS     ((32 * TICKS_PER_SECOND) / 10) // 3.2 sec
// Final forward was 1.0 sec. Add about 0.25 sec.
// Timer ticks are 20 ms each, so 13 ticks = about 0.26 sec.
#define U_FINAL_FORWARD_EXTRA_TICKS (13)
#define U_FINAL_FORWARD_TICKS   ((1 * TICKS_PER_SECOND) + U_FINAL_FORWARD_EXTRA_TICKS)

// V command: same arc as U, alt forward distances
#define V_FORWARD_TICKS         (2 * TICKS_PER_SECOND)
#define V_FINAL_FORWARD_TICKS   (1 * TICKS_PER_SECOND)

// P command: straight to pad 8 for 1.5 sec then stop
#define P_FORWARD_TICKS         ((3 * TICKS_PER_SECOND) / 2)

// BL state timing
#define BL_PAUSE_TICKS          (500)
#define BL_CIRCLE_TICKS         (1200)
#define BL_START_SETTLE_TICKS   (50)
#define BL_SENSOR_IGNORE_TICKS  (5)   // shorter ignore so black is detected almost immediately
#define BL_TURN_SETTLE_TICKS    (50)
#define BL_TRAVEL_TICKS         (450)
#define BL_TURN_MAX_TICKS       (250)
#define BL_INTERCEPT_LOCK_TICKS (3)
#define BL_TURN_EXTRA_TICKS    (4)   // +1 tick: tiny sharper BL Turn alignment nudge
#define BL_SENSOR_DIFF_TICKS    (50)
#define BL_CIRCLE_DIFF_TICKS    (30)
#define BL_BLACK_MARGIN         (35)
#define BL_LOST_STOP_TICKS      (1)

// PWM percentages - single source of truth
#define PERCENT_4               (2000)
#define PERCENT_5               (2500)
#define PERCENT_10              (5000)
#define PERCENT_12              (6000)
#define PERCENT_12_5            (6250)
#define PERCENT_13              (6500)
#define PERCENT_15              (7500)
#define PERCENT_17              (8500)
#define PERCENT_18              (9000)
#define PERCENT_20              (10000)
#define PERCENT_22              (11000)
#define PERCENT_24              (12000)
#define PERCENT_25              (12500)
#define PERCENT_27              (13500)
#define PERCENT_27_5            (13750)
#define PERCENT_30              (15000)
#define PERCENT_34              (17000)
#define PERCENT_35              (17500)
#define PERCENT_40              (20000)
#define PERCENT_42              (21000)
#define PERCENT_45              (22500)
#define PERCENT_48              (24000)
#define PERCENT_50              (25000)
#define PERCENT_55              (27500)
#define PERCENT_57              (28500)
#define PERCENT_60              (30000)
#define PERCENT_63              (31500)
#define PERCENT_65              (32500)
#define PERCENT_70              (35000)
#ifndef PERCENT_80
#define PERCENT_80              (40000)
#endif
#define PERCENT_100             (50000)

// Turn duration trim
#define TURN_DURATION_TRIM_TICKS (5)

// Exit sequence timing
#define EXIT_TURN_TICKS         (80)
#define EXIT_FORWARD_TICKS      (120)

// Pad number range
#define PAD_MIN                 (0)
#define PAD_MAX                 (8)

// Display
#define DISPLAY_LINE_LEN        (10)
#define RETURN_WAIT_TICKS       (300)
#define MAX_COMMAND_LENGTH      (64)

#endif
