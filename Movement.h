// Distance
int distanceSquared(float x0, float y0, float x1, float y1)
{
  const float xDistance = (x1 - x0);
  const float yDistance = (y1 - y0);
  return ((xDistance * xDistance) + (yDistance * yDistance));
}

// Vector Magnitude
float distance(float x0, float y0, float x1, float y1)
{
  return sqrt(distanceSquared(x0, y0, x1, y1));
}

PointF vectorBetween(PointF point0, PointF point1)
{
  return { ((point1.x - point0.x)/distance(point0.x,point0.y,point1.x,point1.y)), ((point1.y - point0.y)/distance(point0.x,point0.y,point1.x,point1.y))};
}


