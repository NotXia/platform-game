#include "Enemy.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "colors.h"

Enemy::Enemy(int health, int points, int money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon *weapon) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = points;
	this->money = money;
	lastPlayerPosition = NULL;
	this->visualRange = ENEMY_RANGE;
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

/*
	Prende in input un oggetto Player
	Aggiorna lastPlayerPosition se la posizione del player è nel campo visivo
*/
void Enemy::search4Player(Player player) {
	Position player_pos = player.getBodyPosition();
	bool foundInX = false, foundInY = false;

	if (player_pos.getX() >= this->position.getX()-visualRange && player_pos.getX() <= this->position.getX()+visualRange) {
		foundInX = true;
	}
	if (player_pos.getY() >= this->position.getY()-visualRange && player_pos.getY() <= this->position.getY()+visualRange) {
		foundInY = true;
	}

	if(foundInX && foundInY) {
		delete lastPlayerPosition;
		lastPlayerPosition = new Position(player_pos.getX(), player_pos.getY());

		visualRange = 2 * ENEMY_RANGE;

		head_left = Pixel(head_left.getValue(), ANGRY_ENEMY_HEAD_COLOR, true);
		head_right = Pixel(head_right.getValue(), ANGRY_ENEMY_HEAD_COLOR, true);
		body = Pixel(body.getValue(), ANGRY_ENEMY_HEAD_COLOR, true);
	}
	else {
		visualRange = ENEMY_RANGE;

		head_left = Pixel(head_left.getValue(), ENEMY_HEAD_COLOR, true);
		head_right = Pixel(head_right.getValue(), ENEMY_HEAD_COLOR, true);
		body = Pixel(body.getValue(), ENEMY_HEAD_COLOR, true);
	}
}

/*
	Prende in input un oggetto Map e Player
	Restituisce l'azione da eseguire
*/
int Enemy::getAction(Map *map, Player player) {
	int action_code = ACTION_DO_NOTHING;
	int weapon_range;
	if (weapon == NULL) {
		weapon_range = 0;
	}
	else {
		weapon_range = weapon->getRange();
	}

	if (lastPlayerPosition != NULL) {
		if (player.getBodyPosition().equals(*lastPlayerPosition)) {
			if (lastPlayerPosition->getX() > this->position.getX()+weapon_range) {
				action_code = ACTION_GO_RIGHT;
			}
			else if (lastPlayerPosition->getX() < this->position.getX()-weapon_range) {
				action_code = ACTION_GO_LEFT;
			}
			else {
				action_code = ACTION_ATTACK;
			}
		}
		else {
			if (lastPlayerPosition->getX() > this->position.getX()) {
				action_code = ACTION_GO_RIGHT;
			}
			else {
				action_code = ACTION_GO_LEFT;
			}
		}

		if (lastPlayerPosition->getY() < this->position.getY()) {
			if (map->isSolidAt(Position(this->position.getX(), this->position.getY()-2))) {
				action_code = ACTION_JUMP;
			}
			else if (lastPlayerPosition->getX() > this->position.getX()) {
				action_code = ACTION_GO_RIGHT;
			}
			else {
				action_code = ACTION_GO_LEFT;
			}
		}
		else if (lastPlayerPosition->getY() > this->position.getY()) {
			action_code = ACTION_FALL;
		}
	}
	else {
		if (!map->isSolidAt(Position(position.getX()+1, position.getY()+1))) {
			if (rand() % 2 == 0) {
				action_code = ACTION_GO_LEFT;
			}
			else {
				action_code = ACTION_DO_NOTHING;
			}
		}
		else if (!map->isSolidAt(Position(position.getX()-1, position.getY()+1))) {
			if (rand() % 2 == 0) {
				action_code = ACTION_GO_RIGHT;
			}
			else {
				action_code = ACTION_DO_NOTHING;
			}
		}
		else {
			int rand_action = rand() % 3;
			if (rand_action == 0) {
				action_code = ACTION_GO_RIGHT;
			}
			else if (rand_action == 1) {
				action_code = ACTION_GO_LEFT;
			}
			else {
				action_code = ACTION_DO_NOTHING;
			}
		}
	}
	return action_code;
}