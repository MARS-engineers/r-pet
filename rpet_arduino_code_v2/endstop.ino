
#define Endstop_filament
#define Endstop_input
#define Endstop_output
bool endstop_trap = false;
uint8_t endstop() {

  if (digitalRead(EndStop_pin)) {
    if (!endstop_trap) {
      stripeRunout = true;
      Serial.println("W: Input string runout!");

      LCD_value_INFO(1);

      TempPreset = TempPresetdefault;
      setTemp();

      SpeedPreset = SpeedPresetdefault;
      setStepperSpeed();

      stepperEnable = false;
      endstop_trap = true;
    }
  } else {
    stripeRunout = false;
    LCD_value_INFO(0);
    endstop_trap = false;
  }
  return 0;
}

void reset_endstop() {
  endstop_trap = false;
}