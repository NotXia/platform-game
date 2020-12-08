#include "Weapon.hpp"

Weapon::Weapon(int range, int damage, Pixel left, Pixel right) {
	this->range = range;
	this->damage = damage;
	this->textureLeft = left;
	this->textureRight = right;
}