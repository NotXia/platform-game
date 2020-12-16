#include "Player.hpp"
#include "settings.h"

Player::Player(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon *weapon) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = 0;
	this->money = 0;
}

/*
	Prende un input un intero
	Incrementa health di quel valore (non può superare il limite)
*/
void Player::heal(int hp) {
	this->health += hp;
	if (health > MAX_LIFE) {
		health = MAX_LIFE;
	}
}

/*
	Prende un input un intero
	Incrementa points di quel valore
*/
void Player::incPoints(int points) {
	this->points += points;
}

/*
	Prende un input un intero
	Incrementa money di quel valore
*/
void Player::incMoney(int money) {
	this->money += money;

}

/*
	Prende un input un intero
	Decrementa money di quel valore
*/
void Player::decMoney(int money) {
	this->money -= money;

}