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
		Pixel('ch sx', colore | BACKGROUND_DEFAULT, false),
		Pixel('ch dx', colore| BACKGROUND_DEFAULT, false),
		Bullet(Pixel('ch proiettile sx', 'ch proiettile dx', colore | BACKGROUND_DEFAULT, false), danni, range),
		n_caricatore,
		tempo_ricarica,
		tempo_sparo
	);
*/

/*
	Inizializza gli array con le armi destinate al giocatore
*/
void WeaponContainer::initForPlayer() {
	insert_tier3(Weapon(
		"Coltellino",
		Pixel('-', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel('-', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 1, 2),
		1,
		0,
		3500
	));

	insert_tier3(Weapon(
		"Fionda",
		Pixel('Y', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', FG_BLACK | BACKGROUND_DEFAULT, false), 1, 5),
		1,
		5000,
		8000
	));

	// TODO: la spada non lancia proiettili, ma una linea retta continua
	insert_tier2(Weapon(
		"Spada",
		Pixel(char(196), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 5, 3),
		1,
		0,
		5500
	));

	insert_tier2(Weapon(
		"Pistola",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('o', FG_BLACK | BACKGROUND_DEFAULT, false), 2, 10),
		10,
		15000,
		12000
	));

	insert_tier1(Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 5, 15),
		20,
		12000,
		5000
	));

	insert_tier1(Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 8, 20),
		35,
		15000,
		3500
	));

	insert_tier1(Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 10, GAME_WIDTH),
		5,
		30000,
		70000
	));

	// TODO: Armi intermedie

	// TODO: Migliorare il laser (togliere il proiettile e fare un effettivo laser)
	insert_special(Weapon(
		"Laser",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel(char(196), FG_DARKRED | BACKGROUND_DEFAULT, false), 30, GAME_WIDTH),
		1,
		100000,
		90000
	));

}

/*
	Inizializza gli array con le armi destinate ai nemici
*/
void WeaponContainer::initForEnemy() {
	insert_tier3(Weapon(
		"Coltellino",
		Pixel('-', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel('-', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 1, 2),
		1,
		0,
		3500
	));

	insert_tier3(Weapon(
		"Fionda",
		Pixel('Y', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel('Y', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('.', FG_BLACK | BACKGROUND_DEFAULT, false), 1, 5),
		1,
		5000,
		8000
	));

	insert_tier2(Weapon(
		"Spada",
		Pixel(char(196), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(196), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel(' ', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 2, 3),
		1,
		0,
		5500
	));

	insert_tier2(Weapon(
		"Pistola",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('o', FG_BLACK | BACKGROUND_DEFAULT, false), 2, 10),
		10,
		15000,
		12000
	));

	insert_tier1(Weapon(
		"Mitraglietta",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 2, 15),
		20,
		12000,
		5000
	));

	insert_tier1(Weapon(
		"Fucile d'assalto",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 3, 20),
		35,
		15000,
		3500
	));

	insert_tier1(Weapon(
		"Fucile di precisione",
		Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
		Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 5, GAME_WIDTH),
		5,
		30000,
		70000
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