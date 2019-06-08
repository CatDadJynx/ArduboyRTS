#include <Arduboy2.h>

#include "Images.h"
#include "Timer.h"
#include "Resource.h"
#include "Building.h"
#include "Person.h"
#include "Camera.h"
#include "Cursor.h"
#include "Deer.h"

Arduboy2 arduboy;

Camera camera;

// Cursor starts off centre screen
Cursor cursor { { (WIDTH / 2), (HEIGHT / 2) } };

Point2F peopleTargetPosition { cursor.position.x, cursor.position.y };

struct Debug
{
  uint8_t deerCounter;
};

Debug debug;

constexpr uint8_t maxResources = 30;
constexpr uint8_t maxBuildings = 4;
constexpr uint8_t maxPeople = 40;
constexpr uint8_t maxDeer = 10;

uint16_t woodCounter = 0;
uint16_t meatCounter = 0;
Resource rock[maxResources];
Resource tree[maxResources];
Deer deer[maxDeer];

uint8_t buildingCount = 0;
Building buildings[maxBuildings];

// You start off with 1 person initialised
uint8_t personCount = 2;
Person people[maxPeople]
{
  {
    PersonState::Idle,
    { cursor.position.x, cursor.position.y },
  }
};

uint8_t personFrame = 0;
uint8_t deerFrame = 0;

uint8_t const *deerSprite = nullptr;

// The number of people selected
uint8_t peopleSelected = 0;

// The start of the selection rectangle
Point2 clickPoint;
Rectangle selectionRectangle;

// Timers
Timer peopleTimer;
Timer resourceTimer;
Timer regenTimer;
Timer deerTimer;

void setup()
{
  // Basic setup
  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(75);
  arduboy.initRandomSeed();

  // Randomise the resources
  populateResources();
  populateDeer();
}

void loop()
{
  if (!arduboy.nextFrame())
    return;

  // Update the button state
  arduboy.pollButtons();

  // Handle input
  handleInput();

  // Input independent updating
  update();

  // Clear the screen
  arduboy.clear();

  // Render everything
  render();

  // Update the screen
  arduboy.display();
}

void handleInput()
{
  constexpr uint8_t cursorMargin = 5;

  // Move cursor and camera
  if (arduboy.pressed(UP_BUTTON))
  {
    if (cursor.position.y >= cursorMargin)
      --cursor.position.y;
    else
      --camera.position.y;
  }

  if (arduboy.pressed(DOWN_BUTTON))
  {
    if (cursor.position.y <= (HEIGHT - cursorMargin))
      ++cursor.position.y;
    else
      ++camera.position.y;
  }

  if (arduboy.pressed(LEFT_BUTTON))
  {
    if (cursor.position.x >= cursorMargin)
      --cursor.position.x;
    else
      --camera.position.x;
  }

  if (arduboy.pressed(RIGHT_BUTTON))
  {
    if (cursor.position.x <= (WIDTH - cursorMargin))
      ++cursor.position.x;
    else
      ++camera.position.x;
  }

  // Start rectangle selection
  if (arduboy.justPressed(A_BUTTON))
  {
    clickPoint = { static_cast<int16_t>(cursor.position.x), static_cast<int16_t>(cursor.position.y) };
    selectionRectangle = Rectangle::fromPoints(clickPoint.x, clickPoint.y, selectionRectangle.getRight(), selectionRectangle.getBottom());
  }

  // During rectangle selection
  if (arduboy.pressed(A_BUTTON))
  {
    selectionRectangle = Rectangle::fromPoints(clickPoint.x, clickPoint.y, cursor.position.x, cursor.position.y);
    selectPeopleInRectangle(selectionRectangle);
  }

  // End rectangle selection
  if (arduboy.justReleased(A_BUTTON))
  {
    selectionRectangle = { 0, 0, 0, 0 };
  }

  // Command units
  if (arduboy.justPressed(B_BUTTON))
  {
    for (uint8_t i = 0; i < maxPeople; ++i)
      if (people[i].state == PersonState::Selected)
        people[i].state = PersonState::Moving;

    peopleTargetPosition = camera.toGlobal(cursor.position);
  }

  // Deselect all units
  if (arduboy.pressed(B_BUTTON) && arduboy.pressed(A_BUTTON))
    deselectAllPeople();

  // Add a building at the player's cursor
  if (arduboy.justPressed(B_BUTTON) && !arduboy.pressed(A_BUTTON))
    if ((woodCounter >= 1) && (peopleSelected == 0))
      addBuildingAt(camera.toGlobal(cursor.position));
}

