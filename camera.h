struct PointPos{
  uint16_t x;
  uint16_t y;
};

PointPos camera;

void cameraInput(){
  if(arduboy.pressed(UP_BUTTON) && playerCursor.y <= 4) {
    camera.y -= 1;
  }
  if(arduboy.pressed(DOWN_BUTTON) && playerCursor.y >= 60) {
    camera.y += 1;
  }
  if(arduboy.pressed(LEFT_BUTTON) && playerCursor.x <= 4) {
    camera.x -= 1;
  }
  if(arduboy.pressed(RIGHT_BUTTON) && playerCursor.x >= 124) {
    camera.x += 1;
  }
}
