#include "EntityGenerator.h"

/*
	Crea un'arma per il giocatore e la restituisce.
	Il tipo di arma dipende dalla difficoltà.
*/
Weapon createPlayerWeapon(int difficulty) {
	Weapon weapon;

	int tier1_chance = 100 + (difficulty-1) * (-10);
	if (tier1_chance < 0) { tier1_chance = 0;  }
	int tier2_chance = 40 + (difficulty-1) * 2;
	int tier3_chance = (difficulty-1) * 8;

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
	int hp = rand() % (difficulty*2) + difficulty;
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
		boss = Boss(
			10 + 5*(difficulty-1),
			BOSS_BASE_POINTS * difficulty,
			BOSS_BASE_MONEY * difficulty,
			Pixel('<', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel('>', FG_DARKRED, BACKGROUND_DEFAULT, true),
			Pixel(char(219), FG_DARKRED, BACKGROUND_DEFAULT, true),
			Position(GAME_WIDTH-2, 0),
			Weapon("Scettro", Pixel(), Pixel(), Bullet(Pixel('o', FG_DARKRED, BACKGROUND_DEFAULT), difficulty, GAME_WIDTH, 100), 8, 1, 12000, WEAPON_RANGED, GAME_WIDTH),
			type,
			8,
			15
		);
	}
	else if (type == BOSS_MELEE) {
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 1; 
	reload_delay = 0; 
	shoot_delay = 4500; 
	range = 2;
	damage = 2;
	weapons[size] = Weapon(
		"Coltellino",
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_MELEE, range
	);
	size++;

	ammo = 1;
	reload_delay = 5000;
	shoot_delay = 9000;
	range = 5;
	damage = 1;
	weapons[size] = Weapon(
		"Fionda",
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', 0, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 1;
	reload_delay = 0;
	shoot_delay = 6500;
	range = 3;
	damage = 3;
	weapons[size] = Weapon(
		"Spada",
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_MELEE, range
	);
	size++;

	ammo = 1;
	reload_delay = 0;
	shoot_delay = 9000;
	range = 2;
	damage = 4;
	weapons[size] = Weapon(
		"Ascia",
		Pixel('q', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('p', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_MELEE, range
	);
	size++;

	ammo = 10;
	reload_delay = 15000;
	shoot_delay = 13000;
	range = 10;
	damage = 2;
	weapons[size] = Weapon(
		"Pistola",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(250), FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 20;
	reload_delay = 12000;
	shoot_delay = 4500;
	range = 15;
	damage = 5;
	weapons[size] = Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	ammo = 35;
	reload_delay = 15000;
	shoot_delay = 6000;
	range = 20;
	damage = 8;
	weapons[size] = Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	ammo = 5;
	reload_delay = 30000;
	shoot_delay = 70000;
	range = GAME_WIDTH;
	damage = 10;
	weapons[size] = Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	ammo = 4;
	reload_delay = 25000;
	shoot_delay = 20000;
	range = 5;
	damage = 15;
	weapons[size] = Weapon(
		"Fucile a pompa",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('=', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 1;
	reload_delay = 50000;
	shoot_delay = 30000;
	range = GAME_WIDTH;
	damage = 200;
	weapons[size] = Weapon(
		"Laser",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_DARKRED, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_MELEE, range
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 1;
	reload_delay = 0;
	shoot_delay = 8000;
	range = 2;
	damage = 1;
	weapons[size] = Weapon(
		"Coltellino",
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_MELEE, range
	);
	size++;

	ammo = 1;
	reload_delay = 5000;
	shoot_delay = 12000;
	range = 5;
	damage = 1;
	weapons[size] = Weapon(
		"Fionda",
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 1;
	reload_delay = 0;
	shoot_delay = 6500;
	range = 3;
	damage = 1;
	weapons[size] = Weapon(
		"Spada",
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_MELEE, range
	);
	size++;

	ammo = 1;
	reload_delay = 0;
	shoot_delay = 9000;
	range = 2;
	damage = 2;
	weapons[size] = Weapon(
		"Ascia",
		Pixel('q', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('p', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, MELEE_BULLET_SPEED),
		ammo,
		reload_delay,
		shoot_delay,
		WEAPON_MELEE,
		range
	);
	size++;

	ammo = 10;
	reload_delay = 15000;
	shoot_delay = 13000;
	range = 10;
	damage = 1;
	weapons[size] = Weapon(
		"Pistola",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('o', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
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
	int ammo, reload_delay, shoot_delay, range, damage;

	ammo = 20;
	reload_delay = 12000;
	shoot_delay = 10000;
	range = 15;
	damage = 1;
	weapons[size] = Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	ammo = 35;
	reload_delay = 15000;
	shoot_delay = 7000;
	range = 20;
	damage = 1;
	weapons[size] = Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	ammo = 5;
	reload_delay = 30000;
	shoot_delay = 70000;
	range = GAME_WIDTH;
	damage = 3;
	weapons[size] = Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	ammo = 4;
	reload_delay = 25000;
	shoot_delay = 20000;
	range = 5;
	damage = 2;
	weapons[size] = Weapon(
		"Fucile a pompa",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('=', FG_BLACK, BACKGROUND_DEFAULT, false), damage, 0, RANGED_BULLET_SPEED),
		ammo, reload_delay, shoot_delay, WEAPON_RANGED, range
	);
	size++;

	return weapons[rand() % size];
}