void update()
{
  // Update timers
  regenTimer.updateCurrentTime();
  peopleTimer.updateCurrentTime();
  resourceTimer.updateCurrentTime();
  deerTimer.updateCurrentTime();

  // Handle resource regeneration
  if (regenTimer.getElapsedTime() >= 1000)
  {
    regenerateResources();
    regenerateDeer();
    regenTimer.updatePreviousTime();
  }

  // Handle people
  updatePeople();

  // Update people animation
  if (peopleTimer.getElapsedTime() >= 500)
  {
    updatePeopleAnimation();
    peopleTimer.updatePreviousTime();
  }

  // Update timer
  if (resourceTimer.getElapsedTime() >= 500)
  {
    resourceTimer.updatePreviousTime();
  }

  // Handle deer
  updateAllDeer();

  // Update deer animation
  if (deerTimer.getElapsedTime() >= 500)
  {
    updateDeerAnimation();
    deerTimer.updatePreviousTime();
  }
}

void render()
{
  drawWorld();
  drawUI();
}

void drawWorld()
{
  // Draw all the 'things'
  drawBuildings();
  drawResources();
  drawPeople();
  drawDeer();
}

void drawUI()
{
  // Draw cursor
  arduboy.fillCircle(cursor.position.x, cursor.position.y, cursor.radius, cursor.colour);

  // Draw selection rectangle
  arduboy.drawRect(selectionRectangle.x, selectionRectangle.y, selectionRectangle.width, selectionRectangle.height);

  // Draw debug info
  drawDebugInfo();
}

//
// People Management
//

void deselectAllPeople()
{
  peopleSelected = 0;

  for (uint8_t i = 0; i < maxPeople; ++i)
    people[i].state = PersonState::Idle;
}

void addPersonAt(float x, float y)
{
  // Avoid trying to add more than the maximum
  if (personCount < maxPeople)
  {
    // Person goes on the end of the list
    people[personCount].position = { x, y };
    ++personCount;
  }
}

void selectPeopleInRectangle(const Rectangle & rectangle)
{
  for (uint8_t i = 0; i < personCount; ++i)
  {
    // Skip non-idle people
    if (people[i].state != PersonState::Idle)
      continue;

    const Point2 position = { static_cast<int16_t>(people[i].position.x), static_cast<int16_t>(people[i].position.y) };

    // If the person is in the rectangle
    if (rectangle.contains(camera.toLocal(position)))
    {
      people[i].state = PersonState::Selected;
      ++peopleSelected;
    }
  }
}

//
// People Updating
//

void updatePeople()
{
  for (uint8_t i = 0; i < personCount; ++i)
    updatePerson(people[i]);
}

void updatePerson(Person & person)
{
  switch (person.state)
  {
    case PersonState::Idle:
      updatePersonIdle(person);
      break;
    case PersonState::Selected:
      updatePersonSelected(person);
      break;
    case PersonState::Moving:
      updatePersonMoving(person);
      break;
  }
}

void updatePersonIdle(Person & person)
{
  // If it's not time to walk, exit immediately
  if (peopleTimer.getElapsedTime() < 500)
    return;

  const uint8_t walkDirection = (rand() % 4);

  // An idle person just wanders around aimlessly
  switch (walkDirection)
  {
    case 0:
      ++person.position.x;
      break;
    case 1:
      --person.position.x;
      break;
    case 2:
      ++person.position.y;
      break;
    case 3:
      --person.position.y;
      break;
  }
}

void updatePersonSelected(Person &)
{
  // A selected person doesn't do anything
}

void updatePersonMoving(Person & person)
{
  const Vector2F between = vectorBetween(person.position, peopleTargetPosition);
  const float distance = between.getMagnitude();

  if (distance > 1)
  {
    // Manually normalise the vector
    const Vector2F direction = { between.x / distance, between.y / distance };
    person.position = { (person.position.x + (direction.x * Person::movementSpeed)), (person.position.y + (direction.y * Person::movementSpeed)) };
  }
  if (distance < 1)
  {
    if (resourceTimer.getElapsedTime() >= 500)
    {
      // For all trees
      for (uint8_t j = 0; j < maxResources; ++j)
      {
        // If the tree is active and the person is touching the tree
        // (Note: the intersection is more expensive, so do that last)
        if ((tree[j].state == ResourceState::Active) && areIntersecting(tree[j].getBounds(), person.getBounds()))
        {
          // Harvest the tree
          tree[j].harvest();
          ++woodCounter;
        }
      }
    }
    //separate
    else if (deerTimer.getElapsedTime() >= 500)
    {
      // For all deer
      for (uint8_t j = 0; j < maxDeer; ++j)
      {
        // If the deer is active and the person is touching the deer
        // (Note: the intersection is more expensive, so do that last)
        if (deer[j].state == DeerState::Idle || deer[j].state == DeerState::Running)
        {
          if (areIntersecting(deer[j].getBounds(), person.getBounds()))
          {
            // Hunt the deer
            deer[j].hunt();
            ++meatCounter;
            --debug.deerCounter;
          }
        }
      }
    }
  }
}

