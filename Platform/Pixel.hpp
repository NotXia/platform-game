#pragma once
#include "settings.h"

class Pixel {
	protected:
		char value;
		int color_fg;
		int color_bg;
		bool solid;

	public:
		Pixel(char value=' ', int color_foreground=FG_GREY, int color_background=BACKGROUND_DEFAULT, bool solid=false);

		char getValue();
		int getColor();
		bool isSolid();

		void setBackgroundColor(int color);
		int getBackgroundColor();
};

