
struct Timer {
  unsigned long currentMillis;
  unsigned long previousMillis = 0;
};

Timer timer;
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
  PointF position;
  PersonState state;
};

Person people[personMax]
// The first set of braces is 'all the people'
{
  // This set of braces is people[0]
  {
    // This is people[0].position
    { playerCursor.localPosition.x, playerCursor.localPosition.y },
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
    if (people[i].state == PersonState::notSelected && intersect(toLocal(people[i].position), rectangle)) {
      people[i].state = PersonState::selected;
      if (people[i].state == PersonState::selected) {
        ++personSelect;
      }
    }
  }
}

void moveSelectedPeople() {
  for (uint8_t i = 0; i < personMax; ++i) {
    if (people[i].state == PersonState::selected) {
      const VectorF between = vectorBetween(people[i].position, playerCursor.lastGlobalPosition);
      const float distance = magnitude(between);
      const VectorF direction = normalise(between);
      const float speed = 1.1f;
      if (distance > 1 && playerCursor.lastGlobalPosition.x != 0 && playerCursor.lastGlobalPosition.y != 0) {
        const VectorF direction = { between.x / distance, between.y / distance };
        people[i].position = { (people[i].position.x + (direction.x * speed)), (people[i].position.y + (direction.y * speed)) };
      }
    }
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