void updatePeopleAnimation()
{
  if (personFrame < 2)
    ++personFrame;
  else
    personFrame = 0;
}

//
// People Drawing
//

void drawPeople()
{
  for (uint8_t i = 0; i < personCount; ++i)
    drawPerson(people[i]);
}

void drawPerson(const Person & person)
{
  // Draw the person sprite
  const Point2F localPosition = camera.toLocal(person.position);
  Sprites::drawSelfMasked(localPosition.x, localPosition.y, personSprite, personFrame);

  // If the person is selected or moving
  if ((person.state == PersonState::Selected) || (person.state == PersonState::Moving))
  {
    constexpr uint8_t highlightMargin = 2;

    const int16_t highlightRectangleX = (static_cast<int16_t>(localPosition.x) - highlightMargin);
    const int16_t highlightRectangleY = (static_cast<int16_t>(localPosition.y) - highlightMargin);
    const int16_t highlightRectangleWidth = (Person::width + (highlightMargin * 2));
    const int16_t highlightRectangleHeight = (Person::height + (highlightMargin * 2));

    // Draw a rectangle around the person
    arduboy.drawRect(highlightRectangleX, highlightRectangleY, highlightRectangleWidth, highlightRectangleHeight);
  }
}

//
// Resource Updating
//

void regenerateResources()
{
  for (uint8_t i = 0; i < maxResources; ++i)
    if (tree[i].state == ResourceState::Inactive)
    {
      if (tree[i].regenerationDelay > 0)
        --tree[i].regenerationDelay;
      else
        tree[i].state = ResourceState::Active;
    }
}

void populateResources()
{
  for (uint8_t i = 0; i < maxResources; ++i)
  {
    rock[i].state = ResourceState::Active;
    rock[i].position.x = (random() % 250);
    rock[i].position.y = (random() % 250);

    tree[i].state = ResourceState::Active;
    tree[i].position.x = (random() % 250);
    tree[i].position.y = (random() % 250);
  }
}

//
// Resource Drawing
//

void drawResources()
{
  drawRocks();
  drawTrees();
}

void drawRocks()
{
  for (uint8_t i = 0; i < maxResources; ++i)
  {
    const Point2 localPosition = camera.toLocal(rock[i].position);
    const uint8_t frame = (rock[i].state == ResourceState::Inactive) ? 1 : 0;
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, rockSprite, frame);
  }
}

void drawTrees()
{
  for (uint8_t i = 0; i < maxResources; ++i)
  {
    const Point2 localPosition = camera.toLocal(tree[i].position);
    const uint8_t frame = (tree[i].state == ResourceState::Inactive) ? 1 : 0;
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, treeSprite, frame);
  }
}

//Deer stuff
void updateAllDeer()
{
  for (uint8_t i = 0; i < maxDeer; ++i)
    updateDeer(deer[i]);
}

void updateDeer(Deer & deer)
{
  switch (deer.state)
  {
    case DeerState::Idle:
      updateDeerIdle(deer);
      break;
    case DeerState::Running:
      updateDeerRunning(deer);
      break;
    case DeerState::Dead:
      break;
  }
}

void updateDeerIdle(Deer & deer)
{
  startDeerRunning(deer);
  // If it's not time to walk, exit immediately
  if (deerTimer.getElapsedTime() < 500)
    return;
  const uint8_t walkDirection = (rand() % 4);

  // An idle deer just wanders around aimlessly
  switch (walkDirection)
  {
    case 0:
      deerSprite = deerSpriteLeft;
      ++deer.position.x;
      ++deer.position.x;
      break;
    case 1:
      deerSprite = deerSpriteRight;
      --deer.position.x;
      --deer.position.x;
      break;
    case 2:
      ++deer.position.y;
      ++deer.position.y;
      break;
    case 3:
      --deer.position.y;
      --deer.position.y;
      break;
  }
}

bool areAnyPeopleInRangeSquared(const Point2F & position, float squaredRange)
{
  for (uint8_t i = 0; i < maxPeople; ++i)
    if (distanceSquared(position, people[i].position) <= squaredRange)
      return true;

  return false;
}

void startDeerRunning(Deer & deer) {
  if (areAnyPeopleInRangeSquared(deer.position, 20 * 20))
  {
    deer.state = DeerState::Running;
  }
}

