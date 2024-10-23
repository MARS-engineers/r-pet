#define VERSION 2.0

void initAll() {
  Serial.begin(SERIAL_BAUD);

  Serial.println(F("RPET pulltruder (MARS) ver: 2.0  File: " __FILE__ " from " __DATE__ "\r\n"));

  setup_lcd();

  setPWMPrescalerM(PWM_pin, 1024);
  TempPreset = TempPresetdefault;
  setTemp();
  setup_temp();

  //Initialization of stepper motor
  stepperEnable = false;
  SpeedPreset = SpeedPresetdefault;
  setStepperSpeed();
  stepper.setMaxSpeed(stepperMaxSpeed);
  stepper.setSpeed(float(SET_SPEED));
  digitalWrite(STP_E_pin, !stepperEnable);
  settupCli();

  //setup_eeprom();
}

void init(uint8_t a, String name) {
  log(name);
  if (!a) {
    Serial.print(F(" ok\n"));
  } else {
    Serial.print(F(" error: "));
    Serial.println(a);
  }
}
void log(String a) {
  Serial.print(F("[ "));
  Serial.print(millis(), 4);
  Serial.print(F(" ] "));
  Serial.print(a);
}
/*
void SetError(byte errorCode) {
  error &= ~(1 << errorCode);
}
void ClearError(byte errorCode) {
  error |= (1 << errorCode);
}
bool GetError(byte errorCode) {
  return (error & (1 << errorCode)) != 0;
}*/