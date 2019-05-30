#pragma once

#include "Utils.h"

struct Building
{
  uint8_t currentCapacity;
  static constexpr uint8_t height = 20;
  static constexpr uint8_t halfWidth = 15;
  Point2 position;
  Point2 getDoorway(Point2 point)
  {
    point.x = position.x + halfWidth;
    point.y = position.y + height - 10;
  return { point };
  }
};
