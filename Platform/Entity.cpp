#include "Entity.hpp"
#include "settings.h"

Entity::Entity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position) {
	this->health = health;
	this->head_left = head_left;
	this->head_right = head_right;
	this->body = body;
	this->position = position;
	this->jump_status = 0;

	this->direction = DIRECTION_RIGHT;
	this->is_jumping = false;
	this->can_jump = true;
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
bool Entity::getDirection() {
	return direction;
}
Position Entity::getBodyPosition() {
	return position;
}
Position Entity::getHeadPosition() {
	Position head_position = position;
	head_position.setY(position.getY()-1);
	return head_position;
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
	direction = DIRECTION_LEFT;
}

/*
	Aggiorna position, incrementando di 1 la posizione sull'asse X
*/
void Entity::goRight() {
	position.setX(position.getX()+1);
	direction = DIRECTION_RIGHT;
}

/*
	Inizializza i parametri per il salto dell'entità
*/
void Entity::initJump() {
	is_jumping = true;
	can_jump = false;
	jump_status = 0;
}

bool Entity::getCanJump() {
	return can_jump;
}

void Entity::setCanJump(bool can_jump) {
	this->can_jump = can_jump;
}

bool Entity::isJumping() {
	return is_jumping;
}

/*
	Se jump_status è inferiore all'altezza massima di salto, aggiorna position decrementando di 1 la posizione sull'asse Y.
	Altrimenti interrompe il salto.
*/
bool Entity::jump() {
	jump_status++;
	if (jump_status < JUMP_HEIGHT) {
		is_jumping = true;
		position.setY(position.getY()-1);
		return true;
	}
	else {
		stopJump();
		return false;
	}
}

/*
	Imposta i parametri di salto dell'entità in modo da interromperlo
*/
void Entity::stopJump() {
	jump_status = 0;
	is_jumping = false;
}

/*
	Aggiorna position incrementando di 1 la posizione sull'asse Y.
*/
void Entity::fall() {
	position.setY(position.getY()+1);
}
