#include "Sprites.h"
#include "camera.h"

uint16_t rockX[50];
uint16_t rockY[50];

void populateRock(){
  for (int x = 0; x < 30; x++) {
    rockX[x] = random(0, 252);
    rockY[x] = random(0, 252);
  }
}

void drawRock() {
  for (uint8_t x = 0; x < 30; x++) {
    Sprites::drawSelfMasked(rockX[x] + camera.x, rockY[x] + camera.y, rock, 0);
  }
}

uint16_t treeX[50];
uint16_t treeY[50];

void populateTree(){
  for (int x = 0; x < 30; x++) {
    treeX[x] = random(0, 252);
    treeY[x] = random(0, 252);
  }
}

void drawTree() {
  for (uint8_t x = 0; x < 30; x++) {
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
