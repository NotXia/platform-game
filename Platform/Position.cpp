#include "Position.hpp"
#include "settings.h"

Position::Position(int x, int y) {
	setX(x);
	setY(y);
}

/*
	Imposta x (rispettando i limiti)
*/
void Position::setX(int x) {
	if (x > GAME_WIDTH) {
		x = GAME_WIDTH;
	}
	if (x < 1) {
		x = 1;
	}
	this->x = x;
}

/*
	Imposta y (rispettando i limiti)
*/
void Position::setY(int y) {
	if (y > GAME_HEIGHT) {
		y = GAME_HEIGHT;
	}
	if (y < 1) {
		y = 1;
	} 
	this->y = y;
}

int Position::getX() {
	return this->x;
}

int Position::getY() {
	return this->y;
}

/*
	Prende in input un oggetto Position
	Restituisce true se il parametro è uguale all'oggetto su cui è chiamato
*/
bool Position::equals(Position p) {
	return this->x == p.x && this->y == p.y;
}
