#include "Player.hpp"
#include "settings.h"

Player::Player(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = 0;
	this->money = 0;
}

int Player::getPoints() {
	return points;
}

int Player::getMoney() {
	return money;
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

/*
	Richiama la funzione attack() della superclasse e imposta il campo hostile dell'oggetto Bullet restituito a false.
	Restituisce tale oggetto.
*/
Bullet Player::attack() {
	Bullet bullet = ArmedEntity::attack();
	bullet.setHostile(false);
	return bullet;
}

/*
	Restituisce la quantità di vita mancante rispetto al massimo
*/
int Player::getMissingHp() {
	return MAX_LIFE - health;
}