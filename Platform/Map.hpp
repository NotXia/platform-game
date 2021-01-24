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
#include "LavaList.hpp"

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
		LavaList lavaList;


		/*
			Inizializza la matrice terrain con gli elementi base di un livello erboso.
		*/
		void generateTerrainGrass();

		/*
			Inizializza la matrice terrain con gli elementi base di un livello roccioso.
		*/
		void generateTerrainRock();

		/*
			Inizializza la matrice terrain con le piattaforme.
		*/
		void generatePlatforms();

		/*
			Prende in input un intero che rappresenta il numero massimo di nemici generabili.
			Genera e inizializza i nemici e li inserisce in enemyList.
		*/
		void generateEnemies(int max_enemies);

		/*
			Prende in input un intero che rappresenta il numero massimo di bonus generabili.
			Genera e inizializza i bonus e li inserisce in bonusList.
		*/
		void generateBonuses(int max_bonus);

		/*
			Inizializza la matrice terrain con gli elementi di un villaggio.
		*/
		void generateTown();

		/*
			Inserisce nella matrice terrain la lava.
			Imposta lavaList.
		*/
		void generateLava();

		/*
			Imposta la matrice terrain per le boss fight
		*/
		void generateMapBossType1();
		void generateMapBossType2();
		void generateMapBossType3();
	public:
		Map(Map *prev=NULL, int level_number=0);

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
			Restituisce la difficoltà corrente, calcolata in base al numero di livelli superati.
		*/
		int getDifficulty();

		/*
			Restituisce true se *prev è NULL, false altrimenti.
		*/
		bool prevNull();

		/*
			Prende in input la posizione da cui il giocatore è uscito dal livello corrente.
			Restituisce il puntatore al livello precedente, impostando right_position con il parametro in input.
		*/
		Map *gotoPrevious(Position exit_position);

		/*
			Prende in input la posizione da cui il giocatore è uscito dal livello corrente.
			Restituisce il puntatore al livello successivo, impostando left_position con il parametro in input.
		*/
		Map *gotoNext(Position exit_position);

		/*
			Prende in input una posizione.
			Restituisce true se quella posizione è solida, false altrimenti.
			Valuta considerando il terreno, i nemici e il boss.
		*/
		bool isSolidAt(Position position);

		/*
			Prende in input una posizione.
			Restituisce quella posizione della mappa, ignorando la lista di nemici.
		*/
		Pixel getTerrainAt(Position position);

		/*
			Prende in input un oggetto Bullet.
			Inserisce il parametro nella lista BulletList.
		*/
		void addBullet(Bullet bullet);

		/*
			Prende in input un oggetto Bonus.
			Inserisce il parametro nella lista BonusList e restituisce la posizione in cui è stato inserito.
			Se c'è già un bonus in tale posizione, il nuovo bonus viene spostato nella prima posizione disponibile nelle vicinanze.
		*/
		Position addBonus(Bonus bonus);

		/*
			Restituisce true boss non è NULL.
		*/
		bool isBossFight();

		/*
			Imposta la matrice terrain in modo da avere l'entrata e l'uscita del livello bloccati.
		*/
		void place_wall();

		/*
			Imposta la matrice terrain alla mappa basilare
		*/
		void endBossFight();

		/*
			Prende in input un oggetto Position.
			Restituisce true se c'è della lava in quella posizione
		*/
		bool isLava(Position position);

};

