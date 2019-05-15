struct VectorF
{
  float x;
  float y;
};

// This is provided for avoiding the expensive sqrt needed to get the actual magnitude
float magnitudeSquared(VectorF vector)
{
  return ((vector.x * vector.x) + (vector.y * vector.y));
}

float magnitude(VectorF vector)
{
  return sqrt(magnitudeSquared(vector));
}

VectorF normalise(VectorF vector)
{
  const float length = magnitude(vector);
  return { vector.x / length, vector.y / length };
}
