#include "Cursor.h"

struct PointPos{
  uint16_t x;
  uint16_t y;
};

PointPos camera;

void cameraInput(){
  if(arduboy.pressed(UP_BUTTON) && playerCursor.localPosition.y <= 4) {
    camera.y -= 1;
  }
  if(arduboy.pressed(DOWN_BUTTON) && playerCursor.localPosition.y >= 60) {
    camera.y += 1;
  }
  if(arduboy.pressed(LEFT_BUTTON) && playerCursor.localPosition.x <= 4) {
    camera.x -= 1;
  }
  if(arduboy.pressed(RIGHT_BUTTON) && playerCursor.localPosition.x >= 124) {
    camera.x += 1;
  }
}

PointF toLocal(PointF globalPoint)
{
  return { globalPoint.x - camera.x, globalPoint.y - camera.y };
}

PointF toGlobal(PointF localPoint)
{
  return { localPoint.x + camera.x, localPoint.y + camera.y };
}
