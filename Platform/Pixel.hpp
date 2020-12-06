#pragma once

class Pixel {
	protected:
		char value;
		int color;
		bool solid;

	public:
		Pixel(char value=' ', int color=7, bool solid=false); // color=7 è il default del cmd (carattere bianco su sfondo nero)

		char getValue();
		int getColor();
		bool isSolid();
};

