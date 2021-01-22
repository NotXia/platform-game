#include "Map.hpp"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "colors.h"
#include <cmath>
#include "EntityGenerator.h"

const Pixel PLATFORM_TEXTURE = Pixel(PLATFORM_SYMBOL, PLATFORM_COLOR_FG, PLATFORM_COLOR_BG, true);
const Pixel TERRAIN_GRASS_TEXTURE = Pixel(GRASS_SYMBOL, GRASS_COLOR_FG, GRASS_COLOR_BG, true);
const Pixel TERRAIN_ROCK_TEXTURE = Pixel(ROCK_SYMBOL, ROCK_COLOR_FG, ROCK_COLOR_BG, true);
const Pixel SKY_TEXTURE = Pixel(SKY_SYMBOL, SKY_COLOR_FG, SKY_COLOR_BG, false);
const Pixel WALL_TEXTURE = Pixel(WALL_SYMBOL, WALL_COLOR_FG, WALL_COLOR_BG, true);
const Pixel LAVA_TEXTURE = Pixel(LAVA_SYMBOL, LAVA_COLOR_FG, LAVA_COLOR_BG, true);

const Pixel HOUSE_WALL_TEXTURE = Pixel(' ', 0, BG_LIGHTGREY, false);
const Pixel HOUSE_BORDER_TEXTURE = Pixel(' ', 0, BG_BLACK, false);
const Pixel HOUSE_DOOR_TEXTURE = Pixel(' ', 0, BG_DARKYELLOW, false);


Map::Map(Map *prev, int level_number) {
	next = NULL;
	this->prev = prev;
	left_position = Position(0, GAME_HEIGHT-TERRAIN_HEIGHT-1);
	right_position = Position(GAME_WIDTH-1, GAME_HEIGHT-TERRAIN_HEIGHT-1);

	enemyList = EnemyList();
	bonusList = BonusList();
	bulletList = BulletList();
	npcList = NPCList();
	lavaList = LavaList();
	boss = NULL;
	this->level_number = level_number;

	if (level_number == getDifficulty()*DIFFICULTY_INCREASE_RATE-1) {
		// Boss
		delete boss;
		boss = new Boss();
		*boss = createBoss(getDifficulty());
		if (boss->getType() == BOSS_SUMMONER) {
			generateMapBossType1();
			left_position.setX(left_position.getX() + 1);
		}
		else if (boss->getType() == BOSS_MAGE) {
			generateMapBossType2();
			left_position.setX(left_position.getX()+EMPTYZONE_LENGTH);
		}
		else if (boss->getType() == BOSS_MELEE) {
			generateMapBossType3();
			left_position.setX(left_position.getX() + 1);
		}
		place_wall();
	}
	else if (level_number == getDifficulty()*DIFFICULTY_INCREASE_RATE-2) {
		// Villaggio
		generateTerrainGrass();
		generateTown();
	}
	else {
		// Livello
		if (rand()%5 == 0) {
			generateTerrainRock();
			generatePlatforms();
			generateLava();
		}
		else {
			generateTerrainGrass();
			generatePlatforms();
		}
		generateEnemies(ceil(1 + log2(getDifficulty())));
		generateBonuses(rand() % 4 + 1);
	}
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
void Map::generateTerrainGrass() {
	// Generazione "pavimento"
	for (int i=0; i<TERRAIN_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = TERRAIN_GRASS_TEXTURE;
		}
	}

	// Generazione cielo
	for (int i=TERRAIN_HEIGHT; i<GAME_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = SKY_TEXTURE;
		}
	}
}

void Map::generateTerrainRock() {
	// Generazione "pavimento"
	for (int i=0; i<TERRAIN_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = TERRAIN_ROCK_TEXTURE;
		}
	}

	// Generazione cielo
	for (int i=TERRAIN_HEIGHT; i<GAME_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = SKY_TEXTURE;
		}
	}
}

/*
	Inizializza la matrice terrain con gli elementi base della mappa
*/
void Map::generatePlatforms() {
	/* 
		Generazione area di gioco
		- L'area iniziale e finale rimangono libere da piattaforme
		- L'area centrale prevede la generazione di piattaforme
	*/
	for (int height=GAME_HEIGHT-TERRAIN_HEIGHT-1; height>=0; height--) {

		// Area centrale con possibilità di generare piattaforme
		for (int width=EMPTYZONE_LENGTH; width<GAME_WIDTH-EMPTYZONE_LENGTH; width++) {
			bool generate_platform = (rand() % PLATFORM_CHANCE) == 0;

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
		}
	}
}

