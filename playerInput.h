void cameraInput() {
  if (arduboy.pressed(UP_BUTTON) && playerCursor.localPosition.y <= 4) {
    camera.y -= 1;
  }
  if (arduboy.pressed(DOWN_BUTTON) && playerCursor.localPosition.y >= 60) {
    camera.y += 1;
  }
  if (arduboy.pressed(LEFT_BUTTON) && playerCursor.localPosition.x <= 4) {
    camera.x -= 1;
  }
  if (arduboy.pressed(RIGHT_BUTTON) && playerCursor.localPosition.x >= 124) {
    camera.x += 1;
  }
}

void moveCursor()
{
  if (arduboy.pressed(UP_BUTTON)) {
    if (playerCursor.localPosition.y >= 5) {
      --playerCursor.localPosition.y;
    }
    --playerCursor.globalPosition.y;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (playerCursor.localPosition.y <= 59) {
      ++playerCursor.localPosition.y;
    }
    ++playerCursor.globalPosition.y;

  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (playerCursor.localPosition.x >= 5) {
      --playerCursor.localPosition.x;
    }
    --playerCursor.globalPosition.x;

  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (playerCursor.localPosition.x <= 123) {
      ++playerCursor.localPosition.x;
    }
    ++playerCursor.globalPosition.x;

  }
}

void handleCursorInput()
{
  if (arduboy.justPressed(A_BUTTON))
  {
    clickPoint = { playerCursor.localPosition.x, playerCursor.localPosition.y };
    rectangle = createRectangleFromPoints(playerCursor.localPosition.x, playerCursor.localPosition.y, (rectangle.x + rectangle.width), (rectangle.y + rectangle.height));
  }

  if (arduboy.pressed(A_BUTTON))
  {
    rectangle = createRectangleFromPoints(clickPoint.x, clickPoint.y, playerCursor.localPosition.x, playerCursor.localPosition.y);
  }
  if (!arduboy.pressed(A_BUTTON)) {
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.height = 0;
    rectangle.width = 0;
  }
  moveCursor();
}

void unitMove()
{
  if (arduboy.justPressed(B_BUTTON))
  {
    for (uint8_t i = 0; i < personMax; ++i) {
      if (people[i].state == PersonState::Selected)
        people[i].state = PersonState::Moving;
      playerCursor.lastGlobalPosition = playerCursor.globalPosition;
    }
  }
  moveSelectedPeople();
}

void deselectAll() {
  if (arduboy.pressed(B_BUTTON) && arduboy.pressed(A_BUTTON)) {
    for (uint8_t i = 0; i < personMax; i++) {
      people[i].state = PersonState::Idle;
      personSelect = 0;
      playerCursor.lastGlobalPosition.x = 0;
      playerCursor.lastGlobalPosition.y = 0;
    }
  }
}

void addBuildingAt(PointF point) {
  if (buildingCount < buildingMax) {
    ++buildingCount;
  }
  for (uint8_t i = 0; i < buildingCount; ++i) {
    house[i].position.x = (static_cast<uint16_t>(point.x));
    house[i].position.y = (static_cast<uint16_t>(point.y));
    house[i].position = {point.x, point.y};
  }
}

void addBuildingAtCursor() {
  if (arduboy.justPressed(B_BUTTON) && !arduboy.pressed(A_BUTTON)) {
    if (resourceCounter >= 1 && personSelect == 0) {
      addBuildingAt(playerCursor.globalPosition);
    }
  }
}

void drawBuilding() {
  for (uint8_t i = 0; i < buildingCount; ++i) {
    const Point localPosition = toLocal(house[i].position);
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, houseSprite, 0);
  }
}

void addPersonAt(int16_t x, int16_t y) {
  // Avoid trying to add more than the maximum
  if (personCount < personMax)
  {
    // Person goes on the end of the list
    people[personCount].position = {x, y};
    ++personCount;
  }
}

/*void addPersonAtCursor() {
  if (arduboy.justPressed(B_BUTTON)) {
    addPersonAt(playerCursor.localPosition.x + camera.x, playerCursor.localPosition.y + camera.y);
  }
  }*/
