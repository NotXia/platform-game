#pragma once
#include "Block.hpp"
#include "Weapon.hpp"
#include <iostream>

const int BONUS_TYPE_MONEY = 0;
const int BONUS_TYPE_HP = 1;
const int BONUS_TYPE_WEAPON = 2;

class Bonus : public Block {
	protected:
		int points;
		int money;
		int hp;
		Weapon *weapon;

	public:
		Bonus(Pixel body=Pixel(), Position position=Position(), int points=0, int money=1, int hp=0, Weapon *weapon=NULL);

		int getPoints();
		void setPosition(Position position);
		Weapon getWeapon();
		void setWeapon(Weapon weapon);

		/*
			Restituisce il tipo del bonus in base ai campi impostati
		*/
		int getType();

		/*
			Restituisce il contenuto del bonus in base al tipo (vita o soldi).
		*/
		int getBonus();
};