#pragma once
#include "ArmedEntity.hpp"
#include "Pixel.hpp"
#include "Weapon.hpp"

class Player : public ArmedEntity {
	protected:
		int points;
		int money;

	public:
		Player(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon *weapon);

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
};

