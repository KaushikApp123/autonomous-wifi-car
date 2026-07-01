/*
 * Kaushik A
 * ECE 306 Project 10
 */
//******************************************************************************
//
//  Description: This file contains the Function prototypes
//
//  Kaushik A
//  Aug 2013
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (5.40.1)
//******************************************************************************
// Functions

// Main
// call a helper function
void main(void);

// Initialization
// call a helper function
void Init_Conditions(void);

// Interrupts
// call a helper function
void enable_interrupts(void);
// call a helper function
__interrupt void Timer0_B0_ISR(void);
// call a helper function
__interrupt void switch_interrupt(void);

// Analog to Digital Converter

// Clocks
// call a helper function
void Init_Clocks(void);

// LED Configurations
// call a helper function
void Init_LEDs(void);
// call a helper function
void IR_LED_control(char selection);
// call a helper function
void Backlite_control(char selection);

  // LCD
// call a helper function
void Display_Process(void);
// call a helper function
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
// call a helper function
void enable_display_update(void);
// call a helper function
void update_string(char *string_data, int string);
// call a helper function
void Init_LCD(void);
// call a helper function
void lcd_clear(void);
// call a helper function
void lcd_putc(char c);
// call a helper function
void lcd_puts(char *s);

// call a helper function
void lcd_power_on(void);
// call a helper function
void lcd_write_line1(void);
// call a helper function
void lcd_write_line2(void);
//void lcd_draw_time_page(void);
//void lcd_power_off(void);
// call a helper function
void lcd_enter_sleep(void);
// call a helper function
void lcd_exit_sleep(void);
//void lcd_write(unsigned char c);
//void out_lcd(unsigned char c);

// call a helper function
void Write_LCD_Ins(char instruction);
// call a helper function
void Write_LCD_Data(char data);
// call a helper function
void ClrDisplay(void);
// call a helper function
void ClrDisplay_Buffer_0(void);
// call a helper function
void ClrDisplay_Buffer_1(void);
// call a helper function
void ClrDisplay_Buffer_2(void);
// call a helper function
void ClrDisplay_Buffer_3(void);

// call a helper function
void SetPostion(char pos);
// call a helper function
void DisplayOnOff(char data);
// call a helper function
void lcd_BIG_mid(void);
// call a helper function
void lcd_BIG_bot(void);
// call a helper function
void lcd_120(void);

// call a helper function
void lcd_4line(void);
// call a helper function
void lcd_out(char *s, char line, char position);
// call a helper function
void lcd_rotate(char view);

//void lcd_write(char data, char command);
// call a helper function
void lcd_write(unsigned char c);
// call a helper function
void lcd_write_line1(void);
// call a helper function
void lcd_write_line2(void);
// call a helper function
void lcd_write_line3(void);

// call a helper function
void lcd_command( char data);
// call a helper function
void LCD_test(void);
// call a helper function
void LCD_iot_meassage_print(int nema_index);

// Menu
// call a helper function
void Menu_Process(void);

// Ports
// call a helper function
void Init_Ports(void);
// call a helper function
void Init_Port_1(void);
// call a helper function
void Init_Port_2(void);
//void Init_Port3(char smclk);
// call a helper function
void Init_Port_3(void);
// call a helper function
void Init_Port_4(void);
// call a helper function
void Init_Port_5(void);
// call a helper function
void Init_Port_6(void);

// SPI
// call a helper function
void Init_SPI_B1(void);
// call a helper function
void SPI_B1_write(char byte);
// call a helper function
void spi_rs_data(void);
// call a helper function
void spi_rs_command(void);
// call a helper function
void spi_LCD_idle(void);
// call a helper function
void spi_LCD_active(void);
// call a helper function
void SPI_test(void);
// call a helper function
void WriteIns(char instruction);
// call a helper function
void WriteData(char data);

// Switches
// call a helper function
void Init_Switches(void);
// call a helper function
void switch_control(void);
// call a helper function
void enable_switch_SW1(void);
// call a helper function
void enable_switch_SW2(void);
// call a helper function
void disable_switch_SW1(void);
// call a helper function
void disable_switch_SW2(void);
// call a helper function
void Switches_Process(void);
// call a helper function
void Init_Switch(void);
// call a helper function
void Switch_Process(void);
// call a helper function
void Switch1_Process(void);
// call a helper function
void Switch2_Process(void);
// call a helper function
void menu_act(void);
// call a helper function
void menu_select(void);

// Timers
// call a helper function
void Init_Timers(void);
// call a helper function
void Init_Timer_B0(void);
// call a helper function
void Init_Timer_B1(void);
// call a helper function
void Init_Timer_B2(void);
// call a helper function
void Init_Timer_B3(void);

// call a helper function
void usleep(unsigned int usec);
// call a helper function
void usleep10(unsigned int usec);
// call a helper function
void five_msec_sleep(unsigned int msec);
// call a helper function
void measure_delay(void);
// call a helper function
void out_control_words(void);

// call a helper function
void movements(void);
// call a helper function
void Run_Circle(void);
// call a helper function
void Run_Eight(void);
// call a helper function
void Run_Triangle(void);
// call a helper function
void stop_movement(void);




