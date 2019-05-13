#include "GameSprites.h"
#include "camera.h"

enum class ResourceState : uint8_t
{
  Active,
  Inactive
};

struct Resource
{
  Point position;
  ResourceState state;
};

// This will take the same amount of RAM
constexpr uint8_t resourceMax = 30;

Resource rock[resourceMax];
Resource tree[resourceMax];

void populateResources(){
  for(uint8_t i = 0; i < resourceMax; i++){
    rock[i].position.x = random(0,250);
    rock[i].position.y = random(0,250);
    rock[i].state = ResourceState::Active;
    tree[i].position.x = random(0,250);
    tree[i].position.y = random(0,250);
    tree[i].state = ResourceState::Active;
  }
}

void drawRock() {
  for (uint8_t i = 0; i < resourceMax; i++) {
    Sprites::drawSelfMasked(rock[i].position.x  - camera.x, rock[i].position.y  - camera.y, rockSprite, 0);
  }
}

void drawTree() {
  for (uint8_t i = 0; i < resourceMax; i++) {
    Sprites::drawSelfMasked(tree[i].position.x  - camera.x, tree[i].position.y  - camera.y, treeSprite, 0);
  }
}


void resourceDraw(){
  drawRock();
  drawTree();
}
