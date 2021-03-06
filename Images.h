#pragma once

#include <avr/pgmspace.h>

const unsigned char rockSprite[] PROGMEM
{
	// Width: 16 Height: 16
	16, 16,

	0xF0, 0xFE, 0x2E, 0x63, 0x43, 0x63, 0xA1, 0xA1, 0x03, 0x0F, 0x01, 0x21, 0x23, 0xCE, 0x58, 0xF0,
	0x1F, 0x3F, 0xF0, 0xC0, 0xA0, 0x90, 0x98, 0x91, 0x90, 0x80, 0x80, 0x88, 0xB1, 0xC0, 0x78, 0x0F,
};

const unsigned char treeSprite[] PROGMEM
{
	// Width: 16 Height: 16
	16, 16,

	0x00, 0x00, 0x80, 0xE0, 0x70, 0xBC, 0x56, 0xA9, 0x56, 0xBC, 0x78, 0xE0, 0xC0, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x17, 0x0A, 0x15, 0xEA, 0xF5, 0x8A, 0x95, 0x8A, 0xF5, 0x0A, 0x15, 0x0B, 0x1E, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE0, 0x98, 0x8C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,

};

const unsigned char personSprite[] PROGMEM
{
	// Width: 5 Height: 8
	5, 8,

	0x04, 0x10, 0x07, 0x10, 0x04,

	0x04, 0x10, 0x07, 0x08, 0x04,

	0x04, 0x08, 0x07, 0x10, 0x04,
};

const unsigned char houseSprite[] PROGMEM
{
	// Width: 15 Height: 16
	30, 20,

	0xFF, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0xFF,
	0xFF, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0xF9, 0x8D, 0x05, 0x05, 0x05, 0x0D, 0xF9, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0xFF,
	0x0F, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0F,
};

const unsigned char deerSpriteLeft[] PROGMEM
{
// Bitmap Image. No transparency
// Width: 7 Height: 8
7, 8, 
0x85, 0x1E, 0x12, 0x11, 0x10, 0x10, 0x88,

0x45, 0x1E, 0x12, 0x11, 0x10, 0x10, 0x88, 

0x85, 0x1E, 0x12, 0x11, 0x10, 0x10, 0x88,

0x85, 0x1E, 0x12, 0x11, 0x10, 0x10, 0x48, 

};

const unsigned char deerSpriteRight[] PROGMEM
{
// Bitmap Image. No transparency
// Width: 7 Height: 8
7, 8, 
0x88, 0x10, 0x10, 0x11, 0x12, 0x1E, 0x85,

0x88, 0x10, 0x10, 0x11, 0x12, 0x1E, 0x45, 

0x88, 0x10, 0x10, 0x11, 0x12, 0x1E, 0x85,

0x88, 0x10, 0x10, 0x11, 0x12, 0x1E, 0x45, 

};
