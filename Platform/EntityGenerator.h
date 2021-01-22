#pragma once
#include "Weapon.hpp"
#include "Bonus.hpp"
#include "Enemy.hpp"
#include "Boss.hpp"

/*
	Crea un'arma per il giocatore e lo restituisce
*/
Weapon createPlayerWeapon(int difficulty);

/*
	Crea un'arma per nemici e lo restituisce
*/
Weapon createEnemyWeapon(int difficulty);

/*
	Crea un bonus e lo restituisce
*/
Bonus createBonus(int difficulty);

/*
	Crea un nemico e lo restituisce
*/
Enemy createEnemy(int difficulty);

/*
	Crea un boss e lo restituisce
*/
Boss createBoss(int difficulty);

/*
*/
Weapon getRandomTier1Player();

/*
*/
Weapon getRandomTier2Player();

/*
*/
Weapon getRandomTier3Player();

/*
*/
Weapon getRandomSpecial();

/*
*/
Weapon getRandomTier1Enemy();

/*
*/
Weapon getRandomTier2Enemy();

/*
*/
Weapon getRandomTier3Enemy();

