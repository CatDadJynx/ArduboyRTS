#include "GameObj.h"

struct Cursor
{
  PointF position {64.0, 32.0};
  PointF globalPosition {64.0, 32.0};
  uint8_t radius = 3;
  uint8_t colour = 1;
};

Point clickPoint { 16, 16 };
Cursor playerCursor;
Rect rectangle { 16, 16, 32, 32 };

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
  arduboy.fillCircle(playerCursor.position.x, playerCursor.position.y, playerCursor.radius, playerCursor.colour);
}

void drawRectangle()
{
  arduboy.drawRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
}

void moveCursor()
{
  if (arduboy.pressed(UP_BUTTON)) {
    if (playerCursor.position.y >= 5) {
      --playerCursor.position.y;
    }
    --playerCursor.globalPosition.y;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (playerCursor.position.y <= 59) {
      ++playerCursor.position.y;
    }
        ++playerCursor.globalPosition.y;

  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (playerCursor.position.x >= 5) {
      --playerCursor.position.x;
    }
        --playerCursor.globalPosition.x;

  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (playerCursor.position.x <= 123) {
      ++playerCursor.position.x;
    }
        ++playerCursor.globalPosition.x;

  }
}

void handleCursorInput()
{
  if (arduboy.justPressed(A_BUTTON))
  {
    clickPoint = { playerCursor.position.x, playerCursor.position.y };
    rectangle = createRectangleFromPoints(playerCursor.position.x, playerCursor.position.y, (rectangle.x + rectangle.width), (rectangle.y + rectangle.height));
  }

  if (arduboy.pressed(A_BUTTON))
  {
    rectangle = createRectangleFromPoints(clickPoint.x, clickPoint.y, playerCursor.position.x, playerCursor.position.y);
  }
  if (!arduboy.pressed(A_BUTTON)) {
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.height = 0;
    rectangle.width = 0;
  }
  moveCursor();
}
