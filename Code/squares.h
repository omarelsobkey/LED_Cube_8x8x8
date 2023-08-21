#define SQUARES_repeat 11

unsigned char SQUARES_floorLayers[] = { 0b11111111, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b11111111 };
unsigned char SQUARES_centerLayers[] = { 0b10000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000001 };

// constants
const unsigned char SQUARES_constFloorLayers[] = { 0b11111111, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b11111111 };
const unsigned char SQUARES_constCenterLayers[] = { 0b10000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000001 };

const unsigned char SQUARES_B_constLayers[] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b11000000 };
const unsigned char SQUARES_F_constLayers[] = { 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };


/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_ArrClear_ToClose() {
  for (int i = 0; i < 8; i++) {
    SQUARES_floorLayers[i] = SQUARES_constFloorLayers[i];
    SQUARES_centerLayers[i] = SQUARES_constCenterLayers[i];
  }
}

void SQUARES_ArrClear_ToOpen_B() {
  for (int i = 0; i < 8; i++) {
    SQUARES_floorLayers[i] = SQUARES_B_constLayers[i];
    SQUARES_centerLayers[i] = SQUARES_B_constLayers[i];
  }
}

void SQUARES_ArrClear_ToOpen_F() {
  for (int i = 0; i < 8; i++) {
    SQUARES_floorLayers[i] = SQUARES_F_constLayers[i];
    SQUARES_centerLayers[i] = SQUARES_F_constLayers[i];
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_Middle_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));

      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (iter == 0) {
          if (layer == 0 || layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        } else if (iter == 1) {
          if (layer == 0 || layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 1 || layer == 6) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        } else if (iter == 2) {
          if (layer == 0 || layer == 1 || layer == 6 || layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 2 || layer == 5) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        } else if (iter == 3) {
          if (layer == 0 || layer == 1 || layer == 2 || layer == 5 || layer == 6 || layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_Middle_Close() {
  for (char iter = 0; iter < 4; iter++) {
    SQUARES_Middle_RevealLayers(iter);

    if (iter < 3) {
      for (char i = 0; i < 8; i++) {
        if (i <= iter) {
          SQUARES_floorLayers[i] = 0;
          SQUARES_centerLayers[i] = 0;
        } else if (i == iter + 1) {
          SQUARES_floorLayers[i] = (0xFF >> i) & (0xFF << i);
          SQUARES_centerLayers[i] = (128 >> i) | (1 << i);
        } else if (i == 6 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[iter + 1];
          SQUARES_centerLayers[i] = SQUARES_centerLayers[iter + 1];
        } else if (i >= 7 - iter) {
          SQUARES_floorLayers[i] = 0;
          SQUARES_centerLayers[i] = 0;
        } else {
          SQUARES_floorLayers[i] = SQUARES_centerLayers[iter + 1];
        }
      }
    }
  }
}

void SQUARES_Middle_Open() {
  for (char iter = 3; iter >= 0; iter--) {
    SQUARES_Middle_RevealLayers(iter);

    if (iter > 0) {
      for (char i = 0; i < 8; i++) {
        if (i < iter - 1) {
        } else if (i == iter - 1) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[i + 1] | (SQUARES_floorLayers[i + 1] << 1) | (SQUARES_floorLayers[i + 1] >> 1);
          SQUARES_centerLayers[i] = (16 << (3 - i)) | (8 >> (3 - i));
        } else if (i == iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[i - 1] - SQUARES_floorLayers[i];
          SQUARES_centerLayers[i] = 0;
        } else if (i == 7 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[iter];
          SQUARES_centerLayers[i] = SQUARES_centerLayers[iter];
        } else if (i == 8 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[iter - 1];
          SQUARES_centerLayers[i] = SQUARES_centerLayers[iter - 1];
        } else if (i > 8 - iter) {
        } else {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[iter];
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_FLB_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 0) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 7) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer >= 7) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer >= 6) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer >= 5) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer >= 4) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer >= 3) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer >= 2) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_FLB_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_FLB_RevealLayers(iter);

    if (iter < 6) {
      SQUARES_floorLayers[0] = SQUARES_floorLayers[0] << 1;
      SQUARES_centerLayers[0] = 128 | (SQUARES_centerLayers[0] << 1);

      for (char i = 1; i < 8; i++) {
        if (i < 6 - iter) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == 6 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[i + 1] << 1;
          SQUARES_floorLayers[i + 1] = 0;

          SQUARES_centerLayers[i] = 128 | (SQUARES_centerLayers[i + 1] << 1);
          SQUARES_centerLayers[i + 1] = 0;
        }
      }
    }
  }
}