void updateDeerRunning(Deer & deer)
{
  constexpr uint8_t runDistance = 30;

  uint8_t peopleInRange = 0;

  Directions directions = Directions::None;

  for (uint8_t index = 0; index < maxPeople; ++index)
  {
    const Vector2F distanceVector = vectorBetween(deer.position, people[index].position);
    const float distance = distanceVector.getMagnitude();

    if (distance <= runDistance)
    {
      directions |= getMostProminentAxis(distanceVector);
      ++peopleInRange;
    }
  }

  if(peopleInRange > 0)
  {
    // Needs to be normalised because of the way the vectors are stored
    const Vector2F escapeVector = normalise(getMovementVector(directions));
    deer.position += (escapeVector * Deer::movementSpeed);
  }
  else
  {
    deer.state = DeerState::Idle;
  }
}

Directions getMostProminentAxis(const Vector2F & vector)
{
  // Technically this is incorrect if x or y are -0
  const float absX = (vector.x < 0) ? -vector.x : vector.x;
  const float absY = (vector.y < 0) ? -vector.y : vector.y;

  if (absX > absY)
    return ((vector.x < 0) ? Directions::NegativeX : Directions::PositiveX);
  else
    return ((vector.y < 0) ? Directions::NegativeY : Directions::PositiveY);
}

Vector2F getMovementVector(Directions directions)
{
  struct CompactVector
  {
    int8_t x;
    int8_t y;
  };

  static const CompactVector vectors[] PROGMEM
  {
    // None
    { 0, 0 },

    // +X
    { -1, 0 },

    // +Y
    { 0, -1 },

    // +X, +Y
    { -1, -1 },

    // -X
    { 1, 0 },

    // -X, +X
    { 0, 1 },

    // -X, +Y
    { 1, 1 },

    // -X, +X, +Y
    { 0, 1 },

    // -Y
    { 0, 1 },

    // -Y, +X
    { -1, 1 },

    // -Y, +Y
    { 1, 0 },

    // -Y, +X, +Y
    { -1, 0 },

    // -Y, -X
    { 1, 1 },

    // -Y, -X, +X
    { 0, 1 },

    // -Y, -X, +Y
    { 1, 0 },

    // -Y, -X, +X, +Y
    { 0, 0 },
  };

  const size_t index = static_cast<size_t>(directions);
  const int8_t x = static_cast<int8_t>(pgm_read_byte(&vectors[index].x));
  const int8_t y = static_cast<int8_t>(pgm_read_byte(&vectors[index].y));

  return { static_cast<float>(x), static_cast<float>(y) };
}

void updateDeerAnimation()
{
  if (deerFrame < 3)
    ++deerFrame;
  else
    deerFrame = 0;
}

void regenerateDeer()
{
  for (uint8_t i = 0; i < maxDeer; ++i)
  {
    if (deer[i].state == DeerState::Dead)
    {
      if (deer[i].regenerationDelay > 0)
      {
        --deer[i].regenerationDelay;
      }
      else {
        populateDeer(deer[i]);
      }
    }
  }
}

//populate all deer
void populateDeer()
{
  for (uint8_t i = 0; i < maxDeer; ++i)
  {
    populateDeer(deer[i]);
  }
}

//populate single deer
void populateDeer(Deer & deer)
{
  ++debug.deerCounter;
  deerSprite = deerSpriteLeft;
  deer.state = DeerState::Idle;
  deer.position.x = (random() % 250);
  deer.position.y = (random() % 250);
}

//
// Deer Drawing
//

//All deer
void drawDeer()
{
  for (uint8_t i = 0; i < maxDeer; ++i)
    drawDeer(deer[i]);
}

//Single Deer
void drawDeer(const Deer & deer)
{
  // Draw the deer sprite
  const Point2F localPosition = camera.toLocal(deer.position);
  Sprites::drawSelfMasked(localPosition.x, localPosition.y, deerSprite, deerFrame);
}

//
// Building Management
//

void addBuildingAt(const Point2F & point)
{
  if (buildingCount < maxBuildings)
  {
    buildings[buildingCount].position = { static_cast<int16_t>(point.x), static_cast<int16_t>(point.y) };
    ++buildingCount;
  }
}

//c
// Building Drawing
//

void drawBuildings()
{
  for (uint8_t i = 0; i < buildingCount; ++i)
  {
    const Point2 localPosition = camera.toLocal(buildings[i].position);
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, houseSprite, 0);
  }
}
//
// Debugging
//

void drawDebugInfo()
{
  //arduboy.setCursor(0, 0);
  //arduboy.print(selectionRectangle.x);

  arduboy.setCursor(20, 0);
  arduboy.print(debug.deerCounter);

  //arduboy.setCursor(0, 20);
  //arduboy.print();

  //arduboy.setCursor(20, 20);
  //arduboy.print();

  //arduboy.setCursor(0, 40);
  //arduboy.print(debug);

  //arduboy.setCursor(20, 40);
  //arduboy.print(meatCounter);
}
