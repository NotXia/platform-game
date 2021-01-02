#pragma once
#include "Pixel.hpp"
#include "settings.h"

class TownGraphics {
	protected:
		//Pixel terrain[GAME_WIDTH][GAME_HEIGHT];
		void createHouse1(Pixel terrain[GAME_WIDTH][GAME_HEIGHT], int& start_x, int start_y);


	public:
		TownGraphics(Pixel terrain[GAME_WIDTH][GAME_HEIGHT], int start_x, int start_y);

};

