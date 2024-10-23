//Macros for numbering pages. For easier reading
#define HOME 0
#define CONFIG 1
#define TEMP_PRESETS 2
#define SPEED_PRESETS 3
#define INFO 4
#define ERROR 200
#define STOP 255

struct _eepromObject {
  uint8_t TempPresetdefault;
  uint8_t SpeedPresetdefault;
  uint16_t TempPresets[10];
  uint16_t SpeedPresets[10];
};
_eepromObject eepromObject;

//presets for temp
const uint8_t TempPresetdefault = 0;
uint8_t TempPreset = TempPresetdefault;
uint16_t TempPresets[] = {
  0,
  100,
  180,
  200,
  205,
  210,
  215,
  220
};
const uint8_t TempPresetLength = (sizeof(TempPresets) / sizeof(TempPresets[0])) - 1;
double SET_TEMP = (double)TempPresets[TempPresetdefault];

//presets for speed
const uint8_t SpeedPresetdefault = 4;
uint8_t SpeedPreset = SpeedPresetdefault;
int16_t SpeedPresets[] = {
  -300
  -200,
  -150,
  -100,
  -50,
  0,
  40,
  50,
  100,
  2000
};
const uint8_t SpeedPresetLength = (sizeof(SpeedPresets) / sizeof(SpeedPresets[0])) - 1;
int16_t SET_SPEED = SpeedPresets[SpeedPresetdefault];

const String menu[] = {
  "Exit",
  "temp presets",
  "speed presets",
  "Info"
};
const uint8_t menuLength = (sizeof(menu) / sizeof(menu[0])) - 1;