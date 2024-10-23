////////////////////////LCD shield/////////////////////////
#define LCD_en_pin 9  // EN pin
#define LCD_rs_pin 8  // RS pin
#define LCD_d7_pin 7  // D7 pin 
#define LCD_d6_pin 6  // D6
#define LCD_d5_pin 5  // D5
#define LCD_d4_pin 4  // D4
#define BTN_pin A0    // Analog buttons

//lcd config
#define lcd_chars 16
#define lcd_rows 2

//analog buttons values
#define RIGHT 0
#define UP 100
#define DOWN 250
#define LEFT 400
#define SELECT 600

//WIP
//#define lcd_i2c //WIP
//#define lcd_address 0x27

///////////////////////////TEMP///////////////////////////
#define Term_pin A1
#define PWM_pin 11    //Pin for PWM signal to the MOSFET driver for controling heating element(the BJT npn with pullup)
#define FAN_pin 10    //Pin for PWM signal to the MOSFET driver for controling fan

#define MAX_TEMP 260    //Maximum temp
#define MAX_PWM 255     //Maximum PWM
#define FAN_TEMP_ON 180 //When turn on the cooling fan for filament
#define FAN_PWM 255     //


////////////////////////STEPPER///////////////////////////
#define STP_E_pin A3
#define STP_D_pin A4
#define STP_S_pin A5

#define stepperMaxSpeed 1000//2000
#define motorInterfaceType 1

/////////////////////////OTHER////////////////////////////
#define SERIAL_BAUD 115200
#define STOP_pin 2 //int
#define EndStop_pin A2  //analog

//#define EEPROM_temps_adr 0
#define EEPROM_data_adr 0 
#define FilamentSensor_adr 0x75
#define ENABLE_PAT9125
//#define DEBUGTEMP

//////////////////////////////////////////////////////////


