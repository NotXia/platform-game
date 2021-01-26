#include "ArmedEntity.hpp"
#include "settings.h"

ArmedEntity::ArmedEntity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon) : Entity(health, head_left, head_right, body, position) {
	this->weapon = weapon;
	mapEventsTimer = AnimationTimer(MAP_EVENT_SPEED);
	mapEvent = false;
}

Weapon ArmedEntity::getWeapon() {
	return this->weapon;
}

void ArmedEntity::setWeapon(Weapon weapon) {
	this->weapon = weapon;
}


/****************************
   INIZIO GESTIONE ATTACCO
****************************/

/*
	Imposta i vari parametri per l'attacco e restituisce un oggetto Bullet identico al Bullet associato all'oggetto weapon.
*/
Bullet ArmedEntity::attack() {
	if (weapon.hasAmmo()) {
		weapon.startShootDelay();

		Bullet out_bullet = weapon.getBullet();
		out_bullet.setPosition(getBodyFrontPosition());
		out_bullet.setDirection(this->direction);

		return out_bullet;
	}
	else {
		return Bullet(Pixel(), 0, 0, direction);
	}

}

/*
	Restituisce true se ci sono le condizioni per attaccare.
*/
bool ArmedEntity::canAttack() {
	return weapon.hasAmmo() && !weapon.isReloading() && !weapon.isShooting();
}

/*
	Se il delay per lo sparo è terminato, permette di sparare nuovamente.
*/
bool ArmedEntity::hasShootDelayFinished() {
	if (weapon.canEndShootDelay()) {
		weapon.endShoot();
		return true;
	}
	return false;
}

/* FINE GESTIONE ATTACCO
*************************/



/*****************************
   INIZIO GESTIONE RICARICA
*****************************/

/*
	Avvia la ricarica dell'arma.
*/
void ArmedEntity::reload() {
	weapon.startReloadDelay();
}

/*
	Restituisce true se ci sono le condizioni per ricaricare.
*/
bool ArmedEntity::canReload() {
	return !weapon.isReloading() && !weapon.isShooting();
}

/*
	Se il delay per la ricarica è terminato, ricarica l'arma.
*/
bool ArmedEntity::hasReloadFinished() {
	if (weapon.canEndReloadDelay()) {
		weapon.endReload();
		return true;
	}
	return false;
}

/* FINE GESTIONE RICARICA
**************************/


/**************************************
   INIZIO GESTIONE EVENTI AMBIENTALI
**************************************/

bool ArmedEntity::isMapEvent() {
	return mapEvent;
}

/*
	Restituisce true se è possibile "valutare" gli eventi ambientali. False altrimenti.
*/
bool ArmedEntity::canMapEvents() {
	return mapEventsTimer.limit();
}

/*
	Imposta i parametri per gestire gli eventi ambientali.
*/
void ArmedEntity::startMapEvent() {
	mapEvent = true;
	mapEventsTimer.reset();
}

/*
	Imposta mapEvent a false.
*/
void ArmedEntity::stopMapEvent() {
	mapEvent = false;
}

/* FINE GESTIONE EVENTI AMBIENTALI
***********************************/


/*
	Incrementa i vari contatori
*/
void ArmedEntity::incCounters() {
	Entity::incCounters();
	weapon.incReloadDelay();
	weapon.incShootDelay();
	mapEventsTimer.incTimer();
}