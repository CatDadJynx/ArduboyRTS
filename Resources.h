#include "GameSprites.h"
#include "camera.h"

uint8_t resourceFrame = 0;

enum class ResourceState : uint8_t
{
  Active,
  Inactive
};

struct Resource
{
  Point position;
  ResourceState state = ResourceState::Active;
  static constexpr uint8_t height = 16;
  static constexpr uint8_t width = 16;
  Rect rectangle { position.x, position.y, height, width };
};

// This will take the same amount of RAM
constexpr uint8_t resourceMax = 30;

uint16_t resourceCounter = 0;
Resource rock[resourceMax];
Resource tree[resourceMax];

void populateResources() {
  for (uint8_t i = 0; i < resourceMax; i++) {
    rock[i].position.x = random(0, 250);
    rock[i].position.y = random(0, 250);
    rock[i].state = ResourceState::Active;
    tree[i].position.x = random(0, 250);
    tree[i].position.y = random(0, 250);
    tree[i].state = ResourceState::Active;
  }
}

void drawRock() {
  for (uint8_t i = 0; i < resourceMax; i++) {
    const Point localPosition = toLocal(rock[i].position);
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, rockSprite, resourceFrame);
  }
}

void drawTree() {
  for (uint8_t i = 0; i < resourceMax; i++) {
    const Point localPosition = toLocal(tree[i].position);
    Sprites::drawSelfMasked(localPosition.x, localPosition.y, treeSprite, resourceFrame);
  }
}


void resourceDraw() {
  drawRock();
  drawTree();
}
