#include "Enemy.hpp"
#include <cstdlib>

Enemy::Enemy(int health, int points, int min_money, int max_money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon *weapon, int visualRange) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = points;
	this->money = rand() % (max_money-min_money+1) + min_money;
	lastPlayerPosition = NULL;
	this->visualRange = visualRange;
}
