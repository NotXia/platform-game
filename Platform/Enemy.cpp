#include "Enemy.hpp"
#include <cstdlib>

Enemy::Enemy(int health, int points, int min_money, int max_money, Pixel head_left, Pixel head_right, Pixel body, Weapon *weapon, Position position, int visualRange) : Entity(health, head_left, head_right, body, position) {
	this->weapon = weapon;
	this->points = points;
	this->money = rand() % (max_money-min_money+1) + min_money;
	this->weapon_loop_counter = 0;
	lastPlayerPosition = NULL;
	this->visualRange = visualRange;
}
