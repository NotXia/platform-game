#include "Map.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "settings.h"
#include "EntityGenerator.h"

const Pixel PLATFORM_TEXTURE = Pixel(PLATFORM_SYMBOL, PLATFORM_COLOR_FG, PLATFORM_COLOR_BG, true);
const Pixel TERRAIN_GRASS_TEXTURE = Pixel(GRASS_SYMBOL, GRASS_COLOR_FG, GRASS_COLOR_BG, true);
const Pixel TERRAIN_ROCK_TEXTURE = Pixel(ROCK_SYMBOL, ROCK_COLOR_FG, ROCK_COLOR_BG, true);
const Pixel SKY_TEXTURE = Pixel(SKY_SYMBOL, SKY_COLOR_FG, SKY_COLOR_BG, false);
const Pixel WALL_TEXTURE = Pixel(WALL_SYMBOL, WALL_COLOR_FG, WALL_COLOR_BG, true);
const Pixel LAVA_TEXTURE = Pixel(LAVA_SYMBOL, LAVA_COLOR_FG, LAVA_COLOR_BG, true);

const Pixel HOUSE_WALL_TEXTURE = Pixel(' ', 0, BG_LIGHTGREY, false);
const Pixel HOUSE_BORDER_TEXTURE = Pixel(' ', 0, BG_BLACK, false);
const Pixel HOUSE_LOWER_ROOF_TEXTURE = Pixel(char(220), FG_BLACK, SKY_COLOR_BG, false);
const Pixel HOUSE_UPPER_ROOF_TEXTURE = Pixel(char(223), FG_BLACK, SKY_COLOR_BG, false);
const Pixel HOUSE_ROOF_PADDING_TEXTURE = Pixel(' ', 0, BG_DARKYELLOW, false);
const Pixel HOUSE_LOWER_ROOF_PADDING_TEXTURE = Pixel(char(220), FG_BLACK, BG_DARKYELLOW, false);
const Pixel HOUSE_UPPER_ROOF_PADDING_TEXTURE = Pixel(char(223), FG_BLACK, BG_DARKYELLOW, false);
const Pixel HOUSE_DOOR_TEXTURE = Pixel(' ', 0, BG_DARKYELLOW, false);
const Pixel HOUSE_WINDOW_TEXTURE = Pixel(char(197), FG_WHITE, BG_GREY, false);

const Pixel TREE_TRUNK_TEXTURE = Pixel(' ', 0, BG_DARKYELLOW, false);
const Pixel TREE_LEAF_TEXTURE = Pixel(' ', 0, BG_DARKGREEN, false);

