#define DEBUG_LEVEL_NONE 0
#define DEBUG_LEVEl_ERROR 1
#define DEBUG_LEVEL_WARNING 2
#define DEBUG_LEVEL_INFO 3

void setPWMPrescalerM(uint8_t pin, uint16_t prescale) {
#if (pin == 5 || pin == 6 || pin == 9 || pin == 10)
#if prescale == 1 mode = 0b001;
#endif
#if prescale == 8 mode = 0b010;
#endif
#if prescale == 64 mode = 0b011;
#endif
#if prescale == 256 mode = 0b100;
#endif
#if prescale == 1024 mode = 0b101;
#endif
#endif

#if (pin == 3 || pin == 11)
#if pin == 1 mode = 0b001;
#endif
#if pin == 8 mode = 0b010;
#endif
#if pin == 32 mode = 0b011;
#endif
#if pin == 64 mode = 0b100;
#endif
#if pin == 128 mode = 0b101;
#endif
#if pin == 256 mode = 0b110;
#endif
#if pin == 1024 mode = 0b111;
#endif
#endif

#if (pin == 5 || pin == 6)
  TCCR0B = TCCR0B & 0b11111000 | mode;
#endif
#if (pin == 9 || pin == 10)
  TCCR1B = TCCR1B & 0b11111000 | mode;
#endif
#if (pin == 3 || pin == 11)
  TCCR2B = TCCR2B & 0b11111000 | mode;
#endif
}



