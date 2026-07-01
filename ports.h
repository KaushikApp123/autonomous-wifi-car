/*
 * Kaushik A
 * ECE 306 Project 10
 */
// preprocessor setup for the project
#define FALSE                  (0x00) //
// preprocessor setup for the project
#define TRUE                   (0x01) //
// preprocessor setup for the project
#define MOTOR                  (0x00) //
// preprocessor setup for the project
#define SMCLK_OFF              (0x00) //
// preprocessor setup for the project
#define SMCLK_ON               (0x01) //
// preprocessor setup for the project
#define PORTS                  (0x00) // RED LED 0
// preprocessor setup for the project
#define PWM_MODE               (0x01) // GREEN LED 1
// preprocessor setup for the project
#define WHEEL_OFF              (0x00)
// preprocessor setup for the project
#define WHEEL_PERIOD          (50005)

// preprocessor setup for the project
#define STEP                   (2000)
// preprocessor setup for the project
#define FORWARD                (0x00) // FORWARD
// preprocessor setup for the project
#define REVERSE                (0x01) // REVERSE


// Port 1 Pins
// preprocessor setup for the project
#define RED_LED                (0x01) // 1.0 RED LED 0
// preprocessor setup for the project
#define A1_SEEED             (0x02) // 1.1 A1_SEEED
// preprocessor setup for the project
#define V_DETECT_L             (0x04) // 1.2 V_DETECT_L
// preprocessor setup for the project
#define V_DETECT_R             (0x08) // 1.3 V_DETECT_R
// preprocessor setup for the project
#define A4_SEEED             (0x10) // 1.4 A4_SEEED
// preprocessor setup for the project
#define V_THUMB                (0x20) // 1.5 V_THUMB
// preprocessor setup for the project
#define UCA0RXD                (0x40) // 1.6 Back Channel UCA0RXD
// preprocessor setup for the project
#define UCA0TXD                (0x80) // 1.7 Back Channel UCA0TXD

// Port 2 Pins
// preprocessor setup for the project
#define SLOW_CLK               (0x01) // 2.0 LCD Reset
// preprocessor setup for the project
#define CHECK_BAT              (0x02) // 2.1 CHECK_BAT
// preprocessor setup for the project
#define IR_LED                 (0x04) // 2.2 IR LED
// preprocessor setup for the project
#define SW2                    (0x08) // 2.3 SW2
// preprocessor setup for the project
#define IOT_RUN_CPU            (0x10) // 2.4 IOT_RUN_RED
// preprocessor setup for the project
#define DAC_ENB                (0x20) // 2.5 DAC_ENB
// preprocessor setup for the project
#define LFXOUT                 (0x40) // 2.6 XOUTR
// preprocessor setup for the project
#define LFXIN                  (0x80) // 2.7 XINR

// Port 3 Pins
// preprocessor setup for the project
#define TEST_PROBE             (0x01) // 3.0 TEST PROBE
// preprocessor setup for the project
#define OA2O                   (0x02) // 3.1 OA2O
// preprocessor setup for the project
#define OA2N                   (0x04) // 3.2 Photodiode Circuit
// preprocessor setup for the project
#define OA2P                   (0x08) // 3.3 Photodiode Circuit
// preprocessor setup for the project
#define SMCLK_OUT              (0x10) // 3.4 SMCLK
// preprocessor setup for the project
#define DAC_CNTL               (0x20) // 3.5 DAC_CNTL
// preprocessor setup for the project
#define IOT_LINK_CPU           (0x40) // 3.6 IOT_LINK_GRN
// preprocessor setup for the project
#define IOT_EN_CPU             (0x80) // 3.7 IOT_EN              1

// Port 4 Pins
// preprocessor setup for the project
#define RESET_LCD              (0x01) // 4.0 DAC_CNTL1
// preprocessor setup for the project
#define SW1                    (0x02) // 4.1 SW1
// preprocessor setup for the project
#define UCA1RXD                (0x04) // 4.2 Back Channel UCA1RXD
// preprocessor setup for the project
#define UCA1TXD                (0x08) // 4.3 Back Channel UCA1TXD
// preprocessor setup for the project
#define UCB1_CS_LCD            (0x10) // 4.4 Chip Select
// preprocessor setup for the project
#define UCB1CLK                (0x20) // 4.5 SPI mode - clock output�UCB1CLK
// preprocessor setup for the project
#define UCB1SIMO               (0x40) // 4.6 UCB1SIMO
// preprocessor setup for the project
#define UCB1SOMI               (0x80) // 4.7 UCB1SOMI

// Port 5 Pins
// preprocessor setup for the project
#define V_BAT                  (0x01) // 5.1 V_BAT
// preprocessor setup for the project
#define V_5_0                  (0x02) // 5.0 V_5_0
// preprocessor setup for the project
#define V_DAC                  (0x04) // 5.2 V_DAC
// preprocessor setup for the project
#define V_3_3                  (0x08) // 5.3 V_3_3
// preprocessor setup for the project
#define IOT_BOOT           (0x10) // 5.4 IOT_BOOT           1

// Port 6 Pins
// preprocessor setup for the project
#define LCD_BACKLITE           (0x01) // 6.0 LCD_BACKLITE
// preprocessor setup for the project
#define R_FORWARD              (0x02) // 6.1 P6_0_PWM
// preprocessor setup for the project
#define L_FORWARD              (0x04) // 6.2 P6_1_PWM
// preprocessor setup for the project
#define R_REVERSE              (0x08) // 6.3 P6_2_PWM
// preprocessor setup for the project
#define L_REVERSE              (0x10) // 6.4 P6_3_PWM
// preprocessor setup for the project
#define P6_5                   (0x20) // 6.5
// preprocessor setup for the project
#define GRN_LED                (0x40) // 6.6 GREEN LED
