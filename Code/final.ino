#include <EEPROM.h>
#include "variables.h"
#include "ledCube.h"
#include "squares.h"
#include "randomVanishing.h"
#include "movingRowsCols.h"
#include "starsElevator.h"
#include "counter.h"
#include "rain.h"

void setup() {
  Serial.begin(9600);

  CUBE_init();
  CUBE_CLR();

  mode = EEPROM.read(0) % 8;
}

void loop() {
  // if (!digitalRead(SW_Mode)) mode = (++mode) % 8;
  Serial.println(mode, DEC);

  if (mode == 0) COUNTER();
  else if (mode == 1) SQUARES();
  else if (mode == 2) RV();
  else if (mode == 3) MRV();
  else if (mode == 4) RAIN();
  else if (mode == 5) SE_StarsElevator();
  else if (mode == 6) CUBE_AllLighten();
  else RandomMode();

  CUBE_CLR();
  // delay(500);
}

void RandomMode() {

  char randomMode = random(500) % 5;

  if (randomMode == 0) SQUARES();
  else if (randomMode == 1) RV();
  else if (randomMode == 2) MRV();
  else if (randomMode == 3) RAIN();
  else if (randomMode == 4) SE_StarsElevator();
}
