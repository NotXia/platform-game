#include "WeaponContainer.hpp"
#include <cstdlib>

WeaponContainer::WeaponContainer() {
	tier1_size = 0;
	tier2_size = 0;
	tier3_size = 0;
	special_size = 0;
}


/*
	Weapon variabile = Weapon(
		"Nome arma",
		Pixel('ch sx', colore_fg, colore_bg, false),
		Pixel('ch dx', colore_fg, colore_bg, false),
		Bullet(Pixel('ch proiettile sx', 'ch proiettile dx', colore, BACKGROUND_DEFAULT, false), danni, range),
		n_caricatore,
		tempo_ricarica,
		tempo_sparo,
		tipo,
		range
	);
*/

/*
	Inizializza gli array con le armi destinate al giocatore
*/
void WeaponContainer::initForPlayer() {
	insert_tier1(Weapon(
		"Coltellino",
		Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false), 2, 0),
		1,
		0,
		3500,
		WEAPON_MELEE,
		2
	));

	insert_tier1(Weapon(
		"Fionda",
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', 0, BACKGROUND_DEFAULT, false), 1, 0),
		1,
		5000,
		8000,
		WEAPON_RANGED,
		5
	));

	insert_tier2(Weapon(
		"Spada",
		Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false), 3, 0),
		1,
		0,
		5500,
		WEAPON_MELEE,
		3
	));

	insert_tier2(Weapon(
		"Ascia",
		Pixel('q', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('p', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false), 4, 0),
		1,
		0,
		8000,
		WEAPON_MELEE,
		2
	));

	insert_tier2(Weapon(
		"Pistola",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(250), FG_BLACK, BACKGROUND_DEFAULT, false), 2, 0),
		10,
		15000,
		12000,
		WEAPON_RANGED,
		10
	));

	insert_tier3(Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), 5, 0),
		20,
		12000,
		3500,
		WEAPON_RANGED,
		15
	));

	insert_tier3(Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), 8, 0),
		35,
		15000,
		5000,
		WEAPON_RANGED,
		20
	));

	insert_tier3(Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false), 10, 0),
		5,
		30000,
		70000,
		WEAPON_RANGED,
		GAME_WIDTH
	));

	insert_tier3(Weapon(
		"Fucile a pompa",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('=', FG_BLACK, BACKGROUND_DEFAULT, false), 15, 0),
		4,
		25000,
		20000,
		WEAPON_RANGED,
		5
	));

	insert_special(Weapon(
		"Laser",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_DARKRED, BACKGROUND_DEFAULT, false), 200, 0),
		1,
		50000,
		30000,
		WEAPON_MELEE,
		GAME_WIDTH
	));
}

/*
	Inizializza gli array con le armi destinate ai nemici
*/
void WeaponContainer::initForEnemy() {
	insert_tier1(Weapon(
		"Coltellino",
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('-', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 1, 0),
		1,
		0,
		3500,
		WEAPON_MELEE,
		2
	));

	insert_tier1(Weapon(
		"Fionda",
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', FG_BLACK, BACKGROUND_DEFAULT, false), 1, 0),
		1,
		5000,
		8000,
		WEAPON_RANGED,
		5
	));

	insert_tier2(Weapon(
		"Spada",
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 2, 0),
		1,
		0,
		5500,
		WEAPON_MELEE,
		3
	));

	insert_tier2(Weapon(
		"Ascia",
		Pixel('q', FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel('p', FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', FG_BLACK, BACKGROUND_DEFAULT, false), 3, 0),
		1,
		0,
		8000,
		WEAPON_MELEE,
		2
	));

	insert_tier2(Weapon(
		"Pistola",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('o', FG_BLACK, BACKGROUND_DEFAULT, false), 2, 0),
		10,
		15000,
		12000,
		WEAPON_RANGED,
		10
	));

	insert_tier3(Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 2, 0),
		20,
		12000,
		5000,
		WEAPON_RANGED,
		15
	));

	insert_tier3(Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 2, 0),
		35,
		15000,
		3500,
		WEAPON_RANGED,
		20
	));

	insert_tier3(Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT, BACKGROUND_DEFAULT, false), 3, 0),
		5,
		30000,
		70000,
		WEAPON_RANGED,
		GAME_WIDTH
	));

	insert_tier3(Weapon(
		"Fucile a pompa",
		Pixel(char(191), FG_BLACK, BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_BLACK, BACKGROUND_DEFAULT, false),
		Bullet(Pixel('=', FG_BLACK, BACKGROUND_DEFAULT, false), 5, 0),
		4,
		25000,
		20000,
		WEAPON_RANGED,
		5
	));
}

/*
	Prende in input un oggetto Weapon.
	Inserisce l'oggetto nell'array tier1.
*/
void WeaponContainer::insert_tier1(Weapon weapon) {
	tier1[tier1_size] = weapon;
	tier1_size++;
}

/*
	Prende in input un oggetto Weapon.
	Inserisce l'oggetto nell'array tier2.
*/
void WeaponContainer::insert_tier2(Weapon weapon) {
	tier2[tier2_size] = weapon;
	tier2_size++;
}

/*
	Prende in input un oggetto Weapon.
	Inserisce l'oggetto nell'array tier3.
*/
void WeaponContainer::insert_tier3(Weapon weapon) {
	tier3[tier3_size] = weapon;
	tier3_size++;
}

/*
	Prende in input un oggetto Weapon.
	Inserisce l'oggetto nell'array special.
*/
void WeaponContainer::insert_special(Weapon weapon) {
	special[special_size] = weapon;
	special_size++;
}

/*
	Restituisce un oggetto Weapon casuale contenuto nell'array tier1
*/
Weapon WeaponContainer::getRandomTier1() {
	return tier1[rand() % tier1_size];
}

/*
	Restituisce un oggetto Weapon casuale contenuto nell'array tier2
*/
Weapon WeaponContainer::getRandomTier2() {
	return tier2[rand() % tier2_size];
}

/*
	Restituisce un oggetto Weapon casuale contenuto nell'array tier3
*/
Weapon WeaponContainer::getRandomTier3() {
	return tier3[rand() % tier3_size];
}

/*
	Restituisce un oggetto Weapon casuale contenuto nell'array special
*/
Weapon WeaponContainer::getRandomSpecial() {
	return special[rand() % special_size];
}