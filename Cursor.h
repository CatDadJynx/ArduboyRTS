struct Cursor
{
  uint8_t x;
  uint8_t y;
  uint8_t radius;
  uint8_t colour;
};

Point clickPoint { 16, 16 };
Cursor playerCursor { 64, 32, 3, 1 };
Rect rectangle { 16, 16, 32, 32 };
Point cursorGlobalPos { 64, 32 };

Rect createRectangleFromPoints(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  const int16_t startX = min(x0, x1);
  const int16_t startY = min(y0, y1);
  const int16_t endX = max(x0, x1);
  const int16_t endY = max(y0, y1);

  return { startX, startY, static_cast<uint8_t>(endX - startX), static_cast<uint8_t>(endY - startY) };
}

void drawCursor()
{
  arduboy.fillCircle(playerCursor.x, playerCursor.y, playerCursor.radius, playerCursor.colour);
}

void drawRectangle()
{
  arduboy.drawRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
}

void moveCursor()
{
  if (arduboy.pressed(UP_BUTTON)) {
    if (playerCursor.y >= 5) {
      --playerCursor.y;
    }
    --cursorGlobalPos.y;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (playerCursor.y <= 59) {
      ++playerCursor.y;
    }
    ++cursorGlobalPos.y;
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (playerCursor.x >= 5) {
      --playerCursor.x;
    }
    --cursorGlobalPos.x;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (playerCursor.x <= 123) {
      ++playerCursor.x;
    }
    ++cursorGlobalPos.x;
  }
}

void handleCursorInput()
{
  if (arduboy.justPressed(A_BUTTON))
  {
    clickPoint = { playerCursor.x, playerCursor.y };
    rectangle = createRectangleFromPoints(playerCursor.x, playerCursor.y, (rectangle.x + rectangle.width), (rectangle.y + rectangle.height));
  }

  if (arduboy.pressed(A_BUTTON))
  {
    rectangle = createRectangleFromPoints(clickPoint.x, clickPoint.y, playerCursor.x, playerCursor.y);
  }
  if (!arduboy.pressed(A_BUTTON)) {
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.height = 0;
    rectangle.width = 0;
  }
  moveCursor();
}
