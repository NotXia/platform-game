#include "Weapon.hpp"
#include "Entity.hpp"
#include <cstring>

Weapon::Weapon(const char name[], Pixel left, Pixel right, Bullet bullet, int ammo, int reloadDelay, int shootDelay) {
	strncpy(this->name, name, STRING_LEN);
	this->textureLeft = left;
	this->textureRight = right;
	this->bullet = bullet;
	
	this->ammo = ammo;
	this->curr_ammo = ammo;

	this->reloading = false;
	this->curr_reloadDelay = 0;
	this->reloadDelay = reloadDelay;


	this->shooting = false;
	this->curr_shootDelay = 0;
	this->shootDelay = shootDelay;
}

Pixel Weapon::getTexture(bool direction) {
	if (direction == DIRECTION_LEFT) {
		return textureLeft;
	} else {
		return textureRight;
	}
}

Bullet Weapon::getBullet() {
	return bullet;
}

char* Weapon::getName() {
	return name;
}

int Weapon::getCurrAmmo() {
	return curr_ammo;
}


bool Weapon::isShooting() {
	return shooting;
}

bool Weapon::isReloading() {
	return reloading;
}

/*
	Restituisce true se curr_ammo è maggiore di 0, false altrimenti
*/
bool Weapon::hasAmmo() {
	return curr_ammo > 0;
}



/*****************************
   INIZIO GESTIONE RICARICA
*****************************/

/*
	Inizializza i parametri per visualizzare il tempo di ricarica
*/
void Weapon::startReloadDelay() {
	if (!reloading) {
		reloading = true;
		curr_reloadDelay = 0;
		curr_ammo = -1;
	}
}

/*
	Incrementa di 1 curr_reloadDelay
*/
void Weapon::incReloadDelay() {
	if (reloading) {
		curr_reloadDelay++;
	}
}

/*
	Restituisce true se curr_reloadDelay ha raggiunto reloadDelay
*/
bool Weapon::canEndReloadDelay() {
	return (curr_reloadDelay >= reloadDelay) && reloading;
}

/*
	Imposta curr_ammo ad ammo e reloading a false
*/
void Weapon::endReload() {
	reloading = false;
	curr_ammo = ammo;
}
/* FINE GESTIONE RICARICA
**************************/



/**************************
   INIZIO GESTIONE SPARO
**************************/

/*
	Inizializza i parametri per visualizzare l'attesa tra un colpo e l'altro
*/
void Weapon::startShootDelay() {
	shooting = true;
	curr_shootDelay = 0;
	curr_ammo--;
}

/*
	Incrementa di 1 curr_shootDelay
*/
void Weapon::incShootDelay() {
	if (shooting) {
		curr_shootDelay++;
	}
}

/*
	Restituisce true se curr_shootDelay ha raggiunto shootDelay
*/
bool Weapon::canEndShootDelay() {
	return curr_shootDelay >= shootDelay && shooting;
}

/*
	Imposta shooting a false
*/
void Weapon::endShoot() {
	shooting = false;
}

/* FINE GESTIONE SPARO
***********************/



/******************************
   INIZIO GESTIONE CONFRONTO
******************************/

/*
	Serve per le funzioni di confronto
	Prende in input un intero.
	Restituisce un carattere in base al valore.
*/
char Weapon::getCheckSymbol(int check) {
	if (check > 0) { 
		return '+'; 
	}
	else if (check == 0) {
		return '='; 
	}
	else {
		return '-'; 
	}
}

char Weapon::higherDamage(Weapon weapon) {
	return getCheckSymbol(this->bullet.getDamage() - weapon.bullet.getDamage());
}

char Weapon::higherAmmo(Weapon weapon) {
	return getCheckSymbol(this->ammo - weapon.ammo);
}

char Weapon::higherRange(Weapon weapon) {
	return getCheckSymbol(this->bullet.getRange() - weapon.bullet.getRange());
}

char Weapon::fasterReload(Weapon weapon) {
	return getCheckSymbol(this->reloadDelay - weapon.reloadDelay);
}

char Weapon::fasterShootRate(Weapon weapon) {
	return getCheckSymbol(weapon.shootDelay - this->shootDelay);
}

/* FINE GESTIONE CONFRONTO
***************************/