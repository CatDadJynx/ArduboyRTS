#include "Cursor.h"

struct PointPos{
  uint16_t x;
  uint16_t y;
};

PointPos camera;

PointF toLocal(PointF globalPoint)
{
  return { globalPoint.x - camera.x, globalPoint.y - camera.y };
}

PointF toGlobal(PointF localPoint)
{
  return { localPoint.x + camera.x, localPoint.y + camera.y };
}
