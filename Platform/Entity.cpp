#include "Entity.hpp"
#include "settings.h"

Entity::Entity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position) {
	this->health = health;
	this->head_left = head_left;
	this->head_right = head_right;
	this->body = body;
	this->position = position;
	this->direction = DIRECTION_RIGHT;
	this->can_move = true;

	this->jump_status = 0;
	this->is_jumping = false;
	this->on_terrain = true;
	this->jump_loop_counter = 0;
	this->fall_loop_counter = 0;
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

// Restituisce le coordinate davanti al corpo (stabilito in base alla direzione)
Position Entity::getFrontPosition() {
	Position front = position;

	if (direction == DIRECTION_LEFT) {
		front.setX(front.getX()-1);
	}
	else {
		front.setX(front.getX()+1);
	}

	return front;
}

void Entity::setCanMove(bool can_move) {
	this->can_move = can_move;
}

bool Entity::getCanMove() {
	return can_move;
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
	on_terrain = false;
	jump_status = 0;
}

bool Entity::isOnTerrain() {
	return on_terrain;
}

void Entity::setOnTerrain(bool on_terrain) {
	this->on_terrain = on_terrain;
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

/*
	Incrementa jump_loop_counter di 1.
	Se supera il limite impostato, viene resettato.
*/
void Entity::incJumpLoopCounter() {
	jump_loop_counter++;
	if (fall_loop_counter > FALL_SPEED) {
		resetJumpLoopCounter();
	}
}

/*
	Resetta jump_loop_counter
*/
void Entity::resetJumpLoopCounter() {
	jump_loop_counter = 0;
}

/*
	Incrementa fall_loop_counter di 1.
	Se supera il limite impostato, viene resettato.
*/
void Entity::incFallLoopCounter() {
	fall_loop_counter++;
	if (fall_loop_counter > FALL_SPEED) {
		resetFallLoopCounter();
	}
}

/*
	Resetta fall_loop_counter
*/
void Entity::resetFallLoopCounter() {
	fall_loop_counter = 0;
}

/*
	Restituisce true quando è possibile avanzare nell'animazione del salto
*/
bool Entity::canJump() {
	return (jump_loop_counter >= JUMP_SPEED);
}

/*
	Restituisce true quando è possibile avanzare nell'animazione della caduta
*/
bool Entity::canFall() {
	return (fall_loop_counter >= FALL_SPEED);
}