const Pixel FENCE_TEXTURE = Pixel(char(197), FG_DARKYELLOW, SKY_COLOR_BG, false);


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

	if (level_number == 0) { // Primo livello
		generateTerrainGrass();
		generateEnemies(ceil(1 + log2(getDifficulty())));
		generateBonuses(rand() % 2);
	}
	else if (level_number == getDifficulty()*DIFFICULTY_INCREASE_RATE-1) { // Boss
		delete boss;
		boss = new Boss();
		*boss = createBoss(getDifficulty());
		left_position.setX(boss->generateTerrain(terrain, lavaList));
		place_wall();
	}
	else if (level_number == getDifficulty()*DIFFICULTY_INCREASE_RATE-2) { // Villaggio
		generateTerrainGrass();
		generateTown();
	}
	else { // Livello
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

/*
	Inizializza la matrice terrain con gli elementi base di un livello roccioso.
*/
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
	Inizializza la matrice terrain con le piattaforme.
*/
void Map::generatePlatforms() {
	/* 
		Generazione piattaforme:
		- L'area iniziale e finale del livello rimangono libere da piattaforme
		- L'area centrale prevede la generazione di piattaforme
	*/
	for (int height=GAME_HEIGHT-TERRAIN_HEIGHT-1; height>=3; height--) {

		for (int width=EMPTYZONE_LENGTH; width<GAME_WIDTH-EMPTYZONE_LENGTH; width++) {
			bool generate_platform = (rand() % PLATFORM_CHANCE) == 0;

			/* 
				La piattaforma viene generata se:
				- La variabile generate_platform � true
				- La distanza dal soffitto � di almeno 3 blocchi
				- I due blocchi immediatamente sotto non sono solidi, mentre il terzo lo � (permette al giocatore di raggiungerlo)
			*/
			if (generate_platform && (!terrain[width][height+1].isSolid() && !terrain[width][height+2].isSolid() && terrain[width][height+3].isSolid())) {
				int platform_size = rand() % (PLATFORM_MAX_LENGTH-PLATFORM_MIN_LENGTH+1) + PLATFORM_MIN_LENGTH;

				if (platform_size <= GAME_WIDTH-width-EMPTYZONE_LENGTH) { // Se la dimensione della piattaforma supera l'area "neutrale" a destra, annulla la generazione
					for (int k=0; k<platform_size; k++) {
						terrain[width][height] = PLATFORM_TEXTURE;
						width++;
					}
				}
			}
		}
	}
}

/*
	Prende in input un intero che rappresenta il numero massimo di nemici generabili.
	Genera e inizializza i nemici e li inserisce in enemyList.
*/
void Map::generateEnemies(int max_enemies) {
	if (max_enemies != 0) {
		int base_chance = 10;
		int chance = base_chance;
		int generate;

		int i = GAME_HEIGHT - TERRAIN_HEIGHT;
		int j;

		// La generazione inizia dall'angolo in basso a destra, scorrendo orizzontalmente l'area di gioco fino a raggiungere l'angolo in alto a sinistra
		while(i >= 0 && max_enemies > 0) {
			j = GAME_WIDTH - (EMPTYZONE_LENGTH * 2);
			while (j > (EMPTYZONE_LENGTH*2) && max_enemies > 0) {
				if (chance > 1000) { chance = 1000; }

				if (terrain[j][i].isSolid() && !lavaList.existsAt(Position(j, i)) && !terrain[j][i-1].isSolid() && !terrain[j][i-2].isSolid()) {
					generate = rand() % 1000 + 1;

					if (generate < chance && max_enemies > 0) {
						Enemy new_enemy = createEnemy(getDifficulty());
						new_enemy.setPosition(Position(j, i-1));
						enemyList.add(new_enemy);

						max_enemies--;
						chance = base_chance;
					}
				}
				j--;
				if (max_enemies > 0) {
					chance += rand() % (max_enemies*2-1) + 1;
				}
			}
			i--;
		}
	}
}

/*
	Prende in input un intero che rappresenta il numero massimo di bonus generabili.
	Genera e inizializza i bonus e li inserisce in bonusList.
*/
void Map::generateBonuses(int max_bonus) {
	if (max_bonus != 0) {
		int base_chance = 10;
		int chance = base_chance;
		int generate;

		int i = 2;
		int j;

		// La generazione inizia dall'angolo in alto a destra, scorrendo orizzontalmente l'area di gioco fino a raggiungere l'angolo in basso a sinistra
		while (i<GAME_HEIGHT-TERRAIN_HEIGHT+1 && max_bonus > 0) {
			j = GAME_WIDTH-(EMPTYZONE_LENGTH*2);

			while  (j>(EMPTYZONE_LENGTH*2) && max_bonus > 0) {
				if (chance > 1000) { chance = 1000; }
				
				if (terrain[j][i].isSolid() && !lavaList.existsAt(Position(j, i)) && !terrain[j][i-1].isSolid() && !terrain[j][i-2].isSolid()) {
					generate = rand() % 1000 + 1;

					if (generate < chance && max_bonus > 0) {
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
				if (max_bonus > 0) {
					chance += rand() % (max_bonus*2-1) + 1;
				}
			}
			i++;
		}
	}
}

/*
	Inizializza la matrice terrain con gli elementi di un villaggio.
*/
void Map::generateTown() {
	int start_x = EMPTYZONE_LENGTH;
	int end_x = GAME_WIDTH-EMPTYZONE_LENGTH;
	int start_y = GAME_HEIGHT-TERRAIN_HEIGHT-1;

	while (end_x-start_x > 15) {
		int left_size = rand() % 4 + 1;
		int right_size = rand() % 4 + 1;

		if (left_size+right_size > end_x-start_x) {
			left_size = (end_x-start_x)/2;
			right_size = (end_x-start_x)/2;
		}

		int structure_type = rand() % 3;

		// Generazione casa
		if (structure_type > 0) {
			int height = rand() % 3 + 5;

			terrain[start_x][start_y - height + 2] = HOUSE_UPPER_ROOF_TEXTURE;
			start_x++;
			terrain[start_x][start_y - height + 1] = HOUSE_BORDER_TEXTURE;
			start_x++;

			// Muro sinistro
			for (int j = 0; j < height - 1; j++)
				terrain[start_x][start_y - j] = HOUSE_BORDER_TEXTURE;
			terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height] = HOUSE_LOWER_ROOF_TEXTURE;
			start_x++;

			// Prima colonna
			for (int j = 0; j < height - 1; j++)
				terrain[start_x][start_y - j] = HOUSE_WALL_TEXTURE;
			terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height] = HOUSE_UPPER_ROOF_PADDING_TEXTURE;
			start_x++;

			// Larghezza a sinistra
			for (int i = 0; i < left_size; i++) {
				for (int j = 0; j < height - 1; j++)
					terrain[start_x][start_y - j] = HOUSE_WALL_TEXTURE;
				terrain[start_x][start_y - height + 3] = HOUSE_WINDOW_TEXTURE;
				terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
				terrain[start_x][start_y - height] = HOUSE_ROOF_PADDING_TEXTURE;
				terrain[start_x][start_y - height - 1] = HOUSE_LOWER_ROOF_TEXTURE;
				start_x++;
			}

			// Colonna prima della porta
			for (int j = 0; j < height - 1; j++)
				terrain[start_x][start_y - j] = HOUSE_WALL_TEXTURE;
			terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height] = HOUSE_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height - 1] = HOUSE_LOWER_ROOF_TEXTURE;
			start_x++;

			// Porta
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++)
					terrain[start_x][start_y - j] = HOUSE_DOOR_TEXTURE;
				for (int j = 0; j < height - 3; j++)
					terrain[start_x][start_y - 2 - j] = HOUSE_WALL_TEXTURE;
				terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
				terrain[start_x][start_y - height] = HOUSE_ROOF_PADDING_TEXTURE;
				terrain[start_x][start_y - height - 1] = HOUSE_LOWER_ROOF_TEXTURE;
				start_x++;
			}

			// Colonna dopo la porta
			for (int j = 0; j < height - 1; j++)
				terrain[start_x][start_y - j] = HOUSE_WALL_TEXTURE;
			terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height] = HOUSE_DOOR_TEXTURE;
			terrain[start_x][start_y - height - 1] = HOUSE_LOWER_ROOF_TEXTURE;
			start_x++;

			// Larghezza a destra
			for (int i = 0; i < right_size; i++) {
				for (int j = 0; j < height - 1; j++)
					terrain[start_x][start_y - j] = HOUSE_WALL_TEXTURE;
				terrain[start_x][start_y - height + 3] = HOUSE_WINDOW_TEXTURE;
				terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
				terrain[start_x][start_y - height] = HOUSE_ROOF_PADDING_TEXTURE;
				terrain[start_x][start_y - height - 1] = HOUSE_LOWER_ROOF_TEXTURE;
				start_x++;
			}

			// Ultima colonna
			for (int j = 0; j < height - 1; j++)
				terrain[start_x][start_y - j] = HOUSE_WALL_TEXTURE;
			terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height] = HOUSE_UPPER_ROOF_PADDING_TEXTURE;
			start_x++;

			// Muro destro
			for (int j = 0; j < height - 1; j++)
				terrain[start_x][start_y - j] = HOUSE_BORDER_TEXTURE;
			terrain[start_x][start_y - height + 1] = HOUSE_LOWER_ROOF_PADDING_TEXTURE;
			terrain[start_x][start_y - height] = HOUSE_LOWER_ROOF_TEXTURE;
			start_x++;

			terrain[start_x][start_y - height + 1] = HOUSE_BORDER_TEXTURE;
			start_x++;

			terrain[start_x][start_y - height + 2] = HOUSE_UPPER_ROOF_TEXTURE;
		}
		else { // Generazione albero
			int height = rand() % 3 + 5;

			// Prime foglie dell'albero senza tronco
			terrain[start_x][start_y - height + 2] = TREE_LEAF_TEXTURE;
			terrain[start_x][start_y - height + 3] = TREE_LEAF_TEXTURE;
			start_x++;

			for(int j=1;j<=4;j++) 
				terrain[start_x][start_y - height + j] = TREE_LEAF_TEXTURE;
			start_x++;

			// Albero con tronco
			for (int i = 0; i < left_size; i++) {
				for (int j = 0; j < height - 1; j++)
					terrain[start_x][start_y - j] = TREE_TRUNK_TEXTURE;
				terrain[start_x][start_y - height] = TREE_LEAF_TEXTURE;
				for (int j = 1; j <= 4; j++)
					terrain[start_x][start_y - height + j] = TREE_LEAF_TEXTURE;
				start_x++;
			}

			// Ultime foglie dell'albero senza tronco
			for (int j = 1; j <= 4; j++)
				terrain[start_x][start_y - height + j] = TREE_LEAF_TEXTURE;
			start_x++;

			terrain[start_x][start_y - height + 2] = TREE_LEAF_TEXTURE;
			terrain[start_x][start_y - height + 3] = TREE_LEAF_TEXTURE;
			start_x++;
		}
		
		// Generazione staccionate tra edifici
		int fence_length = rand() % 5 + 2;
		if (fence_length < end_x-start_x) {
			for (int j = 0; j < fence_length; j++) {
				terrain[start_x][start_y] = FENCE_TEXTURE;
				start_x++;
			} 
		}
	}

	int first_x = rand() % (GAME_WIDTH-EMPTYZONE_LENGTH-EMPTYZONE_LENGTH) + EMPTYZONE_LENGTH;
	int second_x = rand() % (GAME_WIDTH-EMPTYZONE_LENGTH-EMPTYZONE_LENGTH) + EMPTYZONE_LENGTH;

	npcList.insert(
		NPC (
			1, 
			Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false), 
			Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
			Pixel(char(219), NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
			Position(first_x, GAME_HEIGHT-TERRAIN_HEIGHT-1),
			NPC_HOSPITAL, getDifficulty()
		)
	);

	if (first_x != second_x) { // Il secondo mercante viene creato se si trova in una posizione diversa dal primo
		npcList.insert(
			NPC(
				1, 
				Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false), 
				Pixel('<', NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
				Pixel(char(219), NPC_HEAD_COLOR_FG, BACKGROUND_DEFAULT, false),
				Position(second_x, GAME_HEIGHT-TERRAIN_HEIGHT-1),
				NPC_WEAPONSHOP, getDifficulty()
			)
		);
	}
}

