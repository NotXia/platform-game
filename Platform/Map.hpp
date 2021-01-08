#pragma once
#include "Pixel.hpp"
#include "settings.h"
#include "Position.hpp"
#include <iostream>
#include "EnemyList.hpp"
#include "BulletList.hpp"
#include "BonusList.hpp"
#include "NPCList.hpp"
#include "Boss.hpp"

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
		NPCList npcList;
		Boss *boss;


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
		void generateEnemies(int max_enemies);

		/*
			Prende in input due interi, rispettivamente il numero massimo di bonus generabili e la difficoltà.
			Genera e inizializza i bonus e li inserisce in bonusList
		*/
		void generateBonuses(int max_bonus);

		/*
			Inizializza la matrice terrain con gli elementi di un villaggio
		*/
		void generateTown();

		/*
			Crea un'arma per il giocatore e lo restituisce
		*/
		Weapon createPlayerWeapon();

		/*
			Crea un'arma per nemici e lo restituisce
		*/
		Weapon createEnemyWeapon();

		/*
			Crea un bonus e lo restituisce
		*/
		Bonus createBonus();

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
		NPCList getNPCList();
		void setNPCList(NPCList npclist);
		Boss* getBoss();
		void setBoss(Boss *boss);

		/*
			Crea un nemico e lo restituisce
		*/
		Enemy createEnemy();

		/*
			Restituisce la difficoltà corrente, calcolata in base al numero di livelli superati
		*/
		int getDifficulty();

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
		Map *gotoNext(Position enter_position);

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

		/*
			Restituisce true boss non è NULL
		*/
		bool isBossFight();

		/*
			Imposta la matrice terrain in modo da avere l'entrata e l'uscita del livello bloccati
		*/
		void place_wall();

		/*
			Imposta la matrice terrain alla mappa basilare
		*/
		void endBossFight();

};

