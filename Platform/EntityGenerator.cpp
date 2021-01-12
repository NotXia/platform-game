#include "EntityGenerator.h"
#include "WeaponContainer.hpp"

/*
	Crea un'arma per il giocatore e lo restituisce
*/
Weapon createPlayerWeapon(int difficulty) {
	WeaponContainer *weaponcontainer = new WeaponContainer();
	weaponcontainer->initForPlayer();
	Weapon weapon;

	int tier1_chance = 100;
	int tier2_chance = (difficulty-1) * 25;
	int tier3_chance = (difficulty-1) * 11;

	int generate = rand() % (tier1_chance+tier2_chance+tier3_chance);
	if (generate < tier1_chance) {
		weapon = weaponcontainer->getRandomTier1();
	}
	else if (generate < tier1_chance+tier2_chance) {
		weapon = weaponcontainer->getRandomTier2();
	}
	else {
		weapon = weaponcontainer->getRandomTier3();
	}
	delete weaponcontainer;

	return weapon;
}

/*
	Crea un'arma per nemici e lo restituisce
*/
Weapon createEnemyWeapon(int difficulty) {
	WeaponContainer *weaponcontainer = new WeaponContainer();
	weaponcontainer->initForEnemy();
	Weapon weapon;

	int tier1_chance = 100;
	int tier2_chance = (difficulty-1) * 25;
	int tier3_chance = (difficulty-1) * 11;

	int generate = rand() % (tier1_chance+tier2_chance+tier3_chance);
	if (generate < tier1_chance) {
		weapon = weaponcontainer->getRandomTier1();
	}
	else if (generate < tier1_chance+tier2_chance) {
		weapon = weaponcontainer->getRandomTier2();
	}
	else {
		weapon = weaponcontainer->getRandomTier3();
	}
	delete weaponcontainer;

	return weapon;
}

/*
	Crea un nemico e lo restituisce
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
	Crea un bonus e lo restituisce
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
	Crea un boss e lo restituisce
*/
Boss createBoss(int difficulty) {
	int type = 0;
	Boss boss;

	if (type == BOSS_TYPE1) {
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
			4,
			1,
			6,
			8,
			GAME_HEIGHT
		);
	}
	else if (type == BOSS_TYPE2) {
		/*
			Fase 0: Spara sfere di fuoco
			Fase 1: Sta fermo
			Fase 2: Finito di sparare -> Inizio timer e cade
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
			Position(GAME_WIDTH-2, 0),
			Weapon(),
			type,
			4,
			5,
			10,
			8,
			GAME_HEIGHT
		);
	}

	return boss;
}