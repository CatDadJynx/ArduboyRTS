#include "PointF.h"

struct Cursor
{
  PointF localPosition {64.0, 32.0};
  PointF globalPosition {64.0, 32.0};
  PointF lastGlobalPosition;
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
  arduboy.fillCircle(playerCursor.localPosition.x, playerCursor.localPosition.y, playerCursor.radius, playerCursor.colour);
}

void drawRectangle()
{
  arduboy.drawRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
}
