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
  uint8_t deerCount;
};

Debug debug;

constexpr uint8_t maxResources = 20;
constexpr uint8_t maxBuildings = 4;
constexpr uint8_t maxPeople = 40;
constexpr uint8_t maxDeer = 10;
constexpr uint8_t buildingMaxCapacity = 5;

uint16_t woodCount = 2;
uint16_t stoneCount = 0;
uint16_t foodCount = 10;
Resource rock[maxResources];
Resource tree[maxResources];
Resource appleTree[maxResources];
Deer deer[maxDeer];

constexpr uint8_t requiredFood = 5;
uint8_t buildingCount = 0;
Building buildings[maxBuildings];

// You start off with 1 person initialised
uint8_t personCount = 1;
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
Timer deerRegenTimer;

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
    if ((woodCount >= 1) && (peopleSelected == 0))
      addBuildingAt(camera.toGlobal(cursor.position));
}

void update()
{
  // Update timers
  regenTimer.updateCurrentTime();
  peopleTimer.updateCurrentTime();
  resourceTimer.updateCurrentTime();
  deerTimer.updateCurrentTime();
  deerRegenTimer.updateCurrentTime();

  // Handle resource regeneration
  if (regenTimer.getElapsedTime() >= 1000)
  {
    regenerateResources();
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
  //addPersonAtBuilding();
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

  // draw resource and people counters
  arduboy.setCursor(0, 0);
  arduboy.print(personCount);

  arduboy.setCursor(20, 0);
  arduboy.print(peopleSelected);

  arduboy.setCursor(0, 20);
  arduboy.print(foodCount);

  arduboy.setCursor(20, 20);
  arduboy.print(woodCount);

  arduboy.setCursor(0, 40);
  arduboy.print(stoneCount);
  // Draw debug info
  //drawDebugInfo();
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
          ++woodCount;
        }
        // If the tree is active and the person is touching the tree
        // (Note: the intersection is more expensive, so do that last)
        if ((rock[j].state == ResourceState::Active) && areIntersecting(rock[j].getBounds(), person.getBounds()))
        {
          // Harvest the tree
          rock[j].harvest();
          ++stoneCount;
        }
        // If the tree is active and the person is touching the tree
        // (Note: the intersection is more expensive, so do that last)
        if ((appleTree[j].state == ResourceState::Active) && areIntersecting(appleTree[j].getBounds(), person.getBounds()))
        {
          // Harvest the tree
          appleTree[j].harvest();
          ++foodCount;
        }
      }
    }
  }
  if (distance > 1 || distance < 1)
  {
    //separate
    if (deerTimer.getElapsedTime() >= 500)
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
            ++foodCount;
            --debug.deerCount;
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
  {
    if (tree[i].state == ResourceState::Inactive)
    {
      if (tree[i].regenerationDelay > 0)
        --tree[i].regenerationDelay;
      else
        tree[i].state = ResourceState::Active;
    }
    if (rock[i].state == ResourceState::Inactive)
    {
      if (rock[i].regenerationDelay > 0)
        --rock[i].regenerationDelay;
      else
        rock[i].state = ResourceState::Active;
    }
    if (appleTree[i].state == ResourceState::Inactive)
    {
      if (appleTree[i].regenerationDelay > 0)
        --appleTree[i].regenerationDelay;
      else
        appleTree[i].state = ResourceState::Active;
    }
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

    appleTree[i].state = ResourceState::Active;
    appleTree[i].position.x = (random() % 250);
    appleTree[i].position.y = (random() % 250);
  }
}

//
// Resource Drawing
//

void drawResources()
{
  for (uint8_t i = 0; i < maxResources; ++i)
  {
    const Point2 treeLocalPosition = camera.toLocal(tree[i].position);
    const uint8_t treeFrame = (tree[i].state == ResourceState::Inactive) ? 1 : 0;
    Sprites::drawSelfMasked(treeLocalPosition.x, treeLocalPosition.y, treeSprite, treeFrame);

    const Point2 rockLocalPosition = camera.toLocal(rock[i].position);
    const uint8_t rockFrame = (rock[i].state == ResourceState::Inactive) ? 1 : 0;
    Sprites::drawSelfMasked(rockLocalPosition.x, rockLocalPosition.y, rockSprite, rockFrame);

    const Point2 appleTreeLocalPosition = camera.toLocal(appleTree[i].position);
    const uint8_t appleTreeFrame = (appleTree[i].state == ResourceState::Inactive) ? 1 : 0;
    Sprites::drawSelfMasked(appleTreeLocalPosition.x, appleTreeLocalPosition.y, appleTreeSprite, appleTreeFrame);
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
      regenerateDeer(deer);
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
      deer.position.x += 2;
      break;
    case 1:
      deerSprite = deerSpriteRight;
      deer.position.x -= 2;
      break;
    case 2:
      deer.position.y += 2;
      break;
    case 3:
      deer.position.y -= 2;
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
  float totalDistance = 0;
  for (uint8_t j = 0; j < maxPeople; ++j)
  {
    const Vector2F between = vectorBetween(deer.position, people[j].position);
    const float distance = between.getMagnitude();
    if (distance < 30 )
    {
      totalDistance += distance;
      // Manually normalise the vector
      const Vector2F direction = { between.x / totalDistance, between.y / totalDistance };
      deer.position = { (deer.position.x - (direction.x * deer.movementSpeed)), (deer.position.y - (direction.y * deer.movementSpeed)) };
    }
    else
      deer.state = DeerState::Idle;
  }
}

void updateDeerAnimation()
{
  if (deerFrame < 3)
    ++deerFrame;
  else
    deerFrame = 0;
}

void regenerateDeer(Deer & deer)
{
  if (deer.state == DeerState::Dead)
  {
    if (deerRegenTimer.getElapsedTime() >= 1000)
    {
      if (deer.regenerationDelay > 0)
        --deer.regenerationDelay;
      else
        populateDeer(deer);
      deerRegenTimer.updatePreviousTime();
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
  ++debug.deerCount;
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
  {
    if (deer[i].state == DeerState::Idle)
    {
      drawDeer(deer[i]);
    }
  }
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

//void addPersonAtBuilding()
//{
//  //if building's currentCapacity is less than its maxCapacity
//  //if currentMeatCount minus previous count is greater than requiredMeat
//  for (uint8_t i = 0; i < buildingCount; ++i)
//  {
//    if (buildings[i].currentCapacity < buildingMaxCapacity)
//    {
//      // Person goes on the end of the list
//      if (foodCount >= requiredFood)
//      {
//        if (personCount < maxPeople)
//        {
//          Point2 doorway = buildings[i].getDoorway(buildings[i].position);
//          people[personCount].position = { doorway.x, doorway.y };
//          ++personCount;
//          ++buildings[i].currentCapacity;
//        }
//      }
//    }
//  }
//}
//
// Debugging
//

void drawDebugInfo()
{
  //arduboy.setCursor(0, 0);
  //arduboy.print(selectionRectangle.x);

  arduboy.setCursor(20, 0);
  arduboy.print(debug.deerCount);

  //arduboy.setCursor(0, 20);
  //arduboy.print();

  //arduboy.setCursor(20, 20);
  //arduboy.print();

  //arduboy.setCursor(0, 40);
  //arduboy.print(debug);

  //arduboy.setCursor(20, 40);
  //arduboy.print(foodCount);
}