/*
	Inserisce nella matrice terrain la lava.
	Imposta lavaList.
*/
void Map::generateLava() {
	int h = GAME_HEIGHT - TERRAIN_HEIGHT - 3;

	// La lava viene generata sotto le piattaforme all'altezza del giocatore, lasciando lo spazio per evitarle attraversando la piattaforma.
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
	Restituisce la difficolt� corrente, calcolata in base al numero di livelli superati.
*/
int Map::getDifficulty() {
	return int(level_number/DIFFICULTY_INCREASE_RATE)+1;
}

/*
	Restituisce true se *prev � NULL, false altrimenti.
*/
bool Map::prevNull() {
	return (prev == NULL);
}

/*
	Prende in input la posizione da cui il giocatore � uscito dal livello corrente.
	Restituisce il puntatore al livello precedente, impostando right_position con il parametro in input.
*/
Map* Map::gotoPrevious(Position exit_position) {
	this->prev->right_position = Position(GAME_WIDTH-1, exit_position.getY());
	return this->prev;
}

/*
	Prende in input la posizione da cui il giocatore � uscito dal livello corrente.
	Restituisce il puntatore al livello successivo, impostando left_position con il parametro in input.
*/
Map* Map::gotoNext(Position exit_position) {
	if (this->next == NULL) {
		this->next = new Map(this, level_number+1);
	}
	if (!this->next->isBossFight()) {
		this->next->left_position = Position(0, exit_position.getY());
	}
	return this->next;
}

int Map::getLevelNumber() {
	return this->level_number;
}

/*
	Prende in input una posizione.
	Restituisce true se quella posizione � solida, false altrimenti.
	Valuta considerando il terreno, i nemici e il boss.
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
	Restituisce quella posizione della mappa, ignorando la lista di nemici.
*/
Pixel Map::getTerrainAt(Position position) {
	Pixel out;
	out = terrain[position.getX()][position.getY()];
	return out;
}

/*
	Prende in input un oggetto Bullet.
	Inserisce il parametro nella lista BulletList.
*/
void Map::addBullet(Bullet bullet) {
	bulletList.insert(bullet);
}

/*
	Prende in input un oggetto Bonus.
	Inserisce il parametro nella lista BonusList e restituisce la posizione in cui � stato inserito.
	Se c'� gi� un bonus in tale posizione, il nuovo bonus viene spostato nella prima posizione disponibile nelle vicinanze.
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
	Restituisce true boss non � NULL.
*/
bool Map::isBossFight() {
	return boss != NULL;
}

/*
	Imposta la matrice terrain in modo da avere l'entrata e l'uscita del livello bloccati.
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
	generateTerrainRock();
}

/*
	Prende in input un oggetto Position.
	Restituisce true se c'� della lava in quella posizione
*/
bool Map::isLava(Position position) {
	return lavaList.existsAt(position);
}