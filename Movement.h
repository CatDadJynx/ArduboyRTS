// Distance
float distanceSquared(PointF point0, PointF point1)
{
  const float xDistance = (point1.x - point0.x);
  const float yDistance = (point1.y - point0.y);
  return ((xDistance * xDistance) + (yDistance * yDistance));
}

// Vector Magnitude
float distance(PointF point0, PointF point1)
{
  return sqrt(distanceSquared(point0, point1));
}

VectorF vectorBetween(PointF point0, PointF point1)
{
  return { (point1.x - point0.x), (point1.y - point0.y) };
}

VectorF normalise(VectorF vector, PointF point0, PointF point1)
{
  const float length = distance(point0, point1);
  return { vector.x / length, vector.y / length };
}
