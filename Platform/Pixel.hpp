#pragma once
#include "colors.h"

class Pixel {
	protected:
		char value;
		int color;
		bool solid;

	public:
		Pixel(char value=' ', int color=CONSOLE_DEFAULT, bool solid=false);

		char getValue();
		int getColor();
		bool isSolid();
};

