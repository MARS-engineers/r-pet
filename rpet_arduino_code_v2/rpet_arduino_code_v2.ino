//include all needed libraries
#include "./config.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <AccelStepper.h>
#include <PID_v1.h>
#include <thermistor.h>  //Download it here: https://electronoobs.com/eng_arduino_thermistor.php
#include <SimpleCLI.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "vars.h"

//WIP
/*
#ifdef ENABLE_PAT9125
#include "./lib/pat9125/pat9125.h"
PAT9125 PAT(FilamentSensor_adr);
#endif
*/

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

  initAll();
}
void loop() {  //infinite loop

  temp();  //calculate pid and set PWM

  cliTick();  //check for incomming serial input

  endstop();  //check if any endstop is active

  buttonTick();

  stepper.runSpeed();

  if (stepperEnable) {  //run motor at constant speed if stepperEnable == true
    //stepper.di
    digitalWrite(STP_E_pin, LOW);
  } else digitalWrite(STP_E_pin, HIGH);  //stop motor, disable motor

  //check for change in temperature
  currentMillis = millis();
  if (page == HOME && (currentMillis - lcdUpdateMillis >= 200)) {
    lcdUpdateMillis = currentMillis;

    if (lastTemp != PID_Input) {
      LCD_value_TEMP(int(PID_Input));
      lastTemp = PID_Input;
    }
  }

  currentMillis = millis();
  if (currentMillis - remoteMillis >= 2000) {
    if (_debugTemp) {
#ifdef DEBUGTEMP
      DebugTemp();
#endif
    }
    remoteMillis = currentMillis;
  }
}

void buttonTick() {
  //script to handle button presses and menus
  if (anyButton()) {
    currentMillis = millis();
    if ((currentMillis - buttonsMillis >= 300)) {
      buttonsMillis = currentMillis;
      switch (page) {
        case HOME:  //home

          if (button(UP) || button(DOWN)) {
            if (button(UP)) {
              if (TempPreset < TempPresetLength) TempPreset++;
            } else if (TempPreset > 0) TempPreset--;

            setTemp();

          } else if (button(LEFT) || button(RIGHT)) {
            if (button(LEFT)) {
              if (SpeedPreset > 0) SpeedPreset--;
            } else if (SpeedPreset < SpeedPresetLength) SpeedPreset++;

            setStepperSpeed();

          } else if (button(SELECT)) {
            page = CONFIG;
            renderPage(CONFIG, 0);  //display menu
          }

          break;
        case CONFIG:
          if (button(SELECT)) {
            switch (cursorY + menuScroll1) {
              case 0:  //exit
                page = HOME;
                renderPage(page, menuScroll1);
                break;
              case 1:  //temps preset
                page = TEMP_PRESETS;
                renderPage(page, 0);
                break;
              case 2:  //enable motor
                page = SPEED_PRESETS;
                renderPage(page, 0);
                break;
              case 3:  //about
                page = INFO;
                break;
            }
          } else if (button(DOWN)) {
            if (cursorY <= 0) cursorY++;
            else if (menuScroll1 <= (menuLength / lcd_rows)) {
              cursorY = 0;
              menuScroll1 = menuScroll1 + lcd_rows;
            } else cursorY = 1;

            renderPage(CONFIG, menuScroll1);
            lcd.setCursor(0, cursorY);
          } else if (button(UP)) {
            if (cursorY >= 1) cursorY--;
            else {
              if (cursorY != 0) cursorY = 1;
              if (menuScroll1 >= 1) menuScroll1 = menuScroll1 - lcd_rows;
              else menuScroll1 = 0;
            }

            renderPage(CONFIG, menuScroll1);
            lcd.setCursor(0, cursorY);
          }
          break;
        case TEMP_PRESETS:
          if (button(UP) || button(DOWN)) {
            if (button(UP) && menuScroll2 < TempPresetLength) menuScroll2++;
            else if (button(DOWN) && menuScroll2 > 0) menuScroll2--;
            LCD_VALUE_PRESET(menuScroll2);
            lcd.setCursor(0, 1);
            lcd.print("Set temp: ");
            lcd.setCursor(11, 1);
            lcd.print(TempPresets[menuScroll2]);
          } else if (button(SELECT)) {
            page = CONFIG;
            renderPage(CONFIG, menuScroll1);
          }
          break;

        case SPEED_PRESETS:  //motor config page
          if (button(UP) || button(DOWN)) {
            if (button(UP) && menuScroll2 < SpeedPresetLength) menuScroll2++;
            else if (button(DOWN) && menuScroll2 > 0) menuScroll2--;
            LCD_VALUE_PRESET(menuScroll2);
            lcd.setCursor(0, 1);
            lcd.print("Set speed: ");
            lcd.setCursor(12, 1);
            lcd.print(SpeedPresets[menuScroll2]);
          } else if (button(SELECT)) {
            page = CONFIG;
            renderPage(CONFIG, menuScroll1);
          }
          break;
        default:
          Serial.print("W: Unknow page: ");
          Serial.println(page);
          break;
      }
    }
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
  stepper.setSpeed(float(SET_SPEED));
}
