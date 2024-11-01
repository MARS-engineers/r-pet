/*      DEBUG         */
//#define DEBUG_BTN_EN

#ifdef DEBUG_BTN_EN
#define DEBUG_BTN(input) Serial.print(input)
#define DEBUGln_BTN(input) Serial.println(input)
#else
#define DEBUG_BTN(input)
#define DEBUGln_BTN(input)
#endif



void buttonTick() {
  //script to handle button presses and menus
  if (anyButton()) {
    currentMillis = millis();
    if ((currentMillis - buttonsMillis >= 300)) {
      buttonsMillis = currentMillis;
      switch (page) {
        case HOME:  //home

          if (button(btn_val_UP) || button(btn_val_DOWN)) {
            if (button(btn_val_UP)) {
              if (TempPreset < TempPresetLength) TempPreset++;
            } else if (TempPreset > 0) TempPreset--;

            setTemp();

          } else if (button(btn_val_LEFT) || button(btn_val_RIGHT)) {
            if (button(btn_val_LEFT)) {
              if (SpeedPreset > 0) SpeedPreset--;
            } else if (SpeedPreset < SpeedPresetLength) SpeedPreset++;

            setStepperSpeed();

          } else if (button(btn_val_SELECT)) {
            page = CONFIG;
            renderPage(CONFIG, 0);  //display menu
          }

          break;
        case CONFIG:
        
          if (button(btn_val_SELECT)) {
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
          } else if (button(btn_val_DOWN)) {
            if (cursorY <= 0) cursorY++;
            else if (menuScroll1 <= (menuLength / LCD_ROWS)) {
              cursorY = 0;
              menuScroll1 = menuScroll1 + LCD_ROWS;
            } else cursorY = 1;

            renderPage(CONFIG, menuScroll1);
            lcd.setCursor(0, cursorY);
          } else if (button(btn_val_UP)) {
            if (cursorY >= 1) cursorY--;
            else {
              if (cursorY != 0) cursorY = 1;
              if (menuScroll1 >= 1) menuScroll1 = menuScroll1 - LCD_ROWS;
              else menuScroll1 = 0;
            }

            renderPage(CONFIG, menuScroll1);
            lcd.setCursor(0, cursorY);
          }
          break;
        case TEMP_PRESETS:
          if (button(btn_val_UP) || button(btn_val_DOWN)) {
            if (button(btn_val_UP) && menuScroll2 < TempPresetLength) menuScroll2++;
            else if (button(btn_val_DOWN) && menuScroll2 > 0) menuScroll2--;
            LCD_VALUE_PRESET(menuScroll2);
            lcd.setCursor(0, 1);
            lcd.print("Set temp: ");
            lcd.setCursor(11, 1);
            lcd.print(TempPresets[menuScroll2]);
          } else if (button(btn_val_SELECT)) {
            page = CONFIG;
            renderPage(CONFIG, menuScroll1);
          }
          break;

        case SPEED_PRESETS:  //motor config page
          if (button(btn_val_UP) || button(btn_val_DOWN)) {
            if (button(btn_val_UP) && menuScroll2 < SpeedPresetLength) menuScroll2++;
            else if (button(btn_val_DOWN) && menuScroll2 > 0) menuScroll2--;
            LCD_VALUE_PRESET(menuScroll2);
            lcd.setCursor(0, 1);
            lcd.print("Set speed: ");
            lcd.setCursor(12, 1);
            lcd.print(SpeedPresets[menuScroll2]);
          } else if (button(btn_val_SELECT)) {
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

/*function for converting analog value to presset button*/
bool button(int a) {
  int ar = analogRead(BTN_pin);
  if (ar > 1000) return 0;
  else if (a >= (ar - 60) && a <= (ar + 60)) {
#ifdef DEBUG_BTN_EN
    DEBUG_BTN("Button: ");
    DEBUG_BTN(a);
    DEBUG_BTN(":");
    DEBUG_BTN(ar);
    DEBUG_BTN(": ");
    if (RIGHT >= (ar - 60) && RIGHT <= (ar + 60)) DEBUGln_BTN("RIGHT");
    else if (LEFT >= (ar - 60) && LEFT <= (ar + 60)) DEBUGln_BTN("LEFT");
    else if (UP >= (ar - 60) && UP <= (ar + 60)) DEBUGln_BTN("UP");
    else if (DOWN >= (ar - 60) && DOWN <= (ar + 60)) DEBUGln_BTN("DOWN");
    else if (SELECT >= (ar - 60) && SELECT <= (ar + 60)) DEBUGln_BTN("SELECT");
#endif
    return 1;
  } else return 0;
}
uint16_t buttonA() {  //return value of button that been pressed
  int ar = analogRead(BTN_pin);
  if (ar > 1000) return 0;
  if (btn_val_RIGHT >= (ar - 60) && btn_val_RIGHT <= (ar + 60)) return btn_val_RIGHT;
  else if (btn_val_LEFT >= (ar - 60) && btn_val_LEFT <= (ar + 60)) return btn_val_LEFT;
  else if (btn_val_UP >= (ar - 60) && btn_val_UP <= (ar + 60)) return btn_val_UP;
  else if (btn_val_DOWN >= (ar - 60) && btn_val_DOWN <= (ar + 60)) return btn_val_DOWN;
  else if (btn_val_SELECT >= (ar - 60) && btn_val_SELECT <= (ar + 60)) return btn_val_SELECT;
  return 0;
}

bool anyButton() {  // check for any button press
  int ar = analogRead(BTN_pin);
  if (ar > 1000) return 0;
  else if (btn_val_RIGHT >= (ar - 60) && btn_val_RIGHT <= (ar + 60)) {
    DEBUGln_BTN("RIGHT");
    return 1;
  } else if (btn_val_LEFT >= (ar - 60) && btn_val_LEFT <= (ar + 60)) {
    DEBUGln_BTN("LEFT");
    return 1;
  } else if (btn_val_UP >= (ar - 60) && btn_val_UP <= (ar + 60)) {
    DEBUGln_BTN("UP");
    return 1;
  } else if (btn_val_DOWN >= (ar - 60) && btn_val_DOWN <= (ar + 60)) {
    DEBUGln_BTN("DOWN");
    return 1;
  } else if (btn_val_SELECT >= (ar - 60) && btn_val_SELECT <= (ar + 60)) {
    DEBUGln_BTN("SELECT");
    return 1;
  } else return 0;
}