#pragma once

// For uint8_t
#include <stdint.h>

// Represents a set of axial directions.
// Implemented as a bit set, to be manipulated with bitwise operators
enum class Directions : uint8_t
{
	None = 0,
	PositiveX = (1 << 0),
	PositiveY = (1 << 1),
	NegativeX = (1 << 2),
	NegativeY = (1 << 3),
};

// Invert the set (included values become unincluded and vice versa)
constexpr Directions operator ~(Directions directions)
{
	return static_cast<Directions>(~static_cast<uint8_t>(directions) & 0x0F);
}

// Combine directions
constexpr Directions operator |(Directions left, Directions right)
{
	return static_cast<Directions>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

Directions & operator |=(Directions & left, Directions right)
{
	left = (left | right);
	return left;
}

// Isolate directions
constexpr Directions operator &(Directions left, Directions right)
{
	return static_cast<Directions>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

Directions & operator &=(Directions & left, Directions right)
{
	left = (left & right);
	return left;
}

// Toggle/invert directions
constexpr Directions operator ^(Directions left, Directions right)
{
	return static_cast<Directions>(static_cast<uint8_t>(left) ^ static_cast<uint8_t>(right));
}

Directions & operator ^=(Directions & left, Directions right)
{
	left = (left ^ right);
	return left;
}