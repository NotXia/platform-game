#pragma once
#include "Entity.hpp"
#include "Weapon.hpp"
#include <iostream>
class Bullet;

class ArmedEntity : public Entity {
	protected:
		Weapon weapon;
		bool is_attacking;
		int weapon_loop_counter;

	public:
		ArmedEntity(int health=0, Pixel head_left=Pixel(' ', 0, false), Pixel head_right=Pixel(' ', 0, false), Pixel body=Pixel(' ', 0, false), Position position=Position(0, 0), Weapon weapon=Weapon());

		Weapon getWeapon();

		/*
			Restituisce un oggetto Bullet identico al Bullet associato all'oggetto weapon
		*/
		Bullet attack();

		/*
			Incrementa weapon_loop_counter di 1.
			Se supera il limite, viene resettato.
		*/
		void incWeaponLoop();

		/*
			Azzera weapon_loop_counter
		*/
		void resetWeaponLoop();

		/*
			Indica se terminare la visualizzazione dell'arma quando il giocatore attacca
		*/
		bool endWeaponDisplay();

};

