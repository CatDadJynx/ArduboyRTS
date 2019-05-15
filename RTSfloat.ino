#include <Arduboy2.h>
Arduboy2 arduboy;
#include "Resources.h"
#include "units.h"
#include "playerInput.h"

void setup()
{
  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(75);
  arduboy.initRandomSeed();
  populateResources();
  arduboy.clear();
}

void loop()
{
 if (!arduboy.nextFrame())
    return;
    arduboy.clear();
  arduboy.pollButtons();
  timer.currentMillis = millis();
  cameraInput();
  handleCursorInput();
  resourceDraw();
  drawCursor();
  drawRectangle();
  personSelection();
  deselectAll();
  drawPerson();
  personWalk();
  //addPersonAtCursor();
  unitMove();
  drawDebugInfo();
  arduboy.display();
}