void SQUARES_FLB_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_FLB_RevealLayers(iter);

    if (iter > 0) {
      SQUARES_floorLayers[0] = 128 | SQUARES_floorLayers[0] >> 1;
      SQUARES_centerLayers[0] = 128 | (1 << (iter - 1));

      for (char i = 1; i < 8; i++) {
        if (i < 7 - iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == 7 - iter) {
          SQUARES_floorLayers[i + 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i + 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_FRB_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 0) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer >= 7) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer >= 6) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer >= 5) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer >= 4) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer >= 3) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer >= 2) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_FRB_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_FRB_RevealLayers(iter);

    if (iter < 6) {
      SQUARES_floorLayers[0] = SQUARES_floorLayers[0] << 1;
      SQUARES_centerLayers[0] = 128 | (SQUARES_centerLayers[0] << 1);

      for (char i = 1; i < 8; i++) {
        if (i < 6 - iter) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == 6 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[i + 1] << 1;
          SQUARES_floorLayers[i + 1] = 0;

          SQUARES_centerLayers[i] = 128 | (SQUARES_centerLayers[i + 1] << 1);
          SQUARES_centerLayers[i + 1] = 0;
        }
      }
    }
  }
}

void SQUARES_FRB_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_FRB_RevealLayers(iter);

    if (iter > 0) {
      SQUARES_floorLayers[0] = 128 | SQUARES_floorLayers[0] >> 1;
      SQUARES_centerLayers[0] = 128 | (1 << (iter - 1));

      for (char i = 1; i < 8; i++) {
        if (i < 7 - iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == 7 - iter) {
          SQUARES_floorLayers[i + 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i + 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}


/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_FLT_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 7) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 0) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer <= 0) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer <= 1) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer <= 2) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer <= 3) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer <= 4) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer <= 5) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_FLT_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_FLT_RevealLayers(iter);

    if (iter < 6) {
      SQUARES_floorLayers[0] = SQUARES_floorLayers[0] << 1;
      SQUARES_centerLayers[0] = 128 | (SQUARES_centerLayers[0] << 1);

      for (char i = 1; i < 8; i++) {
        if (i < 6 - iter) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == 6 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[i + 1] << 1;
          SQUARES_floorLayers[i + 1] = 0;

          SQUARES_centerLayers[i] = 128 | (SQUARES_centerLayers[i + 1] << 1);
          SQUARES_centerLayers[i + 1] = 0;
        }
      }
    }
  }
}

void SQUARES_FLT_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_FLT_RevealLayers(iter);

    if (iter > 0) {
      SQUARES_floorLayers[0] = 128 | SQUARES_floorLayers[0] >> 1;
      SQUARES_centerLayers[0] = 128 | (1 << (iter - 1));

      for (char i = 1; i < 8; i++) {
        if (i < 7 - iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == 7 - iter) {
          SQUARES_floorLayers[i + 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i + 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_FRT_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 0) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer <= 0) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer <= 1) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer <= 2) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer <= 3) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer <= 4) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer <= 5) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_FRT_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_FRT_RevealLayers(iter);

    if (iter < 6) {
      SQUARES_floorLayers[0] = SQUARES_floorLayers[0] << 1;
      SQUARES_centerLayers[0] = 128 | (SQUARES_centerLayers[0] << 1);

      for (char i = 1; i < 8; i++) {
        if (i < 6 - iter) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == 6 - iter) {
          SQUARES_floorLayers[i] = SQUARES_floorLayers[i + 1] << 1;
          SQUARES_floorLayers[i + 1] = 0;

          SQUARES_centerLayers[i] = 128 | (SQUARES_centerLayers[i + 1] << 1);
          SQUARES_centerLayers[i + 1] = 0;
        }
      }
    }
  }
}

