#define MRV_repeat 10

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void MRV_Col_X(char iter) {
  for (char iterRepeat = 0; iterRepeat < MRV_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layerBranch == iter) shiftOut(dataPin, clockPin, MSBFIRST, 0xFF);
        else shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }

      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);

      delayMicroseconds(10);
    }
  }
}

void MRV_Col_Y(char iter) {
  for (char iterRepeat = 0; iterRepeat < MRV_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        shiftOut(dataPin, clockPin, MSBFIRST, 128 >> iter);
      }

      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);

      delayMicroseconds(10);
    }
  }
}

void MRV_Row(char layer) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

  for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
    shiftOut(dataPin, clockPin, MSBFIRST, 0xFF);
  }

  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);

  delay(100);
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void MRV_ColToBack() {
  for (char iter = 0; iter < 8; iter++) {
    MRV_Col_X(iter);
  }
}

void MRV_ColToFront() {
  for (char iter = 7; iter >= 0; iter--) {
    MRV_Col_X(iter);
  }
}

void MRV_ColToRight() {
  for (char iter = 0; iter < 8; iter++) {
    MRV_Col_Y(iter);
  }
}
void MRV_ColToLeft() {
  for (char iter = 7; iter >= 0; iter--) {
    MRV_Col_Y(iter);
  }
}

void MRV_RowToUp() {
  for (char layer = 0; layer < 8; layer++) {
    MRV_Row(layer);
  }
}

void MRV_RowToDown() {
  for (char layer = 7; layer >= 0; layer--) {
    MRV_Row(layer);
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void MRV() {
  MRV_ColToBack();
  if (CUBE_CheckMode()) return;

  MRV_ColToFront();
  if (CUBE_CheckMode()) return;

  CUBE_CLR();

  MRV_ColToRight();
  if (CUBE_CheckMode()) return;

  MRV_ColToLeft();
  if (CUBE_CheckMode()) return;

  CUBE_CLR();

  MRV_RowToUp();
  if (CUBE_CheckMode()) return;

  MRV_RowToDown();
  if (CUBE_CheckMode()) return;

  CUBE_CLR();
}