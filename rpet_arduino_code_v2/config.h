////////////////////////LCD shield/////////////////////////
#define LCD_en_pin 9  // EN pin
#define LCD_rs_pin 8  // RS pin
#define LCD_d7_pin 7  // D7 pin
#define LCD_d6_pin 6  // D6
#define LCD_d5_pin 5  // D5
#define LCD_d4_pin 4  // D4
#define BTN_pin A0    // Analog buttons

//lcd config
#define LCD_COLS 16
#define LCD_ROWS 2

//analog buttons values
#define btn_val_RIGHT 0
#define btn_val_UP 100
#define btn_val_DOWN 250
#define btn_val_LEFT 400
#define btn_val_SELECT 600

///////////////////////////TEMP///////////////////////////
#define Term_pin A1
#define PWM_pin 11  //Pin for PWM signal to the MOSFET driver for controling heating element(the BJT npn with pullup)
#define FAN_pin 10  //Pin for PWM signal to the MOSFET driver for controling fan
//#define FAN_2_pin 12

#define MAX_TEMP 300   //Maximum temp
#define MAX_PWM 255    //Maximum PWM
#define FAN_TEMP_ON 0  //When turn on the cooling fan for filament
#define FAN_PWM 255    //


////////////////////////STEPPER///////////////////////////
#define STP_E_pin A3
#define STP_D_pin A4
#define STP_S_pin A5

#define stepperMaxSpeed 2000  //2000
#define motorInterfaceType 1

/////////////////////////OTHER////////////////////////////
#define SERIAL_BAUD 115200
#define STOP_pin 2     //int
#define EndStop_pin A2 

//#define DEBUGTEMP


//////////////////////////////////////////////////////////
