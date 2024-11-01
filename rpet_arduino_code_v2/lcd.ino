/*
 *  lcd.ino
 *
 *  Contains pages data
 *
 *  Copyright (C) 2024 Matěj Mrkva
 *  matejmrkva@seznam.cz
 *
 *  This file is part of pet_filament_control https://github.com/matej2005/.
 *
 *  pet_filament_control is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */



//Make temperature character for LCD
#define temp_char 3
uint8_t tempChar[8] = { 0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x11, 0x11, 0x0E };
#define deg_char 4
uint8_t degChar[8] = { 0x0C, 0x12, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00 };
#define motion_char 5
uint8_t motionChar[8] = { 0x00, 0x04, 0x12, 0x09, 0x12, 0x04, 0x00, 0x00 };

const uint8_t cursorIcon = 0x7E;
const uint8_t editCursorIcon = 0x7F;
uint8_t *upArrow = new uint8_t[8]{ 0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04 };
uint8_t *downArrow = new uint8_t[8]{ 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04 };

void setup_lcd() {
  //Initialization of LCD
  lcd.begin(LCD_COLS, LCD_ROWS);

  lcd.createChar(temp_char, tempChar);
  lcd.createChar(deg_char, degChar);
  lcd.createChar(motion_char, motionChar);
  lcd.clear();
}
void renderPage(uint8_t page, uint8_t scroll) {
  lcd.setCursor(0, 0);
  lcd.clear();
  switch (page) {
    case HOME:  //home page
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.write(byte(temp_char));
      lcd.setCursor(2, 0);
      LCD_value_TEMP(0);
      lcd.setCursor(5, 0);
      lcd.print("/");
      lcd.setCursor(6, 0);
      LCD_value_SET_TEMP(TempPresets[TempPresetdefault]);
      lcd.setCursor(11, 0);
      LCD_value_INFO(stripeRunout);
      lcd.setCursor(0, 1);
      lcd.write(byte(motion_char));
      lcd.setCursor(2, 1);
      LCD_value_SPEED(abs(SpeedPresets[SpeedPresetdefault]));
      lcd.setCursor(7, 1);
      LCD_value_DIR(stepperDirection);
      break;
    case CONFIG:  //display config menu, now fixed only to two rows
      lcd.setCursor(1, 0);
      lcd.print(Omenu[scroll]);
      lcd.setCursor(1, 1);
      lcd.print(Omenu[scroll + 1]);
      lcd.setCursor(0, cursorY);
      lcd.write(cursorIcon);

      /*for (uint8_t i = 0; i < 2; i++) {
        lcd.print(menu[i]);
        lcd.setCursor(0, i);
      }*/
      /*Serial.print("CursorY: ");
      Serial.print(cursorY);
      Serial.print(" scroll: ");
      Serial.print(scroll);
      Serial.print(" index: ");
      Serial.println(cursorY + scroll);*/
      break;
    case TEMP_PRESETS:
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Temp preset: ");
      LCD_VALUE_PRESET(TempPresets[TempPresetdefault]);
      break;

    case SPEED_PRESETS:
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Speed preset: ");
      lcd.print(stepperEnable);
      LCD_VALUE_PRESET(SpeedPresets[SpeedPresetdefault]);

      break;
    case INFO:
      break;
    case STOP:
      lcd.noBlink();
      lcd.setCursor(5, 0);
      lcd.print("STOP");
      break;
    case ERROR:
      lcd.setCursor(0, 0);

      //lcd.print(menu[scroll]);
      //lcd.setCursor(0, 1);
      //lcd.print(menu[scroll + 1]);
      lcd.blink();
      lcd.setCursor(0, 0);
      break;
    default:
      Serial.print("Unknow page: ");
      Serial.println(page);
      break;
  }
  //delay(100);
}
void LCD_value_TEMP(uint16_t a) {
  lcd.setCursor(2, 0);
  lcd.print("   ");
  lcd.setCursor(2, 0);
  lcd.print(a);
}
void LCD_value_SET_TEMP(uint16_t a) {
  lcd.setCursor(6, 0);
  lcd.print("    ");
  lcd.setCursor(6, 0);
  lcd.print(a);
  if (a < 10) {
    lcd.setCursor(7, 0);
  } else if (a < 99) {
    lcd.setCursor(8, 0);
  } else if (a < 999) {
    lcd.setCursor(9, 0);
  }
  lcd.write(byte(deg_char));
}
void LCD_value_INFO(uint16_t a) {
  lcd.setCursor(11, 0);
  lcd.print("   ");
  lcd.setCursor(11, 0);
  switch (a) {
    case 1:
      lcd.print("OUT");
      break;

    default:
      lcd.print("   ");
      break;
  }
}
void LCD_value_SPEED(uint32_t a) {
  lcd.setCursor(2, 1);
  lcd.print("    ");
  lcd.setCursor(2, 1);
  lcd.print(a);
}
void LCD_value_DIR(bool a) {
  lcd.setCursor(7, 1);
  lcd.print("   ");
  lcd.setCursor(7, 1);
  if (a) {
    lcd.print("CW");
  } else {
    lcd.print("CCW");
  }
}
void LCD_VALUE_PRESET(int16_t t) {
  lcd.setCursor(13, 0);
  lcd.print("   ");
  lcd.setCursor(13, 0);
  lcd.print(t);
}