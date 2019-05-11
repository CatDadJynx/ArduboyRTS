#include <Arduboy2.h>
Arduboy2 arduboy;
#include "Cursor.h"
#include "Resources.h"
#include "units.h"

void setup()
{
  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
  populateResources();
  populatePerson();
  arduboy.clear();
}

void loop()
{
 if (!arduboy.nextFrame())
    return;
  arduboy.pollButtons();
  timer.currentMillis = millis();
  handleCursorInput();
  cameraInput();
  arduboy.clear();
  resourceDraw();
  drawPerson();
  personWalk();
  drawCursor();
  drawRectangle();
  //drawDebugInfo();
  arduboy.display();
}
