#define RAIN_repeat 8
#define RAIN_Iterations 100

unsigned char RAIN_layers[][10] = {
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
};

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void RAIN_Start() {
  bool flag = 0;
  char i = 0;

  if (flag == 0) i = 0;
  else i = 2;

  for (; i < 8; i += 3) {
    RAIN_layers[7][i] = (unsigned char)ceil(pow(2, random(8)));
  }


  for (char iterRepeat = 0; iterRepeat < RAIN_repeat; iterRepeat++) {
    for (char layer = 7; layer >= 0; layer--) {
      // this loop is for layers shifting
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

      // Serial.println((unsigned char)ceil(pow(2, j))); delay(500);
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        shiftOut(dataPin, clockPin, MSBFIRST, RAIN_layers[layer][layerBranch]);
      }

      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);

      delayMicroseconds(10);
    }
  }

  for (char layer = 0; layer < 7; layer++) {
    for (char row = 0; row < 8; row++) {
      RAIN_layers[layer][row] = RAIN_layers[layer + 1][row];
    }
  }

  flag = !flag;
}

void RAIN() {
  for (char i = 0; i < RAIN_Iterations; i++) {
    RAIN_Start();
    if (CUBE_CheckMode()) return;
  }

  CUBE_CLR();
}