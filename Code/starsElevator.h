#define SE_repeat 5
#define SE_LedsMoved 1000
bool SE_inBot = 0, SE_inTop = 0;

unsigned char SE_Layers[][10] = {
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

void SE_Display() {
  for (char iterRepeat = 0; iterRepeat < SE_repeat; iterRepeat++) {
    for (char layer = 7; layer >= 0; layer--) {
      // this loop is for layers shifting
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

      // Serial.println((unsigned char)ceil(pow(2, j))); delay(500);
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        shiftOut(dataPin, clockPin, MSBFIRST, SE_Layers[layer][layerBranch]);
      }

      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);

      delayMicroseconds(10);
    }
  }
}

void SE_Init() {
  randomSeed(analogRead(A4));
  for (char i = 0; i < 8; i++) {
    SE_Layers[0][i] = random(255);
    SE_Layers[7][i] = 0xFF ^ SE_Layers[0][i];
  }
  SE_Display();
}

void SE_ModifyLedPosition(unsigned char led) {
  for (char layer = 1; layer < 7; layer++) {
    for (char row = 0; row < 8; row++) {
      if (SE_Layers[layer][row] == led) {
        if (SE_inBot) {
          SE_Layers[layer + 1][row] |= led;
          SE_Layers[layer][row] = 0;
          layer++;
          break;
        } else if (SE_inTop) {
          SE_Layers[layer - 1][row] |= led;
          SE_Layers[layer][row] = 0;
        }
      }
    }
  }
}

void SE_Start() {
  SE_inBot = 0;
  SE_inTop = 0;

  unsigned char randomRow = random(8);
  unsigned char randomLed = (unsigned char)ceil(pow(2, random(8)));

  if ((SE_Layers[0][randomRow] & randomLed) > 0) {
    // Serial.println("bot");

    SE_inBot = 1;
    SE_Layers[0][randomRow] ^= randomLed;
    SE_Layers[1][randomRow] = randomLed;
  } else {
    // Serial.println("top");

    SE_inTop = 1;
    SE_Layers[7][randomRow] ^= randomLed;
    SE_Layers[6][randomRow] = randomLed;
  }

  // Serial.println(SE_Layers[0][randomRow]);
  // Serial.println(randomLed);

  for (char iter = 0; iter < 8; iter++) {
    SE_Display();
    SE_ModifyLedPosition(randomLed);
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SE_StarsElevator() {
  if (SE_modeChanged) {
    SE_modeChanged = 0;
    SE_Init();
  }
  
  for (char i = 0; i < SE_LedsMoved; i++) {
    SE_Start();
    if (CUBE_CheckMode()) return;
  }

  CUBE_CLR();
}