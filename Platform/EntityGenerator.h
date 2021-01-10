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