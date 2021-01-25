#include "Bonus.hpp"

Bonus::Bonus(Pixel body, Position position, int points, int money, int hp, Weapon *weapon) : Block(body, position) {
	this->points = points;
	this->money = money;
	this->hp = hp;
	this->weapon = weapon;

	this->on_terrain = true;
	this->fall_animation = AnimationTimer(FALL_SPEED);

	if (money != 0 && hp != 0 && weapon != NULL) {
		money = 1;
		hp = 0;
		weapon = NULL;
	}
}

int Bonus::getPoints() {
	return points;
}

void Bonus::setPosition(Position position) {
	this->position = position;
}

Weapon Bonus::getWeapon() {
	return *weapon;
}

void Bonus::setWeapon(Weapon weapon) {
	delete this->weapon;
	this->weapon = new Weapon;
	*(this->weapon) = weapon;
}

/*
	Restituisce il tipo del bonus in base ai campi impostati.
*/
int Bonus::getType() {
	if (money != 0) {
		return BONUS_TYPE_MONEY;
	}
	else if (hp != 0) {
		return BONUS_TYPE_HP;
	}
	else if (weapon != NULL) {
		return BONUS_TYPE_WEAPON;
	}
}

/*
	Restituisce il contenuto del bonus in base al tipo (vita o soldi).
*/
int Bonus::getBonus() {
	int out = 0;

	int type = getType();
	if (type == BONUS_TYPE_MONEY) {
		out = money;
	}
	else if (type == BONUS_TYPE_HP) {
		out = hp;
	}

	return out;
}


/****************************
	INIZIO GESTIONE CADUTA
****************************/

bool Bonus::isOnTerrain() {
	return on_terrain;
}

/*
	Inizializza i parametri per la caduta.
*/
void Bonus::startFall() {
	on_terrain = false;
	fall_animation.reset();
}

/*
	Muove la posizione di un blocco in basso.
*/
void Bonus::fall() {
	this->position = Position(position.getX(), position.getY()+1);
}

/*
	Imposta i parametri per interrompere la caduta.
*/
void Bonus::stopFall() {
	on_terrain = true;
}

/*
	Restituisce true se si può procedere nell'animazione della caduta. False altrimenti.
*/
bool Bonus::canFall() {
	return fall_animation.limit();
}

/* FINE GESTIONE CADUTA
************************/


void Bonus::incCounters() {
	fall_animation.incTimer();
}
