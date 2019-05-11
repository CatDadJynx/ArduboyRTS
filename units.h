
struct Timer {
  unsigned long currentMillis;
  unsigned long previousMillis = 0;
};

Timer timer;
Point personPos;

constexpr uint8_t personMax = 50;
uint8_t personFrame = 0;
uint8_t walk; //turn into enum class
uint16_t personX[personMax];
uint16_t personY[personMax];
uint8_t personCount;
uint8_t personSelect;
uint8_t personState[personMax];

void populatePerson() {
  personX[0] = playerCursor.x;
  personY[0] = playerCursor.y;
  personCount += 1;
}

void drawPerson() {
  for (uint8_t x = 0; x < personCount; x++) {
    Sprites::drawSelfMasked(personX[x] + camera.x, personY[x] + camera.y, personSprite, personFrame);
    personPos.x = personX[x];
    personPos.y = personY[x];
  }
}

void animatePerson() {
  if (personFrame < 2) {
    ++personFrame;
  }
  else
    personFrame = 0;
}

void personWalk() {
  for (uint8_t x = 0; x < personCount; x++) {
    if (timer.currentMillis - timer.previousMillis >= 500) {
      walk = random(0, 4);
      switch ( walk ) {
        case 0:
          ++personX[x];
          animatePerson();
          break;
        case 1:
          --personX[x];
          animatePerson();
          break;
        case 2:
          ++personY[x];
          animatePerson();
          break;
        case 3:
          --personY[x];
          animatePerson();
          break;
      }
      timer.previousMillis = timer.currentMillis;
    }
  }
}

void personSelection() {
  for (uint8_t x = 0; x < personCount; x++) {
    if (personState[x] == 0 && arduboy.collide(personPos, rectangle)) {
      personState[x] = 1;
      if (personState[x] = 1){
        ++personSelect;  
      }
    }
    if (arduboy.pressed(B_BUTTON)){
      personState[x] = 0;
      personSelect = 0;
    }
  }
}

void drawDebugInfo()
{
  arduboy.setCursor(0, 0);
  arduboy.print(playerCursor.x);

  arduboy.setCursor(20, 0);
  arduboy.print(playerCursor.y);

  arduboy.setCursor(0, 20);
  arduboy.print(rectangle.x);

  arduboy.setCursor(20, 20);
  arduboy.print(rectangle.y);

  arduboy.setCursor(0, 40);
  arduboy.print(personX[0]);

  arduboy.setCursor(20, 40);
  arduboy.print(personSelect);
}
