#include "Boss.hpp"
#include <cstdlib>

Boss::Boss(int difficulty) {
	// Enemy(int health=0, int points=0, int money=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon weapon=Weapon());
	type = BOSS_TYPE1;
	phase = 0;

	if (type == BOSS_TYPE1) {
		/*
			Fase 0: Evoca i nemici
			Fase 1: Va avanti e indietro
			Fase 2: Nemici sconfitti -> Inizio timer e cade
			Fase 3: Caduto
			Fase 4: Timer terminato -> Salta
		*/
		health = 100 + int(100*0.25)*(difficulty-1);
		max_health = health;
		points = BOSS_BASE_POINTS * difficulty;
		money = BOSS_BASE_MONEY * difficulty;
		head_left = Pixel('<', FG_DARKRED , BACKGROUND_DEFAULT, true);
		head_right = Pixel('>', FG_DARKRED, BACKGROUND_DEFAULT, true);
		body = Pixel(char(219), FG_DARKRED, BACKGROUND_DEFAULT, true);
		position = Position(GAME_WIDTH-(GAME_WIDTH*0.25), 1);
		weapon = Weapon();
		default_visualRange = GAME_WIDTH;
		visualRange = GAME_WIDTH;
		num_enemies = 1;
		max_enemies = 6;
		max_phase = 4;
		down_time = AnimationTimer(8);
		max_jump_height = GAME_HEIGHT;
	}
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

int Boss::getNumEnemies() {
	return num_enemies;
}
void Boss::setNumEnemies(int num_enemies) {
	if (num_enemies > max_enemies) {
		num_enemies = max_enemies;
	}
	this->num_enemies = num_enemies;
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
	Prende in input una matrice di Pixel.
	Inizializza la matrice con la mappa del boss.
*/
void Boss::initTerrain(Pixel terrain[GAME_WIDTH][GAME_HEIGHT]) {
	if (type == BOSS_TYPE1) {
		terrain_type1(terrain);
	}
}

void Boss::terrain_type1(Pixel terrain[GAME_WIDTH][GAME_HEIGHT]) {
	const Pixel PLATFORM_TEXTURE = Pixel(char(219), PLATFORM_COLOR_FG, BG_LIGHTGREY, true);
	const Pixel TERRAIN_TEXTURE = Pixel(' ', 0, BG_GREY, true);
	const Pixel SKY_TEXTURE = Pixel(' ', 0, BG_LIGHTGREY, false);

	// Generazione "pavimento"
	for (int i=0; i<TERRAIN_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = TERRAIN_TEXTURE;
		}
	}

	// Generazione cielo
	for (int i=TERRAIN_HEIGHT; i<GAME_HEIGHT; i++) {
		for (int j=0; j<GAME_WIDTH; j++) {
			terrain[j][GAME_HEIGHT-1-i] = SKY_TEXTURE;
		}
	}

	// Generazione piattaforme
	for (int i=0; i<GAME_WIDTH; i++) {
		terrain[i][4] = PLATFORM_TEXTURE;
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
				setNumEnemies(num_enemies+1);
			}
		}
		else if (phase == 1) {
			if (direction == DIRECTION_LEFT && getBodyPosition().getX() > 1) {
				action_code = ACTION_GO_LEFT;
			}
			else if (direction == DIRECTION_LEFT && getBodyPosition().getX() == 1) {
				action_code = ACTION_GO_RIGHT;
			}
			else if (direction == DIRECTION_RIGHT && getBodyPosition().getX() < GAME_WIDTH) {
				action_code = ACTION_GO_RIGHT;
			}
			else if (direction == DIRECTION_RIGHT && getBodyPosition().getX() == GAME_WIDTH) {
				action_code = ACTION_GO_LEFT;
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
