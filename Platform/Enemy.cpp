#include "Enemy.hpp"

Enemy::Enemy(int health, int points, int money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon *weapon, int visualRange) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = points;
	this->money = money;
	lastPlayerPosition = NULL;
	this->visualRange = visualRange;
	this->refreshTime = 7000;
	this->currRefresh = 0;
}

/*
	Indica se è possibile eseguire la prossima azione (currRefresh ha raggiunto refreshTime)
*/
bool Enemy::canRefresh() {
	return currRefresh == refreshTime;
}

/*
	Incrementa currRefresh di 1. Se supera refreshTime, viene azzerato
*/
void Enemy::incRefresh() {
	currRefresh++;
	if (currRefresh > refreshTime) {
		currRefresh = 0;
	}
}