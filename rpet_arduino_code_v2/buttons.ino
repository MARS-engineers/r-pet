/*      DEBUG         */
//#define DEBUG_BTN_EN

#ifdef DEBUG_BTN_EN
#define DEBUG_BTN(input) Serial.print(input)
#define DEBUGln_BTN(input) Serial.println(input)
#else
#define DEBUG_BTN(input)
#define DEBUGln_BTN(input)
#endif



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
uint8_t buttonA() { //return value of button that been pressed
  int ar = analogRead(BTN_pin);
  if (ar > 1000) return 0;
  if (RIGHT >= (ar - 60) && RIGHT <= (ar + 60)) return RIGHT;
  else if (LEFT >= (ar - 60) && LEFT <= (ar + 60)) return LEFT;
  else if (UP >= (ar - 60) && UP <= (ar + 60)) return UP;
  else if (DOWN >= (ar - 60) && DOWN <= (ar + 60)) return DOWN;
  else if (SELECT >= (ar - 60) && SELECT <= (ar + 60)) return SELECT;
}

bool anyButton() {  // check for any button press
  int ar = analogRead(BTN_pin);
  if (ar > 1000) return 0;
  else if (RIGHT >= (ar - 60) && RIGHT <= (ar + 60)) {
    DEBUGln_BTN("RIGHT");
    return 1;
  } else if (LEFT >= (ar - 60) && LEFT <= (ar + 60)) {
    DEBUGln_BTN("LEFT");
    return 1;
  } else if (UP >= (ar - 60) && UP <= (ar + 60)) {
    DEBUGln_BTN("UP");
    return 1;
  } else if (DOWN >= (ar - 60) && DOWN <= (ar + 60)) {
    DEBUGln_BTN("DOWN");
    return 1;
  } else if (SELECT >= (ar - 60) && SELECT <= (ar + 60)) {
    DEBUGln_BTN("SELECT");
    return 1;
  } else return 0;
}