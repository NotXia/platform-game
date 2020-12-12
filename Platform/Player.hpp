#pragma once
#include "Entity.hpp"
#include "Pixel.hpp"
#include "Weapon.hpp"

class Player : public Entity {
	protected:
		Weapon *weapon;
		int points;
		int money;

		int weapon_loop_counter;

	public:
		Player(int health, Pixel head_left, Pixel head_right, Pixel body, Weapon *weapon, Position position);

		/*
			Prende un input un intero
			Incrementa health di quel valore (non può superare il limite)
		*/
		void heal(int hp);

		/*
			Prende un input un intero
			Incrementa points di quel valore
		*/
		void incPoints(int points);

		/*
			Prende un input un intero
			Incrementa money di quel valore
		*/
		void incMoney(int money);

		/*
			Prende un input un intero
			Decrementa money di quel valore
		*/
		void decMoney(int money);

		Weapon *getWeapon();

		void attack();

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

