#include "Bonus.hpp"

Bonus::Bonus(Pixel body, Position position, int points, int money, int hp, Weapon *weapon) : Block(body, position) {
	this->points = points;
	this->money = money;
	this->hp = hp;
	this->weapon = weapon;

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
	Restituisce il tipo del bonus in base ai campi impostati
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