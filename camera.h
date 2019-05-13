#include "Cursor.h"

struct PointPos{
  uint16_t x;
  uint16_t y;
};

PointPos camera;

void cameraInput(){
  if(arduboy.pressed(UP_BUTTON) && playerCursor.position.y <= 4) {
    camera.y -= 1;
  }
  if(arduboy.pressed(DOWN_BUTTON) && playerCursor.position.y >= 60) {
    camera.y += 1;
  }
  if(arduboy.pressed(LEFT_BUTTON) && playerCursor.position.x <= 4) {
    camera.x -= 1;
  }
  if(arduboy.pressed(RIGHT_BUTTON) && playerCursor.position.x >= 124) {
    camera.x += 1;
  }
}

Point toLocal(Point globalPoint)
{
  return { globalPoint.x - camera.x, globalPoint.y - camera.y };
}

Point toGlobal(Point localPoint)
{
  return { localPoint.x + camera.x, localPoint.y + camera.y };
}
