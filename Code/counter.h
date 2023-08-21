#define COUNTER_repeat 7
#define COUNTER_delayingBetweenNumbers 300

unsigned char COUNTER_numbers[][10] = {
  { 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000 },
  { 0b00011000, 0b00100100, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b00111100, 0b00000000 },
  { 0b00111000, 0b00000100, 0b00000100, 0b00111000, 0b00000100, 0b00000100, 0b00111000, 0b00000000 },
  { 0b00000100, 0b00001100, 0b00010100, 0b00100100, 0b01111110, 0b00000100, 0b00000100, 0b00000000 },
  { 0b00111100, 0b00100000, 0b00100000, 0b00111100, 0b00000100, 0b00000100, 0b00111100, 0b00000000 },
  { 0b00111100, 0b00100000, 0b00100000, 0b00111100, 0b00100100, 0b00100100, 0b00111100, 0b00000000 },
  { 0b00111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b00110000, 0b00110000, 0b00000000 },
  { 0b00111100, 0b00100100, 0b00100100, 0b00111100, 0b00100100, 0b00100100, 0b00111100, 0b00000000 },
  { 0b00111100, 0b00100100, 0b00100100, 0b00111100, 0b00000100, 0b00000100, 0b00111100, 0b00000000 },
};

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void COUNTER_Count(char number) {
  for (char iter = 0; iter < 8; iter++) {
    for (char iterRepeat = 0; iterRepeat < COUNTER_repeat; iterRepeat++) {
      for (char layer = 0; layer < 8; layer++) {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

        for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
          if (layerBranch == iter) shiftOut(dataPin, clockPin, MSBFIRST, COUNTER_numbers[number][7 - layer]);
          else shiftOut(dataPin, clockPin, MSBFIRST, 0);
        }

        digitalWrite(latchPin, HIGH);
        digitalWrite(latchPin, LOW);

        delayMicroseconds(10);
      }
    }
  }
}

void COUNTER_CountUp() {
  for (char numberDisplayedIndex = 0; numberDisplayedIndex < 9; numberDisplayedIndex++) {
    COUNTER_Count(numberDisplayedIndex);

    CUBE_CLR();
    delay(COUNTER_delayingBetweenNumbers);
    if (CUBE_CheckMode()) return;
  }
}

void COUNTER_CountDown() {
  for (char numberDisplayedIndex = 8; numberDisplayedIndex >= 0; numberDisplayedIndex--) {
    COUNTER_Count(numberDisplayedIndex);

    CUBE_CLR();
    delay(COUNTER_delayingBetweenNumbers);
    if (CUBE_CheckMode()) return;
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void COUNTER() {
  COUNTER_CountDown();

  CUBE_CLR();
}