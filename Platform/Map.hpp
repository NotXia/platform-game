#pragma once
#include "Pixel.hpp"
#include "settings.h"
#include "Position.hpp"
#include <iostream>

class Map {
	protected:
		Pixel terrain[GAME_WIDTH][GAME_HEIGHT];
		Map *next;
		Map *prev;
		Position left_position;
		Position right_position;

		/*
			Inizializza la matrice terrain con gli elementi della mappa
		*/
		void generateTerrain();

	public:
		Map(Map *prev=NULL);

		void getTerrain(Pixel out[][GAME_HEIGHT]);
		Position getLeftPosition();
		Position getRightPosition();
		
		Map* getPrev();
		Map* getNext();
};

