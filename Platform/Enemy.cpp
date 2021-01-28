#include "Enemy.hpp"
#include "Map.hpp"
#include "colors.h"

const Pixel ANGRY_HEAD_LEFT_TEXTURE = Pixel(ENEMY_HEAD_LEFT, ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel ANGRY_HEAD_RIGHT_TEXTURE = Pixel(ENEMY_HEAD_RIGHT, ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel ANGRY_BODY_TEXTURE = Pixel(ENEMY_BODY, ANGRY_ENEMY_BODY_COLOR_FG, BACKGROUND_DEFAULT, true);

const Pixel ENEMY_HEAD_LEFT_TEXTURE = Pixel(ENEMY_HEAD_LEFT, ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel ENEMY_HEAD_RIGHT_TEXTURE = Pixel(ENEMY_HEAD_RIGHT, ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel ENEMY_BODY_TEXTURE = Pixel(ENEMY_BODY, ENEMY_BODY_COLOR_FG, BACKGROUND_DEFAULT, true);

Enemy::Enemy(int health, int points, int money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon) : ArmedEntity(health, head_left, head_right, body, position, weapon) {
	this->points = points;
	this->money = money;
	lastPlayerPosition = NULL;
	this->default_visualRange = ENEMY_RANGE;
	this->visualRange = ENEMY_RANGE;
	this->refresh = AnimationTimer(ENEMY_REFRESH_RATE);
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
	Indica se è possibile eseguire la prossima azione.
*/
bool Enemy::canRefresh() {
	return refresh.limit();
}

/*
	Prende in input un oggetto Player.
	Aggiorna lastPlayerPosition se la posizione del player è nel campo visivo.
*/
void Enemy::searchForPlayer(Player player) {
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

		head_left = ANGRY_HEAD_LEFT_TEXTURE;
		head_right = ANGRY_HEAD_RIGHT_TEXTURE;
		body = ANGRY_BODY_TEXTURE;
	}
	else {
		visualRange = default_visualRange;

		head_left = ENEMY_HEAD_LEFT_TEXTURE;
		head_right = ENEMY_HEAD_RIGHT_TEXTURE;
		body = ENEMY_BODY_TEXTURE;
	}
}

/*
	Prende in input un oggetto Map e Player.
	Restituisce l'azione da eseguire.
*/
int Enemy::getAction(Map *map, Player player) {
	int action_code = ACTION_DO_NOTHING;
	int weapon_range = weapon.getRange();

	if (lastPlayerPosition != NULL) { // Il nemico ha visto il giocatore almeno una volta
		/*
			Priorità azioni:
			Attacco (se l'ultima posizione nota è effettivamente il giocatore) -> Salto -> Caduta -> Movimento -> Salto (azione di default)
		*/
		if (player.getBodyPosition().equals(*lastPlayerPosition) &&             // L'ultima posizione nota è quella effettiva del player
			lastPlayerPosition->getX() > this->position.getX() &&				// Player a destra
			lastPlayerPosition->getX() <= this->position.getX()+weapon_range && // Player nel raggio dell'arma
			lastPlayerPosition->getY() == this->position.getY() &&				// Player alla stessa altezza
			direction == DIRECTION_RIGHT) {										// Rivolto verso il giocatore
			action_code = ACTION_ATTACK;
		}
		else if (player.getBodyPosition().equals(*lastPlayerPosition) &&				// L'ultima posizione nota è quella effettiva del player
					lastPlayerPosition->getX() < this->position.getX() &&				// Player a sinistra
					lastPlayerPosition->getX() >= this->position.getX()-weapon_range && // Player nel raggio dell'arma
					lastPlayerPosition->getY() == this->position.getY() &&				// Player alla stessa altezza
					direction == DIRECTION_LEFT) {										// Rivolto verso il giocatore
			action_code = ACTION_ATTACK;
		}
		else if (lastPlayerPosition->getY() < this->position.getY() &&						 // Player sopra
				 map->isSolidAt(Position(this->position.getX(), this->position.getY()-2))) { // Blocco solido sopra
			action_code = ACTION_JUMP;
		}
		else if (lastPlayerPosition->getY() > this->position.getY() &&								// Player sotto
				!map->isLava(Position(getBelowPosition().getX(), getBelowPosition().getY()+3)) &&   // Non c'è lava sotto
				!map->isLava(Position(getBelowPosition().getX()+1, getBelowPosition().getY()+3)) && // Non c'è lava sotto a dx
				!map->isLava(Position(getBelowPosition().getX()-1, getBelowPosition().getY()+3))) { // Non c'è lava sotto a sx
			action_code = ACTION_FALL;
		}
		else if ((lastPlayerPosition->getX() > this->position.getX() ||										  // Player a destra
				(lastPlayerPosition->getX() > this->position.getX() && this->direction == DIRECTION_LEFT)) && // Player a destra e nemico rivolto a sinistra
				!map->isLava(Position(getBelowPosition().getX()+1, getBelowPosition().getY())) ) {            // Non c'è lava a destra
			action_code = ACTION_GO_RIGHT;
		}
		else if ((lastPlayerPosition->getX() <= this->position.getX() ||										// Player a sinistra
				(lastPlayerPosition->getX() <= this->position.getX() && this->direction == DIRECTION_RIGHT)) && // Player a sinistra e nemico rivolto a destra
				!map->isLava(Position(getBelowPosition().getX()-1, getBelowPosition().getY())) ) {				// Non c'è lava a sinistra
			action_code = ACTION_GO_LEFT;
		}
		else {
			action_code = ACTION_JUMP;
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

		/* Se il movimento dovesse portare il nemico sulla lava, non fa nulla */
		if (action_code == ACTION_GO_RIGHT && map->isLava(Position(getBelowPosition().getX()+1, getBelowPosition().getY()))) {
			action_code = ACTION_DO_NOTHING;
		}
		else if (action_code == ACTION_GO_LEFT && map->isLava(Position(getBelowPosition().getX()-1, getBelowPosition().getY()))) {
			action_code = ACTION_DO_NOTHING;
		}
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
	Incrementa i vari contatori.
*/
void Enemy::incCounters() {
	ArmedEntity::incCounters();
	refresh.incTimer();
}
