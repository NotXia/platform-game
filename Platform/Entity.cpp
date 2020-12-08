#include "Entity.hpp"

Entity::Entity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position) {
	this->health = health;
	this->head_left = head_left;
	this->head_right = head_right;
	this->body = body;
	this->position = position;
}

int Entity::getHealth() {
	return health;
}
Pixel Entity::getHeadLeft() {
	return head_left;
}
Pixel Entity::getHeadRight() {
	return head_right;
}
Pixel Entity::getBody() {
	return body;
}
Position Entity::getPosition() {
	return position;
}
void Entity::setPosition(Position position) {
	this->position = position;
}

/*
	Prende in input un intero
	Decrementa health di quel valore
*/
void Entity::take_damage(int damage) {
	health -= damage;
}

/*
	Restituisce true se la vita è minore o uguale a zero, false altrimenti
*/
bool Entity::isDead() {
	return (health <= 0);
}

/*
	Aggiorna position, decrementando di 1 la posizione sull'asse X
*/
void Entity::goLeft() {
	position.setX(position.getX()-1);
}

/*
	Aggiorna position, incrementando di 1 la posizione sull'asse X
*/
void Entity::goRight() {
	position.setX(position.getX()+1);
}

