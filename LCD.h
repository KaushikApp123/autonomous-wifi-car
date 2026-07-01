/*
 * Kaushik A
 * ECE 306 Project 10
 */
//------------------------------------------------------------------------------
// Macro Configurations for the LCD
//------------------------------------------------------------------------------
// LCD
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
// variable or helper used by the car
unsigned char CheckBusy(void);

// call a helper function
void SetPostion(char pos);
// call a helper function
void DisplayOnOff(char data);
// call a helper function
void lcd_BIG_mid(void);
// call a helper function
void lcd_BIG_bot(void);
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
void Display_Process(void);
// call a helper function
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
// variable or helper used by the car
int wait_for_character(void);
// call a helper function
void print_CR(void);
// call a helper function
void outchar(char character);

//------------------------------------------------------------------------------

//#define NULL ((void *) 0x0)
//#define LCD_INTERVAL         12500 // 8,000,000 / 8 / 8 / [1/100msec] = 12500

// LCD
// preprocessor setup for the project
#define LCD_HOME_L1           0x80
// preprocessor setup for the project
#define LCD_HOME_L2           0xA0
// preprocessor setup for the project
#define LCD_HOME_L3           0xC0
// preprocessor setup for the project
#define LCD_HOME_L4           0xE0

// preprocessor setup for the project
#define DISPLAY_ON 	          0x04
// preprocessor setup for the project
#define DISPLAY_OFF           0x03
// preprocessor setup for the project
#define CURSOR_ON             0x02
// preprocessor setup for the project
#define CURSOR_OFF            0x05
// preprocessor setup for the project
#define BLINK_ON              0x01
// preprocessor setup for the project
#define BLINK_OFF             0x06
// preprocessor setup for the project
#define BOTTOM                0x05
// preprocessor setup for the project
#define TOP                   0x06

// preprocessor setup for the project
#define CLEAR_DISPLAY         0x01
// preprocessor setup for the project
#define RETURN_HOME           0x02
// preprocessor setup for the project
#define POWER_DOWN_MODE       0x02
// preprocessor setup for the project
#define PD_BIT                0x01 // (set = enter power down mode)
// preprocessor setup for the project
#define ENTRY_MODE_SET        0x04
// preprocessor setup for the project
#define ID_BIT                0x02
// preprocessor setup for the project
#define S_BIT                 0x01
// preprocessor setup for the project
#define BDC_BIT               0x02
// preprocessor setup for the project
#define BDS_BIT               0x01
// preprocessor setup for the project
#define DISPLAY_CONTROL       0x08
// preprocessor setup for the project
#define D_BIT                 0x04
// preprocessor setup for the project
#define EXTENDED_FUNCTION_SET 0x08
// preprocessor setup for the project
#define FW_BIT                0x04
// preprocessor setup for the project
#define BW_BIT                0x02
// preprocessor setup for the project
#define NW_BIT                0x01
// preprocessor setup for the project
#define DH_BIAS_DOT_SHIFT     0x10
// preprocessor setup for the project
#define UD2_BIT               0x08
// preprocessor setup for the project
#define UD1_BIT               0x04
// preprocessor setup for the project
#define BS1_BIT               0x02
// preprocessor setup for the project
#define DH2_BIT               0x01
// preprocessor setup for the project
#define INTERNAL_OSC_FREQ     0x10
// preprocessor setup for the project
#define BS0_BIT               0x08
// preprocessor setup for the project
#define F2_BIT                0x04
// preprocessor setup for the project
#define F1_BIT                0x02
// preprocessor setup for the project
#define F0_BIT                0x01
// preprocessor setup for the project
#define FUNCTION_SET          0x20
// preprocessor setup for the project
#define DL_BIT                0x10
// preprocessor setup for the project
#define N_BIT                 0x08
// preprocessor setup for the project
#define DH_BIT                0x04
// preprocessor setup for the project
#define BE_BIT                0x04
// preprocessor setup for the project
#define RE_BIT                0x02
// preprocessor setup for the project
#define IS_BIT                0x01
// preprocessor setup for the project
#define REV_BIT               0x01
// preprocessor setup for the project
#define POWER_CONTROL         0x50
// preprocessor setup for the project
#define BON_BIT               0x04
// preprocessor setup for the project
#define C5_BIT                0x02
// preprocessor setup for the project
#define C4_BIT                0x01
// preprocessor setup for the project
#define FOLLOWER_CONTROL      0x60
// preprocessor setup for the project
#define DON_BIT               0x08
// preprocessor setup for the project
#define RAB2_BIT              0x04
// preprocessor setup for the project
#define RAB1_BIT              0x02
// preprocessor setup for the project
#define RAB0_BIT              0x01

// preprocessor setup for the project
#define START_WR_INSTRUCTION  0x1f
// preprocessor setup for the project
#define START_WR_DATA         0x5f

