#include "EntityGenerator.h"

/*
	Crea un'arma per il giocatore e la restituisce.
	Il tipo di arma dipende dalla difficoltà.
*/
Weapon createPlayerWeapon(int difficulty) {
	Weapon weapon;

	int tier1_chance = 100;
	int tier2_chance = (difficulty-1) * 25;
	int tier3_chance = (difficulty-1) * 11;

	int generate = rand() % (tier1_chance+tier2_chance+tier3_chance);
	if (generate < tier1_chance) {
		weapon = getRandomTier1Player();
	}
	else if (generate < tier1_chance+tier2_chance) {
		weapon = getRandomTier2Player();
	}
	else {
		weapon = getRandomTier3Player();
	}

	return weapon;
}

/*
	Crea un'arma per nemici e la restituisce.
	Il tipo di arma dipende dalla difficoltà.
*/
Weapon createEnemyWeapon(int difficulty) {
	Weapon weapon;

	int tier1_chance = 100;
	int tier2_chance = (difficulty-1) * 25;
	int tier3_chance = (difficulty-1) * 11;

	int generate = rand() % (tier1_chance+tier2_chance+tier3_chance);
	if (generate < tier1_chance) {
		weapon = getRandomTier1Enemy();
	}
	else if (generate < tier1_chance+tier2_chance) {
		weapon = getRandomTier2Enemy();
	}
	else {
		weapon = getRandomTier3Enemy();
	}

	return weapon;
}

/*
	Crea un nemico e lo restituisce.
*/
Enemy createEnemy(int difficulty) {
	int hp = rand() % (difficulty*2 - difficulty + 1) + difficulty;
	int points = ENEMY_BASE_POINTS * difficulty;
	int money = rand() % difficulty + 1;

	Enemy new_enemy = Enemy(
		hp, points, money,
		Pixel(ENEMY_HEAD_LEFT, ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
		Pixel(ENEMY_HEAD_RIGHT, ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
		Pixel(ENEMY_BODY, ENEMY_BODY_COLOR_FG, BACKGROUND_DEFAULT, true), Position(), createEnemyWeapon(difficulty)
	);

	return new_enemy;
}

/*
	Crea un bonus e lo restituisce.
*/
Bonus createBonus(int difficulty) {
	int type = rand() % 100;
	Bonus bonus;

	int points = BONUS_BASE_POINTS * difficulty;
	if (type < 40) { // Bonus soldi
		int money = rand() % difficulty + 1;
		bonus = Bonus(Pixel(MONEY_SYMBOL, MONEY_COLOR, BACKGROUND_DEFAULT, false), Position(), points, money, 0, NULL);
	}
	else if (type < 80) { // Bonus hp
		int hp = floor((1/(log10(difficulty)+1)) * 5);
		bonus = Bonus(Pixel(HP_SYMBOL, FG_DARKRED, BACKGROUND_DEFAULT, false), Position(), points, 0, hp, NULL);
	}
	else { // Bonus arma
		Weapon *weapon = new Weapon;
		*weapon = createPlayerWeapon(difficulty);
		bonus = Bonus(Pixel(char(170), FG_BLACK, BACKGROUND_DEFAULT, false), Position(), 0, 0, 0, weapon);
	}

	return bonus;
}

/*
	Crea un boss e lo restituisce.
*/
Boss createBoss(int difficulty) {
	int type = rand() % 3;
	Boss boss;

	if (type == BOSS_SUMMONER) {
		/*
			Fase 0: Evoca i nemici
			Fase 1: Va avanti e indietro
			Fase 2: Nemici sconfitti -> Inizio timer e cade
			Fase 3: Caduto
			Fase 4: Timer terminato -> Salta
		*/
		boss = Boss(
			10 + 5*(difficulty-1),
			BOSS_BASE_POINTS * difficulty,
			BOSS_BASE_MONEY * difficulty,
			Pixel('<', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel('>', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel(char(219), FG_DARKRED, BACKGROUND_DEFAULT, true),
			Position(GAME_WIDTH-int(GAME_WIDTH*0.25), 0),
			Weapon(),
			type,
			1,
			6
		);
	}
	else if (type == BOSS_MAGE) {
		/*
			Fase 0: Spara sfere di fuoco
			Fase 1: Sta fermo
			Fase 2: Finito di sparare -> Inizio timer e cade
			Fase 3: Caduto
			Fase 4: Timer terminato -> Si teletrasporta
		*/
		boss = Boss(
			10 + 5*(difficulty-1),
			BOSS_BASE_POINTS * difficulty,
			BOSS_BASE_MONEY * difficulty,
			Pixel('<', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel('>', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel(char(219), FG_DARKRED, BACKGROUND_DEFAULT, true),
			Position(GAME_WIDTH-2, 0),
			Weapon("Scettro", Pixel(), Pixel(), Bullet(Pixel('o', FG_DARKRED, BACKGROUND_DEFAULT), difficulty, GAME_WIDTH), 5, 1, 12000, WEAPON_RANGED, GAME_WIDTH),
			type,
			5,
			10
		);
	}
	else if (type == BOSS_MELEE) {
		/*
			Fase 0: Corre + se tocca player, lo lancia
			Fase 1: Finito di correre -> Inizio timer e stordito
			Fase 2: stordito
			Fase 3: Timer terminato -> Si gira
		*/
		boss = Boss(
			10 + 5*(difficulty-1),
			BOSS_BASE_POINTS * difficulty,
			BOSS_BASE_MONEY * difficulty,
			Pixel('<', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel('>', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel(char(219), FG_DARKRED, BACKGROUND_DEFAULT, true),
			Position(GAME_WIDTH-2, TERRAIN_HEIGHT-2),
			Weapon(),
			type,
			1,
			4
		);
	}

	return boss;
}

/*
	Crea un'arma casuale di tier 1 per il giocatore.
*/
Weapon getRandomTier1Player() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Coltellino",
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false), 2, 0),
		1,
		0,
		3500,
		WEAPON_MELEE,
		2
	);
	size++;

	weapons[size] = Weapon(
		"Fionda",
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', 0, BACKGROUND_DEFAULT, false), 1, 0),
		1,
		5000,
		8000,
		WEAPON_RANGED,
		5
	);
	size++;

	return weapons[rand() % size];
}

/*
	Crea un'arma casuale di tier 2 per il giocatore.
*/
Weapon getRandomTier2Player() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Spada",
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false), 3, 0),
		1,
		0,
		5500,
		WEAPON_MELEE,
		3
	);
	size++;

	weapons[size] = Weapon(
		"Ascia",
		Pixel('q', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('p', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false), 4, 0),
		1,
		0,
		8000,
		WEAPON_MELEE,
		2
	);
	size++;

	weapons[size] = Weapon(
		"Pistola",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(250), FG_BLACK, BACKGROUND_DEFAULT, false), 2, 0),
		10,
		15000,
		12000,
		WEAPON_RANGED,
		10
	);
	size++;

	return weapons[rand() % size];
}

