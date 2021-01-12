#include "Entity.hpp"
#include "settings.h"

Entity::Entity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position) : Block(body, position) {
	this->health = health;
	this->max_health = health;
	this->head_left = head_left;
	this->head_right = head_right;
	this->direction = DIRECTION_LEFT;
	this->can_move = true;

	this->is_jumping = false;
	this->jump_status = 0;
	this->max_jump_height = JUMP_HEIGHT;
	this->jump_animation = AnimationTimer(JUMP_SPEED);
	this->on_terrain = true;
	this->fall_animation = AnimationTimer(FALL_SPEED);
}

int Entity::getHealth() {
	return health;
}


Pixel Entity::getHead() {
	if (direction == DIRECTION_LEFT) {
		return head_left;
	}
	else {
		return head_right;
	}
}


bool Entity::getDirection() {
	return direction;
}

Position Entity::getHeadPosition() {
	Position head_position = position;
	head_position.setY(position.getY()-1);
	return head_position;
}

void Entity::setPosition(Position position) {
	this->position = position;
}

// Restituisce le coordinate davanti alla testa (stabilito in base alla direzione)
Position Entity::getHeadFrontPosition() {
	Position front = getHeadPosition();

	if (direction == DIRECTION_LEFT) {
		front.setX(front.getX()-1);
	}
	else {
		front.setX(front.getX()+1);
	}

	return front;
}

// Restituisce le coordinate davanti al corpo (stabilito in base alla direzione)
Position Entity::getBodyFrontPosition() {
	Position front = position;

	if (direction == DIRECTION_LEFT) {
		front.setX(front.getX()-1);
	}
	else {
		front.setX(front.getX()+1);
	}

	return front;
}

// Restituisce le coordinate dietro al corpo (stabilito in base alla direzione)
Position Entity::getBackPosition() {
	Position back = position;

	if (direction == DIRECTION_LEFT) {
		back.setX(back.getX()+1);
	}
	else {
		back.setX(back.getX()-1);
	}

	return back;
}

Position Entity::getBelowPosition() {
	Position below = position;
	below.setY(below.getY()+1);

	return below;
}

void Entity::setCanMove(bool can_move) {
	this->can_move = can_move;
}

bool Entity::getCanMove() {
	return can_move;
}

bool Entity::isOnTerrain() {
	return on_terrain;
}

void Entity::setOnTerrain(bool on_terrain) {
	this->on_terrain = on_terrain;
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
	Se il valore di attuale direction è DIRECTION_LEFT, aggiorna position decrementando di 1 la posizione sull'asse X
	Aggiorna direction a DIRECTION_LEFT.
*/
void Entity::goLeft() {
	if (direction == DIRECTION_LEFT) {
		position.setX(position.getX()-1);
	}
	direction = DIRECTION_LEFT;
}

/*
	Se il valore di attuale direction è DIRECTION_RIGHT, aggiorna position incrementando di 1 la posizione sull'asse X
	Aggiorna direction a DIRECTION_RIGHT.
*/
void Entity::goRight() {
	if (direction == DIRECTION_RIGHT) {
		position.setX(position.getX()+1);
	}
	direction = DIRECTION_RIGHT;
}

/*
	Restituisce la quantità di vita mancante rispetto al massimo
*/
int Entity::getMissingHp() {
	return max_health - health;
}

/*
	Restituisce la quantità di vita sottoforma di percentuale
*/
int Entity::percHealth() {
	return (100 * health) / max_health;
}

/*
	Prende in input un oggetto Position.
	Restituisce true se la posizione presa in input coincide con una parte dell'entità
*/
bool Entity::existsAt(Position position) {
	return getBodyPosition().equals(position) || getHeadPosition().equals(position);
}


/**************************
   INIZIO GESTIONE SALTO
**************************/

bool Entity::isJumping() {
	return is_jumping;
}

/*
	Inizializza i parametri per il salto dell'entità
*/
void Entity::initJump() {
	is_jumping = true;
	on_terrain = false;
	jump_status = 0;
}

/*
	Se jump_status è inferiore all'altezza massima di salto, aggiorna position decrementando di 1 la posizione sull'asse Y.
	Altrimenti interrompe il salto.
*/
void Entity::jump() {
	jump_status++;
	if (jump_status < max_jump_height) {
		is_jumping = true;
		position.setY(position.getY()-1);
	}
	else {
		stopJump();
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
	Se is_jumping è true: incrementa jump_loop_counter di 1.
	Se supera il limite impostato, viene resettato.
*/
void Entity::incJumpLoopCounter() {
	if (is_jumping) {
		jump_animation.incTimer();
	}
}

/*
	Restituisce true quando è possibile avanzare nell'animazione del salto
*/
bool Entity::canJump() {
	return (jump_animation.limit() && is_jumping);
}

/* FINE GESTIONE SALTO
************************/


/***************************
   INIZIO GESTIONE CADUTA
***************************/

/*
	Se on_terrain è false: incrementa fall_loop_counter di 1.
	Se supera il limite impostato, viene resettato.
*/
void Entity::incFallLoopCounter() {
	if (!on_terrain) {
		fall_animation.incTimer();
	}
}

/*
	Aggiorna position incrementando di 1 la posizione sull'asse Y.
*/
void Entity::fall() {
	position.setY(position.getY()+1);
}

/*
	Restituisce true quando è possibile avanzare nell'animazione della caduta
*/
bool Entity::canFall() {
	return (fall_animation.limit() && !on_terrain && !is_jumping);
}

/* FINE GESTIONE CADUTA
************************/


/*
	Incrementa i vari contatori
*/
void Entity::incCounters() {
	incFallLoopCounter();
	incJumpLoopCounter();
}

