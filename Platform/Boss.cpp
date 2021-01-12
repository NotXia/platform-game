#include "Boss.hpp"
#include <cstdlib>
#include "Map.hpp"

Boss::Boss(int health, int points, int money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon, int type, int max_phase, int ability_num, int ability_max, int down_time, int jump_force) :
	Enemy(health, points, money, head_left, head_right, body, position, weapon) {
	this->type = type;
	this->phase = 0;
	this->max_phase = max_phase;
	this->ability_num = ability_num;
	this->ability_max = ability_max;
	this->down_time = AnimationTimer(down_time);

	default_visualRange = GAME_WIDTH;
	visualRange = GAME_WIDTH;
	max_jump_height = jump_force;
}

Position Boss::getBody2Position() {
	Position out = Enemy::getBodyPosition();
	out.setY(out.getY()-1);
	return out;
}

Position Boss::getHeadPosition() {
	Position out = Enemy::getHeadPosition();
	out.setY(out.getY()-1);
	return out;
}

bool Boss::getType() {
	return type;
}

int Boss::getAbilityNum() {
	return ability_num;
}
void Boss::setAbilityNum(int num_enemies) {
	if (num_enemies > ability_max) {
		num_enemies = ability_max;
	}
	this->ability_num = num_enemies;
}

int Boss::getPhase() {
	return phase;
}

/*
	Prende in input un oggetto Position.
	Restituisce true se il boss è presente in quella posizione
*/
bool Boss::existsAt(Position position) {
	return getHeadPosition().equals(position) || getBodyPosition().equals(position) || getBody2Position().equals(position);
}

/*
	Incrementa phase di 1.
	Se supera max_phase, azzera phase.
*/
void Boss::nextPhase() {
	phase++;
	if (phase > max_phase) {
		phase = 0;
	}
}

/*
	Prende in input un oggetto Map e un oggetto Player.
	Restituisce il codice dell'azione da eseguire
*/
int Boss::getAction(Map *map, Player player) {
	int action_code = ACTION_DO_NOTHING;
	int weapon_range = 0;

	if (type == BOSS_TYPE1) {
		if (phase == 0) {
			if (isOnTerrain()) {
				action_code = ACTION_ATTACK;
				nextPhase();
				setAbilityNum(ability_num+1);
			}
		}
		else if (phase == 1) {
			if (!map->isSolidAt(getBodyFrontPosition())) {
				if (direction == DIRECTION_LEFT) {
					action_code = ACTION_GO_LEFT;
				}
				else {
					action_code = ACTION_GO_RIGHT;
				}
			}
			else if (map->isSolidAt(getBodyFrontPosition())) {
				if (direction == DIRECTION_LEFT) {
					action_code = ACTION_GO_RIGHT;
				}
				else {
					action_code = ACTION_GO_LEFT;
				}
			}
		}
		else if (phase == 2) {
			action_code = ACTION_FALL;
			down_time.reset();
			nextPhase();
		}
		else if (phase == 3) {
			action_code = ACTION_DO_NOTHING;
			down_time.incTimer();
			if (down_time.limit()) {
				nextPhase();
			}
		}
		else if (phase == 4) {
			action_code = ACTION_JUMP;
			nextPhase();
		}
	}

	return action_code;
}

/*
	Incrementa i vari contatori per le animazioni
*/
void Boss::incCounters() {
	Enemy::incCounters();
}
