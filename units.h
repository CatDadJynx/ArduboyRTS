
struct Timer {
  unsigned long currentMillis;
  unsigned long previousMillis = 0;
};

Timer timer;
Point personPos;
constexpr uint8_t personMax = 40;

uint8_t personFrame = 0;
uint8_t walk; //turn into enum class

enum class PersonState : uint8_t
{
  notSelected,
  selected
};

struct Person
{
  Point position;
  PersonState state;
};

Person people[personMax]
// The first set of braces is 'all the people'
{
  // This set of braces is people[0]
  {
    // This is people[0].position
    { playerCursor.x, playerCursor.y },
    // This is people[0].state
    PersonState::notSelected
  }
  // The rest of your people can be left out and they'll just be 'zero initialised'
};

// You start off with 1 person initialised
uint8_t personCount = 1;
uint8_t personSelect;

void drawPerson() {
  for (uint8_t i = 0; i < personCount; i++) {
    Sprites::drawSelfMasked(people[i].position.x - camera.x, people[i].position.y  - camera.y, personSprite, personFrame);
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
  if (timer.currentMillis - timer.previousMillis >= 500) {
    for (uint8_t i = 0; i < personCount; i++) {
      walk = random(0, 4);
      switch ( walk ) {
        case 0:
          ++people[i].position.x;
          break;
        case 1:
          --people[i].position.x;
          break;
        case 2:
          ++people[i].position.y;
          break;
        case 3:
          --people[i].position.y;
          break;
      }
      animatePerson();
      timer.previousMillis = timer.currentMillis;
    }
  }
}

void personSelection() {
  for (uint8_t i = 0; i < personCount; i++) {
    if (people[i].state == PersonState::notSelected && arduboy.collide(toLocal(people[i].position), rectangle)){
      people[i].state = PersonState::selected;
      if (people[i].state == PersonState::selected) {
        ++personSelect;
      }
    }
    if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) {
      people[i].state = PersonState::notSelected;
      personSelect = 0;
    }
  }
}

void addPersonAt(int16_t x, int16_t y)
{
    // Avoid trying to add more than the maximum
    if (personCount < personMax)
    {
      // Person goes on the end of the list
      people[personCount].position = {x, y};
      ++personCount;
    }
}

void addPersonAtCursor(){
   if (arduboy.justPressed(B_BUTTON)) {
    addPersonAt(playerCursor.x + camera.x, playerCursor.y + camera.y);
  }
}

void drawDebugInfo()
{
  arduboy.setCursor(0, 0);
  arduboy.print(rectangle.x);

  arduboy.setCursor(20, 0);
  arduboy.print(rectangle.y);

  arduboy.setCursor(0, 20);
  arduboy.print("");

  arduboy.setCursor(20, 20);
  arduboy.print("");

  arduboy.setCursor(0, 40);
  arduboy.print(personCount);

  arduboy.setCursor(20, 40);
  arduboy.print(personSelect);
}
