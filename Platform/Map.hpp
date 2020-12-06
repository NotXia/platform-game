#pragma once
#include "Pixel.hpp"
#include "settings.h"
#include <iostream>

class Map {
	protected:
		Pixel terrain[GAME_WIDTH][GAME_HEIGHT];
		Map *next;
		Map *prev;

		void generateTerrain();

	public:
		Map(Map *prev=NULL);

		Map* getPrev();
		Map *getNext();

		void getTerrain(Pixel out[][GAME_HEIGHT]);
};

