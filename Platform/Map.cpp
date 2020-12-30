#include "Map.hpp"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "colors.h"
#include "WeaponContainer.hpp"
#include <cmath>

const int TERRAIN_HEIGHT = 3;
const int PLATFORM_MAX_LENGTH = 25;
const int PLATFORM_MIN_LENGTH = 5;
const int EMPTYZONE_LENGTH = 5;

const Pixel PLATFORM_TEXTURE = Pixel(char(219), PLATFORM_COLOR, true);
const Pixel TERRAIN_TEXTURE = Pixel(' ', TERRAIN_COLOR, true);
const Pixel SKY_TEXTURE = Pixel(' ', BACKGROUND_DEFAULT, false);

int platform_chance = 20;

Map::Map(Map *prev, int max_enemies, int level_number) {
	generateTerrain();
	next = NULL;
	this->prev = prev;
	this->level_number = level_number;

	left_position = Position(1, GAME_HEIGHT-TERRAIN_HEIGHT);
	right_position = Position(GAME_WIDTH, GAME_HEIGHT-TERRAIN_HEIGHT);

	enemyList = EnemyList();
	generateEnemy(max_enemies, getDifficulty());
	
	bonusList = BonusList();
	generateBonus(rand() % 4 + 1, getDifficulty());

	bulletList = BulletList();
}

/*
	Restituisce la difficoltà corrente, calcolata in base al numero di livelli superati
*/
int Map::getDifficulty() {
	return int(level_number/DIFFICULTY_INCREASE_RATE)+1;
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

/*
	Prende in input due interi, rispettivamente il numero massimo di nemici generabili e la difficoltà.
	Genera e inizializza i nemici e li inserisce in enemyList
*/
void Map::generateEnemy(int max_enemies, int difficulty) {
	if (max_enemies != 0) {
		int base_chance = 2000 + 500*max_enemies;
		int chance = base_chance;
		int generate;
		WeaponContainer *weapon_container = new WeaponContainer();
		weapon_container->initForEnemy();
		Weapon weapon;

		// La generazione inizia dall'angolo in basso a destra, scorrendo orizzontalmente l'area di gioco fino a raggiungere l'angolo in alto a sinistra
		for (int i=GAME_HEIGHT-TERRAIN_HEIGHT; i>0; i--) {
			base_chance -= 200 * max_enemies;
			if (base_chance <= 0) { base_chance = 1000; }

			for (int j=GAME_WIDTH-(EMPTYZONE_LENGTH*2); j>(EMPTYZONE_LENGTH*2); j--) {
				if (terrain[j][i].isSolid() && !terrain[j][i-1].isSolid() && !terrain[j][i-2].isSolid()) {
					generate = rand() % chance;
					if (generate == 0 && max_enemies > 0) {
						int hp = rand() % (difficulty*2 - difficulty + 1) + difficulty;
						int points = ENEMY_BASE_POINTS * difficulty;
						int money = rand() % difficulty + 1;

						int tier1_chance = 100;
						int tier2_chance = (getDifficulty()-1) * 25;
						int tier3_chance = (getDifficulty()-1) * 11;
						
						int generate = rand() % (tier1_chance+tier2_chance+tier3_chance);
						if (generate < tier1_chance) {
							weapon = weapon_container->getRandomTier1();
						}
						else if (generate >= tier1_chance && generate < tier2_chance) {
							weapon = weapon_container->getRandomTier2();
						}
						else {
							weapon = weapon_container->getRandomTier3();
						}

						Enemy new_enemy = Enemy(hp, points, money, Pixel('<', ENEMY_HEAD_COLOR, true), Pixel('>', ENEMY_HEAD_COLOR, true), Pixel(char(219), ENEMY_BODY_COLOR, true), Position(j+1, i), weapon);
						enemyList.add(new_enemy);
						max_enemies--;
						chance = base_chance;
					}
					else {
						chance -= rand() % ((100*max_enemies) + 1);
						if (chance <= 0) {
							chance = 1;
						}
					}
				}
			}
		}

		delete weapon_container;
	}
}

/*
	Prende in input due interi, rispettivamente il numero massimo di bonus generabili e la difficoltà.
	Genera e inizializza i bonus e li inserisce in bonusList
*/
void Map::generateBonus(int max_bonus, int difficulty) {
	if (max_bonus != 0) {
		int base_chance = 1000 + 500*max_bonus;
		int chance = base_chance;
		int generate;

		// La generazione inizia dall'angolo in alto a destra, scorrendo orizzontalmente l'area di gioco fino a raggiungere l'angolo in basso a sinistra
		for (int i=2; i<GAME_HEIGHT-TERRAIN_HEIGHT; i++) {
			base_chance -= 100 * max_bonus;
			if (base_chance <= 0) { base_chance = 1000; }

			for (int j=GAME_WIDTH-(EMPTYZONE_LENGTH*2); j>(EMPTYZONE_LENGTH*2); j--) {
				if (terrain[j][i].isSolid() && !terrain[j][i-1].isSolid() && !terrain[j][i-2].isSolid()) {
					generate = rand() % chance;

					if (generate == 0 && max_bonus > 0) {
						int type = rand() % 100;
						if (type < 40) { // Bonus soldi
							bonusList.insert(Bonus(Pixel(MONEY_SYMBOL, FG_DARKYELLOW | BACKGROUND_DEFAULT, false), Position(j+1, i), ENEMY_BASE_POINTS * difficulty, rand() % difficulty + 1, 0));
						}
						else if (type < 80) { // Bonus hp
							bonusList.insert(Bonus(Pixel(char(3), FG_DARKRED | BACKGROUND_DEFAULT, false), Position(j+1, i), ENEMY_BASE_POINTS * difficulty, 0, ceil((1/(log10(getDifficulty())+1))*5)));
						}
						else { // Bonus arma
							WeaponContainer *weaponcontainer = new WeaponContainer();
							weaponcontainer->initForPlayer();
							Weapon *weapon = new Weapon();
							int tier1_chance = 100;
							int tier2_chance = (getDifficulty()-1) * 25;
							int tier3_chance = (getDifficulty()-1) * 11;

							int generate = rand() % (tier1_chance+tier2_chance+tier3_chance);
							if (generate < tier1_chance) {
								*weapon = weaponcontainer->getRandomTier1();
							}
							else if (generate >= tier1_chance && generate < tier2_chance) {
								*weapon = weaponcontainer->getRandomTier2();
							}
							else {
								*weapon = weaponcontainer->getRandomTier3();
							}

							delete weaponcontainer;

							bonusList.insert(Bonus(Pixel(char(170), FG_BLACK | BACKGROUND_DEFAULT, false), Position(j+1, i), 0, 0, 0, weapon));
						}
						
						max_bonus--;
						chance = base_chance;
					}
					else {
						chance -= rand() % ((100*max_bonus) + 1);
						if (chance <= 0) {
							chance = 1;
						}
					}
				}
			}
		}
	}
}

Position Map::getLeftPosition() {
	return this->left_position;
}
Position Map::getRightPosition() {
	return this->right_position;
}

EnemyList Map::getEnemyList() {
	return enemyList;
}

BulletList Map::getBulletList() {
	return bulletList;
}

void Map::setEnemyList(EnemyList enemylist) {
	enemylist.initIter();
	this->enemyList = enemylist;
}

void Map::setBulletList(BulletList bulletlist) {
	bulletlist.initIter();
	this->bulletList = bulletlist;
}

BonusList Map::getBonusList() {
	return bonusList;
}

void Map::setBonusList(BonusList bonuslist) {
	bonuslist.initIter();
	this->bonusList = bonuslist;
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
		this->next = new Map(this, max_enemy, level_number+1);
	}
	this->next->left_position.setY(enter_position.getY());
	return this->next;
}

