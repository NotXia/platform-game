#include "Weapon.hpp"
#include "Entity.hpp"

Weapon::Weapon(Pixel left, Pixel right, Bullet bullet, int range) {
	this->textureLeft = left;
	this->textureRight = right;
	this->bullet = bullet;
	this->range = range;
}

Pixel Weapon::getTexture(bool direction) {
	if (direction == DIRECTION_LEFT) {
		return textureLeft;
	} else {
		return textureRight;
	}
}

int Weapon::getRange() {
	return this->range;
}