/*
	Crea un'arma casuale di tier 3 per il giocatore.
*/
Weapon getRandomTier3Player() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), 5, 0),
		20,
		12000,
		3500,
		WEAPON_RANGED,
		15
	);
	size++;

	weapons[size] = Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), 8, 0),
		35,
		15000,
		5000,
		WEAPON_RANGED,
		20
	);
	size++;

	weapons[size] = Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), 10, 0),
		5,
		30000,
		70000,
		WEAPON_RANGED,
		GAME_WIDTH
	);
	size++;

	weapons[size] = Weapon(
		"Fucile a pompa",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('=', FG_BLACK, BACKGROUND_DEFAULT, false), 15, 0),
		4,
		25000,
		20000,
		WEAPON_RANGED,
		5
	);
	size++;

	return weapons[rand() % size];
}

/*
	Crea un'arma speciale casuale.
*/
Weapon getRandomSpecial() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Laser",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_DARKRED, BACKGROUND_DEFAULT, false), 200, 0),
		1,
		50000,
		30000,
		WEAPON_MELEE,
		GAME_WIDTH
	);
	size++;

	return weapons[rand() % size];
}


/*
	Crea un'arma casuale di tier 1 per il nemico.
*/
Weapon getRandomTier1Enemy() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Coltellino",
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 1, 0),
		1,
		0,
		3500,
		WEAPON_MELEE,
		2
	);
	size++;

	weapons[size] = Weapon(
		"Fionda",
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', FG_BLACK, BACKGROUND_DEFAULT, false), 1, 0),
		1,
		5000,
		8000,
		WEAPON_RANGED,
		5
	);
	size++;

	return weapons[rand() % size];
}

/*
	Crea un'arma casuale di tier 2 per il nemico.
*/
Weapon getRandomTier2Enemy() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Spada",
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 2, 0),
		1,
		0,
		5500,
		WEAPON_MELEE,
		3
	);
	size++;

	weapons[size] = Weapon(
		"Ascia",
		Pixel('q', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('p', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false), 3, 0),
		1,
		0,
		8000,
		WEAPON_MELEE,
		2
	);
	size++;

	weapons[size] = Weapon(
		"Pistola",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('o', FG_BLACK, BACKGROUND_DEFAULT, false), 2, 0),
		10,
		15000,
		12000,
		WEAPON_RANGED,
		10
	);
	size++;

	return weapons[rand() % size];
}

/*
	Crea un'arma casuale di tier 3 per il nemico.
*/
Weapon getRandomTier3Enemy() {
	Weapon weapons[10];
	int size = 0;

	weapons[size] = Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 2, 0),
		20,
		12000,
		5000,
		WEAPON_RANGED,
		15
	);
	size++;

	weapons[size] = Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 2, 0),
		35,
		15000,
		3500,
		WEAPON_RANGED,
		20
	);
	size++;

	weapons[size] = Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 3, 0),
		5,
		30000,
		70000,
		WEAPON_RANGED,
		GAME_WIDTH
	);
	size++;

	weapons[size] = Weapon(
		"Fucile a pompa",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('=', FG_BLACK, BACKGROUND_DEFAULT, false), 5, 0),
		4,
		25000,
		20000,
		WEAPON_RANGED,
		5
	);
	size++;

	return weapons[rand() % size];
}