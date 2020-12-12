#include "Map.hpp"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "colors.h"

const int TERRAIN_HEIGHT = 3;
const int PLATFORM_MAX_LENGTH = 25;
const int PLATFORM_MIN_LENGTH = 5;
const int EMPTYZONE_LENGTH = 5;

const Pixel PLATFORM_TEXTURE = Pixel(char(219), PLATFORM_COLOR, true);
const Pixel TERRAIN_TEXTURE = Pixel(' ', TERRAIN_COLOR, true);
const Pixel SKY_TEXTURE = Pixel(' ', BACKGROUND_DEFAULT, false);

int platform_chance = 20;

Map::Map(Map *prev) {
	generateTerrain();
	next = NULL;
	this->prev = prev;

	left_position = Position(1, GAME_HEIGHT-TERRAIN_HEIGHT);
	right_position = Position(GAME_WIDTH, GAME_HEIGHT-TERRAIN_HEIGHT);

	enemyList = NULL;
}

/*
	Inizializza la matrice terrain con gli elementi base della mappa
*/
void Map::generateTerrain() {
	// Generazione "pavimento"
	for (int i=0; i<TERRAIN_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = TERRAIN_TEXTURE;
		}
	}

	/* 
		Generazione area di gioco
		- L'area iniziale e finale rimangono libere da piattaforme
		- L'area centrale prevede la generazione di piattaforme
	*/
	for (int height=GAME_HEIGHT-TERRAIN_HEIGHT-1; height>=0; height--) {
		int width = 0;

		// Area iniziale a sinistra (non genera piattaforme)
		for (width; width<EMPTYZONE_LENGTH; width++) {
			terrain[width][height] = SKY_TEXTURE;
		}

		// Area centrale con possibilità di generare piattaforme
		for (width; width<GAME_WIDTH-EMPTYZONE_LENGTH; width++) {
			bool generate_platform = (rand() % platform_chance) == 0;

			/* 
				La piattaforma viene generata se:
				- La variabile generate_platform è true
				- La distanza dal soffitto è di almeno 3 blocchi
				- I due blocchi immediatamento sotto non sono solidi, mentre il terzo lo è (permette al giocatore di raggiungerlo)
			*/
			if (generate_platform && height >= 3 && (!terrain[width][height+1].isSolid() && !terrain[width][height+2].isSolid() && terrain[width][height+3].isSolid())) {
				int platform_size = rand() % (PLATFORM_MAX_LENGTH-PLATFORM_MIN_LENGTH+1) + PLATFORM_MIN_LENGTH;

				if (platform_size > GAME_WIDTH-width-EMPTYZONE_LENGTH) { // Se la dimensione della piattaforma supera l'area "neutrale" a destra, annulla la generazione
					terrain[width][height] = SKY_TEXTURE;
				}
				else {
					for (int k=0; k<platform_size; k++) {
						terrain[width][height] = PLATFORM_TEXTURE;
						width++;
					}
					width--;
				}
			}
			else {
				terrain[width][height] = SKY_TEXTURE;
			}
		}
		
		// Area finale a destra (non genera piattaforme)
		for (width; width<GAME_WIDTH; width++) {
			terrain[width][height] = SKY_TEXTURE;
		}
	}
}

EnemyList* Map::generateEnemy(int max) {
	EnemyList *out = NULL;

	for (int i=GAME_HEIGHT-1; i>=0; i--) {
		for (int j=GAME_WIDTH-1; j>=0; j--) {
			if (terrain[j][i].isSolid()) {
				if (out == NULL) {
					/*out = new EnemyList;
					out->enemy = new Enemy()*/
				}
			}
		}
	}

	return out;
}

Position Map::getLeftPosition() {
	return this->left_position;
}
Position Map::getRightPosition() {
	return this->right_position;
}

/*
	Restituisce true se *prev è NULL, false altrimenti
*/
bool Map::prevNull() {
	return (prev == NULL);
}

/*
	Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
	Restituisce il puntatore al livello precedente, impostando right_position con il parametro in input.
*/
Map* Map::gotoPrevious(Position exit_position) {
	this->prev->right_position.setY(exit_position.getY());
	return this->prev;
}

/*
	Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
	Restituisce il puntatore al livello successivo, impostando left_position con il parametro in input.
*/
Map* Map::gotoNext(Position enter_position, int max_enemy) {
	if (this->next == NULL) {
		this->next = new Map(this);
		this->next->enemyList = generateEnemy(max_enemy);
	}
	this->next->left_position.setY(enter_position.getY());
	return this->next;
}

/*
	Prende in input una posizione.
	Restituisce quella posizione della mappa.
*/
Pixel Map::getMapAt(Position position) {
	return terrain[position.getX()-1][position.getY()-1];
}