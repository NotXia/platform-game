#include "Weapon.hpp"
#include "Entity.hpp"

Weapon::Weapon(Pixel left, Pixel right, Bullet bullet) {
	this->textureLeft = left;
	this->textureRight = right;
	this->bullet = bullet;
}

Pixel Weapon::getTexture(bool direction) {
	if (direction == DIRECTION_LEFT) {
		return textureLeft;
	} else {
		return textureRight;
	}
}