void SQUARES_FRT_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_FRT_RevealLayers(iter);

    if (iter > 0) {
      SQUARES_floorLayers[0] = 128 | SQUARES_floorLayers[0] >> 1;
      SQUARES_centerLayers[0] = 128 | (1 << (iter - 1));

      for (char i = 1; i < 8; i++) {
        if (i < 7 - iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == 7 - iter) {
          SQUARES_floorLayers[i + 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i + 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_BLB_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 0) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 7) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer >= 7) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer >= 6) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer >= 5) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer >= 4) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer >= 3) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer >= 2) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_BLB_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_BLB_RevealLayers(iter);
    if (iter < 6) {
      SQUARES_floorLayers[7] = SQUARES_floorLayers[7] << 1;
      SQUARES_centerLayers[7] = 128 | (SQUARES_centerLayers[7] << 1);

      for (char i = 0; i < 7; i++) {
        if (i > iter + 1) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == iter) {
          SQUARES_floorLayers[i + 1] = SQUARES_floorLayers[i] << 1;
          SQUARES_floorLayers[i] = 0;

          SQUARES_centerLayers[i + 1] = 128 | (SQUARES_centerLayers[i] << 1);
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

void SQUARES_BLB_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_BLB_RevealLayers(iter);
    if (iter > 0) {
      SQUARES_floorLayers[7] = 128 | SQUARES_floorLayers[7] >> 1;
      SQUARES_centerLayers[7] = 128 | (1 << (iter - 1));

      for (char i = 0; i < 7; i++) {
        if (i > iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == iter) {
          SQUARES_floorLayers[i - 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i - 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_BRB_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 0) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer >= 7) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer >= 6) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer >= 5) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer >= 4) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer >= 3) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer >= 2) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_BRB_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_BRB_RevealLayers(iter);
    if (iter < 6) {
      SQUARES_floorLayers[7] = SQUARES_floorLayers[7] << 1;
      SQUARES_centerLayers[7] = 128 | (SQUARES_centerLayers[7] << 1);

      for (char i = 0; i < 7; i++) {
        if (i > iter + 1) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == iter) {
          SQUARES_floorLayers[i + 1] = SQUARES_floorLayers[i] << 1;
          SQUARES_floorLayers[i] = 0;

          SQUARES_centerLayers[i + 1] = 128 | (SQUARES_centerLayers[i] << 1);
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

void SQUARES_BRB_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_BRB_RevealLayers(iter);
    if (iter > 0) {
      SQUARES_floorLayers[7] = 128 | SQUARES_floorLayers[7] >> 1;
      SQUARES_centerLayers[7] = 128 | (1 << (iter - 1));

      for (char i = 0; i < 7; i++) {
        if (i > iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == iter) {
          SQUARES_floorLayers[i - 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i - 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_BLT_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 7) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 0) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer <= 0) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer <= 1) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer <= 2) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer <= 3) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer <= 4) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer <= 5) shiftOut(dataPin, clockPin, MSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, MSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_BLT_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_BLT_RevealLayers(iter);
    if (iter < 6) {
      SQUARES_floorLayers[7] = SQUARES_floorLayers[7] << 1;
      SQUARES_centerLayers[7] = 128 | (SQUARES_centerLayers[7] << 1);

      for (char i = 0; i < 7; i++) {
        if (i > iter + 1) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == iter) {
          SQUARES_floorLayers[i + 1] = SQUARES_floorLayers[i] << 1;
          SQUARES_floorLayers[i] = 0;

          SQUARES_centerLayers[i + 1] = 128 | (SQUARES_centerLayers[i] << 1);
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

void SQUARES_BLT_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_BLT_RevealLayers(iter);
    if (iter > 0) {
      SQUARES_floorLayers[7] = 128 | SQUARES_floorLayers[7] >> 1;
      SQUARES_centerLayers[7] = 128 | (1 << (iter - 1));

      for (char i = 0; i < 7; i++) {
        if (i > iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == iter) {
          SQUARES_floorLayers[i - 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i - 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_BRT_RevealLayers(char iter) {
  for (char iterRepeat = 0; iterRepeat < SQUARES_repeat; iterRepeat++) {
    for (char layer = 0; layer < 8; layer++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, (unsigned char)ceil(pow(2, layer)));
      for (char layerBranch = 7; layerBranch >= 0; layerBranch--) {
        if (layer == 7) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);

        else if (iter == 0) {
          if (layer == 0) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 1) {
          if (layer <= 0) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 1) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 2) {
          if (layer <= 1) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 2) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 3) {
          if (layer <= 2) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 3) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 4) {
          if (layer <= 3) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 4) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 5) {
          if (layer <= 4) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 5) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }

        else if (iter == 6) {
          if (layer <= 5) shiftOut(dataPin, clockPin, LSBFIRST, 0);
          else if (layer == 6) shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_floorLayers[layerBranch]);
          else shiftOut(dataPin, clockPin, LSBFIRST, SQUARES_centerLayers[layerBranch]);
        }
      }
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
    }
  }
}

