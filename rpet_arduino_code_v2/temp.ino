

//Settup class for thermistor
thermistor therm1(Term_pin, 0);

//pid controler
const uint8_t Kp = 90, Ki = 30, Kd = 80;
PID myPID(&PID_Input, &PID_Output, &SET_TEMP, Kp, Ki, Kd, DIRECT);

void setup_temp() {
  myPID.SetMode(AUTOMATIC);
}

void temp() {
  PID_Input = therm1.analog2temp();  //Read the temperature from termistor

  myPID.Compute();                   //Compute PWM for heat element
  if (stripeRunout || (PID_Input < MAX_TEMP)) {
    analogWrite(PWM_pin, (int)PID_Output);  //Power heat element on givven PWM
  } else {
    digitalWrite(PWM_pin, LOW);  //Power heat element on givven PWM
  }

  if (PID_Input >= FAN_TEMP_ON) {
    analogWrite(FAN_pin, FAN_PWM);
  } else {
    digitalWrite(FAN_pin, LOW);
  }
}

void DebugTemp() {
  Serial.print("Řídící_veličina_(t):");
  Serial.print(SET_TEMP, 1);
  Serial.print(",");
  Serial.print("Zpětná_vazba_(t):");
  Serial.print(PID_Input);
  Serial.print(",");
  Serial.print("Akční_veličina_(pwm):");
  Serial.println(PID_Output);
}