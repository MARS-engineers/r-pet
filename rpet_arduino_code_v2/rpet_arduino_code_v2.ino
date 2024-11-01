//include all needed libraries
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <thermistor.h>  //Download it here: https://electronoobs.com/eng_arduino_thermistor.php
#include <AccelStepper.h>
#include <PID_v1.h>
#include <SimpleCLI.h>
#include "./config.h"
#include "vars.h"

LiquidCrystal lcd(LCD_rs_pin, LCD_en_pin, LCD_d4_pin, LCD_d5_pin, LCD_d6_pin, LCD_d7_pin);

//Settup class for stepper motor
AccelStepper stepper(motorInterfaceType, STP_S_pin, STP_D_pin);


bool stripeRunout = false, _debugTemp = true, stepperDirection, stepperEnable = 0;
uint8_t menuScroll2 = 0;  // Used for scrooling in menu page deepth 2
uint8_t menuScroll1 = 0;  // Used for scrooling in menu page deepth 1


//variables for menus
uint8_t cursorY = 0, page = 0;
double PID_Input, PID_Output, lastTemp = 0;

//delayless timing
unsigned long currentMillis = 0, lcdUpdateMillis = 0, buttonsMillis = 0, remoteMillis = 0;

void setTemp(int16_t temp = TempPresets[TempPreset]);
void setStepperSpeed(int16_t speed = SpeedPresets[SpeedPreset]);

//Initial setup
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("RPET pulltruder (MARS) ver: 2.0  File: " __FILE__ " from " __DATE__ "\r\n"));

  setup_lcd();

  setPWMPrescalerM(PWM_pin, 1024);
  TempPreset = TempPresetdefault;

  setTemp();
  setup_temp();

  //Pins settup
  pinMode(BTN_pin, INPUT);
  pinMode(PWM_pin, OUTPUT);
  pinMode(STP_E_pin, OUTPUT);
  pinMode(STP_D_pin, OUTPUT);
  pinMode(STP_S_pin, OUTPUT);
  pinMode(FAN_pin, OUTPUT);
  digitalWrite(FAN_pin, LOW);
  pinMode(EndStop_pin, INPUT_PULLUP);
  pinMode(STOP_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOP_pin), STOP_script, FALLING);  //attach interput to STOP pin for emergecy stop button
  digitalWrite(STP_D_pin, LOW);                                            //Set stepper to disabled state
  digitalWrite(STP_E_pin, HIGH);
  digitalWrite(STP_S_pin, LOW);

  //Initialization of stepper motor
  stepperEnable = false;
  SpeedPreset = SpeedPresetdefault;
  setStepperSpeed();
  stepper.setMaxSpeed(stepperMaxSpeed);
  stepper.setSpeed(float(SET_SPEED));
  digitalWrite(STP_E_pin, !stepperEnable);
  settupCli();
  //Initialization of stepper motor
  stepperEnable = false;
  SpeedPreset = SpeedPresetdefault;
  setStepperSpeed();
  stepper.setMaxSpeed(stepperMaxSpeed);
  stepper.setSpeed(float(SET_SPEED));
  digitalWrite(STP_E_pin, !stepperEnable);
  settupCli();

  renderPage(HOME, 0);
}
void loop() {  //infinite loop

  cliTick();  //check for incomming serial input
  buttonTick();

  stepper.runSpeed();

  if (stepperEnable) {  //run motor at constant speed if stepperEnable == true
    digitalWrite(STP_E_pin, LOW);
  } else digitalWrite(STP_E_pin, HIGH);  //stop motor, disable motor

  //check for change in temperature
  currentMillis = millis();
  if (page == HOME && (currentMillis - lcdUpdateMillis >= 200)) {
    lcdUpdateMillis = currentMillis;
    temp();     //calculate pid and set PWM
    endstop();  //check if any endstop is active
    if (lastTemp != PID_Input) {
      LCD_value_TEMP(int(PID_Input));
      lastTemp = PID_Input;
    }
  }

  currentMillis = millis();
  if (currentMillis - remoteMillis >= 5000) {
    if (_debugTemp) {
#ifdef DEBUGTEMP
      DebugTemp();
#endif
    }
    remoteMillis = currentMillis;
  }
}

//emergenci STOP script, called by interput
void STOP_script() {              //stops everything
  stepperEnable = false;          //prevents mtotor for turning on
  digitalWrite(STP_E_pin, HIGH);  //disable motor
  SET_TEMP = 0;                   //set temperature to 0
  Serial.print("STOP function has been run! \n Press the reset button.");
  renderPage(STOP, 0);
  while (1)
    ;  //infinite loop
}

void setStepperSpeed(int16_t speed) {
  SET_SPEED = speed;
  //stepperSpeed = speed;

  if (speed < 0) stepperDirection = true;
  else stepperDirection = false;

  if (speed == 0) stepperEnable = false;
  else stepperEnable = true;

  LCD_value_DIR(stepperDirection);
  LCD_value_SPEED(abs(SET_SPEED));
  Serial.print("Set speed: ");
  Serial.println(SET_SPEED);
  stepper.setSpeed(SET_SPEED);
}
void setTemp(int16_t temp) {
  SET_TEMP = (double)temp;
  LCD_value_SET_TEMP((int)SET_TEMP);
  Serial.print("Set temp: ");
  Serial.println(SET_TEMP, 1);
}
