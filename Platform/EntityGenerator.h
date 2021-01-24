#pragma once
#include "Weapon.hpp"
#include "Bonus.hpp"
#include "Enemy.hpp"
#include "Boss.hpp"

/*
	Crea un'arma per il giocatore e la restituisce.
	Il tipo di arma dipende dalla difficoltà.
*/
Weapon createPlayerWeapon(int difficulty);

/*
	Crea un'arma per nemici e la restituisce.
	Il tipo di arma dipende dalla difficoltà.
*/
Weapon createEnemyWeapon(int difficulty);

/*
	Crea un nemico e lo restituisce.
*/
Enemy createEnemy(int difficulty);

/*
	Crea un bonus e lo restituisce.
*/
Bonus createBonus(int difficulty);

/*
	Crea un boss e lo restituisce.
*/
Boss createBoss(int difficulty);

/*
	Crea un'arma casuale di tier 1 per il giocatore.
*/
Weapon getRandomTier1Player();

/*
	Crea un'arma casuale di tier 2 per il giocatore.
*/
Weapon getRandomTier2Player();

/*
	Crea un'arma casuale di tier 3 per il giocatore.
*/
Weapon getRandomTier3Player();

/*
	Crea un'arma speciale casuale.
*/
Weapon getRandomSpecial();

/*
	Crea un'arma casuale di tier 1 per il nemico.
*/
Weapon getRandomTier1Enemy();

/*
	Crea un'arma casuale di tier 2 per il nemico.
*/
Weapon getRandomTier2Enemy();

/*
	Crea un'arma casuale di tier 3 per il nemico.
*/
Weapon getRandomTier3Enemy();

