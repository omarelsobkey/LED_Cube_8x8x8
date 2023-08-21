unsigned char CUBE_LayerTest[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void CUBE_init() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(SW_Mode, INPUT_PULLUP);
}

void CUBE_CLR() {
  digitalWrite(latchPin, LOW);
  for (char layer = 0; layer < 9; layer++) shiftOut(dataPin, clockPin, LSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

bool CUBE_CheckMode() {
  if (!digitalRead(SW_Mode)) {
    if (mode == 1) SQUARES_modeChanged = 1;
    else if (mode == 2) RV_modeChanged = 1;
    else if (mode == 5) SE_modeChanged = 1;

    mode = (++mode) % 8;

    EEPROM.write(0, mode);

    CUBE_CLR();
    delay(1000);
    return 1;
  } else return 0;
}

void CUBE_AllLighten() {
Lighten:
  for (char layer = 0; layer < 8; layer++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

    for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
      shiftOut(dataPin, clockPin, MSBFIRST, CUBE_LayerTest[layerBranch]);
    }
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);

    delayMicroseconds(100);
  }

  if (!CUBE_CheckMode()) goto Lighten;
}