void SQUARES_BRT_Close() {
  for (char iter = 0; iter < 7; iter++) {
    SQUARES_BRT_RevealLayers(iter);
    if (iter < 6) {
      SQUARES_floorLayers[7] = SQUARES_floorLayers[7] << 1;
      SQUARES_centerLayers[7] = 128 | (SQUARES_centerLayers[7] << 1);

      for (char i = 0; i < 7; i++) {
        if (i > iter + 1) {
          SQUARES_floorLayers[i] = 128 | (SQUARES_floorLayers[i] << 1);
        } else if (i == iter) {
          SQUARES_floorLayers[i + 1] = SQUARES_floorLayers[i] << 1;
          SQUARES_floorLayers[i] = 0;

          SQUARES_centerLayers[i + 1] = 128 | (SQUARES_centerLayers[i] << 1);
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

void SQUARES_BRT_Open() {
  for (char iter = 6; iter >= 0; iter--) {
    SQUARES_BRT_RevealLayers(iter);
    if (iter > 0) {
      SQUARES_floorLayers[7] = 128 | SQUARES_floorLayers[7] >> 1;
      SQUARES_centerLayers[7] = 128 | (1 << (iter - 1));

      for (char i = 0; i < 7; i++) {
        if (i > iter) {
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));
        } else if (i == iter) {
          SQUARES_floorLayers[i - 1] = 128 | SQUARES_floorLayers[i] >> 1;
          SQUARES_floorLayers[i] = 128 | (1 << (iter - 1));

          SQUARES_centerLayers[i - 1] = 128 | (1 << (iter - 1));
          SQUARES_centerLayers[i] = 0;
        }
      }
    }
  }
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES_Middle() {
  SQUARES_ArrClear_ToClose();

  SQUARES_Middle_Close();
  SQUARES_Middle_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_FLB() {
  SQUARES_ArrClear_ToClose();

  SQUARES_FLB_Close();
  SQUARES_FLB_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_FRB() {
  SQUARES_ArrClear_ToClose();

  SQUARES_FRB_Close();
  SQUARES_FRB_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_FLT() {
  SQUARES_ArrClear_ToClose();

  SQUARES_FLT_Close();
  SQUARES_FLT_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_FRT() {
  SQUARES_ArrClear_ToClose();

  SQUARES_FRT_Close();
  SQUARES_FRT_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_BLB() {
  SQUARES_ArrClear_ToClose();

  SQUARES_BLB_Close();
  SQUARES_BLB_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_BRB() {
  SQUARES_ArrClear_ToClose();

  SQUARES_BRB_Close();
  SQUARES_BRB_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_BLT() {
  SQUARES_ArrClear_ToClose();

  SQUARES_BLT_Close();
  SQUARES_BLT_Open();

  SQUARES_ArrClear_ToClose();
}

void SQUARES_BRT() {
  SQUARES_ArrClear_ToClose();

  SQUARES_BRT_Close();
  SQUARES_BRT_Open();

  SQUARES_ArrClear_ToClose();
}

/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/
/**********************************************************************************************************************************************/

void SQUARES() {
  randomSeed(analogRead(A4));
  char SQUARES_mode = random(1000) % 8;

  if (SQUARES_modeChanged) {
    SQUARES_modeChanged = 0;
    
    if (SQUARES_mode == 0) {
      SQUARES_ArrClear_ToOpen_F();
      SQUARES_FLB_Open();
    } else if (SQUARES_mode == 1) {
      SQUARES_ArrClear_ToOpen_F();
      SQUARES_FRB_Open();
    } else if (SQUARES_mode == 2) {
      SQUARES_ArrClear_ToOpen_F();
      SQUARES_FLT_Open();
    } else if (SQUARES_mode == 3) {
      SQUARES_ArrClear_ToOpen_F();
      SQUARES_FRT_Open();
    } else if (SQUARES_mode == 4) {
      SQUARES_ArrClear_ToOpen_B();
      SQUARES_BLB_Open();
    } else if (SQUARES_mode == 5) {
      SQUARES_ArrClear_ToOpen_B();
      SQUARES_BRB_Open();
    } else if (SQUARES_mode == 6) {
      SQUARES_ArrClear_ToOpen_B();
      SQUARES_BLT_Open();
    } else {
      SQUARES_ArrClear_ToOpen_B();
      SQUARES_BRT_Open();
    }
    if (CUBE_CheckMode()) return;
  }

  SQUARES_mode = random(1000) % 9;
  if (SQUARES_mode == 0) {
    SQUARES_FLB();
  } else if (SQUARES_mode == 1) {
    SQUARES_FRB();
  } else if (SQUARES_mode == 2) {
    SQUARES_FLT();
  } else if (SQUARES_mode == 3) {
    SQUARES_FRT();
  } else if (SQUARES_mode == 4) {
    SQUARES_BLB();
  } else if (SQUARES_mode == 5) {
    SQUARES_BRB();
  } else if (SQUARES_mode == 6) {
    SQUARES_BLT();
  } else if (SQUARES_mode == 7) {
    SQUARES_BRT();
  } else {
    SQUARES_Middle();
  }
  if (CUBE_CheckMode()) return;


  CUBE_CLR();
}
