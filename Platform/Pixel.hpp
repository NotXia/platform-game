#pragma once
#include "colors.h"

class Pixel {
	protected:
		char value;
		int color;
		bool solid;

	public:
		Pixel(char value=' ', int color=LIGHTGREY_BLACK, bool solid=false);

		char getValue();
		int getColor();
		bool isSolid();
};

