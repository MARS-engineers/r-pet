
/*
//Structure and function for working with EEPROM
struct EepromObject {
  uint8_t preheatT;
  uint8_t workingT;
  uint8_t minimumT;
  uint16_t stepperSpeed;
};
EepromObject dataToGet;

*/


void setup_eeprom() {
  /*
  //Reading from EEPROM stored settings
  EEPROM.get(EEPROM_temps_adr, dataToGet);


  preheatT = dataToGet.preheatT;
  workingT = dataToGet.workingT;
  minTemp = dataToGet.minimumT;
  stepperSpeed = dataToGet.stepperSpeed;
  */
}

void StoreToEEPROM() {
  /*
  EepromObject dataToStore = {
    preheatT,
    workingT,
    minTemp,
    stepperSpeed
  };
  EEPROM.put(EEPROM_temps_adr, dataToStore);
  */
}
void eepromGet() {
  EEPROM.get(EEPROM_data_adr, eepromObject);
}
void eepromPut() {
  EEPROM.put(EEPROM_data_adr, eepromObject);
}
//WIP
#define ADDRESS 0x00
void dump() {
  Wire.beginTransmission(ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  for (byte upper = 0; upper <= 8; upper++) {
    for (int i = 0; i < 32; i++) {
      Wire.beginTransmission(ADDRESS + upper);
      Wire.write(i * 8);
      Wire.endTransmission();

      Wire.beginTransmission(ADDRESS + upper);

      Wire.requestFrom(ADDRESS + upper, 8);

      //Wait till we get all the bytes
      while (Wire.available() < 8) {};

      printInt(upper);
      Serial.print("-");
      printInt(i * 8);
      Serial.print(":     ");

      for (int j = 0; j < 8; j++) {
        printInt(Wire.read());
        Serial.print("     ");
      }

      Serial.println();
      Wire.endTransmission();
    }
  }
}
void printInt(int i) {
  Serial.print("0x");

  if (i < 16)
    Serial.print("0");

  Serial.print(i, HEX);
}