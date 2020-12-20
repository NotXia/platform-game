#include "ArmedEntity.hpp"
#include "settings.h"

ArmedEntity::ArmedEntity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon) : Entity(health, head_left, head_right, body, position) {
	this->weapon = weapon;
	is_attacking = false;
	weapon_loop_counter = 0;
}

Weapon ArmedEntity::getWeapon() {
	return this->weapon;
}

/*
	Restituisce un oggetto Bullet identico al Bullet associato all'oggetto weapon
*/
Bullet ArmedEntity::attack() {
	setCanMove(false);
	resetWeaponLoop();
	is_attacking = true;

	Bullet out_bullet = weapon.getBullet();
	out_bullet.setPosition(getFrontPosition());
	out_bullet.setDirection(this->direction);

	return out_bullet;
}

/*
	Incrementa weapon_loop_counter di 1.
	Se supera il limite, viene resettato.
*/
void ArmedEntity::incWeaponLoop() {
	weapon_loop_counter++;
	if (weapon_loop_counter > WEAPON_DISPLAY_TIME) {
		resetWeaponLoop();
	}
}

/*
	Azzera weapon_loop_counter
*/
void ArmedEntity::resetWeaponLoop() {
	weapon_loop_counter = 0;
}

/*
	Indica se terminare la visualizzazione dell'arma quando il giocatore attacca
*/
bool ArmedEntity::endWeaponDisplay() {
	if (weapon_loop_counter >= WEAPON_DISPLAY_TIME && is_attacking) {
		is_attacking = false;
		return true;
	}
	else {
		return false;
	}
}

