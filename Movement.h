// Distance
int distanceSquared(int x0, int y0, int x1, int y1)
{
  const int xDistance = (x1 - x0);
  const int yDistance = (y1 - y0);
  return ((xDistance * xDistance) + (yDistance * yDistance));
}

// Vector Magnitude
float distance(int x0, int y0, int x1, int y1)
{
  return sqrt(distanceSquared(x0, y0, x1, y1));
}

Point vectorBetween(Point point0, Point point1)
{
  return { (point1.x - point0.x), (point1.y - point0.y) };
}




