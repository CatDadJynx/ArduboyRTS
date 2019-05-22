
struct Timer
{
  unsigned long currentMillis;
  unsigned long previousMillis = 0;

  unsigned long getElapsedTime() const
  {
    return (this->currentMillis - this->previousMillis);
  }

  void updatePreviousTime()
  {
    this->previousMillis = this->currentMillis;
  }

  void updateCurrentTime()
  {
    this->currentMillis = millis();
  }
};

Timer peopleTimer;
Timer resourceTimer;
Timer regenTimer;

constexpr uint8_t personMax = 40;

uint8_t personFrame = 0;
uint8_t walk; //turn into enum class

enum class PersonState : uint8_t
{
  Idle,
  Selected,
  Moving,
};

struct Person
{
  static constexpr uint8_t width = 5;
  static constexpr uint8_t height = 8;
  PointF position;
  PersonState state;

  RectF getBounds() const
  {
    return { position.x, position.y, width, height };
  }
};

Person people[personMax]
// The first set of braces is 'all the people'
{
  // This set of braces is people[0]
  {
    // This is people[0].position
    { playerCursor.localPosition.x, playerCursor.localPosition.y },
    // This is people[0].state
    PersonState::Idle
  }
  // The rest of your people can be left out and they'll just be 'zero initialised'
};

// You start off with 1 person initialised
uint8_t personCount = 1;
uint8_t personSelect;

void drawPerson() {
  for (uint8_t i = 0; i < personCount; i++) {
    const PointF localPosition = toLocal(people[i].position);
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, personSprite, personFrame);
    constexpr uint8_t highlightMargin = 2;
    if (people[i].state == PersonState::Selected || people[i].state == PersonState::Moving) {
      const int16_t highlightRectangleX = (static_cast<int16_t>(localPosition.x) - highlightMargin);
      const int16_t highlightRectangleY = (static_cast<int16_t>(localPosition.y) - highlightMargin);
      const int16_t highlightRectangleWidth = (5 + (highlightMargin * 2));
      const int16_t highlightRectangleHeight = (5 + (highlightMargin * 2));
      arduboy.drawRect(highlightRectangleX, highlightRectangleY, highlightRectangleWidth, highlightRectangleHeight);
    }
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
  if (peopleTimer.getElapsedTime() >= 500) {
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
    }
    peopleTimer.updatePreviousTime();
  }
}

void personSelection() {
  for (uint8_t i = 0; i < personCount; i++) {
    if (people[i].state == PersonState::Idle && intersect(toLocal(people[i].position), rectangle)) {
      people[i].state = PersonState::Selected;
      if (people[i].state == PersonState::Selected) {
        ++personSelect;
      }
    }
  }
}

void moveSelectedPeople() {
  for (uint8_t i = 0; i < personMax; ++i) {
    if (people[i].state == PersonState::Moving) {
      const VectorF between = vectorBetween(people[i].position, playerCursor.lastGlobalPosition);
      const float distance = magnitude(between);
      const VectorF direction = normalise(between);
      const float speed = 1.1f;
      if (distance > 1 && playerCursor.lastGlobalPosition.x != 0 && playerCursor.lastGlobalPosition.y != 0) {
        const VectorF direction = { between.x / distance, between.y / distance };
        people[i].position = { (people[i].position.x + (direction.x * speed)), (people[i].position.y + (direction.y * speed)) };
      }
      if (resourceTimer.getElapsedTime() >= 500) {
        if (distance < 1) {
          for (uint8_t j = 0; j < resourceMax; ++j) {
            if (intersect(tree[j].getBounds(), people[i].getBounds())) {
              if (tree[j].state == ResourceState::Active) {
                tree[j].resourceFrame = 1;
                tree[j].state = ResourceState::Inactive;
                ++resourceCounter;
              }
            }
          }
        }
        resourceTimer.updatePreviousTime();
      }
    }
  }
}

void resourceRegen() {
  if (regenTimer.getElapsedTime() >= 1000){
    for (uint8_t i = 0; i < resourceMax; ++i) {
       if (tree[i].state == ResourceState::Inactive) {
          ++tree[i].regenTimerCheck;
            if (tree[i].regenTimerCheck >= 10) {
              tree[i].resourceFrame = 0;
              tree[i].state = ResourceState::Active;
              tree[i].regenTimerCheck = 0;
            }
        }
      }
    regenTimer.updatePreviousTime(); 
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
    arduboy.print(resourceCounter);

    arduboy.setCursor(20, 40);
    arduboy.print(personSelect);
  }
