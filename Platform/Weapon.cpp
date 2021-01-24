#include "Weapon.hpp"
#include "Entity.hpp"
#include <cstring>

Weapon::Weapon(const char name[], Pixel left, Pixel right, Bullet bullet, int ammo, int reloadDelay, int shootDelay, bool type, int range) {
	strncpy(this->name, name, STRING_LEN);
	this->textureLeft = left;
	this->textureRight = right;
	this->bullet = bullet;
	
	this->ammo = ammo;
	this->curr_ammo = ammo;

	this->reloading = false;
	this->reload_delay = AnimationTimer(reloadDelay);

	this->shooting = false;
	this->shoot_delay = AnimationTimer(shootDelay);

	this->type = type;
	this->range = range;
}

// Restituisce la texture in base alla direzione.
Pixel Weapon::getTexture(bool direction) { 
	if (direction == DIRECTION_LEFT) {
		return textureLeft;
	} else {
		return textureRight;
	}
}

void Weapon::getName(char name[]) {
	strncpy(name, this->name, STRING_LEN);
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

bool Weapon::getType() {
	return type;
}

int Weapon::getRange() {
	return range;
}

void Weapon::setAmmo(int ammo) {
	this->ammo = ammo;
}

/*
	Restituisce l'oggetto Bullet associato all'arma.
	Se si tratta di un'arma a distanza, viene impostata la distanza di percorrenza del proiettile uguale a range.
	Se si tratta di un'arma corpo a corpo, viene impostata la distanza di percorrenza del proiettile a 1.
*/
Bullet Weapon::getBullet() {
	Bullet out = bullet;
	if (type == WEAPON_MELEE) {
		out.setRange(1);
		return out;
	}
	else {
		out.setRange(range);
		return out;
	}
}

/*
	Restituisce true se curr_ammo è maggiore di 0, false altrimenti.
*/
bool Weapon::hasAmmo() {
	return curr_ammo > 0;
}



/*****************************
   INIZIO GESTIONE RICARICA
*****************************/

/*
	Inizializza i parametri per gestire il tempo di ricarica.
*/
void Weapon::startReloadDelay() {
	if (!reloading) {
		reloading = true;
		reload_delay.reset();
		curr_ammo = -1;
	}
}

/*
	Incrementa reload_delay di 1.
*/
void Weapon::incReloadDelay() {
	if (reloading) {
		reload_delay.incTimer();
	}
}

/*
	Restituisce true se la ricarica è terminata.
*/
bool Weapon::canEndReloadDelay() {
	return reload_delay.limit() && reloading;
}

/*
	Imposta curr_ammo ad ammo e reloading a false.
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
	Inizializza i parametri per gestire l'attesa tra un colpo e l'altro.
*/
void Weapon::startShootDelay() {
	shooting = true;
	shoot_delay.reset();
	curr_ammo--;
}

/*
	Incrementa shoot_delay di 1.
*/
void Weapon::incShootDelay() {
	if (shooting) {
		shoot_delay.incTimer();
	}
}

/*
	Restituisce true se è finita l'attesa.
*/
bool Weapon::canEndShootDelay() {
	return shoot_delay.limit() && shooting;
}

/*
	Imposta shooting a false.
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
	Serve per le funzioni di confronto.
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

// Confronto danni.
char Weapon::higherDamage(Weapon weapon) {
	return getCheckSymbol(this->bullet.getDamage() - weapon.bullet.getDamage());
}

// Confronto capienza caricatore.
char Weapon::higherAmmo(Weapon weapon) {
	return getCheckSymbol(this->ammo - weapon.ammo);
}

// Contronto distanza di sparo.
char Weapon::higherRange(Weapon weapon) {
	return getCheckSymbol(this->bullet.getRange() - weapon.bullet.getRange());
}

// Confronto velocità ricarica.
char Weapon::fasterReload(Weapon weapon) {
	return getCheckSymbol(weapon.reload_delay.getMaxTimer() - this->reload_delay.getMaxTimer());
}

// Confronto velocità sparo.
char Weapon::fasterShootRate(Weapon weapon) {
	return getCheckSymbol(weapon.shoot_delay.getMaxTimer() - this->shoot_delay.getMaxTimer());
}

/*
	Prende in input un oggetto Weapon.
	Confronta l'oggetto corrente con il parametro e dice se sono uguali.
*/
bool Weapon::equals(Weapon weapon) {
	return this->bullet.equals(weapon.bullet) && this->ammo == weapon.ammo && (strcmp(this->name, weapon.name) == 0);
}

/* FINE GESTIONE CONFRONTO
***************************/