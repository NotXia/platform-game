#pragma once
#include "Pixel.hpp"
#include "settings.h"
#include "Position.hpp"
#include <iostream>
#include "Enemy.hpp"
#include "EnemyList.hpp"


class Map {
	protected:
		Pixel terrain[GAME_WIDTH][GAME_HEIGHT];
		Map *next;
		Map *prev;
		Position left_position;
		Position right_position;

		EnemyList enemyList;

		/*
			Inizializza la matrice terrain con gli elementi base della mappa
		*/
		void generateTerrain();

		/*
		*/
		void generateEnemy(int max);

	public:
		Map(Map *prev=NULL);

		Position getLeftPosition();
		Position getRightPosition();
		EnemyList getEnemyList();

		/*
			Restituisce true se *prev � NULL, false altrimenti
		*/
		bool prevNull();
		
		/*
			Prende in input la posizione da cui il giocatore � uscito nel livello corrente.
			Restituisce il puntatore al livello precedente, impostando right_position con il parametro in input.
		*/
		Map* gotoPrevious(Position exit_position);

		/*
			Prende in input la posizione da cui il giocatore � uscito nel livello corrente.
			Restituisce il puntatore al livello successivo, impostando left_position con il parametro in input.
		*/
		Map* gotoNext(Position enter_position, int max_enemy);

		/*
			Prende in input una posizione.
			Restituisce quella posizione della mappa.
		*/
		Pixel getMapAt(Position position);
};
