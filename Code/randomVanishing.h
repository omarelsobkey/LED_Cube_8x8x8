unsigned char RV_leds[][10] = { { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
                                { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 } };

bool RV_startClosing = 0;

void RV_Display() {
  for (char layer = 0; layer < 8; layer++) {
    digitalWrite(latchPin, LOW);

    shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

    for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
      shiftOut(dataPin, clockPin, MSBFIRST, RV_leds[layer][layerBranch]);
    }

    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);

    delayMicroseconds(10);
  }
}

void RV_RandomLighten() {
light:
  char randomLayer = random(8);
  char randomRow = random(8);
  RV_leds[randomLayer][randomRow] |= (unsigned char)ceil(pow(2, random(8)));

  RV_Display();

  int sum = 0;
  for (char i = 0; i < 8; i++) {
    for (char j = 0; j < 8; j++) {
      sum += RV_leds[i][j];
    }
  }
  if (CUBE_CheckMode()) return;
  if (sum < 12000) goto light;
}

void RV_RandomTurnOff() {
close:
  char randomLayer = random(8);
  char randomRow = random(8);
  RV_leds[randomLayer][randomRow] &= ((unsigned char)255 ^ (unsigned char)ceil(pow(2, random(8))));

  RV_Display();

  int sum = 0;
  for (char i = 0; i < 8; i++) {
    for (char j = 0; j < 8; j++) {
      sum += RV_leds[i][j];
    }
  }
  if (CUBE_CheckMode()) return;
  if (sum > 1800) goto close;
}

void RV() {
  RV_RandomLighten();
  if (RV_modeChanged) {
    RV_modeChanged = 0;
    return;
  }

  RV_RandomTurnOff();
  if (RV_modeChanged) {
    RV_modeChanged = 0;
    return;
  }

  CUBE_CLR();
}
