#pragma once
#include "Pixel.hpp"
#include "settings.h"
#include "Position.hpp"
#include <iostream>
#include "EnemyList.hpp"
#include "BulletList.hpp"
#include "BonusList.hpp"

class Map {
	protected:
		Pixel terrain[GAME_WIDTH][GAME_HEIGHT];
		Map *next;
		Map *prev;
		Position left_position;
		Position right_position;
		int level_number;

		EnemyList enemyList;
		BulletList bulletList;
		BonusList bonusList;

		/*
			Restituisce la difficoltà corrente, calcolata in base al numero di livelli superati
		*/
		int getDifficulty();

		/*
			Inizializza la matrice terrain con gli elementi base della mappa
		*/
		void generateTerrain();

		/*
			Inizializza la matrice terrain con le piattaforme
		*/
		void generatePlatforms();

		/*
			Prende in input due interi, rispettivamente il numero massimo di nemici generabili e la difficoltà.
			Genera e inizializza i nemici e li inserisce in enemyList
		*/
		void generateEnemy(int max_enemies, int difficulty);

		/*
			Prende in input due interi, rispettivamente il numero massimo di bonus generabili e la difficoltà.
			Genera e inizializza i bonus e li inserisce in bonusList
		*/
		void generateBonus(int max_bonus, int difficulty);

		/*
			Inizializza la matrice terrain con gli elementi di un villaggio
		*/
		void generateTown();

	public:
		Map(Map *prev=NULL, int max_enemies=0, int level_number=0);

		Position getLeftPosition();
		Position getRightPosition();
		EnemyList getEnemyList();
		void setEnemyList(EnemyList enemylist);
		BulletList getBulletList();
		void setBulletList(BulletList bulletlist);
		BonusList getBonusList();
		void setBonusList(BonusList bonuslist);

		/*
			Restituisce true se *prev è NULL, false altrimenti
		*/
		bool prevNull();

		/*
			Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
			Restituisce il puntatore al livello precedente, impostando right_position con il parametro in input.
		*/
		Map *gotoPrevious(Position exit_position);

		/*
			Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
			Restituisce il puntatore al livello successivo, impostando left_position con il parametro in input.
		*/
		Map *gotoNext(Position enter_position, int max_enemy);

		/*
			Prende in input una posizione.
			Restituisce true se quella posizione è solida, false altrimenti
		*/
		bool isSolidAt(Position position);

		/*
			Prende in input una posizione.
			Restituisce quella posizione della mappa, ignorando la lista di nemici
		*/
		Pixel getTerrainAt(Position position);

		/*
			Prende in input un oggetto Bullet.
			Inserisce il parametro nella lista BulletList
		*/
		void addBullet(Bullet bullet);

		/*
			Prende in input un oggetto Bonus.
			Inserisce il parametro nella lista BonusList
		*/
		Position addBonus(Bonus bonus);
};

