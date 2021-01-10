#include "Enemy.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "colors.h"

Enemy::Enemy(int health, int points, int money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = points;
	this->money = money;
	lastPlayerPosition = NULL;
	this->default_visualRange = ENEMY_RANGE;
	this->visualRange = ENEMY_RANGE;
	this->refresh = AnimationTimer(8000);
}

int Enemy::getPoints() {
	return points;
}

int Enemy::getMoney() {
	return money;
}

void Enemy::setMoney(int money) {
	this->money = money;
}

/*
	Indica se è possibile eseguire la prossima azione (currRefresh ha raggiunto refreshTime)
*/
bool Enemy::canRefresh() {
	return refresh.limit();
}

/*
	Incrementa currRefresh di 1. Se supera refreshTime, viene azzerato
*/
void Enemy::incRefresh() {
	refresh.incTimer();
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

		visualRange = 2 * default_visualRange;

		head_left = Pixel(head_left.getValue(), ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
		head_right = Pixel(head_right.getValue(), ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
		body = Pixel(body.getValue(), ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
	}
	else {
		visualRange = default_visualRange;

		head_left = Pixel(head_left.getValue(), ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
		head_right = Pixel(head_right.getValue(), ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
		body = Pixel(body.getValue(), ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
	}
}

/*
	Prende in input un oggetto Map e Player
	Restituisce l'azione da eseguire
*/
int Enemy::getAction(Map *map, Player player) {
	int action_code = ACTION_DO_NOTHING;
	int weapon_range;
	weapon_range = weapon.getRange();
	

	if (lastPlayerPosition != NULL) { // Il nemico ha visto il giocatore almeno una volta
		if (player.getBodyPosition().equals(*lastPlayerPosition)) {
			/*
				Se l'ultima posizione nota è effettivamente il giocatore:
				- Si avvicina entro il raggio d'azione dell'arma
				- Attacca
			*/
			if (lastPlayerPosition->getX() > this->position.getX()+weapon_range) {
				action_code = ACTION_GO_RIGHT;
			}
			else if (lastPlayerPosition->getX() < this->position.getX()-weapon_range) {
				action_code = ACTION_GO_LEFT;
			}
			else if (lastPlayerPosition->getX() > this->position.getX() && direction == DIRECTION_LEFT) {
				action_code = ACTION_GO_RIGHT;
			}
			else  if (lastPlayerPosition->getX() < this->position.getX() && direction == DIRECTION_RIGHT) {
				action_code = ACTION_GO_LEFT;
			}
			else {
				action_code = ACTION_ATTACK;
			}
		}
		else {
			/*
				Se l'ultima posizione nota non è quella attuale del giocatore:
				- Si avvicina
			*/
			if (lastPlayerPosition->getX() > this->position.getX()) {
				action_code = ACTION_GO_RIGHT;
			}
			else {
				action_code = ACTION_GO_LEFT;
			}
		}

		if (lastPlayerPosition->getY() < this->position.getY()) {
			/*
				Se il giocatore si trova più in alto del nemico:
				- Annulla l'azione precedentemente calcolata
				- Salta se sopra c'è una piattaforma
				- Si avvicina al nemico rispetto all'asse X se non può saltare
			*/
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
			/*
				Se il giocatore si trova più in basso del nemico:
				- Annulla l'azione precedentemente calcolata
				- Scende dalla piattaforma su cui è attualmente
			*/
			action_code = ACTION_FALL;
		}
	}
	else { // Il nemico non ha mai visto il giocatore
		/*
			Esegue un'azione casuale, rispettando i limiti della mappa
		*/
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

	if (action_code == ACTION_GO_RIGHT && map->isLava(Position(getBelowPosition().getX()+1, getBelowPosition().getY()))) {
		action_code = ACTION_DO_NOTHING;
	}
	else if (action_code == ACTION_GO_LEFT && map->isLava(Position(getBelowPosition().getX()-1, getBelowPosition().getY()))) {
		action_code = ACTION_DO_NOTHING;
	}

	return action_code;
}

/*
	Richiama la funzione attack() della superclasse e imposta il campo hostile dell'oggetto Bullet restituito a true.
	Restituisce tale oggetto.
*/
Bullet Enemy::attack() {
	Bullet bullet = ArmedEntity::attack();
	bullet.setHostile(true);
	return bullet;
}

/*
	Incrementa i vari contatori
*/
void Enemy::incCounters() {
	ArmedEntity::incCounters();
	incRefresh();
}