/*
	Prende in input una posizione.
	Restituisce true se quella posizione è solida, false altrimenti
*/
bool Map::isSolidAt(Position position) {
	bool out = false;

	if (enemyList.existsAt(position)) {
		out = true;
	}
	else if(terrain[position.getX()-1][position.getY()-1].isSolid()) {
		out = true;
	}
	return out;
}

/*
	Prende in input una posizione.
	Restituisce quella posizione della mappa, ignorando la lista di nemici
*/
Pixel Map::getTerrainAt(Position position) {
	Pixel out;
	out = terrain[position.getX()-1][position.getY()-1];
	return out;
}

/*
	Prende in input un oggetto Bullet.
	Inserisce il parametro nella lista BulletList
*/
void Map::addBullet(Bullet bullet) {
	bulletList.insert(bullet);
}

/*
	Prende in input un oggetto Bonus.
	Inserisce il parametro nella lista BonusList
*/
Position Map::addBonus(Bonus bonus) {
	Position spawn_position = bonus.getBodyPosition();

	while (bonusList.pointAt(spawn_position) && spawn_position.getX() < GAME_WIDTH) {
		spawn_position.setX(spawn_position.getX()+1);
	}

	if (spawn_position.getX() >= GAME_WIDTH) {
		spawn_position = bonus.getBodyPosition();

		while (bonusList.pointAt(spawn_position) && spawn_position.getX() > 1) {
			spawn_position.setX(spawn_position.getX()-1);
		}
	}

	bonus.setPosition(spawn_position);
	bonusList.insert(bonus);

	return spawn_position;
}
