#include "GameSprites.h"
#include "camera.h"

constexpr uint8_t resourceMax = 30;

uint16_t rockX[resourceMax];
uint16_t rockY[resourceMax];

void populateRock(){
  for (int x = 0; x < resourceMax; x++) {
    rockX[x] = random(0, 252);
    rockY[x] = random(0, 252);
  }
}

void drawRock() {
  for (uint8_t x = 0; x < resourceMax; x++) {
    Sprites::drawSelfMasked(rockX[x] + camera.x, rockY[x] + camera.y, rock, 0);
  }
}

uint16_t treeX[resourceMax];
uint16_t treeY[resourceMax];

void populateTree(){
  for (int x = 0; x < resourceMax; x++) {
    treeX[x] = random(0, 252);
    treeY[x] = random(0, 252);
  }
}

void drawTree() {
  for (uint8_t x = 0; x < resourceMax; x++) {
    Sprites::drawSelfMasked(treeX[x] + camera.x, treeY[x] + camera.y, tree, 0);
  }
}

void populateResources(){
    populateRock();
    populateTree();
}

void resourceDraw(){
  drawRock();
  drawTree();
}