/*
	Prende in input due interi, rispettivamente il numero massimo di nemici generabili e la difficoltà.
	Genera e inizializza i nemici e li inserisce in enemyList
*/
void Map::generateEnemies(int max_enemies) {
	if (max_enemies != 0) {
		int base_chance = 2000 + 500*max_enemies;
		int chance = base_chance;
		int generate;

		int i = GAME_HEIGHT - TERRAIN_HEIGHT;
		int j;

		// La generazione inizia dall'angolo in basso a destra, scorrendo orizzontalmente l'area di gioco fino a raggiungere l'angolo in alto a sinistra
		while(i >= 0 && max_enemies > 0) {
			base_chance -= 200 * max_enemies;
			if (base_chance <= 0) { base_chance = 1000; }

			j = GAME_WIDTH - (EMPTYZONE_LENGTH * 2);
			while (j > (EMPTYZONE_LENGTH*2) && max_enemies > 0) {
				if (terrain[j][i].isSolid() && !lavaList.existsAt(Position(j, i)) && !terrain[j][i-1].isSolid() && !terrain[j][i-2].isSolid()) {
					generate = rand() % chance;

					if (generate == 0 && max_enemies > 0) {
						Enemy new_enemy = createEnemy(getDifficulty());
						new_enemy.setPosition(Position(j, i-1));
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
				j--;
			}
			i--;
		}
	}
}

/*
	Prende in input due interi, rispettivamente il numero massimo di bonus generabili e la difficoltà.
	Genera e inizializza i bonus e li inserisce in bonusList
*/
void Map::generateBonuses(int max_bonus) {
	if (max_bonus != 0) {
		int base_chance = 1000 + 500*max_bonus;
		int chance = base_chance;
		int generate;

		int i = 2;
		int j;

		// La generazione inizia dall'angolo in alto a destra, scorrendo orizzontalmente l'area di gioco fino a raggiungere l'angolo in basso a sinistra
		while (i<GAME_HEIGHT-TERRAIN_HEIGHT && max_bonus > 0) {
			base_chance -= 100 * max_bonus;
			if (base_chance <= 0) { base_chance = 1000; }

			j = GAME_WIDTH-(EMPTYZONE_LENGTH*2);
			while  (j>(EMPTYZONE_LENGTH*2) && max_bonus > 0) {
				if (terrain[j][i].isSolid() && !lavaList.existsAt(Position(j, i)) && !terrain[j][i-1].isSolid() && !terrain[j][i-2].isSolid()) {
					generate = rand() % chance;

					if (generate == 0 && max_bonus > 0) {
						Bonus bonus = createBonus(getDifficulty());
						bonus.setPosition(Position(j, i-1));

						bonusList.insert(bonus);
						
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
				j--;
			}
			i++;
		}
	}
}

/*
	Inizializza la matrice terrain con gli elementi di un villaggio
*/
void Map::generateTown() {
	int start_x = EMPTYZONE_LENGTH;
	int end_x = GAME_WIDTH-EMPTYZONE_LENGTH;
	int start_y = GAME_HEIGHT-TERRAIN_HEIGHT-1;


	while (end_x-start_x > 10) {
		int left_size = rand() % 4 + 1;
		int right_size = rand() % 4 + 1;
		if (left_size+right_size > end_x-start_x) {
			left_size = end_x-start_x/2;
			right_size = end_x-start_x/2;
		}
		int height = rand() % 3 + 5;

		// Muro sinistro
		for (int j=0; j<height; j++) {
			terrain[start_x][start_y-j] = HOUSE_BORDER_TEXTURE;
		}
		start_x++;

		// Prima colonna
		for (int j=0; j<height-1; j++) {
			terrain[start_x][start_y-j] = HOUSE_WALL_TEXTURE;
		}
		terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
		start_x++;

		// Larghezza a sinistra
		for (int i=0; i<left_size; i++) {
			for (int j=0; j<height-1; j++) {
				terrain[start_x][start_y-j] = HOUSE_WALL_TEXTURE;
			}
			terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
			start_x++;
		}

		// Colonna prima della porta
		for (int j=0; j<height-1; j++) {
			terrain[start_x][start_y-j] = HOUSE_WALL_TEXTURE;
		}
		terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
		start_x++;

		// Porta
		for (int i=0; i<2; i++) {
			for (int j=0; j<2; j++) {
				terrain[start_x][start_y-j] = HOUSE_DOOR_TEXTURE;
			}
			for (int j=0; j<height-3; j++) {
				terrain[start_x][start_y-2-j] = HOUSE_WALL_TEXTURE;
			}
			terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
			start_x++;
		}

		// Colonna dopo la porta
		for (int j=0; j<height-1; j++) {
			terrain[start_x][start_y-j] = HOUSE_WALL_TEXTURE;
		}
		terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
		start_x++;

		// Larghezza a destra
		for (int i=0; i<right_size; i++) {
			for (int j=0; j<height-1; j++) {
				terrain[start_x][start_y-j] = HOUSE_WALL_TEXTURE;
			}
			terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
			start_x++;
		}

		// Ultima colonna
		for (int j=0; j<height-1; j++) {
			terrain[start_x][start_y-j] = HOUSE_WALL_TEXTURE;
		}
		terrain[start_x][start_y-height+1] = HOUSE_BORDER_TEXTURE;
		start_x++;

		// Muro destro
		for (int j=0; j<height; j++) {
			terrain[start_x][start_y-j] = HOUSE_BORDER_TEXTURE;
		}
		start_x++;

		start_x += rand() % 10 + 2;
	}

	npcList.insert(NPC(1, Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false), Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
				  Pixel(char(219), NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
				  Position(rand() % (GAME_WIDTH-EMPTYZONE_LENGTH-EMPTYZONE_LENGTH) + EMPTYZONE_LENGTH , GAME_HEIGHT-TERRAIN_HEIGHT-1),
				  NPC_HOSPITAL, getDifficulty()
	));
	npcList.insert(NPC(1, Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false), Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
				  Pixel(char(219), NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
				  Position(rand() % (GAME_WIDTH-EMPTYZONE_LENGTH-EMPTYZONE_LENGTH) + EMPTYZONE_LENGTH, GAME_HEIGHT-TERRAIN_HEIGHT-1),
				  NPC_WEAPONSHOP, getDifficulty()
	));
}

/*
	Inserisce nella matrice terrain la lava.
	Imposta lavaList.
*/
void Map::generateLava() {
	int h = GAME_HEIGHT - TERRAIN_HEIGHT - 3;
	for (int i=2; i<GAME_WIDTH-2; i++) {
		if (terrain[i][h].isSolid() && 
			terrain[i-1][h].isSolid() && terrain[i-2][h].isSolid() && 
			terrain[i+1][h].isSolid() && terrain[i+2][h].isSolid()) {
			for (int j=0; j<TERRAIN_HEIGHT; j++) {
				terrain[i][GAME_HEIGHT-j-1] = LAVA_TEXTURE;
				lavaList.insert(Position(i, GAME_HEIGHT-j-1));
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
void Map::setEnemyList(EnemyList enemylist) {
	enemylist.initIter();
	this->enemyList = enemylist;
}

BulletList Map::getBulletList() {
	return bulletList;
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

NPCList Map::getNPCList() {
	return npcList;
}
void Map::setNPCList(NPCList npclist) {
	this->npcList = npclist;
}

Boss *Map::getBoss() {
	return boss;
}
void Map::setBoss(Boss *boss) {
	this->boss = boss;
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
	this->prev->right_position = Position(GAME_WIDTH-1, exit_position.getY());
	return this->prev;
}

/*
	Prende in input la posizione da cui il giocatore è uscito nel livello corrente.
	Restituisce il puntatore al livello successivo, impostando left_position con il parametro in input.
*/
Map* Map::gotoNext(Position enter_position) {
	if (this->next == NULL) {
		this->next = new Map(this, level_number+1);
	}
	if (!this->next->isBossFight()) {
		this->next->left_position = Position(0, enter_position.getY());
	}
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
	else if(terrain[position.getX()][position.getY()].isSolid()) {
		out = true;
	}
	else if (isBossFight()) {
		if (boss->existsAt(position)) {
			out = true;
		}
	}
	return out;
}

/*
	Prende in input una posizione.
	Restituisce quella posizione della mappa, ignorando la lista di nemici
*/
Pixel Map::getTerrainAt(Position position) {
	Pixel out;
	out = terrain[position.getX()][position.getY()];
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

	while (bonusList.pointAt(spawn_position) && spawn_position.getX() < GAME_WIDTH-1) {
		spawn_position.setX(spawn_position.getX()+1);
	}

	if (spawn_position.getX() >= GAME_WIDTH-1) {
		spawn_position = bonus.getBodyPosition();

		while (bonusList.pointAt(spawn_position) && spawn_position.getX() > 0) {
			spawn_position.setX(spawn_position.getX()-1);
		}
	}

	bonus.setPosition(spawn_position);
	bonusList.insert(bonus);

	return spawn_position;
}

/*
	Restituisce true boss non è NULL
*/
bool Map::isBossFight() {
	return boss != NULL;
}

/*
	Imposta la matrice terrain in modo da avere l'entrata e l'uscita del livello bloccati
*/
void Map::place_wall() {
	for (int i=0; i<GAME_HEIGHT; i++) {
		terrain[0][i] = WALL_TEXTURE;
		terrain[GAME_WIDTH-1][i] = WALL_TEXTURE;
	}
}

/*
	Imposta la matrice terrain alla mappa basilare
*/
void Map::endBossFight() {
	generateTerrainGrass();
}

/*
	Prende in input un oggetto Position.
	Restituisce true se c'è della lava in quella posizione
*/
bool Map::isLava(Position position) {
	return lavaList.existsAt(position);
}

void Map::generateMapBossType1() {
	const Pixel PLATFORM_TEXTURE = Pixel(char(219), PLATFORM_COLOR_FG, BG_LIGHTGREY, true);
	const Pixel TERRAIN_TEXTURE = Pixel(' ', 0, BG_GREY, true);
	const Pixel SKY_TEXTURE = Pixel(' ', 0, BG_LIGHTGREY, false);

	// Generazione "pavimento"
	for (int i=0; i<TERRAIN_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = TERRAIN_TEXTURE;
		}
	}

	// Generazione cielo
	for (int i=TERRAIN_HEIGHT; i<GAME_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = SKY_TEXTURE;
		}
	}

	// Generazione piattaforme
	for (int i=0; i<GAME_WIDTH; i++) {
		terrain[i][4] = PLATFORM_TEXTURE;
	}
}

void Map::generateMapBossType2() {
	const Pixel PLATFORM_TEXTURE = Pixel(char(219), PLATFORM_COLOR_FG, BG_LIGHTGREY, true);
	const Pixel TERRAIN_TEXTURE = Pixel(' ', 0, BG_GREY, true);
	const Pixel SKY_TEXTURE = Pixel(' ', 0, BG_LIGHTGREY, false);

	// Generazione cielo
	for (int i=0; i<GAME_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][i] = SKY_TEXTURE;
		}
	}

	// Generazione "pavimento"
	for (int i=0; i<1; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = LAVA_TEXTURE;
			lavaList.insert(Position(j, GAME_HEIGHT-i-1));
		}
	}

	// Generazione piattaforme
	terrain[1][3] = PLATFORM_TEXTURE;
	terrain[GAME_WIDTH-2][3] = PLATFORM_TEXTURE;

	for (int i=0; i<3; i++) {
		for (int j=EMPTYZONE_LENGTH; j<GAME_WIDTH-EMPTYZONE_LENGTH; j++) {
			terrain[j][(GAME_HEIGHT-TERRAIN_HEIGHT)-TERRAIN_HEIGHT*i] = PLATFORM_TEXTURE;
		}
	}
}

void Map::generateMapBossType3() {
	const Pixel PLATFORM_TEXTURE = Pixel(char(219), PLATFORM_COLOR_FG, BG_LIGHTGREY, true);
	const Pixel TERRAIN_TEXTURE = Pixel(' ', 0, BG_GREY, true);
	const Pixel SKY_TEXTURE = Pixel(' ', 0, BG_LIGHTGREY, false);

	// Generazione cielo
	for (int i=0; i<GAME_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][i] = SKY_TEXTURE;
		}
	}

	// Generazione "pavimento"
	for (int i=0; i<TERRAIN_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = TERRAIN_TEXTURE;
		}
	}
}