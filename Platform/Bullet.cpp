#include "Bullet.hpp"

Bullet::Bullet(Pixel texture, int damage, int speed, int range) {
	this->texture = texture;
	this->damage = damage;
	this->speed = speed;
	this->range = range;
}

Pixel Bullet::getTexture() {
	return texture;
}
int Bullet::getDamage() {
	return damage;
}
int Bullet::getSpeed() {
	return speed;
}
int Bullet::getRange() {
	return range;
}