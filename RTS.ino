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
  player();
  units();
  resources();
  drawDebugInfo();
  arduboy.display();
}

void units(){
  drawPerson();
  personWalk();
  //addPersonAtCursor();
  unitMove();
}

void resources(){
  resourceDraw();
}

void player(){
  drawCursor();
  drawRectangle();
  personSelection();
  deselectAll();
  cameraInput();
  handleCursorInput();
}