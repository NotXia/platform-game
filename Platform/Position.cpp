#include "Position.hpp"
#include "settings.h"

Position::Position(int x, int y) {
	setX(x);
	setY(y);
}

/*
	Imposta x (rispettando i limiti).
*/
void Position::setX(int x) {
	if (x > GAME_WIDTH-1) {
		x = GAME_WIDTH-1;
	}
	if (x < 0) {
		x = 0;
	}
	this->x = x;
}

/*
	Imposta y (rispettando i limiti).
*/
void Position::setY(int y) {
	if (y > GAME_HEIGHT-1) {
		y = GAME_HEIGHT-1;
	}
	if (y < 0) {
		y = 0;
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
	Prende in input un oggetto Position.
	Restituisce true se il parametro è uguale all'oggetto su cui è chiamato.
*/
bool Position::equals(Position p) {
	return this->x == p.x && this->y == p.y;
}
