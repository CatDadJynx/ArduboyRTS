#include "Vector.h"

struct PointF
{
  float x;
  float y;
};

VectorF vectorBetween(PointF point0, PointF point1)
{
  return { (point1.x - point0.x), (point1.y - point0.y) };
}

float distanceSquared(PointF point0, PointF point1)
{
  // The square distance between two points is the square magnitude of the vector between them
  return magnitudeSquared(vectorBetween(point0, point1));
}

float distance(PointF point0, PointF point1)
{
  // The distance between two points is the magnitude of the vector between them
  return magnitude(vectorBetween(point0, point1));
}

bool intersect(PointF point, Rect rect)
{
  return ((point.x >= rect.x) && (point.x < rect.x + rect.width) && (point.y >= rect.y) && (point.y < rect.y + rect.height));
}

bool intersect(Rect rect1, Rect rect2)
{
  return !(rect2.x                >= rect1.x + rect1.width  ||
           rect2.x + rect2.width  <= rect1.x                ||
           rect2.y                >= rect1.y + rect1.height ||
           rect2.y + rect2.height <= rect1.y);
}
