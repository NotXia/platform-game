#include "Bullet.hpp"
#include "settings.h"

Bullet::Bullet(Pixel texture, int damage, int range, Position position, bool direction, int speed) : Block(texture, position) {
	this->damage = damage;
	this->range = range;
	this->direction = direction;
	this->refresh = AnimationTimer(speed);
	this->hostile = false;
}

int Bullet::getDamage() {
	return damage;
}
int Bullet::getRange() {
	return range;
}
void Bullet::setRange(int range) {
	this->range = range;
}
void Bullet::setHostile(bool hostile) {
	this->hostile = hostile;
}
bool Bullet::isHostile() {
	return this->hostile;
}
Position Bullet::getPosition() {
	return this->position;
}
void Bullet::setPosition(Position position) {
	this->position = position;
}
void Bullet::setDirection(bool direction) {
	this->direction = direction;
}

/*
	Restituisce true se il range � maggiore di 0, false altrimenti.
	Indica se il proiettile pu� continuare ad avanzare.
*/
bool Bullet::canTravel() {
	return range > 0;
}

/*
	Se range � maggiore di 0: decrementa range e muove la posizione di 1 (in base alla direzione).
	In caso la posizione dovesse superare i limiti della mappa, range viene impostato ad un valore nullo.
	Restituisce true se il range � maggiore di 0, false altrimenti.
*/
bool Bullet::travel() {
	if (canTravel()) {
		range--;
		if (direction == BULLET_LEFT) {
			if (position.getX() == 0) {
				range = -1;
			}
			else {
				position.setX(position.getX()-1);
			}
		}
		else {
			if (position.getX() == GAME_WIDTH-1) {
				range = -1;
			}
			else {
				position.setX(position.getX()+1);
			}
		}
	}

	return canTravel();
}

/*
	Indica se � possibile aggiornare lo stato del Bullet.
*/
bool Bullet::canRefresh() {
	return refresh.limit();
}

/*
	Incrementa il timer refresh di 1.
*/
void Bullet::incRefresh() {
	refresh.incTimer();
}

/*
	Imposta range ad un valore negativo e restituisce il danno del proiettile.
*/
int Bullet::hit() {
	range = -1;
	return damage;
}

/*
	Prende in input un oggetto Bullet.
	Confronta l'oggetto corrente con il parametro e dice se sono uguali.
*/
bool Bullet::equals(Bullet bullet) {
	return this->damage == bullet.damage && this->range == bullet.range;
}
