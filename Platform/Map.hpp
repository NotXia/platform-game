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

		/*
			Restituisce true se *prev è NULL, false altrimenti
		*/
		bool prevNull();
		
		/*
			Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
			Restituisce il puntatore al livello precedente, impostando right_position con il parametro.
		*/
		Map* gotoPrevious(Position exit_position);

		/*
			Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
			Restituisce il puntatore al livello successivo, impostando left_position con il parametro.
		*/
		Map* gotoNext(Position enter_position);
};

