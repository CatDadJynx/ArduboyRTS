struct Timer {
  unsigned long currentMillis;
  unsigned long previousMillis = 0;
};

Point person;
Timer timer;

uint8_t personFrame = 0;
uint8_t walk;

void populatePerson() {
  person.x = playerCursor.x;
  person.y = playerCursor.y;
}

void drawPerson() {
  Sprites::drawSelfMasked(person.x + camera.x, person.y + camera.y, personSprite, personFrame);
}

void animatePerson() {
  if (personFrame < 2) {
    ++personFrame;
  }
  else
    personFrame = 0;
}

void personWalk() {
  if (timer.currentMillis - timer.previousMillis >= 500) {
    walk = random(0, 4);
    switch ( walk ) {
      case 0:
        ++person.x;
        animatePerson();
        break;
      case 1:
        --person.x;
        animatePerson();
        break;
      case 2:
        ++person.y;
        animatePerson();
        break;
      case 3:
        --person.y;
        animatePerson();
        break;
    }
    timer.previousMillis = timer.currentMillis;
  }
}
