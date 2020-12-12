#include "Player.hpp"
#include "settings.h"

Player::Player(int health, Pixel head_left, Pixel head_right, Pixel body, Weapon *weapon, Position position) : Entity(health, head_left, head_right, body, position) {
	this->weapon = weapon;
	this->points = 0;
	this->money = 0;
	this->weapon_loop_counter = 0;
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

Weapon *Player::getWeapon() {
	return this->weapon;
}

void Player::attack() {
	setCanMove(false);
}

/*
	Incrementa weapon_loop_counter di 1.
	Se supera il limite, viene resettato.
*/
void Player::incWeaponLoop() {
	weapon_loop_counter++;
	if (weapon_loop_counter > WEAPON_DISPLAY_TIME) {
		resetWeaponLoop();
	}
}

/*
	Azzera weapon_loop_counter
*/
void Player::resetWeaponLoop() {
	weapon_loop_counter = 0;
}

/*
	Indica se terminare la visualizzazione dell'arma quando il giocatore attacca
*/
bool Player::endWeaponDisplay() {
	return (weapon_loop_counter >= WEAPON_DISPLAY_TIME);
}